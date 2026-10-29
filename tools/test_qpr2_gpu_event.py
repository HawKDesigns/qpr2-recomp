"""Run actual GPU NOP dispatch, event bridge and registration code with pthread
Win32 event shims. No Windows SDK or game files required."""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1]
bridge=(root/'third_party/xboxrecomp/src/kernel/kernel_bridge.c').read_text()
mem=(root/'third_party/xboxrecomp/src/kernel/xbox_memory_layout.c').read_text()
def between(s,a,b):return s[s.index(a):s.index(b,s.index(a))]
shadow=between(bridge,'#define KE_SHADOW_SIZE','/* Insert a mapping')
lookup=between(bridge,'static HANDLE ke_shadow_lookup(uint32_t guest_va)\n{','/* Remove a mapping') if '/* Remove a mapping' in bridge else ''
if not lookup:
 a=bridge.index('static HANDLE ke_shadow_lookup(uint32_t guest_va)\n{');b=bridge.index('static void ke_shadow_remove',a);lookup=bridge[a:b]
events=between(bridge,'static HANDLE ke_guest_event(uint32_t guest_va, int *type)\n{','/* ═══════════════════════════════════════════════════════════')
fence_wait=events[events.index('int xbox_KeWaitGuestFence('):]
events=events[:events.index('/* A title can patch')]
wait=between(bridge,'static void bridge_KeWaitForSingleObject(void)\n{','/* ── NtWaitForSingleObject')
wait_trace=between(bridge,'#define BRIDGE_WAIT_SLOTS','/* ── KeSetEvent')
nt_wait=between(bridge,'static void bridge_NtWaitForSingleObjectEx(void)\n{','/* ── MmQueryAddressProtect')
command_fences=between(mem,'#define XBOX_MAX_COMMAND_FENCES','/* Retail QPR2 196D82')
registration=between(mem,'#define XBOX_MAX_SOFTWARE_EVENTS','/* A guest address is usable')
shim=r'''
#define XBOX_KERNEL_H /* Event APIs are supplied by this fixture. */
#define strtok_s strtok_r
#define XBOX_MEMORY_LAYOUT_H
#define XBOX_CONTIG_BASE 0x80000000u
#define XBOX_CONTIG_SIZE (64u*1024u*1024u)
#include <stdint.h>
int xbox_Nv2aWaitFenceChange(uint32_t,uint32_t,unsigned);
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <sched.h>
#include <time.h>
#include <errno.h>
typedef int32_t LONG;typedef unsigned char BOOLEAN;typedef void *HANDLE;
typedef uint32_t DWORD;
typedef union {int64_t QuadPart;} LARGE_INTEGER;
typedef pthread_rwlock_t SRWLOCK;
#define SRWLOCK_INIT PTHREAD_RWLOCK_INITIALIZER
static void AcquireSRWLockExclusive(SRWLOCK*p){assert(!pthread_rwlock_wrlock(p));}
static void ReleaseSRWLockExclusive(SRWLOCK*p){assert(!pthread_rwlock_unlock(p));}
static volatile LONG tick_extra;
static DWORD GetTickCount(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return (DWORD)((uint64_t)t.tv_sec*1000+t.tv_nsec/1000000+__atomic_load_n(&tick_extra,__ATOMIC_SEQ_CST));}
static DWORD GetCurrentThreadId(void){return (DWORD)(uintptr_t)pthread_self();}
__thread uint32_t g_xbox_kernel_caller;
extern __thread uint32_t g_seh_ebp;
typedef pthread_mutex_t CRITICAL_SECTION;
static LONG InterlockedCompareExchange(volatile LONG *p,LONG n,LONG old) {__atomic_compare_exchange_n(p,&old,n,0,__ATOMIC_SEQ_CST,__ATOMIC_SEQ_CST);return old;}
static LONG InterlockedExchange(volatile LONG *p,LONG n) {return __atomic_exchange_n(p,n,__ATOMIC_SEQ_CST);}
static LONG InterlockedIncrement(volatile LONG *p) {return __atomic_add_fetch(p,1,__ATOMIC_SEQ_CST);}
static volatile LONG initialized;
static void InitializeCriticalSection(CRITICAL_SECTION *p) {__atomic_add_fetch(&initialized,1,__ATOMIC_SEQ_CST);assert(!pthread_mutex_init(p,NULL));}
static void EnterCriticalSection(CRITICAL_SECTION *p) {assert(!pthread_mutex_lock(p));}
static void LeaveCriticalSection(CRITICAL_SECTION *p) {assert(!pthread_mutex_unlock(p));}
static void Sleep(unsigned ms) {(void)ms;sched_yield();}
static uint8_t ram[0x200000];ptrdiff_t g_xbox_mem_offset;static ptrdiff_t g_memory_offset;
static int bridge_va_mapped(uint32_t a,uint32_t n){return a>=0x10000 && a<=sizeof(ram)-n;}
__thread uint32_t g_esp,g_eax;
#define BRIDGE_MEM32(a) (*(volatile uint32_t*)(ram+(uint32_t)(a)))
#define STACK_ARG(n) BRIDGE_MEM32(g_esp+4+4*(n))
#define XBOX_TO_NATIVE(a) ((a)?ram+(a):NULL)
static HANDLE nt_handle;
static HANDLE bridge_resolve_handle(uint32_t token) {return token==0x48000004u?nt_handle:NULL;}
typedef struct {pthread_mutex_t mutex;pthread_cond_t cond;int signaled,manual;} Event;
static Event *made[4096];static unsigned nmade;static pthread_mutex_t made_lock=PTHREAD_MUTEX_INITIALIZER;
static volatile LONG waiting;
static volatile LONG timed_out;
static int legacy_probe;
static HANDLE CreateEventA(void *attr,int manual,int initial,const char *name) {
 (void)attr;(void)name;Event *e=calloc(1,sizeof *e);assert(e);
 pthread_mutex_init(&e->mutex,NULL);pthread_cond_init(&e->cond,NULL);e->manual=manual;e->signaled=initial;
 pthread_mutex_lock(&made_lock);assert(nmade<4096);made[nmade++]=e;pthread_mutex_unlock(&made_lock);return e;
}
static int ResetEvent(HANDLE h) {Event *e=h;pthread_mutex_lock(&e->mutex);e->signaled=0;pthread_mutex_unlock(&e->mutex);return 1;}
static int check_draw_order;
static int SetEvent(HANDLE h) {if(check_draw_order)assert(BRIDGE_MEM32(0x50000)==0xFF112233);Event *e=h;pthread_mutex_lock(&e->mutex);e->signaled=1;pthread_cond_broadcast(&e->cond);pthread_mutex_unlock(&e->mutex);return 1;}
static uint32_t xbox_KeWaitForSingleObject(HANDLE h,unsigned reason,unsigned mode,BOOLEAN alertable,void *timeout) {
 (void)reason;(void)mode;(void)alertable;
 int known=0;pthread_mutex_lock(&made_lock);for(unsigned i=0;i<nmade;i++)known|=made[i]==h;pthread_mutex_unlock(&made_lock);
 if(!known)return 0xC0000001;
 if(legacy_probe && !timeout) {
  assert(BRIDGE_MEM32(0x130200)==90 && BRIDGE_MEM32(0x111968)==0);
  puts("PASS: reproduced retail indefinite event wait despite completed requested fence and absent interrupt");exit(73);
 }
 Event *e=h;pthread_mutex_lock(&e->mutex);
 InterlockedExchange(&waiting,1);
 if(timeout && !*(int64_t*)timeout && !e->signaled){pthread_mutex_unlock(&e->mutex);return 0x102;}
 struct timespec deadline;clock_gettime(CLOCK_REALTIME,&deadline);
 int64_t ns=timeout?(-*(int64_t*)timeout)*100:2000000000LL;
 deadline.tv_sec+=ns/1000000000;deadline.tv_nsec+=ns%1000000000;
 if(deadline.tv_nsec>=1000000000){++deadline.tv_sec;deadline.tv_nsec-=1000000000;}
 while(!e->signaled) {int rc=pthread_cond_timedwait(&e->cond,&e->mutex,&deadline);
  if(rc==ETIMEDOUT && timeout){__atomic_add_fetch(&timed_out,1,__ATOMIC_SEQ_CST);pthread_mutex_unlock(&e->mutex);return 0x102;}assert(!rc);}
 if(!e->manual)e->signaled=0;
 pthread_mutex_unlock(&e->mutex);return 0;
}
#define __declspec(x) __thread
typedef unsigned KPROCESSOR_MODE;
static uint32_t xbox_NtWaitForSingleObjectEx(HANDLE h,KPROCESSOR_MODE mode,BOOLEAN alertable,void *timeout){return xbox_KeWaitForSingleObject(h,0,mode,alertable,timeout);}
static int fence_readable(uint32_t va,uint32_t bytes) {return va>=0x10000 && va<=sizeof(ram)-bytes;}
int xbox_KeSignalGuestEvent(uint32_t);
ptrdiff_t xbox_GetMemoryOffset(void) {return (ptrdiff_t)ram;}
uint32_t xbox_ContiguousAllocatedBytes(void) {return 0;}
uint32_t g_xbox_image_lo,g_xbox_image_hi;
void xbox_FramebufferWindowSet(uint32_t a,uint32_t b) {(void)a;(void)b;}
void xbox_FramebufferWindowStart(void) {}
void xbox_FramebufferWindowPresentPixels(const uint32_t *p,uint32_t w,uint32_t h) {(void)p;(void)w;(void)h;}
void xbox_FramebufferWindowFrameStats(uint32_t n) {(void)n;}
void xbox_Nv2aFrameCounterFlip(void) {}
'''
guest_body=between((root/'src/recomp/gen/recomp_0033.c').read_text(),
    'void sub_00196CF0(void)\n{','/**\n * sub_00196E20')
fence_wrapper=between((root/'src/recomp_manual.c').read_text(),
    'int recomp_nv2a_fence_wait(uint32_t device,uint32_t requested)\n{','/* Raw mesh getter')
fence_wrapper=fence_wrapper.replace('int recomp_nv2a_fence_wait(', 'static int production_fence_wait(')
guest=r'''
#define RECOMP_GENERATED_CODE
#include "recomp/gen/recomp_types.h"
#include "recomp/gen/recomp_funcs.h"
uint32_t g_xbox_code_lo=0x10000,g_xbox_code_hi=0x1C0000;
__thread uint32_t g_ecx,g_edx,g_ebx,g_esi,g_edi,g_ebp,g_seh_ebp;
__thread volatile uint32_t g_recomp_pc;
volatile uint32_t g_icall_trace[16],g_icall_trace_idx;volatile uint64_t g_icall_count;
static unsigned retail_waits;
static int late_patch;
static int late_consumed;
static int production_fence_wait(uint32_t,uint32_t);
int recomp_nv2a_fence_wait(uint32_t device,uint32_t requested) {
 if(legacy_probe)return 0;
 return production_fence_wait(device,requested);
}
static void kernel_wait(void) {++retail_waits;bridge_KeWaitForSingleObject();g_esp+=24;}
recomp_func_t recomp_lookup_manual(uint32_t va) {(void)va;return NULL;}
recomp_func_t recomp_lookup(uint32_t va) {(void)va;return NULL;}
recomp_func_t recomp_lookup_kernel(uint32_t va) {assert(va==0xFE000100);return kernel_wait;}
void recomp_icall_not_code_log(uint32_t va) {(void)va;assert(0);}
void recomp_icall_fail_log(uint32_t va) {(void)va;assert(0);}
void sub_00196AC0(void) {assert(0);}
void sub_00196BD0(void) {g_eax=0x130000;g_esp+=4;}
void sub_00196B70(void) {
 if(late_patch && !late_consumed++) {
  /* The initial retail fence check already saw 0. Consume the real
   * semaphore and placeholder NOP now, before retail patches NOP=5. */
  uint32_t record[]={0x401A4,8,0x41D6C,0,0x41D70,90,0x40100,0,
                     0x40110,0,0x40100,0};
  memcpy(ram+0x140000-24,record,sizeof record);
  setenv("RECOMP_PB_EXEC","1",1);
  nv2a_pb_scan(0x140000-24,0x140018);
  assert(BRIDGE_MEM32(0x130200)==90 && !BRIDGE_MEM32(0x111968));
 }
 g_eax=0x2400;g_esp+=4;
}
void sub_001971F0(void) {
 assert(BRIDGE_MEM32(0x140010)==0x40100 && BRIDGE_MEM32(0x140014)==5);
 if(!late_patch) {
  nv2a_pb_exec_method(0,0x1A4,8);nv2a_pb_exec_method(0,0x1D6C,0);
  nv2a_pb_exec_method(0,0x1D70,90);
  nv2a_pb_exec_method(0,0x100,BRIDGE_MEM32(0x140014));
 }
 g_esp+=4;
}
'''
fixture=r'''
static void *init_thread(void *arg) {(void)arg;ke_shadow_init();return NULL;}
static unsigned event_va=0x110000+0x1964;
static void args(uint32_t timeout) {
 g_esp=0x180000;BRIDGE_MEM32(g_esp+4)=event_va;BRIDGE_MEM32(g_esp+8)=6;
 BRIDGE_MEM32(g_esp+12)=1;BRIDGE_MEM32(g_esp+16)=0;BRIDGE_MEM32(g_esp+20)=timeout;
}
static void *wait_thread(void *unused) {
 (void)unused;args(0);bridge_KeWaitForSingleObject();assert(g_eax==0);return NULL;
}
static void *nt_wait_thread(void *unused){
 (void)unused;g_esp=0x180000;g_xbox_kernel_caller=0xC9B1D;g_seh_ebp=0x170100;
 BRIDGE_MEM32(g_esp+4)=0x48000004;BRIDGE_MEM32(g_esp+8)=1;
 BRIDGE_MEM32(g_esp+12)=BRIDGE_MEM32(g_esp+16)=0;
 BRIDGE_MEM32(g_seh_ebp+4)=0x123456;
 bridge_NtWaitForSingleObjectEx();assert(g_eax==0);return NULL;
}
static volatile LONG fence_returned;
static void *fence_wait_thread(void *unused) {
 (void)unused;assert(xbox_KeWaitGuestFence(event_va,0x11002C,0x130200,90)==1);
 InterlockedExchange(&fence_returned,1);return NULL;
}
int main(int argc,char **argv) {
 (void)argv;g_xbox_mem_offset=g_memory_offset=(ptrdiff_t)ram;
 BRIDGE_MEM32(0x1A50E8)=0x110000;BRIDGE_MEM32(event_va)=0x00040000;
 assert(xbox_Nv2aSoftwareEvent(0x1A50E8,0x1964,5)==0);
 if(argc>1 && !strcmp(argv[1],"disabled")) {
  setenv("RECOMP_TITLE_KEVENTS","0",1);args(0);bridge_KeWaitForSingleObject();assert(g_eax==0xC0000001);
  assert(!xbox_Nv2aHandleSoftwareMethod(0,5));
  puts("PASS: reproduce V37 invalid-handle retry when title events disabled");return 0;
 }
 setenv("RECOMP_TITLE_KEVENTS","1",1);
 assert(!xbox_Nv2aCommandFence(0x1A50E8,0x30,8,32));
 if(argc>1 && !strcmp(argv[1],"legacy-late")) {
  legacy_probe=late_patch=1;
  BRIDGE_MEM32(0x11002C)=100;BRIDGE_MEM32(0x110030)=0x130200;BRIDGE_MEM32(0x130200)=0;
  BRIDGE_MEM32(0x130004)=0x140000;BRIDGE_MEM32(0x1ADEB0)=0xFE000100;
  g_esp=0x180000;BRIDGE_MEM32(g_esp)=0x12345;BRIDGE_MEM32(g_esp+4)=90;BRIDGE_MEM32(g_esp+8)=0;
  sub_00196CF0();assert(0);
 }
 pthread_t initializers[20];
 for(unsigned i=0;i<20;i++)assert(!pthread_create(&initializers[i],NULL,init_thread,NULL));
 for(unsigned i=0;i<20;i++)assert(!pthread_join(initializers[i],NULL));
 assert(initialized==1 && g_ke_shadow_init==2);
 assert(!xbox_Nv2aHandleSoftwareMethod(1,5));assert(!xbox_Nv2aHandleSoftwareMethod(0,0));
 assert(!xbox_Nv2aHandleSoftwareMethod(0,4));assert(!BRIDGE_MEM32(event_va+4));
 /* Actual executor NOP dispatch signals before first wait; unrelated NOP zero doesn't. */
 nv2a_pb_exec_method(0,0x100,0);assert(!BRIDGE_MEM32(event_va+4));
 nv2a_pb_exec_method(0,0x100,5);assert(BRIDGE_MEM32(event_va+4)==1);
 args(0);bridge_KeWaitForSingleObject();assert(g_eax==0 && BRIDGE_MEM32(event_va+4)==1);
 /* Header reset must reset its existing host shadow: poll times out. */
 BRIDGE_MEM32(event_va+4)=0;BRIDGE_MEM32(0x170000)=BRIDGE_MEM32(0x170004)=0;
 args(0x170000);bridge_KeWaitForSingleObject();assert(g_eax==0x102);
 /* Already waiting is woken only at command consumption, not by registration. */
 pthread_t thread;InterlockedExchange(&waiting,0);assert(!pthread_create(&thread,NULL,wait_thread,NULL));
 while(!InterlockedCompareExchange(&waiting,1,1))sched_yield();
 InterlockedExchange(&tick_extra,2000);bridge_wait_dump();
 unsigned traced=0;for(unsigned i=0;i<BRIDGE_WAIT_SLOTS;i++)if(s_waits[i].active){
  assert(s_waits[i].ordinal==159 && s_waits[i].object==event_va && s_waits[i].reported && s_waits[i].stack[1]==event_va);++traced;
 }
 assert(traced==1);
 assert(!BRIDGE_MEM32(event_va+4));nv2a_pb_exec_method(0,0x100,5);
 assert(!pthread_join(thread,NULL));assert(BRIDGE_MEM32(event_va+4)==1);
 for(unsigned i=0;i<BRIDGE_WAIT_SLOTS;i++)assert(!s_waits[i].active);
 puts("PASS: outstanding wait retains ordinal, handle and guest stack before completion, then clears its diagnostic slot");
 /* The Nt wrapper was the V59 stall site. Its diagnostic caller must be the
  * saved return PC rather than arg0 (the handle token). */
 int type=0;nt_handle=ke_guest_event(event_va,&type);assert(nt_handle);ResetEvent(nt_handle);
 InterlockedExchange(&waiting,0);assert(!pthread_create(&thread,NULL,nt_wait_thread,NULL));
 while(!InterlockedCompareExchange(&waiting,1,1))sched_yield();
 InterlockedExchange(&tick_extra,4000);bridge_wait_dump();traced=0;
 for(unsigned i=0;i<BRIDGE_WAIT_SLOTS;i++)if(s_waits[i].active){
  assert(s_waits[i].ordinal==234 && s_waits[i].object==0x48000004 && s_waits[i].caller==0xC9B1D &&
   s_waits[i].frame==0x170100 && s_waits[i].saved_frame[1]==0x123456 && s_waits[i].reported);++traced;
 }
 assert(traced==1);SetEvent(nt_handle);assert(!pthread_join(thread,NULL));
 for(unsigned i=0;i<BRIDGE_WAIT_SLOTS;i++)assert(!s_waits[i].active);
 puts("PASS: actual NtWaitEx records the saved caller, handle and parent frame while blocked, without changing completion status");
 /* Actual pushbuffer decode consumes draws/NOP/draws in that order. */
 BRIDGE_MEM32(event_va+4)=0;args(0x170000);bridge_KeWaitForSingleObject();assert(g_eax==0x102);
 nv2a_pb_exec_method(0,NV097_SET_SURFACE_FORMAT,0x128);
 nv2a_pb_exec_method(0,NV097_SET_SURFACE_COLOR_OFFSET,0x50000);
 nv2a_pb_exec_method(0,NV097_SET_SURFACE_PITCH,32);
 nv2a_pb_exec_method(0,NV097_SET_SURFACE_CLIP_HORIZONTAL,8u<<16);
 nv2a_pb_exec_method(0,NV097_SET_SURFACE_CLIP_VERTICAL,8u<<16);
 uint32_t stream[]={0x40000|NV097_SET_COLOR_CLEAR_VALUE,0xFF112233,
  0x40000|NV097_CLEAR_SURFACE,NV097_CLEAR_COLOR_MASK,0x40100,5,
  0x40000|NV097_SET_COLOR_CLEAR_VALUE,0xFF445566,
  0x40000|NV097_CLEAR_SURFACE,NV097_CLEAR_COLOR_MASK};
 memcpy(ram+0x150000,stream,sizeof stream);assert(!BRIDGE_MEM32(event_va+4));
 setenv("RECOMP_PB_EXEC","1",1);check_draw_order=1;
 nv2a_pb_scan(0x150000,0x150000+sizeof stream);check_draw_order=0;
 assert(BRIDGE_MEM32(event_va+4)==1 && BRIDGE_MEM32(0x50000)==0xFF445566);
 /* New device follows the registration; malformed headers and missing devices are rejected. */
 BRIDGE_MEM32(0x1A50E8)=0;assert(!xbox_Nv2aHandleSoftwareMethod(0,5));
 BRIDGE_MEM32(0x1A50E8)=0x120000;BRIDGE_MEM32(0x121964)=0x00040000;
 nv2a_pb_exec_method(0,0x100,5);assert(BRIDGE_MEM32(0x121968)==1 && nmade==2);
 BRIDGE_MEM32(0x121964)=0;BRIDGE_MEM32(0x121968)=0;assert(!xbox_Nv2aHandleSoftwareMethod(0,5));
 assert(!BRIDGE_MEM32(0x121968));
 /* Retail BlockOnTime takes its event branch and returns with original ABI. */
 BRIDGE_MEM32(0x1A50E8)=0x110000;BRIDGE_MEM32(event_va+4)=0;
 BRIDGE_MEM32(0x11002C)=100;BRIDGE_MEM32(0x110030)=0x130200;BRIDGE_MEM32(0x130200)=0;
 BRIDGE_MEM32(0x130004)=0x140000;BRIDGE_MEM32(0x1ADEB0)=0xFE000100;
 g_esp=0x180000;BRIDGE_MEM32(g_esp)=0x12345;BRIDGE_MEM32(g_esp+4)=90;BRIDGE_MEM32(g_esp+8)=0;
 g_esi=91;g_edi=92;g_ebx=93;g_ebp=g_seh_ebp=94;
 sub_00196CF0();assert(retail_waits==0 && g_esp==0x18000C && g_esi==91 && g_edi==92 && g_ebx==93);
 puts("PASS: actual retail BlockOnTime event branch and RET8/register ABI");
 /* GPU consumes the placeholder and completes between the retail entry
  * check and the CPU interrupt patch. No NOP=5 arrives; real completion is
  * sufficient, without setting any guest event or changing the counter. */
 late_patch=1;BRIDGE_MEM32(0x130200)=0;BRIDGE_MEM32(event_va+4)=0;
 g_esp=0x180000;BRIDGE_MEM32(g_esp)=0x12345;BRIDGE_MEM32(g_esp+4)=90;BRIDGE_MEM32(g_esp+8)=0;
 g_esi=91;g_edi=92;g_ebx=93;g_ebp=g_seh_ebp=94;
 sub_00196CF0();assert(retail_waits==0 && g_esp==0x18000C && g_esi==91 && g_edi==92 && g_ebx==93);
 assert(BRIDGE_MEM32(0x130200)==90 && BRIDGE_MEM32(event_va+4)==0);
 puts("PASS: actual retail late interrupt patch returns on an already completed fence, without inventing an event");
 /* Force completion AFTER native waiting starts, still without an event.
  * Also inject an unrelated notification: it must not release the fence. */
 BRIDGE_MEM32(0x130200)=0;BRIDGE_MEM32(event_va+4)=0;
 InterlockedExchange(&waiting,0);assert(!pthread_create(&thread,NULL,fence_wait_thread,NULL));
 for(;;){nv_fence_signal_lock(&g_command_fence_signal);unsigned n=g_command_fence_signal.sleepers;
  nv_fence_signal_unlock(&g_command_fence_signal);if(n)break;sched_yield();}
 assert(!InterlockedCompareExchange(&fence_returned,0,0));
 xbox_KeSignalGuestEvent(event_va);
 for(unsigned i=0;i<10000;i++)sched_yield();
 assert(!InterlockedCompareExchange(&fence_returned,0,0));
 BRIDGE_MEM32(event_va+4)=0;assert(xbox_Nv2aReleaseFence(8,0,89));
 for(unsigned i=0;i<10000;i++)sched_yield();assert(!InterlockedCompareExchange(&fence_returned,0,0));
 assert(xbox_Nv2aReleaseFence(8,0,90));
 assert(!pthread_join(thread,NULL));assert(fence_returned && BRIDGE_MEM32(0x130200)==90);
 /* Unsigned wrap, already completed, and invalid input guards. */
 BRIDGE_MEM32(0x11002C)=2;BRIDGE_MEM32(0x130200)=1;
 assert(xbox_KeWaitGuestFence(event_va,0x11002C,0x130200,0xFFFFFFFFu)==1);
 assert(!xbox_KeWaitGuestFence(event_va,0x11002C,0x130201,0));
 assert(!xbox_KeWaitGuestFence(0,0x11002C,0x130200,0));
 assert(!production_fence_wait(0,0) && !production_fence_wait(0xFFFFFFFFu,0));
 assert(!xbox_Nv2aWaitFenceChange(0,0,2) && !xbox_Nv2aWaitFenceChange(0x130201,0,2));
 puts("PASS: direct completion wake rejects unrelated events and incomplete fence notifications; missed interrupt, wrap and invalid address guards hold");
 puts("PASS: actual GPU NOP dispatch, title-built event shadow, early/late signal, guest reset, notification persistence, device replacement and invalid state guards");
}
'''
def run_tests():
 with tempfile.TemporaryDirectory() as d:
  source=Path(d)/'test.c';binary=Path(d)/'test'
  renderer='#include "'+str(root/'third_party/xboxrecomp/src/kernel/nv2a_pb_exec.c')+'"\n'
  scan='#include \"'+str(root/'third_party/xboxrecomp/src/kernel/nv2a_pb_scan.c')+'\"\n'
  source.write_text(shim+shadow+lookup+wait_trace+events+fence_wait+wait+nt_wait+command_fences+registration+renderer+scan+guest+fence_wrapper+guest_body+fixture)
  subprocess.run(['gcc','-std=gnu11','-O1','-ffunction-sections','-fdata-sections','-fsanitize=undefined','-I'+str(root/'src'),'-Werror=implicit-function-declaration','-I'+str(root/'third_party/xboxrecomp/src'),'-I'+str(root/'third_party/xboxrecomp/include'),str(source),str(root/'third_party/xboxrecomp/src/kernel/nv2a_vsh_interp.c'),str(root/'third_party/xboxrecomp/src/kernel/nv2a_combiner.c'),'-Wl,--gc-sections','-pthread','-lm','-o',str(binary)],check=True)
  subprocess.run([str(binary),'disabled'],check=True,timeout=10)
  assert subprocess.run([str(binary),'legacy-late'],timeout=10).returncode==73
  subprocess.run([str(binary)],check=True,timeout=10)

if __name__=='__main__':run_tests()
