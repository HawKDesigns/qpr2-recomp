"""Exact GPU input reuse and bounded streaming through production C/API calls."""
import ast
from pathlib import Path
import subprocess
import tempfile

root=Path(__file__).resolve().parents[1]
tree=ast.parse((root/'tools/test_qpr2_gpu_device.py').read_text())
parts={n.targets[0].id:ast.literal_eval(n.value) for n in tree.body if isinstance(n,ast.Assign)
       and isinstance(n.targets[0],ast.Name) and n.targets[0].id in ('api','fixture')}
api=parts['api']
needle='static void ID3D11DeviceContext_UpdateSubresource('
index=api.index(needle)
api=api[:index]+'static unsigned fixture_input_writes;\n'+api[index:]
needle=' (void)c;(void)pitch;(void)dep;assert(!sub && b->desc.Usage==D3D11_USAGE_DEFAULT);'
assert needle in api
api=api.replace(needle,' if(b->desc.BindFlags==D3D11_BIND_SHADER_RESOURCE)++fixture_input_writes;\n'+needle)
api+=r'''
static int fixture_shadow_fail;
static void *shadow_realloc(void *p,size_t n){if(fixture_shadow_fail){fixture_shadow_fail=0;return NULL;}return realloc(p,n);}
#define realloc shadow_realloc
'''
mock=parts['fixture'].split('int main(int argc,char**argv){')[0]
main=r'''
int main(int argc,char**argv){
 assert(argc==2);int compute=!strcmp(argv[1],"compute"),no_stream=!strcmp(argv[1],"no-stream");
 int no_cache=!strcmp(argv[1],"no-cache"),alloc_fail=!strcmp(argv[1],"shadow-fail");
 native_supported=!compute;
 setenv("RECOMP_GPU_RASTER","1",1);setenv("RECOMP_GPU_ASYNC","0",1);
 setenv("RECOMP_GPU_DIAGNOSTIC_COUNTERS","0",1);unsetenv("RECOMP_GPU_AUTOTUNE_EXPERIMENT");
 setenv("RECOMP_GPU_UPLOAD_STREAM",no_stream?"0":"1",1);setenv("RECOMP_GPU_UPLOAD_CACHE",no_cache?"0":"1",1);
 assert(nv_gpu_available());fixture_input_writes=0;
 uint32_t color[256]={0},expected[256]={0};float depth[256],ez[256];
 for(unsigned i=0;i<256;i++)depth[i]=ez[i]=100;
 NvGpuState s={0};NvGpuTextureSource textures[4]={0};NvGpuTri tri[2]={triangle(16),triangle(16)};NvGpuCount ref={0},got;
 s.width=s.height=16;s.pitch=64;s.bpp=4;s.format=0x128;s.color_mask=0x01010101;
 s.depth_available=s.depth_test=s.depth_mask=s.zpass_enable=1;s.depth_func=0x203;
 ID3D11Buffer *seen[4]={0};unsigned seen_count=0;
 for(unsigned draw=0;draw<20;draw++){
  assert(nv_gpu_draw(&s,tri,1,(uint8_t*)color,depth,16,textures)==1);
  if(alloc_fail && draw==0)fixture_shadow_fail=1;
  assert(nv_gpu_submit()==1 && g.active);
  unsigned slot=no_stream?0:(g.upload_cursor-1)%NV_GPU_UPLOAD_SLOTS;
  ID3D11Buffer *buffer=slot?g.upload_spare[slot-1].triangles.buffer:g.triangles.buffer;
  unsigned at;for(at=0;at<seen_count;at++)if(seen[at]==buffer)break;
  if(at==seen_count){assert(seen_count<4);seen[seen_count++]=buffer;}
  assert(!color[0]); /* No CPU publication before explicit completion. */
  for(unsigned y=0;y<16;y++)for(unsigned x=0;x<16;x++){
   NvGpuPixel p=gpu_pixel(s,tri[0],x,y,expected[y*16+x],ez[y*16+x]);
   expected[y*16+x]=p.color;ez[y*16+x]=p.depth;ref.zpass+=p.zpass;
  }
 }
 unsigned inputs=compute?4:2,slots=no_stream?1:4;
 assert(seen_count==slots);
 if(!alloc_fail)assert(fixture_input_writes==inputs*(no_cache?20:slots));
 assert(nv_gpu_sync(&got)==1 && got.zpass==ref.zpass && !memcmp(color,expected,sizeof color) && !memcmp(depth,ez,sizeof depth));
 printf("PASS: 20 accepted batches, slots=%u, input updates=%u, exact pixels/depth/visibility; no CPU publication before completion\n",seen_count,fixture_input_writes);
 /* Same addresses, changed payload/state, then a capacity-growing upload. */
 for(unsigned draw=0;draw<12;draw++){
  tri[0].v[0].d0[0]=tri[0].v[1].d0[0]=tri[0].v[2].d0[0]=(draw+1)/16.0f;
  tri[1]=tri[0];s.color_mask=draw%2?0x01010101:0x01010001;
  unsigned n=draw<6?1:2;ref.zpass=0;
  assert(nv_gpu_draw(&s,tri,n,(uint8_t*)color,depth,16,textures)==1);
  for(unsigned k=0;k<n;k++)for(unsigned y=0;y<16;y++)for(unsigned x=0;x<16;x++){
   NvGpuPixel p=gpu_pixel(s,tri[k],x,y,expected[y*16+x],ez[y*16+x]);
   expected[y*16+x]=p.color;ez[y*16+x]=p.depth;ref.zpass+=p.zpass;
  }
  assert(nv_gpu_sync(&got)==1 && got.zpass==ref.zpass && !memcmp(color,expected,sizeof color) && !memcmp(depth,ez,sizeof depth));
 }
 nv_gpu_report();puts("PASS: byte changes and material changes invalidate reuse; buffer growth, ring wrap and shadow-allocation failure retain exact accepted work");
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-upload-stream-') as folder:
    p=Path(folder)
    for name in ['windows.h','d3d11.h','d3d11_3.h','d3dcompiler.h','dxgi.h']:(p/name).write_text('/* Fixture declarations above. */\n')
    (p/'test.c').write_text(api+'ptrdiff_t xbox_GetMemoryOffset(void){return 0;}\n#include "'+str(root/'third_party/xboxrecomp/src/kernel/nv2a_gpu_raster.c')+'"\n'+mock+main)
    subprocess.run(['gcc','-std=gnu11','-O1','-Werror=implicit-function-declaration','-Werror=incompatible-pointer-types',
        '-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-I'+str(p),'-I'+str(root/'tools'),
        '-I'+str(root/'third_party/xboxrecomp/src/kernel'),str(p/'test.c'),str(root/'third_party/xboxrecomp/src/kernel/nv2a_vsh_interp.c'),str(root/'third_party/xboxrecomp/src/kernel/nv2a_gpu_tiles.c'),'-pthread','-lm','-o',str(p/'test')],check=True)
    for mode in ['native','compute','no-stream','no-cache','shadow-fail']:
        subprocess.run([str(p/'test'),mode],check=True,cwd=p,timeout=180)
    print('PASS: all five production upload-stream modes; API counts are not gameplay FPS')
