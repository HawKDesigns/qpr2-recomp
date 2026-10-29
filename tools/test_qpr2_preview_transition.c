/* Run the retail containing draw routine through its previously missing
 * callback, with the real vector/matrix helpers and mocked device services. */
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
void sub_000F09A0(void);void qpr2_recovered_000FD970(void);
static int legacy,callbacks,flushes;
recomp_func_t recomp_lookup_manual(uint32_t va) {
 if(va==0xfd970) return legacy?NULL:qpr2_recovered_000FD970;
 return NULL;
}
recomp_func_t recomp_lookup_kernel(uint32_t va) {(void)va;return NULL;}
recomp_func_t recomp_lookup(uint32_t va) {(void)va;return NULL;}
void recomp_icall_not_code_log(uint32_t va) {(void)va;assert(0);}
void recomp_icall_fail_log(uint32_t va) {(void)va;assert(0);}
static void fp_result(double d) {g_fp_top=(g_fp_top+7)&7;g_fp_stack[g_fp_top]=d;}
void sub_000CDE60(void) {fp_result(sqrt(MEMF(g_esp+4)));g_esp+=4;}
void sub_000CDE00(void) {fp_result(cos(MEMF(g_esp+4)));g_esp+=4;}
void sub_000CDDF0(void) {fp_result(sin(MEMF(g_esp+4)));g_esp+=4;}
void sub_00092960(void) {memcpy((void*)XBOX_PTR(g_ecx),(const void*)XBOX_PTR(MEM32(g_esp+4)),0x80);g_esp+=8;}
void sub_00015CD0(void) {for(int i=0;i<4;i++)MEM32(g_ecx+4*i)=MEM32(g_esp+4+4*i);g_esp+=20;}
void sub_00105830(void) {g_esp+=4;}
void sub_0012AF20(void) {++flushes;g_esp+=4;}
void sub_000E6A00(void) {g_esp+=4;}
void sub_000E4CA0(void) {g_esp+=4;}
void sub_000E4C60(void) {g_esp+=4;}
void sub_000E5710(void) {g_esp+=4;}
void sub_0001D370(void) {g_esp+=4;}
void sub_00193BC0(void) {g_esp+=4;}
void sub_000E3360(void) {fp_result(640);g_esp+=4;}
void sub_000E3380(void) {fp_result(480);g_esp+=4;}
void sub_000E9C80(void) {
 ++callbacks;
 /* The first pass queues FD970; the next calls it before returning. */
 if(callbacks==2) {
  g_esp-=4;MEM32(g_esp)=0x160000;
  g_esp-=4;MEM32(g_esp)=0xabcdef;
  if(legacy) {g_esp+=4;g_eax=0;} else qpr2_recovered_000FD970();
 }
 g_esp+=4;
}
static void invoke(void) {
 g_esp=0x180000;MEM32(g_esp)=0x12345;
 MEM32(g_esp+4)=0x110000;MEM32(g_esp+8)=0;
 g_ecx=0x100000;g_ebx=91;g_esi=92;g_edi=93;
 g_ebp=g_seh_ebp=94;g_fp_top=0;callbacks=0;flushes=0;
 sub_000F09A0();
}
int main(void) {
 void *ram=calloc(1,0x200000);assert(ram);g_xbox_mem_offset=(ptrdiff_t)ram;
 MEM32(0x100020)=0x120000;
 MEM32(0x12006c)=0x130000;MEM32(0x120058)=0x131000;
 MEM32(0x120060)=0x132000;MEM32(0x12005c)=0x133000;
 MEM32(0x120024)=0x134000;MEM32(0x1200fc)=0x135000;
 MEMF(0x12004c)=10;MEMF(0x1200a4)=1;MEMF(0x1200ac)=1;MEMF(0x1200b0)=1;
 MEMF(0x110000)=MEMF(0x110010)=MEMF(0x110020)=1;
 MEMF(0x110074)=1;MEMF(0x1c91d0)=1;MEMF(0x1c91cc)=0.5;
 MEMF(0x1c91e0)=1e-7f;MEMF(0x1c91dc)=1e-7f;
 MEMF(0x1dc168)=MEMF(0x1dc16c)=0.1f;
 legacy=1;invoke();assert(g_esp==0x180008);assert(g_esi!=92||g_edi!=93||g_ebx!=91);
 for(int alternate=0;alternate<2;alternate++) {
  MEM32(0x1dc3e8)=alternate;legacy=0;invoke();
  assert(g_esp==0x18000c && g_ebx==91 && g_esi==92 && g_edi==93);
  assert(callbacks==2 && flushes==4 && g_fp_top==0);
  assert(MEM32(0x1dc428)==3 && MEM32(0x1dc368)==3 && MEM32(0x1dc444)==0x203);
 }
 free(ram);puts("PASS: actual preview transition draw chain, missing RET4 stack drift reproduced, both restored callback exits preserve saved registers and stack");
}
