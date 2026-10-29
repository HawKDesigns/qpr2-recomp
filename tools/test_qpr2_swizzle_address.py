"""Differential address proof against mask/deposit and a CPU microbenchmark.
The benchmark isolates addressing; it is not a Windows gameplay FPS estimate.
"""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1]
source=r'''
#include "d3d/d3d8_swizzle.h"
#include <assert.h>
#include <stdio.h>
#include <time.h>
static uint32_t reference(unsigned x,unsigned y,unsigned w,unsigned h){
 unsigned mx,my;xbox_swizzle_masks(w,h,&mx,&my);
 return swizzle_deposit(x,mx)|swizzle_deposit(y,my);
}
static uint32_t seed=123;
static unsigned random_bits(void){seed=1664525*seed+1013904223;return seed;}
static volatile unsigned dimension=512;
int main(void){
 uint64_t trials=0;
 for(unsigned w=1;w<=1024;w*=2)for(unsigned h=1;h<=1024;h*=2)
  for(unsigned y=0;y<h;y++)for(unsigned x=0;x<w;x++){
   assert(swizzle_offset(x,y,w,h)==reference(x,y,w,h));++trials;
  }
 for(unsigned wx=0;wx<=15;wx++)for(unsigned hy=0;hy<=15;hy++)
  for(unsigned i=0;i<2000;i++){
   unsigned w=1u<<wx,h=1u<<hy,x=random_bits(),y=random_bits();
   assert(swizzle_offset(x,y,w,h)==reference(x,y,w,h));++trials;
  }
 /* Distinct work/checksums keep both loops observable at optimized builds. */
 uint64_t sum[2]={0};double seconds[2];
 for(unsigned mode=0;mode<2;mode++){
  unsigned w=dimension,h=dimension;clock_t start=clock();
  for(unsigned pass=0;pass<16;pass++)for(unsigned y=0;y<h;y++)for(unsigned x=0;x<w;x++)
   sum[mode]+=mode?swizzle_offset(x,y,w,h):reference(x,y,w,h);
  seconds[mode]=(double)(clock()-start)/CLOCKS_PER_SEC;
 }
 assert(sum[0]==sum[1]);
 printf("PASS: %llu exact square/rectangular addresses including 1D and 32768 dimensions\n",(unsigned long long)trials);
 printf("CPU address microbenchmark: mask/deposit_ms=%.3f fixed_cost_ms=%.3f speed_ratio=%.3f (not gameplay FPS)\n",seconds[0]*1000,seconds[1]*1000,seconds[0]/seconds[1]);
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-swizzle-') as d:
 p=Path(d);(p/'test.c').write_text(source)
 subprocess.run(['gcc','-std=c11','-O2','-fsanitize=undefined','-fno-sanitize-recover=all','-I'+str(root/'third_party/xboxrecomp/src'),str(p/'test.c'),'-o',str(p/'test')],check=True)
 print('UBSan differential proof / instrumented benchmark:',flush=True)
 subprocess.run([str(p/'test')],check=True,timeout=60)
 subprocess.run(['gcc','-std=c11','-O2','-I'+str(root/'third_party/xboxrecomp/src'),str(p/'test.c'),'-o',str(p/'optimized')],check=True)
 print('Optimized CPU benchmark (no sanitizer):',flush=True)
 subprocess.run([str(p/'optimized')],check=True,timeout=60)
