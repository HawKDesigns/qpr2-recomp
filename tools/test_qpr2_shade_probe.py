"""Exercise sparse shade replay on an ordinary draw with strict D3D mocks."""
from pathlib import Path
import subprocess, tempfile, runpy
root=Path(__file__).resolve().parents[1]
k=root/'third_party/xboxrecomp/src/kernel'
t=runpy.run_path(str(root/'tools/test_qpr2_gpu_timing.py'))
geo=t['geo']
api=t['api'].replace('static ID3D11UnorderedAccessView *bound_uav[4];',
                     'static ID3D11UnorderedAccessView *bound_uav[5];')
api=api.replace('assert(n==1 && rtv && *rtv && first==1 && count==3);bound_raster=*rtv;bound_uav[0]=NULL;bound_uav[1]=uav[0];bound_uav[2]=uav[1];bound_uav[3]=uav[2];',
                'assert(n==1 && rtv && *rtv && first==1 && (count==3 || count==4));bound_raster=*rtv;bound_uav[0]=NULL;bound_uav[1]=uav[0];bound_uav[2]=uav[1];bound_uav[3]=uav[2];bound_uav[4]=count==4?uav[3]:NULL;')
api=api.replace('assert(!rtv && !first && count==4);bound_raster=NULL;memcpy(bound_uav,uav,sizeof bound_uav);',
                'assert(!rtv && !first && (count==4 || count==5));bound_raster=NULL;memcpy(bound_uav,uav,count*sizeof *uav);bound_uav[4]=count==5?uav[4]:NULL;')
api=api.replace('for(int i=0;i<4;i++)assert(!bound_uav[i]',
                'for(int i=0;i<5;i++)assert(!bound_uav[i]')
api=api.replace('for(unsigned i=0;i<4;i++)assert(!bound_uav[i]',
                'for(unsigned i=0;i<5;i++)assert(!bound_uav[i]')
fixture=geo.fixture.replace('static void mock_native_draw(ID3D11DeviceContext*ctx,unsigned vertices,unsigned first_vertex){',
'''static unsigned private_probe_draws;
static void mock_native_draw(ID3D11DeviceContext*ctx,unsigned vertices,unsigned first_vertex){
 if(bound_uav[4]){
  assert(!bound_uav[1] && !bound_uav[2] && !bound_uav[3] && g.shade_probe.ready);
  assert(bound_uav[4]==g.shade_probe.sink.uav && vertices);
  ((uint32_t*)bound_uav[4]->buffer->data)[0]=0x51;
  ++private_probe_draws;return;
 }
''')
assert api!=t['api'] and fixture!=geo.fixture
scene=t['main'][t['main'].index('static uint32_t color[1024]'):t['main'].index('static void compute_scene(void)')]
main=r'''
int main(void){
 setenv("RECOMP_GPU_RASTER","1",1);setenv("RECOMP_GPU_ASYNC","0",1);setenv("RECOMP_GPU_GEOMETRY","1",1);
 setenv("RECOMP_GPU_PROFILE","1",1);setenv("RECOMP_GPU_PROFILE_MATERIALS","1",1);
 setenv("RECOMP_GPU_PROFILE_SHADE","1",1);
 native_supported=1;coverage_tier=3;
 assert(nv_gpu_available() && g.native && gpu_time.enabled && gpu_time.materials);
 assert(g.shade_probe.ready && gpu_time.shade && g.shade_probe.sink.buffer);
 assert(query_creates==NV_GPU_TIME_SLOTS*(1+NV_GPU_TIME_KINDS*2+2+2));
 gpu_time.enabled=0;NvGpuCount baseline=scene();uint32_t reference[1024];float zref[1024];
 memcpy(reference,color,sizeof color);memcpy(zref,depth,sizeof depth);
 gpu_time.enabled=1;NvGpuCount profiled=scene();
 assert(!memcmp(&baseline,&profiled,sizeof baseline));
 assert(!memcmp(reference,color,sizeof color) && !memcmp(zref,depth,sizeof depth));
 assert(private_probe_draws==1 && g.shade_probe.replays==1);
 assert(g.shade_probe.scratch_bytes==32u*32u*4u);
 assert(!bound_uav[4] && ((uint32_t*)g.shade_probe.sink.buffer->data)[0]==0x51);
 assert(!query_live_scope && gpu_time.slot[0].closed && gpu_time.slot[0].material_done && gpu_time.slot[0].shade_done);
 query_not_ready=1;gpu_timing_poll();assert(gpu_time.slot[0].closed && !gpu_time.shade_result.matched);
 query_not_ready=0;gpu_timing_poll();
 assert(gpu_time.shade_result.matched==1 && gpu_time.shade_result.triangles==1);
 assert(gpu_time.shade_result.full_ms>0 && gpu_time.shade_result.source_ms>0);
 puts("PASS: ordinary draw paired query, private source replay, exact color/depth/visibility, nonblocking readout");
 return 0;
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-shade-probe-') as d:
 p=Path(d);(p/'logs').mkdir()
 for name in ['windows.h','d3d11.h','d3d11_3.h','d3dcompiler.h','dxgi.h']:(p/name).write_text('/* API shim */\n')
 source=api+t['queries']+'ptrdiff_t xbox_GetMemoryOffset(void){return 0;}\n#include "'+str(k/'nv2a_gpu_raster.c')+'"\n'+fixture+scene+main
 (p/'fixture.c').write_text(source)
 subprocess.run(['gcc','-std=gnu11','-O1','-ffp-contract=off','-Werror=implicit-function-declaration','-Werror=incompatible-pointer-types',
                 '-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-I'+str(p),'-I'+str(root/'tools'),'-I'+str(k),
                 str(p/'fixture.c'),str(k/'nv2a_vsh_interp.c'),str(k/'nv2a_gpu_tiles.c'),'-pthread','-lm','-o',str(p/'fixture')],check=True)
 subprocess.run([str(p/'fixture')],cwd=p,check=True,timeout=120)
