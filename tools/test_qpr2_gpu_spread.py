"""Spread query sampling, bounded asynchronous failure handling and flush stages.
Strict API runs production rendering with scalar shader kernels and artificial
API costs. This validates measurements/isolation, not GPU or gameplay speed.
"""
from pathlib import Path
import subprocess,tempfile
import test_qpr2_gpu_timing as timing
root=timing.root;k=timing.k;api=timing.api
needle='static void QueryPerformanceCounter(LARGE_INTEGER*p){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);p->QuadPart=(int64_t)t.tv_sec*1000000000+t.tv_nsec;}'
assert needle in api;api=api.replace(needle,'static uint64_t profile_clock;\nstatic void QueryPerformanceCounter(LARGE_INTEGER*p){p->QuadPart=profile_clock;}')
api=api.replace(' (void)c;(void)pitch;(void)dep;assert(!sub',' profile_clock+=2000000;\n (void)c;(void)pitch;(void)dep;assert(!sub',1)
# The fixture's actual command entry points receive distinct synthetic costs.
fixture=timing.geo.fixture.replace('static void ID3D11DeviceContext_Dispatch(ID3D11DeviceContext*ctx,unsigned n,unsigned y,unsigned z){', 'static void ID3D11DeviceContext_Dispatch(ID3D11DeviceContext*ctx,unsigned n,unsigned y,unsigned z){profile_clock+=4000000;',1)
assert 'profile_clock+=4000000;' in fixture
needle='static void ID3D11DeviceContext_DrawIndexed(ID3D11DeviceContext*ctx,unsigned count,unsigned first,int base){';assert needle in fixture
fixture=fixture.replace(needle,needle+'profile_clock+=8000000;',1)
helpers=timing.main[:timing.main.index('int main(int argc,char**argv)')]
needle='assert(nv_gpu_draw_raw(&state,&input,(uint8_t*)color,depth,32,textures)==1 && nv_gpu_sync(&count)==1);'
replace=r'''
 assert(nv_gpu_draw_raw(&state,&input,(uint8_t*)color,depth,32,textures)==1);
 uint64_t reads=g.readbacks;memset(&g.flush_time,0,sizeof g.flush_time);profile_clock=0;
 assert(nv_gpu_submit()==1 && g.readbacks==reads && color[5*32+5]==0xFF102030);
 assert(g.flush_time.batches==1 && g.flush_time.upload_ms==2 && g.flush_time.raw_ms==14 && g.flush_time.raster_ms==10 && g.flush_time.total_ms==26);
 assert(nv_gpu_sync(&count)==1 && g.flush_time.total_ms==26);
''';assert needle in helpers;helpers=helpers.replace(needle,replace,1)
needle='assert(nv_gpu_draw(&state,&tri,1,(uint8_t*)color,depth,32,textures)==1 && nv_gpu_sync(&count)==1);'
replace=r'''
 assert(nv_gpu_draw(&state,&tri,1,(uint8_t*)color,depth,32,textures)==1);
 uint64_t reads=g.readbacks;memset(&g.flush_time,0,sizeof g.flush_time);profile_clock=0;
 assert(nv_gpu_submit()==1 && g.readbacks==reads && color[5*32+5]==0xFF102030);
 assert(g.flush_time.batches==1 && g.flush_time.upload_ms==(g.native?4:8) && g.flush_time.raw_ms==0 && g.flush_time.raster_ms==(g.native?10:6) && g.flush_time.total_ms==14);
 assert(nv_gpu_sync(&count)==1);
''';assert needle in helpers;helpers=helpers.replace(needle,replace,1)
main=r'''
static unsigned picked[3],positions[3];
static void scope_properties(void){
 for(unsigned round=0;round<2;round++){
  gpu_timing_initialize();assert(gpu_time.enabled && gpu_time.spread);
  unsigned count[3]={0},position[3]={0};
  for(unsigned i=0;i<32768;i++)for(unsigned c=0;c<3;c++){
   raw.count=c==1;unsigned token=gpu_timing_batch_begin(c==2);
   if(!token)continue;
   ++count[c];position[c]|=1u<<(i%8);
   uint64_t work=i%17+1;
   if(c==1){gpu_timing_phase_begin(NV_GPU_TIME_VERTEX,work);gpu_timing_phase_end(NV_GPU_TIME_VERTEX);gpu_timing_phase_begin(NV_GPU_TIME_PREPARE,work);gpu_timing_phase_end(NV_GPU_TIME_PREPARE);}
   unsigned phase=c==2?NV_GPU_TIME_COPY:NV_GPU_TIME_PIXELS;
   gpu_timing_phase_begin(phase,work);gpu_timing_phase_end(phase);gpu_timing_batch_end(token);gpu_timing_poll();
  }
  raw.count=0;
  for(unsigned c=0;c<3;c++){
   assert(count[c]>300 && count[c]<750 && position[c]==255);
   assert(gpu_time.population[c].batches==32768 && gpu_time.population[c].selected==count[c] && !gpu_time.population[c].dropped);
   unsigned phase=c==2?NV_GPU_TIME_COPY:NV_GPU_TIME_PIXELS;GpuSpreadPhase *p=&gpu_time.by_class[c][phase];
   assert(p->samples==count[c] && p->ms==count[c] && p->min_work==1 && p->max_work==17);
   if(!round){picked[c]=count[c];positions[c]=position[c];}else assert(picked[c]==count[c] && positions[c]==position[c]);
  }
 }
 puts("PASS: deterministic size-independent selection across all three classes, 98304 eligible scopes per round, multiple positions without any frame advance; exact class phase/work accounting");
 /* A delayed driver cannot cause spin-wait, pool growth or a fake fence. */
 gpu_timing_initialize();query_not_ready=1;uint64_t reads=g.readbacks;
 for(unsigned i=0;i<10000 && !gpu_time.dropped;i++){
  unsigned t=gpu_timing_batch_begin(1);if(t){gpu_timing_phase_begin(NV_GPU_TIME_COPY,7);gpu_timing_phase_end(NV_GPU_TIME_COPY);gpu_timing_batch_end(t);}
 }
 assert(gpu_time.dropped==1 && gpu_time.population[2].dropped==1 && !query_live_scope && g.readbacks==reads);
 unsigned pending=0;for(unsigned i=0;i<NV_GPU_TIME_SLOTS;i++)pending+=gpu_time.slot[i].closed;assert(pending==NV_GPU_TIME_SLOTS);
 uint32_t random=gpu_time.random[2];gpu_timing_report();
 assert(gpu_time.random[2]==random && !gpu_time.population[2].batches && !gpu_time.by_class[2][NV_GPU_TIME_COPY].samples);
 pending=0;for(unsigned i=0;i<NV_GPU_TIME_SLOTS;i++)pending+=gpu_time.slot[i].closed;assert(pending==NV_GPU_TIME_SLOTS);
 query_not_ready=0;gpu_timing_poll();assert(gpu_time.by_class[2][NV_GPU_TIME_COPY].samples==NV_GPU_TIME_SLOTS && !gpu_time.population[2].batches);
 puts("PASS: bounded 16-slot pool, nonblocking DONOTFLUSH queries, dropped samples on saturation, PRNG/pending-query lifetime across reports, delayed completed results do not become new-window populations");
 gpu_timing_report();unsigned t=0;while(!t)t=gpu_timing_batch_begin(1);gpu_timing_phase_begin(NV_GPU_TIME_COPY,1);gpu_timing_phase_end(NV_GPU_TIME_COPY);gpu_timing_batch_end(t);
 query_disjoint=1;gpu_timing_poll();query_disjoint=0;assert(gpu_time.invalid==1 && !gpu_time.by_class[2][NV_GPU_TIME_COPY].samples);
 t=0;while(!t)t=gpu_timing_batch_begin(1);gpu_timing_phase_begin(NV_GPU_TIME_COPY,1);gpu_timing_phase_end(NV_GPU_TIME_COPY);gpu_timing_batch_end(t);
 query_error=1;gpu_timing_poll();query_error=0;assert(!gpu_time.enabled && !query_live_scope);
 puts("PASS: invalid timestamp scope discarded; runtime query failure releases diagnostics without completing or changing accepted rendering");
}
int main(int argc,char **argv){
 const char *mode=argc>1?argv[1]:"native";int native=strcmp(mode,"compute")!=0;
 setenv("RECOMP_GPU_RASTER","1",1);setenv("RECOMP_GPU_ASYNC","0",1);setenv("RECOMP_GPU_GEOMETRY","1",1);setenv("RECOMP_GPU_DIAGNOSTIC_COUNTERS","1",1);
 setenv("RECOMP_GPU_PROFILE","1",1);setenv("RECOMP_GPU_PROFILE_SPREAD","1",1);native_supported=native;coverage_tier=3;
 assert(nv_gpu_available() && raw.ready==native && gpu_time.enabled && gpu_time.spread);g.upload_cache=0;
 if(native)scope_properties();
 gpu_timing_initialize();gpu_time.enabled=0;
 uint32_t reference[1024];float zref[1024];NvGpuCount ref={0};
 if(native)ref=scene();else compute_scene();memcpy(reference,color,sizeof color);memcpy(zref,depth,sizeof depth);
 gpu_time.enabled=1;uint64_t reads=g.readbacks;
 for(unsigned i=0;i<256;i++){
  if(native){NvGpuCount got=scene();assert(!memcmp(&ref,&got,sizeof ref));}else compute_scene();
  assert(!memcmp(reference,color,sizeof color) && !memcmp(zref,depth,sizeof depth));
 }
 assert(g.readbacks==reads+256);gpu_timing_poll();
 unsigned c=native?1:0;assert(gpu_time.by_class[c][NV_GPU_TIME_PIXELS].samples && gpu_time.by_class[2][NV_GPU_TIME_COPY].samples);
 if(native){for(unsigned i=0;i<256;i++)compute_scene();gpu_timing_poll();assert(gpu_time.by_class[0][NV_GPU_TIME_PIXELS].samples);}
 nv_gpu_report();assert(!g.flush_time.batches && !g.flush_time.upload_ms && !g.flush_time.raw_ms && !g.flush_time.raster_ms && !g.flush_time.total_ms);
 printf("PASS: %s real render instrumentation; exact pixels/depth/counts and unchanged required readbacks with profiling; exclusive synthetic CPU upload/raw/raster costs, no premature publication and report reset\n",mode);
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-gpu-spread-') as d:
 p=Path(d);(p/'logs').mkdir()
 for name in ['windows.h','d3d11.h','d3d11_3.h','d3dcompiler.h','dxgi.h']:(p/name).write_text('/* strict API */\n')
 (p/'fixture.c').write_text(api+timing.queries+'ptrdiff_t xbox_GetMemoryOffset(void){return 0;}\n#include "'+str(k/'nv2a_gpu_raster.c')+'"\n'+fixture+helpers+main)
 subprocess.run(['gcc','-std=gnu11','-O1','-ffp-contract=off','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-Werror=implicit-function-declaration','-Werror=incompatible-pointer-types','-I'+str(p),'-I'+str(root/'tools'),'-I'+str(k),str(p/'fixture.c'),str(k/'nv2a_vsh_interp.c'),str(k/'nv2a_gpu_tiles.c'),'-pthread','-lm','-o',str(p/'fixture')],check=True)
 for mode in ['native','compute']:
  print('RUN spread profile '+mode,flush=True);subprocess.run([str(p/'fixture'),mode],cwd=p,check=True,timeout=120)
 print('PASS: spread profile native/compute integration; artificial clocks are not performance benchmarks')
