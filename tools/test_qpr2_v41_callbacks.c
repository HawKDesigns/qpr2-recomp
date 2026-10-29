/* Real recovered contact/pose/draw bodies; fixtures supply engine services. */
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
void qpr2_recovered_00020600(void),qpr2_recovered_00028740(void),qpr2_recovered_00029340(void);
static int base_calls,pose_calls,draw_calls,impact_calls;
static void pose(void) {
 assert(g_ecx==0x120000);
 uint32_t p=MEM32(g_esp+4),q=MEM32(g_esp+8);
 assert(MEMF(p)==1 && MEMF(p+4)==12 && MEMF(p+8)==3);
 assert(q==0x16003c);++pose_calls;g_esp+=12;
}
recomp_func_t recomp_lookup_manual(uint32_t va){(void)va;return NULL;}
recomp_func_t recomp_lookup_kernel(uint32_t va){(void)va;return NULL;}
recomp_func_t recomp_lookup(uint32_t va){assert(va==0x150100);return pose;}
void recomp_icall_not_code_log(uint32_t va){(void)va;assert(0);}
void recomp_icall_fail_log(uint32_t va){(void)va;assert(0);}
void sub_00020940(void){++base_calls;g_esp+=8;}
void sub_00071000(void){g_eax=MEM32(MEM32(g_esp+4)+4);g_esp+=8;}
void sub_000FA690(void){++impact_calls;g_esp+=4;}
void sub_00020880(void){g_esp+=8;}
void sub_00193BC0(void){assert(g_ecx==0x40354 || g_ecx==0x4035c);g_esp+=4;}
void sub_000E5430(void){assert(MEM32(g_esp+4)==1);g_esp+=4;}
void sub_001001C0(void){assert(g_ecx==0x110000 && MEM32(g_esp+4)==0x10028c && MEM32(g_esp+8)==0x200);++draw_calls;g_esp+=12;}
static void invoke(void (*fn)(void)) {
 g_ecx=0x100000;g_esp=0x180000;MEM32(g_esp)=0x12345;MEMF(g_esp+4)=1.0f/60;
 g_ebx=91;g_esi=92;g_edi=93;g_ebp=g_seh_ebp=94;g_fp_top=0;
 fn();assert(g_esp==0x180008 && g_ebx==91 && g_esi==92 && g_edi==93 && g_fp_top==0);
}
int main(void) {
 void *ram=calloc(1,0x200000);assert(ram);g_xbox_mem_offset=(ptrdiff_t)ram;
 /* Two contacts mapped to distinct accumulation bins. */
 MEM32(0x100018)=0x110000;MEM32(0x100278)=1;MEM32(0x1004a8)=0x130000;
 MEM32(0x1101bc)=0x140000;MEM32(0x140000)=0x141000;
 MEM32(0x140004)=0;MEM32(0x141004)=1;
 MEM32(0x1ca6d8)=2;MEM32(0x1ca6dc)=5;
 MEMF(0x140078)=2.25f;MEMF(0x141078)=4.5f;
 invoke(qpr2_recovered_00029340);
 assert(base_calls==1 && MEMF(0x130020)==2.25f && MEMF(0x130044)==4.5f && MEMF(0x1004ec)==4.5f);
 MEM32(0x100278)=0;invoke(qpr2_recovered_00029340);assert(base_calls==2);
 MEM32(0x100278)=1;MEM32(0x1101bc)=0;invoke(qpr2_recovered_00029340);assert(MEMF(0x1004ec)==0);
 /* Impact transition inside the contact loop and its final return. */
 MEM32(0x1101bc)=0x140000;MEM32(0x140000)=0;MEM32(0x1004dc)=3;
 MEMF(0x140070)=0;MEMF(0x1af32c)=1;invoke(qpr2_recovered_00029340);
 assert(impact_calls==1 && MEM32(0x1004dc)==4 && MEM32(0x10048c)==1 && MEM32(0x1004a0)==16);
 /* All ten joint vectors plus the duplicated root; virtual position call. */
 MEM32(0x1002e0)=0x110000;MEM32(0x110108)=0x120000;MEM32(0x12000c)=0x130000;
 MEM32(0x1004a4)=0x140000;MEM32(0x100018)=0x160000;MEM32(0x100064)=0x120000;
 MEM32(0x120000)=0x150000;MEM32(0x150008)=0x150100;
 MEMF(0x160008)=1;MEMF(0x16000c)=2;MEMF(0x160010)=3;MEMF(0x1aeb20)=10;
 const unsigned offsets[]={0x48,0x1ec,0x588,0x2e8,0x630,0x390,0x828,0x978,0x87c,0x9cc};
 for(unsigned i=0;i<10;i++)for(unsigned k=0;k<3;k++)MEMF(0x130000+offsets[i]+4*k)=10*i+k;
 invoke(qpr2_recovered_00028740);assert(pose_calls==1);
 for(unsigned i=0;i<10;i++)for(unsigned k=0;k<3;k++)assert(MEMF(0x140000+16*i+4*k)==10*i+k);
 for(unsigned k=0;k<3;k++)assert(MEMF(0x1400a0+4*k)==k);
 /* Draw callback supplies RET4 rather than the old return-only placeholder. */
 MEM32(0x1dc444)=0x207;MEM32(0x1dc3cc)=0;invoke(qpr2_recovered_00020600);
 assert(draw_calls==1 && MEM32(0x1dc444)==0x203 && MEM32(0x1dc3cc)==1);
 assert(MEMF(0x1cf1e0)==1 && MEMF(0x1cf1e4)==1 && MEMF(0x1cf1e8)==1 && MEMF(0x1cf1ec)==1);
 free(ram);puts("PASS: actual populated contact accumulation/impact transition, no-contact exits, ten pose vectors and virtual position, draw RET4 and saved-register/FPU ABI");
}
