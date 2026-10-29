#define RECOMP_GENERATED_CODE
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

#include "qpr2_ui_trace.h"

recomp_func_t recomp_lookup_manual(uint32_t va) { (void)va; return NULL; }
recomp_func_t recomp_lookup_kernel(uint32_t va) { (void)va; return NULL; }
static void good_call(void) { assert(MEM32(g_esp)==0xA256B); g_esp += 12; }
static void short_return(void) { g_esp += 8; }
static void changed_saved(void) { g_esi=0xA256B; g_esp += 12; }
recomp_func_t recomp_lookup(uint32_t va) { assert(va==0x150100); return good_call; }
void recomp_icall_not_code_log(uint32_t va) { (void)va; assert(0); }
void recomp_icall_fail_log(uint32_t va) { (void)va; assert(0); }
static void reset(void) { g_esp=0x180000; g_ebx=123; g_esi=456; g_edi=789; }
int main(void) {
 void *memory=calloc(1,0x200000); assert(memory); g_xbox_mem_offset=(ptrdiff_t)memory;
 reset(); QPR2_LOADER_CALL(0x150100u, good_call, 0xA256Bu, 8u);
 assert(g_esp==0x180008 && g_esi==456 && g_ebx==123 && g_edi==789);
 reset(); QPR2_LOADER_CALL(0x150200u, short_return, 0xA256Bu, 8u);
 assert(g_esp==0x180004); /* Observation must not repair the wrong return. */
 reset(); QPR2_LOADER_CALL(0x150300u, changed_saved, 0xA256Bu, 8u);
 assert(g_esp==0x180008 && g_esi==0xA256B);
 reset(); g_esp-=8; PUSH32(esp,0xA256Bu);
 QPR2_LOADER_ICALL(0x150100u,0x180000u,0xA2566u);
 assert(g_esp==0x180000 && g_esi==456);
 MEM32(0x100000)=0x1B0D08;MEM32(0x100074)=0x110000;MEM32(0x110000)=0x1B0DF0;
 qpr2_academy_register(0,0x100000);
 uint8_t snapshot[0x78];memcpy(snapshot,(void*)(g_xbox_mem_offset+0x100000),sizeof snapshot);
 uint32_t old_s=g_esi,old_sp=g_esp;
 qpr2_ui_trace(1,0x10F45C,0x4CF00,0x100000,0,0,g_esp,g_ebx,g_esi,g_edi);
 qpr2_ui_trace(2,0x10F45C,0x4CF00,0x100000,0,0,g_esp,g_ebx,g_esi,g_edi);
 assert(g_esi==old_s && g_esp==old_sp && !memcmp(snapshot,(void*)(g_xbox_mem_offset+0x100000),sizeof snapshot));
 qpr2_ui_trace_dump(); free(memory);
 puts("PASS: loader direct/indirect cleanup and observational stack/register diagnostics");
}
