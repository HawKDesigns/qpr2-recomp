/* Execute the complete retail HUD callback, including its internal alignment
 * and two nonempty loops. Mock device submission and layout dependencies. */
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
void qpr2_recovered_000C3AF0(void);
static unsigned quads,layout_calls,virtual_calls;
static void rect_callback(void) {++virtual_calls;g_esp+=8;}
recomp_func_t recomp_lookup_manual(uint32_t va) {(void)va;return NULL;}
recomp_func_t recomp_lookup_kernel(uint32_t va) {(void)va;return NULL;}
recomp_func_t recomp_lookup(uint32_t va) {assert(va==0x150100);return rect_callback;}
void recomp_icall_not_code_log(uint32_t va) {fprintf(stderr,"bad target %08x pc %08x\n",va,g_recomp_pc);assert(0);}
void recomp_icall_fail_log(uint32_t va) {(void)va;assert(0);}
void sub_00037EF0(void) {
 const float rect[6]={10,20,200,100,0,1};++layout_calls;
 memcpy((void*)XBOX_PTR(g_ecx),rect,sizeof rect);g_esp+=16;
}
void sub_00037C80(void) {g_esp+=8;}
void sub_0001D270(void) {g_esp+=4;}
void sub_0001D370(void) {g_esp+=4;}
void sub_000E6A00(void) {g_esp+=4;}
void sub_0012AF20(void) {g_esp+=4;}
void sub_001931B0(void) {g_esp+=8;}
void sub_000E5710(void) {assert(isfinite(MEMF(g_esp+4)) && isfinite(MEMF(g_esp+8)));++quads;g_esp+=4;}
void sub_00015CD0(void) {for(int k=0;k<4;k++)MEM32(g_ecx+4*k)=MEM32(g_esp+4+4*k);g_esp+=20;}
void sub_0012B278(void) {g_eax=(uint32_t)(int32_t)g_fp_stack[g_fp_top];g_fp_top=(g_fp_top+1)&7;g_esp+=4;}
static void invoke(void) {
 g_ecx=0x100000;g_esp=0x180000;MEM32(g_esp)=0x98765;MEM32(g_esp+4)=0x170000;
 g_ebx=91;g_esi=92;g_edi=93;g_ebp=g_seh_ebp=94;g_fp_top=0;
 qpr2_recovered_000C3AF0();
 assert(g_esp==0x180008 && g_ebx==91 && g_esi==92 && g_edi==93 && g_fp_top==0);
}
int main(void) {
 void *ram=calloc(1,0x200000);assert(ram);g_xbox_mem_offset=(ptrdiff_t)ram;
 MEM32(0x100000)=0x1a0000;MEM32(0x1a0060)=MEM32(0x1a0064)=0x150100;
 for(unsigned at=0x70;at<=0x88;at+=4)MEM32(0x100000+at)=0x110000;
 MEM32(0x110008)=0x111000;MEMF(0x111010)=16;MEMF(0x111014)=16;
 MEM32(0x1000b4)=0x120000;MEM32(0x1000b8)=0x140000;
 MEM32(0x120058)=0x130000;MEM32(0x120060)=0x160000;MEM32(0x1300c4)=0x160000;
 MEM32(0x1000a8)=0x151000;MEM32(0x1000ac)=0x152000;
 MEMF(0x151000)=0.5f;MEMF(0x14013c)=2;
 MEM32(0x1d9fb8)=0x161000;
 MEMF(0x1c91d0)=1;MEMF(0x1c91cc)=0.5;MEMF(0x1c91e0)=0;
 MEMF(0x1adfec)=0.75;MEMF(0x1c1608)=4;MEMF(0x1ae830)=255;MEMF(0x1c1550)=1;
 MEM32(0x161010)=2;MEM32(0x12005c)=1;
 MEMF(0x152000)=1;MEMF(0x152004)=0;MEM32(0x15200c)=123;
 invoke();
 assert(layout_calls==1 && virtual_calls==2 && quads==3);
 /* The second loop resets this state when it reaches the matching rider. */
 assert(MEM32(0x15200c)==123); /* Zero timer takes the preserved-state path. */
 assert(MEMF(0x100090)==10 && MEMF(0x100094)==20 && MEMF(0x100098)==200 && MEMF(0x10009c)==100);
 MEMF(0x152004)=0.25;quads=layout_calls=virtual_calls=0;
 invoke();assert(MEM32(0x15200c)==0 && MEMF(0x152004)==0 && MEMF(0x152000)==0.75);
 assert(layout_calls==1 && virtual_calls==2 && quads==3);
 free(ram);puts("PASS: complete race HUD callback, nonempty loops across internal alignment, submitted background/marker quads, state reset and final RET4 ABI");
}
