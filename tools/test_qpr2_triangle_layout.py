"""Verify restored full geometry against V84 and replay V85 compact captures.

Separate translation units retain the old ABI and its actual preparation code.
Also exercise both capture formats through the production replay reader.
No hardware HLSL execution or gameplay speed claim is made by this test.
"""
from pathlib import Path
import subprocess,tempfile,sys
root=Path(__file__).resolve().parents[1]
k=root/'third_party/xboxrecomp/src/kernel'
reference=root/'tools/fixtures/v84-prepared-triangle'
common=r'''
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
'''
pixel=r'''
static uint32_t gpu_load_texel(NvGpuTexture t,int st,int face,int x,int y){
 (void)t;return 0x80402010u ^ (uint32_t)(st*11111+face*7777+x*31+y*53);
}
#include "nv2a_gpu_pixel.inc"
#include "nv2a_gpu_shade.inc"
'''
legacy=common+r'''
#include "types.h"
#include "nv2a_gpu_raw_types.h"
#include "prepare.inc"
'''+pixel+r'''
void legacy_prepare(void *out,const void *params,const void *vertices){
 const Nv2aVshOutput *v=vertices;NvGpuTri t=nv_raw_prepare(*(const NvRawParams*)params,v[0],v[1],v[2]);
 assert(sizeof t==688);memcpy(out,&t,sizeof t);
}
void legacy_pixel(void *out,const void *state,const void *tri,int x,int y,uint32_t color,float depth){
 NvGpuTri t;memcpy(&t,tri,sizeof t);NvGpuPixel p=gpu_pixel(*(const NvGpuState*)state,t,x,y,color,depth);memcpy(out,&p,sizeof p);
}
'''
main=common+r'''
#include "nv2a_gpu_types.h"
#include "nv2a_gpu_raw_types.h"
#include "nv2a_gpu_prepare.inc"
'''+pixel+r'''
void legacy_prepare(void*,const void*,const void*);
void legacy_pixel(void*,const void*,const void*,int,int,uint32_t,float);
static uint32_t seed=0xA41C73D9u;
static uint32_t next(void){seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed;}
static float scalar(void){return (int)(next()%4097)/1024.0f-2;}
static void record(FILE *f,uint32_t kind,const void *bytes,uint32_t size){uint32_t h[2]={kind,size};assert(fwrite(h,8,1,f)==1);assert(fwrite(bytes,size,1,f)==1);}
static void capture(const char *name,unsigned version){
 FILE *f=fopen(name,"wb");assert(f);uint32_t header[4]={0x51475235u,version,sizeof(NvGpuState),version==1?688:448};assert(fwrite(header,16,1,f)==1);
 NvGpuState s={0};s.width=s.height=4;s.bpp=4;s.pitch=16;s.color_mask=0x01010101;s.depth_available=s.depth_mask=1;
 unsigned char target[sizeof s+128];memcpy(target,&s,sizeof s);memset(target+sizeof s,0,128);record(f,1,target,sizeof target);
 NvRawParams p={0};p.right=p.bottom=4;p.su=p.sv=1;
 Nv2aVshOutput vertices[3]={0};float positions[3][2]={{0,0},{4,0},{0,4}};
 for(unsigned v=0;v<3;v++){vertices[v].pos[0]=positions[v][0];vertices[v].pos[1]=positions[v][1];vertices[v].pos[2]=2;vertices[v].pos[3]=1;vertices[v].d0[0]=vertices[v].d0[3]=1;}
 NvGpuTri tri=nv_raw_prepare(p,vertices[0],vertices[1],vertices[2]);
 unsigned char batch[12+sizeof s+688];uint32_t bh[3]={1,1,1};memcpy(batch,bh,12);memcpy(batch+12,&s,sizeof s);
 if(version==1)legacy_prepare(batch+12+sizeof s,&p,vertices);
 else {
  unsigned char *out=batch+12+sizeof s;
  for(unsigned v=0;v<3;v++)memcpy(out+v*48,&tri.v[v],48);
  memcpy(out+144,&tri.iw,304);
 }
 record(f,3,batch,12+sizeof s+header[3]);
 struct {uint32_t w,h,color[16];float depth[16];} output={4,4,{0},{0}};
 for(unsigned y=0;y<4;y++)for(unsigned x=0;x<4;x++){unsigned at=y*4+x;NvGpuPixel q=gpu_pixel(s,tri,x,y,0,0);output.color[at]=q.color;output.depth[at]=q.depth;}
 record(f,4,&output,sizeof output);fclose(f);
}
int main(void){
 assert(sizeof(NvGpuTri)==688 && offsetof(NvGpuTri,iw)==384);
 unsigned pixel_checks=0,nonempty=0;
 for(unsigned test=0;test<4096;test++){
  Nv2aVshOutput v[3];for(unsigned j=0;j<3;j++)for(unsigned k=0;k<sizeof v[j]/4;k++)((float*)&v[j])[k]=scalar();
  float pos[3][2]={{1,1},{15,2},{3,15}};
  for(unsigned j=0;j<3;j++){v[j].pos[0]=pos[j][0];v[j].pos[1]=pos[j][1];v[j].pos[2]=1+next()%32;v[j].pos[3]=.5f+(next()%8)*.25f;}
  NvRawParams p={0};p.right=p.bottom=16;p.use_rc=test&1;p.textured=(test/2)&1;p.state=test%8;p.su=scalar();p.sv=scalar();
  p.fog_enable=test%3;p.fog_mode=0x800+(test%5);p.fog_x=scalar();p.fog_y=scalar();for(unsigned j=0;j<4;j++)p.fog_color[j]=scalar();
  p.cull_enable=test%4==1;p.cull_face=0x404+test%2;p.front_face=0x900+(test%2);
  if(test%17==0)v[2]=v[1];if(test%19==0)v[0].pos[3]=0;if(test%23==0)v[1].pos[0]=NAN;
  unsigned char old[688];legacy_prepare(old,&p,v);NvGpuTri t=nv_raw_prepare(p,v[0],v[1],v[2]);
  assert(!memcmp(&t,old,sizeof t));nonempty+=t.maxx>t.minx && t.maxy>t.miny;
  NvGpuState s={0};s.width=s.height=16;s.color_mask=(test%3)?0x01010101:0x01010001;
  s.depth_available=1;s.depth_test=(test/4)&1;s.depth_mask=(test/8)&1;s.depth_func=0x200+test%8;
  s.alpha_test=(test/16)&1;s.alpha_func=0x200+(test/8)%8;s.alpha_ref=test%256;s.zpass_enable=1;
  s.blend_enable=(test/32)&1;s.blend_sfactor=0x302;s.blend_dfactor=0x303;s.blend_equation=0x8006;
  s.rc.stage_program=p.use_rc?1:0;
  for(unsigned st=0;st<4;st++){s.tex[st].width=s.tex[st].height=4;s.tex[st].valid=1;s.tex[st].normalized=1;s.tex[st].filter=0x1010000;}
  for(unsigned sample=0;sample<64;sample++){
   int x=next()%16,y=next()%16;uint32_t c=next();float z=(float)(next()%40);
   NvGpuPixel a=gpu_pixel(s,t,x,y,c,z),b;legacy_pixel(&b,&s,old,x,y,c,z);assert(!memcmp(&a,&b,sizeof a));++pixel_checks;
  }
 }
 assert(nonempty>2500);capture("legacy.bin",1);capture("compact.bin",2);
 printf("PASS: restored 688-byte records match 4096 frozen-V84 preparations and %u exact pixel/depth/visibility checks\n",pixel_checks);
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-triangle-layout-') as folder:
    p=Path(folder);(p/'legacy.c').write_text(legacy);(p/'main.c').write_text(main)
    subprocess.run(['gcc','-std=c11','-O2','-ffp-contract=off','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all',
        '-I'+str(reference),'-I'+str(k),str(p/'legacy.c'),str(p/'main.c'),'-lm','-o',str(p/'test')],check=True)
    subprocess.run([str(p/'test')],check=True,cwd=p)
    for name in ['legacy.bin','compact.bin']:
        subprocess.run([sys.executable,str(root/'tools/replay_qpr2_gpu_capture.py'),str(p/name)],check=True,cwd=p)
    print('PASS: V1 full and V2 compact captures both replay with matching output')
