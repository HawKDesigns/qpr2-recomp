"""Generated literal bodies vs frozen V95/V96 full array evaluator.
Checks exact instruction matching and dynamic values, not hardware performance.
"""
from pathlib import Path
import subprocess,tempfile,json
root=Path(__file__).resolve().parents[1];k=root/'third_party/xboxrecomp/src/kernel';ref=root/'tools/fixtures/v95-material'
# Regeneration must be deterministic and preserve the reviewed checked-in files.
before=[(k/n).read_bytes() for n in ['nv2a_gpu_literal.inc','nv2a_gpu_literal_programs.h']]
subprocess.run(['python3',str(root/'tools/generate_qpr2_literal_materials.py')],check=True)
assert before==[(k/n).read_bytes() for n in ['nv2a_gpu_literal.inc','nv2a_gpu_literal_programs.h']]
common=r'''
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nv2a_gpu_types.h"
#include "nv2a_gpu_literal_programs.h"
static uint32_t gpu_load_texel(NvGpuTexture t,int st,int face,int x,int y){return 0x80402010u^((uint32_t)st*11111u+(uint32_t)face*7777u+(uint32_t)x*31u+(uint32_t)y*53u+t.texel_base);}
'''
wrapper=r'''
void NAME_EVAL(float *out,const Nv2aCombiner *rc,float v[4],float d[4],float fog[4],float t[4][4]){EVAL(*rc,v,d,fog,t,out);}
void NAME_PIXEL(NvGpuPixel *out,const NvGpuState *s,const NvGpuTri *t,int x,int y,uint32_t color,float z){*out=gpu_pixel(*s,*t,x,y,color,z);}
'''
main=common+r'''
void reference_eval(float*,const Nv2aCombiner*,float*,float*,float*,float[4][4]);
void reference_pixel(NvGpuPixel*,const NvGpuState*,const NvGpuTri*,int,int,uint32_t,float);
void literal1_eval(float*,const Nv2aCombiner*,float*,float*,float*,float[4][4]);
void literal2_eval(float*,const Nv2aCombiner*,float*,float*,float*,float[4][4]);
void literal3_eval(float*,const Nv2aCombiner*,float*,float*,float*,float[4][4]);
void literal1_pixel(NvGpuPixel*,const NvGpuState*,const NvGpuTri*,int,int,uint32_t,float);
void literal2_pixel(NvGpuPixel*,const NvGpuState*,const NvGpuTri*,int,int,uint32_t,float);
void literal3_pixel(NvGpuPixel*,const NvGpuState*,const NvGpuTri*,int,int,uint32_t,float);
static uint32_t seed=0x97171513;
static uint32_t next(void){seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed;}
static float value(void){return (int)(next()%8193)/2048.0f-2;}
int main(void){
 unsigned evals=0,pixels=0,mutations=0;
 for(unsigned id=1;id<=3;id++){
  Nv2aCombiner rc;nv_gpu_literal_fixture(&rc,id,0);assert(nv_gpu_literal_program(&rc)==id);
  unsigned n=rc.control&255;
  for(unsigned word=0;word<4;word++)for(unsigned stage=0;stage<n;stage++)for(unsigned bit=0;bit<32;bit++){
   Nv2aCombiner bad=rc;uint32_t *p=word==0?bad.color_icw:word==1?bad.alpha_icw:word==2?bad.color_ocw:bad.alpha_ocw;p[stage]^=1u<<bit;
   assert(!nv_gpu_literal_program(&bad));++mutations;
  }
  for(unsigned field=0;field<3;field++)for(unsigned bit=0;bit<32;bit++){
   Nv2aCombiner bad=rc;uint32_t*p=field==0?&bad.control:field==1?&bad.stage_program:&bad.final1;*p^=1u<<bit;assert(!nv_gpu_literal_program(&bad));++mutations;
  }
  for(unsigned stage=n;stage<8;stage++){rc.color_icw[stage]=next();rc.alpha_icw[stage]=next();rc.color_ocw[stage]=next();rc.alpha_ocw[stage]=next();}assert(nv_gpu_literal_program(&rc)==id);
  for(unsigned i=0;i<65536;i++){
   nv_gpu_literal_fixture(&rc,id,i);
   for(unsigned st=0;st<8;st++){rc.factor0[st]=next();rc.factor1[st]=next();}
   rc.final_c0=next();rc.final_c1=next();assert(nv_gpu_literal_program(&rc)==id);
   float v[4],d[4],fog[4],t[4][4],a[4],b[4];
   for(unsigned c=0;c<4;c++){v[c]=value();d[c]=value();fog[c]=value();for(unsigned st=0;st<4;st++)t[st][c]=value();}
   reference_eval(a,&rc,v,d,fog,t);
   if(id==1)literal1_eval(b,&rc,v,d,fog,t);else if(id==2)literal2_eval(b,&rc,v,d,fog,t);else literal3_eval(b,&rc,v,d,fog,t);
   assert(!memcmp(a,b,sizeof a));++evals;
  }
  for(unsigned scene=0;scene<4096;scene++){
   NvGpuState s={0};nv_gpu_literal_fixture(&s.rc,id,scene);
   for(unsigned st=0;st<8;st++){s.rc.factor0[st]=next();s.rc.factor1[st]=next();}s.rc.final_c0=next();s.rc.final_c1=next();
   s.width=s.height=4;s.bpp=scene&1?2:4;s.pitch=4*s.bpp;s.format=scene%3==0?0x121:0x128;s.control0=scene%5==0?1u<<12:0;
   s.color_mask=next()&0x01010101;s.depth_available=s.depth_mask=1;s.depth_test=scene&2;s.depth_func=0x200+next()%8;
   s.zpass_enable=1;s.alpha_test=scene&1;s.alpha_func=0x200+next()%8;s.alpha_ref=next();s.blend_enable=1;
   s.blend_sfactor=0x300+next()%9;s.blend_dfactor=0x300+next()%9;s.blend_equation=0x8006+next()%6;s.blend_color=next();
   for(unsigned st=0;st<2;st++){NvGpuTexture *t=s.tex+st;t->valid=scene%13!=0;t->width=3+st;t->height=5;t->normalized=scene&1;t->filter=scene%3?0x02020000:0x01010000;t->addr_u=1;t->addr_v=3;}
   NvGpuTri tri={0};tri.maxx=tri.maxy=4;tri.area=16;tri.inv_area=1.0f/16;tri.inclusive[0]=tri.inclusive[1]=tri.inclusive[2]=1;tri.use_rc=scene%31!=0;tri.textured=1;tri.v[1].pos[0]=tri.v[2].pos[1]=4;
   for(unsigned v=0;v<3;v++){tri.iw[v]=.5f+v*.25f;tri.v[v].pos[2]=10+v;tri.vfog[v]=.5f;
    for(unsigned c=0;c<4;c++){tri.v[v].d0[c]=.25f+v*.2f;tri.v[v].d1[c]=.125f+v*.15f;tri.fogc[c]=.1f;tri.uv[v][c%2]=(float)v;
     for(unsigned st=0;st<4;st++)tri.stc[v][st][c]=c==3?1:(c==v?.75f:-.125f);}}
   for(int y=0;y<4;y++)for(int x=0;x<4;x++){
    NvGpuPixel a,b;uint32_t color=next();float z=scene&8?8:100;reference_pixel(&a,&s,&tri,x,y,color,z);
    if(id==1)literal1_pixel(&b,&s,&tri,x,y,color,z);else if(id==2)literal2_pixel(&b,&s,&tri,x,y,color,z);else literal3_pixel(&b,&s,&tri,x,y,color,z);
    assert(!memcmp(&a,&b,sizeof a));++pixels;
   }
  }
 }
 assert(evals==196608 && pixels==196608 && mutations==2592);
 printf("PASS: %u bit-exact frozen combiner results, %u full pixel/depth/count results, %u one-bit program mutations rejected; dynamic factors, finals, aliases, fog/SUM, alpha/depth/blend/masks; deterministic generation\n",evals,pixels,mutations);
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-literal-materials-') as folder:
 p=Path(folder);sources=[]
 for name,id in [('reference',0),('literal1',1),('literal2',2),('literal3',3)]:
  inc=ref if not id else k;flags='#define NV_GPU_QUAD_SAMPLING 1\n'
  if id:flags+=f'#define NV_GPU_FIXED_REGISTERS 1\n#define NV_GPU_VECTOR_COMBINERS 1\n#define NV_GPU_LITERAL_CLASS {id}\n'
  source=flags+common+'#include "'+str(inc/'nv2a_gpu_pixel.inc')+'"\n'
  if id:source+='#include "'+str(k/'nv2a_gpu_literal.inc')+'"\n'
  source+='#include "'+str(inc/'nv2a_gpu_shade.inc')+'"\n'+wrapper.replace('NAME_EVAL',name+'_eval').replace('NAME_PIXEL',name+'_pixel').replace('EVAL','gpu_rc_literal' if id else 'gpu_rc_eval')
  f=p/(name+'.c');f.write_text(source);sources.append(str(f))
 (p/'main.c').write_text(main)
 subprocess.run(['gcc','-std=c11','-O2','-ffp-contract=off','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-I'+str(k),*sources,str(p/'main.c'),'-lm','-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True,timeout=180)
