"""Run actual renderer queues with profiling enabled/disabled; verify isolation.
Portable timing is not a hardware performance measurement.
"""
from pathlib import Path
import subprocess,tempfile
import test_qpr2_gpu_geometry as geo
api=geo.api.replace('static void QueryPerformanceCounter(LARGE_INTEGER*p){','static unsigned profile_clock_calls;\nstatic void QueryPerformanceCounter(LARGE_INTEGER*p){++profile_clock_calls;')
assert api!=geo.api
main=geo.main.replace(' geometry_fail_compile=', ' setenv("RECOMP_GPU_GEOMETRY_PROFILE",!strcmp(mode,"off")?"0":"1",1);\n geometry_fail_compile=',1)
main=main.replace(' assert(nv_gpu_available());',' assert(nv_gpu_available());\n for(unsigned i=0;i<3;i++){assert(!raw.profile[i].rng);for(unsigned j=0;j<3;j++)assert(!raw.profile[i].calls[j] && !raw.profile[i].samples[j]);}',1)
main=main.replace('int bounds=gpu_raw_bounds(m,&input);','int bounds=gpu_raw_profile_bounds(m,&input);assert(bounds==gpu_raw_bounds(m,&input));',1)
needle=' for(unsigned i=0;i<NV_RAW_MESHES;i++)gpu_raw_mesh_release(&raw.mesh[i]);'
extra=r'''
 /* The independent reference uses the original bounds operation. No clock
  * reads or population pollution are permitted during startup or opt-out. */
 NvRawMesh probe={0};unsigned clocks=profile_clock_calls;NvRawProfile saved[3];memcpy(saved,raw.profile,sizeof saved);
 g.selftest=1;for(unsigned i=0;i<10000;i++)assert(gpu_raw_profile_bounds(&probe,&input)==gpu_raw_bounds(&probe,&input));g.selftest=0;
 assert(profile_clock_calls==clocks && !memcmp(saved,raw.profile,sizeof saved));
 if(!raw.profile_enabled){for(unsigned i=0;i<10000;i++)assert(gpu_raw_profile_bounds(&probe,&input)==gpu_raw_bounds(&probe,&input));assert(profile_clock_calls==clocks && !memcmp(saved,raw.profile,sizeof saved));}
 else {uint64_t calls=0,samples=0;for(unsigned i=0;i<3;i++){calls+=raw.profile[1].calls[i];samples+=raw.profile[1].samples[i];}assert(calls>=1800 && samples>calls/256 && samples<calls/16);}
 puts("PASS: actual queued pixels/depth, immutable versions and bounds match with sparse profiling enabled or disabled; startup and opt-out perform no added clock reads");
'''
assert needle in main;main=main.replace(needle,extra+needle,1)
needle=' nv_gpu_report();puts("PASS: exact-bounds'
extra=r''' uint32_t seeds[3];for(unsigned i=0;i<3;i++)seeds[i]=raw.profile[i].rng;
 gpu_raw_profile_report();for(unsigned i=0;i<3;i++){assert(raw.profile[i].rng==seeds[i]);for(unsigned j=0;j<3;j++)assert(!raw.profile[i].calls[j] && !raw.profile[i].samples[j] && !raw.profile[i].ms[j] && !raw.profile[i].peak_ms[j]);}
 puts("PASS: reporting resets only sampled populations/timing, retains sampler sequence, and never submits or reads back GPU work");
'''
assert needle in main;main=main.replace(needle,extra+needle,1)
with tempfile.TemporaryDirectory(prefix='qpr2-geometry-profile-') as d:
 p=Path(d)
 for name in ['windows.h','d3d11.h','d3d11_3.h','d3dcompiler.h','dxgi.h']:(p/name).write_text('/* strict API */\n')
 (p/'fixture.c').write_text(api+'ptrdiff_t xbox_GetMemoryOffset(void){return 0;}\n#include "'+str(geo.k/'nv2a_gpu_raster.c')+'"\n'+geo.fixture+main)
 subprocess.run(['gcc','-std=gnu11','-O1','-ffp-contract=off','-Werror=implicit-function-declaration','-Werror=incompatible-pointer-types','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-I'+str(p),'-I'+str(geo.root/'tools'),'-I'+str(geo.k),str(p/'fixture.c'),str(geo.k/'nv2a_vsh_interp.c'),str(geo.k/'nv2a_gpu_tiles.c'),'-pthread','-lm','-o',str(p/'fixture')],check=True)
 for mode in ['native','off']:
  cwd=p/mode;cwd.mkdir();subprocess.run([str(p/'fixture'),mode],cwd=cwd,check=True,timeout=180)
print('PASS: geometry profiling enabled/disabled integration and startup/report isolation')
