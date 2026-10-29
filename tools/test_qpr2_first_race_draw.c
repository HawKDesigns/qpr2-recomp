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


void sub_0009E940(void);
void qpr2_recovered_0009E240(void);
static int legacy, callback_count, passes, finals;
static void pop0(void) { g_esp+=4; }
recomp_func_t recomp_lookup_manual(uint32_t va) { (void)va;return NULL; }
recomp_func_t recomp_lookup_kernel(uint32_t va) { (void)va;return NULL; }
recomp_func_t recomp_lookup(uint32_t va) { assert(va==0x150100);return pop0; }
void recomp_icall_not_code_log(uint32_t va) { (void)va;assert(0); }
void recomp_icall_fail_log(uint32_t va) { (void)va;assert(0); }
void sub_000E7E50(void) { g_esp+=4; }
void sub_00092960(void) { g_esp+=8; }
void sub_000F09A0(void) { assert(0); }
void sub_000E9C80(void) {
 ++callback_count; g_ecx=0x160000; g_esp-=4; MEM32(g_esp)=0x170000;
 g_esp-=4;MEM32(g_esp)=0x99999;
 if(legacy) { g_esp+=4;g_eax=0; } else qpr2_recovered_0009E240();
 g_esp+=4; /* cdecl return for this render submission helper */
}
void sub_00091A90(void) { g_esp+=4; }
void sub_000EBD60(void) { g_esp+=8; }
void sub_0010FEE0(void) { g_esp+=8; }
void sub_00105830(void) { g_esp+=4; }
void sub_00193A20(void) { ++passes;g_esp+=8; }
void sub_00193BC0(void) { g_esp+=4; }
void sub_00101FB0(void) { ++finals;assert(MEM32(g_esp+8)==0x170000);g_esp+=20; }
void sub_0001ED70(void) { g_esp+=4; }
void sub_00030800(void) { g_esp+=8; }
void sub_00030CA0(void) { g_esp+=8; }
void sub_00033CA0(void) { g_esp+=4; }
void sub_0007E140(void) { g_esp+=4; }
void sub_0009DE50(void) { g_esp+=4; }
void sub_0009DEB0(void) { g_esp+=8; }
void sub_000EE620(void) { g_esp+=4; }
void sub_000EECD0(void) { g_esp+=12; }
void sub_000F1000(void) { g_esp+=16; }
void sub_000F10D0(void) { g_esp+=12; }
void sub_000F2F40(void) { g_esp+=8; }
void sub_000F36D0(void) { g_esp+=4; }
void sub_000F3770(void) { g_esp+=8; }
void sub_000F3970(void) { g_esp+=4; }
void sub_001035D0(void) { g_esp+=4; }
void sub_00105E20(void) { g_esp+=4; }
void sub_00106F50(void) { g_esp+=4; }
void sub_00107DA0(void) { g_esp+=4; }
void sub_00116EA0(void) { g_esp+=8; }
void sub_001173E0(void) { g_esp+=4; }
void sub_001175D0(void) { g_esp+=8; }
void sub_00117D40(void) { g_esp+=4; }
void sub_00117F80(void) { g_esp+=8; }
static void invoke(void) {
 g_ecx=0x160000;g_esp=0x180000;g_ebx=91;g_esi=92;g_edi=93;g_fp_top=0;
 g_ebp=g_seh_ebp=0x177000;
 MEM32(g_esp)=0x98765;MEM32(g_esp+4)=0x170000;
 sub_0009E940();
}
int main(void) {
 void *memory=calloc(1,0x200000);assert(memory);g_xbox_mem_offset=(ptrdiff_t)memory;
 MEM32(0x1D9FB8)=0x100000;MEM32(0x100038)=0x110000;
 MEM32(0x110000)=0x150000;MEM32(0x150028)=MEM32(0x15003C)=0x150100;
 MEM32(0x11001C)=0x120000;MEM32(0x1DA00C)=0x130000;
 MEM32(0x1D9FF0)=0x140000;MEM32(0x1DC340)=0x141000;MEMF(0x141008)=0.5;
 MEM32(0x1D7424)=1;MEM32(0x1DC3CC)=1;MEM32(0x1DC444)=0x203;
 MEM32(0x160048)=2;MEM32(0x1600B4)=0x142000;
 legacy=1;invoke();assert(g_esp==0x180004); /* missing callback leaves four bytes */
 assert(g_esi!=92); /* That shift also restores the wrong saved register. */
 legacy=0;invoke();assert(g_esp==0x180008 && g_ebx==91 && g_esi==92 && g_edi==93);
 assert(callback_count==2 && finals==1 && passes==2 && g_fp_top==0);
 free(memory);puts("PASS: actual first race drawing callback, reproduced missing-callback stack/register drift, restored render-pass ordering and preserved ABI");
}
