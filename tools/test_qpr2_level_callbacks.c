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

void qpr2_recovered_000D0100(void);
void qpr2_recovered_000D0790(void);
void qpr2_recovered_00023060(void);
void qpr2_recovered_000A6E00(void);
void qpr2_recovered_00085820(void);
void qpr2_recovered_000836A0(void);
recomp_func_t recomp_lookup_manual(uint32_t va) { (void)va; return NULL; }
recomp_func_t recomp_lookup_kernel(uint32_t va) { (void)va; return NULL; }
recomp_func_t recomp_lookup(uint32_t va) { (void)va; assert(0); return NULL; }
void recomp_icall_not_code_log(uint32_t va) { (void)va; assert(0); }
void recomp_icall_fail_log(uint32_t va) { (void)va; assert(0); }
/* Only object lookup/resource/camera dependencies are mocked; the two plane
 * callbacks and their retail matrix transform/bounds helpers are all real. */
void sub_00032920(void) { g_eax = ~0u; g_esp += 4; }
void sub_00021EC0(void) { assert(0); }
void sub_00021F90(void) { assert(0); }
void sub_00102570(void) { assert(0); }
void sub_00021AC0(void) { assert(0); }
void sub_00033A80(void) { assert(0); }
void sub_000227F0(void) { assert(0); }
void sub_0012AF20(void) { g_esp += 4; }
void sub_001184A0(void) { g_eax = MEM32(g_esp + 4); g_esp += 8; }
void sub_0001FF80(void) { g_esp += 8; }
void sub_00015CD0(void) { MEM32(g_ecx) = 0x80FFFFFF; g_esp += 20; }
void sub_00013760(void) { g_eax = MEM32(g_ecx + 4); g_esp += 4; }
void sub_000CDE60(void) {
 g_fp_top = (g_fp_top + 7) & 7;
 g_fp_stack[g_fp_top] = sqrt(MEMF(g_esp + 4)); g_esp += 4;
}
void sub_00030990(void) { g_eax = 0; g_esp += 8; }
void sub_0001FE00(void) { assert(0); }
static void invoke(void (*fn)(void), unsigned cleanup) {
 g_ecx=0x100000; g_esp=0x180000; g_ebx=123; g_esi=456; g_edi=789;
 g_ebp=g_seh_ebp=0x170000; g_fp_top=0;
 MEM32(g_esp)=0x12345; MEM32(g_esp+4)=0x110000; MEM32(g_esp+8)=0x140000;
 fn();
 assert(g_esp==0x180004+cleanup && g_ebx==123 && g_esi==456 && g_edi==789);
 assert(g_fp_top==0);
}
int main(void) {
 void *memory=calloc(1,0x200000); assert(memory); g_xbox_mem_offset=(ptrdiff_t)memory;
 MEM32(0x100024)=0x120000; MEM32(0x100028)=0x121000; MEM32(0x10002C)=0x122000;
 MEM32(0x100030)=2;
 MEMF(0x1C1F94)=1e20f;
 for(unsigned i=0;i<9;i++) MEMF(0x140000+i*4)=(i%4==0)?1:0;
 MEMF(0x110000)=10; MEMF(0x110004)=20; MEMF(0x110008)=30;
 MEMF(0x120000)=1; MEMF(0x120004)=2; MEMF(0x120008)=3; MEMF(0x12000C)=0.5f;
 MEMF(0x120010)=-2; MEMF(0x120014)=-4; MEMF(0x120018)=5; MEMF(0x12001C)=1;
 invoke(qpr2_recovered_000D0100,8);
 assert(MEMF(0x121000)==11 && MEMF(0x121004)==22 && MEMF(0x121008)==33);
 assert(MEMF(0x121010)==8 && MEMF(0x121014)==16 && MEMF(0x121018)==35);
 assert(MEMF(0x100008)==7 && MEMF(0x10000C)==15 && MEMF(0x100010)==32.5f);
 assert(MEMF(0x100014)==11.5f && MEMF(0x100018)==22.5f && MEMF(0x10001C)==36);
 invoke(qpr2_recovered_000D0790,8);
 assert(MEMF(0x12100C)==-139.5f && MEMF(0x12101C)==-49);
 assert(MEMF(0x100008)==17 && MEMF(0x10000C)==35 && MEMF(0x100010)==62.5f);
 assert(!memcmp((void*)(g_xbox_mem_offset+0x121000),(void*)(g_xbox_mem_offset+0x122000),32));
 MEM32(0x110004)=0x160000; invoke(qpr2_recovered_00023060,4);
 MEM32(0x1001DC)=1; MEM32(0x100004)=0x110000;
 invoke(qpr2_recovered_000A6E00,0);
 assert(MEMF(0x1000EC)==10 && MEM32(0x100104)==1 && MEM32(0x100180)==0x1100A0);
 assert(MEM32(0x100200)==3 && MEM32(0x10020C)==3);
 MEM32(0x100004)=0x110000; MEM32(0x110008)=0x120000; MEM32(0x120018)=0x130000;
 MEM32(0x1D9FB8)=0x150000; MEM32(0x150038)=0x150100;
 for(unsigned i=0;i<9;i++) MEMF(0x13003C+i*4)=(i%4==0)?1:0;
 for(unsigned i=0;i<3;i++) MEMF(0x130008+i*4)=MEMF(0x110010+i*4)=0;
 MEMF(0x1C91D0)=1; MEMF(0x1BC498)=0.00001f;
 for(unsigned i=0;i<3;i++) MEMF(0x10000C+i*4)=0;
 invoke(qpr2_recovered_00085820,4);
 invoke(qpr2_recovered_000836A0,4);
 assert(MEMF(0x11001C)==1 && MEMF(0x11002C)==1 && MEMF(0x11003C)==1);
 free(memory);
 puts("PASS: six restored callbacks, retail plane transforms/bounds/translation, camera zero-distance path, setup/failure, preserved registers and stack");
}
