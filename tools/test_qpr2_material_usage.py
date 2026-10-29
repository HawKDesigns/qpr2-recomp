"""Frozen V95 full sampling versus production CPU read sets/native stage skip.
Exact pixels, depth, counts and retained texel-load order; no hardware FPS claim.
"""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1];k=root/'third_party/xboxrecomp/src/kernel';old=root/'tools/fixtures/v95-material'
common=r'''
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nv2a_gpu_types.h"
#include "nv2a_gpu_material_usage.h"
typedef struct {int st,face,x,y;} Load;
static Load loads[64];static unsigned count;static uint32_t live;
static uint32_t gpu_load_texel(NvGpuTexture t,int st,int face,int x,int y){
 assert(count<64);loads[count++]=(Load){st,face,x,y};return 0x80402010u^((uint32_t)st*11111u+(uint32_t)face*7777u+(uint32_t)x*31u+(uint32_t)y*53u+t.texel_base);
}
'''
wrapper=r'''
void NAME(NvGpuPixel*out,const NvGpuState*s,const NvGpuTri*t,int x,int y,uint32_t color,float z,Load*trace,unsigned*size){
 live=nv_gpu_texture_reads(&s->rc)|nv_gpu_texture_clip(&s->rc);count=0;
 *out=gpu_pixel(*s,*t,x,y,color,z);memcpy(trace,loads,count*sizeof *loads);*size=count;
}
'''
main=common+r'''
void old_pixel(NvGpuPixel*,const NvGpuState*,const NvGpuTri*,int,int,uint32_t,float,Load*,unsigned*);
void new_pixel(NvGpuPixel*,const NvGpuState*,const NvGpuTri*,int,int,uint32_t,float,Load*,unsigned*);
static uint32_t seed=0x96170913;
static uint32_t next(void){seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed;}
int main(void){
 for(unsigned byte=0;byte<256;byte++)for(unsigned shift=0;shift<32;shift+=8){
  uint32_t r=byte&15,word=byte<<shift;assert(nv_gpu_texture_word_reads(word)==(r>=8 && r<=11?1u<<(r-8):0));
 }
 Nv2aCombiner rc={0};rc.stage_program=1;assert(nv_gpu_texture_reads(&rc)==1);
 rc.stage_program=0;rc.final1=0x80;assert(!nv_gpu_texture_reads(&rc));
 for(unsigned reg=8;reg<=11;reg++)for(unsigned shift=8;shift<=24;shift+=8){rc.final1=reg<<shift;assert(nv_gpu_texture_reads(&rc)==(1u<<(reg-8)));}
 unsigned checks=0,omitted=0,sparse=0,clips=0;
 for(unsigned scene=0;scene<16384;scene++){
  NvGpuState s={0};s.width=s.height=4;s.bpp=scene&1?2:4;s.pitch=4*s.bpp;s.format=scene%3==0?0x121:0x128;
  s.control0=scene%5==0?1u<<12:0;s.color_mask=next()&0x01010101;s.depth_available=scene%11!=0;s.depth_test=scene&2;s.depth_mask=1;s.depth_func=0x200+next()%8;
  s.zpass_enable=1;s.alpha_test=scene&1;s.alpha_func=0x200+next()%8;s.alpha_ref=next();s.blend_enable=1;
  s.blend_sfactor=0x300+next()%9;s.blend_dfactor=0x300+next()%9;s.blend_equation=0x8006+next()%6;s.clip_plane_mode=next();
  s.rc.control=next();s.rc.control=(s.rc.control&~255u)+(scene%10);
  for(unsigned st=0;st<8;st++){
   s.rc.color_icw[st]=next();s.rc.alpha_icw[st]=next();s.rc.color_ocw[st]=next();s.rc.alpha_ocw[st]=next();s.rc.factor0[st]=next();s.rc.factor1[st]=next();
  }
  s.rc.final0=scene%3?next():0;s.rc.final1=scene%3?next():0;s.rc.final_c0=next();s.rc.final_c1=next();
  for(unsigned st=0;st<4;st++){
   unsigned mode=scene%7==0?5:scene%5==0?3:scene%3==0?4:(scene+st)%4;s.rc.stage_program|=mode<<(st*5);
   NvGpuTexture*t=s.tex+st;t->width=3+st;t->height=5;t->valid=scene%13!=0;t->cube=mode==3;t->normalized=scene&1;t->filter=0x02020000;t->addr_u=t->addr_v=1;
  }
  if(scene%4==0){
   /* Three enabled/sampleable textures are never referenced. R0 initial alpha
    * still makes T0 live. Output destinations may overwrite texture registers. */
   s.rc.control=1;s.rc.stage_program=1|(1<<5)|(1<<10)|(1<<15);
   s.rc.color_icw[0]=0xC8C40000;s.rc.alpha_icw[0]=0xD8D41010;s.rc.color_ocw[0]=0x100C0;s.rc.alpha_ocw[0]=0xC0;s.rc.final0=0x130C0300;s.rc.final1=0x1C80;
   s.alpha_test=0;s.depth_test=0;s.color_mask=0x01010101;++sparse;
  }
  if(scene%17==0){s.rc.stage_program=(s.rc.stage_program&~(31u<<10))|(5u<<10);++clips;assert(nv_gpu_texture_clip(&s.rc)&4);}
  NvGpuTri t={0};t.maxx=t.maxy=4;t.area=16;t.inv_area=1.0f/16;t.inclusive[0]=t.inclusive[1]=t.inclusive[2]=1;t.use_rc=scene%31!=0;t.textured=1;t.v[1].pos[0]=t.v[2].pos[1]=4;
  for(unsigned v=0;v<3;v++){
   t.iw[v]=.5f+v*.25f;t.v[v].pos[2]=10+v;t.vfog[v]=.5f;
   for(unsigned c=0;c<4;c++){
    t.v[v].d0[c]=.25f+(float)v*.2f;t.v[v].d1[c]=.125f;t.fogc[c]=.1f;t.uv[v][c%2]=(float)v;
    for(unsigned st=0;st<4;st++)t.stc[v][st][c]=c==3?1:(c==v?.75f:-.125f);
   }
  }
  uint32_t mask=nv_gpu_texture_reads(&s.rc)|nv_gpu_texture_clip(&s.rc);
  for(int y=0;y<4;y++)for(int x=0;x<4;x++){
   NvGpuPixel a,b;Load at[64],bt[64],retained[64];unsigned ac,bc,n=0;uint32_t color=next();float z=scene&8?8:100;
   old_pixel(&a,&s,&t,x,y,color,z,at,&ac);new_pixel(&b,&s,&t,x,y,color,z,bt,&bc);assert(!memcmp(&a,&b,sizeof a));
   for(unsigned i=0;i<ac;i++)if(!t.use_rc || (mask&(1u<<at[i].st)))retained[n++]=at[i];
   assert(n==bc && !memcmp(retained,bt,n*sizeof *bt));omitted+=ac-bc;++checks;
  }
 }
 assert(checks==262144 && sparse && clips && omitted>10000);
 printf("PASS: %u frozen V95 pixel/depth/count comparisons and exact retained load traces; %u omitted texel loads; aliases, all input mappings, finals, T0 alpha, CLIP, cube, mask, blend and non-RC fallback\n",checks,omitted);
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-material-usage-') as folder:
 p=Path(folder);sources=[]
 for name,ref in [('old',old),('new',k)]:
  defines='#define NV_GPU_QUAD_SAMPLING 1\n'
  if name=='new':defines+='#define NV_GPU_FIXED_REGISTERS 1\n#define NV_GPU_VECTOR_COMBINERS 1\n#define NV_GPU_STAGE_LIVENESS 1\n#define GPU_TEXTURE_STAGE_MASK(tri) live\n'
  body=common+defines+'#include "'+str(ref/'nv2a_gpu_pixel.inc')+'"\n#include "'+str(ref/'nv2a_gpu_shade.inc')+'"\n'+wrapper.replace('NAME',name+'_pixel')
  source=p/(name+'.c');source.write_text(body);sources.append(str(source))
 (p/'main.c').write_text(main)
 subprocess.run(['gcc','-std=c11','-O2','-ffp-contract=off','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-I'+str(k),*sources,str(p/'main.c'),'-lm','-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True,timeout=180)
