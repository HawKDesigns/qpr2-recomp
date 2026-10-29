#include "../third_party/xboxrecomp/src/apu/apu_vp.c"
#include <assert.h>
static uint8_t test_ram[0x10000];
struct McpxApuDebug g_dbg, g_dbg_cache;
int g_dbg_voice_monitor = -1;
uint64_t g_dbg_muted_voices[4];
uint8_t *mcpx_apu_phys(uint64_t address)
{ assert(address < sizeof(test_ram)); return test_ram + address; }

static void test_voice(float ratio, int source_frames)
{
    MCPXAPUState *d = calloc(1, sizeof(*d));
    float block[32][2];
    int total = 0, calls = 0;
    assert(d); memset(test_ram, 0, sizeof(test_ram));
    d->regs[NV_PAPU_VPVADDR] = 0x1000;
    d->regs[NV_PAPU_VPSGEADDR] = 0x8000;
    d->regs[NV_PAPU_FENADDR] = 0x9000;
    stl_le_phys(NULL, 0x8000, 0x4000);
    for (int i = 0; i < source_frames; ++i) stw_le_phys(NULL, 0x4000 + i * 2, i * 100);
    voice_set_mask(d, 0, NV_PAVS_VOICE_CFG_FMT, NV_PAVS_VOICE_CFG_FMT_SAMPLE_SIZE, 1);
    voice_set_mask(d, 0, NV_PAVS_VOICE_CFG_FMT, NV_PAVS_VOICE_CFG_FMT_CONTAINER_SIZE, 1);
    voice_set_mask(d, 0, NV_PAVS_VOICE_PAR_STATE, NV_PAVS_VOICE_PAR_STATE_ACTIVE_VOICE, 1);
    voice_set_mask(d, 0, NV_PAVS_VOICE_PAR_NEXT, NV_PAVS_VOICE_PAR_NEXT_EBO, source_frames - 1);
    do {
        int count = voice_resample(d, 0, block, 32, ratio);
        assert(count >= 0 && count <= 32 && ++calls < 100);
        for (int i = 0; i < count; ++i) {
            double position = (total + i) / (double)ratio;
            if (position > source_frames - 1) position = source_frames - 1;
            float expected = (float)(position * 100.0 / 32768.0);
            assert(fabsf(block[i][0] - expected) < 0.00002f);
            assert(block[i][1] == block[i][0]);
        }
        total += count;
        if (!count) break;
        if (total < (int)ceil(source_frames * ratio))
            assert(voice_get_mask(d, 0, NV_PAVS_VOICE_PAR_STATE,
                                 NV_PAVS_VOICE_PAR_STATE_ACTIVE_VOICE));
    } while (1);
    assert(total == (int)ceil(source_frames * ratio));
    assert(!voice_get_mask(d, 0, NV_PAVS_VOICE_PAR_STATE, NV_PAVS_VOICE_PAR_STATE_ACTIVE_VOICE));
    free(d);
}
typedef struct ArraySource { int at, count, temporary_end; } ArraySource;
static void test_stream_completion(void)
{
    MCPXAPUState *d = calloc(1, sizeof(*d));
    const float ratio = 48000.0f / 44100.0f;
    const int lengths[2] = {65, 67};
    int total = 0, completed[2] = {0, 0};
    assert(d); memset(test_ram, 0, sizeof(test_ram));
    d->regs[NV_PAPU_VPVADDR] = 0x1000;
    d->regs[NV_PAPU_FENADDR] = 0x9000;
    d->regs[NV_PAPU_VPSSLADDR] = 0xa000;
    voice_set_mask(d, 0, NV_PAVS_VOICE_CFG_FMT, NV_PAVS_VOICE_CFG_FMT_SAMPLE_SIZE, 1);
    voice_set_mask(d, 0, NV_PAVS_VOICE_CFG_FMT, NV_PAVS_VOICE_CFG_FMT_CONTAINER_SIZE, 1);
    voice_set_mask(d, 0, NV_PAVS_VOICE_CFG_FMT, NV_PAVS_VOICE_CFG_FMT_STEREO, 1);
    voice_set_mask(d, 0, NV_PAVS_VOICE_CFG_FMT, NV_PAVS_VOICE_CFG_FMT_SAMPLES_PER_BLOCK, 1);
    voice_set_mask(d, 0, NV_PAVS_VOICE_CFG_FMT, NV_PAVS_VOICE_CFG_FMT_DATA_TYPE, 1);
    voice_set_mask(d, 0, NV_PAVS_VOICE_CFG_FMT, NV_PAVS_VOICE_CFG_FMT_PERSIST, 1);
    voice_set_mask(d, 0, NV_PAVS_VOICE_PAR_STATE, NV_PAVS_VOICE_PAR_STATE_ACTIVE_VOICE, 1);
    for (int segment = 0; segment < 2; ++segment) {
        uint32_t base = segment ? 0x5000 : 0x4000;
        d->vp.ssl[0].base[segment] = segment;
        d->vp.ssl[0].count[segment] = 1;
        stl_le_phys(NULL, 0xa000 + segment * 8, base);
        stl_le_phys(NULL, 0xa004 + segment * 8,
                   lengths[segment] | (1 << 16) | (1 << 18) | (1 << 23));
        int start = segment ? lengths[0] : 0;
        for (int i = 0; i < lengths[segment]; ++i) {
            stw_le_phys(NULL, base + i * 4, (start + i) * 100);
            stw_le_phys(NULL, base + i * 4 + 2, -(start + i) * 100);
        }
        int notifier = MCPX_HW_NOTIFIER_BASE_OFFSET + MCPX_HW_NOTIFIER_SSLA_DONE + segment;
        test_ram[0x9000 + notifier * 16 + 15] = 0x80;
    }
    for (int slice = 0; slice < 20; ++slice) {
        float output[32][2];
        int n = voice_resample(d, 0, output, 32, ratio);
        assert(n >= 0 && n <= 32);
        for (int i = 0; i < n; ++i) {
            double position = (total + i) / (double)ratio;
            if (position > 131) position = 131;
            float expected = (float)(position * 100 / 32768.0);
            assert(fabsf(output[i][0] - expected) < 0.00002f);
            assert(fabsf(output[i][1] + expected) < 0.00002f);
        }
        total += n;
        for (int segment = 0; segment < 2; ++segment) {
            int notifier = MCPX_HW_NOTIFIER_BASE_OFFSET + MCPX_HW_NOTIFIER_SSLA_DONE + segment;
            if (test_ram[0x9000 + notifier * 16 + 15] == NV1BA0_NOTIFICATION_STATUS_DONE_SUCCESS) {
                completed[segment] = 1;
                /* DirectSound retires completed SSLs after their notifier. */
                d->vp.ssl[0].count[segment] = 0;
            }
        }
        if (!n) break;
    }
    assert(total == (int)ceil(132 * (double)ratio));
    assert(completed[0] && completed[1]);
    free(d);
}
static int read_array(void *context, float samples[][2], int requested)
{
    ArraySource *source = context;
    int n = 0;
    if (source->temporary_end) return 0;
    while (n < requested && source->at < source->count) {
        samples[n][0] = source->at;
        samples[n++][1] = -source->at++;
    }
    return n;
}
static void test_final_mix(int pitched, int legacy)
{
    MCPXAPUState *d = calloc(1, sizeof(*d));
    float mixbins[NUM_MIXBINS][NUM_SAMPLES_PER_FRAME] = {{0}};
    float monitor[NUM_SAMPLES_PER_FRAME][2] = {{0}};
    assert(d); memset(test_ram, 0, sizeof(test_ram));
    d->regs[NV_PAPU_VPVADDR] = 0x1000; d->regs[NV_PAPU_VPSGEADDR] = 0x8000;
    d->regs[NV_PAPU_FENADDR] = 0x9000; d->monitor.point = MCPX_APU_DEBUG_MON_VP;
    stl_le_phys(NULL, 0x8000, 0x4000);
    for (int i = 0; i < 7; ++i) stw_le_phys(NULL, 0x4000 + i * 2, (i + 1) * 1000);
    voice_set_mask(d, 0, NV_PAVS_VOICE_CFG_FMT, NV_PAVS_VOICE_CFG_FMT_SAMPLE_SIZE, 1);
    voice_set_mask(d, 0, NV_PAVS_VOICE_CFG_FMT, NV_PAVS_VOICE_CFG_FMT_CONTAINER_SIZE, 1);
    voice_set_mask(d, 0, NV_PAVS_VOICE_PAR_STATE, NV_PAVS_VOICE_PAR_STATE_ACTIVE_VOICE, 1);
    voice_set_mask(d, 0, NV_PAVS_VOICE_PAR_NEXT, NV_PAVS_VOICE_PAR_NEXT_EBO, 6);
    voice_set_mask(d, 0, NV_PAVS_VOICE_TAR_PITCH_LINK,
                   NV_PAVS_VOICE_TAR_PITCH_LINK_PITCH, pitched ? (uint16_t)-4096 : 0);
    voice_process(d, mixbins, monitor, 0, 0);
    for (int i = 0; i < 32; ++i) {
        float expected = 0;
        int duration = pitched ? 14 : 7;
        if (!legacy && i < duration) {
            float position = pitched ? i * 0.5f : i;
            if (position > 6) position = 6;
            expected = (position + 1) * 1000 / 32768.0f;
        }
        assert(fabsf(monitor[i][0] - expected) < 0.00002f);
        assert(monitor[i][0] == monitor[i][1]);
        assert(fabsf(mixbins[0][i] - expected * 8) < 0.0001f);
    }
    free(d);
}
static void test_source_capture(void)
{
    MCPXAPUState *d = calloc(1, sizeof(*d));
    assert(d); memset(test_ram, 0, sizeof(test_ram));
    d->regs[NV_PAPU_VPVADDR] = 0x1000;
    d->regs[NV_PAPU_VPSGEADDR] = 0x8000;
    stl_le_phys(NULL, 0x8000, 0x4000);
    stw_le_phys(NULL, 0x4000, 1024);
    voice_set_mask(d, 0, NV_PAVS_VOICE_CFG_FMT, NV_PAVS_VOICE_CFG_FMT_CONTAINER_SIZE, 2);
    voice_set_mask(d, 0, NV_PAVS_VOICE_PAR_NEXT, NV_PAVS_VOICE_PAR_NEXT_EBO, 63);
    voice_set_mask(d, 0, NV_PAVS_VOICE_PAR_OFFSET, NV_PAVS_VOICE_PAR_OFFSET_CBO, 19);
    MCPXAPUState *before = malloc(sizeof(*d)); assert(before); memcpy(before, d, sizeof(*d));
    uint8_t before_ram[sizeof(test_ram)]; memcpy(before_ram, test_ram, sizeof(test_ram));
    setenv("RECOMP_VOICE_DUMP", "/tmp/qpr2-v31-test-voice-", 1);
    voice_dump_adpcm(d, 0, 1);
    unsetenv("RECOMP_VOICE_DUMP");
    assert(!memcmp(d, before, sizeof(*d)) && !memcmp(test_ram, before_ram, sizeof(test_ram)));
    FILE *f = fopen("/tmp/qpr2-v31-test-voice-00-v0-source-48000Hz.wav", "rb");
    uint32_t header[11]; int16_t first;
    assert(f && fread(header, sizeof(header), 1, f)==1);
    assert(header[0]==0x46464952 && header[6]==48000 && header[10]==128);
    assert(fread(&first, sizeof(first), 1, f)==1 && first==1024);
    fclose(f);
    f = fopen("/tmp/qpr2-v31-test-voice-00-v0-source-48000Hz.wav.adpcm", "rb");
    uint8_t raw[36];
    assert(f && fread(raw, 1, sizeof raw, f) == sizeof raw && fgetc(f) == EOF);
    assert(!memcmp(raw, test_ram + 0x4000, sizeof raw));
    fclose(f); free(before); free(d);
}
int main(int argc, char **argv)
{
    (void)argv;
    if (argc > 1) {
        test_final_mix(0, 1);
        puts("REPRODUCED: V28 drops the entire final short-voice block before mixing");
        return 0;
    }
    APUVoiceRateConverter converter = {0};
    ArraySource source = {0, 65, 0};
    float out[160][2];
    test_voice(1, 33); /* Inclusive EBO: no dropped final source frame. */
    test_voice(2, 33); /* Upsampled tail must outlive input prefetch. */
    test_voice(0.5f, 65);
    test_voice(48000.0f / 44100.0f, 65);
    test_final_mix(0, 0);
    test_final_mix(1, 0);
    test_stream_completion();
    test_source_capture();
    int n = apu_rate_convert(&converter, out, 32, 2, read_array, &source);
    assert(n == 32);
    n += apu_rate_convert(&converter, out + n, 128, 2, read_array, &source);
    assert(n == 130);
    for (int i = 0; i < n; ++i) {
        float expected = i < 128 ? i * 0.5f : 64;
        assert(out[i][0] == expected && out[i][1] == -expected);
    }
    assert(!apu_rate_convert(&converter, out, 32, NAN, read_array, &source));
    assert(!apu_rate_convert(&converter, out, 32, 0, read_array, &source));
    memset(&converter, 0, sizeof(converter)); source.at = 0; source.count = 1;
    assert(apu_rate_convert(&converter, out, 32, 1, read_array, &source) == 1);
    source.count = 3;
    assert(apu_rate_convert(&converter, out, 32, 1, read_array, &source) == 2);
    assert(out[0][0] == 1 && out[1][0] == 2);
    puts("PASS: actual VP native/pitched PCM, stereo SSL packet completion, rate/duration, inclusive end, final-block mixing, tail drain and underrun resume");
}
