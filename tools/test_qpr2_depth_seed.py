"""Compare frozen-depth source rejection with production rendering; no FPS claim."""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1];k=root/'third_party/xboxrecomp/src/kernel'
source=r'''
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "nv2a_gpu_types.h"
#include "nv2a_gpu_depth_seed.h"
static uint32_t gpu_load_texel(NvGpuTexture t,int st,int face,int x,int y){return 0x80402010u^((uint32_t)st*11111u+(uint32_t)face*7777u+(uint32_t)x*31u+(uint32_t)y*53u+t.texel_base);}
#include "nv2a_gpu_pixel.inc"
#include "nv2a_gpu_shade.inc"
static uint32_t seed=0x94171513u;
static uint32_t next(void){seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed;}
static float initial(unsigned i){float edge[]={NAN,INFINITY,-INFINITY,0,-0.0f,-1,1,10,8388609,16777215};return i%7?((int)(next()%120)-20):edge[(i/7)%10];}
int main(void){
 NvGpuState states[3]={{0}};for(unsigned i=0;i<3;i++){states[i].depth_available=states[i].depth_test=states[i].depth_mask=1;states[i].depth_func=0x203;}
 assert(!nv_gpu_depth_seed_batch_safe(states,0));
 for(unsigned slot=0;slot<3;slot++){for(unsigned f=0;f<0x210;f++){
  states[slot].depth_func=f;assert(nv_gpu_depth_seed_batch_safe(states,3)==(f>=0x200 && f<=0x203));
 }states[slot].depth_func=0x203;}
 for(unsigned slot=0;slot<3;slot++){states[slot].depth_test=0;assert(!nv_gpu_depth_seed_batch_safe(states,3));states[slot].depth_test=1;states[slot].depth_available=0;assert(nv_gpu_depth_seed_batch_safe(states,3));states[slot].depth_available=1;states[slot].depth_mask=0;states[slot].depth_func=0x207;assert(nv_gpu_depth_seed_batch_safe(states,3));states[slot].depth_mask=1;states[slot].depth_func=0x203;}
 for(unsigned i=0;i<3;i++)states[i].depth_func=i&1?0x202:0x200;assert(nv_gpu_depth_seed_batch_safe(states,3) && !nv_gpu_depth_seed_batch_useful(states,3));
 unsigned skipped=0,checks=0,safe_batches=0,unsafe_batches=0;
 for(unsigned batch=0;batch<4096;batch++){
  NvGpuState s[4]={{0}};NvGpuTri t={0};t.maxx=t.maxy=8;t.area=64;t.inv_area=1.0f/64;t.inclusive[0]=t.inclusive[1]=t.inclusive[2]=1;t.use_rc=1;t.v[1].pos[0]=t.v[2].pos[1]=8;
  for(unsigned st=0;st<4;st++){
   s[st].width=s[st].height=8;s[st].bpp=batch&1?2:4;s[st].format=batch%3==0?0x121:0x128;s[st].control0=batch%5==0?1u<<12:0;
   s[st].depth_available=s[st].depth_test=1;s[st].depth_func=st&1?0x201:0x203;s[st].depth_mask=next()&1;
   s[st].color_mask=next()&0x01010101;s[st].zpass_enable=next()&1;s[st].alpha_test=batch&1;s[st].alpha_ref=next();s[st].alpha_func=0x200+next()%8;
   s[st].blend_enable=1;s[st].blend_sfactor=0x300+next()%9;s[st].blend_dfactor=0x300+next()%9;s[st].blend_equation=0x8006+next()%6;
   s[st].rc.control=1;s[st].rc.stage_program=1;s[st].rc.color_icw[0]=0xC8C40000;s[st].rc.color_ocw[0]=0x100C0;s[st].rc.alpha_icw[0]=0xD8D41010;s[st].rc.alpha_ocw[0]=0xC0;s[st].rc.final0=0x130C0300;s[st].rc.final1=0x1C80;
   s[st].tex[0].valid=1;s[st].tex[0].width=s[st].tex[0].height=8;s[st].tex[0].normalized=1;s[st].tex[0].filter=0x02020000;s[st].tex[0].addr_u=s[st].tex[0].addr_v=1;
  }
  if(batch&1)s[3].depth_func=0x200+((batch/2)%8);
  if(batch%17==0)s[3].depth_test=0;if(batch%19==0)s[3].depth_available=0;
  int enabled=nv_gpu_depth_seed_batch_safe(s,4);if(enabled)++safe_batches;else ++unsafe_batches;
  uint32_t color[64],actual[64];float z[64],az[64],frozen[64];
  for(unsigned at=0;at<64;at++){color[at]=actual[at]=next();z[at]=az[at]=frozen[at]=initial(batch+at);}
  for(unsigned draw=0;draw<16;draw++){
   NvGpuState *state=&s[draw%4];float depth=initial(batch+draw);
   for(unsigned v=0;v<3;v++){t.iw[v]=.5f+v*.25f;t.v[v].pos[2]=depth;t.vfog[v]=.5f;
    for(unsigned c=0;c<4;c++){t.v[v].d0[c]=.25f+v*.2f;t.fogc[c]=.1f;t.stc[v][0][c]=c==3?1:(c==v?.75f:-.125f);}}
   for(unsigned y=0;y<8;y++)for(unsigned x=0;x<8;x++){
    unsigned at=y*8+x;NvGpuPixel old=gpu_pixel(*state,t,x,y,color[at],z[at]);
    NvGpuBary bary=gpu_bary(*state,t,x,y);NvGpuSource source={0};int shade=bary.covered;
    if(enabled && shade){shade=gpu_seed_source_visible(*state,bary,frozen[at]);if(!shade)++skipped;}
    if(shade)source=gpu_source_color(*state,t,bary.l0,bary.l1,bary.l2);
    NvGpuPixel out=gpu_commit(*state,bary,source,actual[at],az[at]);assert(!memcmp(&old,&out,sizeof out));
    color[at]=old.color;z[at]=old.depth;actual[at]=out.color;az[at]=out.depth;++checks;
   }
  }
 }
 assert(checks==4194304 && skipped>10000 && safe_batches && unsafe_batches);
 printf("PASS: %u exact pixel/depth/count comparisons; safe=%u unsafe=%u omitted source evaluations=%u; NaN/infinity, masks, alpha, blend and later unsafe states\n",checks,safe_batches,unsafe_batches,skipped);
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-depth-seed-proof-') as folder:
 p=Path(folder);(p/'test.c').write_text(source)
 subprocess.run(['gcc','-std=c11','-O2','-ffp-contract=off','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-I'+str(k),str(p/'test.c'),'-lm','-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True,timeout=120)
