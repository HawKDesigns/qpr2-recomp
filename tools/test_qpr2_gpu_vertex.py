"""Independent differential test of geometry-CS scalar semantics against NV2A CPU interpreter.
No HLSL compiler is emulated here. Windows startup separately compiles and compares GPU results.
"""
from pathlib import Path
import subprocess,tempfile
import test_qpr2_vertex_native as native
root=native.root;k=native.source.parent
shared=r'''
#include "nv2a_gpu_raw_types.h"
static const uint8_t*test_bytes;static NvRawAttr test_attrs[16];
static NvRawParams test_params;static const uint32_t*test_program;
static uint32_t nv_raw_data(uint32_t at){uint32_t v;memcpy(&v,test_bytes+at*4,4);return v;}
static NvRawAttr nv_raw_attr(uint32_t draw,uint32_t at){(void)draw;return test_attrs[at];}
static float nv_raw_asfloat(uint32_t v){float f;memcpy(&f,&v,4);return f;}
static float nv_raw_constant(uint32_t draw,int c,int k){(void)draw;return s_const[c][k];}
static NvRawParams nv_raw_params(uint32_t draw){(void)draw;return test_params;}
static uint32_t nv_raw_program(uint32_t at){return test_program[at];}
#include "nv2a_gpu_vsh.inc"
'''
fixture=native.fixture.replace('int main(int argc,char**argv){',shared+'\nint main(int argc,char**argv){')
fixture=fixture.replace('assert(nv2a_vsh_run_interpreted(in,&a));assert(nv2a_vsh_run(in,&b));check(&a,&b,v,trial);++checked;',r'''
   assert(nv2a_vsh_run_interpreted(in,&a));
   test_bytes=(const uint8_t*)in;test_program=&p->words[0][0];test_params.count=p->count;
   for(unsigned j=0;j<16;j++)test_attrs[j]=(NvRawAttr){j*16,16,2,4};
   b=nv_raw_vertex(0,0);check(&a,&b,v,trial);++checked;
''')
# This fixture compares the new scalar kernel only; remove old-native counter/benchmark assertions.
fixture=fixture.replace('if(argc==1)assert(s_native_runs==count*193);','')
fixture=fixture[:fixture.index(' if(argc>1)return 0;')]+r'''
 /* Unaligned packed and short formats, absent attributes, default W. */
 uint8_t packed[64]={0};test_bytes=packed;
 for(unsigned kind=0;kind<7;kind++)if(kind!=3)for(unsigned size=1;size<=4;size++)for(unsigned off=0;off<4;off++){
  for(unsigned j=0;j<sizeof packed;j++)packed[j]=(uint8_t)(next()>>24);
  test_attrs[0]=(NvRawAttr){off,19,kind,size};NvRawVec v=nv_raw_attribute(0,1,0);const uint8_t*b=packed+off+19;
  float ref[4]={0,0,0,1};uint32_t w;memcpy(&w,b,4);
  for(unsigned j=0;j<size;j++){
   if(kind==2)memcpy(ref+j,b+j*4,4);
   if(kind==4)ref[j]=b[j]/255.0f;
   if(kind==1 || kind==5){int16_t n;memcpy(&n,b+j*2,2);ref[j]=kind==1?n/32767.0f:(float)n;}
  }
  if(kind==0){ref[0]=b[2]/255.0f;ref[1]=b[1]/255.0f;ref[2]=b[0]/255.0f;ref[3]=b[3]/255.0f;}
  if(kind==6){ref[0]=(float)((int32_t)(w<<21)>>21)/1023;ref[1]=(float)((int32_t)(w<<10)>>21)/1023;ref[2]=(float)((int32_t)w>>22)/511;}
  for(unsigned j=0;j<4;j++)assert(v.v[j]==ref[j] || (isnan(v.v[j])&&isnan(ref[j])));
 }
 test_attrs[0].size=0;NvRawVec def=nv_raw_attribute(0,0,0);assert(def.v[0]==0 && def.v[3]==1);
 puts("PASS: GPU scalar vertex interpreter matches independent CPU interpreter for all 180 programs and 17,280 random/IEEE-edge trials; unaligned packed formats and absent inputs");
 return 0;
}
'''
if __name__=='__main__':
 with tempfile.TemporaryDirectory(prefix='qpr2-gpu-vertex-') as d:
  p=Path(d);(p/'fixture.c').write_text(fixture)
  subprocess.run(['gcc','-std=gnu11','-O2','-ffp-contract=off','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-I'+str(k),str(p/'fixture.c'),'-lm','-o',str(p/'fixture')],check=True)
  subprocess.run([str(p/'fixture')],check=True,timeout=120)
