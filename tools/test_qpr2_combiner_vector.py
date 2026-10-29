"""Frozen V91 array reference versus scalar-named and grouped-named writes.
Separate translation units preserve the independent source. CPU fixtures verify
bits/order, not HLSL code generation or GPU performance; Windows gates remain.
"""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1];k=root/'third_party/xboxrecomp/src/kernel';ref=root/'tools/fixtures/v91-fragment'
common=r'''
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nv2a_gpu_types.h"
static uint32_t gpu_load_texel(NvGpuTexture t,int st,int face,int x,int y){(void)t;return 0x80402010u^((uint32_t)st*11111u+(uint32_t)face*7777u+(uint32_t)x*31u+(uint32_t)y*53u);}
'''
wrap=r'''
void NAME_EVAL(const Nv2aCombiner *rc,const float a[4],const float b[4],const float fog[4],const float t[4][4],float out[4]){gpu_rc_eval(*rc,(float*)a,(float*)b,(float*)fog,(float(*)[4])t,out);}
void NAME_PIXEL(NvGpuPixel *out,const NvGpuState *state,const NvGpuTri *tri,int x,int y,uint32_t color,float depth){*out=gpu_pixel(*state,*tri,x,y,color,depth);}
'''
write=r'''
void NAME_WRITE(void *out,const void *in,uint32_t reg,const float value[3],int blue){
 _Static_assert(sizeof(NvGpuRegisters)==60*sizeof(uint32_t),"named register fixture layout");
 NvGpuRegisters r;memcpy(&r,in,sizeof r);
#if NV_GPU_VECTOR_COMBINERS
 gpu_reg_write_rgb(&r,reg,(float*)value,blue);
#else
 gpu_reg_set(&r,reg,0,value[0]);gpu_reg_set(&r,reg,1,value[1]);gpu_reg_set(&r,reg,2,value[2]);if(blue)gpu_reg_set(&r,reg,3,value[2]);
#endif
 memcpy(out,&r,sizeof r);
}
'''
main=r'''
void reference_eval(const Nv2aCombiner*,const float*,const float*,const float*,const float(*)[4],float*);
void scalar_eval(const Nv2aCombiner*,const float*,const float*,const float*,const float(*)[4],float*);
void vector_eval(const Nv2aCombiner*,const float*,const float*,const float*,const float(*)[4],float*);
void reference_pixel(NvGpuPixel*,const NvGpuState*,const NvGpuTri*,int,int,uint32_t,float);
void scalar_pixel(NvGpuPixel*,const NvGpuState*,const NvGpuTri*,int,int,uint32_t,float);
void vector_pixel(NvGpuPixel*,const NvGpuState*,const NvGpuTri*,int,int,uint32_t,float);
void scalar_write(void*,const void*,uint32_t,const float*,int);
void vector_write(void*,const void*,uint32_t,const float*,int);
static uint32_t seed=0x92F1A3D7u;
static uint32_t next(void){seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed;}
static float value(void){return ((int)(next()%8193)-4096)/2048.0f;}
static Nv2aCombiner program(unsigned i){
 Nv2aCombiner r={0};r.control=(next()&~255u)|(i%9);
 for(unsigned j=0;j<8;j++){
  r.color_icw[j]=next();r.alpha_icw[j]=next();r.color_ocw[j]=next();r.alpha_ocw[j]=next();r.factor0[j]=next();r.factor1[j]=next();
  /* Deliberately alias all RGB/alpha destinations in half the cases. */
  if(i&1){unsigned dest=(i>>1)&15;r.color_ocw[j]=(r.color_ocw[j]&~0xFFFu)|dest|(dest<<4)|(dest<<8);r.alpha_ocw[j]=(r.alpha_ocw[j]&~0xFFFu)|dest|(dest<<4)|(dest<<8);}
 }
 r.final0=i%7?next():0;r.final1=i%7?next():0;r.final_c0=next();r.final_c1=next();
 for(unsigned st=0;st<4;st++)r.stage_program|=(next()%6)<<(st*5);
 return r;
}
int main(void){
 for(unsigned i=0;i<65536;i++){
  uint32_t before[60],a[60],b[60],bits[3];float v[3];
  static const uint32_t edge[]={0,0x80000000u,0x7FC12345u,0xFFC54321u,0x7F800000u,0xFF800000u,1,0x80000001u};
  for(unsigned j=0;j<60;j++)before[j]=next();for(unsigned j=0;j<3;j++)bits[j]=(i%16)?next():edge[(i/16+j)%8];memcpy(v,bits,sizeof v);
  unsigned reg=i%33,blue=(i/33)&1;scalar_write(a,before,reg,v,blue);vector_write(b,before,reg,v,blue);assert(!memcmp(a,b,sizeof a));
 }
 puts("PASS: 65536 exact named register writes: arbitrary float bits, signed zero/NaN payloads, optional blue-to-alpha and ignored register IDs");
 for(unsigned i=0;i<200000;i++){
  Nv2aCombiner rc=program(i);float a[4],b[4],fog[4],t[4][4],r[4],s[4],v[4];
  for(unsigned j=0;j<4;j++){a[j]=value();b[j]=value();fog[j]=value();for(unsigned st=0;st<4;st++)t[st][j]=value();}
  reference_eval(&rc,a,b,fog,t,r);scalar_eval(&rc,a,b,fog,t,s);vector_eval(&rc,a,b,fog,t,v);
  assert(!memcmp(r,s,sizeof r) && !memcmp(r,v,sizeof r));
 }
 puts("PASS: 200000 bit-exact frozen V91 array/scalar-named/grouped-named combiner results: 0-8 stages, all destination aliases, mux/output mappings, blue-to-alpha and final programs");
 for(unsigned scene=0;scene<256;scene++){
  NvGpuState s={0};s.width=s.height=16;s.bpp=4;s.pitch=64;s.format=0x128;s.rc=program(scene);
  s.color_mask=next()&0x01010101u;s.alpha_test=scene&1;s.alpha_func=0x200+next()%8;s.alpha_ref=next();s.clip_plane_mode=next();
  s.depth_available=1;s.depth_test=(scene>>1)&1;s.depth_mask=(scene>>2)&1;s.depth_func=0x200+next()%8;s.zpass_enable=1;
  s.blend_enable=1;s.blend_sfactor=0x300+next()%9;s.blend_dfactor=0x300+next()%9;s.blend_equation=0x8006+next()%6;s.blend_color=next();
  for(unsigned st=0;st<4;st++){s.tex[st].valid=1;s.tex[st].width=s.tex[st].height=8;s.tex[st].addr_u=1;s.tex[st].addr_v=3;s.tex[st].cube=st&1;s.tex[st].filter=scene&8?0x02020000u:0;}
  NvGpuTri t={0};t.maxx=t.maxy=16;t.area=256;t.inv_area=1.0f/256;t.inclusive[0]=t.inclusive[1]=t.inclusive[2]=1;t.use_rc=1;
  t.v[1].pos[0]=t.v[2].pos[1]=16;
  for(unsigned j=0;j<3;j++){t.iw[j]=1;t.v[j].pos[2]=10+j;t.v[j].pos[3]=1;t.vfog[j]=.5f;
   for(unsigned c=0;c<4;c++){t.v[j].d0[c]=.5f;t.v[j].d1[c]=.25f;t.fogc[c]=.125f;for(unsigned st=0;st<4;st++)t.stc[j][st][c]=c==3?1:(c==2?.5f:(j==c?4:.25f));}}
  for(int y=0;y<16;y++)for(int x=0;x<16;x++){
   NvGpuPixel r,a,b;uint32_t old=0xFF123456u;float z=(scene&16)?8:20;
   reference_pixel(&r,&s,&t,x,y,old,z);scalar_pixel(&a,&s,&t,x,y,old,z);vector_pixel(&b,&s,&t,x,y,old,z);
   assert(!memcmp(&r,&a,sizeof r) && !memcmp(&r,&b,sizeof r));
  }
 }
 puts("PASS: 65536 frozen V91 pixel/depth/count comparisons with general textures, alpha/depth/blend/channel masks and clip modes");
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-vector-combiner-') as folder:
 p=Path(folder);sources=[]
 for name,inc,flags in [('reference',ref,''),('scalar',k,'#define NV_GPU_FIXED_REGISTERS 1\n#define NV_GPU_VECTOR_COMBINERS 0\n'),('vector',k,'#define NV_GPU_FIXED_REGISTERS 1\n#define NV_GPU_VECTOR_COMBINERS 1\n')]:
  code=flags+common+'\n#include "'+str(inc/'nv2a_gpu_pixel.inc')+'"\n#include "'+str(inc/'nv2a_gpu_shade.inc')+'"\n'+wrap.replace('NAME_EVAL',name+'_eval').replace('NAME_PIXEL',name+'_pixel')
  if name!='reference':code+=write.replace('NAME_WRITE',name+'_write')
  f=p/(name+'.c');f.write_text(code);sources.append(str(f))
 (p/'main.c').write_text(common+main);sources.append(str(p/'main.c'))
 subprocess.run(['gcc','-std=c11','-O2','-ffp-contract=off','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-I'+str(k),*sources,'-lm','-o',str(p/'fixture')],check=True)
 subprocess.run([str(p/'fixture')],check=True,timeout=120)
