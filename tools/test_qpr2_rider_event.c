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



void qpr2_recovered_0002EBA0(void);
static unsigned impacts,effects,events;static int result;
static void virtual_query(void) {g_eax=result;g_esp+=4;}
recomp_func_t recomp_lookup_manual(uint32_t va) {(void)va;return NULL;}
recomp_func_t recomp_lookup_kernel(uint32_t va) {(void)va;return NULL;}
recomp_func_t recomp_lookup(uint32_t va) {assert(va==0x150100);return virtual_query;}
void recomp_icall_not_code_log(uint32_t va) {(void)va;assert(0);}
void recomp_icall_fail_log(uint32_t va) {(void)va;assert(0);}
void sub_0002AD00(void) {g_eax=0;g_esp+=16;}
void sub_000264C0(void) {assert(MEM32(g_esp+4)==1);++impacts;g_esp+=8;}
void sub_0009DDC0(void) {g_eax=0;g_esp+=4;}
void sub_0002CEC0(void) {++events;g_esp+=4;}
void sub_00107440(void) {
 unsigned vector=MEM32(g_esp+4);assert(MEM32(g_esp+8)==1);
 assert(MEMF(vector)==10 && MEMF(vector+4)==20.5 && MEMF(vector+8)==30 && MEMF(vector+12)==10);
 ++effects;g_esp+=12;
}
static void invoke(void) {
 g_ecx=0x100000;g_esp=0x180000;MEM32(g_esp)=0x98765;MEM32(g_esp+4)=0x110000;
 g_esi=91;g_edi=92;g_ebx=93;g_ebp=g_seh_ebp=94;g_fp_top=0;
 qpr2_recovered_0002EBA0();
 assert(g_esp==0x180008 && g_esi==91 && g_edi==92 && g_ebx==93 && !g_fp_top && g_seh_ebp==94);
}
int main(void) {
 void *ram=calloc(1,0x200000);assert(ram);g_xbox_mem_offset=(ptrdiff_t)ram;
 MEM32(0x100478)=0x120000;MEM32(0x120000)=0x150000;MEM32(0x15003C)=0x150100;
 MEM32(0x100018)=0x130000;MEM32(0x1D9FB8)=0x140000;
 invoke();assert(!MEM32(0x100494));
 MEM32(0x10049C)=1;MEM8(0x110006)=1;MEM16(0x110002)=30;MEM16(0x110024)=35;
 invoke();assert(MEM32(0x100494)==8);
 MEM32(0x100494)=0;MEM16(0x110024)=40;invoke();assert(!MEM32(0x100494));
 MEM8(0x110006)=0;MEM16(0x110004)=60;MEM16(0x110024)=55;invoke();assert(MEM32(0x100494)==8);
 /* Special collision mode executes actual vector transform and both increments. */
 MEM32(0x100488)=5;MEM16(0x110024)=11;MEMF(0x13003C)=MEMF(0x13004C)=MEMF(0x13005C)=1;
 invoke();assert(MEM32(0x1200C8)==2);
 MEM32(0x100488)=0;MEM32(0x1004DC)=7;MEM32(0x10048C)=3;MEM16(0x110004)=30;MEM16(0x110024)=20;
 result=0;invoke();assert(!impacts && !events);
 result=1;invoke();assert(impacts==1 && events==1 && MEM32(0x10048C)==1 && MEM32(0x1004DC)==8);
 MEM8(0x110000)=0x2B;MEM16(0x110024)=0x24;MEMF(0x130008)=10;MEMF(0x13000C)=20;MEMF(0x130010)=30;
 MEMF(0x1C91CC)=0.5;MEM32(0x1DA00C)=0x160000;invoke();assert(effects==1);
 free(ram);puts("PASS: recovered rider event thresholds, actual vector helper/counts, virtual query/collision transitions, effect position and preserved RET4/register/FPU ABI");
}
