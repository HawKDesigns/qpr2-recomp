"""Compare a bounded F8 GPU capture with the shared scalar pixel program.
Usage: python replay_qpr2_gpu_capture.py logs/fifty-third-gpu-capture.bin
Requires a C11 compiler; no game assets, Windows SDK or hardware needed.
"""
from pathlib import Path
import subprocess,tempfile,sys
root=Path(__file__).resolve().parents[1]
source=r'''
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include "nv2a_gpu_raster.h"
#include "nv2a_frame_capture.h"
static uint32_t*texture[128];static uint32_t texture_count[128];
static uint32_t gpu_load_texel(NvGpuTexture t,int st,int face,int x,int y){
 (void)st;unsigned at=((unsigned)face*t.height+(unsigned)y)*t.width+(unsigned)x;
 assert(t.cache_slot<128 && at<texture_count[t.cache_slot]);return texture[t.cache_slot][at];
}
#include "nv2a_gpu_pixel.inc"
#include "nv2a_gpu_shade.inc"
static void read_exact(FILE*f,void*p,size_t n){if(fread(p,1,n,f)!=n){fprintf(stderr,"Truncated capture record\n");exit(2);}}
/* Runtime V1 uses full vertex outputs. V85 V2 captures stored only position
 * and diffuse colors; expand them for replay with unused fields zeroed. The
 * prepared interpolation suffix is byte-identical in both formats. */
static NvGpuTri *read_triangles(const uint8_t *bytes,unsigned count,unsigned stride){
 NvGpuTri *out=calloc(count,sizeof *out);assert(out);
 for(unsigned i=0;i<count;i++){
  const uint8_t *src=bytes+(size_t)i*stride;
  if(stride==sizeof(NvGpuTri))memcpy(out+i,src,sizeof *out);
  else {
   for(unsigned v=0;v<3;v++)memcpy(&out[i].v[v],src+v*48,48);
   memcpy(&out[i].iw,src+144,sizeof(NvGpuTri)-offsetof(NvGpuTri,iw));
  }
 }
 return out;
}
int main(int argc,char**argv){
 assert(argc==2);FILE*f=fopen(argv[1],"rb");assert(f);uint32_t header[4];read_exact(f,header,sizeof header);
 assert(header[0]==0x51475235u && header[2]==sizeof(NvGpuState));
 assert(sizeof(NvGpuTri)==688 && ((header[1]==1 && header[3]==688) || (header[1]==2 && header[3]==448)));
 uint32_t*color=NULL;float*depth=NULL;NvGpuState target={0};unsigned batches=0,checks=0,frames=0,mismatches=0;
 uint32_t trace_initial[1024];float trace_initial_z[1024];
 NvGpuState trace_states[2];NvGpuTri trace_tri[8];int trace_ready=0;
 uint32_t record[2];
 while(fread(record,sizeof record,1,f)==1){
  assert(record[1]<=192u*1024u*1024u);void*raw=malloc(record[1]);assert(raw);read_exact(f,raw,record[1]);
  uint32_t*h=raw;
  if(record[0]==1){
   assert(record[1]>=sizeof target);memcpy(&target,raw,sizeof target);
   size_t count=(size_t)target.width*target.height;assert(count && count<=1024u*1024u && record[1]==sizeof target+count*8);
   free(color);free(depth);color=malloc(count*4);depth=malloc(count*4);assert(color && depth);
   memcpy(color,(uint8_t*)raw+sizeof target,count*4);memcpy(depth,(uint8_t*)raw+sizeof target+count*4,count*4);
   if(count==1024){memcpy(trace_initial,color,sizeof trace_initial);memcpy(trace_initial_z,depth,sizeof trace_initial_z);}
  }else if(record[0]==2){
   assert(record[1]>=12 && h[0]<128 && record[1]==12+(size_t)h[2]*4);
   free(texture[h[0]]);texture[h[0]]=malloc((size_t)h[2]*4);assert(texture[h[0]]);
   texture_count[h[0]]=h[2];memcpy(texture[h[0]],h+3,(size_t)h[2]*4);
  }else if(record[0]==3){
   assert(color && record[1]>=12 && h[1]<=NV_GPU_MAX_STATES && h[2]<=NV_GPU_MAX_TRIANGLES);
   size_t states=(size_t)h[1]*sizeof(NvGpuState),tris=(size_t)h[2]*header[3];
   assert(record[1]==12+states+tris);NvGpuState*s=(NvGpuState*)(h+3);NvGpuTri*t=read_triangles((uint8_t*)s+states,h[2],header[3]);
   trace_ready=(h[0]&1) && h[1]==2 && h[2]==8 && target.width==32 && target.height==32;
   if(trace_ready){memcpy(trace_states,s,sizeof trace_states);memcpy(trace_tri,t,sizeof trace_tri);}
   for(unsigned i=0;i<h[2];i++){
    assert(t[i].state_index<h[1]);NvGpuState state=s[t[i].state_index];assert(state.width==target.width && state.height==target.height);
    for(int y=t[i].miny;y<t[i].maxy;y++)for(int x=t[i].minx;x<t[i].maxx;x++){
     assert(x>=0 && y>=0 && x<(int)target.width && y<(int)target.height);unsigned at=(unsigned)y*target.width+x;
     NvGpuPixel p=gpu_pixel(state,t[i],x,y,color[at],depth[at]);color[at]=p.color;depth[at]=p.depth;
    }
   }++batches;free(t);
  }else if(record[0]==4){
   assert(color && h[0]==target.width && h[1]==target.height);size_t n=(size_t)h[0]*h[1];assert(record[1]==8+n*8);
   uint32_t*actual=h+2;float*actual_depth=(float*)(actual+n);unsigned colors=0,zs=0;
   for(size_t i=0;i<n;i++){colors+=color[i]!=actual[i];zs+=depth[i]!=actual_depth[i];}
   ++checks;if(colors || zs){
    ++mismatches;fprintf(stderr,"MISMATCH completion=%u color_pixels=%u depth_pixels=%u\n",checks,colors,zs);
    if(mismatches<=4){char path[256];snprintf(path,sizeof path,"capture-%u-reference.bmp",checks);nv2a_frame_write_bmp(path,color,target.width,target.height);
     snprintf(path,sizeof path,"capture-%u-hardware.bmp",checks);nv2a_frame_write_bmp(path,actual,target.width,target.height);}
   }
  }else if(record[0]==5)++frames;
  else if(record[0]==6){
   assert(record[1]>=12);
   assert(trace_ready && h[0]==32 && h[1]==32 && h[2]==8 && record[1]==12+8*1024*16);
   const uint32_t*trace=h+3;unsigned ordering=0,arithmetic=0,missing=0,shown=0;
   for(unsigned at=0;at<1024;at++){
    uint32_t previous=trace_initial[at],previous_z;memcpy(&previous_z,trace_initial_z+at,4);
    for(unsigned i=0;i<8;i++){
     const uint32_t*q=trace+(i*1024+at)*4;
     if(!(q[0]|q[1]|q[2]|q[3])){NvGpuTri t=trace_tri[i];if(gpu_bary(trace_states[t.state_index],t,at%32,at/32).covered)++missing;continue;}
     int wrong_order=q[0]!=previous || q[1]!=previous_z;
     float z;memcpy(&z,q+1,4);NvGpuTri t=trace_tri[i];
     NvGpuPixel p=gpu_pixel(trace_states[t.state_index],t,at%32,at/32,q[0],z);
     uint32_t expected_z;memcpy(&expected_z,&p.depth,4);int wrong_math=expected_z!=q[3];
     for(unsigned shift=0;shift<32;shift+=8){int a=(p.color>>shift)&255,b=(q[2]>>shift)&255;wrong_math|=abs(a-b)>1;}
     ordering+=wrong_order;arithmetic+=wrong_math;
     if((wrong_order || wrong_math) && shown++<8)
      printf("TRACE pixel=%u,%u primitive=%u order=%d arithmetic=%d before=%08X/%08X after=%08X/%08X expected_after=%08X/%08X\n",
       at%32,at/32,i,wrong_order,wrong_math,q[0],q[1],q[2],q[3],p.color,expected_z);
     previous=q[2];previous_z=q[3];
    }
   }
   printf("Native transaction trace: %u nonsequential inputs, %u arithmetic differences (>1 color component or exact depth), %u missing covered pixels\n",ordering,arithmetic,missing);
  }
  else if(record[0]==7){assert(record[1]==4 && h[0]==0);}
  else if(record[0]==8){
   assert(color && record[1]==36 && h[0]==target.width && h[1]==target.height);
   assert(h[2]<h[4] && h[3]<h[5] && h[4]<=h[0] && h[5]<=h[1] && h[6] && !(h[6]&~3u));
   for(unsigned y=h[3];y<h[5];y++)for(unsigned x=h[2];x<h[4];x++){
    unsigned at=y*h[0]+x;if(h[6]&1)color[at]=h[7];if(h[6]&2)memcpy(depth+at,h+8,4);
   }
  }
  else {fprintf(stderr,"Unknown capture record %u\n",record[0]);return 2;}
  free(raw);
 }
 fclose(f);printf("Capture comparison: %u batches, %u GPU completions, %u frames, %u mismatched completions\n",batches,checks,frames,mismatches);
 return mismatches?1:0;
}
'''
def replay(path):
 with tempfile.TemporaryDirectory(prefix='qpr2-gpu-replay-') as d:
  p=Path(d);(p/'replay.c').write_text(source)
  subprocess.run(['gcc','-std=c11','-O2','-ffp-contract=off','-Werror=implicit-function-declaration',
   '-I'+str(root/'third_party/xboxrecomp/src/kernel'),str(p/'replay.c'),'-lm','-o',str(p/'replay')],check=True)
  return subprocess.run([str(p/'replay'),str(Path(path).resolve())],timeout=600).returncode
if __name__=='__main__':
 if len(sys.argv)!=2:raise SystemExit(__doc__)
 raise SystemExit(replay(sys.argv[1]))
