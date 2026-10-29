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


void qpr2_recovered_00027E00(void);
void qpr2_recovered_00084440(void);
void sub_00027E00(void);
static unsigned setters, updates;
static void pop0(void) { g_esp+=4; }
static void pop4(void) { g_esp+=8; }
static void set_args(void) { assert(MEM32(g_esp+4)==123 && MEM32(g_esp+8)==456); ++setters; g_esp+=12; }
recomp_func_t recomp_lookup_manual(uint32_t va) { (void)va; return NULL; }
recomp_func_t recomp_lookup_kernel(uint32_t va) { (void)va; return NULL; }
recomp_func_t recomp_lookup(uint32_t va) {
 if(va==0x150100)return set_args;
 if(va==0x150200)return pop0;
 if(va==0x150300)return pop4;
 assert(0);return NULL;
}
void recomp_icall_not_code_log(uint32_t va) { (void)va;assert(0); }
void recomp_icall_fail_log(uint32_t va) { (void)va;assert(0); }
void sub_0001A720(void) { ++updates;g_esp+=4; }
void sub_00028950(void) { ++updates;g_esp+=4; }
void sub_00029460(void) { assert(MEM32(g_esp+4)==0x110054 && MEM32(g_esp+8)==0x110088);g_esp+=12; }
void sub_00026B30(void) { assert(MEM32(g_esp+4)==0x140000);g_esp+=8; }
void sub_000AC0D0(void) { g_esp+=4; }
void sub_000AD350(void) { g_esp+=4; }
void sub_0007B960(void) { ++updates;g_esp+=4; }
void sub_0002EDD0(void) { assert(MEM32(g_esp+4)==1);g_esp+=8; }
void sub_0006E470(void) { assert(g_ecx==0x11004C);g_esp+=8; }
void sub_00026F20(void) { assert(MEM32(g_esp+4)==1);g_esp+=8; }
void sub_00013760(void) { g_eax=0x110000;g_esp+=4; }
void sub_000CE350(void) { g_fp_top=(g_fp_top+7)&7;g_fp_stack[g_fp_top]=0.1;g_esp+=4; }
void sub_000CDDF0(void) { g_fp_top=(g_fp_top+7)&7;g_fp_stack[g_fp_top]=sin(MEMF(g_esp+4));g_esp+=4; }
void sub_000CDE00(void) { g_fp_top=(g_fp_top+7)&7;g_fp_stack[g_fp_top]=cos(MEMF(g_esp+4));g_esp+=4; }
static void invoke(void (*fn)(void),unsigned cleanup) {
 g_ecx=0x100000;g_esp=0x180000;g_ebx=91;g_esi=92;g_edi=93;g_fp_top=0;
 MEM32(g_esp)=0x98765;MEM32(g_esp+4)=123;MEM32(g_esp+8)=456;
 fn();assert(g_esp==0x180004+cleanup && g_ebx==91 && g_esi==92 && g_edi==93 && !g_fp_top);
}
int main(void) {
 void *memory=calloc(1,0x200000);assert(memory);g_xbox_mem_offset=(ptrdiff_t)memory;
 MEM32(0x100000)=0x150000;MEM32(0x150054)=0x150100;MEM32(0x150004)=0x150200;MEM32(0x15000C)=0x150300;
 MEM32(0x10003C)=0x110000;MEM32(0x100044)=0x120000;MEM32(0x100048)=0x121000;MEM32(0x10005C)=0x122000;
 MEM32(0x110000)=0x151000;MEM32(0x15106C)=0x150200;
 MEM32(0x122000)=0x152000;MEM32(0x152010)=0x150200;
 MEM32(0x100020)=2;MEM32(0x120278)=1;MEM32(0x1D9FB8)=0x140000;
 invoke(qpr2_recovered_00027E00,8);assert(g_eax==1 && setters==1 && updates==3);
 assert(MEM32(0x100020)==0 && MEMF(0x100034)==1 && MEM32(0x100140)==0 && MEM32(0x100090)==0);
 invoke(sub_00027E00,8);assert(setters==2); /* actual former-stub direct bridge */
 MEMF(0x110034)=10;MEMF(0x110038)=20;MEMF(0x11003C)=30;MEMF(0x110040)=4;
 MEMF(0x10000C)=1;MEMF(0x1AF36C)=1;
 invoke(qpr2_recovered_00084440,4);
 assert(MEMF(0x100034)==10 && MEMF(0x100038)==20 && MEMF(0x10003C)==30);
 assert(fabsf(MEMF(0x100014)-(cosf(0.025f)*10+sinf(0.025f)*30))<0.00001f);
 assert(MEMF(0x100018)==20 && MEM32(0x100028)==0);
 free(memory);puts("PASS: race reset/camera callbacks, direct tail bridge, retail rotation/vector helpers, state setup and callee ABI");
}
