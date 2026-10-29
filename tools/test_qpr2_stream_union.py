"""Shared immutable stream bytes, independent legacy layout and real queues.
Synthetic byte savings/API commands are not hardware FPS measurements.
"""
from pathlib import Path
import subprocess,tempfile,os
import test_qpr2_gpu_geometry as geo
r=geo.root;k=geo.k
api=geo.api+'\nstatic int stream_union_bad,union_alloc_fail;static void *union_test_calloc(size_t,size_t);\n'
fixture=geo.fixture.replace('if(geometry_bad_vertex)dst[jobs[i].output].pos[0]+=10;', 'if(geometry_bad_vertex || (stream_union_bad && raw.stream_union))dst[jobs[i].output].pos[0]+=10;')
assert fixture!=geo.fixture
helpers=r'''
static void *union_test_calloc(size_t a,size_t b){if(union_alloc_fail){union_alloc_fail=0;return NULL;}return calloc(a,b);}
static void union_clear(void){
 assert(!raw.count && !g.queued_count);
 for(unsigned i=0;i<NV_RAW_MESHES;i++){assert(!raw.mesh[i].pinned);gpu_raw_mesh_release(raw.mesh+i);}
 assert(!raw.entries && !raw.host_bytes && !raw.resident_bytes);
}
static unsigned rng=0x10217151u;
static unsigned random_next(void){rng=rng*1664525u+1013904223u;return rng;}
static size_t original_bytes(const NvRawInput *in){size_t bytes=0;for(unsigned a=0;a<16;a++)bytes+=(in->stream[a].bytes+11u)&~(size_t)3u;return bytes;}
/* Independent baseline packer: no union-planning or mesh-cache helper. */
static void legacy_pack(const NvRawInput *in,uint8_t *data,NvRawAttr *attrs){
 unsigned offset=0;
 for(unsigned a=0;a<16;a++){
  const NvRawStream *s=in->stream+a;attrs[a]=(NvRawAttr){offset,s->stride,s->type,s->size};
  memset(data+offset,0,(s->bytes+11u)&~(size_t)3u);
  if(s->bytes)memcpy(data+offset,s->data,s->bytes);offset+=(unsigned)((s->bytes+11u)&~(size_t)3u);
 }
}
static void plans(void){
 uint8_t data[4096],other[512];for(unsigned i=0;i<sizeof data;i++)data[i]=(uint8_t)(i*29u);
 NvRawInput in={0};NvRawStreamPlan p;assert(gpu_raw_stream_plan(&in,&p,1) && !p.count && !p.logical && !p.unique && p.bytes==8);
 in.stream[5]=(NvRawStream){data,32,8,2,2};in.stream[1]=(NvRawStream){data+48,32,8,2,2};in.stream[9]=(NvRawStream){data+24,40,8,2,2};
 assert(gpu_raw_stream_plan(&in,&p,1) && p.count==1 && p.unique==80 && p.offset[5]==0 && p.offset[1]==48 && p.offset[9]==24);
 in.stream[9]=(NvRawStream){data+32,16,8,2,2};assert(gpu_raw_stream_plan(&in,&p,1) && p.count==3); /* Adjacent objects stay distinct. */
 memset(&in,0,sizeof in);in.stream[0]=(NvRawStream){data,64,16,2,4};in.stream[1]=(NvRawStream){data+3,7,1,4,1};
 assert(gpu_raw_stream_plan(&in,&p,1) && p.count==1 && p.offset[1]==3 && p.unique==64);
 in.stream[0].data=(const uint8_t*)(uintptr_t)(UINTPTR_MAX-2);in.stream[0].bytes=4;assert(!gpu_raw_stream_plan(&in,&p,1));
 in.stream[0].data=NULL;assert(!gpu_raw_stream_plan(&in,&p,1));in.stream[0].bytes=4u*1024u*1024u+1;assert(!gpu_raw_stream_plan(&in,&p,1));
 unsigned coverage[4096],checks=0;
 for(unsigned trial=0;trial<10000;trial++){
  memset(&in,0,sizeof in);memset(coverage,0,sizeof coverage);size_t logical=0,unique=0;
  for(unsigned a=0;a<16;a++){
   unsigned off=random_next()%3000,n=random_next()%128;if((random_next()&7)==0)n=0;
   in.stream[a]=(NvRawStream){data+off,n,16,2,4};logical+=n;
   for(unsigned j=0;j<n;j++)coverage[off+j]=1;
  }
  for(unsigned j=0;j<4096;j++)unique+=coverage[j];
  assert(gpu_raw_stream_plan(&in,&p,1) && p.logical==logical && p.unique==unique && p.bytes<=original_bytes(&in));
  for(unsigned i=0;i<p.count;i++)for(size_t j=0;j<p.spans[i].bytes;j++)assert(coverage[p.spans[i].first-(uintptr_t)data+j]);
  uint8_t packed[8192]={0};for(unsigned i=0;i<p.count;i++)memcpy(packed+p.spans[i].offset,(void*)p.spans[i].first,p.spans[i].bytes);
  for(unsigned a=0;a<16;a++)if(in.stream[a].bytes){assert(!memcmp(packed+p.offset[a],in.stream[a].data,in.stream[a].bytes));++checks;}
 }
 /* Canonical order does not depend on the address ordering of disjoint groups. */
 memset(&in,0,sizeof in);in.stream[0]=(NvRawStream){other,20,4,2,1};in.stream[8]=(NvRawStream){data+16,20,4,2,1};
 assert(gpu_raw_stream_plan(&in,&p,1) && p.count==2 && p.offset[0]==0 && p.offset[8]==28);
 puts("PASS: 10000 independently enumerated union layouts; bridge/nesting, adjacent isolation, null/overflow/oversize rejection and canonical group order");
}
static void typed_views(NvRawInput *in){
 /* Unaligned groups, all six formats. Scalar loader reads a few padding bytes;
  * only the format's advertised element may contribute to the result. */
 uint8_t source[2048],baseline[32768];NvRawAttr attrs[16];
 for(unsigned i=0;i<sizeof source;i++)source[i]=(uint8_t)random_next();
 const unsigned kinds[6]={0,1,2,4,5,6};
 for(unsigned kind=0;kind<6;kind++)for(unsigned size=1;size<=4;size++){
  for(unsigned i=0;i<16;i++)in->stream[i]=(NvRawStream){0};
  float position[4][4]={{2,2,20,1},{27,3,20,1},{4,27,20,1},{25,25,20,1}};
  in->stream[0]=(NvRawStream){(uint8_t*)position,sizeof position,16,2,4};
  for(unsigned a=1;a<16;a++)in->stream[a]=(NvRawStream){source+a,(size_t)3*31+16,31,kinds[kind],size};
  legacy_pack(in,baseline,attrs);NvRawMesh*m=gpu_raw_mesh(in);assert(m && gpu_raw_mesh_equal(m,in));if(!raw.stream_union)assert(m->byte_count==original_bytes(in) && !memcmp(m->bytes,baseline,m->byte_count) && !memcmp(m->attrs,attrs,sizeof attrs));
  for(unsigned v=0;v<4;v++)for(unsigned a=0;a<16;a++){
   raw.ref_bytes=baseline;raw.ref_attrs=attrs;NvRawVec expect=nv_raw_attribute(0,v,a);
   raw.ref_bytes=m->bytes;raw.ref_attrs=m->attrs;NvRawVec got=nv_raw_attribute(0,v,a);assert(!memcmp(&got,&expect,sizeof got));
  }
 }
 union_clear();puts("PASS: all six attribute formats/sizes at unaligned overlapping addresses match independent separate-stream decoding exactly");
}
'''
head=geo.main[:geo.main.index(' uint64_t reads=g.readbacks;')]
head=head.replace('assert(s_startup==3 && !raw.ready);','assert(s_startup==3 && !raw.ready && raw.stream_union && !raw.stream_union_rejected);',1)
head=head.replace(' geometry_fail_compile=', ' plans();\n geometry_fail_compile=',1)
a=' assert(nv_gpu_available());'
b=''' stream_union_bad=!strcmp(mode,"reject");geometry_bad_vertex=!strcmp(mode,"both");geometry_fail_map=!strcmp(mode,"map");
 setenv("RECOMP_GPU_STREAM_UNION",!strcmp(mode,"legacy")?"0":"1",1);setenv("RECOMP_GPU_MESH_CACHE_SLOTS","64",1);
 if(geometry_fail_map){assert(!nv_gpu_available() && s_startup==3 && !raw.ready && !raw.stream_union_rejected);puts("PASS: actual Map failure rejects readiness without retrying storage policy");return 0;}
 assert(nv_gpu_available());
 if(!strcmp(mode,"both")){assert(!raw.ready && raw.stream_union_rejected);puts("PASS: both storage representations rejected retain CPU geometry");return 0;}
 assert(raw.stream_union==(strcmp(mode,"legacy") && strcmp(mode,"reject")));
 assert(raw.stream_union_rejected==(!strcmp(mode,"reject")));
 if(!strcmp(mode,"reject")){assert(raw.ready && nv_gpu_geometry_available());puts("PASS: union-only corruption rechecks and retains valid separate-stream geometry");return 0;}
'''
assert a in head;head=head.replace(a,b,1)
main=head+r'''
 state.zpass_enable=1;union_clear();
 NvRawInput empty=input;memset(empty.stream,0,sizeof empty.stream);NvRawMesh*defaults=gpu_raw_mesh(&empty);
 assert(defaults && defaults->byte_count==(raw.stream_union?8u:128u) && gpu_raw_mesh_equal(defaults,&empty) && gpu_raw_resident(defaults));union_clear();
 typed_views(&input);input.stream[0]=(NvRawStream){(const uint8_t*)vertices,sizeof vertices,16,2,4};
 for(unsigned a=1;a<16;a++)input.stream[a]=(NvRawStream){0};
 float interleaved[4][16][4];for(unsigned v=0;v<4;v++)for(unsigned a=0;a<16;a++)for(unsigned c=0;c<4;c++)interleaved[v][a][c]=.1f+(float)(v+a+c)*.01f;
 for(unsigned v=0;v<4;v++)memcpy(interleaved[v][0],vertices[v],16);
 for(unsigned a=0;a<16;a++)input.stream[a]=(NvRawStream){(uint8_t*)interleaved+a*16,3*sizeof interleaved[0]+16,sizeof interleaved[0],2,4};
 input.inputs=0xffff;input.count=6;input.primitive=5;
 NvRawMesh*m=gpu_raw_mesh(&input);assert(m && gpu_raw_mesh_equal(m,&input));
 unsigned legacy=(unsigned)original_bytes(&input);assert(m->byte_count==(raw.stream_union?sizeof interleaved+8:legacy));
 printf("PASS: %s immutable payload=%u legacy_payload=%u; original layout and every strided byte preserved\n",mode,m->byte_count,legacy);
 assert(gpu_raw_resident(m));uint64_t uploads=raw.uploads;NvRawMesh*original=m;
 float relocated[4][16][4];memcpy(relocated,interleaved,sizeof relocated);
 for(unsigned a=0;a<16;a++)input.stream[a].data=(uint8_t*)relocated+a*16;
 assert(gpu_raw_mesh(&input)==m && raw.uploads==uploads); /* Relocation, no new upload. */
 relocated[0][15][3]+=.01f;assert(!gpu_raw_mesh_equal(m,&input));assert(gpu_raw_mesh(&input)!=m);
 for(unsigned a=0;a<16;a++)input.stream[a].data=(uint8_t*)interleaved+a*16;
 assert(gpu_raw_mesh(&input)==original);
 union_clear();
 /* Allocation failure leaves no half-inserted version or budget mutation. */
 union_alloc_fail=1;unsigned entries=raw.entries;size_t bytes=raw.host_bytes;
 assert(!gpu_raw_mesh(&input) && raw.entries==entries && raw.host_bytes==bytes);assert(gpu_raw_mesh(&input));union_clear();
 /* More than64 distinct accepted draws: source mutates, old unions remain
  * pinned, and cache pressure submits them before reusing slots/ranges. */
 uint8_t baseline[32768];NvRawAttr attrs[16];uint64_t reads=g.readbacks,dispatches=raw.dispatches;NvGpuCount ref={0};
 for(unsigned draw=0;draw<80;draw++){
  interleaved[0][0][0]=2+(float)draw/1024;constants[101][0]=(float)draw/128;constants[101][3]=.75f;
  legacy_pack(&input,baseline,attrs);
  NvRawDraw cpu={0};memcpy(cpu.constants,constants,sizeof constants);cpu.p=input.params;cpu.p.program=raw.offsets[input.program_id];unsigned instructions;nv2a_vsh_known_program(input.program_id,&instructions);cpu.p.count=instructions;
  raw.ref_draws=&cpu;raw.ref_words=raw.words;raw.ref_bytes=baseline;raw.ref_attrs=attrs;
  /* Fixed independent triangle-list indices, not the production cache. */
  for(unsigned t=0;t<input.count/3;t++){
   unsigned j=t*3;NvGpuTri tri=nv_raw_prepare(cpu.p,nv_raw_vertex(0,input.indices[j]),nv_raw_vertex(0,input.indices[j+1]),nv_raw_vertex(0,input.indices[j+2]));
   for(unsigned y=0;y<32;y++)for(unsigned x=0;x<32;x++){
    unsigned at=y*32+x;NvGpuPixel p=gpu_pixel(state,tri,x,y,expected[at],expected_depth[at]);expected[at]=p.color;expected_depth[at]=p.depth;ref.pixels+=p.pixels;ref.depth_fail+=p.depth_fail;ref.zpass+=p.zpass;
   }
  }
  assert(nv_gpu_draw_raw(&state,&input,(uint8_t*)color,depth,32,textures)==1);
  assert(g.readbacks==reads);
 }
 assert(raw.dispatches>=dispatches+2);for(unsigned i=0;i<1024;i++)assert(color[i]==0xFF123456u);
 assert(nv_gpu_sync(&count)==1 && !memcmp(color,expected,sizeof color) && !memcmp(depth,expected_depth,sizeof depth));
 assert(ref.pixels && ref.zpass && count.pixels==ref.pixels && count.depth_fail==ref.depth_fail && count.zpass==ref.zpass);
 assert(raw.host_bytes<=NV_RAW_HOST_LIMIT && raw.resident_bytes<=NV_RAW_ATLAS_LIMIT);
 union_clear();puts("PASS: 80 actual overlapping raw draws under64-slot pressure match independent legacy pixels/depth/nonzero visibility with delayed publication and immutable source versions");
 nv_gpu_report();return 0;
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-stream-union-') as directory:
 p=Path(directory)
 for name in ['windows.h','d3d11.h','d3d11_3.h','d3dcompiler.h','dxgi.h']:(p/name).write_text('/* strict API */\n')
 (p/'test.c').write_text(api+'ptrdiff_t xbox_GetMemoryOffset(void){return 0;}\n#define calloc union_test_calloc\n#include "'+str(k/'nv2a_gpu_raster.c')+'"\n#undef calloc\n'+fixture+helpers+main)
 subprocess.run(['gcc','-std=gnu11','-O1','-ffp-contract=off','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-Werror=implicit-function-declaration','-Werror=incompatible-pointer-types','-I'+str(p),'-I'+str(r/'tools'),'-I'+str(k),str(p/'test.c'),str(k/'nv2a_vsh_interp.c'),str(k/'nv2a_gpu_tiles.c'),'-pthread','-lm','-o',str(p/'test')],check=True)
 for mode in ['merged','legacy','reject','both','map']:
  print('RUN stream union '+mode,flush=True);cwd=p/mode;cwd.mkdir();subprocess.run([str(p/'test'),mode],cwd=cwd,check=True,timeout=180)
 print('PASS: all 5 immutable stream-union identity/queue/failure modes')
 # AddressSanitizer on the pure planner, avoiding the large mock GPU buffers.
 pure='#include <assert.h>\n#include <stdio.h>\n#include <stdint.h>\n#include <stdlib.h>\n#include <string.h>\n#include "nv2a_gpu_raster.h"\n#include "nv2a_gpu_stream_union.inc"\n'
 pure+=helpers[helpers.index('static unsigned rng='):helpers.index('static void typed_views(')]
 pure+='int main(void){plans();puts("PASS: ASan/UBSan independent stream-plan byte enumeration");}\n'
 (p/'plan.c').write_text(pure)
 subprocess.run(['gcc','-std=c11','-O1','-no-pie','-fsanitize=address,undefined','-fno-sanitize-recover=all','-I'+str(k),str(p/'plan.c'),'-o',str(p/'plan')],check=True)
 # LeakSanitizer cannot inspect sibling tasks in this managed environment.
 # This stack-only proof needs ASan bounds checks, not a process leak scan.
 env=dict(os.environ);env['ASAN_OPTIONS']='detect_leaks=0'
 subprocess.run([str(p/'plan')],check=True,timeout=60,env=env)
