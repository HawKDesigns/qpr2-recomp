"""Verify real FIFO fences can complete before GPU pixels reach guest RAM."""
from pathlib import Path
import subprocess, tempfile

root = Path(__file__).resolve().parents[1]
base = root / 'tools/test_qpr2_fence_batch.py'
source = base.read_text()
namespace = {'__file__': str(base)}
exec(compile(source[:source.index('\nwith tempfile.TemporaryDirectory(')], str(base), 'exec'), namespace)
k = namespace['k']
fixture = namespace['fixture']
fixture = fixture.replace(
    'assert(!g.active && !g.pending_count && !g.queued_count && !g.readback_failed);',
    'assert(!g.queued_count && !g.readback_failed);', 1)
fixture = fixture[:fixture.index('int main(int argc,char **argv)')] + r'''
/* RECOMP_GPU_COMPLETION_ASYNC: a completion-only fence is released once its
 * commands are submitted. Everything that reads GPU results must still see
 * them, in order, and a blocking marker must still bound the run. */
static int async_main(void){
 setenv("RECOMP_GPU_RASTER","1",1);setenv("RECOMP_GPU_ASYNC","0",1);
 setenv("RECOMP_GPU_COMPLETION_TOKEN","1",1);
 setenv("RECOMP_GPU_COMPLETION_ASYNC","1",1);setenv("RECOMP_GPU_COMPLETION_ASYNC_MAX","2",1);
 setenv("RECOMP_GPU_DIAGNOSTIC_COUNTERS","0",1);
 native_supported=1;coverage_tier=3;assert(nv_gpu_available() && g.native && g.completion.enabled);
 assert(g.completion.async_enabled && g.completion.async_limit==2);
 MEM(0x130000)=0x110000;
 assert(!xbox_Nv2aCommandFence(0x130000,0x30,8,32));
 setup(1);draw();execute();
 unsigned flushes=context_flushes;
 nv2a_pb_exec_fifo_begin();nv2a_pb_exec_method(0,0x1D70,7);nv2a_pb_exec_fifo_end();
 assert(MEM(0x80070000)==7 && g.active && g.completion.unpublished);
 assert(g.completion.waits==0 && g.completion.async_releases==1 && g.completion.async_forced==0);
 assert(context_flushes==flushes+1 && g.readbacks==0 && MEM(0x80050000)==0);
 nv2a_pb_exec_guest_access(0x80050000,4);
 assert(!g.active && !g.completion.unpublished && MEM(0x80050000)==0xFFFFFFFF);
 assert(g.completion.access_drains==1 && g.readbacks==1);
 puts("PASS: async fence releases with no marker Map; CPU target read still publishes exact pixels");
 setup(1);draw();execute();
 nv2a_pb_exec_fifo_begin();nv2a_pb_exec_method(0,0x1D70,8);nv2a_pb_exec_fifo_end();
 assert(MEM(0x80070000)==8 && g.active && MEM(0x80050000)==0);
 assert(g.completion.waits==0 && g.completion.async_releases==2);
 gpu_finish_named("flip");
 assert(!g.active && MEM(0x80050000)==0xFFFFFFFF && s_gpu.zpass_count);
 puts("PASS: flip after an async fence drains deferred pixels and visibility counters");
 setup(1);draw();execute();
 nv2a_pb_exec_fifo_begin();nv2a_pb_exec_method(0,0x1D70,9);nv2a_pb_exec_fifo_end();
 assert(MEM(0x80070000)==9 && g.completion.waits==1 && g.completion.async_forced==1);
 assert(g.completion.async_releases==2 && g.active && MEM(0x80050000)==0);
 puts("PASS: the third consecutive completion takes the blocking marker (limit 2)");
 nv2a_pb_exec_fifo_begin();nv2a_pb_exec_method(0,0x1D70,10);nv2a_pb_exec_fifo_end();
 assert(MEM(0x80070000)==10 && g.completion.waits==1 && g.completion.async_releases==3);
 s_gpu.zpass_count=0;gpu_finish_named("visibility-report");
 assert(g.active && s_gpu.zpass_count && !MEM(0x80050000));
 gpu_finish_named("fixture-cleanup");
 puts("PASS: visibility report after an async fence reads the retained counters");
 /* Fence 10 was the first release since the forced marker, so fence 11 is
  * the second; the one after it is forced, and its Map fails: no release. */
 setup(1);draw();execute();
 nv2a_pb_exec_fifo_begin();nv2a_pb_exec_method(0,0x1D70,11);nv2a_pb_exec_fifo_end();
 assert(MEM(0x80070000)==11 && g.completion.waits==1);
 assert(g.completion.async_releases==4 && g.completion.async_forced==1);
 gpu_finish_named("fixture-cleanup");
 setup(1);draw();execute();
 fflush(NULL);pid_t child=fork();assert(child>=0);
 if(!child){nv2a_pb_exec_fifo_begin();nv2a_pb_exec_method(0,0x1D70,99);
  fail_next_map=1;nv2a_pb_exec_fifo_end();_exit(9);}
 int status;assert(waitpid(child,&status,0)==child && WIFEXITED(status) && WEXITSTATUS(status)==4);
 assert(!MEM(0x80070000));          /* setup() cleared it; fence 99 never landed */
 gpu_finish_named("fixture-cleanup");
 puts("PASS: a failed forced marker Map still cannot release a guest fence");
 return 0;
}
int main(void){
 ram=mmap(NULL,0x84000000ull,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS|MAP_NORESERVE,-1,0);
 assert(ram!=MAP_FAILED);g_memory_offset=(ptrdiff_t)ram;
 if(getenv("FIXTURE_ASYNC"))return async_main();
 assert(!getenv("RECOMP_GPU_COMPLETION_ASYNC"));
 setenv("RECOMP_GPU_RASTER","1",1);setenv("RECOMP_GPU_ASYNC","0",1);
 setenv("RECOMP_GPU_COMPLETION_TOKEN","1",1);
 setenv("RECOMP_GPU_DIAGNOSTIC_COUNTERS","0",1);
 native_supported=1;coverage_tier=3;assert(nv_gpu_available() && g.native && g.completion.enabled);
 assert(!g.completion.async_enabled);           /* off unless requested */
 MEM(0x130000)=0x110000;
 assert(!xbox_Nv2aCommandFence(0x130000,0x30,8,32));
 setup(1);draw();execute();
 nv2a_pb_exec_fifo_begin();nv2a_pb_exec_method(0,0x1D70,7);nv2a_pb_exec_fifo_end();
 assert(MEM(0x80070000)==7 && g.active && g.completion.unpublished);
 assert(g.completion.waits==1 && g.readbacks==0 && MEM(0x80050000)==0);
 assert(!g.completion.async_releases && !g.completion.async_forced);
 nv2a_pb_exec_guest_access(0x80050000,4);
 assert(!g.active && !g.completion.unpublished && MEM(0x80050000)==0xFFFFFFFF);
 assert(g.completion.access_drains==1 && g.readbacks==1);
 puts("PASS: fence releases after GPU marker, CPU target read publishes exact pixels later");
 setup(1);draw();execute();
 nv2a_pb_exec_fifo_begin();nv2a_pb_exec_method(0,0x1D70,8);nv2a_pb_exec_fifo_end();
 assert(g.active && MEM(0x80050000)==0);
 gpu_finish_named("flip");
 assert(!g.active && MEM(0x80050000)==0xFFFFFFFF && s_gpu.zpass_count);
 puts("PASS: flip drains deferred pixels and visibility counters");
 setup(1);draw();execute();
 nv2a_pb_exec_fifo_begin();nv2a_pb_exec_method(0,0x1D70,9);nv2a_pb_exec_fifo_end();
 NvGpuState next=g.target;uint32_t other_color[256]={0};float other_depth[256]={0};
 assert(gpu_target(&next,(uint8_t*)other_color,other_depth,16)==1);
 assert(g.pending_count==1 && MEM(0x80050000)==0);
 NvGpuCount counts;assert(nv_gpu_sync_named(&counts,"fixture-target-switch")==1);
 assert(!g.pending_count && MEM(0x80050000)==0xFFFFFFFF && counts.zpass);
 puts("PASS: post-fence target switch retains ordered previous pixels and counters");
 setup(1);draw();execute();
 nv2a_pb_exec_fifo_begin();nv2a_pb_exec_method(0,0x1D70,10);nv2a_pb_exec_fifo_end();
 assert(g.active && !s_gpu.zpass_count && !MEM(0x80050000));
 gpu_finish_named("visibility-report");
 assert(g.active && s_gpu.zpass_count && !MEM(0x80050000));
 assert(nv_gpu_reset_report()==1);
 s_gpu.zpass_count=0;gpu_finish_named("visibility-report");assert(!s_gpu.zpass_count);
 gpu_finish_named("fixture-cleanup");
 puts("PASS: visibility report reads retained counters and ordered reset discards them");
 setup(1);draw();execute();assert(g.active);
 fflush(NULL);pid_t child=fork();assert(child>=0);
 if(!child){nv2a_pb_exec_fifo_begin();nv2a_pb_exec_method(0,0x1D70,99);
  fail_next_map=1;nv2a_pb_exec_fifo_end();_exit(9);}
 int status;assert(waitpid(child,&status,0)==child && WIFEXITED(status) && WEXITSTATUS(status)==4);
 assert(!MEM(0x80070000));
 gpu_finish_named("fixture-cleanup");
 puts("PASS: failed marker Map cannot release a guest fence");
 g.completion.reuse_enabled=1;
 setup(1);draw();execute();
 uint64_t waits=g.completion.waits;
 for(unsigned value=11;value<=12;value++){
  nv2a_pb_exec_fifo_begin();nv2a_pb_exec_method(0,0x1D70,value);nv2a_pb_exec_fifo_end();
  assert(MEM(0x80070000)==value);
 }
 assert(g.completion.waits==waits+1 && g.completion.reused==1 && g.completion.unpublished);
 assert(MEM(0x80050000)==0);
 assert(nv_gpu_clear_depth(g.host_depth,g.depth_stride,42)==1);
 nv2a_pb_exec_fifo_begin();nv2a_pb_exec_method(0,0x1D70,13);nv2a_pb_exec_fifo_end();
 assert(MEM(0x80070000)==13 && g.completion.waits==waits+2 && g.completion.reused==1);
 draw();execute();
 nv2a_pb_exec_fifo_begin();nv2a_pb_exec_method(0,0x1D70,14);nv2a_pb_exec_fifo_end();
 assert(MEM(0x80070000)==14 && g.completion.waits==waits+3 && g.completion.reused==1);
 gpu_finish_named("flip");
 assert(MEM(0x80050000)==0xFFFFFFFF);
 nv2a_pb_exec_fifo_begin();nv2a_pb_exec_method(0,0x1D70,15);nv2a_pb_exec_fifo_end();
 assert(MEM(0x80070000)==15 && g.completion.waits==waits+4 && g.completion.reused==1);
 setup(1);draw();execute();
 fflush(NULL);child=fork();assert(child>=0);
 if(!child){
  nv2a_pb_exec_fifo_begin();nv2a_pb_exec_method(0,0x1D70,99);
  fail_next_map=1;nv2a_pb_exec_fifo_end();_exit(9);
 }
 assert(waitpid(child,&status,0)==child && WIFEXITED(status) && WEXITSTATUS(status)==4);
 assert(!MEM(0x80070000));
 puts("PASS: consecutive fences reuse a confirmed marker; clears, draws and transfers require a fresh completion");
 return 0;
}
'''

with tempfile.TemporaryDirectory(prefix='qpr2-completion-token-') as folder:
    temp = Path(folder)
    for name in ['windows.h', 'd3d11.h', 'd3d11_3.h', 'd3dcompiler.h', 'dxgi.h']:
        (temp/name).write_text('/* fixture API */\n')
    includes = ''.join('#include "'+str(k/name)+'"\n' for name in
                       ['nv2a_gpu_raster.c','nv2a_pb_exec.c','nv2a_pb_scan.c'])
    code = ''.join(namespace[name] for name in ['api','shim','fences','reader','wait_shim','wait_body'])
    code += includes + namespace['mock'] + namespace['ack_shim'] + fixture
    src = temp/'test.c'; exe = temp/'test'; src.write_text(code)
    subprocess.run(['gcc','-std=gnu11','-O1','-fsanitize=undefined,float-cast-overflow',
                    '-fno-sanitize-recover=all','-Werror=implicit-function-declaration',
                    '-Werror=incompatible-pointer-types','-ffunction-sections','-fdata-sections',
                    '-I'+str(temp),'-I'+str(root/'tools'),'-I'+str(k),
                    '-I'+str(root/'third_party/xboxrecomp/src'),str(src),
                    str(k/'nv2a_gpu_tiles.c'),str(k/'nv2a_vsh_interp.c'),
                    str(k/'nv2a_combiner.c'),'-Wl,--gc-sections','-pthread','-lm','-o',str(exe)],check=True)
    import os
    baseline = {key: value for key, value in os.environ.items() if not key.startswith('RECOMP_')}
    subprocess.run([str(exe)],cwd=temp,check=True,timeout=90,env=baseline)
    subprocess.run([str(exe)],cwd=temp,check=True,timeout=90,env=dict(baseline, FIXTURE_ASYNC='1'))
