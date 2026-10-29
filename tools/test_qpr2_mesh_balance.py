"""Byte-budget and stale-protection admission tests using actual resident cache.
Compare V99 poisoning at equal capacities; synthetic counts are not FPS.
"""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1];path=root/'tools/test_qpr2_mesh_cache.py'
s=path.read_text();ns={'__file__':str(path)};exec(compile(s[:s.index('\nwith tempfile.TemporaryDirectory(')],str(path),'exec'),ns)
geo=ns['geo'];k=geo.k;head=ns['head'];helpers=ns['helpers']
head=head.replace(' assert(nv_gpu_available());',' setenv("RECOMP_GPU_MESH_CACHE_BALANCE",!strcmp(mode,"v99")?"0":"1",1);\n assert(nv_gpu_available());',1)
helpers=helpers.replace('size_t host=0,bytes=0;','size_t host=0,bytes=0,hot_host=0,hot_gpu=0;')
helpers=helpers.replace('host+=m->byte_count+(size_t)m->count*4+(size_t)m->triangles*12;','host+=m->byte_count+(size_t)m->count*4+(size_t)m->triangles*12;if(m->cache_hot){hot_host+=gpu_raw_mesh_host_cost(m);hot_gpu+=m->atlas_bytes;assert(m->resident);}')
helpers=helpers.replace(' free(seen);',' assert(hot_host==raw.hot_host_bytes && hot_gpu==raw.hot_atlas_bytes);if(raw.cache_balance)assert(hot_host<=NV_RAW_HOST_LIMIT*3u/4u);\n free(seen);')
helpers=helpers.replace('assert(!raw.entries && !raw.hot_entries','assert(!raw.hot_host_bytes && !raw.hot_atlas_bytes);\n assert(!raw.entries && !raw.hot_entries')
main=head+r'''
 state.zpass_enable=1;mesh_cache_clear();
 assert(raw.cache_balance==(strcmp(mode,"v99") && strcmp(mode,"lru")));
 if(!strcmp(mode,"lru")){
  for(unsigned i=0;i<8;i++)mesh_cache_get(&input,vertices,i,1);
  for(unsigned frame=0;frame<80;frame++){nv_gpu_capture_present(NULL,0,0);for(unsigned i=0;i<8;i++)mesh_cache_get(&input,vertices,i,1);}
  assert(!raw.hot_entries && !raw.hot_host_bytes && !raw.cache_age_demotions && !raw.cache_byte_demotions);mesh_cache_clear();puts("PASS: single LRU ignores protected balancing and aging");return 0;
 }
 if(!strcmp(mode,"aging")){
  NvRawMesh*m=mesh_cache_get(&input,vertices,100,0);
  nv_gpu_capture_present(NULL,0,0);assert(mesh_cache_get(&input,vertices,100,0)==m && !m->cache_hot);
  assert(gpu_raw_resident(m));for(unsigned i=0;i<4;i++)assert(mesh_cache_get(&input,vertices,100,1)==m && !m->cache_hot);
  nv_gpu_capture_present(NULL,0,0);assert(mesh_cache_get(&input,vertices,100,1)==m && m->cache_hot);
  puts("PASS: first GPU use of an old CPU-only candidate cannot promote via same-frame repeats");
  mesh_cache_clear();NvRawMesh*saved[8];uint64_t keys[8];unsigned bases[8];
  for(unsigned i=0;i<8;i++){saved[i]=mesh_cache_get(&input,vertices,i,1);keys[i]=saved[i]->key;bases[i]=saved[i]->atlas_begin;}
  nv_gpu_capture_present(NULL,0,0);for(unsigned i=0;i<8;i++)assert(mesh_cache_get(&input,vertices,i,1)==saved[i]);assert(raw.hot_entries==8);
  for(unsigned frame=0;frame<63;frame++)nv_gpu_capture_present(NULL,0,0);gpu_raw_cache_age();assert(raw.hot_entries==8);
  uint64_t aged=raw.cache_age_demotions,uploads=raw.uploads;
  nv_gpu_capture_present(NULL,0,0);mesh_cache_get(&input,vertices,999,0);assert(!raw.hot_entries && raw.cache_age_demotions==aged+8);
  for(unsigned i=0;i<8;i++)assert(saved[i]->resident && saved[i]->key==keys[i] && saved[i]->atlas_begin==bases[i] && !saved[i]->cache_hot);
  assert(mesh_cache_get(&input,vertices,0,1)==saved[0] && raw.uploads==uploads && raw.hot_entries==1);mesh_cache_check();mesh_cache_clear();
  puts("PASS: 63/64 idle-frame boundary demotes stale protection without freeing immutable versions; reuse makes no upload");return 0;
 }
 if(!strcmp(mode,"pinned")){
  NvRawMesh*pin=mesh_cache_get(&input,vertices,100,1);nv_gpu_capture_present(NULL,0,0);assert(mesh_cache_get(&input,vertices,100,1)==pin && pin->cache_hot);
  assert(nv_gpu_draw_raw(&state,&input,(uint8_t*)color,depth,32,textures)==1 && pin->pinned);uint64_t key=pin->key;unsigned base=pin->atlas_begin;
  NvRawDraw*d=&raw.draws[0];raw.ref_bytes=pin->bytes;raw.ref_attrs=pin->attrs;raw.ref_draws=raw.draws;raw.ref_words=raw.words;NvGpuCount ref={0};
  for(unsigned t=0;t<pin->triangles;t++){
   unsigned j=t*3;NvGpuTri tri=nv_raw_prepare(d->p,nv_raw_vertex(0,pin->indices[j]),nv_raw_vertex(0,pin->indices[j+1]),nv_raw_vertex(0,pin->indices[j+2]));
   for(int y=0;y<32;y++)for(int x=0;x<32;x++){unsigned at=(unsigned)y*32+(unsigned)x;NvGpuPixel p=gpu_pixel(state,tri,x,y,expected[at],expected_depth[at]);expected[at]=p.color;expected_depth[at]=p.depth;ref.pixels+=p.pixels;ref.zpass+=p.zpass;ref.depth_fail+=p.depth_fail;}
  }
  uint64_t reads=g.readbacks;
  for(unsigned frame=0;frame<64;frame++)nv_gpu_capture_present(NULL,0,0);
  for(unsigned i=0;i<256;i++){mesh_cache_get(&input,vertices,1000+i,1);assert(pin->pinned && pin->resident && pin->key==key && pin->atlas_begin==base && !pin->cache_hot);mesh_cache_check();}
  assert(raw.cache_age_demotions && g.readbacks==reads && raw.count==1);for(unsigned i=0;i<1024;i++)assert(color[i]==0xFF123456u);
  assert(nv_gpu_sync(&count)==1 && !memcmp(color,expected,sizeof color) && !memcmp(depth,expected_depth,sizeof depth));
  assert(ref.pixels && ref.zpass && count.pixels==ref.pixels && count.zpass==ref.zpass && count.depth_fail==ref.depth_fail);mesh_cache_clear();
  puts("PASS: aged pinned geometry survives mutation/eviction pressure and preserves exact pixels/depth/nonzero visibility with delayed publication");return 0;
 }
 assert(!strcmp(mode,"balanced") || !strcmp(mode,"v99"));
 uint8_t *padding=calloc(1,2u*1024u*1024u);assert(padding);input.stream[1]=(NvRawStream){padding,2u*1024u*1024u,16,2,4};
 for(unsigned i=0;i<28;i++)mesh_cache_get(&input,vertices,i,1);
 nv_gpu_capture_present(NULL,0,0);for(unsigned i=0;i<28;i++)mesh_cache_get(&input,vertices,i,1);
 if(raw.cache_balance)assert(raw.hot_entries<28 && raw.hot_host_bytes<=NV_RAW_HOST_LIMIT*3u/4u && raw.cache_byte_demotions);else assert(raw.hot_entries==28 && raw.hot_host_bytes>NV_RAW_HOST_LIMIT*3u/4u);
 uint64_t reuploads=0,remisses=0,aged=raw.cache_age_demotions;
 for(unsigned pass=0;pass<10;pass++){
  nv_gpu_capture_present(NULL,0,0);
  for(unsigned i=0;i<6;i++){uint64_t uploads=raw.uploads,misses=raw.misses;mesh_cache_get(&input,vertices,1000+i,1);if(pass){reuploads+=raw.uploads-uploads;remisses+=raw.misses-misses;}}
  mesh_cache_check();
 }
 assert(raw.cache_age_demotions==aged); /* Isolate byte budgets from aging. */
 if(raw.cache_balance)assert(!reuploads && !remisses);else assert(reuploads==54 && remisses==54);
 printf("PASS: %s protected-payload replay at equal 64 slots/64 MiB: new_working_set_reuploads=%llu remisses=%llu; synthetic counts, not FPS\n",mode,(unsigned long long)reuploads,(unsigned long long)remisses);
 input.stream[1]=(NvRawStream){0};free(padding);mesh_cache_clear();nv_gpu_report();return 0;
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-mesh-balance-') as folder:
 p=Path(folder)
 for name in ['windows.h','d3d11.h','d3d11_3.h','d3dcompiler.h','dxgi.h']:(p/name).write_text('/* strict API */\n')
 (p/'test.c').write_text(geo.api+'ptrdiff_t xbox_GetMemoryOffset(void){return 0;}\n#include "'+str(k/'nv2a_gpu_raster.c')+'"\n'+geo.fixture+helpers+main)
 subprocess.run(['gcc','-std=gnu11','-O1','-ffp-contract=off','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-Werror=implicit-function-declaration','-Werror=incompatible-pointer-types','-I'+str(p),'-I'+str(root/'tools'),'-I'+str(k),str(p/'test.c'),str(k/'nv2a_vsh_interp.c'),str(k/'nv2a_gpu_tiles.c'),'-pthread','-lm','-o',str(p/'test')],check=True)
 for mode in ['balanced','v99','aging','pinned','lru']:
  print('RUN mesh balance '+mode,flush=True);cwd=p/mode;cwd.mkdir();subprocess.run([str(p/'test'),mode],cwd=cwd,check=True,timeout=180)
 print('PASS: all 5 mesh balance byte/age/pinning/publication modes')
