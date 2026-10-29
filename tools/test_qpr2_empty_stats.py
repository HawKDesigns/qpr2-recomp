"""Production counter liveness and D3D traffic, without changing pixel work.

This strict API fixture executes scalar pixel arithmetic, not a real driver.
"""
from pathlib import Path
import ast
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
tree = ast.parse((root/"tools/test_qpr2_gpu_device.py").read_text())
parts = {n.targets[0].id: ast.literal_eval(n.value) for n in tree.body
         if isinstance(n, ast.Assign) and isinstance(n.targets[0], ast.Name)
         and n.targets[0].id in ("api", "fixture")}
api = parts["api"]
needle = "static void ID3D11DeviceContext_UpdateSubresource("
index = api.index(needle)
api = api[:index] + r'''
static ID3D11Buffer *tracked_stats,*stats_staging[64];
static unsigned stats_staging_count,stats_copies,stats_maps,stats_updates,pixel_maps;
static int fail_stats_map,fail_pixel_map;
''' + api[index:]
needle = " (void)c;(void)pitch;(void)dep;assert(!sub && b->desc.Usage==D3D11_USAGE_DEFAULT);"
assert needle in api
api = api.replace(needle, " if(b==tracked_stats)++stats_updates;\n"+needle)
needle = 'static void ID3D11DeviceContext_CopyResource(ID3D11DeviceContext*c,ID3D11Resource*dst,ID3D11Resource*src){'
assert needle in api
api = api.replace(needle, needle + r'''
 if(src==tracked_stats){++stats_copies;unsigned i;for(i=0;i<stats_staging_count;i++)if(stats_staging[i]==dst)break;
  if(i==stats_staging_count){assert(i<64);stats_staging[stats_staging_count++]=dst;}}
''')
needle = 'static HRESULT ID3D11DeviceContext_Map(ID3D11DeviceContext*c,ID3D11Resource*b,unsigned sub,unsigned mode,unsigned flags,D3D11_MAPPED_SUBRESOURCE*out){'
assert needle in api
api = api.replace(needle, needle + r'''
 if(tracked_stats){unsigned i;for(i=0;i<stats_staging_count;i++)if(stats_staging[i]==b)break;
  if(i<stats_staging_count){++stats_maps;if(fail_stats_map){fail_stats_map=0;return -1;}}
  else {++pixel_maps;if(fail_pixel_map){fail_pixel_map=0;return -1;}}}
''')
mock = parts["fixture"].split("int main(int argc,char**argv){")[0]
main = r'''
enum {SIDE=16,STRIDE=20};
static uint32_t pixels[4][SIDE*SIDE],expected[4][SIDE*SIDE];
static float z[4][SIDE*STRIDE],ez[4][SIDE*STRIDE];
static NvGpuCount reference;
static int full;
static void prepare(void){
 memset(&reference,0,sizeof reference);
 for(unsigned k=0;k<4;k++){
  for(unsigned i=0;i<SIDE*SIDE;i++)pixels[k][i]=expected[k][i]=0xFF112233;
  for(unsigned i=0;i<SIDE*STRIDE;i++)z[k][i]=ez[k][i]=100;
 }
}
static void draw(unsigned k,unsigned visibility){
 NvGpuState s={0};NvGpuTextureSource tex[4]={0};NvGpuTri t=triangle(SIDE);
 s.width=s.height=SIDE;s.pitch=SIDE*4;s.bpp=4;s.format=0x128;s.color_mask=0x01010101;
 s.depth_available=s.depth_test=s.depth_mask=1;s.zpass_enable=visibility;s.depth_func=0x203;
 assert(nv_gpu_draw(&s,&t,1,(uint8_t*)pixels[k],z[k],STRIDE,tex)==1);
 for(unsigned y=0;y<SIDE;y++)for(unsigned x=0;x<SIDE;x++){
  NvGpuPixel p=gpu_pixel(s,t,x,y,expected[k][y*SIDE+x],ez[k][y*STRIDE+x]);
  expected[k][y*SIDE+x]=p.color;ez[k][y*STRIDE+x]=p.depth;reference.zpass+=p.zpass;
  if(full){reference.pixels+=p.pixels;reference.depth_fail+=p.depth_fail;
   if((p.pixel_max&0xFFFFFF)>(reference.pixel_max&0xFFFFFF))reference.pixel_max=p.pixel_max;}
 }
}
static void finish(void){
 NvGpuCount got;assert(nv_gpu_sync(&got)==1);
 assert(got.pixels==reference.pixels && got.depth_fail==reference.depth_fail &&
        got.zpass==reference.zpass && got.pixel_max==reference.pixel_max);
 for(unsigned k=0;k<4;k++){
  assert(!memcmp(pixels[k],expected[k],sizeof pixels[k]));
  assert(!memcmp(z[k],ez[k],sizeof z[k]));
 }
 memset(&reference,0,sizeof reference);
}
int main(int argc,char**argv){
 assert(argc==2);int disabled=!strcmp(argv[1],"disabled"),diagnostic=!strcmp(argv[1],"diagnostic");
 int compute=!strcmp(argv[1],"compute"),capture=!strcmp(argv[1],"capture");
 int stats_fail=!strcmp(argv[1],"stats-map-fail"),pixel_fail=!strcmp(argv[1],"pixel-map-fail");
 int pending=!strcmp(argv[1],"pending");
 native_supported=!compute;coverage_tier=0;
 setenv("RECOMP_GPU_RASTER","1",1);setenv("RECOMP_GPU_ASYNC","0",1);
 setenv("RECOMP_GPU_AUTOTUNE","1",1);unsetenv("RECOMP_GPU_AUTOTUNE_EXPERIMENT");
 setenv("RECOMP_GPU_DIAGNOSTIC_COUNTERS",diagnostic?"1":"0",1);
 setenv("RECOMP_GPU_EMPTY_STATS",disabled?"0":"1",1);
 assert(nv_gpu_available() && !g.auto_enabled);
 full=compute || diagnostic;tracked_stats=g.stats.buffer;
 stats_copies=stats_maps=stats_updates=pixel_maps=0;
 if(capture){s_capture=tmpfile();assert(s_capture);}
 prepare();
 if(stats_fail || pixel_fail){
  draw(0,stats_fail);assert(nv_gpu_submit()==1);
  if(stats_fail)fail_stats_map=1;else fail_pixel_map=1;
  NvGpuCount got={0};assert(nv_gpu_sync(&got)<0 && !got.zpass && !g.completed.zpass);
  for(unsigned i=0;i<SIDE*SIDE;i++)assert(pixels[0][i]==0xFF112233);
  puts("PASS: failed required Map cannot publish output or visibility");return 0;
 }
 for(unsigned i=0;i<24;i++){
  /* Stale nonzero device storage may not leak into an unused target. */
  if(!full && !disabled && !capture)memset(g.stats.buffer->data,0xFF,g.stats.bytes);
  draw(0,0);finish();
 }
 int omit=!full && !disabled && !capture;
 assert(stats_copies==(omit?0u:24u) && stats_maps==stats_copies && stats_updates==stats_copies);
 assert(pixel_maps==(compute?48u:24u));
 printf("PASS: 24 completed zero-visibility targets: stats copies/maps/updates=%u/%u/%u; exact pixels/depth/padding\n",stats_copies,stats_maps,stats_updates);
 /* An earlier enabled state remains live after a later disabled state,
  * both within one immutable queue and across actual submissions. */
 prepare();draw(0,1);draw(0,0);unsigned copies=stats_copies;finish();assert(stats_copies==copies+1);
 prepare();draw(0,1);assert(nv_gpu_submit()==1);draw(0,0);copies=stats_copies;finish();assert(stats_copies==copies+1);
 /* Reset discards pre-reset visibility; later writers start at zero. */
 prepare();draw(0,1);assert(nv_gpu_reset_report()==1);reference.zpass=0;
 copies=stats_copies;finish();assert(stats_copies==copies+(omit?0:1));
 prepare();draw(0,1);assert(nv_gpu_reset_report()==1);reference.zpass=0;draw(0,1);finish();
 if(pending){
  prepare();copies=stats_copies;draw(0,0);draw(1,1);draw(2,0);draw(3,0);
  assert(g.pending_count==3 && !g.pending[0].has_stats && g.pending[1].has_stats && !g.pending[2].has_stats);
  finish();assert(stats_copies==copies+1);
  puts("PASS: independent pending targets retain per-snapshot counter liveness");
 }
 if(s_capture){fclose(s_capture);s_capture=NULL;}
 nv_gpu_report();puts("PASS: mixed enable states, previous submissions, reset/discard/new writer, native defaults and exact semantics");
}
'''
with tempfile.TemporaryDirectory(prefix="qpr2-empty-stats-") as folder:
    p = Path(folder)
    for name in ("windows.h","d3d11.h","d3d11_3.h","d3dcompiler.h","dxgi.h"):
        (p/name).write_text("/* API provided by fixture. */\n")
    (p/"test.c").write_text(api+'ptrdiff_t xbox_GetMemoryOffset(void){return 0;}\n#include "'+
        str(root/"third_party/xboxrecomp/src/kernel/nv2a_gpu_raster.c")+'"\n'+mock+main)
    subprocess.run(["gcc","-std=gnu11","-O1","-Werror=implicit-function-declaration",
        "-Werror=incompatible-pointer-types","-fsanitize=undefined,float-cast-overflow",
        "-fno-sanitize-recover=all","-I"+str(p),"-I"+str(root/"tools"),
        "-I"+str(root/"third_party/xboxrecomp/src/kernel"),str(p/"test.c"),
        str(root/"third_party/xboxrecomp/src/kernel/nv2a_vsh_interp.c"),str(root/"third_party/xboxrecomp/src/kernel/nv2a_gpu_tiles.c"),"-lm","-pthread","-o",str(p/"test")],check=True)
    for mode in ("quiet","disabled","diagnostic","compute","capture","stats-map-fail","pixel-map-fail","pending"):
        subprocess.run([str(p/"test"),mode],cwd=p,check=True,timeout=180)
    print("PASS: all eight production counter-liveness modes; no gameplay FPS benchmark")
