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


#include <sys/mman.h>
#include <pthread.h>
#include <time.h>
#include <sched.h>
#include "recomp/gen/recomp_funcs.h"
static void *gpu_consume(void *pc_pointer) {
 volatile uint32_t *pc=pc_pointer;struct timespec start,now;
 clock_gettime(CLOCK_MONOTONIC,&start);
 while(MEM32(0xFD003240)!=0x13000 || *pc!=0x197130) {
  clock_gettime(CLOCK_MONOTONIC,&now);assert(now.tv_sec-start.tv_sec<3);sched_yield();
 }
 assert(MEM32(0xFD003244)==0);
 MEM32(0xFD003244)=0x13000;return NULL;
}
int main(void) {
 const size_t size=0xFD900000ull;
 void *ram=mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS|MAP_NORESERVE,-1,0);
 assert(ram!=MAP_FAILED);g_xbox_mem_offset=(ptrdiff_t)ram;
 MEM32(0x1A50E8)=0x110000;MEM32(0x110000+0x17C0)=0xFD000000;
 MEM32(0x100000+0x17BC)=0x120000;MEM32(0x100000)=0x80013000;
 MEM32(0x120044)=0x77777;MEM32(0xFD400700)=0x1234;
 MEM32(0xFD100410)=0xA5000123;g_esp=0x180000;g_esi=0x54321;
 MEM32(g_esp)=0x98765;MEM32(g_esp+4)=0x100000;
 sub_00196FB0();
 assert(g_esp==0x180008 && g_esi==0x54321);
 assert(MEM32(0xFD100410)==0xA5000123 && MEM32(0x120040)==0x13000);
 assert(MEM32(0x120044)==0x77777 && MEM32(0xFD400700)==0x1234);
 MEM32(0xFD100414)=0x10000;recomp_nv2a_cache_flush(0xFD100414);
 assert(MEM32(0xFD100414)==0x10000);
 g_esp=0x180000;MEM32(g_esp)=0x98765;sub_001971F0();
 assert(g_esp==0x180004 && g_esi==0x54321 && MEM32(0xFD100410)==0xA5000123);
 assert(MEM32(0x120044)==0x77777 && MEM32(0xFD400700)==0x1234);
 MEM32(0x110000+0x4F8)=0xFD000000;MEM32(0xFD400700)=0;
 MEM32(0xFD003240)=MEM32(0xFD003244)=0x13000;
 g_esp=0x180000;MEM32(g_esp)=0x98765;MEM32(g_esp+4)=0x100000;
 sub_001970C0();assert(g_esp==0x180008 && g_esi==0x54321);
 assert(MEM32(0xFD100410)==0xA5000123 && MEM32(0x120044)==0x77777);
 MEM32(0x100000+0x17BC)=0xFD800000;
 MEM32(0xFD003240)=MEM32(0xFD003244)=0;
 g_esp=0x180000;MEM32(g_esp)=0x98765;MEM32(g_esp+4)=0x100000;
 g_recomp_pc=0;pthread_t gpu_thread;
 assert(!pthread_create(&gpu_thread,NULL,gpu_consume,(void*)&g_recomp_pc));
 sub_001970C0();assert(!pthread_join(gpu_thread,NULL));
 assert(g_esp==0x180008 && g_esi==0x54321 && MEM32(0xFD003244)==0x13000);
 MEM32(0xFD003244)=0x1234;MEM32(0xFD800044)=0x5678;MEM32(0xFD400700)=0xABC;
 recomp_nv2a_submit(0xFD800040,0x2468);
 assert(MEM32(0xFD003240)==0x2468 && MEM32(0xFD003244)==0x1234);
 assert(MEM32(0xFD800044)==0x5678 && MEM32(0xFD400700)==0xABC);
 recomp_nv2a_submit(0x120040,0x9999);assert(MEM32(0xFD003240)==0x2468);
 setenv("RECOMP_MESH_TRACE","1",1);
 for(unsigned i=0;i<40;i++)qpr2_mesh_args_record(i,0xDEADBEEF,1,2,3,4);
 puts("PASS: all three actual retail PFB flush sites/ABI and hardware submit/idle wait, other bits, DMA_GET and GPU status preserved; raw mesh ring accepts invalid object without reading it");
 munmap(ram,size);
}
