"""Exact pending-target resume and CPU/fence hazards in production C.
Strict D3D calls execute shared scalar pixels; no Windows hardware FPS claim.
"""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1];base=root/'tools/test_qpr2_readback_queue.py'
s=base.read_text();ns={'__file__':str(base)}
exec(compile(s[:s.index('\nwith tempfile.TemporaryDirectory(')],str(base),'exec'),ns)
f=ns['ns'];api=ns['api']
api=api.replace('static void ID3D11DeviceContext_CopyResource(', 'static int resume_corrupt,resume_map_fail;\nstatic int fixture_resume_testing(void);\nstatic void ID3D11DeviceContext_CopyResource(',1)
# Corrupt only the restore operation, not the already-validated bridge/queue.
old='memcpy(dst->data,src->data,src->desc.ByteWidth);'
new=old+'if(fixture_resume_testing() && dst->desc.Usage==D3D11_USAGE_DEFAULT && dst->desc.BindFlags==D3D11_BIND_UNORDERED_ACCESS){if(resume_corrupt)dst->data[resume_corrupt==2?4:0]^=0x40;if(resume_map_fail)fail_next_map=1;}'
assert old in api;api=api.replace(old,new,1)
# Isolate the baseline allocator so startup/runtime exhaustion is testable.
helpers=ns['fixture'].split('int main(int argc,char **argv)')[0]
fixture=r'''
static int fail_resume_malloc;
static void *resume_fixture_alloc(size_t bytes){
 if((fail_resume_malloc==1 && g.resume.testing && bytes==16*16*8) || (fail_resume_malloc==2 && bytes==24*24*8))return NULL;
 return malloc(bytes);
}
static int fixture_resume_testing(void){return g.resume.testing;}
static void begin_resume(uint32_t argb){reset_queue();tint(argb);draw_target(0);draw_target(1);assert(g.pending_count==1 && !all_maps && !MEM(colors_at(0)));}
static void consume_source(unsigned address){nv2a_pb_exec_guest_access(address,4);}
int main(int argc,char **argv){
 const char *mode=argc>1?argv[1]:"default";
 ram=mmap(NULL,0x84000000ull,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS|MAP_NORESERVE,-1,0);assert(ram!=MAP_FAILED);g_memory_offset=(ptrdiff_t)ram;
 setenv("RECOMP_GPU_RASTER","1",1);setenv("RECOMP_GPU_ASYNC","0",1);setenv("RECOMP_GPU_FENCE_BATCH","1",1);setenv("RECOMP_GPU_SLICE_PIPELINE","0",1);
 setenv("RECOMP_GPU_DIAGNOSTIC_COUNTERS",!strcmp(mode,"quiet")?"0":"1",1);
 if(!strcmp(mode,"disabled"))setenv("RECOMP_GPU_TARGET_RESUME","0",1);
 if(!strcmp(mode,"no-bridge"))setenv("RECOMP_GPU_TEXTURE_BRIDGE","0",1);
 fail_resume_malloc=!strcmp(mode,"allocation");
 resume_corrupt=!strcmp(mode,"color")?1:(!strcmp(mode,"depth")?2:0);resume_map_fail=!strcmp(mode,"map");
 native_supported=1;coverage_tier=!strcmp(mode,"rectangle")?0:3;
 int ready=nv_gpu_available();
 if(resume_map_fail){assert(!ready && s_startup==3 && !g.pending_count && !g.active && !g.host_color && !g.host_depth);puts("PASS: resume startup readback failure cannot publish readiness or retain private host buffers");return 0;}
 assert(ready && g.native && g.readback_queue);
 NvGpuCount count;
 if(resume_corrupt || fail_resume_malloc || !strcmp(mode,"disabled") || !strcmp(mode,"no-bridge")){
  assert(!g.resume.enabled);begin_resume(0xFF123456);draw_target(0);
  assert(!g.resume.restores && all_maps && MEM(colors_at(0))==0xFF123456 && nv_gpu_sync(&count)==1);
  puts("PASS: color/depth comparison failure and opt-out retain validated native rendering with CPU target dependencies");return 0;
 }
 assert(g.resume.enabled && g.bridge.enabled);
 /* Actual binding wrapper only stores a pointer; no CPU pixels are read. */
 begin_resume(0xFF123456);memset(&s_gpu,0,sizeof s_gpu);s_gpu.color_offset=colors_at(0)-0x80000000u;
 s_gpu.pitch=64;s_gpu.clip_w=s_gpu.clip_h=16;s_gpu.format=0x128;
 assert(surface_begin_batch(ram) && s_surface==ram+colors_at(0) && !all_maps && !MEM(colors_at(0)));
 uint64_t restored=g.resume.restores;tint(0xFFAABBCC);for(unsigned v=0;v<3;v++)queue_tri.v[v].pos[2]=20;
 draw_target(0);assert(g.resume.restores==restored+1 && !all_maps && g.pending_count==2 && !MEM(colors_at(0)) && !g.resident);
 assert(nv_gpu_sync(&count)==1 && MEM(colors_at(0))==0xFF123456 && queue_depths[0][0]==10 && MEM(colors_at(1))==0xFF123456);
 puts("PASS: binding wrapper plus exact resume avoids Map/publication; saved depth rejects a farther draw; final publication and visibility remain ordered");
 /* Reject host writes both before and after the queued snapshot exists. */
 for(unsigned during=0;during<2;during++)for(unsigned depth_edit=0;depth_edit<2;depth_edit++){
  reset_queue();tint(0xFF314253);draw_target(0);
  if(!during)draw_target(1);
  if(depth_edit)queue_depths[0][0]=99;else MEM(colors_at(0))=0xFF998877;
  if(during)draw_target(1);
  restored=g.resume.restores;draw_target(0);
  assert(g.resume.restores==restored && all_maps && nv_gpu_sync(&count)==1 && MEM(colors_at(0))==0xFF314253);
 }
 puts("PASS: CPU color/depth edits during the original pass and after enqueue reject snapshot reuse");
 /* Layout/alias/shared-depth ambiguity cannot qualify. */
 for(unsigned kind=0;kind<4;kind++){
  begin_resume(0xFF425364);restored=g.resume.restores;
  uint8_t *destination=ram+colors_at(0);float *z=queue_depths[0];
  if(kind==0)destination+=4;
  if(kind==1)queue_state.format=0x04040228;
  if(kind==2)z=queue_depths[1];
  if(kind==3)queue_state.pitch=68;
  assert(nv_gpu_draw(&queue_state,&queue_tri,1,destination,z,16,empty_sources)==1);
  assert(g.resume.restores==restored && all_maps && nv_gpu_sync(&count)==1);
 }
 reset_queue();tint(0xFF123456);draw_target(0);restored=g.resume.restores;
 assert(nv_gpu_draw(&queue_state,&queue_tri,1,ram+colors_at(1),queue_depths[0],16,empty_sources)==1);
 assert(g.resume.restores==restored && all_maps && !g.pending_count && nv_gpu_sync(&count)==1);
 puts("PASS: shifted aliases, changed layout/pitch, different depth and shared-depth writers retain normal dependency drains");
 /* A second outstanding version of one target is intentionally ambiguous. */
 begin_resume(0xFF536475);draw_target(0);assert(!all_maps);draw_target(2);restored=g.resume.restores;
 draw_target(0);assert(g.resume.restores==restored && all_maps && nv_gpu_sync(&count)==1);
 puts("PASS: multiple queued versions of the same target force ordered CPU publication");
 /* Active resumed writes must be visible to real CPU reads, even after an
  * unrelated dependency has already consumed the older queued version. */
 for(unsigned drained=0;drained<2;drained++){
  begin_resume(0x41800000);tint(0x42000000);for(unsigned v=0;v<3;v++)queue_tri.v[v].pos[2]=5;draw_target(0);assert(!all_maps);
  if(drained){consume_source(colors_at(1));assert(!g.pending_count && g.active);}
  s_gpu.attr[0].offset=colors_at(0);s_gpu.attr[0].type=2;s_gpu.attr[0].size=4;s_gpu.attr[0].stride=16;
  float attr[4];fetch_attr(&s_gpu.attr[0],0,attr);assert(attr[0]==32 && !g.active && !g.pending_count && nv_gpu_sync(&count)==1);
 }
 puts("PASS: actual CPU vertex reads see latest resumed writes before/after older snapshots have drained");
 begin_resume(0xFF173859);tint(0xFF28496A);for(unsigned v=0;v<3;v++)queue_tri.v[v].pos[2]=5;draw_target(0);
 assert(nv_gpu_guest_access(UINT64_MAX-1,8)==1 && !g.active && !g.pending_count && MEM(colors_at(0))==0xFF28496A && nv_gpu_sync(&count)==1);
 puts("PASS: invalid/overflowing CPU ranges conservatively complete the latest resumed target");
 begin_resume(0xFF395A7B);tint(0xFF4A6B8C);for(unsigned v=0;v<3;v++)queue_tri.v[v].pos[2]=5;draw_target(0);
 assert(nv_gpu_reset_report()==1 && nv_gpu_sync(&count)==1 && !count.zpass && MEM(colors_at(0))==0xFF4A6B8C);
 puts("PASS: visibility reset drops all earlier resumed counts while preserving rendered color/depth");
 begin_resume(0xFF5B7C9D);tint(0xFF6C8DAE);for(unsigned v=0;v<3;v++)queue_tri.v[v].pos[2]=5;draw_target(0);
 restored=g.resume.restores;setenv("RECOMP_GPU_CAPTURE","resume-capture.bin",1);nv_gpu_capture_request();draw_target(0);
 assert(s_capture && !g.pending_count && all_maps && g.resume.restores==restored && MEM(colors_at(0))==0xFF6C8DAE);
 assert(nv_gpu_sync(&count)==1);gpu_capture_close("test");unsetenv("RECOMP_GPU_CAPTURE");
 puts("PASS: capture completes latest resumed pixels and uses coherent CPU input instead of replaying stale mirrors");

 /* Texture handoff and target resume can coexist without downloading either. */
 begin_resume(0xFF647586);NvGpuTextureSource src[4]={{0}};
 queue_state.tex[0].width=queue_state.tex[0].height=16;queue_state.tex[0].valid=1;queue_state.tex[0].format=0x12;queue_state.tex[0].addr_u=queue_state.tex[0].addr_v=3;
 src[0].raw=ram+colors_at(1);src[0].bytes=1024;src[0].opaque=ram+colors_at(1);src[0].decode=decode;src[0].gpu_argb_layout=1;
 queue_tri.textured=1;tint(0xFFFFFFFF);for(unsigned v=0;v<3;v++)queue_tri.v[v].pos[2]=5;
 restored=g.resume.restores;
 assert(nv_gpu_draw(&queue_state,&queue_tri,1,ram+colors_at(0),queue_depths[0],16,src)==1);
 assert(!all_maps && g.resume.restores==restored+1 && g.bound[0]->gpu_serial && nv_gpu_sync(&count)==1 && MEM(colors_at(0))==0xFF647586);
 puts("PASS: GPU texture producer can feed a resumed target with no intermediate Map");
 /* Real guest fence failure must never expose success or intermediate data. */
 MEM(0x130000)=0x110000;MEM(0x110030)=0x80070000;assert(!xbox_Nv2aCommandFence(0x130000,0x30,8,32));
 setup(1);begin_resume(0xFF758697);tint(0xFF8697A8);for(unsigned v=0;v<3;v++)queue_tri.v[v].pos[2]=5;draw_target(0);
 nv2a_pb_exec_fifo_begin();nv2a_pb_exec_method(0,0x1D70,96);assert(!MEM(0x80070000) && s_pending_fence_count==1 && !all_maps);
 fflush(NULL);pid_t child=fork();assert(child>=0);
 if(!child){queued_maps=0;fail_queued_map_at=1;nv2a_pb_exec_fifo_end();_exit(9);}
 int status;assert(waitpid(child,&status,0)==child && WIFEXITED(status) && WEXITSTATUS(status)==4 && !MEM(0x80070000));
 nv2a_pb_exec_fifo_end();assert(MEM(0x80070000)==96 && MEM(colors_at(0))==0xFF8697A8 && !g.pending_count && !g.active);
 puts("PASS: resume preserves final fence ordering; failed required readback exits before guest completion");
 /* Baseline allocation failure after real accepted draws must leave both
  * target versions publishable through the prior path. */
 reset_queue();uint32_t large[2][24*24]={{0}};float lz[2][24*24];for(unsigned d=0;d<2;d++)for(unsigned i=0;i<24*24;i++)lz[d][i]=100;
 NvGpuState ls=queue_state;ls.width=ls.height=24;ls.pitch=96;NvGpuTri lt=triangle(24);
 assert(nv_gpu_draw(&ls,&lt,1,(uint8_t*)large[0],lz[0],24,empty_sources)==1);fail_resume_malloc=2;
 assert(nv_gpu_draw(&ls,&lt,1,(uint8_t*)large[1],lz[1],24,empty_sources)==1 && !g.resume.enabled && !g.readback_failed);
 assert(nv_gpu_sync(&count)==1 && large[0][0]==0xFFFFFFFF && large[1][0]==0xFFFFFFFF);
 puts("PASS: runtime CPU baseline allocation failure preserves accepted GPU work through normal completion");
 nv_gpu_report();puts("PASS: target resume integration complete");return 0;
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-target-resume-') as folder:
 tmp=Path(folder)
 for name in ['windows.h','d3d11.h','d3d11_3.h','d3dcompiler.h','dxgi.h']:(tmp/name).write_text('/* fixture API */\n')
 k=root/'third_party/xboxrecomp/src/kernel';includes=''.join('#include "'+str(k/n)+'"\n' for n in ['nv2a_gpu_raster.c','nv2a_pb_exec.c','nv2a_pb_scan.c'])
 code=api+'\nstatic void *resume_fixture_alloc(size_t);\n#define malloc resume_fixture_alloc\n'+f['shim']+f['fences']+f['reader']+f['wait_shim']+f['wait_body']+includes+'\n#undef malloc\n'+f['mock']+f['ack_shim']+ns['head']+helpers+fixture
 (tmp/'test.c').write_text(code);exe=tmp/'test'
 subprocess.run(['gcc','-std=gnu11','-O1','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-Werror=implicit-function-declaration','-Werror=incompatible-pointer-types','-ffunction-sections','-fdata-sections','-I'+str(tmp),'-I'+str(root/'tools'),'-I'+str(k),'-I'+str(root/'third_party/xboxrecomp/src'),str(tmp/'test.c'),str(k/'nv2a_gpu_tiles.c'),str(k/'nv2a_vsh_interp.c'),str(k/'nv2a_combiner.c'),'-Wl,--gc-sections','-pthread','-lm','-o',str(exe)],check=True)
 for mode in ['default','quiet','rectangle','disabled','no-bridge','color','depth','allocation','map']:
  print('RUN target resume '+mode,flush=True);cwd=tmp/mode;cwd.mkdir();subprocess.run([str(exe),mode],cwd=cwd,check=True,timeout=90)
 print('PASS: all 9 target resume modes passed')
