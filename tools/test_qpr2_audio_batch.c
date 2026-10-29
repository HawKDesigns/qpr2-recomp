#include "../third_party/xboxrecomp/src/apu/apu_output_batch.h"
#include <assert.h>
#include <stdio.h>
static int blocked, emitted;
static int16_t output[12][1024][2];
static int emit(void *context, const int16_t *samples, int count)
{
    (void)context;
    assert(count == 1024);
    if (blocked) return 0;
    memcpy(output[emitted++], samples, sizeof(output[0]));
    return 1;
}
int main(void)
{
    APUOutputBatch batch = {0};
    int16_t input[256][2];
    /* Distinct blocks, both channels, and retained data after queue-full. */
    for (int b = 0; b < 16; ++b) {
        for (int i = 0; i < 256; ++i) {
            input[i][0] = (int16_t)(b * 256 + i);
            input[i][1] = (int16_t)(-b * 256 - i);
        }
        blocked = b == 7;
        assert(apu_output_batch_push(&batch, &input[0][0], 256, emit, NULL));
        memset(input, 0x7f, sizeof(input)); /* Producer may reuse its storage. */
        if (b == 7) {
            assert(batch.count == 1024 && emitted == 1);
            assert(!apu_output_batch_push(&batch, &input[0][0], 256, emit, NULL));
            assert(batch.samples[1023][0] == 2047);
        }
    }
    assert(emitted == 4 && batch.count == 0);
    for (int b = 0; b < 4; ++b) for (int i = 0; i < 1024; ++i) {
        assert(output[b][i][0] == b * 1024 + i);
        assert(output[b][i][1] == -b * 1024 - i);
    }
    assert(!apu_output_batch_push(&batch, NULL, 256, emit, NULL));
    assert(!apu_output_batch_push(&batch, &input[0][0], 1025, emit, NULL));
    assert(!apu_output_batch_push(&batch, &input[0][0], 0, emit, NULL));
    puts("PASS: ordered stereo batching, producer reuse, queue-full retry, no pending overwrite");
}
