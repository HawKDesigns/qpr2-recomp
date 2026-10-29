"""Native general/simple sampling keys, startup rejection and real readback failure.
API simulation does not compile or execute Windows HLSL.
"""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1];k=root/'third_party/xboxrecomp/src/kernel';path=root/'tools/test_qpr2_gpu_device.py'
s=path.read_text();ns={'__file__':str(path)};exec(compile(s[:s.index('with tempfile.TemporaryDirectory(')],str(path),'exec'),ns)
api=ns['api'];fixture=ns['fixture'].split('int main(int argc,char**argv)')[0]
api=api.replace('static int native_batch_corrupt,index_create_fail;', 'static int native_batch_corrupt,index_create_fail;\nstatic int quad_compile_fail,quad_create_fail,quad_color_bad,quad_depth_bad,quad_count_bad,quad_material_bad,quad_map_fail,quad_simple_map_fail,general_quad,simple_quad;\nstatic unsigned quad_compiles;')
a=' if(failed_compile ||';b=''' int quad=0;for(D3D_SHADER_MACRO*m=b;m && m->Name;m++)if(!strcmp(m->Name,"NV_GPU_QUAD_SAMPLING"))quad=atoi(m->Definition);
 if(quad){assert(!strcmp(e,"ps_5_0"));++quad_compiles;}
 if((quad_compile_fail && quad) || failed_compile ||''';assert a in api;api=api.replace(a,b,1)
a='*out=blob;*error=NULL;return 0;';b='''if(quad){size_t bytes=blob->bytes;char*data=malloc(bytes+16);assert(data);memcpy(data,"//quad-sampling\\n",16);memcpy(data+16,blob->data,bytes);if(blob->owned)free((void*)blob->data);blob->data=data;blob->bytes=bytes+16;blob->owned=1;}
 *out=blob;*error=NULL;return 0;''';assert a in api;api=api.replace(a,b,1)
a='int simple=n>16 && !memcmp(p,"//native-simple\\n",16);';b='''int quad=n>16 && !memcmp(p,"//quad-sampling\\n",16);if(quad && quad_create_fail)return -1;if(quad){p=(const char*)p+16;n-=16;}int simple=n>16 && !memcmp(p,"//native-simple\\n",16);if(simple)simple_quad=quad;else general_quad=quad;''';assert a in api;api=api.replace(a,b,1)
a='   uint32_t depth_bits;memcpy(&depth_bits,&p.depth,4);';b='''   if((bound_pixel==&pixel_shader && general_quad) || (bound_pixel==&simple_pixel_shader && simple_quad)){
    if(quad_color_bad)p.color^=0x40;
    if(quad_depth_bad)p.depth+=1;
    if(quad_count_bad)p.zpass=0;
    if(quad_material_bad && state->width==256)p.color^=0x40;
    if(quad_map_fail || (quad_simple_map_fail && bound_pixel==&simple_pixel_shader))fail_next_map=1;
   }
'''+a;assert a in fixture;fixture=fixture.replace(a,b,1)
main=r'''
int main(int argc,char **argv){
 const char *mode=argc>1?argv[1]:"normal";
 setenv("RECOMP_GPU_RASTER","1",1);setenv("RECOMP_GPU_ASYNC","0",1);setenv("RECOMP_GPU_PROFILE","0",1);
 if(!strcmp(mode,"disabled"))setenv("RECOMP_GPU_QUAD_SAMPLING","0",1);
 if(!strcmp(mode,"array"))setenv("RECOMP_GPU_FIXED","0",1);
 if(!strcmp(mode,"vector-off"))setenv("RECOMP_GPU_VECTOR_COMBINERS","0",1);
 quad_compile_fail=!strcmp(mode,"compile");quad_create_fail=!strcmp(mode,"create");quad_color_bad=!strcmp(mode,"color");quad_depth_bad=!strcmp(mode,"depth");quad_count_bad=!strcmp(mode,"count");quad_material_bad=!strcmp(mode,"material");quad_map_fail=!strcmp(mode,"map");quad_simple_map_fail=!strcmp(mode,"simple-map");
 native_simple_compile_fail=!strcmp(mode,"simple-compile");native_simple_create_fail=!strcmp(mode,"simple-create");native_simple_corrupt=!strcmp(mode,"simple-bad");native_corrupt=!strcmp(mode,"both-bad");
 native_supported=1;coverage_tier=3;int ready=nv_gpu_available();
 if(quad_map_fail || quad_simple_map_fail){assert(!ready && s_startup==3 && !g.active && !g.host_color && !g.host_depth);puts("PASS: real readback failure rejects readiness");return 0;}
 assert(ready);
 if(native_corrupt || native_simple_compile_fail || native_simple_create_fail || native_simple_corrupt){assert(!g.native);puts("PASS: rejected native variants retain independently validated legacy compute");return 0;}
 int optimized=!strcmp(mode,"normal") || !strcmp(mode,"warm") || !strcmp(mode,"array") || !strcmp(mode,"vector-off");
 assert(g.native && g.quad_sampling==optimized && general_quad==optimized && simple_quad==optimized && g.simple_pixel_shader);
 if(!strcmp(mode,"disabled"))assert(!quad_compiles);
 if(!strcmp(mode,"warm"))assert(!compiler_calls);
 unsigned saved=g.quad_sampling;g.quad_sampling=1;unsigned a=gpu_native_variant(0),sa=gpu_native_variant(2);g.quad_sampling=0;unsigned b=gpu_native_variant(0),sb=gpu_native_variant(2);g.quad_sampling=saved;
 assert(a!=b && sa!=sb && (a^b)==32 && (sa^sb)==32);
 assert(gpu_compile_key(nv_gpu_rov_source,sizeof nv_gpu_rov_source-1,"pixel_main","ps_5_0",a)!=gpu_compile_key(nv_gpu_rov_source,sizeof nv_gpu_rov_source-1,"pixel_main","ps_5_0",b));
 uint32_t color[256]={0};float z[256];for(unsigned i=0;i<256;i++)z[i]=100;
 NvGpuState state={0};state.width=state.height=16;state.pitch=64;state.bpp=4;state.format=0x128;state.color_mask=0x01010101;
 state.depth_available=state.depth_test=state.depth_mask=state.zpass_enable=1;state.depth_func=0x203;
 NvGpuTri tri=triangle(16);NvGpuTextureSource tex[4]={0};NvGpuCount count;
 assert(nv_gpu_draw(&state,&tri,1,(uint8_t*)color,z,16,tex)==1 && !color[5*16+5]);
 assert(nv_gpu_sync(&count)==1 && color[5*16+5]==0xFFFFFFFF && z[5*16+5]==10 && count.zpass);
 nv_gpu_report();printf("PASS: %s; native general/simple sampling variants and exact delayed color/depth/visibility\n",mode);
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-quad-gate-') as folder:
 p=Path(folder)
 for name in ['windows.h','d3d11.h','d3d11_3.h','d3dcompiler.h','dxgi.h']:(p/name).write_text('/* strict fixture API */\n')
 (p/'test.c').write_text(api+'ptrdiff_t xbox_GetMemoryOffset(void){return 0;}\n#include "'+str(k/'nv2a_gpu_raster.c')+'"\n'+fixture+main)
 subprocess.run(['gcc','-std=gnu11','-O1','-ffp-contract=off','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-Werror=implicit-function-declaration','-Werror=incompatible-pointer-types','-I'+str(p),'-I'+str(root/'tools'),'-I'+str(k),str(p/'test.c'),str(k/'nv2a_gpu_tiles.c'),str(k/'nv2a_vsh_interp.c'),'-pthread','-lm','-o',str(p/'test')],check=True)
 modes=['normal','warm','disabled','array','vector-off','compile','create','color','depth','count','material','map','both-bad','simple-compile','simple-create','simple-bad','simple-map']
 for mode in modes:
  print('RUN quad gate '+mode,flush=True);cwd=p/('normal' if mode=='warm' else mode);cwd.mkdir(exist_ok=True);subprocess.run([str(p/'test'),mode],cwd=cwd,check=True,timeout=120)
 print('PASS: all 17 native shared-address gate/cache/fallback modes')
