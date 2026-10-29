"""Resident mesh eviction under scan pressure, byte identity and pinned draws.
Actual production cache/geometry/API paths; synthetic cache replay is not FPS.
"""
from pathlib import Path
import subprocess,tempfile
import test_qpr2_gpu_geometry as geo
root=geo.root;k=geo.k
head=geo.main[:geo.main.index(' uint64_t reads=g.readbacks;')]
a=' assert(nv_gpu_available());'
assert a in head
head=head.replace(a,''' setenv("RECOMP_GPU_MESH_CACHE_2Q",!strcmp(mode,"lru") || !strcmp(mode,"legacy8192")?"0":"1",1);
 setenv("RECOMP_GPU_MESH_CACHE_SLOTS",!strcmp(mode,"legacy8192")?"8192":!strcmp(mode,"default")?"32768":!strcmp(mode,"invalid")?"invalid":"64",1);
 if(!strcmp(mode,"metadata"))target_create_fail_bytes=64u*NV_RAW_META*sizeof(NvRawAttr);
 assert(nv_gpu_available());
 if(!strcmp(mode,"metadata")){assert(!raw.ready && !nv_gpu_geometry_available());puts("PASS: mesh metadata allocation failure retains CPU geometry and validated pixels");return 0;}
 assert(raw.cache_limit==(!strcmp(mode,"legacy8192")?8192u:(!strcmp(mode,"default") || !strcmp(mode,"invalid"))?32768u:64u));
 assert(raw.metadata.bytes==raw.cache_limit*NV_RAW_META*sizeof(NvRawAttr));
 if(!strcmp(mode,"default") || !strcmp(mode,"invalid") || !strcmp(mode,"legacy8192")){nv_gpu_report();puts("PASS: bounded slot configuration and metadata allocation; V98 slot/LRU opt-out");return 0;}
''')
helpers=r'''
static void mesh_cache_check(void){
 unsigned *seen=calloc(NV_RAW_MESHES,sizeof *seen);assert(seen);unsigned entries=0,hot=0,resident=0;size_t host=0,bytes=0;
 for(unsigned q=0;q<2;q++){
  unsigned last=0,n=0;
  for(unsigned link=q?raw.lru_hot_head:raw.lru_head;link;link=raw.mesh[link-1].lru_next){
   assert(link<=raw.cache_limit && ++n<=raw.cache_limit);NvRawMesh*m=&raw.mesh[link-1];assert(m->bytes && !seen[link-1]++ && m->lru_prev==last && m->cache_hot==q);last=link;++entries;hot+=q;
   host+=m->byte_count+(size_t)m->count*4+(size_t)m->triangles*12;
   if(m->resident){++resident;bytes+=m->atlas_bytes;assert((uint64_t)m->atlas_begin+m->atlas_bytes<=NV_RAW_ATLAS_LIMIT);}
  }
  assert(last==(q?raw.lru_hot_tail:raw.lru_tail));
 }
 for(unsigned i=0;i<raw.free_slot_count;i++){assert(raw.free_slots[i]<raw.cache_limit && !seen[raw.free_slots[i]]++);}
 for(unsigned i=0;i<raw.cache_limit;i++)assert(seen[i]==1);
 assert(entries==raw.entries && hot==raw.hot_entries && raw.entries+raw.free_slot_count==raw.cache_limit && resident==raw.resident_entries);
 assert(host==raw.host_bytes && bytes==raw.resident_bytes && host<=NV_RAW_HOST_LIMIT && bytes<=NV_RAW_ATLAS_LIMIT);
 free(seen);
}
static void mesh_cache_clear(void){
 assert(!raw.count && !g.queued_count);
 for(unsigned i=0;i<NV_RAW_MESHES;i++){assert(!raw.mesh[i].pinned);gpu_raw_mesh_release(raw.mesh+i);}
 assert(!raw.entries && !raw.hot_entries && raw.free_slot_count==raw.cache_limit && !raw.lru_head && !raw.lru_tail && !raw.lru_hot_head && !raw.lru_hot_tail);
 assert(raw.free_count==1 && !raw.free_ranges[0].first && raw.free_ranges[0].bytes==NV_RAW_ATLAS_LIMIT);mesh_cache_check();
}
static NvRawMesh *mesh_cache_get(NvRawInput *in,float vertices[4][4],unsigned id,int resident){
 vertices[0][0]=2.0f+(float)id/65536.0f;NvRawMesh*m=gpu_raw_mesh(in);assert(m && gpu_raw_mesh_equal(m,in));if(resident)assert(gpu_raw_resident(m));return m;
}
'''
main=head+r'''
 state.zpass_enable=1;
 mesh_cache_clear();
 for(unsigned i=0;i<16;i++)mesh_cache_get(&input,vertices,i,1);
 for(unsigned repeat=0;repeat<4;repeat++)for(unsigned i=0;i<16;i++)mesh_cache_get(&input,vertices,i,1);
 assert(!raw.hot_entries);puts("PASS: same-frame multi-pass hits do not promote changing meshes");
 nv_gpu_capture_present(NULL,0,0);
 NvRawMesh*cpu=mesh_cache_get(&input,vertices,100,0);nv_gpu_capture_present(NULL,0,0);assert(mesh_cache_get(&input,vertices,100,0)==cpu && !cpu->resident && !cpu->cache_hot);
 puts("PASS: CPU-only bounds candidates remain probationary across presents");
 mesh_cache_clear();
 for(unsigned i=0;i<16;i++)mesh_cache_get(&input,vertices,i,1);
 nv_gpu_capture_present(NULL,0,0);for(unsigned i=0;i<16;i++)mesh_cache_get(&input,vertices,i,1);
 assert(raw.hot_entries==(raw.cache_segmented?16u:0u));
 unsigned static_misses=0;uint64_t static_uploads=0;
 for(unsigned frame=0;frame<12;frame++){
  nv_gpu_capture_present(NULL,0,0);
  for(unsigned i=0;i<16;i++){uint64_t misses=raw.misses,uploads=raw.uploads;mesh_cache_get(&input,vertices,i,1);static_misses+=(unsigned)(raw.misses-misses);static_uploads+=raw.uploads-uploads;}
  for(unsigned i=0;i<256;i++){mesh_cache_get(&input,vertices,1000+frame*256+i,1);mesh_cache_get(&input,vertices,1000+frame*256+i,1);}
  mesh_cache_check();
 }
 if(raw.cache_segmented)assert(!static_misses && !static_uploads && raw.hot_entries==16);else assert(static_misses==176 && static_uploads==176 && !raw.hot_entries);
 printf("PASS: %s scan replay; static_misses=%u static_uploads=%llu (synthetic counts, not game FPS)\n",mode,static_misses,(unsigned long long)static_uploads);
 if(!raw.cache_segmented){mesh_cache_clear();return 0;}
 mesh_cache_clear();
 for(unsigned i=0;i<49;i++)mesh_cache_get(&input,vertices,i,1);
 nv_gpu_capture_present(NULL,0,0);for(unsigned i=0;i<49;i++)mesh_cache_get(&input,vertices,i,1);
 assert(raw.hot_entries==48 && raw.cache_demotions && !raw.mesh[raw.lru_head-1].cache_hot);mesh_cache_check();
 puts("PASS: protected queue cap demotes old content without releasing its resident range");
 mesh_cache_clear();
 NvRawMesh *hot0=mesh_cache_get(&input,vertices,6000,1),*hot1=mesh_cache_get(&input,vertices,6001,1);
 nv_gpu_capture_present(NULL,0,0);assert(mesh_cache_get(&input,vertices,6000,1)==hot0 && mesh_cache_get(&input,vertices,6001,1)==hot1);
 NvRawMesh *cold=mesh_cache_get(&input,vertices,6002,0);hot0->pinned=1;
 assert(raw.hot_entries==2 && gpu_raw_victim(1,hot0)==hot1 && gpu_raw_victim(0,hot0)==cold);
 hot1->pinned=1;assert(!gpu_raw_victim(1,NULL));hot0->pinned=hot1->pinned=0;
 puts("PASS: resident pressure falls back to unpinned protected data while CPU-only probation and avoided ranges are excluded");
 mesh_cache_clear();
 /* Force host-byte pressure before entry capacity, retaining a pinned range. */
 uint8_t *padding=calloc(1,1024u*1024u);assert(padding);input.stream[1]=(NvRawStream){padding,1024u*1024u,16,2,4};
 NvRawMesh*pin=mesh_cache_get(&input,vertices,8000,1);pin->pinned=1;uint64_t key=pin->key;unsigned begin=pin->atlas_begin;
 for(unsigned i=0;i<80;i++){mesh_cache_get(&input,vertices,9000+i,1);assert(pin->pinned && pin->key==key && pin->atlas_begin==begin && pin->resident);mesh_cache_check();}
 assert(raw.cache_host_evictions);pin->pinned=0;input.stream[1]=(NvRawStream){0};free(padding);mesh_cache_clear();
 puts("PASS: 64 MiB host pressure cannot evict pinned data; full-byte versions stay distinct at reused addresses");
 /* Reserve an atlas guard so resident pressure occurs before host/slot limits. */
 unsigned guard;assert(gpu_raw_range_alloc(NV_RAW_ATLAS_LIMIT-4096,&guard));
 pin=mesh_cache_get(&input,vertices,7000,1);pin->pinned=1;key=pin->key;begin=pin->atlas_begin;
 for(unsigned i=0;i<256;i++){mesh_cache_get(&input,vertices,7100+i,1);assert(pin->key==key && pin->resident && pin->atlas_begin==begin);}
 assert(raw.cache_atlas_evictions);pin->pinned=0;gpu_raw_range_free(guard,NV_RAW_ATLAS_LIMIT-4096);mesh_cache_clear();
 puts("PASS: atlas pressure skips pinned ranges and fully coalesces freed storage");
 /* Pin every entry through actual accepted draws, then force a submission.
  * Reference all draws independently before checking publication and output. */
 memset(expected,0,sizeof expected);memset(color,0,sizeof color);
 for(unsigned i=0;i<1024;i++)depth[i]=expected_depth[i]=100;
 uint64_t dispatch_before=raw.dispatches,reads=g.readbacks;
 NvGpuCount reference_count={0};
 for(unsigned draw=0;draw<65;draw++){
  vertices[0][0]=2.0f+(float)draw/65536.0f;constants[101][0]=(float)draw/128.0f;
  assert(nv_gpu_draw_raw(&state,&input,(uint8_t*)color,depth,32,textures)==1);
  NvRawMesh*m=&raw.mesh[raw.meshes[raw.count-1]];NvRawDraw*d=&raw.draws[raw.count-1];
  raw.ref_bytes=m->bytes;raw.ref_attrs=m->attrs;raw.ref_draws=raw.draws;raw.ref_words=raw.words;
  for(unsigned t=0;t<m->triangles;t++){
   unsigned j=t*3;NvGpuTri tri=nv_raw_prepare(d->p,nv_raw_vertex(raw.count-1,m->indices[j]),nv_raw_vertex(raw.count-1,m->indices[j+1]),nv_raw_vertex(raw.count-1,m->indices[j+2]));
   for(int y=0;y<32;y++)for(int x=0;x<32;x++){
    unsigned at=(unsigned)y*32+(unsigned)x;NvGpuPixel p=gpu_pixel(state,tri,x,y,expected[at],expected_depth[at]);expected[at]=p.color;expected_depth[at]=p.depth;reference_count.pixels+=p.pixels;reference_count.depth_fail+=p.depth_fail;reference_count.zpass+=p.zpass;
   }
  }
  if(draw==63){assert(raw.entries==64 && raw.count==64);for(unsigned i=0;i<64;i++)assert(raw.mesh[i].pinned);}
 }
 assert(raw.dispatches==dispatch_before+2 && raw.count==1 && g.readbacks==reads);for(unsigned i=0;i<1024;i++)assert(!color[i]);
 assert(nv_gpu_sync(&count)==1 && raw.dispatches==dispatch_before+4);
 assert(!memcmp(color,expected,sizeof color) && !memcmp(depth,expected_depth,sizeof depth));
 assert(reference_count.pixels>0 && reference_count.zpass>0);
 assert(count.pixels==reference_count.pixels && count.depth_fail==reference_count.depth_fail && count.zpass==reference_count.zpass);
 mesh_cache_check();mesh_cache_clear();
 puts("PASS: all-pinned capacity submits original consumers before replacement; 65 actual raw draws preserve pixels/depth/visibility and delayed publication");
 nv_gpu_report();return 0;
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-mesh-cache-') as folder:
 p=Path(folder)
 for name in ['windows.h','d3d11.h','d3d11_3.h','d3dcompiler.h','dxgi.h']:(p/name).write_text('/* strict API */\n')
 (p/'test.c').write_text(geo.api+'ptrdiff_t xbox_GetMemoryOffset(void){return 0;}\n#include "'+str(k/'nv2a_gpu_raster.c')+'"\n'+geo.fixture+helpers+main)
 subprocess.run(['gcc','-std=gnu11','-O1','-ffp-contract=off','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-Werror=implicit-function-declaration','-Werror=incompatible-pointer-types','-I'+str(p),'-I'+str(root/'tools'),'-I'+str(k),str(p/'test.c'),str(k/'nv2a_vsh_interp.c'),str(k/'nv2a_gpu_tiles.c'),'-pthread','-lm','-o',str(p/'test')],check=True)
 for mode in ['segmented','lru','legacy8192','metadata','invalid','default']:
  print('RUN mesh cache '+mode,flush=True);cwd=p/mode;cwd.mkdir();subprocess.run([str(p/'test'),mode],cwd=cwd,check=True,timeout=180)
 print('PASS: all 6 mesh cache admission/budget/pressure/publication modes')
