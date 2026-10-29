"""Production readback timing boundaries with a deterministic API/CPU clock.

Map, CopyResource, Unmap and CPU unpack receive different artificial costs.
No real performance timing or hardware execution is inferred from this fixture.
"""
from pathlib import Path
import ast,subprocess,tempfile
root=Path(__file__).resolve().parents[1];k=root/'third_party/xboxrecomp/src/kernel'
tree=ast.parse((root/'tools/test_qpr2_gpu_device.py').read_text())
parts={n.targets[0].id:ast.literal_eval(n.value) for n in tree.body if isinstance(n,ast.Assign) and isinstance(n.targets[0],ast.Name) and n.targets[0].id in ('api','fixture')}
api=parts['api']
old='static void QueryPerformanceCounter(LARGE_INTEGER*p){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);p->QuadPart=(int64_t)t.tv_sec*1000000000+t.tv_nsec;}'
assert old in api
api=api.replace(old,'static uint64_t phase_clock;static int phase_checking;\nstatic void QueryPerformanceCounter(LARGE_INTEGER*p){p->QuadPart=phase_clock;}')
api=api.replace('p->QuadPart=1000000000;','p->QuadPart=1000000;')
api=api.replace('(void)c;if(fail_next_map)', '(void)c;if(phase_checking)phase_clock+=1000;if(fail_next_map)',1)
api=api.replace('(void)c;assert(dst->desc.ByteWidth==src->desc.ByteWidth);','(void)c;if(phase_checking)phase_clock+=2000;assert(dst->desc.ByteWidth==src->desc.ByteWidth);',1)
needle='static void ID3D11DeviceContext_Unmap('
index=api.index(needle);tail=api[index:];tail=tail.replace('(void)c;', '(void)c;if(phase_checking)phase_clock+=3000;',1);api=api[:index]+tail
shim=r'''
#define nv_gpu_surface_copy phase_copy_real
#include "nv2a_gpu_surface_copy.h"
#undef nv_gpu_surface_copy
static int nv_gpu_surface_copy(const void *src,void *color,void *depth,size_t n,int simd){
 if(phase_checking)phase_clock+=5000;return phase_copy_real(src,color,depth,n,simd);
}
ptrdiff_t xbox_GetMemoryOffset(void){return 0;}
'''
main=r'''
static void exact(double actual,double expected){assert(fabs(actual-expected)<0.000001);}
int main(int argc,char **argv){
 const char *mode=argc>1?argv[1]:"native";int scalar=!strcmp(mode,"scalar"),compute=!strcmp(mode,"compute"),queued=!strcmp(mode,"queued");
 setenv("RECOMP_GPU_RASTER","1",1);setenv("RECOMP_GPU_ASYNC","0",1);setenv("RECOMP_GPU_DIAGNOSTIC_COUNTERS","1",1);
 setenv("RECOMP_GPU_READBACK_SIMD",scalar?"0":"1",1);setenv("RECOMP_GPU_TEXTURE_BRIDGE","0",1);setenv("RECOMP_GPU_TARGET_RESUME","0",1);
 native_supported=!compute;assert(nv_gpu_available());assert(g.readback_simd==(!scalar && NV_GPU_SURFACE_SSE2));
 memset(&g.readback_time,0,sizeof g.readback_time);g.map_ms=g.copy_ms=g.readback_ms=0;phase_clock=0;phase_checking=1;
 NvGpuState s={0};s.width=s.height=16;s.pitch=64;s.bpp=4;s.format=0x128;s.color_mask=0x01010101;
 s.depth_available=s.depth_test=s.depth_mask=s.zpass_enable=1;s.depth_func=0x203;
 NvGpuTextureSource sources[4]={0};NvGpuTri t=triangle(16);uint32_t color[2][256]={{0}};float z[2][256];
 for(unsigned d=0;d<2;d++)for(unsigned i=0;i<256;i++)z[d][i]=100;
 assert(nv_gpu_draw(&s,&t,1,(uint8_t*)color[0],z[0],16,sources)==1);
 if(queued){assert(nv_gpu_draw(&s,&t,1,(uint8_t*)color[1],z[1],16,sources)==1);assert(g.pending_count==1);}
 assert(!color[0][5*16+5]);NvGpuCount count;assert(nv_gpu_sync(&count)==1);
 assert(color[0][5*16+5]==0xFFFFFFFFu && z[0][5*16+5]==10 && g.readback_time.current==1 && g.readback_time.queued==(unsigned)queued);
 if(queued)assert(color[1][5*16+5]==0xFFFFFFFFu && z[1][5*16+5]==10);
 exact(g.readback_time.enqueue_ms,compute?6:4);exact(g.readback_time.map_wait_ms,compute?3:queued?4:2);
 exact(g.readback_time.unpack_ms,compute?0:80);exact(g.readback_time.publish_ms,queued?80:0);
 exact(g.readback_time.counters_ms,0);exact(g.readback_time.unmap_ms,compute?9:queued?12:6);exact(g.readback_time.capture_ms,0);
 exact(g.map_ms,compute?9:queued?88:86); /* Legacy broad timer remains comparable. */
 exact(g.copy_ms,queued?86:0); /* Queued legacy copy timer also includes Unmap. */
 exact(g.readback_ms,compute?18:queued?180:92);
 assert(g.readback_time.pixels==(compute?0:queued?512:256));
 assert(g.readback_time.simd_rows==(!scalar && !compute?queued?32:16:0));
 assert(g.readback_time.scalar_rows==(scalar?16:0));
 nv_gpu_report();assert(!g.readback_time.current && !g.readback_time.queued && !g.readback_time.simd_rows && g.readback_time.map_wait_ms==0);
 printf("PASS: %s; actual Map cost excludes CPU unpack and copy enqueue; exact current/queued/compute output, phase counters and report reset\n",mode);
}
'''
fixture=parts['fixture'].split('int main(int argc,char**argv){')[0]
with tempfile.TemporaryDirectory(prefix='qpr2-readback-phases-') as folder:
    p=Path(folder)
    for name in ['windows.h','d3d11.h','d3d11_3.h','d3dcompiler.h','dxgi.h']:(p/name).write_text('/* strict API above */\n')
    (p/'test.c').write_text(api+shim+'\n#include "'+str(k/'nv2a_gpu_raster.c')+'"\n'+fixture+main)
    subprocess.run(['gcc','-std=gnu11','-O1','-ffp-contract=off','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all',
        '-Werror=implicit-function-declaration','-Werror=incompatible-pointer-types','-I'+str(p),'-I'+str(root/'tools'),'-I'+str(k),
        str(p/'test.c'),str(k/'nv2a_vsh_interp.c'),str(k/'nv2a_gpu_tiles.c'),'-pthread','-lm','-o',str(p/'test')],check=True)
    for mode in ['native','scalar','queued','compute']:subprocess.run([str(p/'test'),mode],check=True,cwd=p,timeout=120)
