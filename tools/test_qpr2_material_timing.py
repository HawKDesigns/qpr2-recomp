"""Sparse material GPU ranges: correct attribution, no waits/publication changes."""
from pathlib import Path
import subprocess,tempfile
import test_qpr2_gpu_timing as base
r=base.root;k=base.k
queries=base.queries.replace('static uint64_t query_clock;','static uint64_t query_clock;\nstatic ID3D11Query *material_block_query;static int material_query_error;')
queries=queries.replace('query_clock+=1000;q->value=query_clock;', 'q->value=query_clock;')
queries=queries.replace('if(query_error)return -1;', 'if(q==material_block_query)return material_query_error?-1:1;\n if(query_error)return -1;')
helpers=base.main[:base.main.index('int main(int argc,char**argv)')]
main=r'''
int main(int argc,char**argv){
 const char *mode=argc>1?argv[1]:"native";
 setenv("RECOMP_GPU_RASTER","1",1);setenv("RECOMP_GPU_ASYNC","0",1);setenv("RECOMP_GPU_GEOMETRY","1",1);
 setenv("RECOMP_GPU_PROFILE","1",1);setenv("RECOMP_GPU_PROFILE_SPREAD","1",1);
 setenv("RECOMP_GPU_PROFILE_MATERIALS",!strcmp(mode,"off")?"0":"1",1);
 native_supported=1;coverage_tier=3;if(!strcmp(mode,"allocation"))query_fail_at=11;
 assert(nv_gpu_available() && raw.ready);
 if(!strcmp(mode,"allocation")){assert(!gpu_time.enabled && query_releases==query_creates-1);scene();puts("PASS: new timestamp allocation failure retains exact live renderer");return 0;}
 assert(gpu_time.enabled);
 if(!strcmp(mode,"off")){assert(!gpu_time.materials && query_creates==NV_GPU_TIME_SLOTS*9);scene();assert(!gpu_time.material[1].draws);puts("PASS: material opt-out allocates no extra queries and keeps existing renderer");return 0;}
 assert(gpu_time.materials && query_creates==NV_GPU_TIME_SLOTS*11);
 for(unsigned c=0;c<5;c++)assert(!gpu_time.material[c].draws && !gpu_time.material[c].samples);
 /* Independent simulated GPU costs differ by material; repeated ranges for
  * the same class must not accidentally span intervening draws. */
 unsigned expected[5]={0};gpu_time.spread=0;
 for(unsigned run=0;run<5000;run++){
  gpu_time.next_frame=gpu_time.frame;gpu_time.want_copy=0;
  unsigned batch=gpu_timing_batch_begin(0);assert(batch);gpu_timing_material_select(5);
  unsigned pick=gpu_time.slot[batch-1].material_pick;++expected[pick];
  gpu_timing_phase_begin(NV_GPU_TIME_PIXELS,15);
  for(unsigned c=0;c<5;c++){
   unsigned t=gpu_timing_material_begin(c,c+1);assert(!!t==(c==pick));query_clock+=1000*(c+1);gpu_timing_material_end(t);
  }
  gpu_timing_phase_end(NV_GPU_TIME_PIXELS);gpu_timing_batch_end(batch);gpu_timing_poll();
 }
 for(unsigned c=0;c<5;c++){
  assert(expected[c]>800 && expected[c]<1200);
  assert(gpu_time.material[c].draws==5000 && gpu_time.material[c].triangles==5000*(c+1));
  assert(gpu_time.material[c].samples==expected[c] && gpu_time.material[c].ms==expected[c]*(c+1));
  assert(gpu_time.material[c].weighted_ms==5*gpu_time.material[c].ms && gpu_time.material[c].run_weights==5*expected[c]);
 }
 assert(gpu_time.phase[NV_GPU_TIME_PIXELS].ms==75000);
 puts("PASS: five distinct GPU costs, one range per batch, population/sample/weight accounting and coverage of every range position");
 gpu_timing_report();gpu_time.next_frame=gpu_time.frame;gpu_time.want_copy=0;
 unsigned token=gpu_timing_batch_begin(0);assert(token);gpu_timing_material_select(1);
 unsigned chosen=gpu_timing_material_begin(0,7);query_clock+=7000;gpu_timing_material_end(chosen);gpu_timing_batch_end(token);
 material_block_query=gpu_time.slot[token-1].query[9];unsigned gets=query_gets;uint64_t reads=g.readbacks;gpu_timing_poll();
 assert(query_gets==gets+2 && gpu_time.slot[token-1].closed && !gpu_time.material[0].samples && g.readbacks==reads);
 uint32_t seed=gpu_time.material_random;gpu_timing_report();assert(gpu_time.material_random==seed && gpu_time.slot[token-1].closed);
 material_block_query=NULL;gpu_timing_poll();assert(gpu_time.material[0].samples==1 && gpu_time.material[0].ms==7 && !gpu_time.material[0].draws);
 puts("PASS: delayed material timestamps never spin, flush or publish; pending queries/PRNG survive report boundaries");
 gpu_timing_initialize();gpu_time.enabled=0;NvGpuCount reference=scene();uint32_t pixels[1024];float z[1024];memcpy(pixels,color,sizeof pixels);memcpy(z,depth,sizeof z);
 gpu_time.enabled=1;reads=g.readbacks;
 for(unsigned i=0;i<256;i++){
  NvGpuCount got=scene();assert(!memcmp(&reference,&got,sizeof got));assert(!memcmp(color,pixels,sizeof pixels) && !memcmp(depth,z,sizeof z));
 }
 assert(g.readbacks==reads+256);gpu_timing_poll();assert(gpu_time.material[1].draws==256 && gpu_time.material[1].samples>0);
 puts("PASS: actual instrumented draw path preserves pixels/depth/visibility and required readback counts");
 gpu_time.spread=0;gpu_time.next_frame=gpu_time.frame;gpu_time.want_copy=0;token=gpu_timing_batch_begin(0);assert(token);gpu_timing_material_select(1);
 chosen=gpu_timing_material_begin(0,1);gpu_timing_material_end(chosen);gpu_timing_batch_end(token);
 material_block_query=gpu_time.slot[token-1].query[9];material_query_error=1;gpu_timing_poll();assert(!gpu_time.enabled && !query_live_scope);
 material_block_query=NULL;scene();puts("PASS: material query failure disables diagnostics without stopping valid rendering");return 0;
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-material-time-') as d:
 p=Path(d)
 for n in ['windows.h','d3d11.h','d3d11_3.h','d3dcompiler.h','dxgi.h']:(p/n).write_text('/* strict API */\n')
 (p/'test.c').write_text(base.api+queries+'ptrdiff_t xbox_GetMemoryOffset(void){return 0;}\n#include "'+str(k/'nv2a_gpu_raster.c')+'"\n'+base.geo.fixture+helpers+main)
 subprocess.run(['gcc','-std=gnu11','-O1','-ffp-contract=off','-Werror=implicit-function-declaration','-Werror=incompatible-pointer-types','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-I'+str(p),'-I'+str(r/'tools'),'-I'+str(k),str(p/'test.c'),str(k/'nv2a_vsh_interp.c'),str(k/'nv2a_gpu_tiles.c'),'-pthread','-lm','-o',str(p/'test')],check=True)
 for mode in ['native','off','allocation']:
  cwd=p/mode;cwd.mkdir();subprocess.run([str(p/'test'),mode],cwd=cwd,check=True,timeout=180)
print('PASS: material timestamp attribution, renderer isolation and query failures')
