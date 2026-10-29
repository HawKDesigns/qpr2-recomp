"""Exercise actual Win32 raster-worker code with pthread event/API shims.

Optional renderer path compares a previous executor. Frame/depth buffers must
match serial output byte for byte. Reported timing is a local shim benchmark,
not Windows FPS. New renderer must also return exact pixel/depth statistics.
"""
from pathlib import Path
import subprocess,tempfile,sys
import test_qpr2_gpu_event as gpu
root=gpu.root
args=sys.argv[1:]
sanitize='--sanitize' in args
if sanitize:args.remove('--sanitize')
renderer=Path(args[0]).resolve() if args else root/'third_party/xboxrecomp/src/kernel/nv2a_pb_exec.c'
native=r'''
#define _WIN32 1
#define __declspec(x) __thread
#define WINAPI
typedef uint32_t DWORD;
typedef void *LPVOID;
typedef struct {DWORD dwNumberOfProcessors;} SYSTEM_INFO;
#define INFINITE 0xffffffffu
#define FALSE 0
int xbox_Nv2aReleaseFence(uint32_t,uint32_t,uint32_t);
int xbox_Nv2aResolveFence(uint32_t,uint32_t,uint32_t*);
int xbox_Nv2aReleaseResolvedFence(uint32_t,uint32_t,uint32_t,uint32_t);
int xbox_Nv2aHandleSoftwareMethod(uint32_t,uint32_t);
int xbox_Nv2aResolveSoftwareEvent(uint32_t,uint32_t,uint32_t*);
int xbox_Nv2aReleaseResolvedSoftwareEvent(uint32_t,uint32_t,uint32_t);
static int QueryPerformanceFrequency(LARGE_INTEGER*p){p->QuadPart=1000000000;return 1;}
static int QueryPerformanceCounter(LARGE_INTEGER*p){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);p->QuadPart=(int64_t)t.tv_sec*1000000000+t.tv_nsec;return 1;}
static void GetSystemInfo(SYSTEM_INFO*p){p->dwNumberOfProcessors=8;}
static LONG InterlockedDecrement(volatile LONG*p){return __atomic_sub_fetch(p,1,__ATOMIC_SEQ_CST);}
static DWORD WaitForSingleObject(HANDLE h,DWORD ms){
 (void)ms;Event*e=h;pthread_mutex_lock(&e->mutex);
 while(!e->signaled)pthread_cond_wait(&e->cond,&e->mutex);
 if(!e->manual)e->signaled=0;pthread_mutex_unlock(&e->mutex);return 0;
}
typedef struct {DWORD(*fn)(LPVOID);LPVOID arg;pthread_t thread;} Worker;
static void *worker_thunk(void*p){Worker*w=p;w->fn(w->arg);return NULL;}
static HANDLE CreateThread(void*a,size_t b,DWORD(*fn)(LPVOID),LPVOID arg,DWORD flags,DWORD*id){
 (void)a;(void)b;(void)flags;(void)id;Worker*w=calloc(1,sizeof *w);assert(w);w->fn=fn;w->arg=arg;
 assert(!pthread_create(&w->thread,NULL,worker_thunk,w));pthread_detach(w->thread);return w;
}
static int CloseHandle(HANDLE h){(void)h;return 1;}
'''
fixture=r'''
static void setup(void){
 memset(ram,0,sizeof ram);memset(&s_gpu,0,sizeof s_gpu);
 s_gpu.format=0x128;s_gpu.color_offset=0x1000;s_gpu.pitch=2048;
 s_gpu.clip_w=s_gpu.clip_h=512;s_gpu.color_mask=0x01010101;
 s_gpu.depth_test=s_gpu.depth_mask=1;s_gpu.depth_func=0x203;s_gpu.zeta_offset=0x180000;
 float*z=zbuf_current(1);for(size_t i=0;i<(size_t)NV_ZBUF_W*NV_ZBUF_H;i++)z[i]=100;
}
static Nv2aVshOutput point(float x,float y,float z){
 Nv2aVshOutput p={0};p.pos[0]=x;p.pos[1]=y;p.pos[2]=z;p.pos[3]=1;
 p.d0[0]=p.d0[3]=1;return p;
}
static void quad(int size,float z){
 Nv2aVshOutput a=point(0,0,z),b=point(size,0,z),c=point(size,size,z),d=point(0,size,z);
 raster_xf_clipped(&a,&b,&c);raster_xf_clipped(&a,&c,&d);
}
static void ordered_passes(void){
 s_gpu.zpass_enable=1;quad(512,50);
 s_gpu.blend_enable=1;s_gpu.blend_sfactor=0x302;s_gpu.blend_dfactor=0x303;s_gpu.blend_equation=0x8006;
 Nv2aVshOutput a=point(-32,32,20),b=point(480,16,20),c=point(512,480,20);
 a.d0[0]=b.d0[0]=c.d0[0]=0;a.d0[1]=b.d0[1]=c.d0[1]=1;
 a.d0[3]=b.d0[3]=c.d0[3]=0.5f;raster_xf_clipped(&a,&b,&c);
 quad(512,80); /* depth-fail behind prior geometry */
 s_gpu.alpha_test=1;s_gpu.alpha_func=0x204;s_gpu.alpha_ref=128;
 a.d0[3]=b.d0[3]=c.d0[3]=0;raster_xf_clipped(&a,&b,&c);
}
static uint32_t hash_bytes(const void*p,size_t n){const uint8_t*b=p;uint32_t h=2166136261u;for(size_t i=0;i<n;i++)h=(h^b[i])*16777619u;return h;}
int main(void){
 setenv("RECOMP_RASTER_THREADS","4",1);setenv("RECOMP_RENDER_TIMING","1",1);
 assert(raster_pool_size()==4);
 setup();s_pool.n=1;ordered_passes();
 uint8_t*reference=malloc(512*512*4);float*depth=malloc(NV_ZBUF_W*NV_ZBUF_H*4);
 assert(reference && depth);memcpy(reference,ram+0x1000,512*512*4);
 memcpy(depth,zbuf_current(0),NV_ZBUF_W*NV_ZBUF_H*4);
 uint64_t pixels=s_gpu.xf_pixels,fails=s_gpu.xf_depth_fail;uint32_t zpass=s_gpu.zpass_count,max=s_gpu.pixel_max;
 setup();s_pool.n=4;ordered_passes();
 assert(!memcmp(reference,ram+0x1000,512*512*4));
 assert(!memcmp(depth,zbuf_current(0),NV_ZBUF_W*NV_ZBUF_H*4));
 assert(s_gpu.xf_pixels==pixels && s_gpu.xf_depth_fail==fails && s_gpu.zpass_count==zpass);
 for(int pass=0;pass<12;pass++){
  setup();s_pool.n=pass%3==0?1:pass%3==1?2:4;ordered_passes();
  assert(!memcmp(reference,ram+0x1000,512*512*4));
  assert(!memcmp(depth,zbuf_current(0),NV_ZBUF_W*NV_ZBUF_H*4));
  assert(s_gpu.xf_pixels==pixels && s_gpu.xf_depth_fail==fails && s_gpu.zpass_count==zpass);
#ifdef NV_RASTER_PIXELS_PER_THREAD
  assert(s_gpu.pixels==pixels && s_gpu.pixel_max==max);
#endif
 }
 s_pool.n=4;
#ifdef NV_RASTER_PIXELS_PER_THREAD
 assert(s_gpu.pixels==pixels && s_gpu.pixel_max==max);
 setup();memset(&s_render_time,0,sizeof s_render_time);quad(128,50);
 assert(!s_render_time.parallel_jobs);quad(512,50);assert(s_render_time.parallel_jobs==2);
#endif
 setup();s_gpu.depth_test=s_gpu.depth_mask=0;memset(&s_render_time,0,sizeof s_render_time);
 double begin=render_wall_ms();for(int i=0;i<200;i++)quad(128,50);double medium=render_wall_ms()-begin;
 uint64_t medium_dispatch=s_render_time.parallel_jobs;
 memset(&s_render_time,0,sizeof s_render_time);begin=render_wall_ms();for(int i=0;i<20;i++)quad(512,50);double large=render_wall_ms()-begin;
 printf("{\"medium_ms\":%.3f,\"large_ms\":%.3f,\"medium_dispatch\":%llu,\"large_dispatch\":%llu,\"frame_checksum\":%u,\"pixel_count\":%llu,\"expected_count\":%llu}\n",
  medium,large,(unsigned long long)medium_dispatch,(unsigned long long)s_render_time.parallel_jobs,
  hash_bytes(ram+0x1000,512*512*4),(unsigned long long)s_gpu.pixels,(unsigned long long)s_gpu.xf_pixels);
#ifdef NV_RASTER_PIXELS_PER_THREAD
 assert(s_gpu.pixels==s_gpu.xf_pixels);
#endif
 free(reference);free(depth);
 puts("PASS: actual Win32 worker implementation using native-event shims matches serial color/depth bytes, blending/order/alpha/depth tests and visibility counts");
}
'''
def run_fixture(renderer, fixture, sanitize=False, extra_sources=(), defines=()):
 with tempfile.TemporaryDirectory(prefix='qpr2-raster-workers-') as d:
  p=Path(d);(p/'windows.h').write_text('/* API supplied by fixture */\n')
  source=p/'fixture.c';exe=p/'fixture'
  source.write_text(gpu.shim+native+'#include "'+str(renderer)+'"\n'+fixture)
  subprocess.run(['gcc','-std=gnu11','-O2','-Werror=implicit-function-declaration','-ffunction-sections','-fdata-sections',
   *(['-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all'] if sanitize else []),
   *['-D'+d for d in defines],
   '-I'+str(p),'-I'+str(root/'third_party/xboxrecomp/src/kernel'),
   '-I'+str(root/'third_party/xboxrecomp/src'),'-I'+str(root/'third_party/xboxrecomp/include'),
   str(source),str(root/'third_party/xboxrecomp/src/kernel/nv2a_vsh_interp.c'),
   str(root/'third_party/xboxrecomp/src/kernel/nv2a_combiner.c'),
   *map(str,extra_sources),
   '-Wl,--gc-sections','-pthread','-lm','-o',str(exe)],check=True)
  result=subprocess.run([str(exe)],timeout=90,text=True,capture_output=True)
  print(result.stderr,end='',file=sys.stderr)
  print(result.stdout,end='')
  result.check_returncode()
  return result.stdout

if __name__ == '__main__':
 run_fixture(renderer, fixture, sanitize)
