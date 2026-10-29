"""Native depth snapshots, shader rejection and real publication; API simulation."""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1];k=root/'third_party/xboxrecomp/src/kernel';path=root/'tools/test_qpr2_gpu_device.py'
s=path.read_text();ns={'__file__':str(path)};exec(compile(s[:s.index('with tempfile.TemporaryDirectory(')],str(path),'exec'),ns)
api=ns['api'];fixture=ns['fixture'].split('int main(int argc,char**argv)')[0]
api=api.replace('static int native_batch_corrupt,index_create_fail;', 'static int native_batch_corrupt,index_create_fail;\nstatic int seed_compile_fail,seed_create_fail,seed_color_bad,seed_depth_bad,seed_count_bad,seed_material_bad,seed_map_fail,seed_alloc_fail,general_seed,simple_seed;\nstatic unsigned seed_compiles,seed_skips,seed_shades;')
fixture=fixture.replace('if(shade && bound_seed_srv[1] && *(uint32_t*)bound_seed_srv[1]->buffer->data)',
                        'if(shade && bound_seed_srv[1] && *(uint32_t*)bound_seed_srv[1]->buffer->data && primitive<((uint32_t*)bound_seed_srv[1]->buffer->data)[1])')
a=' if(failed_compile ||';b=''' int seed=0;for(D3D_SHADER_MACRO*m=b;m && m->Name;m++)if(!strcmp(m->Name,"NV_GPU_DEPTH_SEED"))seed=atoi(m->Definition);
 if(seed){assert(!strcmp(e,"ps_5_0"));++seed_compiles;}
 if((seed_compile_fail && seed) || failed_compile ||''';assert a in api;api=api.replace(a,b,1)
a='*out=blob;*error=NULL;return 0;';b='''if(seed){size_t bytes=blob->bytes;char*data=malloc(bytes+16);assert(data);memcpy(data,"//depth-seed-v1\\n",16);memcpy(data+16,blob->data,bytes);if(blob->owned)free((void*)blob->data);blob->data=data;blob->bytes=bytes+16;blob->owned=1;}
 *out=blob;*error=NULL;return 0;''';assert a in api;api=api.replace(a,b,1)
a='int simple=n>16 && !memcmp(p,"//native-simple\\n",16);';b='''int seed=n>16 && !memcmp(p,"//depth-seed-v1\\n",16);if(seed && seed_create_fail)return -1;if(seed){p=(const char*)p+16;n-=16;}int simple=n>16 && !memcmp(p,"//native-simple\\n",16);if(simple)simple_seed=seed;else general_seed=seed;''';assert a in api;api=api.replace(a,b,1)
a=' (void)d;++buffer_creates;';assert a in api;api=api.replace(a,a+'if(seed_alloc_fail && s->Usage==D3D11_USAGE_DEFAULT && s->BindFlags==D3D11_BIND_SHADER_RESOURCE && s->StructureByteStride==8)return -1;',1)
a='   /* Frozen visibility is settled before source work or ordered writes. */';assert a in fixture;fixture=fixture.replace(a,'   if(bary.covered){if(shade)++seed_shades;else ++seed_skips;}\n'+a,1)
a='   uint32_t depth_bits;memcpy(&depth_bits,&p.depth,4);';b='''   if((bound_pixel==&pixel_shader && general_seed) || (bound_pixel==&simple_pixel_shader && simple_seed)){
    if(seed_color_bad)p.color^=0x40;if(seed_depth_bad)p.depth+=1;if(seed_count_bad)p.zpass=0;
    if(seed_material_bad && state->width==256)p.color^=0x40;if(seed_map_fail)fail_next_map=1;
   }
'''+a;assert a in fixture;fixture=fixture.replace(a,b,1)
main=r'''
int main(int argc,char **argv){
 const char *mode=argc>1?argv[1]:"normal";
 setenv("RECOMP_GPU_RASTER","1",1);setenv("RECOMP_GPU_ASYNC","0",1);setenv("RECOMP_GPU_PROFILE","0",1);
 if(!strcmp(mode,"disabled"))setenv("RECOMP_GPU_DEPTH_SEED","0",1);
 if(!strcmp(mode,"array"))setenv("RECOMP_GPU_FIXED","0",1);
 if(!strcmp(mode,"vector-off"))setenv("RECOMP_GPU_VECTOR_COMBINERS","0",1);
 seed_compile_fail=!strcmp(mode,"compile");seed_create_fail=!strcmp(mode,"create");seed_color_bad=!strcmp(mode,"color");seed_depth_bad=!strcmp(mode,"depth");seed_count_bad=!strcmp(mode,"count");seed_material_bad=!strcmp(mode,"material");seed_map_fail=!strcmp(mode,"map");
 native_supported=1;coverage_tier=3;int ready=nv_gpu_available();
 if(seed_map_fail){assert(!ready && s_startup==3 && !g.active && !g.host_color && !g.host_depth);puts("PASS: real seed candidate readback failure rejects readiness");return 0;}
 assert(ready && g.native);
 int candidate=strcmp(mode,"disabled") && !seed_compile_fail && !seed_create_fail && !seed_color_bad && !seed_depth_bad && !seed_count_bad && !seed_material_bad;
 assert(g.seed.enabled==candidate && general_seed==candidate && simple_seed==candidate);
 if(!strcmp(mode,"warm"))assert(!compiler_calls);
 if(!strcmp(mode,"disabled"))assert(!seed_compiles);
 if(!g.array_registers && strcmp(mode,"vector-off"))assert(g.vector_combiner);
 assert(g.quad_sampling);
 int saved=g.seed.enabled;g.seed.enabled=1;unsigned a=gpu_native_variant(0),sa=gpu_native_variant(2);g.seed.enabled=0;unsigned b=gpu_native_variant(0),sb=gpu_native_variant(2);g.seed.enabled=saved;
 assert((a^b)==64 && (sa^sb)==64);
 uint32_t color[1024]={0};float z[1024];for(unsigned i=0;i<1024;i++)z[i]=100;
 NvGpuState state={0};state.width=state.height=32;state.pitch=128;state.bpp=4;state.format=0x128;state.color_mask=0x01010101;
 state.depth_available=state.depth_test=state.depth_mask=state.zpass_enable=1;state.depth_func=0x203;
 NvGpuTri tri[128];for(unsigned i=0;i<128;i++)tri[i]=triangle(32);
 NvGpuTextureSource tex[4]={0};NvGpuCount count;g.seed.batches=g.seed.copies=g.seed.bytes=g.seed.unsafe=g.seed.cold=g.seed.small_batches=g.seed.allocation_failures=0;
 seed_skips=seed_shades=0;
 assert(nv_gpu_draw(&state,tri,128,(uint8_t*)color,z,32,tex)==1 && gpu_flush());
 assert(!g.seed.copies && !color[5*32+5]);
 for(unsigned i=0;i<128;i++)for(unsigned v=0;v<3;v++)tri[i].v[v].pos[2]=20;
 if(!strcmp(mode,"alloc")){gpu_buffer_release(&g.seed.image);seed_alloc_fail=1;}
 if(!strcmp(mode,"no-write"))state.depth_mask=0;
 seed_skips=seed_shades=0;unsigned n=!strcmp(mode,"small")?1:128;
 assert(nv_gpu_draw(&state,tri,n,(uint8_t*)color,z,32,tex)==1);
 if(!strcmp(mode,"unsafe") || !strcmp(mode,"readonly") || !strcmp(mode,"equal") || !strcmp(mode,"never")){
  state.depth_func=!strcmp(mode,"equal")?0x202:!strcmp(mode,"never")?0x200:0x207;if(!strcmp(mode,"readonly"))state.depth_mask=0;
  NvGpuTri raised=tri[0];for(unsigned v=0;v<3;v++)raised.v[v].pos[2]=!strcmp(mode,"equal")?10:100;
  assert(nv_gpu_draw(&state,&raised,1,(uint8_t*)color,z,32,tex)==1);state.depth_func=0x203;state.depth_mask=1;
  assert(nv_gpu_draw(&state,tri,128,(uint8_t*)color,z,32,tex)==1);
 }
 assert(nv_gpu_sync(&count)==1 && color[5*32+5]==0xFFFFFFFF && z[5*32+5]==(!strcmp(mode,"unsafe")?20:10) && count.zpass);
 int use=candidate && strcmp(mode,"alloc") && strcmp(mode,"small");assert(g.seed.copies==(unsigned)use);
 if(use){assert(seed_skips>0);if(!strcmp(mode,"readonly") || !strcmp(mode,"equal") || !strcmp(mode,"never") || !strcmp(mode,"unsafe"))assert(seed_shades>0);else assert(!seed_shades);}
 else assert(!seed_skips && seed_shades>0);
 if(!strcmp(mode,"alloc"))assert(g.seed.allocation_failures==1 && g.seed.enabled);
 if(!strcmp(mode,"unsafe")){assert(g.seed.partial_batches==(unsigned)use);assert(g.seed.partial_triangles==(unsigned)(use?128:0));}if(!strcmp(mode,"small"))assert(g.seed.small_batches>0);
 if(!strcmp(mode,"target")){
  uint32_t other[1024]={0};float oz[1024];for(unsigned i=0;i<1024;i++)oz[i]=100;unsigned copies=g.seed.copies;
  assert(nv_gpu_draw(&state,tri,128,(uint8_t*)other,oz,32,tex)==1 && nv_gpu_sync(&count)==1 && other[5*32+5]==0xFFFFFFFF && oz[5*32+5]==20);assert(g.seed.copies==copies);
 }
 assert(!bound_seed_srv[0] && !bound_seed_srv[1]);nv_gpu_report();printf("PASS: %s; private frozen copy, ordered prefix, real publication and candidate keys\n",mode);
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-depth-seed-gate-') as folder:
 p=Path(folder)
 for name in ['windows.h','d3d11.h','d3d11_3.h','d3dcompiler.h','dxgi.h']:(p/name).write_text('/* strict fixture API */\n')
 (p/'test.c').write_text(api+'ptrdiff_t xbox_GetMemoryOffset(void){return 0;}\n#include "'+str(k/'nv2a_gpu_raster.c')+'"\n'+fixture+main)
 subprocess.run(['gcc','-std=gnu11','-O1','-ffp-contract=off','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-Werror=implicit-function-declaration','-Werror=incompatible-pointer-types','-I'+str(p),'-I'+str(root/'tools'),'-I'+str(k),str(p/'test.c'),str(k/'nv2a_gpu_tiles.c'),str(k/'nv2a_vsh_interp.c'),'-pthread','-lm','-o',str(p/'test')],check=True)
 modes=['normal','warm','disabled','array','vector-off','compile','create','color','depth','count','material','map','alloc','unsafe','small','no-write','target','readonly','equal','never']
 for mode in modes:
  print('RUN depth seed gate '+mode,flush=True);cwd=p/('normal' if mode=='warm' else mode);cwd.mkdir(exist_ok=True);subprocess.run([str(p/'test'),mode],cwd=cwd,check=True,timeout=120)
 print('PASS: all 20 depth-seed cache/failure/ordering modes')
