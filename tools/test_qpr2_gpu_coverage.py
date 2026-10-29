"""Independent candidate containment checks; no HLSL compilation or GPU emulation."""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1]
source=r'''
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "nv2a_gpu_types.h"
#include "nv2a_raster_rules.h"
static uint32_t gpu_load_texel(NvGpuTexture t,int s,int f,int x,int y){(void)t;(void)s;(void)f;(void)x;(void)y;return 0xFFFFFFFFu;}
#include "nv2a_gpu_pixel.inc"
#include "nv2a_gpu_shade.inc"
#include "qpr2_gpu_coverage_fixture.h"
static unsigned seed=0x91576234;
static unsigned next(void){seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed;}
int main(void){
 unsigned covered=0,thin_covered=0,checks=0;NvGpuState state={0};state.width=640;state.height=480;
 for(unsigned test=0;test<12000;test++){
  NvGpuTri t={0},mapped;int cx=1+(int)(next()%638),cy=1+(int)(next()%478);
  for(unsigned k=0;k<3;k++){t.v[k].pos[0]=(next()%655361)/1024.0f;t.v[k].pos[1]=(next()%491521)/1024.0f;}
  if(test%3==0){float p[3][2]={{cx+0.4999f,cy+0.4999f},{cx+0.5001f,cy+0.4999f},{cx+0.5f,cy+0.5001f}};for(unsigned k=0;k<3;k++){t.v[k].pos[0]=p[k][0];t.v[k].pos[1]=p[k][1];}}
  else if(test%3==1){t.v[0].pos[0]=1.5f;t.v[0].pos[1]=cy+0.4999f;t.v[1].pos[0]=638.5f;t.v[1].pos[1]=cy+0.5001f;t.v[2].pos[0]=638.5f;t.v[2].pos[1]=cy+0.4999f;}
  if(test&1){Nv2aVshOutput swap=t.v[1];t.v[1]=t.v[2];t.v[2]=swap;}
  const float *a=t.v[0].pos,*b=t.v[1].pos,*c=t.v[2].pos;
  t.area=(b[0]-a[0])*(c[1]-a[1])-(b[1]-a[1])*(c[0]-a[0]);assert(t.area!=0);t.inv_area=1/t.area;t.maxx=640;t.maxy=480;
  for(unsigned k=0;k<3;k++)t.inclusive[k]=nv2a_edge_inclusive(t.v[k].pos,t.v[(k+1)%3].pos,t.area);
  mapped=t;
  for(unsigned k=0;k<3;k++){float clip_x=t.v[k].pos[0]*2.0f/640.0f-1.0f,clip_y=1.0f-t.v[k].pos[1]*2.0f/480.0f;mapped.v[k].pos[0]=(clip_x+1.0f)*320.0f;mapped.v[k].pos[1]=(1.0f-clip_y)*240.0f;}
  for(unsigned sample=0;sample<80;sample++){
   int x=sample<16?cx+(int)(sample%4)-2:(int)(next()%640),y=sample<16?cy+(int)(sample/4)-2:(int)(next()%480);
   if(x<0 || y<0 || x>=640 || y>=480)continue;++checks;
   if(gpu_bary(state,t,x,y).covered){assert(coverage_candidate(&t,x,y) && coverage_candidate(&mapped,x,y));++covered;if(test%3!=2)++thin_covered;}
  }
 }
 assert(covered>10000 && thin_covered>5000);
 printf("PASS: %u sample checks, %u covered including %u thin/tiny; independent SAT and float viewport round trips retain exact covered samples in both windings\n",checks,covered,thin_covered);
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-coverage-') as d:
 p=Path(d);(p/'fixture.c').write_text(source)
 subprocess.run(['gcc','-std=c11','-O2','-ffp-contract=off','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-I'+str(root/'third_party/xboxrecomp/src/kernel'),'-I'+str(root/'tools'),str(p/'fixture.c'),'-lm','-o',str(p/'fixture')],check=True)
 subprocess.run([str(p/'fixture')],check=True,timeout=60)
