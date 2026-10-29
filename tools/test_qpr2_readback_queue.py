"""Production D3D/FIFO dependency tests for queued native target copies.

This uses a strict D3D API shim and shared scalar pixels, not a Windows GPU.
"""
from pathlib import Path
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
fence_test = root / "tools/test_qpr2_fence_batch.py"
source = fence_test.read_text()
ns = {"__file__": str(fence_test)}
# Reuse fixture declarations; omit its compiler/runner block.
exec(compile(source[:source.index("with tempfile.TemporaryDirectory(")], str(fence_test), "exec"), ns)
api = ns["api"]
api = api.replace("static HRESULT ID3D11Device_CreateBuffer(", "static unsigned queued_maps, all_maps, copy_calls, fail_queued_map_at, fail_staging_bytes;\nstatic int corrupt_queued_copy;\nstatic int fixture_is_queued(ID3D11Resource*);\nstatic HRESULT ID3D11Device_CreateBuffer(", 1)
api = api.replace("(void)d;++buffer_creates;", "(void)d;++buffer_creates;if(fail_staging_bytes && s->Usage==D3D11_USAGE_STAGING && s->ByteWidth==fail_staging_bytes)return -1;", 1)
api = api.replace("(void)c;assert(dst->desc.ByteWidth==src->desc.ByteWidth);", "(void)c;if(dst->desc.Usage==D3D11_USAGE_STAGING)++copy_calls;assert(dst->desc.ByteWidth==src->desc.ByteWidth);", 1)
api = api.replace("memcpy(dst->data,src->data,src->desc.ByteWidth);", "memcpy(dst->data,src->data,src->desc.ByteWidth);if(corrupt_queued_copy && fixture_is_queued(dst))dst->data[0]^=0x80;", 1)
api = api.replace("(void)c;if(fail_next_map)", "(void)c;++all_maps;if(fixture_is_queued(b) && ++queued_maps==fail_queued_map_at)return -1;if(fail_next_map)", 1)
fixture = r'''
static int fixture_is_queued(ID3D11Resource *buffer){
 for(unsigned i=0;i<NV_GPU_PENDING_READBACKS;i++)if(buffer==g.pending[i].surface || buffer==g.pending[i].stats)return 1;
 return 0;
}
static uint32_t colors_at(unsigned i){return 0x80090000u+i*0x10000u;}
static float queue_depths[8][256];
static NvGpuState queue_state;
static NvGpuTri queue_tri;
static NvGpuTextureSource empty_sources[4];
static void reset_queue(void){
 NvGpuCount ignored;assert(nv_gpu_sync(&ignored)==1);
 memset(&queue_state,0,sizeof queue_state);queue_state.width=queue_state.height=16;
 queue_state.pitch=64;queue_state.bpp=4;queue_state.format=0x128;queue_state.color_mask=0x01010101;
 queue_state.depth_available=queue_state.depth_test=queue_state.depth_mask=queue_state.zpass_enable=1;queue_state.depth_func=0x203;
 queue_tri=triangle(16);
 for(unsigned i=0;i<8;i++){memset(ram+colors_at(i),0,1024);for(unsigned at=0;at<256;at++)queue_depths[i][at]=100;}
 all_maps=queued_maps=copy_calls=0;
}
static void draw_target(unsigned i){assert(nv_gpu_draw(&queue_state,&queue_tri,1,ram+colors_at(i),queue_depths[i],16,empty_sources)==1);}
static void tint(uint32_t argb){
 for(unsigned k=0;k<3;k++){queue_tri.v[k].d0[0]=((argb>>16)&255)/255.0f;queue_tri.v[k].d0[1]=((argb>>8)&255)/255.0f;queue_tri.v[k].d0[2]=(argb&255)/255.0f;queue_tri.v[k].d0[3]=(argb>>24)/255.0f;}
}
static int palette_decode(void *opaque,uint32_t face,uint32_t x,uint32_t y,uint32_t *out){
 (void)face;const uint8_t *indices=opaque;*out=MEM(colors_at(0)+indices[y*16+x]*4);return 1;
}
int main(int argc,char **argv){
 const char *mode=argc>1?argv[1]:"default";
 ram=mmap(NULL,0x84000000ull,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS|MAP_NORESERVE,-1,0);assert(ram!=MAP_FAILED);g_memory_offset=(ptrdiff_t)ram;
 setenv("RECOMP_GPU_RASTER","1",1);setenv("RECOMP_GPU_ASYNC","0",1);setenv("RECOMP_GPU_FENCE_BATCH","1",1);setenv("RECOMP_GPU_SLICE_PIPELINE","0",1);
 setenv("RECOMP_GPU_DIAGNOSTIC_COUNTERS",!strcmp(mode,"quiet")?"0":"1",1);
 if(!strcmp(mode,"disabled"))setenv("RECOMP_GPU_READBACK_QUEUE","0",1);else unsetenv("RECOMP_GPU_READBACK_QUEUE");
 if(!strcmp(mode,"gate-corrupt"))corrupt_queued_copy=1;
 if(!strcmp(mode,"gate-map"))fail_queued_map_at=1;
 native_supported=1;coverage_tier=3;
 int ready=nv_gpu_available();
 if(!strcmp(mode,"gate-map")){assert(!ready && s_startup==3 && !g.pending_count && !g.host_color && !g.host_depth);puts("PASS: queued startup Map failure prevents hardware readiness and drops all private stack destinations");return 0;}
 assert(ready && g.native);
 if(!strcmp(mode,"gate-corrupt")){assert(!g.readback_queue && !g.pending_count);puts("PASS: corrupted queued startup copies reject only this optimization and retain validated synchronous hardware");return 0;}
 reset_queue();
 if(!strcmp(mode,"disabled")){assert(!g.readback_queue);draw_target(0);draw_target(1);assert(all_maps==2 && !g.pending_count && MEM(colors_at(0))==0xFFFFFFFF);puts("PASS: explicit opt-out retains synchronous target-switch completion");return 0;}
 assert(g.readback_queue);
 for(unsigned i=0;i<5;i++){
  tint(0xFF112233+i*0x10101);draw_target(i);
  assert(all_maps==0 && g.pending_count==i && !MEM(colors_at(0)));
 }
 assert(copy_calls==(g.deferred_readback.enabled && g.bridge.enabled?4u:8u));tint(0xFF667788);draw_target(5);
 assert(all_maps==8 && g.pending_count==1 && MEM(colors_at(0))==0xFF112233);
 NvGpuCount count;assert(nv_gpu_sync(&count)==1 && !g.pending_count);
 for(unsigned i=0;i<6;i++)assert(MEM(colors_at(i))==(i==5?0xFF667788:0xFF112233+i*0x10101) && queue_depths[i][0]==10);
 assert(count.zpass>0 && (g.native_diagnostics?count.pixels>0:!count.pixels));
 puts("PASS: five independent targets retain exact snapshots with zero Maps; bounded queue drains on capacity; immutable snapshots and visibility counts survive GPU-buffer reuse");
 /* Dirty bounds, 16-bit conversion, Morton layouts and row padding. */
 for(unsigned layout=0;layout<4;layout++){
  reset_queue();unsigned bpp=(layout&1)?2:4;
  queue_state.bpp=bpp;queue_state.pitch=16*bpp+((layout&2)?0:8);
  queue_state.format=(layout&2)?((bpp==2)?0x04040221:0x04040228):((bpp==2)?0x121:0x128);
  queue_tri.minx=4;queue_tri.maxx=12;queue_tri.miny=3;queue_tri.maxy=10;
  uint8_t expected_bytes[1280];float padded_depth[16*20],expected_depth[16*20];
  memset(ram+colors_at(0),0xA5,sizeof expected_bytes);memcpy(expected_bytes,ram+colors_at(0),sizeof expected_bytes);
  for(unsigned y=0;y<16;y++)for(unsigned x=0;x<20;x++)padded_depth[y*20+x]=expected_depth[y*20+x]=x<16?100:-123;
  for(unsigned y=0;y<16;y++)for(unsigned x=0;x<16;x++){
   size_t at=gpu_color_at(&queue_state,x,y);uint32_t original;
   if(bpp==4)memcpy(&original,expected_bytes+at,4);
   else {uint16_t v;memcpy(&v,expected_bytes+at,2);original=0xFF000000u|((v&0xF800u)<<8)|((v&0x07E0u)<<5)|((v&31u)<<3);}
   NvGpuPixel px=gpu_pixel(queue_state,queue_tri,(int)x,(int)y,original,100);expected_depth[y*20+x]=px.depth;
   if(bpp==4)memcpy(expected_bytes+at,&px.color,4);
   else {uint32_t v=px.color;uint16_t packed=(uint16_t)(((v>>8)&0xF800)|((v>>5)&0x07E0)|((v>>3)&31));memcpy(expected_bytes+at,&packed,2);}
  }
  assert(nv_gpu_draw(&queue_state,&queue_tri,1,ram+colors_at(0),padded_depth,20,empty_sources)==1);
  draw_target(1);assert(g.pending_count==1 && !all_maps);
  assert(nv_gpu_sync(&count)==1);
  assert(!memcmp(expected_bytes,ram+colors_at(0),sizeof expected_bytes) && !memcmp(expected_depth,padded_depth,sizeof expected_depth));
 }
 puts("PASS: queued output preserves exact linear/swizzled 16/32-bit color, dirty rectangles, color padding and strided depth padding");
 /* Shared depth and shifted color aliases must synchronize before upload. */
 reset_queue();draw_target(0);
 assert(nv_gpu_draw(&queue_state,&queue_tri,1,ram+colors_at(1),queue_depths[0],16,empty_sources)==1);
 assert(all_maps==2 && !g.pending_count && MEM(colors_at(0))==0xFFFFFFFF && queue_depths[0][0]==10);
 assert(nv_gpu_sync(&count)==1);
 reset_queue();draw_target(0);queue_state.color_mask=0;
 assert(nv_gpu_draw(&queue_state,&queue_tri,1,ram+colors_at(0)+4,queue_depths[1],16,empty_sources)==1);
 assert(all_maps==2 && !g.pending_count && nv_gpu_sync(&count)==1 && MEM(colors_at(0)+4)==0xFFFFFFFF);
 puts("PASS: shared depth and partially overlapping color targets drain before reading completed CPU mirrors");
 /* Raw and palette sampling of the previous target cannot see stale pixels. */
 for(unsigned palette=0;palette<2;palette++){
  reset_queue();draw_target(0);queue_tri.textured=1;
  queue_state.tex[0].width=queue_state.tex[0].height=16;queue_state.tex[0].valid=1;
  queue_state.tex[0].format=palette?0x0B:0x12;queue_state.tex[0].addr_u=queue_state.tex[0].addr_v=3;
  uint8_t indices[256]={0};NvGpuTextureSource sources[4]={0};
  sources[0].raw=palette?indices:ram+colors_at(0);sources[0].bytes=palette?256:1024;
  sources[0].opaque=palette?(void*)indices:ram+colors_at(0);sources[0].decode=palette?palette_decode:decode;
  if(palette){sources[0].palette=ram+colors_at(0);sources[0].palette_bytes=1024;}
  assert(nv_gpu_draw(&queue_state,&queue_tri,1,ram+colors_at(1),queue_depths[1],16,sources)==1);
  assert(all_maps==2 && MEM(colors_at(0))==0xFFFFFFFF && !g.pending_count);
  assert(nv_gpu_sync(&count)==1 && MEM(colors_at(1))==0xFFFFFFFF);
 }
 puts("PASS: render-target raw/palette texture dependencies drain before cache validation and decoding");
 /* CPU consumers use the actual executor barrier, including tiled aliases. */
 for(unsigned alias=0;alias<2;alias++){
  reset_queue();draw_target(0);draw_target(1);assert(g.pending_count==1 && !MEM(colors_at(0)));
  nv2a_pb_exec_guest_access(colors_at(0)+(alias?0x70000000ull:0),4);
  assert(!g.pending_count && all_maps==2 && MEM(colors_at(0))==0xFFFFFFFF);
 }
 reset_queue();tint(0x41800000);draw_target(0);draw_target(1);
 s_gpu.attr[0].offset=colors_at(0);s_gpu.attr[0].type=2;s_gpu.attr[0].size=4;s_gpu.attr[0].stride=16;
 float attributes[4];fetch_attr(&s_gpu.attr[0],0,attributes);
 assert(attributes[0]==16 && all_maps==2 && !g.pending_count);
 MEM(0x130000)=0x110000;MEM(0x110030)=0x80070000;assert(!xbox_Nv2aCommandFence(0x130000,0x30,8,32));
 reset_queue();tint(0x00041D70);draw_target(0);draw_target(1);MEM(0x80070000)=0;
 s_gpu.semaphore_context=8;s_gpu.semaphore_offset=0;memset(&s_fifo,0,sizeof s_fifo);
 uint32_t at=colors_at(0);assert(nv2a_pb_execute_fifo(at,at+8,at,at+16)==at+8);
 assert(MEM(0x80070000)==0x00041D70 && !g.pending_count && !s_pending_fence_count);
 puts("PASS: actual command decoder and vertex fetch observe completed target bytes; tiled guest aliases cannot bypass dependency drains");
 reset_queue();draw_target(0);draw_target(1);
 assert(!nv_gpu_cpu_write_disjoint(ram+colors_at(0),4));
 assert(!nv_gpu_cpu_write_disjoint((void*)((uintptr_t)ram+colors_at(0)+0x70000000ull),4));
 assert(nv_gpu_cpu_write_disjoint(ram+0x801F0000,4));
 assert(nv_gpu_reset_report()==1 && !g.pending_count && !g.completed.zpass);
 uint32_t expected_zpass=0;
 for(int y=0;y<16;y++)for(int x=0;x<16;x++)expected_zpass+=gpu_pixel(queue_state,queue_tri,x,y,0,10).zpass;
 draw_target(1);assert(nv_gpu_sync(&count)==1 && count.zpass==expected_zpass);
 puts("PASS: CPU clear disjointness includes queued outputs; report reset discards all pre-reset queued visibility counts");
 /* Allocation failure before enqueue safely returns to existing sync copies. */
 reset_queue();uint32_t large[24*24]={0};float zlarge[24*24];for(unsigned i=0;i<24*24;i++)zlarge[i]=100;
 NvGpuState large_state=queue_state;large_state.width=large_state.height=24;large_state.pitch=96;NvGpuTri big=triangle(24);
 assert(nv_gpu_draw(&large_state,&big,1,(uint8_t*)large,zlarge,24,empty_sources)==1);
 fail_staging_bytes=24*24*8;draw_target(0);fail_staging_bytes=0;
 assert(large[0]==0xFFFFFFFF && !g.pending_count && !g.readback_failed && !g.readback_queue && nv_gpu_sync(&count)==1);
 g.readback_queue=1; /* Re-enable the already validated path for fault injection. */
 puts("PASS: staging allocation failure before enqueue retains accepted rendering through synchronous fallback");
 /* A later queued Map failure must not publish even the first guest fence. */
 for(unsigned failed_map=1;failed_map<=3;failed_map+=2){
  setup(1);reset_queue();release_count=0;draw_target(0);draw_target(1);draw_target(2);
  nv2a_pb_exec_fifo_begin();nv2a_pb_exec_method(0,0x1D70,91);
  assert(!MEM(0x80070000) && g.pending_count==2 && s_pending_fence_count==1);
  fflush(NULL);pid_t child=fork();assert(child>=0);
  if(!child){queued_maps=0;fail_queued_map_at=failed_map;nv2a_pb_exec_fifo_end();_exit(9);}
  int status;assert(waitpid(child,&status,0)==child && WIFEXITED(status) && WEXITSTATUS(status)==4 && !MEM(0x80070000));
  nv2a_pb_exec_fifo_end();assert(MEM(0x80070000)==91 && !g.pending_count);
 }
 puts("PASS: first/later queued readback failure exits before any guest completion publication; ordered success drains every target before release");
}
'''
head = ns["fixture"].split("int main(int argc,char **argv)")[0]
with tempfile.TemporaryDirectory(prefix="qpr2-readback-queue-") as folder:
    tmp = Path(folder)
    for name in ["windows.h", "d3d11.h", "d3d11_3.h", "d3dcompiler.h", "dxgi.h"]:
        (tmp / name).write_text("/* fixture API */\n")
    kernel = root / "third_party/xboxrecomp/src/kernel"
    includes = "".join('#include "' + str(kernel / n) + '"\n' for n in ["nv2a_gpu_raster.c", "nv2a_pb_exec.c", "nv2a_pb_scan.c"])
    code = api + ns["shim"] + ns["fences"] + ns["reader"] + ns["wait_shim"] + ns["wait_body"] + includes + ns["mock"] + ns["ack_shim"] + head + fixture
    (tmp / "test.c").write_text(code)
    exe = tmp / "test"
    subprocess.run(["gcc", "-std=gnu11", "-O1", "-fsanitize=undefined,float-cast-overflow", "-fno-sanitize-recover=all", "-Werror=implicit-function-declaration", "-Werror=incompatible-pointer-types", "-ffunction-sections", "-fdata-sections", "-I" + str(tmp), "-I" + str(root / "tools"), "-I" + str(kernel), "-I" + str(root / "third_party/xboxrecomp/src"), str(tmp / "test.c"), str(kernel / "nv2a_gpu_tiles.c"), str(kernel / "nv2a_vsh_interp.c"), str(kernel / "nv2a_combiner.c"), "-Wl,--gc-sections", "-pthread", "-lm", "-o", str(exe)], check=True)
    for mode in ["default", "quiet", "disabled", "gate-corrupt", "gate-map"]:
        print("RUN readback queue " + mode, flush=True)
        subprocess.run([str(exe), mode], cwd=tmp, check=True, timeout=90)
    print("PASS: all 5 readback queue modes passed")
