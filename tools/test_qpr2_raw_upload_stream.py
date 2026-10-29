"""Separate stage jobs and bounded raw input rings through real production C.

The strict API fixture checks stage bindings/upload order, ring wrap/growth,
live constant/index/vertex snapshots and allocation fallback. Scalar kernels
execute GPU commands here; this is not a Windows driver speed measurement.
"""
from pathlib import Path
import subprocess,tempfile
import test_qpr2_gpu_geometry as geo
root=geo.root;k=geo.k
api=geo.api
needle='static HRESULT ID3D11Device_CreateBuffer('
api=api.replace(needle,'static int input_allocation_fail(const D3D11_BUFFER_DESC*);\n'+needle,1)
api=api.replace(' (void)d;++buffer_creates;', ' if(input_allocation_fail(s))return -1;\n (void)d;++buffer_creates;',1)
needle='static void ID3D11DeviceContext_UpdateSubresource('
api=api.replace(needle,'static void input_update_probe(ID3D11Resource*);\n'+needle,1)
api=api.replace(' (void)c;(void)pitch;(void)dep;assert(!sub', ' input_update_probe(b);\n (void)c;(void)pitch;(void)dep;assert(!sub',1)
fixture=geo.fixture.replace('++geometry_dispatches;assert(bound_uav[0]', 'input_dispatch_probe();++geometry_dispatches;assert(bound_uav[0]',1)
fixture='static void input_dispatch_probe(void);\n'+fixture
main=r'''
static unsigned checking,stage_pending,between_stage_updates,slot_mask,transforms,prepares,fail_kind,job_allocations;
static ID3D11Buffer *transform_draws,*transform_jobs;
static int input_allocation_fail(const D3D11_BUFFER_DESC *d){
 if(!checking || !fail_kind || d->Usage!=D3D11_USAGE_DEFAULT)return 0;
 int fail=(fail_kind==1 || fail_kind==4) && d->StructureByteStride==4;
 if(d->StructureByteStride==16){++job_allocations;fail|=fail_kind==2 || (fail_kind==3 && job_allocations==2);}
 if(fail){if(fail_kind!=4)fail_kind=0;return 1;}return 0;
}
static void input_update_probe(ID3D11Resource *b){
 if(!checking || !stage_pending)return;
 if(b==transform_jobs){++between_stage_updates;assert(!raw.input_stream);}
}
static void input_dispatch_probe(void){
 if(!checking)return;
 if(bound_compute==&geometry_transform){
  assert(!stage_pending);stage_pending=1;++transforms;
  transform_draws=bound_srv[3]->buffer;transform_jobs=bound_srv[4]->buffer;
  if(raw.input_stream){
   unsigned at=(raw.input_cursor-1)%NV_RAW_UPLOAD_SLOTS;slot_mask|=1u<<at;
   assert(transform_draws==raw.input[at].draws.buffer && transform_jobs==raw.input[at].jobs[0].buffer);
   for(unsigned other=0;other<NV_RAW_UPLOAD_SLOTS;other++)if(other!=at){
    assert(!raw.input[other].draws.buffer || raw.input[other].draws.buffer!=transform_draws);
    assert(!raw.input[other].jobs[0].buffer || raw.input[other].jobs[0].buffer!=transform_jobs);
   }
  }else assert(transform_draws==raw.draw_buffer.buffer && transform_jobs==raw.jobs.buffer);
 }else{
  assert(stage_pending && bound_compute==&geometry_prepare);stage_pending=0;++prepares;
  assert(bound_srv[3]->buffer==transform_draws);
  if(raw.input_stream){
   unsigned at=(raw.input_cursor-1)%NV_RAW_UPLOAD_SLOTS;
   assert(bound_srv[4]->buffer==raw.input[at].jobs[1].buffer && bound_srv[4]->buffer!=transform_jobs);
  }else assert(bound_srv[4]->buffer==transform_jobs);
 }
}
static void reference_draw(NvGpuState state,NvGpuTri tri,uint32_t *color,float *z,NvGpuCount *count){
 for(unsigned y=0;y<32;y++)for(unsigned x=0;x<32;x++){
  unsigned at=y*32+x;NvGpuPixel p=gpu_pixel(state,tri,x,y,color[at],z[at]);color[at]=p.color;z[at]=p.depth;
  count->pixels+=p.pixels;count->depth_fail+=p.depth_fail;count->zpass+=p.zpass;
  if((p.pixel_max&0xFFFFFFu)>(count->pixel_max&0xFFFFFFu))count->pixel_max=p.pixel_max;
 }
}
int main(int argc,char **argv){
 const char *mode=argc>1?argv[1]:"stream";
 setenv("RECOMP_GPU_RASTER","1",1);setenv("RECOMP_GPU_ASYNC","0",1);setenv("RECOMP_GPU_GEOMETRY","1",1);
 setenv("RECOMP_GPU_DIAGNOSTIC_COUNTERS","1",1);setenv("RECOMP_GPU_RAW_UPLOAD_STREAM",!strcmp(mode,"legacy")?"0":"1",1);
 native_supported=1;coverage_tier=3;assert(nv_gpu_available() && raw.ready);
 if(!strcmp(mode,"no-cache"))g.upload_cache=0;
 /* Startup work is fully completed. Fresh private inputs make failures
  * deterministic and exercise growth on all ring slots after initialization. */
 for(unsigned i=0;i<NV_RAW_UPLOAD_SLOTS;i++){
  gpu_buffer_release(&raw.input[i].draws);for(unsigned j=0;j<2;j++)gpu_buffer_release(&raw.input[i].jobs[j]);
 }
 gpu_buffer_release(&raw.draw_buffer);gpu_buffer_release(&raw.jobs);raw.input_cursor=0;
 raw.input_batches=raw.input_rotations=raw.input_rewrites=raw.input_failures=0;raw.input_ms=0;
 checking=1;fail_kind=!strcmp(mode,"draw-fail")?1:!strcmp(mode,"vertex-jobs-fail")?2:!strcmp(mode,"prepare-jobs-fail")?3:!strcmp(mode,"fallback-fail")?4:0;
 unsigned injected=fail_kind!=0;
 float vertices[96][4],constants[192][4]={{0}};uint32_t indices[96];
 for(unsigned row=0;row<4;row++){constants[96+row][row]=1;constants[58][row]=1;}
 constants[186][1]=constants[186][3]=1;
 NvRawInput in={0};in.indices=indices;in.primitive=5;in.inputs=0x3d;in.position_kind=1;in.constants=constants;
 in.params.right=in.params.bottom=32;in.params.su=in.params.sv=1;
 NvGpuState s={0};s.width=s.height=32;s.pitch=128;s.bpp=4;s.format=0x128;s.color_mask=0x01010101;
 s.depth_available=s.depth_test=s.depth_mask=s.zpass_enable=1;s.depth_func=0x203;s.blend_enable=1;s.blend_sfactor=0x302;s.blend_dfactor=0x303;s.blend_equation=0x8006;
 uint32_t color[1024]={0},expected[1024]={0};float z[1024],expected_z[1024];
 for(unsigned i=0;i<1024;i++)z[i]=expected_z[i]=100;
 NvGpuTextureSource sources[4]={0};NvGpuCount ref={0},got;
 uint64_t reads=g.readbacks;
 for(unsigned batch=0;batch<32;batch++){
  unsigned n=batch<16?3:96;in.count=in.vertices=n;
  for(unsigned v=0;v<n;v++){
   const float p[3][2]={{2,2},{27,3},{4,27}};vertices[v][0]=p[v%3][0]+(batch%4)*.125f;vertices[v][1]=p[v%3][1];vertices[v][2]=20;vertices[v][3]=1;indices[v]=v;
  }
  if(batch&1){uint32_t q=indices[1];indices[1]=indices[2];indices[2]=q;}
  in.stream[0]=(NvRawStream){(const uint8_t*)vertices,n*16,16,2,4};
  constants[101][0]=(batch%8)*.125f;constants[101][1]=.5f;constants[101][2]=.25f;constants[101][3]=.5f;
  if(batch%3==0){NvGpuTri cpu=triangle(32);for(unsigned v=0;v<3;v++)cpu.v[v].pos[2]=30;assert(nv_gpu_draw(&s,&cpu,1,(uint8_t*)color,z,32,sources)==1);reference_draw(s,cpu,expected,expected_z,&ref);}
  assert(nv_gpu_draw_raw(&s,&in,(uint8_t*)color,z,32,sources)==1 && raw.count==1);
  NvRawMesh *m=&raw.mesh[raw.meshes[0]];raw.ref_bytes=m->bytes;raw.ref_attrs=m->attrs;raw.ref_draws=raw.draws;raw.ref_words=raw.words;
  for(unsigned t=0;t<m->triangles;t++){
   unsigned at=t*3;NvGpuTri tri=nv_raw_prepare(raw.draws[0].p,nv_raw_vertex(0,m->indices[at]),nv_raw_vertex(0,m->indices[at+1]),nv_raw_vertex(0,m->indices[at+2]));reference_draw(s,tri,expected,expected_z,&ref);
  }
  int submitted=nv_gpu_submit();
  if(!strcmp(mode,"fallback-fail")){
   assert(batch==0 && submitted==-1 && !stage_pending && !transforms && !prepares && g.readbacks==reads && g.queued_count && raw.count);
   for(unsigned i=0;i<1024;i++)assert(!color[i] && z[i]==100);
   puts("PASS: failed stream AND legacy input allocation propagates accepted-work failure before either dispatch or CPU publication");return 0;
  }
  assert(submitted==1 && !stage_pending && g.readbacks==reads);
  /* Accepted batches stay private until the existing completion operation. */
  for(unsigned i=0;i<1024;i++)assert(!color[i] && z[i]==100);
 }
 assert(transforms==32 && prepares==32 && raw.input_batches==32);
 if(!strcmp(mode,"legacy") || injected){assert(!raw.input_stream && !raw.input_rotations && raw.input_rewrites==32 && raw.input_failures==injected);assert(between_stage_updates>0);}
 else assert(raw.input_stream && raw.input_rotations==32 && !raw.input_rewrites && !raw.input_failures && !between_stage_updates && slot_mask==15);
 assert(nv_gpu_sync(&got)==1 && !memcmp(&ref,&got,sizeof ref) && !memcmp(color,expected,sizeof color) && !memcmp(z,expected_z,sizeof z));
 nv_gpu_report();assert(!raw.input_batches && !raw.input_rotations && !raw.input_rewrites && !raw.input_failures && raw.input_ms==0);
 printf("PASS: %s; 32 mixed raw/CPU batches, exact pixels/depth/visibility, four-slot wrap/growth or legacy fallback, no premature CPU publication\n",mode);
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-raw-upload-stream-') as folder:
    p=Path(folder);(p/'logs').mkdir()
    for name in ['windows.h','d3d11.h','d3d11_3.h','d3dcompiler.h','dxgi.h']:(p/name).write_text('/* strict API above */\n')
    (p/'fixture.c').write_text(api+'ptrdiff_t xbox_GetMemoryOffset(void){return 0;}\n#include "'+str(k/'nv2a_gpu_raster.c')+'"\n'+fixture+main)
    subprocess.run(['gcc','-std=gnu11','-O1','-ffp-contract=off','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all',
        '-Werror=implicit-function-declaration','-Werror=incompatible-pointer-types','-I'+str(p),'-I'+str(root/'tools'),'-I'+str(k),
        str(p/'fixture.c'),str(k/'nv2a_vsh_interp.c'),str(k/'nv2a_gpu_tiles.c'),'-pthread','-lm','-o',str(p/'fixture')],check=True)
    for mode in ['stream','no-cache','legacy','draw-fail','vertex-jobs-fail','prepare-jobs-fail','fallback-fail']:
        subprocess.run([str(p/'fixture'),mode],check=True,cwd=p,timeout=120)
    print('PASS: all seven raw-input modes; API ordering checks do not establish gameplay FPS')
