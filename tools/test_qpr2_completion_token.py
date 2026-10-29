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
int main(void){
 ram=mmap(NULL,0x84000000ull,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS|MAP_NORESERVE,-1,0);
 assert(ram!=MAP_FAILED);g_memory_offset=(ptrdiff_t)ram;
 setenv("RECOMP_GPU_RASTER","1",1);setenv("RECOMP_GPU_ASYNC","0",1);
 setenv("RECOMP_GPU_COMPLETION_TOKEN","1",1);
 setenv("RECOMP_GPU_DIAGNOSTIC_COUNTERS","0",1);
 native_supported=1;coverage_tier=3;assert(nv_gpu_available() && g.native && g.completion.enabled);
 MEM(0x130000)=0x110000;
 assert(!xbox_Nv2aCommandFence(0x130000,0x30,8,32));
 setup(1);draw();execute();
 nv2a_pb_exec_fifo_begin();nv2a_pb_exec_method(0,0x1D70,7);nv2a_pb_exec_fifo_end();
 assert(MEM(0x80070000)==7 && g.active && g.completion.unpublished);
 assert(g.completion.waits==1 && g.readbacks==0 && MEM(0x80050000)==0);
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
    subprocess.run([str(exe)],cwd=temp,check=True,timeout=90)
