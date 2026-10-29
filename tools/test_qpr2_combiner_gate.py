"""Production native shader variant keys, startup validation and scalar fallback.
Strict D3D fixture simulates compiler/driver faults; no Windows HLSL/FPS claim.
"""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1];k=root/'third_party/xboxrecomp/src/kernel';path=root/'tools/test_qpr2_gpu_device.py'
s=path.read_text();ns={'__file__':str(path)};exec(compile(s[:s.index('with tempfile.TemporaryDirectory(')],str(path),'exec'),ns)
api=ns['api'];fixture=ns['fixture'].split('int main(int argc,char**argv)')[0]
api=api.replace('static int native_batch_corrupt,index_create_fail;', 'static int native_batch_corrupt,index_create_fail;\nstatic int vector_compile_fail,vector_create_fail,vector_color_bad,vector_depth_bad,vector_count_bad,vector_material_bad,vector_map_fail,general_is_vector;\nstatic unsigned vector_compiles;')
a=' if(failed_compile ||';b=''' int vector_macro=0;for(D3D_SHADER_MACRO*m=b;m && m->Name;m++)if(!strcmp(m->Name,"NV_GPU_VECTOR_COMBINERS"))vector_macro=atoi(m->Definition);
 if(vector_macro){assert(!simple_macro && !strcmp(e,"ps_5_0"));++vector_compiles;}
 if((vector_compile_fail && vector_macro) || failed_compile ||''';assert a in api;api=api.replace(a,b,1)
a=' if(simple_macro && !strcmp(e,"ps_5_0")){';b=''' if(vector_macro){char*data=malloc(n+16);assert(data);memcpy(data,"//native-vector\\n",16);memcpy(data+16,s,n);blob->data=data;blob->bytes=n+16;blob->owned=1;}
 else if(simple_macro && !strcmp(e,"ps_5_0")){''';assert a in api;api=api.replace(a,b,1)
a='int simple=n>16 && !memcmp(p,"//native-simple\\n",16);';b='''int simple=n>16 && !memcmp(p,"//native-simple\\n",16);int vector=n>16 && !memcmp(p,"//native-vector\\n",16);if(vector && vector_create_fail)return -1;if(!simple && !(n>16 && !memcmp(p,"//literal-",10)))general_is_vector=vector;''';assert a in api;api=api.replace(a,b,1)
a='   uint32_t depth_bits;memcpy(&depth_bits,&p.depth,4);';b='''   if(general_is_vector && bound_pixel==&pixel_shader){
    if(vector_color_bad)p.color^=0x40;
    if(vector_depth_bad)p.depth+=1;
    if(vector_count_bad)p.zpass=0;
    if(vector_map_fail)fail_next_map=1;
    if(vector_material_bad && state->width==256)p.color^=0x40;
   }
'''+a;assert a in fixture;fixture=fixture.replace(a,b,1)
main=r'''
int main(int argc,char **argv){
 const char *mode=argc>1?argv[1]:"normal";
 setenv("RECOMP_GPU_RASTER","1",1);setenv("RECOMP_GPU_ASYNC","0",1);setenv("RECOMP_GPU_PROFILE","0",1);
 if(!strcmp(mode,"disabled"))setenv("RECOMP_GPU_VECTOR_COMBINERS","0",1);
 if(!strcmp(mode,"array"))setenv("RECOMP_GPU_FIXED","0",1);
 vector_compile_fail=!strcmp(mode,"compile");vector_create_fail=!strcmp(mode,"create");vector_color_bad=!strcmp(mode,"color");vector_depth_bad=!strcmp(mode,"depth");vector_count_bad=!strcmp(mode,"count");vector_material_bad=!strcmp(mode,"material");vector_map_fail=!strcmp(mode,"map");
 native_corrupt=!strcmp(mode,"both-bad");native_supported=1;coverage_tier=3;int ready=nv_gpu_available();
 if(vector_map_fail){assert(!ready && s_startup==3 && !g.active && !g.host_color && !g.host_depth);puts("PASS: candidate readback failure rejects readiness, without retrying through a false successful completion");return 0;}
 assert(ready);
 int optimized=!strcmp(mode,"normal") || !strcmp(mode,"warm");
 if(!strcmp(mode,"both-bad")){assert(!g.native);puts("PASS: both native variants rejected; independently validated compute fallback remains");return 0;}
 assert(g.native && general_is_vector==optimized);
 if(optimized){assert(g.vector_combiner && !g.array_registers);}
 else if(!strcmp(mode,"array")){assert(g.array_registers && !vector_compiles);}
 else assert(!g.vector_combiner && !g.array_registers);
 if(!strcmp(mode,"disabled"))assert(!vector_compiles);
 if(!strcmp(mode,"warm"))assert(!compiler_calls);
 unsigned saved=g.vector_combiner,saved_array=g.array_registers;g.array_registers=0;g.vector_combiner=1;unsigned a=gpu_native_variant(0),simple_a=gpu_native_variant(2);g.vector_combiner=0;unsigned b=gpu_native_variant(0),simple_b=gpu_native_variant(2);g.vector_combiner=saved;g.array_registers=saved_array;
 assert(a!=b && simple_a==simple_b);
 assert(gpu_compile_key(nv_gpu_rov_source,sizeof nv_gpu_rov_source-1,"pixel_main","ps_5_0",a)!=gpu_compile_key(nv_gpu_rov_source,sizeof nv_gpu_rov_source-1,"pixel_main","ps_5_0",b));
 uint32_t color[256]={0};float z[256];for(unsigned i=0;i<256;i++)z[i]=100;
 NvGpuState state={0};state.width=state.height=16;state.pitch=64;state.bpp=4;state.format=0x128;state.color_mask=0x01010101;
 state.depth_available=state.depth_test=state.depth_mask=state.zpass_enable=1;state.depth_func=0x203;
 NvGpuTri tri=triangle(16);NvGpuTextureSource tex[4]={0};NvGpuCount count;
 assert(nv_gpu_draw(&state,&tri,1,(uint8_t*)color,z,16,tex)==1 && !color[5*16+5]);
 assert(nv_gpu_sync(&count)==1 && color[5*16+5]==0xFFFFFFFF && z[5*16+5]==10 && count.zpass);
 nv_gpu_report();printf("PASS: %s; validated variant/fallback, distinct cache key, unchanged simple variant and exact final output with real delayed publication\n",mode);
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-combiner-gate-') as folder:
 p=Path(folder)
 for name in ['windows.h','d3d11.h','d3d11_3.h','d3dcompiler.h','dxgi.h']:(p/name).write_text('/* strict fixture API */\n')
 (p/'test.c').write_text(api+'ptrdiff_t xbox_GetMemoryOffset(void){return 0;}\n#include "'+str(k/'nv2a_gpu_raster.c')+'"\n'+fixture+main)
 subprocess.run(['gcc','-std=gnu11','-O1','-ffp-contract=off','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-Werror=implicit-function-declaration','-Werror=incompatible-pointer-types','-I'+str(p),'-I'+str(root/'tools'),'-I'+str(k),str(p/'test.c'),str(k/'nv2a_gpu_tiles.c'),str(k/'nv2a_vsh_interp.c'),'-pthread','-lm','-o',str(p/'test')],check=True)
 for mode in ['normal','warm','disabled','array','compile','create','color','depth','count','material','map','both-bad']:
  print('RUN combiner gate '+mode,flush=True);cwd=p/('normal' if mode=='warm' else mode);cwd.mkdir(exist_ok=True);subprocess.run([str(p/'test'),mode],cwd=cwd,check=True,timeout=120)
 print('PASS: all 12 native vector-combiner gate/cache/fallback modes')
