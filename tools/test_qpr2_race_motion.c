/* Actual recovered motion/interpolation and camera collision bodies, with
 * real vector/angle helpers. Resource/object lookup is the only camera mock. */
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
void qpr2_recovered_000E0220(void);void qpr2_recovered_000DA770(void);void qpr2_recovered_00085680(void);
void sub_0006DA60(void) {assert(0);}
void sub_00013760(void) {g_eax=0x110000;g_esp+=4;}
void sub_000CDE60(void) {g_fp_top=(g_fp_top+7)&7;g_fp_stack[g_fp_top]=sqrt(MEMF(g_esp+4));g_esp+=4;}
static void invoke(void (*fn)(void),int args) {
 g_ecx=0x100000;g_esp=0x180000;MEM32(g_esp)=0x12345;MEMF(g_esp+4)=0.25;
 g_ebx=91;g_esi=92;g_edi=93;g_ebp=g_seh_ebp=94;g_fp_top=0;
 fn();assert(g_esp==0x180004+4*args && g_ebx==91 && g_esi==92 && g_edi==93 && g_fp_top==0);
}
int main(void) {
 void *ram=calloc(1,0x200000);assert(ram);g_xbox_mem_offset=(ptrdiff_t)ram;
 setenv("RECOMP_GAME_TIMING","1",1);
 MEMF(0x1c91d0)=1;MEMF(0x1c91e0)=0;
 for(int mode=0;mode<2;mode++) {
  memset((void*)XBOX_PTR(0x100000),0,0x200);
  MEM32(0x100004)=0x110000;MEM32(0x100108)=1;MEM32(0x1000bc)=mode;
  MEMF(0x110008)=10;MEMF(0x11000c)=20;MEMF(0x110010)=30;
  MEMF(0x11003c)=MEMF(0x11004c)=MEMF(0x11005c)=1;
  MEMF(0x11063c)=2;
  MEMF(0x100008)=0.5;MEMF(0x10000c)=1;MEMF(0x100010)=0.2;
  MEM32(0x100028)=1;MEMF(0x100038)=2;MEMF(0x100040)=1;
  MEMF(0x10004c)=4;MEMF(0x100050)=2;MEMF(0x100058)=4;MEMF(0x10005c)=2;
  MEMF(0x100060)=0;MEMF(0x100064)=1;MEMF(0x100068)=10;
  invoke(qpr2_recovered_000E0220,1);
  float travel=mode?0.75f:0.6875f;
  assert(MEMF(0x100008)==travel && MEMF(0x10000c)==(mode?1:0.75f));
  assert(fabsf(MEMF(0x100010)-0.45f)<1e-6f && MEMF(0x100014)==1);
  assert(MEMF(0x100080)==1 && MEMF(0x100084)==0 && MEMF(0x100088)==0);
  assert(MEMF(0x10008c)==10+travel && MEMF(0x100090)==22 && MEMF(0x100094)==30);
 }
 const unsigned offsets[]={0x368,0x374,0x4a4,0x4b0,0x4bc,0x4c8,0x4d4,0x4e0,0x498,0x414,0x43c,0x448,0x454,0x460,0x3c4,0x3ec};
 for(unsigned k=0;k<16;k++){MEMF(0x100000+offsets[k]+4)=4+k;MEMF(0x100000+offsets[k]+8)=20+k;}
 for(int fraction=0;fraction<3;fraction++) {
  MEMF(0x100650)=fraction*0.5f;invoke(qpr2_recovered_000DA770,0);
  for(unsigned k=0;k<16;k++)assert(MEMF(0x100000+offsets[k])==4+k+16*(1-fraction*0.5f));
 }
 memset((void*)XBOX_PTR(0x100000),0,0x200);
 memset((void*)XBOX_PTR(0x110000),0,0x700);
 MEM32(0x110008)=0x120000;MEM32(0x120018)=0x130000;MEM32(0x1d9fb8)=0x140000;
 MEMF(0x10000c)=2;MEMF(0x100010)=4;MEMF(0x1bc594)=0.001;
 invoke(qpr2_recovered_00085680,1);
 assert(MEMF(0x100010)==3.5 && MEMF(0x110010)==0 && MEMF(0x110018)==0);
 free(ram);puts("PASS: active vehicle spring/travel and wheel angular motion, world coordinates, all 16 interpolation outputs/endpoints, zero-distance camera radius update and preserved ABI");
}
