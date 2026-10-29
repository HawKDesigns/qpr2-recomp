/* Link with actual core/VP/DSP. Rename the VP/DSP layout exports while
 * compiling those two files to qpr2_test_vp_layout/qpr2_test_dsp_layout. */
#include "../third_party/xboxrecomp/src/apu/apu_state.h"
#include <assert.h>

MCPXAPUStateLayout qpr2_test_vp_layout(void);
MCPXAPUStateLayout qpr2_test_dsp_layout(void);
static int bad_module, bad_field;

static MCPXAPUStateLayout corrupt(MCPXAPUStateLayout value, int module)
{
    if (bad_module != module) return value;
    switch (bad_field) {
    case 0: value.size += 8; break;
    case 1: value.filters += 8; break;
    case 2: value.ssl += 8; break;
    case 3: value.regs += 8; break;
    case 4: value.monitor += 8; break;
    case 5: value.frame_div += 8; break;
    }
    return value;
}

MCPXAPUStateLayout mcpx_apu_vp_state_layout(void)
{ return corrupt(qpr2_test_vp_layout(), 1); }
MCPXAPUStateLayout mcpx_apu_dsp_state_layout(void)
{ return corrupt(qpr2_test_dsp_layout(), 2); }

int main(void)
{
    assert(mcpx_apu_validate_state_layout());
    for (bad_module = 1; bad_module <= 2; ++bad_module)
        for (bad_field = 0; bad_field < 6; ++bad_field)
            assert(!mcpx_apu_validate_state_layout());
    bad_module = 0;
    assert(mcpx_apu_validate_state_layout());
    puts("PASS: actual core/VP/DSP agree; stale size and each shared offset are rejected");
    return 0;
}
