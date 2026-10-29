/* Compile with the repository src include directory; no game files required. */
#include <assert.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include "recomp/gen/recomp_types.h"

ptrdiff_t g_xbox_mem_offset;
uint32_t g_xbox_code_lo, g_xbox_code_hi;
RECOMP_TLS uint32_t g_eax, g_ecx, g_edx, g_esp, g_ebx, g_esi, g_edi;
RECOMP_TLS volatile uint32_t g_recomp_pc;
volatile uint32_t g_icall_trace[16], g_icall_trace_idx;
volatile uint64_t g_icall_count;
static jmp_buf stop;
static unsigned callbacks, locks, unlocks, yields, sleeps, mode;
static uint32_t initial_sp;
void qpr2_recovered_00128880(void);
void qpr2_recovered_00087CC0(void);

static void callback(void)
{
    if (mode == 1) {
        assert(g_ecx == 0x100000u);
        assert(g_esp == initial_sp - 8u);
        assert(MEM32(g_esp) == 0x0012888Cu);
    } else {
        assert(g_ecx == 0x100000u || g_ecx == 0x100100u);
        assert(g_esp == initial_sp - 12u);
        assert(MEM32(g_esp) == 0x00087D13u);
    }
    g_esp += 4;
    ++callbacks;
    if (mode == 1 && callbacks == 4) longjmp(stop, 1);
}
recomp_func_t recomp_lookup_manual(uint32_t va) { (void)va; return NULL; }
recomp_func_t recomp_lookup(uint32_t va) { assert(va == 0x120000u); return callback; }
recomp_func_t recomp_lookup_kernel(uint32_t va) { (void)va; assert(0); return NULL; }
void recomp_icall_fail_log(uint32_t va) { (void)va; assert(0); }
void recomp_icall_not_code_log(uint32_t va) { (void)va; assert(0); }
void sub_000C9B9F(void)
{
    assert(MEM32(g_esp + 4) == 0x1234u);
    assert(MEM32(g_esp + 8) == 0xFFFFFFFFu);
    ++locks; g_esp += 12;
}
void sub_000C9AE9(void)
{
    assert(MEM32(g_esp + 4) == 0x1234u);
    ++unlocks; g_esp += 8;
}
void sub_000C9CEB(void) { ++yields; g_esp += 4; }
void sub_000C9BB1(void)
{
    assert(MEM32(g_esp + 4) == (mode == 3 ? 1000u : 8u));
    g_esp += 8; ++sleeps;
    assert(g_esp == initial_sp - 8u);
    if (mode == 3 || sleeps == 4) longjmp(stop, 1);
}
int main(void)
{
    void *ram = calloc(1, 4u * 1024u * 1024u);
    assert(ram); g_xbox_mem_offset = (ptrdiff_t)ram;
    initial_sp = 0x200000u;
    MEM32(0x100000u) = 0x110000u;
    MEM32(0x100100u) = 0x110000u;
    MEM32(0x11000Cu) = MEM32(0x110020u) = 0x120000u;
    g_esp = initial_sp; MEM32(g_esp + 4) = 0x100000u;
    mode = 1;
    if (!setjmp(stop)) qpr2_recovered_00128880();
    assert(callbacks == 4 && g_esp == initial_sp - 4u);
    callbacks = 0; mode = 2; g_esp = initial_sp;
    MEM32(0x1D9348u) = 2; MEM32(0x1D934Cu) = 0x1234u;
    MEM32(0x1D9358u) = 0x130000u;
    MEM32(0x130000u) = 0x100000u; MEM32(0x130004u) = 0x100100u;
    if (!setjmp(stop)) qpr2_recovered_00087CC0();
    assert(callbacks == 4 && locks == 4 && unlocks == 4 && yields == 4);
    mode = 3; sleeps = 0; g_esp = initial_sp;
    MEM32(0x1D9358u) = 0;
    if (!setjmp(stop)) qpr2_recovered_00087CC0();
    assert(sleeps == 1 && callbacks == 4);
    free(ram);
    puts("PASS: object worker repeats; service worker locks, dispatches, yields, sleeps; empty list sleeps; guest stack remains balanced.");
    return 0;
}
