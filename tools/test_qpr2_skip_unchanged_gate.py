"""Atomic native store-policy admission, cache keys and exact publication.

Strict API simulation checks host fallback, not Windows HLSL or GPU ordering.
"""
from pathlib import Path
import subprocess,tempfile
r=Path(__file__).resolve().parents[1];k=r/'third_party/xboxrecomp/src/kernel';p=r/'tools/test_qpr2_gpu_device.py'
s=p.read_text();ns={'__file__':str(p)};exec(compile(s[:s.index('with tempfile.TemporaryDirectory(')],str(p),'exec'),ns)
api=ns['api'];fixture=ns['fixture'].split('int main(int argc,char**argv)')[0]
a='static int native_batch_corrupt,index_create_fail;'
b=a+'''
static int store_compile_bad=-1,store_create_bad=-1,store_color_bad=-1,store_depth_bad=-1,store_count_bad=-1,store_material_bad,store_map_bad,store_noop_bad,store_drop_write;
static unsigned store_compiles[5],store_candidate_draws[5];static uint64_t store_writes,store_reads,store_read_zpass;
''';assert a in api;api=api.replace(a,b,1)
a=' if(failed_compile ||'
b=''' int store=0;for(D3D_SHADER_MACRO*m=b;m && m->Name;m++)if(!strcmp(m->Name,"NV_GPU_SKIP_UNCHANGED"))store=atoi(m->Definition);
 int store_id=literal_macro?literal_macro+1:simple_macro?1:0;
 if(store){assert(!strcmp(e,"ps_5_0"));++store_compiles[store_id];}
 if((store && store_compile_bad==store_id) || failed_compile ||''';assert a in api;api=api.replace(a,b,1)
a='*out=blob;*error=NULL;return 0;'
b='''if(store){size_t bytes=blob->bytes;char*data=malloc(bytes+16);assert(data);memcpy(data,"//skip-store-v1\\n",16);memcpy(data+16,blob->data,bytes);if(blob->owned)free((void*)blob->data);blob->data=data;blob->bytes=bytes+16;blob->owned=1;}
 *out=blob;*error=NULL;return 0;''';assert a in api;api=api.replace(a,b,1)
a='int simple=n>16 && !memcmp(p,"//native-simple\\n",16);'
b='''int store=n>16 && !memcmp(p,"//skip-store-v1\\n",16);
 if(store){p=(const char*)p+16;n-=16;unsigned id=n>16 && !memcmp(p,"//literal-",10)?((const char*)p)[10]-'0'+1:n>16 && !memcmp(p,"//native-simple\\n",16)?1:0;if(store_create_bad==(int)id)return -1;}
 '''+a;assert a in api;api=api.replace(a,b,1)
a='   uint32_t depth_bits;memcpy(&depth_bits,&p.depth,4);'
b='''   unsigned store_id=literal?literal+1:bound_pixel==&simple_pixel_shader?1:0;
   if(g.skip_unchanged){
    ++store_candidate_draws[store_id];
    if(store_color_bad==(int)store_id)p.color^=0x40;
    if(store_depth_bad==(int)store_id)p.depth=-123;
    if(store_count_bad==(int)store_id)p.zpass^=1;
    if(store_material_bad && literal==2 && g.literal.variant==1)p.color^=0x40;
    if(store_map_bad)fail_next_map=1;
   }
'''+a+'''
   int changed=p.color!=before_color || depth_bits!=before_z;
   if(g.skip_unchanged && !changed){++store_reads;if(p.zpass)++store_read_zpass;if(store_noop_bad)p.zpass=0;}
   else ++store_writes;
''';assert a in fixture;fixture=fixture.replace(a,b,1)
a='   surface[at].color=p.color;surface[at].depth_bits=depth_bits;'
b='''   if(!g.skip_unchanged || changed){
    if(!(g.skip_unchanged && store_drop_write && x==5 && y==5)){surface[at].color=p.color;surface[at].depth_bits=depth_bits;}
   }''';assert a in fixture;fixture=fixture.replace(a,b,1)
# Simulate D3D11 reusing the same shader object pointer with a new COM ref.
# Candidate rollback must release its reference even when the pointer aliases.
a='*out=literal_pixel_shader+id-1;return 0;';assert a in api;api=api.replace(a,'++store_refs[id+1];'+a,1)
a='*out=simple?&simple_pixel_shader:&pixel_shader;return 0;';assert a in api;api=api.replace(a,'++store_refs[simple?1:0];'+a,1)
a='assert(s==&pixel_shader || s==&simple_pixel_shader || mock_literal_id(s));'
b=a+'unsigned id=mock_literal_id(s);unsigned slot=id?id+1:s==&simple_pixel_shader?1:0;assert(store_refs[slot]>0);--store_refs[slot];';assert a in api;api=api.replace(a,b,1)
api=api.replace('static unsigned store_compiles[5]', 'static unsigned store_refs[5];static unsigned store_compiles[5]',1)
main=r'''
int main(int argc,char **argv){
 const char *mode=argc>1?argv[1]:"normal";int fault=0;
 setenv("RECOMP_GPU_RASTER","1",1);setenv("RECOMP_GPU_ASYNC","0",1);setenv("RECOMP_GPU_PROFILE","0",1);
 if(!strcmp(mode,"disabled"))setenv("RECOMP_GPU_SKIP_UNCHANGED","0",1);
 if(!strcmp(mode,"compute"))setenv("RECOMP_GPU_NATIVE","0",1);
 if(!strcmp(mode,"diagnostic"))setenv("RECOMP_GPU_DIAGNOSTIC_COUNTERS","1",1);
 if(!strcmp(mode,"array"))setenv("RECOMP_GPU_FIXED","0",1);
 if(!strcmp(mode,"literal-off"))setenv("RECOMP_GPU_LITERAL_MATERIALS","0",1);
 if(!strcmp(mode,"compact-off"))setenv("RECOMP_GPU_COMPACT_MATERIALS","0",1);
 if(!strcmp(mode,"batch-off"))setenv("RECOMP_GPU_BATCH_UPLOADS","0",1);
 if(!strcmp(mode,"simple-off"))setenv("RECOMP_GPU_NATIVE_SIMPLE","0",1);
 int id;
 if(sscanf(mode,"compile-%d",&id)==1){store_compile_bad=id;fault=1;}
 if(sscanf(mode,"create-%d",&id)==1){store_create_bad=id;fault=1;}
 if(sscanf(mode,"color-%d",&id)==1){store_color_bad=id;fault=1;}
 if(sscanf(mode,"depth-%d",&id)==1){store_depth_bad=id;fault=1;}
 if(sscanf(mode,"count-%d",&id)==1){store_count_bad=id;fault=1;}
 store_material_bad=!strcmp(mode,"material");store_noop_bad=!strcmp(mode,"noop-count");store_drop_write=!strcmp(mode,"lost-write");store_map_bad=!strcmp(mode,"map");
 fault|=store_material_bad || store_noop_bad || store_drop_write;
 native_supported=1;coverage_tier=3;int ready=nv_gpu_available();
 if(store_map_bad){assert(!ready && s_startup==3 && !g.active && !g.host_color && !g.host_depth);puts("PASS: real candidate readback failure rejects readiness");return 0;}
 assert(ready);
 if(!strcmp(mode,"compute")){assert(!g.native && !g.skip_unchanged);puts("PASS: unchanged compute fallback");return 0;}
 int optimized=strcmp(mode,"disabled") && !fault;
 assert(g.native && g.skip_unchanged==(unsigned)optimized && g.store_rejected==(unsigned)fault);
 unsigned expected=1u|(g.simple_pixel_shader?2u:0u)|(g.literal.ready<<2);
 assert(g.store_ready==(optimized?expected:0u));
 for(unsigned i=0;i<5;i++)assert(store_refs[i]==((expected>>i)&1u));
 if(!strcmp(mode,"warm"))assert(!compiler_calls);
 if(!strcmp(mode,"disabled"))for(unsigned i=0;i<5;i++)assert(!store_compiles[i]);
 unsigned save=g.skip_unchanged;g.skip_unchanged=1;unsigned a=gpu_native_variant(0),sa=gpu_native_variant(2);g.skip_unchanged=0;unsigned b=gpu_native_variant(0),sb=gpu_native_variant(2);g.skip_unchanged=save;
 assert((a^b)==512 && (sa^sb)==512);
 assert(gpu_compile_key(nv_gpu_rov_source,sizeof nv_gpu_rov_source-1,"pixel_main","ps_5_0",a)!=gpu_compile_key(nv_gpu_rov_source,sizeof nv_gpu_rov_source-1,"pixel_main","ps_5_0",b));
 if(optimized && g.literal.ready)for(unsigned i=0;i<5;i++)if(expected&(1u<<i))assert(store_candidate_draws[i]>0);
 uint32_t color[256]={0},expected_color[256]={0};float depth[256],expected_z[256];for(unsigned i=0;i<256;i++)depth[i]=expected_z[i]=100;
 NvGpuState s={0};s.width=s.height=16;s.pitch=64;s.bpp=4;s.format=0x128;s.color_mask=0x01010101;
 s.depth_available=s.depth_test=s.depth_mask=s.zpass_enable=1;s.depth_func=0x203;
 NvGpuTri t[3];for(unsigned i=0;i<3;i++)t[i]=triangle(16);
 for(unsigned v=0;v<3;v++){t[2].v[v].pos[2]=9;t[2].v[v].d0[0]=0;t[2].v[v].d0[2]=0;}
 NvGpuTextureSource tex[4]={0};NvGpuCount count,reference={0};
 unsigned compiles=compiler_calls;store_writes=store_reads=store_read_zpass=0;
 assert(nv_gpu_draw(&s,t,3,(uint8_t*)color,depth,16,tex)==1 && !color[5*16+5]);
 assert(nv_gpu_sync(&count)==1 && color[5*16+5]==0xFF00FF00 && depth[5*16+5]==9);
 for(unsigned j=0;j<3;j++)for(unsigned y=0;y<16;y++)for(unsigned x=0;x<16;x++){
  unsigned at=y*16+x;NvGpuPixel p=gpu_pixel(s,t[j],x,y,expected_color[at],expected_z[at]);expected_color[at]=p.color;expected_z[at]=p.depth;reference.zpass+=p.zpass;
 }
 assert(!memcmp(color,expected_color,sizeof color) && !memcmp(depth,expected_z,sizeof depth) && count.zpass==reference.zpass && count.zpass>0 && compiler_calls==compiles);
 if(optimized)assert(store_writes>0 && store_reads>0 && store_read_zpass>0);
 nv_gpu_report();printf("PASS: %s mask=%u; atomic native variants, exact delayed publication/visibility, no runtime compilation\n",mode,g.store_ready);
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-store-gate-') as folder:
 p=Path(folder)
 for name in ['windows.h','d3d11.h','d3d11_3.h','d3dcompiler.h','dxgi.h']:(p/name).write_text('/* strict API */\n')
 (p/'test.c').write_text(api+'ptrdiff_t xbox_GetMemoryOffset(void){return 0;}\n#include "'+str(k/'nv2a_gpu_raster.c')+'"\n'+fixture+main)
 subprocess.run(['gcc','-std=gnu11','-O1','-ffp-contract=off','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-Werror=implicit-function-declaration','-Werror=incompatible-pointer-types','-I'+str(p),'-I'+str(r/'tools'),'-I'+str(k),str(p/'test.c'),str(k/'nv2a_gpu_tiles.c'),str(k/'nv2a_vsh_interp.c'),'-pthread','-lm','-o',str(p/'test')],check=True)
 modes=['normal','warm','disabled','compute','diagnostic','array','literal-off','compact-off','batch-off','simple-off','material','noop-count','lost-write','map']+[f'{fault}-{id}' for fault in ['compile','create','color','depth','count'] for id in range(5)]
 assert len(modes)==39
 for mode in modes:
  print('RUN store gate '+mode,flush=True);cwd=p/('normal' if mode=='warm' else mode);cwd.mkdir(exist_ok=True);subprocess.run([str(p/'test'),mode],cwd=cwd,check=True,timeout=120)
 print('PASS: all 39 atomic store-policy gate/cache/fallback/publication modes')
