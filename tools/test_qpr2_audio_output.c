#include "../third_party/xboxrecomp/src/apu/apu_core.c"
#include <assert.h>
static int test_active = 1, test_calls;
static int16_t received[256][2];
int xa2_is_active(void) { return test_active; }
int xa2_submit_samples(const int16_t *samples, int count)
{
    assert(count == 256);
    memcpy(received, samples, sizeof(received));
    ++test_calls; return 1;
}
int main(void)
{
    MCPXAPUState *d = calloc(1, sizeof(*d));
    assert(d);
    /* Emulate the eight consecutive 32-sample DSP slices. */
    for (int frame = 0; frame < 8; ++frame) {
        d->ep_frame_div = frame;
        for (int i = 0; i < 32; ++i) {
            d->monitor.mix_buf[frame * 32 + i][0] = (int16_t)(1000 + frame);
            d->monitor.mix_buf[frame * 32 + i][1] = (int16_t)(-2000 - frame);
        }
        mcpx_apu_monitor_frame(d);
        assert(test_calls == (frame == 7 ? 1 : 0));
    }
    for (int i = 0; i < 256; ++i) {
        assert(received[i][0] == 1000 + i / 32);
        assert(received[i][1] == -2000 - i / 32);
        assert(d->monitor.frame_buf[i][0] == 0 && d->monitor.frame_buf[i][1] == 0);
    }
    g_audio_muted = 1;
    d->monitor.mix_buf[0][0] = 3000;
    mcpx_apu_monitor_frame(d);
    assert(received[0][0] == 0 && test_calls == 2);
    g_audio_muted = 0;
    test_active = 0;
    g_waveout.initialized = true;
    for (int block = 0; block < 8; ++block) {
        for (int i = 0; i < 256; ++i)
            d->monitor.mix_buf[i][0] = d->monitor.mix_buf[i][1] = (int16_t)(block + 10);
        mcpx_apu_monitor_frame(d);
    }
    assert(g_waveout.frames_written == 1 && g_waveout.fill_samples == 0);
    for (int i = 0; i < 2048; ++i)
        assert(g_waveout.bufs[0][i][0] == i / 256 + 10);
    /* Real DSP mixdown -> final limiter -> copied backend output. */
    test_active=1;
    static float bins[NUM_MIXBINS][NUM_SAMPLES_PER_FRAME];
    for(int i=0;i<32;i++){bins[0][i]=4.0f;bins[1][i]=-2.0f;}
    for(int f=0;f<8;f++) {
        d->ep_frame_div=f;mcpx_apu_dsp_frame(d,bins);mcpx_apu_monitor_frame(d);
    }
    const char *setting=getenv("RECOMP_AUDIO_LIMITER");
    if(!setting || atoi(setting)!=0) {
        assert(received[0][0]>=32759 && received[0][0]<=32760);
        assert(received[0][1]==-16380);
    } else assert(received[0][0]==32767 && received[0][1]==-32768);
    free(d);
    puts("PASS: DSP samples preserved, 256-sample output, clear after copy, mute, eight distinct waveOut blocks");
    return 0;
}
