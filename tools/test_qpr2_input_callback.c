#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "recomp/gen/recomp_types.h"
#include "usb/usb_gamepad.h"
#include "input/xinput_xbox.h"
static XBOX_INPUT_STATE host_state;
DWORD xbox_InputGetState(DWORD port, XBOX_INPUT_STATE *state)
{ assert(port == 0); *state = host_state; return 0; }
int xbox_FramebufferKeyDown(int vk) { (void)vk; return 0; }
ptrdiff_t g_xbox_mem_offset;
uint32_t g_xbox_code_lo, g_xbox_code_hi;
RECOMP_TLS uint32_t g_eax, g_ecx, g_edx, g_esp, g_ebx, g_esi, g_edi, g_ebp, g_seh_ebp;
RECOMP_TLS int g_df;
RECOMP_TLS volatile uint32_t g_recomp_pc;
static unsigned wakes;
void sub_000CDCC9(void) { ++wakes; g_esp += 4; }
void qpr2_recovered_001A7D43(void);

static void invoke(uint32_t object)
{
    g_ecx = object;
    g_esp = 0x180000;
    g_ebx = 0x12345678;
    g_esi = 0x23456789;
    g_edi = 0x3456789a;
    qpr2_recovered_001A7D43();
    assert(g_esp == 0x180004);
    assert(g_ebx == 0x12345678 && g_esi == 0x23456789 && g_edi == 0x3456789a);
}
int main(void)
{
    void *memory = calloc(1, 0x200000);
    uint32_t o = 0x100000;
    assert(memory); g_xbox_mem_offset = (ptrdiff_t)memory;
    MEM32(o + 0x66) = 20;
    MEM8(o + 0x34) = 0x10; /* USB report payload: Start */
    invoke(o);
    assert(MEM8(o + 0x14) == 0x10 && wakes == 1);
    MEM8(o + 0x34) = 0;
    MEM8(o + 0x36) = 255; /* A pressure */
    MEM8(o + 0x37) = 31; /* Below retail analog threshold */
    MEM16(o + 0x3e) = 12345; /* Left stick X */
    invoke(o);
    assert(MEM8(o + 0x16) == 255 && MEM8(o + 0x17) == 0);
    assert(MEM16(o + 0x1e) == 12345 && wakes == 2);
    memset((void *)XBOX_PTR(o + 0x34), 0, 18);
    invoke(o); assert(wakes == 2);
    MEM32(o + 0x66) = 1;
    MEM8(o + 0x14) = 0x55;
    invoke(o); assert(MEM8(o + 0x14) == 0x55 && wakes == 2);
    {
        uint8_t report[20];
        host_state.dwPacketNumber = 42;
        host_state.Gamepad.wButtons = XBOX_GAMEPAD_START;
        host_state.Gamepad.bAnalogButtons[XBOX_BUTTON_A] = 255;
        host_state.Gamepad.sThumbLX = 16000;
        assert(usb_gamepad_report(0, report, sizeof(report)) == 20);
        assert(report[0] == 0 && report[1] == 20);
        memcpy((void *)XBOX_PTR(o + 0x34), report + 2, 18);
        MEM32(o + 0x66) = 20;
        invoke(o);
        assert(MEM8(o + 0x14) == 0x10 && MEM8(o + 0x16) == 255);
        assert(MEM16(o + 0x1e) == 16000 && wakes == 3);
    }
    free(memory);
    puts("PASS: Start/A report publication, analog threshold, stick copy, neutral/short reports, saved registers and guest stack");
    return 0;
}
