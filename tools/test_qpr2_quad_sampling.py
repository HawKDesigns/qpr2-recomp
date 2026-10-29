"""Frozen V91/V92 sampler versus shared addresses, with exact load traces.
Portable arithmetic/API checks are not hardware shader benchmarks.
"""
from pathlib import Path
import subprocess,tempfile,zipfile
root=Path(__file__).resolve().parents[1];k=root/'third_party/xboxrecomp/src/kernel';ref=root/'tools/fixtures/v91-fragment'
common=r'''
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nv2a_gpu_types.h"
typedef struct {int st,face,x,y;uint32_t base;} Load;
static Load loads[64];static unsigned load_count,wrap_count;
static uint32_t gpu_load_texel(NvGpuTexture t,int st,int face,int x,int y){
 assert(load_count<64);Load *p=&loads[load_count++];p->st=st;p->face=face;p->x=x;p->y=y;p->base=t.texel_base;
 return 0x80402010u^((uint32_t)st*11111u+(uint32_t)face*7777u+(uint32_t)x*31u+(uint32_t)y*53u+t.texel_base);
}
'''
wrapper=r'''
void NAME_SAMPLE(const NvGpuState *s,int st,int face,float u,float v,float out[4],Load *trace,unsigned *counts){
 load_count=wrap_count=0;gpu_sample(*s,st,face,u,v,out);memcpy(trace,loads,load_count*sizeof(Load));counts[0]=load_count;counts[1]=wrap_count;
}
void NAME_PIXEL(NvGpuPixel *out,const NvGpuState *s,const NvGpuTri *t,int x,int y,uint32_t color,float z){load_count=wrap_count=0;*out=gpu_pixel(*s,*t,x,y,color,z);}
'''
main=r'''
void old_sample(const NvGpuState*,int,int,float,float,float*,Load*,unsigned*);
void new_sample(const NvGpuState*,int,int,float,float,float*,Load*,unsigned*);
void old_pixel(NvGpuPixel*,const NvGpuState*,const NvGpuTri*,int,int,uint32_t,float);
void new_pixel(NvGpuPixel*,const NvGpuState*,const NvGpuTri*,int,int,uint32_t,float);
static uint32_t seed=0x93170113u;
static uint32_t next(void){seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed;}
int main(void){
 const unsigned sizes[]={1,2,3,4,7,16,31,64,256,257,1024};
 unsigned bilinear=0,point=0,invalid=0;
 for(unsigned i=0;i<131072;i++){
  NvGpuState s={0};unsigned st=i%4;NvGpuTexture *t=&s.tex[st];
  t->valid=i%17!=0;t->width=sizes[next()%11];t->height=sizes[next()%11];t->normalized=(i>>2)&1;
  t->addr_u=next()%5;t->addr_v=next()%5;t->texel_base=next();t->cube=(i>>3)&1;
  const unsigned filters[]={0x01010000,0x02010000,0x01020000,0x01040000,0x01060000,0x01030000};t->filter=filters[i%6];
  float u=((int)(next()%32769)-16384)/1024.0f,v=((int)(next()%32769)-16384)/1024.0f;
  if(i%32==0){u=(float)((int)(i%31)-15)+0.5f;v=-u;}
  float a[4],b[4];Load at[64],bt[64];unsigned ac[2],bc[2];unsigned face=i%6;
  old_sample(&s,st,face,u,v,a,at,ac);new_sample(&s,st,face,u,v,b,bt,bc);
  assert(!memcmp(a,b,sizeof a) && ac[0]==bc[0] && !memcmp(at,bt,ac[0]*sizeof(Load)));
  int linear=i%6!=0 && i%6!=5;
  if(!t->valid){assert(!ac[0] && !ac[1] && !bc[1]);++invalid;}
  else if(linear){assert(ac[0]==4 && ac[1]==8 && bc[1]==4);++bilinear;}
  else {assert(ac[0]==1 && ac[1]==2 && bc[1]==2);++point;}
 }
 printf("PASS: 131072 frozen sampler comparisons; identical float bits and ordered texel addresses; bilinear=%u point=%u invalid=%u; valid bilinear wrap calls 8->4\n",bilinear,point,invalid);
 for(unsigned scene=0;scene<512;scene++){
  NvGpuState s={0};s.width=s.height=16;s.bpp=scene&1?2:4;s.pitch=16*s.bpp;s.format=0x128;
  s.color_mask=next()&0x01010101u;s.alpha_test=scene&1;s.alpha_func=0x200+next()%8;s.alpha_ref=next();s.zpass_enable=1;
  s.depth_available=s.depth_mask=1;s.depth_test=(scene>>1)&1;s.depth_func=0x200+next()%8;
  s.blend_enable=1;s.blend_sfactor=0x300+next()%9;s.blend_dfactor=0x300+next()%9;s.blend_equation=0x8006+next()%6;
  s.rc.control=1;s.rc.stage_program=scene%3==0?1:scene%3==1?3:(1|(5u<<5));
  s.rc.color_icw[0]=0xC8C40000u;s.rc.color_ocw[0]=0x100C0u;s.rc.alpha_icw[0]=0xD8D41010u;s.rc.alpha_ocw[0]=0xC0u;
  s.rc.final0=0x130C0300u;s.rc.final1=0x1C80u;s.clip_plane_mode=next();
  for(unsigned st=0;st<4;st++){NvGpuTexture*t=&s.tex[st];t->valid=scene%19!=0;t->width=sizes[next()%11];t->height=sizes[next()%11];t->addr_u=scene%5;t->addr_v=next()%5;t->normalized=scene&1;t->cube=scene%3==1;t->filter=0x02020000;}
  NvGpuTri t={0};t.maxx=t.maxy=16;t.area=256;t.inv_area=1.0f/256;t.inclusive[0]=t.inclusive[1]=t.inclusive[2]=1;t.use_rc=scene%7!=0;t.textured=1;t.v[1].pos[0]=t.v[2].pos[1]=16;
  for(unsigned j=0;j<3;j++){t.iw[j]=.5f+j*.25f;t.v[j].pos[2]=10+j;t.v[j].pos[3]=1;t.vfog[j]=.5f;
   for(unsigned c=0;c<4;c++){t.v[j].d0[c]=.5f;t.v[j].d1[c]=.25f;t.fogc[c]=.125f;t.uv[j][c%2]=(float)j;
    for(unsigned st=0;st<4;st++)t.stc[j][st][c]=c==3?1:(c==2?.5f:(j==c?4:-.25f));}}
  for(int y=0;y<16;y++)for(int x=0;x<16;x++){NvGpuPixel a,b;uint32_t color=next();float z=scene&16?8:20;old_pixel(&a,&s,&t,x,y,color,z);new_pixel(&b,&s,&t,x,y,color,z);assert(!memcmp(&a,&b,sizeof a));}
 }
 puts("PASS: 131072 frozen full pixel/depth/count comparisons: normalized/cube/clip stages, bilinear/point, alpha/depth/blend/masks");
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-quad-sampling-') as folder:
 p=Path(folder);sources=[]
 for name,inc,flag in [('old',ref,0),('new',k,1)]:
  shade=(inc/'nv2a_gpu_shade.inc').read_text().replace('GPU_FN int gpu_wrap(', 'GPU_FN int gpu_wrap_impl(',1)
  anchor='GPU_FN uint32_t gpu_texel('
  assert anchor in shade
  shade=shade.replace(anchor,'GPU_FN int gpu_wrap(int c,uint32_t size,uint32_t mode){++wrap_count;return gpu_wrap_impl(c,size,mode);}\n'+anchor,1)
  code='#define NV_GPU_QUAD_SAMPLING '+str(flag)+'\n'+common+'\n#include "'+str(inc/'nv2a_gpu_pixel.inc')+'"\n'+shade+wrapper.replace('NAME_SAMPLE',name+'_sample').replace('NAME_PIXEL',name+'_pixel')
  f=p/(name+'.c');f.write_text(code);sources.append(str(f))
 (p/'main.c').write_text(common+main);sources.append(str(p/'main.c'))
 subprocess.run(['gcc','-std=c11','-O2','-ffp-contract=off','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-I'+str(k),*sources,'-lm','-o',str(p/'fixture')],check=True)
 subprocess.run([str(p/'fixture')],check=True,timeout=120)
