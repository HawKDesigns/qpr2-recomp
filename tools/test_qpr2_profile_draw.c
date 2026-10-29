#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "recomp/gen/recomp_types.h"
ptrdiff_t g_xbox_mem_offset;
uint32_t g_xbox_code_lo = 0x10000, g_xbox_code_hi = 0x1C0000;
RECOMP_TLS uint32_t g_eax, g_ecx, g_edx, g_esp, g_ebx, g_esi, g_edi, g_ebp, g_seh_ebp;
RECOMP_TLS volatile uint32_t g_recomp_pc;
RECOMP_TLS int g_df, g_fp_top, g_fp_cmp;
RECOMP_TLS uint16_t g_fp_control_word, g_fp_cc;
RECOMP_TLS double g_fp_stack[8];
volatile uint32_t g_icall_trace[16], g_icall_trace_idx;
volatile uint64_t g_icall_count;
void sub_00038120(void);
void sub_0010F440(void);
void sub_0010FEE0(void);
static unsigned draws, binds, begins, ends;
static void pre_draw(void) { g_eax = 1; g_esp += 8; }
static void post_draw(void) { g_esp += 8; }
recomp_func_t recomp_lookup_manual(uint32_t va) { (void)va; return NULL; }
recomp_func_t recomp_lookup_kernel(uint32_t va) { (void)va; return NULL; }
recomp_func_t recomp_lookup(uint32_t va)
{
    if (va == 0x38120) return sub_00038120;
    if (va == 0x150100) return pre_draw;
    if (va == 0x150200) return post_draw;
    assert(!"Unexpected drawing target"); return NULL;
}
void recomp_icall_not_code_log(uint32_t va) { (void)va; assert(!"Bad code pointer"); }
void recomp_icall_fail_log(uint32_t va) { (void)va; assert(!"Unresolved target"); }
/* Only host/GPU submission dependencies are mocked. The retail alias, list
 * traversal, base drawing, rectangle transforms and recovered body are real. */
void sub_0001D270(void) { ++begins; g_esp += 4; }
void sub_0001D370(void) { ++ends; g_esp += 4; }
void sub_000E6A00(void)
{ assert(MEM32(g_esp + 8) == 0x130000); ++binds; g_esp += 4; }
void sub_00112200(void)
{
    assert(MEM32(g_esp + 4) == 0xBF800000 && MEM32(g_esp + 8) == 0x3F800000);
    assert(MEM32(g_esp + 12) == 0x40000000 && MEM32(g_esp + 16) == 0xC0000000);
    ++draws; g_esp += 44;
}
static void reset_stack(uint32_t object)
{
    g_esp = 0x180000; g_ecx = object;
    g_esi = 0x11111111; g_edi = 0x120000; g_ebx = 0x33333333;
    MEM32(g_esp) = 0x12345; MEM32(g_esp + 4) = 0x120000;
}
static void assert_return(void)
{
    assert(g_esp == 0x180008 && g_esi == 0x11111111);
    assert(g_edi == 0x120000 && g_ebx == 0x33333333);
}
int main(int argc, char **argv)
{
    const uint32_t parent = 0x100000, child = 0x110000;
    void *memory = calloc(1, 0x200000);
    (void)argv; assert(memory); g_xbox_mem_offset = (ptrdiff_t)memory;
    MEM32(parent) = 0x140000; MEM32(parent + 0x48) = 2; MEM32(parent + 0x50) = 1;
    MEM32(parent + 0x70 + 8) = child;
    MEM32(child) = 0x141000; MEM32(child + 0x48) = 2;
    MEM32(0x141018) = 0x38120;
    MEM32(0x140060) = MEM32(0x141060) = 0x150100;
    MEM32(0x140064) = MEM32(0x141064) = 0x150200;
    MEM32(0x1D7E90) = 1; /* Enables the exact tail-jump branch in the crash. */
    reset_stack(parent + 0x70); sub_0010F440();
    if (argc > 1) {
        assert(g_esp == 0x180004 && g_esi == 0x120000);
        puts("REPRODUCED: V27 stub leaves stack 4 bytes low and replaces saved ESI with rectangle pointer");
        free(memory); return 0;
    }
    assert_return();
    MEM32(0x1D7E90) = 0;
    reset_stack(parent + 0x70); sub_0010F440(); assert_return();
    MEM32(0x1D7E90) = 1; MEM32(child + 0xA4) = 1;
    MEM32(child + 0x94) = 0x130000;
    for (unsigned o = parent; o <= child; o += child - parent) {
        MEMF(o + 0xC) = MEMF(o + 0x10) = -1;
        MEMF(o + 0x14) = MEMF(o + 0x18) = 2;
        MEMF(o + 0x24) = 1;
    }
    MEMF(0x120000) = MEMF(0x120004) = -1;
    MEMF(0x120008) = MEMF(0x12000C) = 2; MEMF(0x120010) = 1;
    MEMF(0x1C91D0) = 1; MEMF(0x1C91CC) = 0.5f;
    MEMF(0x1AF32C) = -1;
    MEMF(0x1AE830) = MEMF(0x1AE82C) = 255;
    reset_stack(parent); sub_0010FEE0(); assert_return();
    assert(draws == 1 && binds == 1 && begins == 1 && ends == 1);
    assert(MEM32(0x1DC428) == 3 && MEM32(0x1DC368) == 3);
    free(memory);
    puts("PASS: actual alias/list/base-draw chain, inactive and active paths, drawing submission, saved registers and stack");
}
