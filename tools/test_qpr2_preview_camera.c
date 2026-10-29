#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "recomp/gen/recomp_types.h"
ptrdiff_t g_xbox_mem_offset;
uint32_t g_xbox_code_lo=0x10000,g_xbox_code_hi=0x1c0000;
RECOMP_TLS uint32_t g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_ebp,g_seh_ebp;
RECOMP_TLS volatile uint32_t g_recomp_pc;
RECOMP_TLS int g_df,g_fp_top,g_fp_cmp;
RECOMP_TLS uint16_t g_fp_control_word,g_fp_cc;
RECOMP_TLS double g_fp_stack[8];
volatile uint32_t g_icall_trace[16],g_icall_trace_idx;
volatile uint64_t g_icall_count;
void qpr2_recovered_00083560(void);void qpr2_recovered_000D0600(void);void qpr2_recovered_000D0B80(void);
static void invoke(void (*fn)(void),int args) {
 g_ecx=0x100000;g_esp=0x180000;MEM32(g_esp)=0x98765;
 g_ebx=91;g_esi=92;g_edi=93;g_ebp=g_seh_ebp=94;g_fp_top=0;
 fn();assert(g_esp==0x180004+4*args && g_ebx==91 && g_esi==92 && g_edi==93 && g_fp_top==0);
}
int main(void) {
 void *ram=calloc(1,0x200000);assert(ram);g_xbox_mem_offset=(ptrdiff_t)ram;
 MEMF(0x10001c)=4;MEMF(0x100020)=2;
 for(int k=0;k<3;k++) {MEMF(0x110000+4*k)=10*(k+1);MEMF(0x100010+4*k)=k+1;}
 MEM32(0x180004)=0x110000;MEM32(0x180008)=0x111000;MEMF(0x18000c)=0.25;
 invoke(qpr2_recovered_00083560,3);
 for(int k=0;k<3;k++) {assert(MEMF(0x100010+4*k)==10.5f*(k+1));assert(MEMF(0x100004+4*k)==2.625f*(k+1));}
 memset((void*)XBOX_PTR(0x100000),0,0x100);
 MEM32(0x100024)=0x120000;MEM32(0x100028)=0x121000;MEM32(0x10002c)=0x122000;MEM32(0x100030)=1;
 MEMF(0x120000)=1;MEMF(0x12000c)=2;MEMF(0x1c1f94)=1e20f;
 MEMF(0x130000)=5;MEMF(0x130004)=7;MEMF(0x130008)=9;
 MEMF(0x131000)=MEMF(0x131010)=MEMF(0x131020)=1;
 MEM32(0x180004)=0x130000;MEM32(0x180008)=0x131000;
 invoke(qpr2_recovered_000D0600,2);
 assert(MEM32(0x100028)==0x122000 && MEM32(0x10002c)==0x121000);
 assert(MEMF(0x122000)==6 && MEMF(0x122004)==7 && MEMF(0x122008)==9 && MEMF(0x12200c)==2);
 assert(MEMF(0x100038)==4 && MEMF(0x10003c)==5 && MEMF(0x100040)==7);
 assert(MEMF(0x100044)==8 && MEMF(0x100048)==9 && MEMF(0x10004c)==11);
 MEM32(0x180004)=0x130000;MEM32(0x180008)=0x131000;
 invoke(qpr2_recovered_000D0B80,2);
 assert(MEM32(0x100028)==0x121000 && MEM32(0x10002c)==0x122000);
 assert(MEMF(0x121000)==1 && MEMF(0x121004)==0 && MEMF(0x121008)==0 && MEMF(0x12100c)==-3);
 free(ram);puts("PASS: preview camera spring position/velocity update, nonempty geometry plane transforms and bounds, buffer swap, RET12/RET8 ABI");
}
