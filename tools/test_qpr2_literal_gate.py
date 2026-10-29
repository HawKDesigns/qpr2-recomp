"""Dedicated program admission/fallback, ordered mixed draws and cache guards.
Actual production bodies run through strict API simulation, not hardware HLSL.
"""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1];k=root/'third_party/xboxrecomp/src/kernel';path=root/'tools/test_qpr2_gpu_device.py'
s=path.read_text();ns={'__file__':str(path)};exec(compile(s[:s.index('with tempfile.TemporaryDirectory(')],str(path),'exec'),ns)
api=ns['api'];fixture=ns['fixture'].split('int main(int argc,char**argv)')[0]
main=r'''
static int literal_test_decode(void *opaque,uint32_t face,uint32_t x,uint32_t y,uint32_t *out){(void)face;*out=((uint32_t*)opaque)[y*4+x];return 1;}
int main(int argc,char**argv){
 const char *mode=argc>1?argv[1]:"normal";
 setenv("RECOMP_GPU_COMPACT_MATERIALS","0",1);
 setenv("RECOMP_GPU_RASTER","1",1);setenv("RECOMP_GPU_ASYNC","0",1);setenv("RECOMP_GPU_PROFILE","0",1);
 if(!strcmp(mode,"disabled"))setenv("RECOMP_GPU_LITERAL_MATERIALS","0",1);
 if(!strcmp(mode,"no-batch"))setenv("RECOMP_GPU_BATCH_UPLOADS","0",1);
 if(!strcmp(mode,"array"))setenv("RECOMP_GPU_FIXED","0",1);
 if(!strcmp(mode,"vector-off"))setenv("RECOMP_GPU_VECTOR_COMBINERS","0",1);
 if(!strcmp(mode,"usage-off"))setenv("RECOMP_GPU_STAGE_LIVENESS","0",1);
 if(!strcmp(mode,"seed-off"))setenv("RECOMP_GPU_DEPTH_SEED","0",1);
 if(!strcmp(mode,"empty-off"))setenv("RECOMP_GPU_EMPTY_REJECT","0",1);
 if(!strcmp(mode,"diagnostic"))setenv("RECOMP_GPU_DIAGNOSTIC_COUNTERS","1",1);
 if(!strncmp(mode,"compile",7))literal_compile_fail=atoi(mode+7);
 if(!strncmp(mode,"create",6))literal_create_fail=atoi(mode+6);
 if(!strncmp(mode,"color",5))literal_color_bad=atoi(mode+5);
 if(!strncmp(mode,"depth",5))literal_depth_bad=atoi(mode+5);
 if(!strncmp(mode,"count",5))literal_count_bad=atoi(mode+5);
 if(!strncmp(mode,"map",3))literal_map_fail=atoi(mode+3);
 if(!strcmp(mode,"variant"))literal_variant_bad=1;
 native_supported=strcmp(mode,"compute")!=0;coverage_tier=3;int ready=nv_gpu_available();
 if(literal_map_fail){assert(!ready && s_startup==3 && !g.active && !g.host_color && !g.host_depth);puts("PASS: actual literal Map failure rejects readiness");return 0;}
 assert(ready && !g.literal.compiling && !g.literal.testing);
 unsigned expected=7,failed=0;
 if(!g.native || !g.native_batch || !g.literal.enabled)expected=0;
 else {
  if(literal_compile_fail==99 || literal_create_fail==99)failed=7;
  else for(unsigned id=1;id<=3;id++)if(literal_compile_fail==(int)id || literal_create_fail==(int)id || literal_color_bad==(int)id || literal_depth_bad==(int)id || literal_count_bad==(int)id || (literal_variant_bad && id==2))failed|=1u<<(id-1);
  expected&=~failed;
 }
 assert(g.literal.ready==expected && g.literal.rejected==failed);
 if(!strcmp(mode,"warm"))assert(!compiler_calls);
 if(!strcmp(mode,"disabled") || !strcmp(mode,"no-batch") || !strcmp(mode,"compute"))for(unsigned id=0;id<3;id++)assert(!g.literal.shader[id]);
 for(unsigned id=1;id<=3;id++)if(expected&(1u<<(id-1))){
  uint64_t key=gpu_compile_key(nv_gpu_rov_source,sizeof nv_gpu_rov_source-1,"pixel_main","ps_5_0",gpu_native_variant(g.array_registers?1u:0u)|(id<<12));
  char path[128];snprintf(path,sizeof path,"shader-cache/qpr2-rov-literal%u-%016llx.dxbc",id,(unsigned long long)key);
  FILE*f=fopen(path,"rb");uint64_t header[4];assert(f && fread(header,sizeof header,1,f)==1 && header[1]==key && header[2]==id+3);fclose(f);
  ID3D11PixelShader *general=g.pixel_shader,*special=g.literal.shader[id-1];unsigned calls=compiler_calls;
  g.literal.compiling=id;assert(gpu_rov_code(0,0,path,key));g.literal.compiling=0;
  assert(compiler_calls==calls && g.pixel_shader==general && g.literal.shader[id-1]==special);
 }
 memset(literal_draws,0,sizeof literal_draws);g.literal.draws=g.literal.triangles=g.literal.generic_draws=0;
 uint32_t color[256]={0},reference[256]={0},texels[16];float depth[256],rz[256];
 for(unsigned i=0;i<256;i++)depth[i]=rz[i]=100;
 for(unsigned i=0;i<16;i++)texels[i]=0x70000000u+i*0x09051317u;
 NvGpuState state={0};state.width=state.height=16;state.pitch=64;state.bpp=4;state.format=0x128;state.color_mask=0x01010101;
 state.depth_available=state.depth_test=state.depth_mask=state.zpass_enable=1;state.depth_func=0x203;
 state.blend_enable=1;state.blend_sfactor=0x302;state.blend_dfactor=0x303;state.blend_equation=0x8006;
 NvGpuTextureSource sources[4]={0};
 for(unsigned st=0;st<2;st++){
  state.tex[st].valid=state.tex[st].normalized=1;state.tex[st].width=state.tex[st].height=4;state.tex[st].addr_u=state.tex[st].addr_v=1;state.tex[st].format=0x12;state.tex[st].filter=0x02020000;
  sources[st].raw=(const uint8_t*)texels;sources[st].bytes=sizeof texels;sources[st].opaque=texels;sources[st].decode=literal_test_decode;
 }
 NvGpuTri triangles[2]={triangle(16),triangle(16)};
 for(unsigned t=0;t<2;t++)for(unsigned v=0;v<3;v++){
  triangles[t].use_rc=1;triangles[t].vfog[v]=.3f+.2f*v;
  for(unsigned c=0;c<4;c++){triangles[t].v[v].d0[c]=.25f+.13f*(v+c);triangles[t].v[v].d1[c]=.1f+.17f*c;triangles[t].fogc[c]=.125f;
   for(unsigned st=0;st<4;st++)triangles[t].stc[v][st][c]=c==3?1:(c==v?1.25f:-.125f)+(float)st*.25f;}
 }
 NvGpuCount ref={0},got;
 /* Unknown, all three literal shapes, another six-stage final, and the
  * existing simple shader share the same ordered submission sequence. */
 unsigned sequence[8]={1,2,3,1,2,3,2,0};
 for(unsigned draw=0;draw<8;draw++){
  if(sequence[draw])nv_gpu_literal_fixture(&state.rc,sequence[draw],draw);
  else {memset(&state.rc,0,sizeof state.rc);state.rc.control=0x11101;state.rc.stage_program=1;state.rc.color_icw[0]=0xC8C40000;state.rc.color_ocw[0]=0x100C0;state.rc.alpha_icw[0]=0xD8301010;state.rc.alpha_ocw[0]=0xC0;state.rc.final0=0x130C0300;state.rc.final1=0x1C80;}
  if(!draw){state.rc.color_icw[0]^=1;assert(!nv_gpu_literal_program(&state.rc));}
  for(unsigned stage=0;stage<8;stage++){state.rc.factor0[stage]=0x10203040u+stage*0x01030507u+draw;state.rc.factor1[stage]=0xB8A89888u-stage*0x07050301u;}
  assert(nv_gpu_draw(&state,triangles,2,(uint8_t*)color,depth,16,sources)==1 && !color[5*16+5]);
  NvGpuState uploaded=g.queued_state[g.state_count-1];
  for(unsigned t=0;t<2;t++)for(int y=0;y<16;y++)for(int x=0;x<16;x++){
   unsigned at=y*16+x;NvGpuPixel p=gpu_pixel(uploaded,triangles[t],x,y,reference[at],rz[at]);reference[at]=p.color;rz[at]=p.depth;ref.depth_fail+=p.depth_fail;ref.pixels+=p.pixels;ref.zpass+=p.zpass;
  }
 }
 assert(nv_gpu_sync(&got)==1);gpu_reference_counts(&ref);
 assert(!memcmp(color,reference,sizeof color) && !memcmp(depth,rz,sizeof depth) && got.zpass==ref.zpass && got.pixels==ref.pixels && got.depth_fail==ref.depth_fail);
 for(unsigned id=0;id<3;id++)if(expected&(1u<<id))assert(literal_draws[id]>0);else assert(!literal_draws[id]);
 if(expected)assert(g.literal.draws && g.literal.triangles && g.literal.generic_draws);else assert(!g.literal.draws && !g.literal.triangles);
 assert(!bound_material_srv && !bound_seed_srv[0] && !bound_seed_srv[1]);
 nv_gpu_report();printf("PASS: %s; independent family gates, exact cache stages, mixed program/state order, unknown/simple fallback and delayed pixel/depth/visibility\n",mode);
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-literal-gate-') as folder:
 p=Path(folder)
 for name in ['windows.h','d3d11.h','d3d11_3.h','d3dcompiler.h','dxgi.h']:(p/name).write_text('/* strict fixture API */\n')
 (p/'test.c').write_text(api+'ptrdiff_t xbox_GetMemoryOffset(void){return 0;}\n#include "'+str(k/'nv2a_gpu_raster.c')+'"\n'+fixture+main)
 subprocess.run(['gcc','-std=gnu11','-O1','-ffp-contract=off','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-Werror=implicit-function-declaration','-Werror=incompatible-pointer-types','-I'+str(p),'-I'+str(root/'tools'),'-I'+str(k),str(p/'test.c'),str(k/'nv2a_gpu_tiles.c'),str(k/'nv2a_vsh_interp.c'),'-pthread','-lm','-o',str(p/'test')],check=True)
 modes=['normal','warm','disabled','no-batch','array','vector-off','usage-off','seed-off','empty-off','diagnostic','variant','compile99','create99','compute']+[kind+str(id) for kind in ['compile','create','color','depth','count','map'] for id in range(1,4)]
 for mode in modes:
  print('RUN literal gate '+mode,flush=True);cwd=p/('normal' if mode=='warm' else mode);cwd.mkdir(exist_ok=True);subprocess.run([str(p/'test'),mode],cwd=cwd,check=True,timeout=180)
 print('PASS: all '+str(len(modes))+' literal shader cache/failure/mixed-publication modes')
