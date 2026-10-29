"""Production rendered-texture dependencies with strict, CPU-executed D3D calls.
No Windows driver, gameplay benchmark, or real HLSL compiler is substituted.
"""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1]
base=root/'tools/test_qpr2_readback_queue.py'
s=base.read_text();ns={'__file__':str(base)}
exec(compile(s[:s.index('\nwith tempfile.TemporaryDirectory(')],str(base),'exec'),ns)
f=ns['ns'];api=ns['api']
api=api.replace('static HRESULT ID3D11Device_CreateBuffer(', 'static int fail_bridge_snapshot,fail_bridge_pack;\nstatic HRESULT ID3D11Device_CreateBuffer(',1)
api=api.replace('(void)d;++buffer_creates;', '(void)d;++buffer_creates;if(s->Usage==D3D11_USAGE_DEFAULT && ((fail_bridge_snapshot && s->StructureByteStride==8 && s->BindFlags==D3D11_BIND_SHADER_RESOURCE) || (fail_bridge_pack && s->StructureByteStride==4 && s->BindFlags==(D3D11_BIND_SHADER_RESOURCE|D3D11_BIND_UNORDERED_ACCESS))))return -1;',1)
helpers=ns['fixture'].split('int main(int argc,char **argv)')[0]
fixture=r'''
static unsigned decode_calls;
static int tracked_decode(void *raw,uint32_t face,uint32_t x,uint32_t y,uint32_t *out){++decode_calls;return decode(raw,face,x,y,out);}
static void prepare_consumer(NvGpuTextureSource sources[4],unsigned layout){
 queue_tri.textured=1;queue_state.tex[0].width=queue_state.tex[0].height=16;queue_state.tex[0].valid=1;
 queue_state.tex[0].format=layout==2?0x06:0x12;queue_state.tex[0].addr_u=queue_state.tex[0].addr_v=3;
 memset(sources,0,4*sizeof *sources);sources[0].raw=ram+colors_at(0);sources[0].bytes=1024;
 sources[0].opaque=ram+colors_at(0);sources[0].decode=tracked_decode;sources[0].gpu_argb_layout=layout;
}
static void begin_producer(uint32_t color){reset_queue();tint(color);draw_target(0);}
int main(int argc,char **argv){
 const char *mode=argc>1?argv[1]:"default";
 ram=mmap(NULL,0x84000000ull,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS|MAP_NORESERVE,-1,0);assert(ram!=MAP_FAILED);g_memory_offset=(ptrdiff_t)ram;
 setenv("RECOMP_GPU_RASTER","1",1);setenv("RECOMP_GPU_ASYNC","0",1);setenv("RECOMP_GPU_FENCE_BATCH","1",1);setenv("RECOMP_GPU_SLICE_PIPELINE","0",1);
 setenv("RECOMP_GPU_DIAGNOSTIC_COUNTERS",!strcmp(mode,"quiet")?"0":"1",1);
 if(!strcmp(mode,"disabled"))setenv("RECOMP_GPU_TEXTURE_BRIDGE","0",1);
 bridge_compile_fail=!strcmp(mode,"compile");bridge_create_fail=!strcmp(mode,"create");bridge_corrupt=!strcmp(mode,"corrupt");bridge_map_fail=!strcmp(mode,"map");
 native_supported=1;coverage_tier=!strcmp(mode,"rectangle")?0:3;
 int ready=nv_gpu_available();
 if(bridge_map_fail){assert(!ready && s_startup==3 && !g.pending_count && !g.active && !g.host_color && !g.host_depth);puts("PASS: failed private readback cannot publish readiness or retain private destinations");return 0;}
 assert(ready && g.native && g.readback_queue);
 NvGpuCount count;NvGpuTextureSource sources[4];
 if(!strcmp(mode,"disabled") || bridge_compile_fail || bridge_create_fail || bridge_corrupt){
  assert(!g.bridge.enabled);begin_producer(0xFF123456);prepare_consumer(sources,1);tint(0xFFFFFFFF);decode_calls=0;
  assert(nv_gpu_draw(&queue_state,&queue_tri,1,ram+colors_at(1),queue_depths[1],16,sources)==1);
  assert(all_maps>0 && decode_calls>0 && !g.bound[0]->gpu_serial && MEM(colors_at(0))==0xFF123456);
  assert(nv_gpu_sync(&count)==1 && MEM(colors_at(1))==0xFF123456);
  puts("PASS: opt-out and shader compile/create/comparison failure keep coherent CPU decoding and validated native rendering");return 0;
 }
 assert(g.bridge.enabled && bridge_dispatches>=2);
 /* Real wrapper metadata capture must not publish the producer before the
  * renderer has the layout needed to decide whether its GPU copy is exact. */
 begin_producer(0xFF123456);draw_target(2);assert(g.pending_count==1 && !all_maps);
 memset(&s_gpu,0,sizeof s_gpu);s_gpu.pitch=64;s_gpu.format=0x128;s_gpu.clip_w=s_gpu.clip_h=16;s_gpu.color_mask=0x01010101;
 Texture *tx=&s_gpu.texs[0];tx->offset=colors_at(0);tx->width=tx->height=16;tx->pitch=64;tx->color=0x12;tx->levels=1;tx->valid=1;
 XfTri first={0};first.bpp=4;first.textured=1;NvGpuState captured;
 assert(gpu_capture_draw(&first,&captured,sources) && sources[0].gpu_argb_layout==1 && !all_maps && !MEM(colors_at(0)));
 queue_tri.textured=1;tint(0xFFFFFFFF);
 assert(nv_gpu_draw(&captured,&queue_tri,1,ram+colors_at(1),queue_depths[1],16,sources)==1);
 assert(!all_maps && !MEM(colors_at(0)) && g.bound[0]->gpu_serial);
 uint64_t serial=g.bound[0]->gpu_serial,packs=g.bridge.packs;
 assert(nv_gpu_draw(&captured,&queue_tri,1,ram+colors_at(1),queue_depths[1],16,sources)==1);
 assert(g.bound[0]->gpu_serial==serial && g.bridge.packs==packs && g.bridge.hits>0 && !all_maps);
 assert(nv_gpu_sync(&count)==1 && MEM(colors_at(0))==0xFF123456 && MEM(colors_at(1))==0xFF123456);
 puts("PASS: actual wrapper defers pixel hazards; GPU pack and generation cache hits need no Map, CPU decode or host publication");
 /* Same address, new frame: old serial must never hit. */
 begin_producer(0xFF789ABC);prepare_consumer(sources,1);tint(0xFFFFFFFF);decode_calls=0;
 assert(nv_gpu_draw(&queue_state,&queue_tri,1,ram+colors_at(1),queue_depths[1],16,sources)==1);
 assert(g.bound[0]->gpu_serial!=serial && !all_maps && !decode_calls && !MEM(colors_at(0)));
 assert(nv_gpu_sync(&count)==1 && MEM(colors_at(1))==0xFF789ABC);
 puts("PASS: reusing a host address requires a new immutable snapshot generation");
 /* Conservative matcher rejects format/layout/size/palette/depth ambiguity. */
 begin_producer(0xFF102030);draw_target(1);prepare_consumer(sources,1);
 NvGpuTexture meta=queue_state.tex[0];assert(gpu_bridge_source(&meta,sources));
 sources[0].gpu_argb_layout=0;assert(!gpu_bridge_source(&meta,sources));sources[0].gpu_argb_layout=2;assert(!gpu_bridge_source(&meta,sources));sources[0].gpu_argb_layout=1;
 sources[0].raw++;assert(!gpu_bridge_source(&meta,sources));sources[0].raw--;
 sources[0].bytes+=128;assert(!gpu_bridge_source(&meta,sources));sources[0].bytes-=128;
 sources[0].palette_bytes=4;assert(!gpu_bridge_source(&meta,sources));sources[0].palette_bytes=0;
 meta.cube=1;assert(!gpu_bridge_source(&meta,sources));meta.cube=0;
 g.pending[0].depth_stride=16;float *saved=g.pending[0].host_depth;g.pending[0].host_depth=(float*)sources[0].raw;
 assert(!gpu_bridge_source(&meta,sources));g.pending[0].host_depth=saved;
 sources[0].gpu_argb_layout=0;decode_calls=0;tint(0xFFFFFFFF);
 assert(nv_gpu_draw(&queue_state,&queue_tri,1,ram+colors_at(2),queue_depths[2],16,sources)==1);
 assert(all_maps && decode_calls && !g.bound[0]->gpu_serial && nv_gpu_sync(&count)==1 && MEM(colors_at(2))==0xFF102030);
 puts("PASS: unproven formats, subranges, mip bytes, cubes, palettes and depth aliases retain CPU dependency checks");
 /* Real CPU vertex reads and fences remain completion barriers. */
 begin_producer(0x41800000);prepare_consumer(sources,1);tint(0xFFFFFFFF);
 assert(nv_gpu_draw(&queue_state,&queue_tri,1,ram+colors_at(1),queue_depths[1],16,sources)==1 && !all_maps);
 s_gpu.attr[0].offset=colors_at(0);s_gpu.attr[0].type=2;s_gpu.attr[0].size=4;s_gpu.attr[0].stride=16;float attr[4];fetch_attr(&s_gpu.attr[0],0,attr);
 assert(attr[0]==16 && all_maps && !g.pending_count && nv_gpu_sync(&count)==1);
 MEM(0x130000)=0x110000;MEM(0x110030)=0x80070000;assert(!xbox_Nv2aCommandFence(0x130000,0x30,8,32));
 setup(1);begin_producer(0xFF425364);prepare_consumer(sources,1);tint(0xFFFFFFFF);
 assert(nv_gpu_draw(&queue_state,&queue_tri,1,ram+colors_at(1),queue_depths[1],16,sources)==1 && !all_maps);
 nv2a_pb_exec_fifo_begin();nv2a_pb_exec_method(0,0x1D70,95);assert(!MEM(0x80070000) && s_pending_fence_count==1);
 fflush(NULL);pid_t child=fork();assert(child>=0);
 if(!child){queued_maps=0;fail_queued_map_at=1;nv2a_pb_exec_fifo_end();_exit(9);}
 int status;assert(waitpid(child,&status,0)==child && WIFEXITED(status) && WEXITSTATUS(status)==4 && !MEM(0x80070000));
 nv2a_pb_exec_fifo_end();assert(MEM(0x80070000)==95 && MEM(colors_at(0))==0xFF425364 && MEM(colors_at(1))==0xFF425364 && !g.pending_count);
 puts("PASS: actual vertex consumers drain GPU-only sources; failed readback never publishes a guest fence");
 /* Capture must force coherent CPU data and discard GPU-only cache entries. */
 begin_producer(0xFF567890);prepare_consumer(sources,1);tint(0xFFFFFFFF);
 assert(nv_gpu_draw(&queue_state,&queue_tri,1,ram+colors_at(1),queue_depths[1],16,sources)==1 && g.bound[0]->gpu_serial && !all_maps);
 setenv("RECOMP_GPU_CAPTURE","handoff-capture.bin",1);nv_gpu_capture_request();decode_calls=0;
 assert(nv_gpu_draw(&queue_state,&queue_tri,1,ram+colors_at(1),queue_depths[1],16,sources)==1 && s_capture);
 assert(all_maps && decode_calls && !g.bound[0]->gpu_serial && MEM(colors_at(0))==0xFF567890);
 assert(nv_gpu_sync(&count)==1);gpu_capture_close("test");unsetenv("RECOMP_GPU_CAPTURE");
 puts("PASS: capture synchronizes and redecodes real host pixels instead of serializing GPU-only placeholders");
 /* Force allocation of a different snapshot/scratch size after startup. */
 for(unsigned kind=0;kind<2;kind++){
  reset_queue();uint32_t original[24*24]={0},dest[24*24]={0};float z[24*24],dz[24*24];for(unsigned i=0;i<24*24;i++)z[i]=dz[i]=100;
  NvGpuState state=queue_state;state.width=state.height=24;state.pitch=96;NvGpuTri big=triangle(24);uint32_t *host=original;
  assert(nv_gpu_draw(&state,&big,1,(uint8_t*)host,z,24,empty_sources)==1);
  fail_bridge_snapshot=kind==0;fail_bridge_pack=kind==1;
  state.tex[0].width=state.tex[0].height=24;state.tex[0].valid=1;state.tex[0].format=0x12;state.tex[0].addr_u=state.tex[0].addr_v=3;big.textured=1;
  NvGpuTextureSource src[4]={0};src[0].raw=(uint8_t*)host;src[0].bytes=sizeof original;src[0].opaque=host;src[0].decode=decode;src[0].gpu_argb_layout=1;
  /* All sampled coordinates are zero: decode()'s fixture row width is 16. */
  for(unsigned v=0;v<3;v++)big.uv[v][0]=big.uv[v][1]=0;
  assert(nv_gpu_draw(&state,&big,1,(uint8_t*)dest,dz,24,src)==1 && !g.bridge.enabled && original[0]==0xFFFFFFFF);
  assert(nv_gpu_sync(&count)==1 && dest[0]==0xFFFFFFFF && !g.readback_failed);
  fail_bridge_snapshot=fail_bridge_pack=0;g.bridge.enabled=1;
 }
 puts("PASS: snapshot and pack allocation failures preserve accepted work through coherent CPU fallback");
 nv_gpu_report();puts("PASS: rendered-texture handoff integration complete");return 0;
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-texture-bridge-') as folder:
 tmp=Path(folder)
 for name in ['windows.h','d3d11.h','d3d11_3.h','d3dcompiler.h','dxgi.h']:(tmp/name).write_text('/* fixture API */\n')
 kernel=root/'third_party/xboxrecomp/src/kernel'
 includes=''.join('#include "'+str(kernel/n)+'"\n' for n in ['nv2a_gpu_raster.c','nv2a_pb_exec.c','nv2a_pb_scan.c'])
 code=api+f['shim']+f['fences']+f['reader']+f['wait_shim']+f['wait_body']+includes+f['mock']+f['ack_shim']+ns['head']+helpers+fixture
 (tmp/'test.c').write_text(code);exe=tmp/'test'
 subprocess.run(['gcc','-std=gnu11','-O1','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-Werror=implicit-function-declaration','-Werror=incompatible-pointer-types','-ffunction-sections','-fdata-sections','-I'+str(tmp),'-I'+str(root/'tools'),'-I'+str(kernel),'-I'+str(root/'third_party/xboxrecomp/src'),str(tmp/'test.c'),str(kernel/'nv2a_gpu_tiles.c'),str(kernel/'nv2a_vsh_interp.c'),str(kernel/'nv2a_combiner.c'),'-Wl,--gc-sections','-pthread','-lm','-o',str(exe)],check=True)
 for mode in ['default','quiet','rectangle','disabled','compile','create','corrupt','map']:
  print('RUN texture bridge '+mode,flush=True)
  # Isolate DXBC fixture caches so injected compile/create faults execute.
  cwd=tmp/mode;cwd.mkdir()
  subprocess.run([str(exe),mode],cwd=cwd,check=True,timeout=90)
 print('PASS: all 8 rendered-texture bridge modes passed')
