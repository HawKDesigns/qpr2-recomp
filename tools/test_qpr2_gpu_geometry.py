"""Strict D3D11 resource/queue integration of raw geometry, with shader failures.
The API shim executes scalar kernels; actual HLSL compilation and hardware
comparison are mandatory on Windows startup before this feature is enabled.
"""
from pathlib import Path
import subprocess,tempfile,sys
root=Path(__file__).resolve().parents[1];k=root/'third_party/xboxrecomp/src/kernel'
base=root/'tools/test_qpr2_gpu_device.py';ns={'__file__':str(base)}
s=base.read_text();exec(compile(s[:s.index('with tempfile.TemporaryDirectory(')],str(base),'exec'),ns)
api=ns['api'];fixture=ns['fixture'][:ns['fixture'].index('int main(int argc,char**argv)')]
api=api.replace('static ID3D11ComputeShader shader;', 'static ID3D11ComputeShader shader,geometry_transform,geometry_prepare;\nstatic int geometry_fail_compile,geometry_fail_create,geometry_bad_vertex,geometry_bad_prepare,geometry_fail_map;\nstatic unsigned geometry_dispatches;')
api=api.replace('assert(n>20000 ||', 'assert(!strcmp(a,"qpr2_gpu_geometry") || n>20000 ||')
api=api.replace('if(failed_compile ||', 'if((geometry_fail_compile && !strcmp(a,"qpr2_gpu_geometry")) || failed_compile ||')
api=api.replace('if(simple_macro && !strcmp(e,"ps_5_0")){',r'''if(!strcmp(a,"qpr2_gpu_geometry")){
  int transform=0;for(D3D_SHADER_MACRO*m=b;m && m->Name;m++)if(!strcmp(m->Name,"NV_RAW_TRANSFORM"))transform=atoi(m->Definition);
  char*data=malloc(n+16);assert(data);memset(data,' ',16);memcpy(data,transform?"//raw-transform":"//raw-prepare",transform?15:13);memcpy(data+16,s,n);blob->data=data;blob->bytes=n+16;blob->owned=1;
 }else if(simple_macro && !strcmp(e,"ps_5_0")){''')
api=api.replace('assert(n>0);int clear=', 'assert(n>0);if(n>16 && !memcmp(p,"//raw-",6)){if(geometry_fail_create)return -1;*out=!memcmp(p,"//raw-transform",15)?&geometry_transform:&geometry_prepare;return 0;}int clear=')
api=api.replace('s==&shader || s==&report_shader || s==&clear_shader','s==&shader || s==&report_shader || s==&clear_shader || s==&geometry_transform || s==&geometry_prepare')
api=api.replace('s->Usage==D3D11_USAGE_DEFAULT && !s->CPUAccessFlags', '(s->Usage==D3D11_USAGE_DEFAULT || (s->Usage==D3D11_USAGE_IMMUTABLE && i && i->pSysMem && s->BindFlags==D3D11_BIND_SHADER_RESOURCE)) && !s->CPUAccessFlags')
api=api.replace('s->StructureByteStride && s->ByteWidth%s->StructureByteStride==0','s->StructureByteStride && s->StructureByteStride<=2048 && s->ByteWidth%s->StructureByteStride==0')
api=api.replace('b->desc.BindFlags==D3D11_BIND_SHADER_RESOURCE &&', '(b->desc.BindFlags&D3D11_BIND_SHADER_RESOURCE) &&').replace('b->desc.BindFlags==D3D11_BIND_UNORDERED_ACCESS &&','(b->desc.BindFlags&D3D11_BIND_UNORDERED_ACCESS) &&')
fixture=fixture.replace('if(bound_compute==&clear_shader){',r'''
 if(bound_compute==&geometry_transform || bound_compute==&geometry_prepare){
  ++geometry_dispatches;assert(bound_uav[0] && !bound_uav[1] && !bound_uav[2]);for(unsigned i=0;i<5;i++)assert(bound_srv[i]);
  raw.ref_draws=(const NvRawDraw*)bound_srv[3]->buffer->data;
  const NvRawJob*jobs=(const NvRawJob*)bound_srv[4]->buffer->data;
  assert(n*64*sizeof(*jobs)<=bound_srv[4]->buffer->desc.ByteWidth);
  if(bound_compute==&geometry_transform){
   raw.ref_atlas=1;raw.ref_bytes=bound_srv[0]->buffer->data;raw.ref_attrs=(const NvRawAttr*)bound_srv[1]->buffer->data;raw.ref_words=(const uint32_t*)bound_srv[2]->buffer->data;
   Nv2aVshOutput*dst=(Nv2aVshOutput*)bound_uav[0]->buffer->data;
   for(unsigned i=0;i<n*64;i++)if(jobs[i].draw!=UINT32_MAX){assert((jobs[i].output+1)*sizeof(*dst)<=bound_uav[0]->buffer->desc.ByteWidth);dst[jobs[i].output]=nv_raw_vertex(jobs[i].draw,jobs[i].element);if(geometry_bad_vertex)dst[jobs[i].output].pos[0]+=10;}
  }else{
   const Nv2aVshOutput*src=(const Nv2aVshOutput*)bound_srv[0]->buffer->data;const uint32_t*indices=(const uint32_t*)bound_srv[2]->buffer->data;NvGpuTri*dst=(NvGpuTri*)bound_uav[0]->buffer->data;
   for(unsigned i=0;i<n*64;i++)if(jobs[i].draw!=UINT32_MAX){NvRawParams p=nv_raw_params(jobs[i].draw);const NvRawAttr*meta=(const NvRawAttr*)bound_srv[1]->buffer->data;unsigned at=meta[p.reserved+16].offset+jobs[i].element*3;dst[jobs[i].output]=nv_raw_prepare(p,src[p.vertex_base+indices[at]],src[p.vertex_base+indices[at+1]],src[p.vertex_base+indices[at+2]]);if(geometry_bad_prepare)dst[jobs[i].output].inclusive[0]^=1;}
   if(geometry_fail_map)fail_next_map=1;
  }
  raw.ref_atlas=0;return;
 }
 if(bound_compute==&clear_shader){''')
main=r'''
int main(int argc,char**argv){
 const char*mode=argc>1?argv[1]:"native";
 geometry_fail_compile=!strcmp(mode,"compile");geometry_fail_create=!strcmp(mode,"create");geometry_bad_vertex=!strcmp(mode,"vertex");geometry_bad_prepare=!strcmp(mode,"prepare");geometry_fail_map=!strcmp(mode,"map");
 setenv("RECOMP_GPU_RASTER","1",1);setenv("RECOMP_GPU_ASYNC","0",1);setenv("RECOMP_GPU_GEOMETRY",!strcmp(mode,"disabled")?"0":"1",1);
 setenv("RECOMP_GPU_DIAGNOSTIC_COUNTERS","1",1);target_create_fail_bytes=!strcmp(mode,"atlas")?NV_RAW_ATLAS_LIMIT:0;native_supported=strcmp(mode,"compute")!=0;coverage_tier=strcmp(mode,"coverage")?3:0;
 if(geometry_fail_map){assert(!nv_gpu_available());assert(s_startup==3 && !raw.ready);puts("PASS: failed geometry startup Map disables hardware before guest work");return 0;}
 assert(nv_gpu_available());
 int enabled=!geometry_fail_compile && !geometry_fail_create && !geometry_bad_vertex && !geometry_bad_prepare && native_supported && coverage_tier && strcmp(mode,"disabled") && strcmp(mode,"atlas");
 assert(nv_gpu_geometry_available()==enabled);
 if(!enabled){assert(!raw.ready);puts("PASS: unsupported/disabled geometry or compile/create/comparison failure retains validated CPU geometry + pixel backend");return 0;}
 float vertices[4][4]={{2,2,20,1},{27,3,20,1},{4,27,20,1},{25,25,20,1}},constants[192][4]={{0}};
 for(unsigned j=0;j<4;j++)constants[96+j][j]=1;
 for(unsigned j=0;j<4;j++)constants[58][j]=constants[101][j]=1;
 constants[186][1]=1;constants[186][3]=1;
 uint32_t indices[12]={0,1,2,0,2,3,0,1,2,3,0,1};
 NvRawInput input={0};input.indices=indices;input.count=6;input.vertices=4;input.primitive=5;input.inputs=0x3d;input.position_kind=1;input.constants=constants;input.bounds_reject_enabled=1;
 input.stream[0]=(NvRawStream){(const uint8_t*)vertices,sizeof vertices,16,2,4};input.params.right=input.params.bottom=32;input.params.su=input.params.sv=1;
 uint32_t color[1024],expected[1024];float depth[1024],expected_depth[1024];
 NvGpuState state={0};state.width=state.height=32;state.pitch=128;state.bpp=4;state.format=0x128;state.color_mask=0x01010101;
 state.depth_available=state.depth_test=state.depth_mask=1;state.depth_func=0x203;state.blend_enable=1;state.blend_sfactor=0x302;state.blend_dfactor=0x303;state.blend_equation=0x8006;
 NvGpuTextureSource textures[4]={0};NvGpuCount count;
 for(unsigned i=0;i<1024;i++){color[i]=expected[i]=0xFF123456;depth[i]=expected_depth[i]=100;}
 uint64_t reads=g.readbacks;unsigned dispatches=geometry_dispatches;
 const unsigned prims[]={5,6,7,8,9,10};
 for(unsigned scene=0;scene<80;scene++){
  input.primitive=prims[(scene/2)%6];input.count=(input.primitive==8 || input.primitive==9)?4:6;
  /* Same address mutation must retain all previously queued geometry. */
  vertices[0][0]=2+((scene/2)%7)*0.25f;constants[101][0]=(scene%8)*.1f;constants[101][3]=.5f;
  if(scene==40){NvGpuTri cpu=triangle(32);for(unsigned y=0;y<32;y++)for(unsigned x=0;x<32;x++){unsigned at=y*32+x;NvGpuPixel p=gpu_pixel(state,cpu,x,y,expected[at],expected_depth[at]);expected[at]=p.color;expected_depth[at]=p.depth;}
   assert(nv_gpu_draw(&state,&cpu,1,(uint8_t*)color,depth,32,textures)==1);}
  assert(nv_gpu_draw_raw(&state,&input,(uint8_t*)color,depth,32,textures)==1);
  NvRawMesh*m=&raw.mesh[raw.meshes[raw.count-1]];NvRawDraw*d=&raw.draws[raw.count-1];
  raw.ref_bytes=m->bytes;raw.ref_attrs=m->attrs;raw.ref_draws=raw.draws;raw.ref_words=raw.words;
  for(unsigned t=0;t<m->triangles;t++){
   unsigned j=t*3;NvGpuTri tri=nv_raw_prepare(d->p,nv_raw_vertex(raw.count-1,m->indices[j]),nv_raw_vertex(raw.count-1,m->indices[j+1]),nv_raw_vertex(raw.count-1,m->indices[j+2]));
   for(unsigned y=0;y<32;y++)for(unsigned x=0;x<32;x++){unsigned at=y*32+x;NvGpuPixel p=gpu_pixel(state,tri,x,y,expected[at],expected_depth[at]);expected[at]=p.color;expected_depth[at]=p.depth;}
  }
 }
 assert(g.readbacks==reads);assert(nv_gpu_sync(&count)==1 && geometry_dispatches==dispatches+2);
 for(unsigned i=0;i<1024;i++){assert(color[i]==expected[i]);assert(depth[i]==expected_depth[i]);}
 assert(raw.hits && raw.misses && raw.uploads);assert(raw.host_bytes<=NV_RAW_HOST_LIMIT);
 puts("PASS: raw mesh reuse; same-address mutation and eviction while pinned; per-draw constant snapshots; six primitive modes; mixed CPU/raw draw order; exact pixels/depth; no runtime geometry Map");

 /* A warm set substantially larger than the former 32 entries. Every tiny
  * mesh stays resident; all 320 draws share precisely two geometry dispatches. */
 input.primitive=5;input.count=6;
 for(unsigned pass=0;pass<2;pass++){
  uint64_t uploads=raw.uploads,hits=raw.hits;unsigned before=geometry_dispatches;
  for(unsigned mesh=0;mesh<320;mesh++){
   vertices[0][0]=3+(float)mesh/1024;constants[101][0]=(float)mesh/512;
   assert(nv_gpu_draw_raw(&state,&input,(uint8_t*)color,depth,32,textures)==1);
  }
  assert(raw.count==320);assert(nv_gpu_sync(&count)==1 && geometry_dispatches==before+2);
  if(pass)assert(raw.uploads==uploads && raw.hits==hits+320);
 }
 puts("PASS: 320 unique tiny resident meshes share two compute dispatches per queue; warm second pass makes zero mesh uploads");


 for(unsigned pass=0;pass<2;pass++){
  uint64_t uploads=raw.uploads,hits=raw.hits;unsigned before=geometry_dispatches;
  for(unsigned mesh=0;mesh<3072;mesh++){
   vertices[0][0]=5+(float)mesh/16384;assert(nv_gpu_draw_raw(&state,&input,(uint8_t*)color,depth,32,textures)==1);
  }
  assert(nv_gpu_sync(&count)==1 && geometry_dispatches==before+12);
  if(pass)assert(raw.uploads==uploads && raw.hits==hits+3072);
 }
 assert(raw.entries>1024 && raw.host_bytes<=NV_RAW_HOST_LIMIT);
 puts("PASS: 3072-mesh working set survives queue-limit flushes with zero warm uploads and twelve geometry dispatches per pass");
 /* Keep an accepted resident version pinned while more than the entire cache
  * worth of CPU-only content arrives at the same backing address. */
 for(unsigned i=0;i<1024;i++){color[i]=0xFF112233;depth[i]=100;}
 vertices[0][0]=4.5f;assert(nv_gpu_draw_raw(&state,&input,(uint8_t*)color,depth,32,textures)==1);
 unsigned pinned=raw.meshes[raw.count-1];NvRawMesh*pin=&raw.mesh[pinned];uint64_t pin_key=pin->key;unsigned pin_base=pin->atlas_begin;
 raw.ref_bytes=pin->bytes;raw.ref_attrs=pin->attrs;raw.ref_draws=raw.draws;raw.ref_words=raw.words;
 NvGpuTri reference=nv_raw_prepare(raw.draws[raw.count-1].p,nv_raw_vertex(raw.count-1,0),nv_raw_vertex(raw.count-1,1),nv_raw_vertex(raw.count-1,2));
 memcpy(expected,color,sizeof color);memcpy(expected_depth,depth,sizeof depth);
 for(unsigned t=0;t<pin->triangles;t++){
  unsigned j=t*3;reference=nv_raw_prepare(raw.draws[raw.count-1].p,nv_raw_vertex(raw.count-1,pin->indices[j]),nv_raw_vertex(raw.count-1,pin->indices[j+1]),nv_raw_vertex(raw.count-1,pin->indices[j+2]));
  for(unsigned y=0;y<32;y++)for(unsigned x=0;x<32;x++){unsigned at=y*32+x;NvGpuPixel p=gpu_pixel(state,reference,x,y,expected[at],expected_depth[at]);expected[at]=p.color;expected_depth[at]=p.depth;}
 }
 for(unsigned i=0;i<NV_RAW_MESHES+64;i++){
  vertices[0][0]=4.51f+(float)i/65536;assert(!gpu_raw_mesh_equal(pin,&input));assert(gpu_raw_mesh(&input));
  assert(pin->pinned && pin->resident && pin->key==pin_key && pin->atlas_begin==pin_base);
 }
 assert(nv_gpu_sync(&count)==1);assert(!memcmp(color,expected,sizeof color) && !memcmp(depth,expected_depth,sizeof depth));
 puts("PASS: accepted resident version survives more than 1024 same-address cache replacements while pinned; full-byte proof rejects mutations; final pixels/depth retain the original version");
 size_t saved_bytes=input.stream[0].bytes;input.stream[0].bytes=1;
 assert(nv_gpu_draw_raw(&state,&input,(uint8_t*)color,depth,32,textures)==0);input.stream[0].bytes=saved_bytes;
 /* Bounds and controls: uncertain/partial goes to CPU, whole outside rejects
  * only when requested; changed bytes and indices create new cache entries. */
 vertices[0][0]=-10;assert(nv_gpu_draw_raw(&state,&input,(uint8_t*)color,depth,32,textures)==0);
 for(unsigned v=0;v<4;v++)vertices[v][0]=-50-(float)v;
 assert(nv_gpu_draw_raw(&state,&input,(uint8_t*)color,depth,32,textures)==2);
 input.bounds_reject_enabled=0;assert(nv_gpu_draw_raw(&state,&input,(uint8_t*)color,depth,32,textures)==0);
 vertices[0][0]=NAN;assert(nv_gpu_draw_raw(&state,&input,(uint8_t*)color,depth,32,textures)==0);
 input.program_id=999;assert(nv_gpu_draw_raw(&state,&input,(uint8_t*)color,depth,32,textures)==0);input.program_id=0;
 s_capture=(FILE*)1;assert(!nv_gpu_geometry_available());s_capture=NULL;

 /* Conservative interval bounds checked against actual scalar vertex output
  * over all 180 programs; random affine matrices, W ranges and viewports. */
 uint32_t random=0x12345678;unsigned kinds[256];
 for(unsigned id=0;id<raw.program_count;id++){
  unsigned n;const uint32_t(*prog)[4]=nv2a_vsh_known_program(id,&n);
  for(unsigned j=0;j<n;j++)nv2a_vsh_set_instruction(j,prog[j]);nv2a_vsh_set_start_slot(0);
  Nv2aVshSnapshot snap;assert(nv2a_vsh_snapshot(&snap));kinds[id]=snap.position_kind;
 }
 for(unsigned trial=0;trial<1800;trial++){
  unsigned id=trial%raw.program_count;input.program_id=id;input.position_kind=kinds[id];
  for(unsigned v=0;v<4;v++)for(unsigned j=0;j<4;j++){
   random=random*1664525u+1013904223u;vertices[v][j]=((int)(random%2001)-1000)/50.0f;
  }
  for(unsigned row=0;row<4;row++)for(unsigned j=0;j<4;j++){
   random=random*1664525u+1013904223u;constants[96+row][j]=((int)(random%2001)-1000)/1000.0f;
  }
  constants[58][0]=constants[58][1]=1;constants[59][0]=constants[59][1]=16;
  NvRawMesh*m=gpu_raw_mesh(&input);assert(m);int bounds=gpu_raw_bounds(m,&input);
  NvRawDraw draw={0};draw.p=input.params;draw.p.program=raw.offsets[id];unsigned n;nv2a_vsh_known_program(id,&n);draw.p.count=n;memcpy(draw.constants,constants,sizeof constants);
  raw.ref_draws=&draw;raw.ref_bytes=m->bytes;raw.ref_attrs=m->attrs;raw.ref_words=raw.words;
  unsigned outside=31;
  for(unsigned v=0;v<4;v++){
   Nv2aVshOutput out=nv_raw_vertex(0,v);unsigned code=0;
   if(out.pos[0]<0)code|=1;if(out.pos[0]>32)code|=2;if(out.pos[1]<0)code|=4;if(out.pos[1]>32)code|=8;if(out.pos[3]<=0)code|=16;
   outside&=code;
   if(bounds==1)assert(!code && isfinite(out.pos[0]) && isfinite(out.pos[1]) && isfinite(out.pos[3]));
  }
  if(bounds<0)assert(outside);
 }
 puts("PASS: 1800 interval-bounds property trials across all 180 known program forms; no falsely rejected visible boxes or accepted boundary boxes");

 /* Correlated vertices stress the faster point proof: independent extrema
  * cross the viewport even though the actual positions often do not. */
 for(unsigned trial=0;trial<1800;trial++){
  unsigned id=trial%raw.program_count;input.program_id=id;input.position_kind=kinds[id];
  for(unsigned v=0;v<4;v++){
   random=random*1664525u+1013904223u;
   float t=((int)(random%601)-300)/25.0f;
   random=random*1664525u+1013904223u;
   float delta=((int)(random%101)-50)/15.0f;
   vertices[v][0]=t;vertices[v][1]=t+delta;vertices[v][2]=10;vertices[v][3]=1;
  }
  for(unsigned row=0;row<4;row++)for(unsigned j=0;j<4;j++)constants[96+row][j]=0;
  constants[96][0]=1;constants[96][1]=-1;constants[96][3]=16;
  constants[97][0]=0.5f;constants[97][3]=16;
  constants[98][2]=1;constants[99][3]=1;
  constants[58][0]=constants[58][1]=constants[58][2]=1;
  constants[59][0]=constants[59][1]=0;
  NvRawMesh*m=gpu_raw_mesh(&input);assert(m);int bounds=gpu_raw_bounds(m,&input);
  NvRawDraw draw={0};draw.p=input.params;draw.p.program=raw.offsets[id];unsigned n;
  nv2a_vsh_known_program(id,&n);draw.p.count=n;memcpy(draw.constants,constants,sizeof constants);
  raw.ref_draws=&draw;raw.ref_bytes=m->bytes;raw.ref_attrs=m->attrs;raw.ref_words=raw.words;
  unsigned outside=31;
  for(unsigned v=0;v<4;v++){
   Nv2aVshOutput out=nv_raw_vertex(0,v);unsigned code=0;
   if(out.pos[0]<0)code|=1;if(out.pos[0]>32)code|=2;
   if(out.pos[1]<0)code|=4;if(out.pos[1]>32)code|=8;
   if(out.pos[3]<=0)code|=16;outside&=code;
   if(bounds==1)assert(!code && isfinite(out.pos[0]) && isfinite(out.pos[1]) && isfinite(out.pos[3]));
  }
  if(bounds<0)assert(outside);
 }
 puts("PASS: 1800 correlated-vertex proofs across all 180 program forms preserve inside and outside decisions");

 for(unsigned i=0;i<NV_RAW_MESHES;i++)gpu_raw_mesh_release(&raw.mesh[i]);
 assert(!raw.entries && !raw.resident_entries && raw.free_slot_count==NV_RAW_MESHES && !raw.lru_head && !raw.lru_tail);
 assert(!raw.host_bytes && !raw.resident_bytes && raw.free_count==1 && raw.free_ranges[0].first==0 && raw.free_ranges[0].bytes==NV_RAW_ATLAS_LIMIT);
 for(unsigned i=0;i<NV_RAW_HASH;i++)assert(!raw.buckets[i]);
 unsigned allocated[512],sizes[512];
 for(unsigned i=0;i<512;i++){sizes[i]=(i%17+1)*128;assert(gpu_raw_range_alloc(sizes[i],&allocated[i]));}
 for(unsigned phase=0;phase<2;phase++)for(unsigned i=phase;i<512;i+=2)gpu_raw_range_free(allocated[i],sizes[i]);
 assert(raw.free_count==1 && raw.free_ranges[0].bytes==NV_RAW_ATLAS_LIMIT);unsigned offset;
 assert(gpu_raw_range_alloc(NV_RAW_ATLAS_LIMIT,&offset) && !offset && !gpu_raw_range_alloc(4,&offset));
 gpu_raw_range_free(0,NV_RAW_ATLAS_LIMIT);
 puts("PASS: bounded cache/hash eviction and resident range reclamation; fragmented free ranges fully coalesce, capacity exhaustion cannot overlap live data");
 nv_gpu_report();puts("PASS: exact-bounds fallback, nonfinite/unsupported input fallback, frustum opt-out, capture opt-out, bounded mesh cache");return 0;
}
'''
if __name__=='__main__':
 with tempfile.TemporaryDirectory(prefix='qpr2-gpu-geometry-') as d:
  p=Path(d);(p/'logs').mkdir()
  for name in ['windows.h','d3d11.h','d3d11_3.h','d3dcompiler.h','dxgi.h']:(p/name).write_text('/* API above */\n')
  (p/'fixture.c').write_text(api+'ptrdiff_t xbox_GetMemoryOffset(void){return 0;}\n#include "'+str(k/'nv2a_gpu_raster.c')+'"\n'+fixture+main)
  subprocess.run(['gcc','-std=gnu11','-O1','-ffp-contract=off','-Werror=implicit-function-declaration','-Werror=incompatible-pointer-types','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-I'+str(p),'-I'+str(root/'tools'),'-I'+str(k),str(p/'fixture.c'),str(k/'nv2a_vsh_interp.c'),str(k/'nv2a_gpu_tiles.c'),'-pthread','-lm','-o',str(p/'fixture')],check=True)
  for mode in (sys.argv[1:] or ['native','compile','create','vertex','prepare','map','compute','coverage','disabled','atlas']):
   print('RUN geometry '+mode,flush=True)
   for f in (p/'shader-cache').glob('*'):f.unlink()
   subprocess.run([str(p/'fixture'),mode],cwd=p,timeout=120,check=True)
