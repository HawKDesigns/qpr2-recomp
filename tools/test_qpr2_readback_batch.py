"""Validate all required current/saved copies precede the first blocking Map.
Reuses real renderer/FIFO and independent pixel/depth/visibility fixtures.
The API shim checks command order, not hardware performance.
"""
from pathlib import Path
root=Path(__file__).resolve().parents[1]
base=root/'tools/test_qpr2_deferred_readback.py';source=base.read_text()
prefix,runner=source.split('\nwith tempfile.TemporaryDirectory(',1)
ns={'__file__':str(base)};exec(compile(prefix,str(base),'exec'),ns)
api=ns['api'];fixture=ns['fixture']
api=api.replace('static void deferred_observe_copy(', 'static void batch_observe_copy(ID3D11Resource*);\nstatic void deferred_observe_copy(',1)
api=api.replace('(void)c;deferred_observe_copy(dst,src);','(void)c;batch_observe_copy(dst);deferred_observe_copy(dst,src);',1)
fixture=r'''
static ID3D11Resource *batch_expected[2+NV_GPU_PENDING_READBACKS];
static unsigned batch_seen[2+NV_GPU_PENDING_READBACKS],batch_count,batch_maps;
static int batch_checking,batch_fail_current;
static void batch_observe_copy(ID3D11Resource *dst){
 if(!batch_checking)return;
 for(unsigned i=0;i<batch_count;i++)if(dst==batch_expected[i])++batch_seen[i];
}
static void batch_check_map(ID3D11Resource *b){
 if(batch_fail_current && b==g.surface.readback){assert(!g.pending_count && !MEM(0x80070000));fail_next_map=1;}
 if(!batch_checking)return;
 ++batch_maps;
 if(g.readback_batch.enabled)for(unsigned i=0;i<batch_count;i++)assert(batch_seen[i]==1);
}
static void batch_start(void){
 batch_count=batch_maps=0;memset(batch_seen,0,sizeof batch_seen);
 assert(g.active && g.native);
 batch_expected[batch_count++]=g.surface.readback;
 if(g.stats_required)batch_expected[batch_count++]=g.stats.readback;
 for(unsigned i=0;i<g.pending_count;i++)if(g.pending[i].surface_copy_pending && g.pending[i].publish_surface)
  batch_expected[batch_count++]=g.pending[i].surface;
 batch_checking=1;
}
static void batch_end(void){
 assert(batch_maps);
 for(unsigned i=0;i<batch_count;i++)assert(batch_seen[i]==1);
 batch_checking=0;
}
'''+fixture
fixture=fixture.replace('static void retire_count_map(ID3D11Resource *b){','static void retire_count_map(ID3D11Resource *b){batch_check_map(b);',1)
needle='const char *mode=argc>1?argv[1]:"default";'
fixture=fixture.replace(needle,needle+'\n if(!strcmp(mode,"batch-disabled")){setenv("RECOMP_GPU_BATCH_READBACK","0",1);mode="default";}',1)
needle='  assert(nv_gpu_sync(&count)==1);\n  assert(deferred_required_copies=='
assert needle in fixture
fixture=fixture.replace(needle,'''  unsigned batch_pending=g.pending_count;
  uint64_t before_batches=g.readback_batch.batches,before_older=g.readback_batch.older_targets;
  batch_start();assert(nv_gpu_sync(&count)==1);batch_end();
  unsigned expected_batches=g.readback_batch.enabled && batch_pending;
  assert(g.readback_batch.batches==before_batches+expected_batches);
  assert(g.readback_batch.older_targets==before_older+(expected_batches?batch_pending:0));
  assert(deferred_required_copies==''',1)
fixture=fixture.replace('nv_gpu_report();assert(!g.deferred_readback.deferred', 'nv_gpu_report();assert(!g.readback_batch.batches && !g.readback_batch.older_targets && !g.readback_batch.current_bytes);assert(!g.deferred_readback.deferred',1)
a=fixture.index(' fflush(NULL);pid_t child=fork();')
b=fixture.index(' nv2a_pb_exec_fifo_end();assert(MEM(0x80070000)==97',a)
fixture=fixture[:a]+r'''
 for(unsigned failure=0;failure<2;failure++){
  fflush(NULL);pid_t child=fork();assert(child>=0);
  if(!child){queued_maps=0;fail_queued_map_at=failure?0:1;batch_fail_current=failure;nv2a_pb_exec_fifo_end();_exit(9);}
  int status;assert(waitpid(child,&status,0)==child && WIFEXITED(status) && WEXITSTATUS(status)==4 && !MEM(0x80070000));
 }
'''+fixture[b:]
ns.update(api=api,fixture=fixture)
runner=runner.replace("['default','quiet'", "['batch-disabled','default','quiet'").replace('all 11 deferred readback modes','all 12 batched completion modes')
exec(compile('\nwith tempfile.TemporaryDirectory('+runner,str(base),'exec'),ns)
