"""Exercise production live-batch selection with deterministic API costs.

Costs below are injected test clocks, never hardware performance claims.
Both backends execute the shared scalar kernel through the strict D3D shim.
"""
import ast
from pathlib import Path
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
tree = ast.parse((root / "tools/test_qpr2_gpu_device.py").read_text())
values = {n.targets[0].id: ast.literal_eval(n.value) for n in tree.body
          if isinstance(n, ast.Assign) and isinstance(n.targets[0], ast.Name)
          and n.targets[0].id in ("api", "fixture")}
api = values["api"]
old = 'static void QueryPerformanceCounter(LARGE_INTEGER*p){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);p->QuadPart=(int64_t)t.tv_sec*1000000000+t.tv_nsec;}'
assert old in api
api = api.replace(old, 'static int64_t fixture_ns=1000000000;static int fixture_mode;\nstatic void QueryPerformanceCounter(LARGE_INTEGER*p){p->QuadPart=fixture_ns;}')
api += r'''
static int fixture_allocation_failure;
static void *fixture_malloc(size_t bytes){if(fixture_allocation_failure){fixture_allocation_failure=0;return NULL;}return malloc(bytes);}
#define malloc fixture_malloc
'''
mock = values["fixture"].split('int main(int argc,char**argv){')[0]
needle = 'static void ID3D11DeviceContext_Dispatch(ID3D11DeviceContext*ctx,unsigned n,unsigned y,unsigned z){'
mock = mock.replace(needle, needle + '\n if(g.auto_busy)fixture_ns+=fixture_mode==1?20000000:fixture_mode==2?9000000:1000000;')
needle = 'static void mock_native_draw(ID3D11DeviceContext*ctx,unsigned vertices,unsigned first_vertex){'
mock = mock.replace(needle, needle + '\n if(g.auto_busy){fixture_ns+=10000000;if(fixture_mode==5)fail_next_map=1;}')
needle = 'if(corrupt_shader || (fixed_corrupt'
assert needle in mock
mock = mock.replace(needle, 'if(g.auto_busy && fixture_mode==11)stats[2]++;\n if(g.auto_busy && fixture_mode==3)color[17]^=0xFF00;\n if(g.auto_busy && fixture_mode==4)depth[17]+=1;\n '+needle)
main = r'''
int main(int argc,char**argv){
 assert(argc==2);fixture_mode=atoi(argv[1]);native_supported=1;coverage_tier=3;
 setenv("RECOMP_GPU_RASTER","1",1);setenv("RECOMP_GPU_ASYNC","0",1);setenv("RECOMP_GPU_NATIVE","1",1);
 setenv("RECOMP_GPU_AUTOTUNE_EXPERIMENT",fixture_mode>=13?"0":"1",1);
 setenv("RECOMP_GPU_AUTOTUNE",fixture_mode==6?"0":"1",1);
 setenv("RECOMP_GPU_DIAGNOSTIC_COUNTERS",fixture_mode==12?"1":"0",1);
 assert(nv_gpu_available() && g.native && g.simple_shader);
 enum {W=128,H=128,STRIDE=132,N=512};
 static uint32_t color[W*H],expected[W*H];static float depth[STRIDE*H],zexpected[STRIDE*H];
 NvGpuTri tri[N];NvGpuState s={0};NvGpuTextureSource tex[4]={0};NvGpuCount count={0};
 s.width=W;s.height=H;s.pitch=W*4;s.bpp=4;s.format=0x128;s.color_mask=0x01010101;
 s.depth_available=s.depth_test=s.depth_mask=s.zpass_enable=1;s.depth_func=0x203;
 for(unsigned i=0;i<W*H;i++)color[i]=expected[i]=0xFF112233;
 for(unsigned i=0;i<STRIDE*H;i++)depth[i]=zexpected[i]=100;
 unsigned expected_zpass=0;
 for(unsigned i=0;i<N;i++){
  tri[i]=triangle(2);int left=(i%32)*4,top=(i/32)*4;
  tri[i].minx=left;tri[i].maxx=left+2;tri[i].miny=top;tri[i].maxy=top+2;
  for(unsigned k=0;k<3;k++){tri[i].v[k].pos[0]+=left;tri[i].v[k].pos[1]+=top;}
  for(int y=top;y<top+2;y++)for(int x=left;x<left+2;x++){
   NvGpuPixel p=gpu_pixel(s,tri[i],x,y,expected[y*W+x],zexpected[y*STRIDE+x]);
   expected[y*W+x]=p.color;zexpected[y*STRIDE+x]=p.depth;expected_zpass+=p.zpass;
  }
 }
 if(fixture_mode==8){
  assert(gpu_target(&s,(uint8_t*)color,depth,STRIDE)==1);
  assert(nv_gpu_clear_color(&s,(uint8_t*)color,0,100,8,108,0xFF445566)==1);
  for(unsigned y=100;y<108;y++)for(unsigned x=0;x<8;x++)expected[y*W+x]=0xFF445566;
 }
 if(fixture_mode==9){
  static uint32_t pixels[256];for(unsigned i=0;i<256;i++)pixels[i]=0xFF335577+i;
  assert(nv_gpu_draw(&s,tri,N/2,(uint8_t*)color,depth,STRIDE,tex)==1);
  s.tex[0].valid=1;s.tex[0].width=s.tex[0].height=16;s.tex[0].format=0x12;s.tex[0].addr_u=s.tex[0].addr_v=3;
  tex[0].raw=(const uint8_t*)pixels;tex[0].bytes=sizeof pixels;tex[0].opaque=pixels;tex[0].decode=decode;
  for(unsigned i=N/2;i<N;i++)tri[i].textured=tri[i].use_rc=1;
  assert(nv_gpu_draw(&s,tri+N/2,N/2,(uint8_t*)color,depth,STRIDE,tex)==1 && g.state_count==2);
  assert(g.queued_kind[0]==1 && g.queued_kind[N/2]==0);
  NvGpuState captured=g.queued_state[1];
  for(unsigned i=N/2;i<N;i++)for(int y=tri[i].miny;y<tri[i].maxy;y++)for(int x=tri[i].minx;x<tri[i].maxx;x++)
   expected[y*W+x]=gpu_pixel(captured,tri[i],x,y,0xFF112233,100).color;
 }else assert(nv_gpu_draw(&s,tri,N,(uint8_t*)color,depth,STRIDE,tex)==1 && g.queued_count==N);
 assert(!g.auto_done);
 if(fixture_mode==7){s_capture=tmpfile();assert(s_capture);}
 if(fixture_mode==5){
  assert(nv_gpu_submit()<0 && !g.auto_choose_compute && !g.auto_busy && g.native);
  assert(!g.completed.zpass);
  for(unsigned i=0;i<W*H;i++)assert(color[i]==0xFF112233);
  for(unsigned i=0;i<STRIDE*H;i++)assert(depth[i]==100);
  puts("PASS: private Map failure stops accepted submission without guest output or false visibility");return 0;
 }
 if(fixture_mode==10)fixture_allocation_failure=1;
 assert(nv_gpu_submit()==1 && g.native); /* Selection cannot change an active target. */
 if((fixture_mode>=6 && fixture_mode<=8) || fixture_mode>=13)assert(!g.auto_done && !g.auto_choose_compute);
 else assert(g.auto_done && g.auto_choose_compute==(fixture_mode==0 || fixture_mode==9 || fixture_mode==12));
 assert(!g.completed.zpass);
 for(unsigned i=0;i<W*H;i++)assert(color[i]==0xFF112233);
 assert(nv_gpu_sync(&count)==1 && count.zpass==expected_zpass);
 assert(!memcmp(color,expected,sizeof color) && !memcmp(depth,zexpected,sizeof depth));
 assert(g.native==(fixture_mode!=0 && fixture_mode!=9 && fixture_mode!=12) && !g.auto_choose_compute);
 /* Another accepted draw proves correct target/resource allocation after switching. */
 assert(nv_gpu_draw(&s,tri,N,(uint8_t*)color,depth,STRIDE,tex)==1);
 assert(nv_gpu_sync(&count)==1 && count.zpass==expected_zpass);
 assert(!memcmp(color,expected,sizeof color) && !memcmp(depth,zexpected,sizeof depth));
 if(s_capture){fclose(s_capture);s_capture=NULL;}
 printf("PASS: mode %d: exact guest output/padding, private trials excluded, deferred selection and next draw\n",fixture_mode);
}
'''
with tempfile.TemporaryDirectory(prefix="qpr2-autotune-") as folder:
    p = Path(folder)
    for name in ("windows.h", "d3d11.h", "d3d11_3.h", "d3dcompiler.h", "dxgi.h"):
        (p / name).write_text("/* Declared by fixture. */\n")
    (p / "test.c").write_text(api + 'ptrdiff_t xbox_GetMemoryOffset(void){return 0;}\n#include "' +
        str(root / "third_party/xboxrecomp/src/kernel/nv2a_gpu_raster.c") + '"\n' + mock + main)
    subprocess.run(["gcc", "-std=gnu11", "-O1", "-Werror=implicit-function-declaration",
        "-Werror=incompatible-pointer-types", "-fsanitize=undefined,float-cast-overflow",
        "-fno-sanitize-recover=all", "-I"+str(p), "-I"+str(root/"tools"),
        "-I"+str(root/"third_party/xboxrecomp/src/kernel"), str(p/"test.c"),
        str(root/"third_party/xboxrecomp/src/kernel/nv2a_vsh_interp.c"),str(root/"third_party/xboxrecomp/src/kernel/nv2a_gpu_tiles.c"), "-pthread", "-lm", "-o", str(p/"test")], check=True)
    for mode in range(14):
        subprocess.run([str(p/"test"),str(mode)],cwd=p,check=True,timeout=180)
    print("PASS: fourteen production backend-selection modes; injected costs are not GPU measurements")
