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


void qpr2_recovered_00070C30(void);void qpr2_recovered_00070D60(void);
static unsigned virtual_calls;
static void update(void) { ++virtual_calls;g_esp+=12; }
recomp_func_t recomp_lookup_manual(uint32_t va) { (void)va;return NULL; }
recomp_func_t recomp_lookup_kernel(uint32_t va) { (void)va;return NULL; }
recomp_func_t recomp_lookup(uint32_t va) { assert(va==0x150100);return update; }
void recomp_icall_not_code_log(uint32_t va) { (void)va;assert(0); }
void recomp_icall_fail_log(uint32_t va) { (void)va;assert(0); }
static void invoke(void (*fn)(void)) {
 g_ecx=0x100000;g_esp=0x180000;g_ebx=91;g_esi=92;g_edi=93;g_fp_top=0;g_ebp=g_seh_ebp=94;
 MEM32(g_esp)=0x98765;MEM32(g_esp+4)=0;MEM32(g_esp+8)=0;
 fn();assert(g_esp==0x18000C && g_ebx==91 && g_esi==92 && g_edi==93 && !g_fp_top);
}
int main(void) {
 void *memory=calloc(1,0x200000);assert(memory);g_xbox_mem_offset=(ptrdiff_t)memory;
 MEMF(0x1BB27C)=1e20f;MEMF(0x1C91CC)=0.5;
 MEM32(0x100020)=0x110000;MEM32(0x110650)=0x120000;MEM32(0x110654)=1;
 MEMF(0x120008)=3;MEMF(0x120034)=5;MEMF(0x120038)=7;MEMF(0x12003C)=9;
 MEM32(0x100048)=0x130000;
 MEM32(0x100024)=MEM32(0x10008C)=0x150000;MEM32(0x150004)=MEM32(0x150008)=0x150100;
 MEM32(0x100158)=0x140000;MEM32(0x100164)=1;
 MEMF(0x140000)=10;MEMF(0x140004)=20;MEMF(0x140008)=30;MEMF(0x14000C)=1;
 invoke(qpr2_recovered_00070C30);
 assert(MEMF(0x130000)==5 && MEMF(0x130004)==10 && MEMF(0x130008)==9);
 assert(MEMF(0x1001A4)==9 && MEMF(0x1001B0)==11);
 invoke(qpr2_recovered_00070D60);
 assert(virtual_calls==4);free(memory);
 puts("PASS: rider bounds callbacks, real bounds helpers, nonempty loop across internal alignment NOP, copied model points and preserved ABI");
}
