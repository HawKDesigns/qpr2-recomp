#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "recomp/gen/recomp_types.h"
ptrdiff_t g_xbox_mem_offset;
uint32_t g_xbox_code_lo = 0x10000, g_xbox_code_hi = 0x1C0000;
RECOMP_TLS uint32_t g_eax, g_ecx, g_edx, g_esp, g_ebx, g_esi, g_edi, g_ebp, g_seh_ebp;
RECOMP_TLS volatile uint32_t g_recomp_pc;
RECOMP_TLS int g_df;
volatile uint32_t g_icall_trace[16], g_icall_trace_idx;
volatile uint64_t g_icall_count;
static int match = 1, duplicate, saved, hidden, finished, errors, cancelled;
static void init_profile(void) { assert(g_ecx == 0x130000); g_esp += 4; }
static void finish_profile(void) { ++finished; g_esp += 4; }
recomp_func_t recomp_lookup_manual(uint32_t va) { (void)va; return NULL; }
recomp_func_t recomp_lookup_kernel(uint32_t va) { (void)va; return NULL; }
recomp_func_t recomp_lookup(uint32_t va)
{ if (va == 0x150100) return init_profile; if (va == 0x150200) return finish_profile; assert(0); return NULL; }
void recomp_icall_not_code_log(uint32_t va) { (void)va; assert(0); }
void recomp_icall_fail_log(uint32_t va) { (void)va; assert(0); }
void sub_000E12E0(void) { assert(MEM32(g_esp + 4) == 0x1B62EC); g_eax = !match; g_esp += 4; }
void sub_0005A050(void) { g_eax = 0x160000; g_esp += 4; }
void sub_00012E40(void) { assert(MEM32(g_esp + 4) == 0x160000); g_eax = duplicate; g_esp += 8; }
void sub_000BAB20(void) { assert(MEM32(g_esp + 4) == 42); g_eax = 0x130000; g_esp += 8; }
void sub_000144B0(void) { assert(g_ecx == 0x130000); g_esp += 4; }
void sub_000E18A0(void) { assert(g_ecx == 0x130008 && MEM32(g_esp + 4) == 0x160000); g_esp += 8; }
void sub_00012C00(void) { assert(MEM32(g_esp + 4) == 0x130000); ++saved; g_esp += 8; }
void sub_00013060(void) { assert(MEM32(g_esp + 4) == 0x130000); g_esp += 8; }
void sub_000EBD00(void) { assert(MEM32(g_esp + 4) == 0x1C0884); ++hidden; g_esp += 12; }
void sub_0005A070(void) { ++errors; g_esp += 8; }
void sub_0012B267(void) { g_eax = 0x170000; g_esp += 4; }
void sub_000BAAC0(void) { g_eax = g_ecx; g_esp += 12; }
void sub_000BCA10(void) { ++cancelled; g_esp += 8; }
void qpr2_recovered_000BAC20(void);
static void invoke(unsigned kind, unsigned reason, unsigned expected)
{
    g_ecx = 0x110000; g_esp = 0x180000; g_esi = 123; g_edi = 456; g_ebx = 789;
    MEM32(g_esp + 4) = 0x120000;
    MEM32(0x120000) = 99; MEM32(0x120004) = kind; MEM32(0x120008) = reason;
    qpr2_recovered_000BAC20();
    assert(g_eax == expected && g_esp == 0x180008);
    assert(g_esi == 123 && g_edi == 456 && g_ebx == 789);
}
int main(void)
{
    void *memory = calloc(1, 0x200000); assert(memory); g_xbox_mem_offset = (ptrdiff_t)memory;
    MEM32(0x110000) = 0x151000; MEM32(0x15100C) = 0x150200;
    MEM32(0x110018) = 42; MEM32(0x11001C) = 77;
    MEM32(0x130000) = 0x152000; MEM32(0x152004) = 0x150100; MEM32(0x130020) = 0x140000;
    invoke(0, 0, 1);
    assert(saved == 1 && hidden == 1 && finished == 1);
    assert(MEM32(0x110014) == 1 && MEM32(0x110028) == 2);
    assert(MEM32(0x140004) == 77 && MEM32(0x13003C) == 1);
    invoke(0, 0, 0); assert(saved == 1); /* Busy: no duplicate submission. */
    MEM32(0x110014) = 0; duplicate = 3;
    invoke(0, 0, 1); assert(errors == 1 && saved == 1);
    duplicate = 0; match = 0;
    invoke(0, 0, 0); assert(saved == 1);
    invoke(6, 2, 1); assert(MEM32(0x110028) == 2);
    invoke(6, 8, 1); assert(MEM32(0x110028) == 2);
    invoke(6, 9, 1); assert(MEM32(0x110028) == 4);
    invoke(1, 0, 1); assert(cancelled == 1);
    free(memory);
    puts("PASS: name acceptance/profile setup, busy and duplicate-name guards, result events, cancel, saved registers and stack");
}
