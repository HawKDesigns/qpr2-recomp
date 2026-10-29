"""Exact SSE2 readback deinterleave, including tails, unaligned data and aliases."""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1]
source=r'''
#include <assert.h>
#include <stdio.h>
#include "nv2a_gpu_surface_copy.h"
static uint32_t seed=0x73ACB59Du;
static uint32_t next(void){seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed;}
static void reference(const unsigned char *src,unsigned char *c,unsigned char *z,size_t count){
 for(size_t at=0;at<count;at++){if(c)memcpy(c+at*4,src+at*8,4);if(z)memcpy(z+at*4,src+at*8+4,4);}
}
int main(void){
 unsigned char source[1024*8+64],actual[1024*12+256],expected[sizeof actual];unsigned checks=0,vectors=0;
 const unsigned sizes[]={0,1,2,3,4,5,7,8,15,16,17,31,32,33,127,128,129,639,640,641,1023,1024};
 const uint32_t edge[]={0,0x80000000u,0x3F800000u,0x7F800000u,0xFF800000u,0x7FC00001u,0x7FA12345u,0xFFFFFFFFu,1,0x00800000u};
 for(unsigned i=0;i<sizeof source;i++)source[i]=(unsigned char)next();
 for(unsigned i=0;i<sizeof edge/sizeof edge[0];i++)memcpy(source+16+i*4,&edge[i],4);
 for(unsigned n=0;n<sizeof sizes/sizeof sizes[0];n++)for(unsigned align=0;align<16;align++)for(unsigned outputs=0;outputs<4;outputs++)for(unsigned simd=0;simd<2;simd++){
  size_t count=sizes[n];memset(actual,0xA5,sizeof actual);memcpy(expected,actual,sizeof actual);
  unsigned c_at=32+align,z_at=1024*4+96+((align*7)%16);
  reference(source+align,outputs&1?expected+c_at:NULL,outputs&2?expected+z_at:NULL,count);
  int used=nv_gpu_surface_copy(source+align,outputs&1?actual+c_at:NULL,outputs&2?actual+z_at:NULL,count,simd);
  assert(!memcmp(actual,expected,sizeof actual));vectors+=used;++checks;
  assert(used==(NV_GPU_SURFACE_SSE2 && simd && outputs && count>=4));
 }
 /* Overlapping host outputs retain per-pixel color-then-depth order even
  * for shifts that cross SIMD groups, and when both outputs are identical. */
 for(unsigned count=1;count<=128;count++)for(int delta=-21;delta<=21;delta++){
  memset(actual,0xCD,sizeof actual);memcpy(expected,actual,sizeof actual);
  unsigned c_at=64,z_at=(unsigned)(64+delta);
  reference(source+3,expected+c_at,expected+z_at,count);
  int used=nv_gpu_surface_copy(source+3,actual+c_at,actual+z_at,count,1);
  assert(!memcmp(actual,expected,sizeof actual));
  if((unsigned)(delta<0?-delta:delta)<count*4)assert(!used);++checks;
 }
 assert(checks==8320);
 printf("PASS: %u exact byte comparisons, %u SIMD cases; arbitrary depth/color bits, tails, unaligned rows, NULL outputs, aliases and canaries; SSE2=%d\n",checks,vectors,NV_GPU_SURFACE_SSE2);
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-surface-copy-') as folder:
    p=Path(folder);(p/'test.c').write_text(source)
    for mode,flags in [('sse2',[]),('portable',['-DNV_GPU_SURFACE_NO_SSE2=1'])]:
        subprocess.run(['gcc','-std=c11','-O2','-ffp-contract=off','-fstrict-aliasing','-fsanitize=undefined','-fno-sanitize-recover=all',
            *flags,'-I'+str(root/'third_party/xboxrecomp/src/kernel'),str(p/'test.c'),'-o',str(p/mode)],check=True)
        subprocess.run([str(p/mode)],check=True)
