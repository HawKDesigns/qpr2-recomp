"""Native empty-transaction rejection, candidate keys, fallback and publication.
API simulation does not compile or execute Windows HLSL.
"""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1];k=root/'third_party/xboxrecomp/src/kernel';path=root/'tools/test_qpr2_gpu_device.py'
s=path.read_text();ns={'__file__':str(path)};exec(compile(s[:s.index('with tempfile.TemporaryDirectory(')],str(path),'exec'),ns)
api=ns['api'];fixture=ns['fixture'].split('int main(int argc,char**argv)')[0]
api=api.replace('static int native_batch_corrupt,index_create_fail;', 'static int native_batch_corrupt,index_create_fail;\nstatic int reject_compile_fail,reject_create_fail,reject_color_bad,reject_depth_bad,reject_count_bad,reject_material_bad,reject_map_fail,reject_simple_map_fail,general_reject,simple_reject;\nstatic unsigned reject_compiles;\nstatic int reject_only_simple,reject_simple_compile,reject_simple_create;')
a=' if(failed_compile ||';b=''' int reject=0;for(D3D_SHADER_MACRO*m=b;m && m->Name;m++)if(!strcmp(m->Name,"NV_GPU_EMPTY_REJECT"))reject=atoi(m->Definition);
 if(reject){assert(!strcmp(e,"ps_5_0"));++reject_compiles;}
 if((reject_compile_fail && reject) || (reject_simple_compile && reject && simple_macro) || failed_compile ||''';assert a in api;api=api.replace(a,b,1)
a='*out=blob;*error=NULL;return 0;';b='''if(reject){size_t bytes=blob->bytes;char*data=malloc(bytes+16);assert(data);memcpy(data,"//empty-reject!\\n",16);memcpy(data+16,blob->data,bytes);if(blob->owned)free((void*)blob->data);blob->data=data;blob->bytes=bytes+16;blob->owned=1;}
 *out=blob;*error=NULL;return 0;''';assert a in api;api=api.replace(a,b,1)
a='int simple=n>16 && !memcmp(p,"//native-simple\\n",16);';b='''int reject=n>16 && !memcmp(p,"//empty-reject!\\n",16);if(reject && reject_create_fail)return -1;if(reject){p=(const char*)p+16;n-=16;}int simple=n>16 && !memcmp(p,"//native-simple\\n",16);if(simple && reject && reject_simple_create)return -1;if(simple)simple_reject=reject;else general_reject=reject;''';assert a in api;api=api.replace(a,b,1)
a='   uint32_t depth_bits;memcpy(&depth_bits,&p.depth,4);';b='''   if((!reject_only_simple && bound_pixel==&pixel_shader && general_reject) || (bound_pixel==&simple_pixel_shader && simple_reject)){
    if(reject_color_bad)p.color^=0x40;
    if(reject_depth_bad)p.depth+=1;
    if(reject_count_bad)p.zpass=0;
    if(reject_material_bad && state->width==256)p.color^=0x40;
    if(reject_map_fail || (reject_simple_map_fail && bound_pixel==&simple_pixel_shader))fail_next_map=1;
   }
'''+a;assert a in fixture;fixture=fixture.replace(a,b,1)
main=r'''
int main(int argc,char **argv){
 const char *mode=argc>1?argv[1]:"normal";
 setenv("RECOMP_GPU_RASTER","1",1);setenv("RECOMP_GPU_ASYNC","0",1);setenv("RECOMP_GPU_PROFILE","0",1);
 if(!strcmp(mode,"disabled"))setenv("RECOMP_GPU_EMPTY_REJECT","0",1);
 if(!strcmp(mode,"array"))setenv("RECOMP_GPU_FIXED","0",1);
 if(!strcmp(mode,"vector-off"))setenv("RECOMP_GPU_VECTOR_COMBINERS","0",1);
 if(!strcmp(mode,"seed-off"))setenv("RECOMP_GPU_DEPTH_SEED","0",1);
 if(!strcmp(mode,"diagnostic"))setenv("RECOMP_GPU_DIAGNOSTIC_COUNTERS","1",1);
 reject_compile_fail=!strcmp(mode,"compile");reject_create_fail=!strcmp(mode,"create");reject_color_bad=!strcmp(mode,"color");reject_depth_bad=!strcmp(mode,"depth");reject_count_bad=!strcmp(mode,"count");reject_material_bad=!strcmp(mode,"material");reject_map_fail=!strcmp(mode,"map");reject_simple_map_fail=!strcmp(mode,"simple-map");
 native_simple_compile_fail=!strcmp(mode,"simple-compile");native_simple_create_fail=!strcmp(mode,"simple-create");native_simple_corrupt=!strcmp(mode,"simple-bad");native_corrupt=!strcmp(mode,"both-bad");
 reject_only_simple=!strncmp(mode,"candidate-simple-",17);
 if(reject_only_simple){const char *reason=mode+17;reject_color_bad=!strcmp(reason,"color");reject_depth_bad=!strcmp(reason,"depth");reject_count_bad=!strcmp(reason,"count");reject_material_bad=!strcmp(reason,"material");reject_simple_compile=!strcmp(reason,"compile");reject_simple_create=!strcmp(reason,"create");}
 native_supported=1;coverage_tier=3;int ready=nv_gpu_available();
 if(reject_map_fail || reject_simple_map_fail){assert(!ready && s_startup==3 && !g.active && !g.host_color && !g.host_depth);puts("PASS: real readback failure rejects readiness");return 0;}
 assert(ready);
 if(native_corrupt || native_simple_compile_fail || native_simple_create_fail || native_simple_corrupt){assert(!g.native);puts("PASS: rejected native variants retain independently validated legacy compute");return 0;}
 int optimized=!strcmp(mode,"normal") || !strcmp(mode,"warm") || !strcmp(mode,"array") || !strcmp(mode,"vector-off") || !strcmp(mode,"seed-off") || !strcmp(mode,"diagnostic");
 assert(g.native && g.empty_reject==optimized && general_reject==optimized && simple_reject==optimized && g.simple_pixel_shader);
 if(!strcmp(mode,"disabled"))assert(!reject_compiles);
 if(!strcmp(mode,"warm"))assert(!compiler_calls);
 unsigned saved=g.empty_reject;g.empty_reject=1;unsigned a=gpu_native_variant(0),sa=gpu_native_variant(2);g.empty_reject=0;unsigned b=gpu_native_variant(0),sb=gpu_native_variant(2);g.empty_reject=saved;
 assert(a!=b && sa!=sb && (a^b)==128 && (sa^sb)==128);
 assert(gpu_compile_key(nv_gpu_rov_source,sizeof nv_gpu_rov_source-1,"pixel_main","ps_5_0",a)!=gpu_compile_key(nv_gpu_rov_source,sizeof nv_gpu_rov_source-1,"pixel_main","ps_5_0",b));
 uint32_t color[256]={0};float z[256];for(unsigned i=0;i<256;i++)z[i]=100;
 NvGpuState state={0};state.width=state.height=16;state.pitch=64;state.bpp=4;state.format=0x128;state.color_mask=0x01010101;
 state.depth_available=state.depth_test=state.depth_mask=state.zpass_enable=1;state.depth_func=0x203;
 NvGpuTri tri=triangle(16);NvGpuTextureSource tex[4]={0};NvGpuCount count;
 mock_reject_before_source=mock_reject_after_source=mock_ordered_transactions=0;
 assert(nv_gpu_draw(&state,&tri,1,(uint8_t*)color,z,16,tex)==1 && !color[5*16+5]);
 assert(nv_gpu_sync(&count)==1 && color[5*16+5]==0xFFFFFFFF && z[5*16+5]==10 && count.zpass);
 assert(mock_ordered_transactions>0);
 if(optimized && !g.native_diagnostics)assert(mock_reject_before_source>0);else assert(!mock_reject_before_source);
 state.alpha_test=1;state.alpha_func=0x200;
 assert(nv_gpu_draw(&state,&tri,1,(uint8_t*)color,z,16,tex)==1 && nv_gpu_sync(&count)==1 && !count.zpass);
 assert(color[5*16+5]==0xFFFFFFFF && z[5*16+5]==10);
 if(optimized && !g.native_diagnostics)assert(mock_reject_after_source>0);else assert(!mock_reject_after_source);
 nv_gpu_report();printf("PASS: %s; native general/simple rejection variants and exact delayed color/depth/visibility\n",mode);
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-reject-gate-') as folder:
 p=Path(folder)
 for name in ['windows.h','d3d11.h','d3d11_3.h','d3dcompiler.h','dxgi.h']:(p/name).write_text('/* strict fixture API */\n')
 (p/'test.c').write_text(api+'ptrdiff_t xbox_GetMemoryOffset(void){return 0;}\n#include "'+str(k/'nv2a_gpu_raster.c')+'"\n'+fixture+main)
 subprocess.run(['gcc','-std=gnu11','-O1','-ffp-contract=off','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-Werror=implicit-function-declaration','-Werror=incompatible-pointer-types','-I'+str(p),'-I'+str(root/'tools'),'-I'+str(k),str(p/'test.c'),str(k/'nv2a_gpu_tiles.c'),str(k/'nv2a_vsh_interp.c'),'-pthread','-lm','-o',str(p/'test')],check=True)
 modes=['normal','warm','disabled','array','vector-off','compile','create','color','depth','count','material','map','both-bad','simple-compile','simple-create','simple-bad','simple-map','seed-off','diagnostic','candidate-simple-compile','candidate-simple-create','candidate-simple-color','candidate-simple-depth','candidate-simple-count','candidate-simple-material']
 for mode in modes:
  print('RUN reject gate '+mode,flush=True);cwd=p/('normal' if mode=='warm' else mode);cwd.mkdir(exist_ok=True);subprocess.run([str(p/'test'),mode],cwd=cwd,check=True,timeout=120)
 print('PASS: all 25 native empty-transaction gate/cache/fallback modes')
