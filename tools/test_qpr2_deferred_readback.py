"""Assert real queued staging transfers are deferred, batched and retired.

Reuses the independent color/depth/count and real FIFO failure fixtures. The
strict API adds transfer-source and pre-Map batch-order assertions. Synthetic
API execution validates behavior; it does not measure hardware performance.
"""
from pathlib import Path
import subprocess,tempfile
from test_qpr2_gpu_timing import queries
root=Path(__file__).resolve().parents[1];base=root/'tools/test_qpr2_target_retire.py'
s=base.read_text();ns={'__file__':str(base)}
exec(compile(s[:s.index('\nwith tempfile.TemporaryDirectory(')],str(base),'exec'),ns)
api=ns['api'].replace('#define NV_GPU_NO_TIMESTAMPS 1','')+queries;f=ns['f'];helpers=ns['helpers'];fixture=ns['fixture']
api=api.replace('static void ID3D11DeviceContext_CopyResource(', 'static ID3D11Resource *deferred_corrupt_dst;\nstatic void deferred_observe_copy(ID3D11Resource*,ID3D11Resource*);\nstatic void ID3D11DeviceContext_CopyResource(',1)
a='(void)c;if(dst->desc.Usage==D3D11_USAGE_STAGING)++copy_calls;';assert a in api
api=api.replace(a,'(void)c;deferred_observe_copy(dst,src);if(dst->desc.Usage==D3D11_USAGE_STAGING)++copy_calls;',1)
api=api.replace('memcpy(dst->data,src->data,src->desc.ByteWidth);','memcpy(dst->data,src->data,src->desc.ByteWidth);if(dst==deferred_corrupt_dst){dst->data[0]^=0x40;deferred_corrupt_dst=NULL;}',1)
fixture=r'''
static int deferred_fault,profile_deferred;
static unsigned deferred_required_copies,eager_queued_copies;
static void deferred_observe_copy(ID3D11Resource *dst,ID3D11Resource *src){
 for(unsigned i=0;i<NV_GPU_PENDING_READBACKS;i++){
  GpuPendingReadback *p=g.pending+i;
  if(dst!=p->surface)continue;
  if(p->surface_copy_pending){
   assert(i<g.pending_count && p->publish_surface && p->serial);
   assert(src==p->snapshot.buffer && src!=g.surface.buffer);
   if(profile_deferred && !g.selftest && !deferred_required_copies){
    if(gpu_time.active){
     GpuTimeSlot *time=&gpu_time.slot[gpu_time.active-1];
     assert(time->work[NV_GPU_TIME_COPY]==(uint64_t)p->target.width*p->target.height);
     assert(time->query[1+NV_GPU_TIME_COPY*2]->ended);
    }else{
     /* A full completion may sample its current copy first. The same bounded
      * profiler must not open a second scope just to time the saved copy. */
     assert(g.readback_batch.enabled && g.active && !gpu_time.want_copy);
     assert(gpu_time.next_frame>gpu_time.frame);
    }
   }
   ++deferred_required_copies;
   if(g.selftest && deferred_fault==1)deferred_corrupt_dst=dst;
   if(g.selftest && deferred_fault==2)fail_next_map=1;
  }else ++eager_queued_copies;
 }
}
''' + fixture
needle='static void retire_count_map(ID3D11Resource *b){';assert needle in fixture
fixture=fixture.replace(needle,needle+r'''
 /* Every non-retired saved transfer must already be issued before ANY Map,
  * including the first retired version's counter-only Map. */
 for(unsigned i=0;i<g.pending_count;i++)assert(!g.pending[i].surface_copy_pending);
''',1)
fixture=fixture.replace('surface_maps=retired_surface_maps=statistics_maps=all_maps=queued_maps=0;',
 'surface_maps=retired_surface_maps=statistics_maps=all_maps=queued_maps=0;deferred_required_copies=eager_queued_copies=0;if(profile_deferred){gpu_time.next_frame=0;gpu_time.want_copy=1;}',1)
needle='const char *mode=argc>1?argv[1]:"default";';assert needle in fixture
fixture=fixture.replace(needle,needle+r'''
 if(!strcmp(mode,"profile")){setenv("RECOMP_GPU_PROFILE","1",1);profile_deferred=1;mode="default";}
 if(!strcmp(mode,"gate-color"))deferred_fault=1;
 if(!strcmp(mode,"gate-map"))deferred_fault=2;
 if(!strcmp(mode,"eager")){setenv("RECOMP_GPU_DEFER_READBACK","0",1);mode="default";}
 if(!strcmp(mode,"scalar")){setenv("RECOMP_GPU_READBACK_SIMD","0",1);mode="default";}
''',1)
needle='assert(nv_gpu_sync(&count)==1);\n  for(unsigned d=0;d<2;d++)';assert needle in fixture
fixture=fixture.replace(needle,r'''
  int deferred=g.deferred_readback.enabled && g.bridge.enabled;
  assert(deferred_required_copies==(deferred?2u-g.pending_count:0u));
  assert(eager_queued_copies==(deferred?0u:2u));
  unsigned required=0,avoided=0;
  for(unsigned q=0;q<g.pending_count;q++)if(g.pending[q].surface_copy_pending){
   if(g.pending[q].publish_surface)++required;else ++avoided;
  }
  unsigned before_copies=deferred_required_copies;
  uint64_t old_issued=g.deferred_readback.issued,old_avoided=g.deferred_readback.avoided;
  uint64_t old_bytes=g.deferred_readback.avoided_bytes;
  assert(nv_gpu_sync(&count)==1);
  assert(deferred_required_copies==before_copies+required);
  assert(g.deferred_readback.issued==old_issued+required);
  assert(g.deferred_readback.avoided==old_avoided+avoided);
  assert(g.deferred_readback.avoided_bytes==old_bytes+avoided*16u*16u*8u);
  for(unsigned d=0;d<2;d++)''',1)
needle='native_supported=1;coverage_tier=!strcmp(mode,"rectangle")?0:3;assert(nv_gpu_available());';assert needle in fixture
fixture=fixture.replace(needle,r'''native_supported=1;coverage_tier=!strcmp(mode,"rectangle")?0:3;
 int ready=nv_gpu_available();
 if(deferred_fault==2){assert(!ready && s_startup==3 && !g.pending_count && !g.host_color && !g.host_depth);puts("PASS: deferred startup Map failure prevents hardware readiness and discards private destinations");return 0;}
 assert(ready);
 if(deferred_fault==1){assert(!g.bridge.enabled && !g.resume.enabled && !g.pending_count && !g.host_color && !g.host_depth);puts("PASS: corrupt deferred staging output rejects bridge/resume before guest rendering");return 0;}
''',1)
fixture=fixture.replace('nv_gpu_report();puts("PASS: superseded target retirement complete");',
 'nv_gpu_report();assert(!g.deferred_readback.deferred && !g.deferred_readback.issued && !g.deferred_readback.avoided && !g.deferred_readback.avoided_bytes && !g.deferred_readback.enqueue_ms);puts("PASS: real deferred transfers, exact snapshot source, all-copy-before-Map ordering, retirement bytes, reporting and completion failures");',1)
with tempfile.TemporaryDirectory(prefix='qpr2-deferred-readback-') as folder:
 tmp=Path(folder)
 for name in ['windows.h','d3d11.h','d3d11_3.h','d3dcompiler.h','dxgi.h']:(tmp/name).write_text('/* fixture API */\n')
 k=root/'third_party/xboxrecomp/src/kernel';includes=''.join('#include "'+str(k/n)+'"\n' for n in ['nv2a_gpu_raster.c','nv2a_pb_exec.c','nv2a_pb_scan.c'])
 code=api+f['shim']+f['fences']+f['reader']+f['wait_shim']+f['wait_body']+includes+f['mock']+f['ack_shim']+ns['ns']['head']+helpers+fixture
 (tmp/'test.c').write_text(code);exe=tmp/'test'
 subprocess.run(['gcc','-std=gnu11','-O1','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-Werror=implicit-function-declaration','-Werror=incompatible-pointer-types','-ffunction-sections','-fdata-sections','-I'+str(tmp),'-I'+str(root/'tools'),'-I'+str(k),'-I'+str(root/'third_party/xboxrecomp/src'),str(tmp/'test.c'),str(k/'nv2a_gpu_tiles.c'),str(k/'nv2a_vsh_interp.c'),str(k/'nv2a_combiner.c'),'-Wl,--gc-sections','-pthread','-lm','-o',str(exe)],check=True)
 for mode in ['default','quiet','rectangle','eager','scalar','retire-disabled','resume-disabled','bridge-disabled','profile','gate-color','gate-map']:
  print('RUN deferred readback '+mode,flush=True);cwd=tmp/mode;cwd.mkdir();subprocess.run([str(exe),mode],cwd=cwd,check=True,timeout=90)
 print('PASS: all 11 deferred readback modes passed')
