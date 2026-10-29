#include "../third_party/xboxrecomp/src/kernel/nv2a_pb_exec.c"
#include <assert.h>

static uint8_t ram[0x100000];
uint32_t g_xbox_image_lo, g_xbox_image_hi;
ptrdiff_t xbox_GetMemoryOffset(void) { return (ptrdiff_t)ram; }
uint32_t xbox_ContiguousAllocatedBytes(void) { return 0; }
void xbox_FramebufferWindowSet(uint32_t base, uint32_t pitch) { (void)base; (void)pitch; }
void xbox_FramebufferWindowStart(void) {}

/* The exact retail F24 clear packing at 00198770..0019878D. */
static uint32_t retail_f24_pack(float value)
{
    double wide = value;
    uint64_t bits;
    memcpy(&bits, &wide, 8);
    return (((uint32_t)(bits >> 32) + 0xF8000000u) & ~15u) << 4;
}

static void test_depth(void)
{
    s_gpu.format = 0x128; s_gpu.control0 = 0; s_gpu.depth_func = 0x203;
    s_gpu.zstencil_clear = 0xFFFFFFAB;
    assert(zclear_value() == 16777215);
    s_gpu.format = 0x118; s_gpu.zstencil_clear = 0xABCDFFFF;
    assert(zclear_value() == 65535);
    s_gpu.format = 0x128; s_gpu.control0 = 1u << 12;
    s_gpu.zstencil_clear = retail_f24_pack(1e30f) | 0xAB;
    float clear = zclear_value();
    assert(clear > 0.99999e30f && clear <= 1e30f);
    /* The logged rider vertex failed against the old integer clear. */
    assert(!depth_pass(6.93433277e29f, (float)(s_gpu.zstencil_clear >> 8)));
    assert(depth_pass(6.93433277e29f, clear));
    s_gpu.zstencil_clear = 0; assert(zclear_value() == 0);
    s_gpu.format = 0x118; s_gpu.zstencil_clear = 0xFFFF;
    assert(zclear_value() == 511.9375f);
}

static void test_texture_sizes(void)
{
    tex_stage_method(NV097_SET_TEXTURE_IMAGE_RECT, (8u << 16) | 256);
    tex_stage_method(NV097_SET_TEXTURE_FORMAT, (9u << 20) | (9u << 24) | (0x0Fu << 8));
    assert(s_gpu.texs[0].width == 512 && s_gpu.texs[0].height == 512);
    tex_stage_method(NV097_SET_TEXTURE_IMAGE_RECT, (8u << 16) | 256);
    assert(s_gpu.texs[0].width == 512 && s_gpu.texs[0].height == 512);
    tex_stage_method(NV097_SET_TEXTURE_FORMAT, 0x24u << 8);
    assert(s_gpu.texs[0].width == 8 && s_gpu.texs[0].height == 256);
    tex_stage_method(NV097_SET_TEXTURE_IMAGE_RECT, (1024u << 16) | 512);
    assert(s_gpu.texs[0].width == 1024 && s_gpu.texs[0].height == 512);
}

static void test_surface(int swizzled)
{
    const unsigned w = 8, h = 4;
    uint32_t pixel;
    memset(ram, 0, sizeof(ram));
    memset(&s_gpu, 0, sizeof(s_gpu));
    s_gpu.format = (3u << 16) | (2u << 24) | (swizzled ? 0x228 : 0x128);
    s_gpu.color_offset = 0x1000; s_gpu.pitch = w * 4;
    s_gpu.clip_w = w; s_gpu.clip_h = h; s_gpu.color_mask = 0x01010101;
    s_gpu.clear_color = 0xFF123456;
    clear_surface(NV097_CLEAR_COLOR_MASK);
    s_surface = ram + 0x1000;
    Texture texture = {0};
    texture.offset = 0x1000; texture.width = w; texture.height = h;
    texture.color = swizzled ? 6 : 0x12; texture.pitch = w * 4;
    texture.valid = 1;
    for (unsigned y = 0; y < h; ++y)
        for (unsigned x = 0; x < w; ++x) {
            assert(sample_tex(&texture, 0, x, y, &pixel));
            assert(pixel == s_gpu.clear_color);
            put_pixel(ram, 4, x, y, 0xFF000000 | (y << 12) | x);
        }
    for (unsigned y = 0; y < h; ++y)
        for (unsigned x = 0; x < w; ++x) {
            assert(sample_tex(&texture, 0, x, y, &pixel));
            assert(pixel == (0xFF000000 | (y << 12) | x));
        }
    /* Masked writes must read the same Morton-addressed destination. */
    s_gpu.color_mask = 0x00010000;
    put_pixel(ram, 4, 5, 2, 0xFFAABBCC);
    assert(sample_tex(&texture, 0, 5, 2, &pixel));
    assert(pixel == 0xFFAA2005);
}

int main(void)
{
    test_depth(); test_texture_sizes(); test_surface(0); test_surface(1);
    puts("PASS: actual depth clear/rider test, compressed versus linear dimensions, surface clear/draw/sample roundtrip and masked writes");
    return 0;
}
