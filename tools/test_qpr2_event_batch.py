"""Production registered events + FIFO + renderer + D3D readback ordering.

The D3D API shim executes scalar pixel arithmetic. It proves structural
readback reduction and guest publication order, not Windows race FPS.
"""
from pathlib import Path
import re
import subprocess
import sys
import tempfile

root = Path(__file__).resolve().parents[1]
fence_test = root / 'tools/test_qpr2_fence_batch.py'
source = fence_test.read_text()
ns = {'__file__': str(fence_test)}
exec(compile(source[:source.index('with tempfile.TemporaryDirectory(')], str(fence_test), 'exec'), ns)
k = root / 'third_party/xboxrecomp/src/kernel'
memory = (k / 'xbox_memory_layout.c').read_text()
registration = memory[memory.index('#define XBOX_MAX_SOFTWARE_EVENTS'):memory.index('/* A guest address is usable')]
shim = ns['shim']
shim = re.sub(r'int xbox_Nv2aHandleSoftwareMethod\([^\n]+\n', '', shim)
shim = re.sub(r'int xbox_Nv2aResolveSoftwareEvent\([^\n]+\n', '', shim)
shim = re.sub(r'int xbox_Nv2aReleaseResolvedSoftwareEvent\([^\n]+\n', '', shim)
shim += '\nstatic int xbox_KeSignalGuestEvent(uint32_t);\nstatic void publication_note(uint32_t);\n'
head = ns['fixture'][:ns['fixture'].index('int main(int argc,char **argv)')]
head = head.replace('release_values[release_count++]=(uint32_t)value;',
                    'release_values[release_count++]=(uint32_t)value;publication_note((uint32_t)value);')
fixture = r'''
static uint32_t publications[8192];static unsigned published;
static pthread_mutex_t event_lock=PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t event_cond=PTHREAD_COND_INITIALIZER;
static volatile LONG waiter_ready,waiter_returned;
static void publication_note(uint32_t v){assert(published<8192);publications[published++]=v;}
static int xbox_KeSignalGuestEvent(uint32_t address){
 assert(!g.active && !g.pending_count && !g.queued_count && !g.readback_failed);
 if(MEM(address)!=0x00040000u)return 0;
 if(check_first_pixel)assert(MEM(0x80050000)==0xFFFFFFFF);
 publication_note(0xE0000000u|address);++events;
 pthread_mutex_lock(&event_lock);MEM(address+4)=1;pthread_cond_broadcast(&event_cond);pthread_mutex_unlock(&event_lock);return 1;
}
static const uint32_t event_at=0x111964;
static void*event_waiter(void*unused){
 (void)unused;pthread_mutex_lock(&event_lock);InterlockedExchange(&waiter_ready,1);
 while(!MEM(event_at+4))pthread_cond_wait(&event_cond,&event_lock);
 assert(MEM(0x80050000)==0xFFFFFFFF && MEM(0x80070000)==42);
 pthread_mutex_unlock(&event_lock);InterlockedExchange(&waiter_returned,1);return NULL;
}
static void event_reset(void){
 MEM(0x130000)=0x110000;setup(1);
 assert(!s_pending_event_count && !s_pending_completion_count);
 MEM(event_at)=0x00040000;MEM(event_at+4)=MEM(event_at+8)=MEM(event_at+12)=0;
 MEM(0x121964)=0x00040000;MEM(0x121968)=0;
 events=release_count=published=0;check_first_pixel=1;
}
static void immediate_draw(void){
 nv2a_pb_exec_method(0,NV097_SET_BEGIN_END,NV_PRIM_TRIANGLES);
 nv2a_pb_exec_method(0,NV097_DRAW_ARRAYS,2u<<24);
 nv2a_pb_exec_method(0,NV097_SET_BEGIN_END,0);
}
static void event(void){nv2a_pb_exec_method(0,0x0100,5);}
static void queued_event_draw(void){nv2a_pb_exec_fifo_begin();immediate_draw();event();}
int main(int argc,char**argv){
 const char*mode=argc>1?argv[1]:"native";
 int cpu=!strcmp(mode,"cpu"),compute=!strcmp(mode,"compute"),disabled=!strcmp(mode,"disabled");
 ram=mmap(NULL,0x84000000ull,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS|MAP_NORESERVE,-1,0);
 assert(ram!=MAP_FAILED);g_memory_offset=(ptrdiff_t)ram;
 setenv("RECOMP_GPU_RASTER",cpu?"0":"1",1);setenv("RECOMP_GPU_ASYNC","0",1);setenv("RECOMP_RASTER_THREADS","1",1);
 setenv("RECOMP_RENDER_TIMING","1",1);setenv("RECOMP_GPU_SLICE_PIPELINE","1",1);
 setenv("RECOMP_GPU_DIAGNOSTIC_COUNTERS",!strcmp(mode,"quiet")?"0":"1",1);
 if(disabled)setenv("RECOMP_GPU_EVENT_BATCH","0",1);else unsetenv("RECOMP_GPU_EVENT_BATCH");
 native_supported=!compute;coverage_tier=3;assert(nv_gpu_available()==!cpu);
 if(!cpu)assert(g.native==!compute);
 MEM(0x130000)=0x110000;assert(!xbox_Nv2aCommandFence(0x130000,0x30,8,32));
 assert(!xbox_Nv2aSoftwareEvent(0x130000,0x1964,5));
 nv2a_pb_exec_fifo_begin();nv2a_pb_exec_fifo_end();assert(s_event_batch_enabled==!disabled);
 /* An empty-work event is immediate, even in a continuing FIFO scope. */
 event_reset();check_first_pixel=0;uint64_t before=g.readbacks;
 nv2a_pb_exec_fifo_begin();event();assert(events==1 && !s_pending_completion_count && g.readbacks==before);
 nv2a_pb_exec_fifo_end();check_first_pixel=1;
 puts("PASS: notifications without accepted GPU work incur no extra batching latency");
 if(!strcmp(mode,"unregistered")){
  event_reset();g_software_event_count=0;draw();semaphore(42);method(0x100,5);execute();
  assert(!events && !s_pending_completion_count && MEM(0x80070000)==42);
  puts("PASS: unknown/unregistered software commands retain conservative completion and unhandled behavior");return 0;
 }
 uint32_t pixels[2][256];float depth[256];uint32_t zpass=0;uint64_t reads[2];
 for(unsigned on=0;on<2;++on){
  event_reset();s_event_batch_enabled=disabled?0:(int)on;before=g.readbacks;
  for(unsigned i=0;i<12;++i){draw();semaphore(i+1);method(0x100,5);}
  execute();assert(events==12 && release_count==12 && published==24);
  assert(!s_pending_event_count && !s_pending_completion_count && MEM(0x80070000)==12 && MEM(event_at+4)==1);
  for(unsigned i=0;i<12;++i){assert(publications[i*2]==i+1);assert(publications[i*2+1]==(0xE0000000u|event_at));}
  reads[on]=g.readbacks-before;completed(pixels[on]);float*z=zbuf_current(0);
  if(!on){for(unsigned y=0;y<16;++y)memcpy(depth+y*16,z+y*NV_ZBUF_W,64);zpass=s_gpu.zpass_count;}
  else {for(unsigned y=0;y<16;++y)assert(!memcmp(depth+y*16,z+y*NV_ZBUF_W,64));assert(zpass==s_gpu.zpass_count);}
 }
 assert(!memcmp(pixels[0],pixels[1],sizeof pixels[0]));
 if(!cpu)assert(reads[0]==12 && reads[1]==(disabled?12:1));else assert(!reads[0]&&!reads[1]);
 printf("PASS: 12 interleaved fence/event/draw requests: %llu -> %llu real readbacks, exact pixels/depth/visibility, exact F1/E/F2/E publication order; mode=%s\n",(unsigned long long)reads[0],(unsigned long long)reads[1],mode);
 if(cpu || disabled)return 0;
 s_event_batch_enabled=1;
 /* Actual ACK worker / command decoder: events alone must retain the 4096
  * word GET quantum and submit immutable draws before each ring advance. */
 event_reset();before=g.readbacks;
 const uint32_t lo=0x80010000,hi=lo+0x40000;
 assert(!xbox_Nv2aPushBufferRing(0x130000,0x24,0x28));MEM(0x110024)=lo;MEM(0x110028)=hi;
 for(unsigned i=0;i<4;++i){draw();method(0x100,5);while(used<(i+1)*4096)stream[used++]=0;}
 memcpy(ram+lo,stream,used*4);memset(&s_fifo,0,sizeof s_fifo);uint32_t get=lo,put=lo+used*4;
 uint8_t*regs=calloc(1,0x800100);assert(regs);
 for(unsigned i=0;i<4;++i){
  *(uint32_t*)(regs+NV2A_USER_DMA_GET)=get&0x0FFFFFFF;
  fixture_ack_iteration(regs,put&0x0FFFFFFF);
  get=(*(uint32_t*)(regs+NV2A_USER_DMA_GET))|0x80000000;
  assert(get==lo+(i+1)*4096*4 && *(uint32_t*)(regs+NV2A_PFIFO_DMA_GET)==(get&0x0FFFFFFF));
  assert(!g.queued_count);
  if(i<3)assert(!events && s_pending_event_count==i+1 && g.readbacks==before);
 }
 assert(events==4 && g.readbacks==before+1 && !s_pending_completion_count);free(regs);used=0;
 puts("PASS: actual ACK/FIFO event-only stream: both GET mirrors advance every 4096 words, four slices share one readback, and input submits precede GET");
 /* A real waiting host thread cannot see the event before completed pixels
  * and its preceding semaphore; idle notification wakes it without a tick. */
 event_reset();waiter_ready=waiter_returned=0;pthread_t waiting_thread;
 assert(!pthread_create(&waiting_thread,NULL,event_waiter,NULL));
 while(!InterlockedCompareExchange(&waiter_ready,0,0))sched_yield();
 nv2a_pb_exec_fifo_begin();immediate_draw();nv2a_pb_exec_method(0,0x1D70,42);event();
 nv2a_pb_exec_fifo_end_continue(1);assert(!InterlockedCompareExchange(&waiter_returned,0,0));
 nv2a_pb_exec_fifo_idle();assert(!pthread_join(waiting_thread,NULL) && waiter_returned);
 puts("PASS: a blocked event consumer wakes only after real readback and its earlier fence, without waiting for another FIFO tick");
 event_reset();assert(!xbox_Nv2aSoftwareEvent(0x130000,0x2964,6));MEM(0x112964)=0x00040000;MEM(0x112968)=0;
 queued_event_draw();nv2a_pb_exec_method(0,0x1D70,7);nv2a_pb_exec_method(0,0x100,6);nv2a_pb_exec_fifo_end();
 assert(published==3 && publications[0]==(0xE0000000u|event_at) && publications[1]==7 && publications[2]==0xE0112964u);
 assert(MEM(event_at+4)==1 && MEM(0x112968)==1);g_software_event_count=1;
 /* Visibility reset/report remains authoritative across a delayed event. */
 uint32_t expected_report=0;
 for(unsigned on=0;on<2;++on){
  event_reset();s_event_batch_enabled=on;queued_event_draw();nv2a_pb_exec_method(0,0x17C8,0);
  immediate_draw();event();nv2a_pb_exec_method(0,0x17D0,0x72000);
  assert(events==2 && !s_pending_completion_count && MEM(0x8007200C)==0);
  if(!on)expected_report=MEM(0x80072008);else assert(MEM(0x80072008)==expected_report);
  assert(expected_report>0);nv2a_pb_exec_fifo_end();
 }
 s_event_batch_enabled=1;
 /* Event-only queues also submit immutable input before yielding GET. */
 event_reset();before=g.readbacks;queued_event_draw();assert(s_pending_event_count==1 && !events);
 nv2a_pb_exec_fifo_end_continue(1);assert(!s_fifo_scope && s_pending_event_count==1 && !g.queued_count && g.readbacks==before);
 nv2a_pb_exec_fifo_idle();assert(events==1 && g.readbacks==before+1 && !s_pending_completion_count);
 /* Same lifetime, without pipeline carry, completes at the first slice. */
 event_reset();s_slice_pipeline_enabled=0;queued_event_draw();nv2a_pb_exec_fifo_end_continue(1);assert(events==1 && !s_pending_completion_count);s_slice_pipeline_enabled=1;
 /* Capacity is shared with fences, so the total cannot exceed 64 records. */
 event_reset();before=g.readbacks;nv2a_pb_exec_fifo_begin();
 for(unsigned i=0;i<70;++i){immediate_draw();event();assert(s_pending_completion_count<=64);}
 assert(events==64 && s_pending_event_count==6);nv2a_pb_exec_fifo_end();assert(events==70 && g.readbacks==before+2);
 event_reset();before=g.readbacks;
 for(unsigned i=0;i<20;++i){queued_event_draw();nv2a_pb_exec_fifo_end_continue(i<19);
  if(i<15)assert(!events);else if(i<19)assert(events==16);}
 assert(events==20 && g.readbacks==before+2 && !s_pending_completion_count);
 event_reset();queued_event_draw();nv2a_pb_exec_fifo_end_continue(1);assert(!events);
 fixture_qpc_ns+=9000000;queued_event_draw();nv2a_pb_exec_fifo_end_continue(1);assert(events==2 && !s_pending_completion_count);
 puts("PASS: event-only idle, no-pipeline, shared 64-record capacity, 16-slice limit and 8 ms CPU carry deadline force real completion");
 /* Independent target snapshots may remain queued, but every snapshot must
  * publish before either event. Pre-create the second CPU depth-cache slot. */
 event_reset();s_gpu.zeta_offset=0x124;float*other_z=zbuf_current(1);
 for(unsigned i=0;i<NV_ZBUF_W*NV_ZBUF_H;++i)other_z[i]=50;
 s_gpu.zeta_offset=0x123;memset(ram+0x80090000,0,1024);
 queued_event_draw();s_gpu.zeta_offset=0x124;s_gpu.color_offset=0x90000;immediate_draw();event();
 assert(!events && s_pending_event_count==2);
 if(!compute){
  assert(g.pending_count==1);fflush(NULL);pid_t failed_copy=fork();assert(failed_copy>=0);
  if(!failed_copy){fail_next_map=1;nv2a_pb_exec_fifo_end();_exit(9);}
  int status;assert(waitpid(failed_copy,&status,0)==failed_copy && WIFEXITED(status) && WEXITSTATUS(status)==4 && !MEM(event_at+4));
 }
 nv2a_pb_exec_fifo_end();assert(events==2 && MEM(0x80050000)==0xFFFFFFFF && MEM(0x80090000)==0xFFFFFFFF);
 puts("PASS: visibility reset/report counts and queued independent-target publication remain exact; queued readback failure cannot signal an event");
 /* Capture destinations independently of later changes to the device. */
 event_reset();queued_event_draw();MEM(0x130000)=0x120000;nv2a_pb_exec_fifo_end();
 assert(events==1 && MEM(event_at+4)==1 && !MEM(0x121968));
 /* The event object's state is a dependency for decoder and vertex inputs. */
 event_reset();queued_event_draw();uint32_t word;assert(fifo_read(event_at+4,&word) && word==1 && events==1);nv2a_pb_exec_fifo_end();
 event_reset();queued_event_draw();s_gpu.attr[0].offset=event_at;Nv2aVshOutput out;
 assert(transform_vertex(0,&out));memcpy(&word,out.pos+1,4);assert(word==1 && events==1);nv2a_pb_exec_fifo_end();
 event_reset();queued_event_draw();nv2a_pb_exec_guest_access(event_at+8,4);assert(events==1);nv2a_pb_exec_fifo_end();
 event_reset();MEM(0x130000)=0x80090000-0x1964;MEM(0x80090000)=0x00040000;MEM(0x80090004)=0;
 queued_event_draw();nv2a_pb_exec_guest_access(0xF0090004ull,4);assert(events==1 && MEM(0x80090004)==1);nv2a_pb_exec_fifo_end();
 puts("PASS: captured original destination; command/vertex/event-header hazards; tiled contiguous aliases drain before CPU reads");
 /* Changing a registration pointer via active GPU output must be read fresh.
  * Here the first rendered ARGB dword is also a valid aligned device VA. */
 if(!compute){
  event_reset();check_first_pixel=0;s_gpu.depth_test=s_gpu.depth_mask=0;
  const float address_color[4]={0,0x11/255.0f,0,0x80/255.0f};nv2a_vsh_set_constant(0,address_color);
  g_software_events[0].device_ptr_va=0x80050000;MEM(0x80050000)=0;
  const uint32_t device=0x80001100,object=device+0x1964;MEM(object)=0x00040000;MEM(object+4)=0;
  queued_event_draw();assert(MEM(0x80050000)==device && s_pending_event_count==1);
  nv2a_pb_exec_fifo_end();assert(events==1 && MEM(object+4)==1);
  g_software_events[0].device_ptr_va=0x130000;check_first_pixel=1;
 }
 /* Invalid event objects retain the old handler's unhandled behavior. */
 event_reset();MEM(event_at)=0;queued_event_draw();nv2a_pb_exec_fifo_end();assert(!events && !s_pending_completion_count);
 const uint32_t invalid_devices[]={0,0xFFFFFFF0u,0x05000000u};
 for(unsigned i=0;i<3;++i){event_reset();MEM(0x130000)=invalid_devices[i];queued_event_draw();nv2a_pb_exec_fifo_end();assert(!events && !s_pending_completion_count);}
 /* A late-patched zero NOP is still inert, and unknown nonzero NOPs drain. */
 event_reset();queued_event_draw();nv2a_pb_exec_method(0,0x100,0);assert(!events);
 nv2a_pb_exec_method(0,0x100,999);assert(events==1 && !s_pending_completion_count);nv2a_pb_exec_fifo_end();
 /* Accepted readback failure must precede every fence and event publication. */
 event_reset();queued_event_draw();nv2a_pb_exec_method(0,0x1D70,99);fflush(NULL);pid_t child=fork();assert(child>=0);
 if(!child){fail_next_map=1;nv2a_pb_exec_fifo_end();_exit(9);}
 int status;assert(waitpid(child,&status,0)==child && WIFEXITED(status) && WEXITSTATUS(status)==4);
 assert(!MEM(event_at+4) && !MEM(0x80070000));nv2a_pb_exec_fifo_end();assert(events==1 && MEM(0x80070000)==99);
 puts("PASS: active-target pointer coherence; invalid/unknown/zero NOP compatibility; readback failure publishes neither event nor fence");
 puts("PASS: registered GPU completion-event batching preserves exact renderer output and guest ordering (not a Windows FPS benchmark)");
}
'''

if __name__ == '__main__':
    with tempfile.TemporaryDirectory(prefix='qpr2-event-batch-') as folder:
        p = Path(folder)
        for name in ['windows.h', 'd3d11.h', 'd3d11_3.h', 'd3dcompiler.h', 'dxgi.h']:
            (p / name).write_text('/* fixture API */\n')
        includes = ''.join('#include "' + str(k / n) + '"\n' for n in ['nv2a_gpu_raster.c', 'nv2a_pb_exec.c', 'nv2a_pb_scan.c'])
        code = ns['api'] + shim + ns['fences'] + ns['reader'] + ns['wait_shim'] + ns['wait_body'] + registration + includes + ns['mock'] + ns['ack_shim'] + head + fixture
        (p / 'test.c').write_text(code)
        exe = p / 'test'
        subprocess.run(['gcc', '-std=gnu11', '-O1', '-fsanitize=undefined,float-cast-overflow', '-fno-sanitize-recover=all',
            '-Werror=implicit-function-declaration', '-Werror=incompatible-pointer-types', '-ffunction-sections', '-fdata-sections',
            '-I'+str(p), '-I'+str(root/'tools'), '-I'+str(k), '-I'+str(root/'third_party/xboxrecomp/src'), str(p/'test.c'),
            str(k/'nv2a_gpu_tiles.c'), str(k/'nv2a_vsh_interp.c'), str(k/'nv2a_combiner.c'), '-Wl,--gc-sections', '-pthread', '-lm', '-o', str(exe)], check=True)
        modes = sys.argv[1:] or ['native', 'quiet', 'compute', 'cpu', 'disabled', 'unregistered']
        for mode in modes:
            print('RUN event batching ' + mode, flush=True)
            subprocess.run([str(exe), mode], cwd=p, check=True, timeout=90)
        print('PASS: all', len(modes), 'event batching modes passed')
