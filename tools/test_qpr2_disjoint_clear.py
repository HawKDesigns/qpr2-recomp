"""Actual executor clears plus actual GPU range proof under native API shims.

The fake completion writes pending pixels into the CPU mirrors. Tests assert
that aliases complete before CPU overwrites, while unrelated clears don't wait.
Hardware rendering/readback itself is covered by test_qpr2_gpu_device.py.
"""
from pathlib import Path
import subprocess,tempfile
import test_qpr2_raster_workers as workers
import test_qpr2_gpu_event as gpu
root=gpu.root
backend=(root/'third_party/xboxrecomp/src/kernel/nv2a_gpu_raster.c').read_text()
proof=gpu.between(backend,'static uintptr_t gpu_canonical_host','static int gpu_pending_access(const void *address,size_t bytes)\n{')+gpu.between(backend,'static int gpu_ranges_disjoint','static int gpu_native_target(unsigned width,unsigned height)\n{')
shim=r'''
#define NV2A_ENABLE_GPU_RASTER 1
#include "nv2a_gpu_raster.h"
static volatile LONG s_startup=2;
static int gpu_startup_owned(void){return 0;}
typedef struct {NvGpuState target;uint8_t *host_color;float *host_depth;unsigned depth_stride;} GpuPendingReadback;
static struct {int active,selftest;NvGpuState target;uint8_t *host_color;float *host_depth;unsigned depth_stride;uint64_t disjoint_writes;unsigned pending_count;GpuPendingReadback pending[4];} g;
static unsigned completions;
int nv_gpu_geometry_available(void){return 0;}
int nv_gpu_draw_raw(const NvGpuState*s,const NvRawInput*i,uint8_t*c,float*d,unsigned stride,const NvGpuTextureSource t[4])
{(void)s;(void)i;(void)c;(void)d;(void)stride;(void)t;return 0;}
int nv_gpu_submit(void){return 1;}
int nv_gpu_has_pending_work(void){return 0;}
int nv_gpu_guest_read(uint64_t a,uint64_t n){(void)a;(void)n;return 1;}
int nv_gpu_guest_access(uint64_t a,uint64_t n){(void)a;(void)n;return 1;}
int nv_gpu_available(void){return 0;}
int nv_gpu_draw(const NvGpuState*s,const NvGpuTri*t,unsigned n,uint8_t*c,float*d,unsigned stride,const NvGpuTextureSource tex[4])
{(void)s;(void)t;(void)n;(void)c;(void)d;(void)stride;(void)tex;return 0;}
int nv_gpu_sync_named(NvGpuCount *count,const char *reason){
 (void)reason;memset(count,0,sizeof *count);++completions;
 if(g.active){uint32_t pending=0xFFABCDEF;memcpy(g.host_color,&pending,4);if(g.host_depth)*g.host_depth=50;g.active=0;}
 return 1;
}
int nv_gpu_clear_color(const NvGpuState*s,uint8_t*c,unsigned l,unsigned t,unsigned r,unsigned b,uint32_t v)
{(void)s;(void)c;(void)l;(void)t;(void)r;(void)b;(void)v;return 0;}
int nv_gpu_clear_depth(float*d,unsigned stride,float v){(void)d;(void)stride;(void)v;return 0;}
int nv_gpu_reset_report(void){return 0;}
void nv_gpu_capture_present(const uint32_t*p,unsigned w,unsigned h){(void)p;(void)w;(void)h;}
void nv_gpu_capture_request(void){}
void nv_gpu_report(void){}
int xbox_Nv2aReleaseFence(uint32_t a,uint32_t b,uint32_t c){(void)a;(void)b;(void)c;return 1;}
int xbox_Nv2aResolveFence(uint32_t c,uint32_t o,uint32_t*a){(void)c;(void)o;(void)a;return 0;}
int xbox_Nv2aReleaseResolvedFence(uint32_t c,uint32_t o,uint32_t a,uint32_t v){(void)c;(void)o;(void)a;(void)v;assert(0);return 0;}
int xbox_Nv2aHandleSoftwareMethod(uint32_t a,uint32_t b){(void)a;(void)b;return 1;}
int xbox_Nv2aResolveSoftwareEvent(uint32_t s,uint32_t v,uint32_t*a){(void)s;(void)v;(void)a;return 0;}
int xbox_Nv2aReleaseResolvedSoftwareEvent(uint32_t s,uint32_t v,uint32_t a){(void)s;(void)v;(void)a;return 0;}
'''
fixture=r'''
static void setup(void){
 memset(ram,0,sizeof ram);memset(&s_gpu,0,sizeof s_gpu);memset(&g,0,sizeof g);completions=0;
 s_gpu.format=0x128;s_gpu.color_offset=0x50000;s_gpu.pitch=80;s_gpu.clip_w=s_gpu.clip_h=16;
 s_gpu.clear_color=0xFF123456;
 g.active=1;g.target.width=g.target.height=16;g.target.pitch=80;g.target.bpp=4;g.target.format=0x128;
 g.host_color=ram+0x60000;
}
int main(void){
 g_xbox_mem_offset=(ptrdiff_t)ram;
 setup();clear_surface(NV097_CLEAR_COLOR_MASK);
 assert(!completions && g.active && BRIDGE_MEM32(0x50000)==s_gpu.clear_color && !BRIDGE_MEM32(0x60000));
 /* Different starting addresses can still overlap; pending output must land
  * BEFORE the later CPU clear overwrites the overlapping pixel. */
 setup();s_gpu.color_offset=0x60004;clear_surface(NV097_CLEAR_COLOR_MASK);
 assert(completions==1 && !g.active && BRIDGE_MEM32(0x60000)==0xFFABCDEF && BRIDGE_MEM32(0x60004)==s_gpu.clear_color);
 setup();s_gpu.color_offset=0x5FFFC;s_gpu.clip_w=2;s_gpu.clip_h=1;clear_surface(NV097_CLEAR_COLOR_MASK);
 assert(completions==1 && !g.active && BRIDGE_MEM32(0x60000)==s_gpu.clear_color);
 /* A row starting outside can reach the active surface through pitch. */
 setup();s_gpu.color_offset=0x5FF00;s_gpu.pitch=256;s_gpu.clip_w=1;s_gpu.clip_h=2;
 clear_surface(NV097_CLEAR_COLOR_MASK);assert(completions==1 && BRIDGE_MEM32(0x60000)==s_gpu.clear_color);
 /* Reject a guest color write aliasing the backend's depth mirror too. */
 setup();g.host_depth=(float*)(ram+0x50000);g.depth_stride=20;
 clear_surface(NV097_CLEAR_COLOR_MASK);assert(completions==1 && BRIDGE_MEM32(0x50000)==s_gpu.clear_color);
 /* Swizzled writes require the whole Morton extent, RGB565 uses two bytes. */
 setup();s_gpu.format=0x04040203;s_gpu.pitch=32;clear_surface(NV097_CLEAR_COLOR_MASK);assert(!completions);
 setup();s_gpu.format=0x04040203;s_gpu.pitch=32;s_gpu.color_offset=0x5FF00;
 clear_surface(NV097_CLEAR_COLOR_MASK);assert(completions==1);
 /* Depth-cache clearing is independent unless either active mirror aliases. */
 setup();s_gpu.zeta_offset=0x777;float *z=zbuf_current(1);assert(z);completions=0;g.active=1;
 zbuf_clear();assert(!completions && g.active);
 g.host_depth=z;g.depth_stride=NV_ZBUF_W;zbuf_clear();assert(completions==1 && !g.active && z[0]==zclear_value());
 puts("PASS: actual linear/pitched/swizzled/RGB565 color and depth clears skip only disjoint waits; aliases complete before CPU writes");
}
'''
with tempfile.TemporaryDirectory() as d:
 p=Path(d);(p/'windows.h').write_text('/* fixture */\n');c=p/'test.c';exe=p/'test'
 c.write_text(gpu.shim+workers.native+shim+proof+'#include "'+str(root/'third_party/xboxrecomp/src/kernel/nv2a_pb_exec.c')+'"\n'+fixture)
 subprocess.run(['gcc','-std=gnu11','-O1','-ffunction-sections','-fdata-sections','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-Werror=implicit-function-declaration','-I'+str(p),'-I'+str(root/'third_party/xboxrecomp/src/kernel'),'-I'+str(root/'third_party/xboxrecomp/src'),str(c),str(root/'third_party/xboxrecomp/src/kernel/nv2a_combiner.c'),str(root/'third_party/xboxrecomp/src/kernel/nv2a_vsh_interp.c'),'-Wl,--gc-sections','-pthread','-lm','-o',str(exe)],check=True)
 subprocess.run([str(exe)],check=True,timeout=20)
