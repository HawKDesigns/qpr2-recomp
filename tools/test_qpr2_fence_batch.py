"""Production FIFO -> vertex/clip executor -> D3D backend -> guest fences.

The strict D3D mock executes the shared pixel program. Completion addresses,
memory hazards, actual readbacks and publication are production code. This is
not a Windows hardware or gameplay benchmark.
"""
from pathlib import Path
import ast,subprocess,tempfile
root=Path(__file__).resolve().parents[1];k=root/'third_party/xboxrecomp/src/kernel'
tree=ast.parse((root/'tools/test_qpr2_gpu_device.py').read_text())
api=next(ast.literal_eval(n.value) for n in tree.body if isinstance(n,ast.Assign) and any(isinstance(t,ast.Name) and t.id=='api' for t in n.targets))
# Deterministic GPU/executor clock: latency expiry is tested explicitly.
api=api.replace('static void QueryPerformanceCounter(LARGE_INTEGER*p){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);p->QuadPart=(int64_t)t.tv_sec*1000000000+t.tv_nsec;}',
    'static int64_t fixture_qpc_ns=1000000000;\nstatic void QueryPerformanceCounter(LARGE_INTEGER*p){p->QuadPart=fixture_qpc_ns;}')
api=api.replace('static LONG InterlockedExchange(volatile LONG*p,LONG n){',
    'static void completion_probe(volatile LONG*,LONG);\nstatic LONG InterlockedExchange(volatile LONG*p,LONG n){completion_probe(p,n);')
mock=next(ast.literal_eval(n.value) for n in tree.body if isinstance(n,ast.Assign) and any(isinstance(t,ast.Name) and t.id=='fixture' for t in n.targets))
mock=mock[:mock.index('int main(int argc,char**argv)')]
memory=(k/'xbox_memory_layout.c').read_text()
def between(s,a,b):return s[s.index(a):s.index(b,s.index(a))]
fences=between(memory,'#define XBOX_MAX_COMMAND_FENCES','/* Retail QPR2 196D82')
reader=between(memory,'int xbox_Nv2aFifoRead(uint32_t va,uint32_t *word)\n{','/*\n * Two counters inside the device')
ack_body=between(memory,'                if(fifo_ring_bounds(&ring_lo,&ring_hi)) {','\n            } else if (put != last_put')
import re
ack_defines='\n'.join(re.findall(r'^#define (?:NV2A_USER_DMA_GET|NV2A_PFIFO_DMA_GET|NV2A_PFIFO_DMA_SUBROUTINE) .*$',memory,re.M))
ack_shim=ack_defines+'\nstatic void fixture_ack_iteration(void *regs,uint32_t put){uint32_t ring_lo,ring_hi;DWORD now_ms=0;\n'+ack_body+'\n}\n'
shim=r'''
#define XBOX_KERNEL_H
#define XBOX_MEMORY_LAYOUT_H
#define XBOX_CONTIG_BASE 0x80000000u
#define XBOX_CONTIG_SIZE 0x04000000u
#define NV2A_ENABLE_GPU_RASTER 1
#define __declspec(x) __thread
#define FALSE 0
#define strtok_s strtok_r
#define INFINITE 0xFFFFFFFFu
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
typedef struct {unsigned dwNumberOfProcessors;} SYSTEM_INFO;
static void GetSystemInfo(SYSTEM_INFO*p){p->dwNumberOfProcessors=1;}
static LONG InterlockedDecrement(volatile LONG*p){return __atomic_sub_fetch(p,1,__ATOMIC_SEQ_CST);}
static HANDLE CreateEventA(void*a,int b,int c,const char*d){(void)a;(void)b;(void)c;(void)d;assert(0);return NULL;}
static int SetEvent(HANDLE h){(void)h;assert(0);return 0;}
static DWORD WaitForSingleObject(HANDLE h,DWORD t){(void)h;(void)t;assert(0);return 0;}
static DWORD GetTickCount(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return (DWORD)((uint64_t)t.tv_sec*1000+t.tv_nsec/1000000);}
static uint8_t *ram;static ptrdiff_t g_memory_offset;
uint32_t g_xbox_image_lo,g_xbox_image_hi;
#define MEM(a) (*(volatile uint32_t*)(ram+(uint32_t)(a)))
ptrdiff_t xbox_GetMemoryOffset(void){return (ptrdiff_t)ram;}
uint32_t xbox_ContiguousAllocatedBytes(void){return XBOX_CONTIG_SIZE;}
void xbox_FramebufferWindowSet(uint32_t a,uint32_t b){(void)a;(void)b;}
void xbox_FramebufferWindowStart(void){}
void xbox_FramebufferWindowPresentPixels(const uint32_t*p,uint32_t w,uint32_t h){(void)p;(void)w;(void)h;}
void xbox_FramebufferWindowFrameStats(uint32_t n){(void)n;}
void xbox_Nv2aFrameCounterFlip(void){}
static int fence_readable(uint32_t a,uint32_t n){return a>=0x10000 && ((uint64_t)a+n<=0x4000000ull || (a>=0x80000000 && (uint64_t)a+n<=0x84000000ull));}
static unsigned events;
static void check_event_order(void);
int xbox_Nv2aHandleSoftwareMethod(uint32_t s,uint32_t v){assert(!s && v==5);check_event_order();++events;return 1;}
/* This legacy suite deliberately has no registered software events. The
 * event-batching suite below exercises production registration and dispatch. */
int xbox_Nv2aResolveSoftwareEvent(uint32_t s,uint32_t v,uint32_t*a){(void)s;(void)v;(void)a;return 0;}
int xbox_Nv2aReleaseResolvedSoftwareEvent(uint32_t s,uint32_t v,uint32_t a){(void)s;(void)v;(void)a;return 0;}
int xbox_Nv2aResolveFence(uint32_t,uint32_t,uint32_t*);
int xbox_Nv2aReleaseResolvedFence(uint32_t,uint32_t,uint32_t,uint32_t);
int xbox_Nv2aReleaseFence(uint32_t,uint32_t,uint32_t);
'''
bridge=(k/'kernel_bridge.c').read_text()
wait_body=between(bridge,'int xbox_KeWaitGuestFence(', '/* ═══════════════════════════════════════════════════════════')
# Exercise the actual completion-wait loop together with FIFO batching and
# D3D completion, using a native event shim which receives no GPU interrupt.
wait_shim=r'''
#include <errno.h>
#include <sched.h>
static LONG InterlockedIncrement(volatile LONG*p){return __atomic_add_fetch(p,1,__ATOMIC_SEQ_CST);}
#define BRIDGE_MEM32(a) MEM(a)
static int bridge_va_mapped(uint32_t a,uint32_t n){return fence_readable(a,n);}
static pthread_mutex_t completion_lock=PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t completion_cond=PTHREAD_COND_INITIALIZER;
static LONG completion_signaled,completion_timeouts,completion_waits,completion_returned;
static HANDLE ke_guest_event(uint32_t a,int*type){*type=1;return a==0x180000?(HANDLE)&completion_signaled:NULL;}
static int ResetEvent(HANDLE e){assert(e==&completion_signaled);InterlockedExchange(&completion_signaled,0);return 1;}
static int fixture_SetEvent(HANDLE e){assert(e==&completion_signaled);pthread_mutex_lock(&completion_lock);
 InterlockedExchange(&completion_signaled,1);pthread_cond_broadcast(&completion_cond);pthread_mutex_unlock(&completion_lock);return 1;}
#define SetEvent fixture_SetEvent
static int bridge_wait_begin(unsigned o,uint32_t a,HANDLE e,unsigned t){(void)o;(void)a;(void)e;(void)t;return 0;}
static void bridge_wait_end(int slot){assert(slot==0);}
static void Sleep(unsigned ms){struct timespec t={ms/1000,(ms%1000)*1000000};nanosleep(&t,NULL);}
static uint32_t xbox_KeWaitForSingleObject(HANDLE e,unsigned r,unsigned m,unsigned a,void*raw){
 (void)r;(void)m;(void)a;assert(e==&completion_signaled && raw);
 assert(((LARGE_INTEGER*)raw)->QuadPart==-20000);
 struct timespec deadline;clock_gettime(CLOCK_REALTIME,&deadline);deadline.tv_nsec+=2000000;
 if(deadline.tv_nsec>=1000000000){++deadline.tv_sec;deadline.tv_nsec-=1000000000;}
 pthread_mutex_lock(&completion_lock);InterlockedIncrement(&completion_waits);
 while(!InterlockedCompareExchange(&completion_signaled,0,0)){
  int rc=pthread_cond_timedwait(&completion_cond,&completion_lock,&deadline);
  if(rc==ETIMEDOUT){InterlockedIncrement(&completion_timeouts);pthread_mutex_unlock(&completion_lock);return 0x102;}
  assert(!rc);
 }
 InterlockedExchange(&completion_signaled,0);pthread_mutex_unlock(&completion_lock);return 0;
}
'''
fixture=r'''
static unsigned long fixture_fence_waits(void){nv_fence_signal_lock(&g_command_fence_signal);unsigned long n=g_command_fence_signal.waits;nv_fence_signal_unlock(&g_command_fence_signal);return n;}
static unsigned long fixture_fence_timeouts(void){nv_fence_signal_lock(&g_command_fence_signal);
 unsigned long n=g_command_fence_signal.timeouts;nv_fence_signal_unlock(&g_command_fence_signal);return n;}
static void *wait_batched_completion(void *unused){(void)unused;
 assert(xbox_KeWaitGuestFence(0x180000,0x180010,0x80070000,12)==1);
 InterlockedExchange(&completion_returned,1);return NULL;
}
static uint32_t release_values[1024];static unsigned release_count;static int check_first_pixel;
static void completion_probe(volatile LONG *p,LONG value){
 if(ram && (uintptr_t)p>=(uintptr_t)ram && (uintptr_t)p<(uintptr_t)ram+0x84000000ull){
  assert(!g.active && !g.pending_count && !g.queued_count && !g.readback_failed);
  if(check_first_pixel)assert(MEM(0x80050000)==0xFFFFFFFF);
  assert(release_count<1024);release_values[release_count++]=(uint32_t)value;
 }
}
static void check_event_order(void){assert(!g.active && !s_pending_fence_count && MEM(0x80070000)==42);}
static uint32_t stream[100000],used;
static void method(uint32_t m,uint32_t p){stream[used++]=0x40000|m;stream[used++]=p;}
static void draw(void){method(NV097_SET_BEGIN_END,NV_PRIM_TRIANGLES);method(NV097_DRAW_ARRAYS,2u<<24);method(NV097_SET_BEGIN_END,0);}
static void semaphore(uint32_t v){method(0x1D70,v);}
static void execute(void){
 const uint32_t lo=0x80010000,hi=lo+0x40000;memcpy(ram+lo,stream,used*4);
 memset(&s_fifo,0,sizeof s_fifo);uint32_t put=lo+used*4,get=lo;
 while(get!=put){uint32_t next=nv2a_pb_execute_fifo(get,put,lo,hi);assert(next!=get);get=next;}
 assert(!s_pending_fence_count && !s_fifo_scope);used=0;
}
static void setup(int batch){
 gpu_finish_named("fixture-reset");assert(!s_pending_fence_count);
 memset(&s_gpu,0,sizeof s_gpu);memset(ram+0x80050000,0,16*16*4);
 MEM(0x110030)=0x80070000;for(unsigned i=0;i<32;i+=4)MEM(0x80070000+i)=0;
 s_fence_batch_enabled=batch;s_fence_deferred=s_fence_groups=s_fence_hazards=s_fence_peak=0;
 s_gpu.format=0x128;s_gpu.color_offset=0x50000;s_gpu.pitch=64;s_gpu.clip_w=s_gpu.clip_h=16;
 s_gpu.color_mask=0x01010101;s_gpu.xf_mode=2;s_gpu.prim=NV_PRIM_TRIANGLES;
 s_gpu.depth_test=s_gpu.depth_mask=1;s_gpu.depth_func=0x203;s_gpu.zeta_offset=0x123;
 float *depth=zbuf_current(1);assert(depth);for(unsigned i=0;i<NV_ZBUF_W*NV_ZBUF_H;i++)depth[i]=50;
 s_gpu.semaphore_context=8;s_gpu.semaphore_offset=0;s_gpu.zpass_enable=1;
 s_gpu.attr[0].offset=0x100000;s_gpu.attr[0].type=2;s_gpu.attr[0].size=4;s_gpu.attr[0].stride=16;
 float positions[3][4]={{0,0,20,1},{16,0,20,1},{0,16,20,1}};memcpy(ram+0x100000,positions,sizeof positions);
 uint32_t pos[4]={0,0x0020001b,0x08000000,0x0000f800};
 uint32_t color[4]={0,0x0020001b,0x0c000000,0x0000f819};float white[4]={1,1,1,1};
 nv2a_vsh_set_instruction(0,pos);nv2a_vsh_set_instruction(1,color);nv2a_vsh_set_constant(0,white);
 nv2a_vsh_set_start_slot(0);nv2a_vsh_set_cxt_write(0);used=0;
}
static void completed(uint32_t *pixels){gpu_finish_named("fixture-check");memcpy(pixels,ram+0x80050000,16*16*4);}
int main(int argc,char **argv){
 size_t bytes=0x84000000ull;ram=mmap(NULL,bytes,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS|MAP_NORESERVE,-1,0);assert(ram!=MAP_FAILED);g_memory_offset=(ptrdiff_t)ram;
 int pipeline=argc>1 && !strncmp(argv[1],"pipeline",8);
 if(pipeline && !strcmp(argv[1],"pipeline-default"))unsetenv("RECOMP_GPU_SLICE_PIPELINE");
 else setenv("RECOMP_GPU_SLICE_PIPELINE",pipeline?"1":"0",1);
 int cpu=argc>1 && !strcmp(argv[1],"cpu");
 int compute=argc>1 && (!strcmp(argv[1],"compute") || !strcmp(argv[1],"pipeline-compute"));
 int control=cpu || (argc>1 && !strcmp(argv[1],"disabled"));
 int default_gate=argc>1 && !strcmp(argv[1],"default");
 setenv("RECOMP_GPU_DIAGNOSTIC_COUNTERS",argc>1 && (!strcmp(argv[1],"quiet") || !strcmp(argv[1],"pipeline-quiet"))?"0":"1",1);
 setenv("RECOMP_GPU_RASTER",cpu?"0":"1",1);setenv("RECOMP_GPU_ASYNC","0",1);setenv("RECOMP_RASTER_THREADS","1",1);
 if(default_gate)unsetenv("RECOMP_GPU_FENCE_BATCH");
 else setenv("RECOMP_GPU_FENCE_BATCH",control?"0":"1",1);
 setenv("RECOMP_RENDER_TIMING","1",1);native_supported=!compute;coverage_tier=3;
 assert(nv_gpu_available()==!cpu);if(!cpu)assert(g.native==!compute);MEM(0x130000)=0x110000;
 assert(!xbox_Nv2aCommandFence(0x130000,0x30,8,32));
 assert(nv2a_pb_exec_fifo_begin()==(4096u));nv2a_pb_exec_fifo_end();
 if(pipeline){
  assert(s_slice_pipeline_enabled==1);
  const uint32_t lo=0x80010000,hi=lo+0x40000;
  uint8_t *regs=calloc(1,0x800100);assert(regs);
  assert(!xbox_Nv2aPushBufferRing(0x130000,0x24,0x28));
  MEM(0x110024)=lo;MEM(0x110028)=hi;
  uint32_t reference[256],actual[256];float expected_depth[256];uint32_t expected_zpass=0;
  uint64_t reads[2];
  for(int on=0;on<2;on++){
   setup(1);s_slice_pipeline_enabled=on;release_count=0;uint64_t before=g.readbacks;
   for(unsigned slice=0;slice<4;slice++){
    draw();semaphore(slice+1);
    while(used<(slice+1)*4096)stream[used++]=0; /* legal empty method headers */
   }
   memcpy(ram+lo,stream,used*4);memset(&s_fifo,0,sizeof s_fifo);
   uint32_t put=lo+used*4,get=lo;
   for(unsigned slice=0;slice<4;slice++){
    *(uint32_t*)(regs+NV2A_USER_DMA_GET)=get&0x0FFFFFFF;
    fixture_ack_iteration(regs,put&0x0FFFFFFF);
    uint32_t next=(*(uint32_t*)(regs+NV2A_USER_DMA_GET))|0x80000000;
    assert(*(uint32_t*)(regs+NV2A_PFIFO_DMA_GET)==(next&0x0FFFFFFF));
    assert(next==get+4096*4);get=next;
    if(on && slice<3){
     assert(MEM(0x80070000)==0 && s_pending_fence_count==slice+1 && s_fence_carried_slices==slice+1);
     assert(g.readbacks==before && g.queued_count==0 && g.active);
    }else assert(MEM(0x80070000)==slice+1 && !s_pending_fence_count);
   }
   reads[on]=g.readbacks-before;assert(release_count==4);
   for(unsigned i=0;i<4;i++)assert(release_values[i]==i+1);
   completed(on?actual:reference);
   float *z=zbuf_current(0);
   if(!on){for(unsigned y=0;y<16;y++)for(unsigned x=0;x<16;x++)expected_depth[y*16+x]=z[y*NV_ZBUF_W+x];expected_zpass=s_gpu.zpass_count;}
   else {for(unsigned y=0;y<16;y++)for(unsigned x=0;x<16;x++)assert(expected_depth[y*16+x]==z[y*NV_ZBUF_W+x]);assert(s_gpu.zpass_count==expected_zpass);}
  }
  assert(reads[0]==4 && reads[1]==1 && !memcmp(reference,actual,sizeof actual));
  puts("PASS: actual ACK worker publishes both GET mirrors every 4096 words; four completion waits become one, with exact pixels/depth/visibility and ordered releases");
  s_slice_pipeline_enabled=1;
  /* An endless submitted stream cannot starve completion: limit 16 slices.
   * Use a ring disjoint from all output/vertex/fence fixture memory. */
  {
   const uint32_t wide_lo=0x80080000,wide_hi=wide_lo+0x80000;
   setup(1);uint64_t before=g.readbacks;release_count=0;
   for(unsigned slice=0;slice<20;slice++){
    draw();semaphore(slice+1);while(used<(slice+1)*4096)stream[used++]=0;
   }
   memcpy(ram+wide_lo,stream,used*4);memset(&s_fifo,0,sizeof s_fifo);
   MEM(0x110024)=wide_lo;MEM(0x110028)=wide_hi;uint32_t get=wide_lo,put=get+used*4;
   for(unsigned slice=0;slice<20;slice++){
    *(uint32_t*)(regs+NV2A_USER_DMA_GET)=get&0x0FFFFFFF;
    fixture_ack_iteration(regs,put&0x0FFFFFFF);
    get=(*(uint32_t*)(regs+NV2A_USER_DMA_GET))|0x80000000;
    assert(get==wide_lo+(slice+1)*4096*4);
    assert(*(uint32_t*)(regs+NV2A_PFIFO_DMA_GET)==(get&0x0FFFFFFF));
    if(slice<15)assert(!MEM(0x80070000) && s_pending_fence_count==slice+1);
    else if(slice<19)assert(MEM(0x80070000)==16 && s_pending_fence_count==slice-15);
    else assert(MEM(0x80070000)==20 && !s_pending_fence_count);
   }
   assert(g.readbacks==before+2 && release_count==20);
   for(unsigned i=0;i<20;i++)assert(release_values[i]==i+1);
   MEM(0x110024)=lo;MEM(0x110028)=hi;
  }
  /* The 8 ms deadline drains sooner even when unread PUT data remains. */
  setup(1);release_count=0;uint64_t timed_reads=g.readbacks;
  for(unsigned slice=0;slice<3;slice++){draw();semaphore(slice+1);while(used<(slice+1)*4096)stream[used++]=0;}
  memcpy(ram+lo,stream,used*4);memset(&s_fifo,0,sizeof s_fifo);
  uint32_t timed_put=lo+used*4,timed_get=nv2a_pb_execute_fifo(lo,timed_put,lo,hi);
  assert(timed_get==lo+4096*4 && !MEM(0x80070000));
  fixture_qpc_ns+=9000000;
  timed_get=nv2a_pb_execute_fifo(timed_get,timed_put,lo,hi);
  assert(timed_get==lo+8192*4 && timed_get!=timed_put && MEM(0x80070000)==2 && g.readbacks==timed_reads+1 && !s_pending_fence_count);
  nv2a_pb_execute_fifo(timed_get,timed_put,lo,hi);assert(MEM(0x80070000)==3 && g.readbacks==timed_reads+2);
  puts("PASS: 20 slices require 2 readbacks; both GET mirrors advance every 4096 words; 8 ms deadline forces earlier completion with unread commands");
  /* V121's 16 ms budget carries through the first 9 ms boundary, then
   * completes before the remaining PUT data after 17 ms. The 16-slice cap
   * and every dependency barrier remain unchanged. */
  setup(1);s_fence_carry_ms=16.0;release_count=0;timed_reads=g.readbacks;
  for(unsigned slice=0;slice<4;slice++){draw();semaphore(slice+1);while(used<(slice+1)*4096)stream[used++]=0;}
  memcpy(ram+lo,stream,used*4);memset(&s_fifo,0,sizeof s_fifo);
  timed_put=lo+used*4;timed_get=nv2a_pb_execute_fifo(lo,timed_put,lo,hi);
  assert(timed_get==lo+4096*4 && !MEM(0x80070000));
  fixture_qpc_ns+=9000000;
  timed_get=nv2a_pb_execute_fifo(timed_get,timed_put,lo,hi);
  assert(timed_get==lo+8192*4 && !MEM(0x80070000) && s_pending_fence_count==2);
  fixture_qpc_ns+=8000000;
  timed_get=nv2a_pb_execute_fifo(timed_get,timed_put,lo,hi);
  assert(timed_get==lo+12288*4 && MEM(0x80070000)==3 && g.readbacks==timed_reads+1);
  nv2a_pb_execute_fifo(timed_get,timed_put,lo,hi);
  assert(MEM(0x80070000)==4 && g.readbacks==timed_reads+2);
  s_fence_carry_ms=8.0;
  puts("PASS: 16 ms carry delays the first 9 ms boundary, expires by 17 ms, and publishes every fence in order");
  /* Every mandatory boundary must end a carried group, without a later poll. */
  for(unsigned boundary=0;boundary<9;boundary++){
   setup(1);uint32_t value=boundary==2?42:12;draw();semaphore(value);
   while(used<4096)stream[used++]=0;
   if(boundary==0)method(NV097_SET_COLOR_CLEAR_VALUE,0x12345678); /* then worker goes idle */
   if(boundary==1)stream[used++]=0x60000003; /* decoder error */
   if(boundary==2)method(0x100,5); /* software event */
   if(boundary==3)method(0x17D0,0x72000); /* visibility report */
   if(boundary==4){draw();} /* vertex hazard */
   if(boundary==5){draw();} /* target hazard */
   if(boundary==6){draw();}
   if(boundary==7)stream[used++]=0; /* ring bounds reset/idle */
   if(boundary==8){method(NV097_SET_COLOR_CLEAR_VALUE,123);method(NV097_SET_BEGIN_END,0);} /* finite continuation */
   memcpy(ram+lo,stream,used*4);memset(&s_fifo,0,sizeof s_fifo);uint32_t put=lo+used*4;
   assert(nv2a_pb_execute_fifo(lo,put,lo,hi)==lo+4096*4 && !MEM(0x80070000) && s_pending_fence_count);
   if(boundary==4)s_gpu.attr[0].offset=0x80070000;
   if(boundary==5)s_gpu.color_offset=0x70000;
   if(boundary==6){s_gpu.texs[0].offset=0x80070000;s_gpu.texs[0].color=0x12;s_gpu.texs[0].width=s_gpu.texs[0].height=1;s_gpu.texs[0].pitch=4;s_gpu.texs[0].valid=1;}
   if(boundary==0 || boundary==7){
    MEM(0x110028)=boundary==7?lo:hi;
    *(uint32_t*)(regs+NV2A_USER_DMA_GET)=(lo+4096*4)&0x0FFFFFFF;
    fixture_ack_iteration(regs,(lo+4096*4)&0x0FFFFFFF);
    MEM(0x110028)=hi;
   }
   else nv2a_pb_execute_fifo(lo+4096*4,put,lo,hi);
   assert(MEM(0x80070000)==value && !s_pending_fence_count && !s_fence_carried_slices);
  }
  /* A command word that aliases a carried fence sees its completed value. */
  setup(1);draw();semaphore(0xFF123456);while(used<4096)stream[used++]=0;
  method(NV097_SET_COLOR_CLEAR_VALUE,0);memcpy(ram+lo,stream,used*4);memset(&s_fifo,0,sizeof s_fifo);
  assert(nv2a_pb_execute_fifo(lo,lo+used*4,lo,hi)==lo+4096*4 && !MEM(0x80070000));
  nv2a_pb_exec_guest_access(0xF0070000,4);assert(MEM(0x80070000)==0xFF123456 && !s_pending_fence_count);
  /* CALL bodies may exceed one slice and live outside the pushbuffer ring. */
  setup(1);draw();execute();memset(&s_fifo,0,sizeof s_fifo);
  const uint32_t sub=0x80060000;for(unsigned i=0;i<8192;i++)MEM(sub+i*4)=0;
  MEM(lo)=0x00060002;MEM(lo+4)=0x41D70;MEM(lo+8)=12;
  MEM(sub)=0x41D70;MEM(sub+4)=11;MEM(sub+8191*4)=0x00020000;
  uint32_t call_get=nv2a_pb_execute_fifo(lo,lo+12,lo,hi);
  assert(call_get==sub+4095*4 && s_fifo.in_call && !MEM(0x80070000));
  call_get=nv2a_pb_execute_fifo(call_get,lo+12,lo,hi);
  assert(call_get==sub+8191*4 && s_fifo.in_call && !MEM(0x80070000) && s_pending_fence_count==1);
  assert(nv2a_pb_execute_fifo(call_get,lo+12,lo,hi)==lo+12 && !s_fifo.in_call && MEM(0x80070000)==12);
  /* A waiter may enter after GET publication and before a continuation slice.
   * Completion notification, rather than an interrupt or timeout, releases it. */
  setup(1);MEM(0x180000)=0x00040001;MEM(0x180004)=0;MEM(0x180010)=100;completion_returned=0;
  draw();semaphore(12);while(used<4096)stream[used++]=0;method(NV097_SET_COLOR_CLEAR_VALUE,9);
  memcpy(ram+lo,stream,used*4);memset(&s_fifo,0,sizeof s_fifo);
  uint32_t next=nv2a_pb_execute_fifo(lo,lo+used*4,lo,hi);assert(next==lo+4096*4 && !MEM(0x80070000));
  pthread_t waiter;unsigned long waits=fixture_fence_waits();
  assert(!pthread_create(&waiter,NULL,wait_batched_completion,NULL));
  while(fixture_fence_waits()==waits)sched_yield();
  assert(!completion_returned);nv2a_pb_execute_fifo(next,lo+used*4,lo,hi);
  assert(!pthread_join(waiter,NULL) && completion_returned && MEM(0x80070000)==12);
  /* Map failure on the final boundary cannot publish either pending fence. */
  setup(1);draw();semaphore(99);while(used<4096)stream[used++]=0;method(NV097_SET_COLOR_CLEAR_VALUE,7);
  memcpy(ram+lo,stream,used*4);memset(&s_fifo,0,sizeof s_fifo);next=nv2a_pb_execute_fifo(lo,lo+used*4,lo,hi);
  assert(!MEM(0x80070000));fflush(NULL);pid_t pid=fork();assert(pid>=0);
  if(!pid){fail_next_map=1;nv2a_pb_execute_fifo(next,lo+used*4,lo,hi);_exit(9);}
  int status;assert(waitpid(pid,&status,0)==pid && WIFEXITED(status) && WEXITSTATUS(status)==4 && !MEM(0x80070000));
  nv2a_pb_fifo_idle();assert(MEM(0x80070000)==99);
  /* Upload failure during submit-only continuation also preserves the fence. */
  setup(1);draw();semaphore(101);while(used<4096)stream[used++]=0;method(NV097_SET_COLOR_CLEAR_VALUE,8);
  memcpy(ram+lo,stream,used*4);memset(&s_fifo,0,sizeof s_fifo);fflush(NULL);pid=fork();assert(pid>=0);
  if(!pid){g.triangles.bytes=0;for(unsigned slot=0;slot<NV_GPU_UPLOAD_SLOTS-1;slot++)g.upload_spare[slot].triangles.bytes=0;target_create_fail_bytes=sizeof(NvGpuTri);nv2a_pb_execute_fifo(lo,lo+used*4,lo,hi);_exit(9);}
  assert(waitpid(pid,&status,0)==pid && WIFEXITED(status) && WEXITSTATUS(status)==4 && !MEM(0x80070000));
  puts("PASS: carried groups drain on idle/error/events/reports/vertex/texture/target hazards; long CALL bodies progress; real waiter wakes on completed pixels; submission/readback failures never publish completion");
  return 0;
 }
 if(control){
  setup(0);for(unsigned i=0;i<3;i++){draw();semaphore(i+1);}uint64_t before=g.readbacks;execute();
  assert(MEM(0x80070000)==3 && !s_fence_deferred && g.readbacks-before==(cpu?0u:3u));
  assert(MEM(0x80050000)==0xFFFFFFFF);puts("PASS: explicit opt-out and CPU-unavailable gate retain immediate completion and 4096-word budget");return 0;
 }
 /* The real V62 wait must survive the missed-interrupt case with batching
  * enabled: no fence may satisfy it until the actual D3D readback completes. */
 setup(1);MEM(0x180000)=0x00040001;MEM(0x180004)=0;MEM(0x180010)=100;
 for(unsigned i=0;i<12;i++){draw();semaphore(i+1);}
 pthread_t waiter;assert(!pthread_create(&waiter,NULL,wait_batched_completion,NULL));
 while(fixture_fence_timeouts()<2)sched_yield();
 assert(!InterlockedCompareExchange(&completion_returned,0,0) && !MEM(0x80070000));
 /* A separate notification cannot stand in for completed guest work. */
 MEM(0x180004)=1;SetEvent(&completion_signaled);
 unsigned long prior_timeouts=fixture_fence_timeouts();
 while(fixture_fence_timeouts()<prior_timeouts+2)sched_yield();
 assert(!InterlockedCompareExchange(&completion_returned,0,0));
 unsigned long notify_timeouts=fixture_fence_timeouts();
 uint64_t wait_reads=g.readbacks;execute();
 assert(!pthread_join(waiter,NULL) && completion_returned && MEM(0x80070000)==12 && g.readbacks==wait_reads+1);
 assert(fixture_fence_timeouts()==notify_timeouts);
 unsigned long waits_before=g_command_fence_signal.waits;
 assert(xbox_KeWaitGuestFence(0x180000,0x180010,0x80070000,12)==1);
 assert(g_command_fence_signal.waits==waits_before);
 puts("PASS: actual completed-fence wait + batched FIFO/D3D readback survives absent/unrelated interrupt, already-completed entry and coarse native timeout without an extra tick");
 uint32_t reference[256],actual[256];uint64_t reads[2],counts[2][3];float reference_depth[256];
 check_first_pixel=1;release_count=0;
 for(int batch=0;batch<2;batch++){
  setup(batch);uint64_t before=g.readbacks;
  for(unsigned i=0;i<12;i++){draw();semaphore(i+1);}
  execute();reads[batch]=g.readbacks-before;assert(MEM(0x80070000)==12);
  completed(batch?actual:reference);
  counts[batch][0]=s_gpu.pixels;counts[batch][1]=s_gpu.zpass_count;counts[batch][2]=s_gpu.xf_depth_fail;
  float *z=zbuf_current(0);for(unsigned y=0;y<16;y++)for(unsigned x=0;x<16;x++){
   if(batch)assert(reference_depth[y*16+x]==z[y*NV_ZBUF_W+x]);else reference_depth[y*16+x]=z[y*NV_ZBUF_W+x];
  }
 }
 assert(reads[0]==12 && reads[1]==1 && !memcmp(reference,actual,sizeof actual) && !memcmp(counts[0],counts[1],sizeof counts[0]));
 assert(actual[0]==0xFFFFFFFF && s_fence_deferred==12 && s_fence_groups==1);
 assert(release_count==24);for(unsigned i=0;i<24;i++)assert(release_values[i]==i%12+1);check_first_pixel=0;
 printf("PASS: production FIFO/vertex/clip/D3D/readback/fence chain: 12 -> 1 readbacks with exact pixels, depth, counts and final fence\n");
 /* Aliased vertex data must see a pending write, before transformation. */
 for(int batch=0;batch<2;batch++){
  setup(batch);MEM(0x110030)=0x80070010;s_gpu.attr[0].offset=0x80070000;
  float positions[3][4]={{0,0,20,1},{4,0,20,1},{0,16,20,1}};memcpy(ram+0x80070000,positions,sizeof positions);
  draw();semaphore(0x41800000);draw();execute();completed(batch?actual:reference);
 }
 assert(!memcmp(reference,actual,sizeof actual) && s_fence_hazards);
 /* Raw and palette texture reads see the preceding semaphore write. */
 for(unsigned palette=0;palette<2;palette++)for(int batch=0;batch<2;batch++){
  setup(batch);s_gpu.texs[0].offset=palette?0x80071000:0x80070000;
  s_gpu.texs[0].palette=0x80070000;s_gpu.texs[0].color=palette?0x0B:0x12;
  s_gpu.texs[0].width=s_gpu.texs[0].height=1;s_gpu.texs[0].pitch=4;s_gpu.texs[0].valid=1;
  MEM(0x80071000)=0;MEM(0x80070000)=0xFFFF0000;
  draw();semaphore(0xFF00FF00);draw();execute();completed(batch?actual:reference);
  if(batch)assert(!memcmp(reference,actual,sizeof actual) && actual[0]==0xFF00FF00 && s_fence_hazards);
 }
 /* Render-target and clear aliases cannot reorder stores after later draws. */
 for(unsigned clear=0;clear<2;clear++)for(int batch=0;batch<2;batch++){
  setup(batch);s_gpu.color_offset=0x70000;
  draw();semaphore(0xFF123456);
  if(clear){method(NV097_SET_COLOR_CLEAR_VALUE,0xFFAABBCC);method(NV097_CLEAR_SURFACE,NV097_CLEAR_COLOR_MASK);}else draw();
  execute();gpu_finish_named("alias-check");assert(MEM(0x80070000)==(clear?0xFFAABBCC:0xFFFFFFFF));
  if(batch)assert(s_fence_hazards);
 }
 /* A future pushbuffer parameter aliases a queued fence destination. */
 setup(1);MEM(0x110030)=0x8001000C;semaphore(0xFF123456);method(NV097_SET_COLOR_CLEAR_VALUE,0x11111111);execute();
 assert(s_gpu.clear_color==0xFF123456 && s_fence_hazards==1);
 /* Software event drains preceding fences. Decoder error also drains before
  * returning GET, but never consumes the bad or following command. */
 setup(1);draw();semaphore(42);method(0x100,5);execute();assert(events && MEM(0x80070000)==42);
 /* Visibility reset/report may split a batch; preserve counts and publish
  * preceding fences before the report's CPU memory is written. */
 uint32_t report_counts[2];
 for(int batch=0;batch<2;batch++){
  setup(batch);draw();semaphore(21);method(0x17C8,0);draw();method(0x17D0,0x72000);execute();
  assert(MEM(0x80070000)==21);report_counts[batch]=MEM(0x80072008);
 }
 assert(report_counts[0] && report_counts[0]==report_counts[1]);
 setup(1);draw();semaphore(43);stream[used++]=0x60000003;
 memcpy(ram+0x80010000,stream,used*4);memset(&s_fifo,0,sizeof s_fifo);
 uint32_t stop=nv2a_pb_execute_fifo(0x80010000,0x80010000+used*4,0x80010000,0x80050000);
 assert(stop==0x80010000+(used-1)*4 && MEM(0x80070000)==43 && !s_pending_fence_count);used=0;
 /* Batch capacity and 4096-word budget yields drain without losing a release. */
 setup(1);for(unsigned i=0;i<140;i++){draw();semaphore(i+1);}execute();assert(MEM(0x80070000)==140 && s_fence_peak==64 && s_fence_groups==3);
 setup(1);draw();semaphore(1);for(unsigned i=0;i<9000;i++)method(NV097_SET_COLOR_CLEAR_VALUE,i);draw();semaphore(55);
 memcpy(ram+0x80010000,stream,used*4);memset(&s_fifo,0,sizeof s_fifo);
 uint32_t slice_put=0x80010000+used*4;
 uint32_t slice_get=nv2a_pb_execute_fifo(0x80010000,slice_put,0x80010000,0x80050000);
 assert(slice_get==0x80010000+4096*4 && slice_get!=slice_put && MEM(0x80070000)==1 && !s_pending_fence_count);
 while(slice_get!=slice_put){uint32_t next=nv2a_pb_execute_fifo(slice_get,slice_put,0x80010000,0x80050000);assert(next!=slice_get);slice_get=next;}
 assert(MEM(0x80070000)==55 && s_fence_groups==2);used=0;
 puts("PASS: actual scanner yields after 4096 words and publishes completed batched fences before returning GET");
 /* Real wrap/CALL/RETURN and a partially published semaphore packet. */
 setup(1);draw();execute();memset(&s_fifo,0,sizeof s_fifo);uint64_t before=g.readbacks;
 MEM(0x800100E0)=0x41D70;MEM(0x800100E4)=1;MEM(0x800100E8)=0x20010020;
 MEM(0x80010020)=0x41D70;MEM(0x80010024)=2;
 assert(nv2a_pb_execute_fifo(0x800100E0,0x80010028,0x80010000,0x80010100)==0x80010028);
 assert(MEM(0x80070000)==2 && g.readbacks==before+1 && s_fence_groups==1);
 setup(1);draw();execute();memset(&s_fifo,0,sizeof s_fifo);before=g.readbacks;
 MEM(0x80010000)=0x00030002;MEM(0x80010004)=0x41D70;MEM(0x80010008)=2;
 MEM(0x80030000)=0x41D70;MEM(0x80030004)=1;MEM(0x80030008)=0x00020000;
 assert(nv2a_pb_execute_fifo(0x80010000,0x8001000C,0x80010000,0x80010100)==0x8001000C);
 assert(MEM(0x80070000)==2 && g.readbacks==before+1 && !s_fifo.in_call);
 setup(1);draw();execute();memset(&s_fifo,0,sizeof s_fifo);before=g.readbacks;
 MEM(0x80010000)=0x41D70;MEM(0x80010004)=3;
 assert(nv2a_pb_execute_fifo(0x80010000,0x80010004,0x80010000,0x80010100)==0x80010004 && s_fifo.remaining==1 && !MEM(0x80070000));
 assert(nv2a_pb_execute_fifo(0x80010004,0x80010008,0x80010000,0x80010100)==0x80010008 && !s_fifo.remaining);
 assert(MEM(0x80070000)==3 && g.readbacks==before+1);
 /* Unsupported main-RAM destinations remain immediate. */
 setup(1);MEM(0x110030)=0x120000;draw();semaphore(71);execute();assert(MEM(0x120000)==71 && !s_fence_deferred);
 /* Destination is resolved at command consumption, even if device metadata
  * changes before this bounded batch completes. */
 setup(1);assert(nv2a_pb_exec_fifo_begin()==4096);nv2a_pb_exec_method(0,0x1D70,81);MEM(0x110030)=0x80071000;nv2a_pb_exec_fifo_end();
 assert(MEM(0x80070000)==81 && MEM(0x80071000)==0);
 /* Even the context's guest pointer chain may alias an earlier fence. */
 setup(1);g_command_fences[0].device_ptr_va=0x80073000;MEM(0x80073000)=0x110000;
 MEM(0x110030)=0x80073000;MEM(0x111030)=0x80074000;MEM(0x80074000)=0;
 nv2a_pb_exec_fifo_begin();nv2a_pb_exec_method(0,0x1D70,0x111000);nv2a_pb_exec_method(0,0x1D70,91);nv2a_pb_exec_fifo_end();
 assert(MEM(0x80073000)==0x111000 && MEM(0x80074000)==91 && s_fence_hazards);
 g_command_fences[0].device_ptr_va=0x130000;MEM(0x130000)=0x110000;
 setup(1);MEM(0x130000)=0x80073000;MEM(0x80073030)=0x80073030;MEM(0x80074000)=0;
 nv2a_pb_exec_fifo_begin();nv2a_pb_exec_method(0,0x1D70,0x80074000);nv2a_pb_exec_method(0,0x1D70,92);nv2a_pb_exec_fifo_end();
 assert(MEM(0x80073030)==0x80074000 && MEM(0x80074000)==92 && s_fence_hazards);MEM(0x130000)=0x110000;
 /* Tiled aliases and overflowing ranges cannot bypass the pending write. */
 setup(1);nv2a_pb_exec_fifo_begin();nv2a_pb_exec_method(0,0x1D70,83);
 nv2a_pb_exec_guest_access(0xF0070000,4);assert(MEM(0x80070000)==83 && s_fence_hazards==1);nv2a_pb_exec_fifo_end();
 setup(1);nv2a_pb_exec_fifo_begin();nv2a_pb_exec_method(0,0x1D70,84);
 nv2a_pb_exec_guest_access(UINT64_MAX-3,8);assert(MEM(0x80070000)==84);nv2a_pb_exec_fifo_end();
 setup(1);nv2a_pb_exec_fifo_begin();nv2a_pb_exec_fifo_begin();nv2a_pb_exec_method(0,0x1D70,85);
 nv2a_pb_exec_fifo_end();assert(!MEM(0x80070000) && s_pending_fence_count==1);nv2a_pb_exec_fifo_end();assert(MEM(0x80070000)==85);
 puts("PASS: vertex/raw/palette/target/clear/PB memory hazards, events, decoder errors, capacity/budget drains, captured destination and conservative fallback");
 /* A runtime readback failure cannot release an accepted guest fence. */
 setup(1);draw();execute();assert(g.active);fflush(NULL);pid_t pid=fork();assert(pid>=0);
 if(!pid){nv2a_pb_exec_fifo_begin();nv2a_pb_exec_method(0,0x1D70,99);fail_next_map=1;nv2a_pb_exec_fifo_end();_exit(9);}
 int status;assert(waitpid(pid,&status,0)==pid && WIFEXITED(status) && WEXITSTATUS(status)==4 && MEM(0x80070000)==0);
 gpu_finish_named("parent-completion");puts("PASS: accepted GPU readback failure exits without publishing a false fence");
 munmap(ram,bytes);
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-fence-batch-') as d:
 p=Path(d);(p/'logs').mkdir()
 for n in ['windows.h','d3d11.h','d3d11_3.h','d3dcompiler.h','dxgi.h']:(p/n).write_text('/* fixture API */\n')
 c=p/'test.c';exe=p/'test'
 includes=''.join('#include "'+str(k/n)+'"\n' for n in ['nv2a_gpu_raster.c','nv2a_pb_exec.c','nv2a_pb_scan.c'])
 c.write_text(api+shim+fences+reader+wait_shim+wait_body+includes+mock+ack_shim+fixture)
 subprocess.run(['gcc','-std=gnu11','-O1','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-Werror=implicit-function-declaration','-Werror=incompatible-pointer-types','-ffunction-sections','-fdata-sections','-I'+str(p),'-I'+str(root/'tools'),'-I'+str(k),'-I'+str(root/'third_party/xboxrecomp/src'),str(c),str(k/'nv2a_gpu_tiles.c'),str(k/'nv2a_vsh_interp.c'),str(k/'nv2a_combiner.c'),'-Wl,--gc-sections','-pthread','-lm','-o',str(exe)],check=True)
 import sys
 modes=['native','compute','disabled','cpu','default','quiet','pipeline','pipeline-compute','pipeline-quiet','pipeline-default']
 if len(sys.argv)>1:modes=[m for m in modes if m in sys.argv[1:]]
 assert modes
 for mode in modes:
  print('RUN fence '+mode,flush=True)
  subprocess.run([str(exe),*([mode] if mode!='native' else [])],cwd=p,check=True,timeout=90)
 print(f'PASS: all {len(modes)} completion modes passed')
