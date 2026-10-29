"""Native empty-transaction usageion, candidate keys, fallback and publication.
API simulation does not compile or execute Windows HLSL.
"""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1];k=root/'third_party/xboxrecomp/src/kernel';path=root/'tools/test_qpr2_gpu_device.py'
s=path.read_text();ns={'__file__':str(path)};exec(compile(s[:s.index('with tempfile.TemporaryDirectory(')],str(path),'exec'),ns)
api=ns['api'];fixture=ns['fixture'].split('int main(int argc,char**argv)')[0]
api=api.replace('static int native_batch_corrupt,index_create_fail;', 'static int native_batch_corrupt,index_create_fail;\nstatic int usage_compile_fail,usage_create_fail,usage_color_bad,usage_depth_bad,usage_count_bad,usage_material_bad,usage_map_fail,usage_simple_map_fail,general_usage,simple_usage;\nstatic unsigned usage_compiles;\nstatic int mask_alloc_fail;\nstatic int usage_only_simple,usage_simple_compile,usage_simple_create;')
a=' if(failed_compile ||';b=''' int usage=0;for(D3D_SHADER_MACRO*m=b;m && m->Name;m++)if(!strcmp(m->Name,"NV_GPU_STAGE_LIVENESS"))usage=atoi(m->Definition);
 if(usage){assert(!strcmp(e,"ps_5_0"));++usage_compiles;}
 if((usage_compile_fail && usage) || (usage_simple_compile && usage && simple_macro) || failed_compile ||''';assert a in api;api=api.replace(a,b,1)
a='*out=blob;*error=NULL;return 0;';b='''if(usage){size_t bytes=blob->bytes;char*data=malloc(bytes+16);assert(data);memcpy(data,"//stage-usage!!\\n",16);memcpy(data+16,blob->data,bytes);if(blob->owned)free((void*)blob->data);blob->data=data;blob->bytes=bytes+16;blob->owned=1;}
 *out=blob;*error=NULL;return 0;''';assert a in api;api=api.replace(a,b,1)
a='int simple=n>16 && !memcmp(p,"//native-simple\\n",16);';b='''int usage=n>16 && !memcmp(p,"//stage-usage!!\\n",16);if(usage && usage_create_fail)return -1;if(usage){p=(const char*)p+16;n-=16;}int simple=n>16 && !memcmp(p,"//native-simple\\n",16);if(simple && usage && usage_simple_create)return -1;if(simple)simple_usage=usage;else general_usage=usage;''';assert a in api;api=api.replace(a,b,1)
a='   uint32_t depth_bits;memcpy(&depth_bits,&p.depth,4);';b='''   if((!usage_only_simple && bound_pixel==&pixel_shader && general_usage) || (bound_pixel==&simple_pixel_shader && simple_usage)){
    if(usage_color_bad)p.color^=0x40;
    if(usage_depth_bad)p.depth+=1;
    if(usage_count_bad)p.zpass=0;
    if(usage_material_bad && state->width==256)p.color^=0x40;
    if(usage_map_fail || (usage_simple_map_fail && bound_pixel==&simple_pixel_shader))fail_next_map=1;
   }
'''+a;assert a in fixture;fixture=fixture.replace(a,b,1)
a=' (void)d;++buffer_creates;';assert a in api;api=api.replace(a,a+'if(mask_alloc_fail && s->Usage==D3D11_USAGE_DEFAULT && s->BindFlags==D3D11_BIND_SHADER_RESOURCE && s->StructureByteStride==4)return -1;',1)
main=r'''
int main(int argc,char **argv){
 const char *mode=argc>1?argv[1]:"normal";
 setenv("RECOMP_GPU_RASTER","1",1);setenv("RECOMP_GPU_ASYNC","0",1);setenv("RECOMP_GPU_PROFILE","0",1);
 if(!strcmp(mode,"disabled"))setenv("RECOMP_GPU_STAGE_LIVENESS","0",1);
 if(!strcmp(mode,"array"))setenv("RECOMP_GPU_FIXED","0",1);
 if(!strcmp(mode,"vector-off"))setenv("RECOMP_GPU_VECTOR_COMBINERS","0",1);
 if(!strcmp(mode,"seed-off"))setenv("RECOMP_GPU_DEPTH_SEED","0",1);
 if(!strcmp(mode,"diagnostic"))setenv("RECOMP_GPU_DIAGNOSTIC_COUNTERS","1",1);
 usage_compile_fail=!strcmp(mode,"compile");usage_create_fail=!strcmp(mode,"create");usage_color_bad=!strcmp(mode,"color");usage_depth_bad=!strcmp(mode,"depth");usage_count_bad=!strcmp(mode,"count");usage_material_bad=!strcmp(mode,"material");usage_map_fail=!strcmp(mode,"map");usage_simple_map_fail=!strcmp(mode,"simple-map");
 native_simple_compile_fail=!strcmp(mode,"simple-compile");native_simple_create_fail=!strcmp(mode,"simple-create");native_simple_corrupt=!strcmp(mode,"simple-bad");native_corrupt=!strcmp(mode,"both-bad");
 usage_only_simple=!strncmp(mode,"candidate-simple-",17);
 if(usage_only_simple){const char *reason=mode+17;usage_color_bad=!strcmp(reason,"color");usage_depth_bad=!strcmp(reason,"depth");usage_count_bad=!strcmp(reason,"count");usage_material_bad=!strcmp(reason,"material");usage_simple_compile=!strcmp(reason,"compile");usage_simple_create=!strcmp(reason,"create");}
 native_supported=1;coverage_tier=3;int ready=nv_gpu_available();
 if(usage_map_fail || usage_simple_map_fail){assert(!ready && s_startup==3 && !g.active && !g.host_color && !g.host_depth);puts("PASS: real stage candidate Map failure rejects readiness");return 0;}
 assert(ready);
 if(native_corrupt || native_simple_compile_fail || native_simple_create_fail || native_simple_corrupt){assert(!g.native);puts("PASS: rejected native variants retain independently validated compute");return 0;}
 int optimized=!strcmp(mode,"normal") || !strcmp(mode,"warm") || !strcmp(mode,"array") || !strcmp(mode,"vector-off") || !strcmp(mode,"seed-off") || !strcmp(mode,"diagnostic") || !strcmp(mode,"alloc");
 assert(g.native && g.material.enabled==optimized && general_usage==optimized && simple_usage==optimized && g.simple_pixel_shader);
 if(!strcmp(mode,"disabled"))assert(!usage_compiles);
 if(!strcmp(mode,"warm"))assert(!compiler_calls);
 int saved=g.material.enabled;g.material.enabled=1;unsigned a=gpu_native_variant(0),sa=gpu_native_variant(2);g.material.enabled=0;unsigned b=gpu_native_variant(0),sb=gpu_native_variant(2);g.material.enabled=saved;
 assert((a^b)==256 && (sa^sb)==256);
 if(!strcmp(mode,"alloc")){gpu_buffer_release(&g.material.stage_masks);mask_alloc_fail=1;}
 g.material.batches=g.material.mask_batches=g.material.fallback_batches=0;mock_masked_sources=0;
 uint32_t color[256]={0};float z[256];for(unsigned i=0;i<256;i++)z[i]=100;
 NvGpuState state={0};state.width=state.height=16;state.pitch=64;state.bpp=4;state.format=0x128;state.color_mask=0x01010101;
 state.depth_available=state.depth_test=state.depth_mask=state.zpass_enable=1;state.depth_func=0x203;
 state.rc.control=1;state.rc.stage_program=1|(1u<<5)|(5u<<10)|(1u<<15);
 state.rc.color_icw[0]=0xC8C40000;state.rc.color_ocw[0]=0x100C0;state.rc.alpha_icw[0]=0xD8D41010;state.rc.alpha_ocw[0]=0xC0;state.rc.final0=0xC;state.rc.final1=0x1C80;
 NvGpuTri tri=triangle(16);tri.use_rc=1;
 for(unsigned v=0;v<3;v++)for(unsigned st=0;st<4;st++)tri.stc[v][st][3]=1;
 NvGpuTextureSource tex[4]={0};NvGpuCount count;
 assert(nv_gpu_draw(&state,&tri,1,(uint8_t*)color,z,16,tex)==1 && !color[5*16+5]);
 assert(nv_gpu_texture_reads(&state.rc)==1 && nv_gpu_texture_clip(&state.rc)==4);
 /* A second state reads T3 passthrough and must receive its own mask. */
 state.rc.stage_program=(state.rc.stage_program&~(31u<<15))|(4u<<15);state.rc.color_icw[0]=0xCBC40000;
 for(unsigned v=0;v<3;v++)for(unsigned c=0;c<3;c++)tri.stc[v][3][c]=.25f;
 assert(nv_gpu_texture_reads(&state.rc)==9);
 assert(nv_gpu_draw(&state,&tri,1,(uint8_t*)color,z,16,tex)==1 && !color[5*16+5]);
 /* CLIP is unreferenced but still rejects the third draw. */
 state.clip_plane_mode=1u<<8;
 assert(nv_gpu_draw(&state,&tri,1,(uint8_t*)color,z,16,tex)==1 && !color[5*16+5]);
 assert(nv_gpu_sync(&count)==1 && color[5*16+5]==0xFF808080 && z[5*16+5]==10 && count.zpass);
 assert(!bound_material_srv && g.material.masks[0]==5 && g.material.masks[1]==13 && g.material.masks[2]==13);
 if(optimized){assert(g.material.batches==1 && mock_masked_sources>0);if(!strcmp(mode,"alloc"))assert(g.material.fallback_batches==1 && !g.material.mask_batches);else assert(g.material.mask_batches==1 && !g.material.fallback_batches);}
 else assert(!g.material.batches && !g.material.mask_batches && !mock_masked_sources);
 /* Color-disabled depth/visibility work needs no material mask upload. */
 uint64_t uploads=g.material.mask_batches;state.clip_plane_mode=0;state.color_mask=0;
 for(unsigned v=0;v<3;v++)tri.v[v].pos[2]=9;
 assert(nv_gpu_draw(&state,&tri,1,(uint8_t*)color,z,16,tex)==1 && nv_gpu_sync(&count)==1);
 assert(color[5*16+5]==0xFF808080 && z[5*16+5]==9 && count.zpass && g.material.mask_batches==uploads);
 /* Alpha-only work must still evaluate the source and preserve rejection. */
 state.alpha_test=1;state.alpha_func=0x200;
 for(unsigned v=0;v<3;v++)tri.v[v].pos[2]=8;
 assert(nv_gpu_draw(&state,&tri,1,(uint8_t*)color,z,16,tex)==1 && nv_gpu_sync(&count)==1);
 assert(color[5*16+5]==0xFF808080 && z[5*16+5]==9 && !count.zpass);
 if(optimized && strcmp(mode,"alloc"))assert(g.material.mask_batches==uploads+1);
 nv_gpu_report();printf("PASS: %s; state-index masks, T0 initial alpha, unused active textures, CLIP, all-live fallback and exact delayed output\n",mode);
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-material-usage-gate-') as folder:
 p=Path(folder)
 for name in ['windows.h','d3d11.h','d3d11_3.h','d3dcompiler.h','dxgi.h']:(p/name).write_text('/* strict fixture API */\n')
 (p/'test.c').write_text(api+'ptrdiff_t xbox_GetMemoryOffset(void){return 0;}\n#include "'+str(k/'nv2a_gpu_raster.c')+'"\n'+fixture+main)
 subprocess.run(['gcc','-std=gnu11','-O1','-ffp-contract=off','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-Werror=implicit-function-declaration','-Werror=incompatible-pointer-types','-I'+str(p),'-I'+str(root/'tools'),'-I'+str(k),str(p/'test.c'),str(k/'nv2a_gpu_tiles.c'),str(k/'nv2a_vsh_interp.c'),'-pthread','-lm','-o',str(p/'test')],check=True)
 modes=['normal','warm','disabled','array','vector-off','seed-off','diagnostic','compile','create','color','depth','count','material','map','simple-map','both-bad','simple-compile','simple-create','simple-bad','candidate-simple-compile','candidate-simple-create','candidate-simple-color','candidate-simple-depth','candidate-simple-count','candidate-simple-material','alloc']
 for mode in modes:
  print('RUN material usage gate '+mode,flush=True);cwd=p/('normal' if mode=='warm' else mode);cwd.mkdir(exist_ok=True);subprocess.run([str(p/'test'),mode],cwd=cwd,check=True,timeout=120)
 print('PASS: all 26 material usage cache/failure/publication modes')
