"""Retire superseded CPU pixel copies without losing pixels, counts or fences.
This executes production queues and strict D3D calls, not a hardware benchmark.
"""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1];base=root/'tools/test_qpr2_readback_queue.py'
s=base.read_text();ns={'__file__':str(base)}
exec(compile(s[:s.index('\nwith tempfile.TemporaryDirectory(')],str(base),'exec'),ns)
f=ns['ns'];api=ns['api']
api=api.replace('static HRESULT ID3D11DeviceContext_Map(', 'static void retire_count_map(ID3D11Resource*);\nstatic HRESULT ID3D11DeviceContext_Map(',1)
api=api.replace('(void)c;++all_maps;', '(void)c;retire_count_map(b);++all_maps;',1)
helpers=ns['fixture'].split('int main(int argc,char **argv)')[0]
fixture=r'''
static unsigned surface_maps,retired_surface_maps,statistics_maps;
static void retire_count_map(ID3D11Resource *b){
 if(b==g.surface.readback)++surface_maps;
 if(b==g.stats.readback)++statistics_maps;
 for(unsigned i=0;i<g.pending_count;i++){
  if(b==g.pending[i].surface){++surface_maps;if(!g.pending[i].publish_surface)++retired_surface_maps;}
  if(b==g.pending[i].stats)++statistics_maps;
 }
}
static void reference_draw(NvGpuState state,NvGpuTri tri,uint8_t *color,float *z,NvGpuCount *count){
 for(unsigned y=0;y<16;y++)for(unsigned x=0;x<16;x++){
  size_t at=gpu_color_at(&state,x,y);uint32_t old;memcpy(&old,color+at,4);
  NvGpuPixel p=gpu_pixel(state,tri,x,y,old,z[y*20+x]);memcpy(color+at,&p.color,4);z[y*20+x]=p.depth;
  count->pixels+=p.pixels;count->depth_fail+=p.depth_fail;count->zpass+=p.zpass;
  if((p.pixel_max&0xFFFFFFu)>(count->pixel_max&0xFFFFFFu))count->pixel_max=p.pixel_max;
 }
}
static void color_tri(NvGpuTri *tri,uint32_t color,float z){
 for(unsigned v=0;v<3;v++){
  tri->v[v].pos[2]=z;for(unsigned c=0;c<4;c++)tri->v[v].d0[c]=((color>>(c==3?24:16-c*8))&255)/255.0f;
 }
}
static void reset_map_counts(void){surface_maps=retired_surface_maps=statistics_maps=all_maps=queued_maps=0;}
int main(int argc,char **argv){
 const char *mode=argc>1?argv[1]:"default";
 ram=mmap(NULL,0x84000000ull,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS|MAP_NORESERVE,-1,0);assert(ram!=MAP_FAILED);g_memory_offset=(ptrdiff_t)ram;
 setenv("RECOMP_GPU_RASTER","1",1);setenv("RECOMP_GPU_ASYNC","0",1);setenv("RECOMP_GPU_FENCE_BATCH","1",1);setenv("RECOMP_GPU_SLICE_PIPELINE","0",1);
 setenv("RECOMP_GPU_DIAGNOSTIC_COUNTERS",!strcmp(mode,"quiet")?"0":"1",1);
 if(!strcmp(mode,"retire-disabled"))setenv("RECOMP_GPU_RETIRE_RESUMED_PIXELS","0",1);
 if(!strcmp(mode,"resume-disabled"))setenv("RECOMP_GPU_TARGET_RESUME","0",1);
 if(!strcmp(mode,"bridge-disabled"))setenv("RECOMP_GPU_TEXTURE_BRIDGE","0",1);
 native_supported=1;coverage_tier=!strcmp(mode,"rectangle")?0:3;assert(nv_gpu_available());
 /* Switching an untouched target must not queue a fake GPU publication. */
 {
  reset_queue();
  NvGpuState idle=queue_state;idle.pitch=80;idle.format=0x128;
  float idle_depth[2][16*20];NvGpuCount idle_count;
  uint64_t skips=g.target_shape[3].empty_skips;
  memset(idle_depth,0,sizeof idle_depth);
  assert(!g.active && !g.pending_count);
  assert(gpu_target(&idle,ram+colors_at(0),idle_depth[0],20)==1);
  assert(gpu_target(&idle,ram+colors_at(1),idle_depth[1],20)==1);
  assert(!g.pending_count && g.target_shape[3].empty_skips==skips+1);
  assert(nv_gpu_sync(&idle_count)==1);
 }
 int retiring=g.resume.enabled && g.resume.retire_pixels;
 assert(retiring==(!strcmp(mode,"default") || !strcmp(mode,"quiet") || !strcmp(mode,"rectangle")));
 NvGpuCount count;
 for(unsigned layout=0;layout<2;layout++)for(unsigned visibility=0;visibility<2;visibility++){
  reset_queue();reset_map_counts();NvGpuState s=queue_state;s.pitch=80;s.format=layout?0x04040228:0x128;s.zpass_enable=visibility;
  uint8_t expected[2][1280];float z[2][16*20],expected_z[2][16*20];NvGpuCount reference={0};
  for(unsigned d=0;d<2;d++){
   memset(ram+colors_at(d),0xA5,1280);memcpy(expected[d],ram+colors_at(d),1280);
   for(unsigned y=0;y<16;y++)for(unsigned x=0;x<20;x++)z[d][y*20+x]=expected_z[d][y*20+x]=x<16?100:-123;
  }
  NvGpuTri t=triangle(16);color_tri(&t,0xFF123456,10);reference_draw(s,t,expected[0],expected_z[0],&reference);
  assert(nv_gpu_draw(&s,&t,1,ram+colors_at(0),z[0],20,empty_sources)==1);
  color_tri(&t,0xFF789ABC,10);reference_draw(s,t,expected[1],expected_z[1],&reference);
  assert(nv_gpu_draw(&s,&t,1,ram+colors_at(1),z[1],20,empty_sources)==1);
  color_tri(&t,0xFF456789,5);t.minx=t.miny=4;t.maxx=t.maxy=8;
  reference_draw(s,t,expected[0],expected_z[0],&reference);
  uint64_t old_retired=g.resume.retired,old_empty=g.resume.empty_retired;
  assert(nv_gpu_draw(&s,&t,1,ram+colors_at(0),z[0],20,empty_sources)==1);
  if(g.resume.enabled){assert(!all_maps && g.pending_count==2);assert(g.pending[0].publish_surface==!retiring);}
  if(retiring){assert(g.resume.retired==old_retired+1 && g.dirty_left==0 && g.dirty_top==0 && g.dirty_right==16 && g.dirty_bottom==16);}
  assert(nv_gpu_sync(&count)==1);
  for(unsigned d=0;d<2;d++)assert(!memcmp(expected[d],ram+colors_at(d),1280) && !memcmp(expected_z[d],z[d],sizeof z[d]));
  gpu_reference_counts(&reference);assert(!memcmp(&reference,&count,sizeof count));
  assert(!retired_surface_maps && surface_maps==(retiring?2u:3u));
  if(!g.native_diagnostics && !visibility){assert(!statistics_maps && (g.resume.empty_retired==old_empty+(retiring?1:0)));}
  else assert(statistics_maps==3);
 }
 puts("PASS: exact linear/Morton output and strided padding survive tiny resumed draws; final depth/visibility/diagnostics match; superseded surface is never mapped");
 if(!retiring){puts("PASS: independent retirement/resume/bridge opt-outs retain all original CPU pixel readbacks");return 0;}
 /* Draining an unrelated queued target must not publish the superseded
  * version or lose its dirty rectangle before the resumed target is queued. */
 reset_queue();reset_map_counts();tint(0xFF112233);draw_target(0);draw_target(1);
 tint(0xFF445566);queue_tri.minx=queue_tri.miny=4;queue_tri.maxx=queue_tri.maxy=8;for(unsigned v=0;v<3;v++)queue_tri.v[v].pos[2]=5;draw_target(0);
 nv2a_pb_exec_guest_access(colors_at(1),4);assert(!g.pending_count && g.active && !MEM(colors_at(0)) && MEM(colors_at(1))==0xFF112233 && !retired_surface_maps);
 draw_target(2);assert(g.pending_count==1 && g.pending[0].publish_surface && g.pending[0].left==0 && g.pending[0].right==16);
 assert(nv_gpu_sync(&count)==1 && MEM(colors_at(0))==0xFF112233 && MEM(colors_at(0)+(5*16+5)*4)==0xFF445566 && !retired_surface_maps);
 puts("PASS: unrelated CPU dependency drains counters only; queued latest target inherits every earlier dirty pixel and depth value");
 /* Resumed CPU reads, even through tiled aliases, complete latest output.
  * The fixture cannot dereference the tiled aperture but its barrier can. */
 for(unsigned alias=0;alias<2;alias++){
  reset_queue();reset_map_counts();tint(0x41800000);draw_target(0);draw_target(1);tint(0x42000000);for(unsigned v=0;v<3;v++)queue_tri.v[v].pos[2]=5;draw_target(0);
  nv2a_pb_exec_guest_access((uint64_t)colors_at(0)+(alias?0x70000000ull:0),4);
  assert(!g.active && !g.pending_count && MEM(colors_at(0))==0x42000000 && !retired_surface_maps && nv_gpu_sync(&count)==1);
 }
 puts("PASS: actual CPU reads and tiled-alias barriers observe the latest active target after superseded pixels are retired");
 /* Existing report reset must keep its pre-reset count discard, including
  * contributions retained from the retired version. */
 reset_queue();tint(0xFF254769);draw_target(0);draw_target(1);draw_target(0);
 assert(nv_gpu_reset_report()==1 && nv_gpu_sync(&count)==1 && !count.zpass && MEM(colors_at(0))==0xFF254769);
 puts("PASS: GPU visibility reset discards retired-version counts without losing final color/depth");
 /* The first queued Map is now the old version's required counters. Failure
  * must still prevent the real guest fence from becoming visible. */
 MEM(0x130000)=0x110000;MEM(0x110030)=0x80070000;assert(!xbox_Nv2aCommandFence(0x130000,0x30,8,32));
 setup(1);reset_queue();reset_map_counts();tint(0xFF36587A);draw_target(0);draw_target(1);tint(0xFF47698B);for(unsigned v=0;v<3;v++)queue_tri.v[v].pos[2]=5;draw_target(0);
 assert(g.pending[0].has_stats && !g.pending[0].publish_surface);
 nv2a_pb_exec_fifo_begin();nv2a_pb_exec_method(0,0x1D70,97);assert(!MEM(0x80070000) && s_pending_fence_count==1);
 fflush(NULL);pid_t child=fork();assert(child>=0);
 if(!child){queued_maps=0;fail_queued_map_at=1;nv2a_pb_exec_fifo_end();_exit(9);}
 int status;assert(waitpid(child,&status,0)==child && WIFEXITED(status) && WEXITSTATUS(status)==4 && !MEM(0x80070000));
 nv2a_pb_exec_fifo_end();assert(MEM(0x80070000)==97 && MEM(colors_at(0))==0xFF47698B && !retired_surface_maps && !g.pending_count && !g.active);
 puts("PASS: counter-only readback failure prevents guest completion; success publishes the latest pixels before the fence");
 nv_gpu_report();puts("PASS: superseded target retirement complete");return 0;
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-target-retire-') as folder:
 tmp=Path(folder)
 for name in ['windows.h','d3d11.h','d3d11_3.h','d3dcompiler.h','dxgi.h']:(tmp/name).write_text('/* fixture API */\n')
 k=root/'third_party/xboxrecomp/src/kernel';includes=''.join('#include "'+str(k/n)+'"\n' for n in ['nv2a_gpu_raster.c','nv2a_pb_exec.c','nv2a_pb_scan.c'])
 code=api+f['shim']+f['fences']+f['reader']+f['wait_shim']+f['wait_body']+includes+f['mock']+f['ack_shim']+ns['head']+helpers+fixture
 (tmp/'test.c').write_text(code);exe=tmp/'test'
 subprocess.run(['gcc','-std=gnu11','-O1','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-Werror=implicit-function-declaration','-Werror=incompatible-pointer-types','-ffunction-sections','-fdata-sections','-I'+str(tmp),'-I'+str(root/'tools'),'-I'+str(k),'-I'+str(root/'third_party/xboxrecomp/src'),str(tmp/'test.c'),str(k/'nv2a_gpu_tiles.c'),str(k/'nv2a_vsh_interp.c'),str(k/'nv2a_combiner.c'),'-Wl,--gc-sections','-pthread','-lm','-o',str(exe)],check=True)
 for mode in ['default','quiet','rectangle','retire-disabled','resume-disabled','bridge-disabled']:
  print('RUN target retire '+mode,flush=True);cwd=tmp/mode;cwd.mkdir();subprocess.run([str(exe),mode],cwd=cwd,check=True,timeout=90)
 print('PASS: all 6 target retirement modes passed')
