#include <assert.h>
#include <stdio.h>
#include "../third_party/xboxrecomp/src/d3d/d3d8_swizzle.h"
int main(void)
{
    uint8_t block[16] = {0};
    uint32_t pixel;
    /* DXT1 four-color mode: red and blue, with both interpolated selectors. */
    block[0] = 0x00; block[1] = 0xF8; block[2] = 0x1F; block[3] = 0x00;
    block[4] = 2 | (3 << 2);
    assert(d3d8_dxt_decode_texel(block, 0x0C, 0, 0, 4, &pixel) && pixel == 0xFFAA0055);
    assert(d3d8_dxt_decode_texel(block, 0x0C, 1, 0, 4, &pixel) && pixel == 0xFF5500AA);
    /* DXT1 three-color and transparent mode. */
    block[0] = 0x1F; block[1] = 0; block[2] = 0; block[3] = 0xF8;
    assert(d3d8_dxt_decode_texel(block, 0x0C, 0, 0, 4, &pixel) && pixel == 0xFF7F007F);
    assert(d3d8_dxt_decode_texel(block, 0x0C, 1, 0, 4, &pixel) && pixel == 0);
    /* DXT3 carries alpha even with ascending color endpoints. */
    memset(block, 0, sizeof(block));
    block[0] = 0xF7; block[8] = 0x1F; block[11] = 0xF8; block[12] = 2 | (3 << 2);
    assert(d3d8_dxt_decode_texel(block, 0x0E, 0, 0, 4, &pixel) && pixel == 0x775500AA);
    assert(d3d8_dxt_decode_texel(block, 0x0E, 1, 0, 4, &pixel) && pixel == 0xFFAA0055);
    /* DXT5 explicit transparent and opaque selectors. */
    block[0] = 0; block[1] = 255; block[2] = 6 | (7 << 3);
    assert(d3d8_dxt_decode_texel(block, 0x0F, 0, 0, 4, &pixel) && pixel == 0x005500AA);
    assert(d3d8_dxt_decode_texel(block, 0x0F, 1, 0, 4, &pixel) && pixel == 0xFFAA0055);
    puts("PASS: DXT1/3/5 interpolated colors and transparency");
}
