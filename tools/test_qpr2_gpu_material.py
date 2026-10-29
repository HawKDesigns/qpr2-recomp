"""Verify the specialized material algebra and signed wrapping against C reference."""
from pathlib import Path
import subprocess, tempfile
root=Path(__file__).resolve().parents[1]
source=r'''
#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <assert.h>
#include "nv2a_gpu_types.h"
static uint32_t gpu_load_texel(NvGpuTexture t,int st,int face,int x,int y){(void)t;(void)st;(void)face;(void)x;(void)y;return 0xFFFFFFFF;}
#include "nv2a_gpu_pixel.inc"
#include "nv2a_gpu_shade.inc"
static uint32_t seed=0xCC32DB71;
static uint32_t random_word(void){seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed;}
static float value(void){return ((int)(random_word()%65537)-32768)/16384.0f;}
int main(void){
 unsigned checks=0;
 for(unsigned i=0;i<200000;i++){
  Nv2aCombiner rc={0};float a[4],b[4],fog[4],tex[4][4],reference[4],actual[4];
  rc.control=1|(random_word()&0xFFFFFF00u);rc.stage_program=(i&1)?1:2;
  rc.color_icw[0]=0xC8C40000u;rc.color_ocw[0]=(i&2)?0x100C0u:0xC0u;
  rc.alpha_icw[0]=(i&4)?0xD8301010u:0xD8D41010u;rc.alpha_ocw[0]=0xC0u;
  rc.final0=(i&8)?0x130C0300u:0xCu;rc.final1=(i&16)?0x1C80u:0x1C00u;
  rc.final_c0=random_word();rc.final_c1=random_word();
  for(int k=0;k<4;k++){a[k]=value();b[k]=value();fog[k]=value();for(int st=0;st<4;st++)tex[st][k]=value();}
  for(int st=0;st<8;st++){rc.factor0[st]=random_word();rc.factor1[st]=random_word();}
  assert(gpu_rc_simple_kind(rc));gpu_rc_eval(rc,a,b,fog,tex,reference);gpu_rc_simple(rc,a,fog,tex[0],actual);
  for(int k=0;k<4;k++)assert(reference[k]==actual[k]);assert(gpu_pack(reference)==gpu_pack(actual));++checks;
  Nv2aCombiner rejected=rc;rejected.control=(rc.control&~255u)|2;assert(!gpu_rc_simple_kind(rejected));
  rejected=rc;rejected.stage_program|=1u<<5;assert(!gpu_rc_simple_kind(rejected));
  rejected=rc;rejected.color_icw[0]^=1;assert(!gpu_rc_simple_kind(rejected));
  rejected=rc;rejected.color_ocw[0]|=0x4000;assert(!gpu_rc_simple_kind(rejected));
  rejected=rc;rejected.alpha_icw[0]^=1;assert(!gpu_rc_simple_kind(rejected));
  rejected=rc;rejected.alpha_ocw[0]^=1;assert(!gpu_rc_simple_kind(rejected));
  rejected=rc;rejected.final0^=1;assert(!gpu_rc_simple_kind(rejected));
  rejected=rc;rejected.final1^=0x100;assert(!gpu_rc_simple_kind(rejected));
 }
 const unsigned sizes[]={1,2,3,4,5,10,16,31,64,128,256,257,1024};
 for(unsigned n=0;n<sizeof sizes/sizeof sizes[0];n++)for(unsigned i=0;i<10000;i++){
  int c=i==0?INT_MIN:(i==1?INT_MAX:(int)random_word());int64_t expected=((int64_t)c%sizes[n]+sizes[n])%sizes[n];
  assert(gpu_wrap(c,sizes[n],1)==expected);
 }
 printf("PASS: %u exact specialized/general material comparisons, classifier exclusions, 130000 signed/unsigned wrap cases including INT_MIN and non-power-of-two sizes\n",checks);
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-gpu-material-') as d:
    p=Path(d);src=p/'fixture.c';exe=p/'fixture';src.write_text(source)
    for registers in [[],['-DNV_GPU_FIXED_REGISTERS=1']]:
      subprocess.run(['gcc','-std=c11','-O2','-ffp-contract=off','-fsanitize=undefined,float-cast-overflow',
        '-fno-sanitize-recover=all',*registers,'-I'+str(root/'third_party/xboxrecomp/src/kernel'),str(src),'-lm','-o',str(exe)],check=True)
      subprocess.run([str(exe)],check=True,timeout=60)
