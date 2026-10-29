"""Block decoder versus the independent scalar DXT reference, all texels."""
from pathlib import Path
import subprocess
import tempfile
root=Path(__file__).resolve().parents[1]
fixture=r'''
#include <assert.h>
#include <stdio.h>
#include "d3d/d3d8_swizzle.h"
static uint32_t seed=0x9e3779b9;
static uint32_t random_word(void){seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed;}
int main(void){
 uint8_t block[16];uint32_t pixels[16],old;
 uint32_t formats[]={0x0c,0x0e,0x0f};
 for(unsigned f=0;f<3;f++)for(unsigned n=0;n<40000;n++){
  for(unsigned i=0;i<16;i++)block[i]=(uint8_t)random_word();
  /* Exercise equal endpoints, endpoint order, explicit alpha, and DXT5's
   * six/eight-entry alpha palettes independently of random samples. */
  unsigned color=f==0?0:8;
  if(n%16==0){block[color]=block[color+2];block[color+1]=block[color+3];}
  if(n%16==1)block[0]=block[1];
  if(n%16==2){block[0]=0;block[1]=255;}
  if(n%16==3){block[0]=255;block[1]=0;}
  assert(d3d8_dxt_decode_block(block,formats[f],pixels));
  for(unsigned i=0;i<16;i++){
   assert(d3d8_dxt_decode_texel(block,formats[f],i%4,i/4,4,&old));
   assert(old==pixels[i]);
  }
 }
 for(unsigned i=0;i<16;i++)pixels[i]=0xabcdef12;
 assert(!d3d8_dxt_decode_block(block,0x24,pixels));
 for(unsigned i=0;i<16;i++)assert(pixels[i]==0xabcdef12);
 puts("PASS: 1,920,000 DXT1/3/5 texels match independent scalar decoder exactly, including transparent/equal endpoint and both DXT5 alpha modes");
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-dxt-blocks-') as d:
    src=Path(d)/'fixture.c';exe=Path(d)/'fixture';src.write_text(fixture)
    subprocess.run(['gcc','-O2','-fsanitize=undefined','-fno-sanitize-recover=all',
        '-I'+str(root/'third_party/xboxrecomp/src'),str(src),'-o',str(exe)],check=True)
    subprocess.run([str(exe)],check=True,timeout=15)
