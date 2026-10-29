#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "recomp/gen/recomp_types.h"
ptrdiff_t g_xbox_mem_offset;
uint32_t g_xbox_code_lo, g_xbox_code_hi;
RECOMP_TLS uint32_t g_eax, g_ecx, g_edx, g_esp, g_ebx, g_esi, g_edi, g_ebp, g_seh_ebp;
RECOMP_TLS volatile uint32_t g_recomp_pc;
RECOMP_TLS int g_df, g_fp_top, g_fp_cmp;
RECOMP_TLS uint16_t g_fp_control_word, g_fp_cc;
RECOMP_TLS double g_fp_stack[8];
static unsigned setup, properties, apply;
void sub_000FA520(void)
{
    assert(MEM32(g_esp + 8) == 2); /* Existing animation ID. */
    assert(MEM32(g_esp + 12) == 2); /* Input flag bit. */
    assert(MEM32(g_esp + 16) == 77);
    assert(MEM32(g_esp + 28) == 99);
    MEM8(MEM32(g_esp + 4) + 6) = 8;
    ++setup; g_esp += 4;
}
void sub_000FA800(void)
{ assert(MEM32(g_esp + 4) == 5); ++properties; g_esp += 4; }
void sub_000EA770(void)
{ assert(g_ecx == 0x120000); assert(MEM8(MEM32(g_esp + 4) + 6) == 8); ++apply; g_esp += 8; }
void sub_000CE100(void) { assert(!"Unexpected random-selection path"); }
void sub_000FA690(void) { assert(!"Unexpected new-animation path"); }
void qpr2_recovered_00038870(void);
int main(void)
{
    void *memory = calloc(1, 0x200000);
    assert(memory); g_xbox_mem_offset = (ptrdiff_t)memory;
    g_ecx = 0x100000; g_esp = 0x180000;
    g_esi = 123; g_edi = 456; g_ebx = 789;
    MEM32(g_esp + 4) = 0x110000; MEM8(0x110006) = 10;
    MEM32(0x1000D4) = 0x120000; MEM32(0x1000D8) = 77; MEM32(0x1000DC) = 2;
    MEM32(0x1C3E54) = 99; MEM32(0x1D3ED0) = 0x130000;
    MEM32(0x130004) = 0x140000; MEM16(0x140000 + 2 * 0x2C + 0x1A) = 5;
    qpr2_recovered_00038870();
    assert(setup == 1 && properties == 1 && apply == 1);
    assert(g_esp == 0x180008 && g_esi == 123 && g_edi == 456 && g_ebx == 789);
    free(memory);
    puts("PASS: recovered existing-animation setup, property selection, apply, saved registers and stack");
}
