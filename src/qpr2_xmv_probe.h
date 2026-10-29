#ifndef QPR2_XMV_PROBE_H
#define QPR2_XMV_PROBE_H
#include <stdint.h>
/* Copies of operands at the two timing compares; no guest state is changed. */
typedef struct QPR2_XMV_TIMING {
    uint32_t elapsed_valid, elapsed_ticks;
    uint32_t ready_valid, ready_due_ticks, ready_ms, ready_sync_ms, ready_interval;
    uint32_t decode_valid, decode_due_ticks, decode_ms, decode_sync_ms, decode_interval;
} QPR2_XMV_TIMING;
void qpr2_xmv_probe(uint32_t object, uint32_t result, uint32_t hr,
                    uint32_t stage, const QPR2_XMV_TIMING *timing);
#endif
