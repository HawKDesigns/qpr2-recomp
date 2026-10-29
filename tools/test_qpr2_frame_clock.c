#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "../third_party/xboxrecomp/src/kernel/xbox_frame_clock.h"

static uint64_t run(uint32_t refresh, uint64_t duration, uint64_t step)
{
    XBOX_FRAME_CLOCK clock = {0};
    uint64_t total = 0, now;
    assert(xbox_frame_clock_advance(&clock, 1000000000000ull, refresh) == 0);
    for (now = step; now < duration; now += step)
        total += xbox_frame_clock_advance(&clock, 1000000000000ull + now, refresh);
    total += xbox_frame_clock_advance(&clock, 1000000000000ull + duration, refresh);
    /* A flip/read at the same time cannot add another tick. */
    assert(xbox_frame_clock_advance(&clock, 1000000000000ull + duration, refresh) == 0);
    return total;
}

int main(void)
{
    XBOX_FRAME_CLOCK clock = {0};
    uint64_t step;
    uint32_t counter = UINT32_MAX - 10u;
    for (step = 1; step <= 1000000; step *= 10) {
        assert(run(60, 1001000, step) == 60);
        assert(run(60, 180000000, step) == 10789);
        assert(run(50, 180000000, step) == 9000);
    }
    /* Reproduce coarse 26ms wakes: previous code yielded only ~38Hz. */
    assert(run(60, 1001000, 26000) == 60);
    assert(run(60, 180000000, 26000) == 10789);
    /* Multi-second rendering stall retains the entire elapsed interval. */
    assert(xbox_frame_clock_advance(&clock, 0, 60) == 0);
    counter += (uint32_t)xbox_frame_clock_advance(&clock, 5005000, 60);
    assert(counter == 289u);
    assert(xbox_frame_clock_advance(&clock, 5005000, 60) == 0);
    /* Device/display mode changes establish a fresh fractional phase. */
    assert(xbox_frame_clock_advance(&clock, 5005000, 50) == 0);
    assert(xbox_frame_clock_advance(&clock, 5025000, 50) == 1);
    assert(xbox_frame_clock_advance(&clock, 0, 50) == 0);
    assert(xbox_frame_clock_advance(&clock, 20000, 50) == 1);
    assert(run(60, 86400000000ull, 123456789ull) == 5178821ull);
    puts("PASS: elapsed ticks, fractional phase, late wakes, flips, PAL/NTSC, counter wrap and long runs");
    return 0;
}
