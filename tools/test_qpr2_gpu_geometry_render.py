"""Real PB wrapper -> raw GPU kernels -> ordered native pixels vs CPU geometry.
API mock executes portable scalar kernels, not hardware HLSL.
"""
from pathlib import Path
import subprocess,tempfile
import test_qpr2_gpu_geometry as geo
root=geo.root;k=geo.k
base=root/'tools/test_qpr2_fence_batch.py';ns={'__file__':str(base)}
s=base.read_text();exec(compile(s[:s.index('with tempfile.TemporaryDirectory(')],str(base),'exec'),ns)
api=geo.api.replace('static LONG InterlockedExchange(volatile LONG*p,LONG n){','static void completion_probe(volatile LONG*,LONG);\nstatic LONG InterlockedExchange(volatile LONG*p,LONG n){completion_probe(p,n);')
head=ns['fixture'][:ns['fixture'].index('int main(int argc,char **argv)')]
main=r'''
static void asset_setup(unsigned scene,int enable){
 setup(1);raw.ready=enable;s_gpu.pitch=128;s_gpu.clip_w=s_gpu.clip_h=32;memset(ram+0x80050000,0,32*128);
 s_gpu.prim=(unsigned[]){5,6,7,8,9,10}[scene%6];s_gpu.idx_count=12;
 s_gpu.clip_z_seen=3;s_gpu.clip_z_min=1;s_gpu.clip_z_max=80;
 s_gpu.blend_enable=1;s_gpu.blend_sfactor=0x302;s_gpu.blend_dfactor=0x303;s_gpu.blend_equation=0x8006;
 s_gpu.cull_enable=(scene/6)%2;s_gpu.front_face=(scene/12)%2?0x900:0x901;s_gpu.cull_face=0x405;
 s_gpu.fog_enable=1;s_gpu.fog_mode=0x800+(scene%5);s_gpu.fog_param[0]=.25f;s_gpu.fog_param[1]=.125f;s_gpu.fog_color=0xFF102040;
 s_gpu.rc_seen=(scene/24)%2;
 const float one[4]={1,1,1,1},zero[4]={0},fog[4]={0,1,0,1};
 nv2a_vsh_set_constant(58,one);nv2a_vsh_set_constant(59,zero);nv2a_vsh_set_constant(101,one);nv2a_vsh_set_constant(186,fog);
 for(unsigned row=0;row<4;row++){float c[4]={0};c[row]=1;nv2a_vsh_set_constant(96+row,c);}
 unsigned instructions;const uint32_t(*program)[4]=nv2a_vsh_known_program(0,&instructions);
 for(unsigned j=0;j<instructions;j++)nv2a_vsh_set_instruction(j,program[j]);
 nv2a_vsh_set_start_slot(0);nv2a_vsh_set_cxt_write(0);
 for(unsigned a=0;a<16;a++){s_gpu.attr[a].offset=0x100000+a*0x1000;s_gpu.attr[a].stride=16;s_gpu.attr[a].size=4;s_gpu.attr[a].type=2;}
 for(unsigned i=0;i<6;i++){
  float p[4]={2+(float)((i*13)%26),2+(float)((i*17)%26),20+(float)i,1};
  if(scene%8==6 && i==0)p[0]-=20; // boundary crossing must use original CPU clipping
  if(scene%8==7)p[0]-=60; // wholly outside may reject before vertex execution
  for(unsigned a=0;a<16;a++){float v[4]={.25f+i*.05f,.5f,.75f,.5f};memcpy(ram+s_gpu.attr[a].offset+i*16,a?v:p,16);}
 }
 for(unsigned i=0;i<12;i++)s_gpu.idx[i]=i%6;
}
int main(void){
 ram=mmap(NULL,0x84000000ull,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS|MAP_NORESERVE,-1,0);assert(ram!=MAP_FAILED);g_memory_offset=(ptrdiff_t)ram;
 setenv("RECOMP_GPU_RASTER","1",1);setenv("RECOMP_GPU_ASYNC","0",1);setenv("RECOMP_RASTER_THREADS","1",1);setenv("RECOMP_GPU_GEOMETRY","1",1);setenv("RECOMP_GPU_DIAGNOSTIC_COUNTERS","1",1);
 native_supported=1;coverage_tier=3;assert(nv_gpu_available() && raw.ready);MEM(0x130000)=0x110000;assert(!xbox_Nv2aCommandFence(0x130000,0x30,8,32));
 uint32_t color[1024];float depth[1024];
 for(unsigned scene=0;scene<72;scene++){
  uint32_t zpass=0;
  for(unsigned enable=0;enable<2;enable++){
   asset_setup(scene,enable);
   nv2a_pb_exec_fifo_begin();raster_batch_program();nv2a_pb_exec_fifo_end();gpu_finish_named("geometry-render-check");
   float*z=zbuf_current(0);
   if(!enable){memcpy(color,ram+0x80050000,sizeof color);for(unsigned y=0;y<32;y++)memcpy(depth+y*32,z+y*1024,128);zpass=s_gpu.zpass_count;}
   else {if(memcmp(color,ram+0x80050000,sizeof color)){fprintf(stderr,"scene=%u rawdraws=%llu fallback=%llu\n",scene,(unsigned long long)raw.draws_queued,(unsigned long long)raw.boundary_fallbacks);for(unsigned q=0;q<1024;q++)if(color[q]!=((uint32_t*)(ram+0x80050000))[q]){fprintf(stderr,"pixel=%u cpu=%08x raw=%08x\n",q,color[q],((uint32_t*)(ram+0x80050000))[q]);break;}abort();}for(unsigned y=0;y<32;y++)assert(!memcmp(depth+y*32,z+y*1024,128));assert(zpass==s_gpu.zpass_count);}
  }
 }
 assert(raw.draws_queued && raw.boundary_fallbacks && raw.bounds_rejected);
 printf("PASS: 72 actual renderer-wrapper scenes match CPU geometry pixels/depth/visibility: six topologies, culling, fog/combiners, inside/boundary/outside, live constant snapshots; raw_draws=%llu fallback=%llu rejected=%llu\n",(unsigned long long)raw.draws_queued,(unsigned long long)raw.boundary_fallbacks,(unsigned long long)raw.bounds_rejected);

 asset_setup(0,1);nv2a_pb_exec_fifo_begin();raster_batch_program();
 assert(raw.count && g.queued_count);nv2a_pb_exec_method(0,0x1D70,99);assert(s_pending_fence_count==1 && !MEM(0x80070000));
 fflush(NULL);pid_t pid=fork();assert(pid>=0);
 if(!pid){fail_next_map=1;nv2a_pb_exec_fifo_end();_exit(9);}
 int status;assert(waitpid(pid,&status,0)==pid && WIFEXITED(status) && WEXITSTATUS(status)==4 && !MEM(0x80070000));
 nv2a_pb_exec_fifo_end();assert(MEM(0x80070000)==99);
 puts("PASS: accepted batched raw geometry readback failure cannot publish a false guest fence; successful completion publishes afterward");
 return 0;
}
'''
if __name__=='__main__':
 with tempfile.TemporaryDirectory(prefix='qpr2-gpu-geometry-render-') as d:
  p=Path(d);(p/'logs').mkdir()
  for name in ['windows.h','d3d11.h','d3d11_3.h','d3dcompiler.h','dxgi.h']:(p/name).write_text('/* fixture */\n')
  includes=''.join('#include "'+str(k/n)+'"\n' for n in ['nv2a_gpu_raster.c','nv2a_pb_exec.c','nv2a_pb_scan.c'])
  (p/'fixture.c').write_text(api+ns['shim']+ns['fences']+ns['reader']+ns['wait_shim']+ns['wait_body']+includes+geo.fixture+ns['ack_shim']+head+main)
  subprocess.run(['gcc','-std=gnu11','-O1','-ffp-contract=off','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-Werror=implicit-function-declaration','-Werror=incompatible-pointer-types','-ffunction-sections','-fdata-sections','-I'+str(p),'-I'+str(root/'tools'),'-I'+str(k),'-I'+str(root/'third_party/xboxrecomp/src'),str(p/'fixture.c'),str(k/'nv2a_vsh_interp.c'),str(k/'nv2a_gpu_tiles.c'),str(k/'nv2a_combiner.c'),'-Wl,--gc-sections','-pthread','-lm','-o',str(p/'fixture')],check=True)
  subprocess.run([str(p/'fixture')],cwd=p,check=True,timeout=120)
