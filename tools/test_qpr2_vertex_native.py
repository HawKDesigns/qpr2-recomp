"""Differential-test all asset specializations against the unchanged interpreter.

Host CPU throughput is reported separately from race FPS. The fixture includes
the production translation unit so the reference arithmetic is independent of
the Python generator's bitfield decoding and SSA register assignment.
"""
from pathlib import Path
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
source = root / 'third_party/xboxrecomp/src/kernel/nv2a_vsh_interp.c'
fixture = r'''
#include <assert.h>
#include <time.h>
#include "nv2a_vsh_interp.c"
static uint32_t rng=0x41424344;
static uint32_t next(void){rng=rng*1664525u+1013904223u;return rng;}
static float sample(unsigned edge){
 if(!edge)return ((int)(next()%16385)-8192)/2048.0f;
 const uint32_t special[]={0,0x80000000,0x3f800000,0xbf800000,0x7f800000,
  0xff800000,0x7fc01234,0xffc05678,1,0x80000001,0x7f7fffff,0xff7fffff};
 uint32_t bits=special[next()%(sizeof special/sizeof special[0])];float f;memcpy(&f,&bits,4);return f;
}
static void check(const Nv2aVshOutput*a,const Nv2aVshOutput*b,unsigned variant,unsigned trial){
 const float*x=(const float*)a,*y=(const float*)b;
 for(unsigned j=0;j<sizeof(*a)/sizeof(float);++j){
  uint32_t p,q;memcpy(&p,x+j,4);memcpy(&q,y+j,4);
  if(p!=q && !(isnan(x[j])&&isnan(y[j]))){
   fprintf(stderr,"variant=%u trial=%u component=%u %08x != %08x (%.9g / %.9g)\n",variant,trial,j,p,q,x[j],y[j]);abort();
  }
 }
}
static void install(const NvVshCompiled*p,unsigned start){
 const uint32_t zero[4]={0};
 for(unsigned j=0;j<NV2A_VSH_SLOTS;++j)nv2a_vsh_set_instruction(j,zero);
 nv2a_vsh_set_load_slot(start);
 for(unsigned j=0;j<p->count;++j)for(unsigned k=0;k<4;++k)nv2a_vsh_program_word(p->words[j][k]);
 nv2a_vsh_set_start_slot(start);
}
static double ms(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec*1000.0+t.tv_nsec/1e6;}
static volatile float sink;
int main(int argc,char**argv){
 if(argc>1)setenv("RECOMP_VERTEX_NATIVE","0",1);
 unsigned count=sizeof nv_vsh_compiled/sizeof nv_vsh_compiled[0];assert(count==180);
 float in[16][4];Nv2aVshOutput a,b;
 unsigned checked=0;
 for(unsigned v=0;v<count;++v){
  const NvVshCompiled*p=&nv_vsh_compiled[v];install(p,7);
  assert(nv2a_vsh_input_mask()==(argc>1?0xffffu:p->inputs));
  for(unsigned trial=0;trial<96;++trial){
   for(unsigned j=0;j<192;++j){float c[4];for(unsigned k=0;k<4;++k)c[k]=sample(trial>=64);nv2a_vsh_set_constant(j,c);}
   for(unsigned j=0;j<16;++j)for(unsigned k=0;k<4;++k)in[j][k]=sample(trial>=64);
   assert(nv2a_vsh_run_interpreted(in,&a));assert(nv2a_vsh_run(in,&b));check(&a,&b,v,trial);++checked;
   /* Skipped input attributes must never affect any native output. */
   for(unsigned j=0;j<16;++j)if(!(p->inputs&(1u<<j)))for(unsigned k=0;k<4;++k)in[j][k]=NAN;
   assert(nv2a_vsh_run(in,&b));check(&a,&b,v,trial);
  }
  nv2a_vsh_set_cxt_write(1);assert(nv2a_vsh_input_mask()==0xffffu);
  uint64_t before=s_interpreted_runs;assert(nv2a_vsh_run(in,&b));assert(s_interpreted_runs==before+1);
  assert(nv2a_vsh_run_interpreted(in,&a));check(&a,&b,v,99);nv2a_vsh_set_cxt_write(0);
  /* Every dword, including reserved bits, participates in exact matching. */
  nv2a_vsh_set_load_slot(7);nv2a_vsh_program_word(p->words[0][0]^1u);
  assert(!nv2a_vsh_resolve_native());assert(nv2a_vsh_run(in,&b));
  assert(nv2a_vsh_run_interpreted(in,&a));check(&a,&b,v,100);
  nv2a_vsh_set_load_slot(7);nv2a_vsh_program_word(p->words[0][0]);
  assert((nv2a_vsh_resolve_native()!=NULL)==(argc==1));
  /* Partial instruction changes are visible immediately, before 4 words. */
  nv2a_vsh_program_word(p->words[0][1]^(1u<<13));assert(!nv2a_vsh_resolve_native());
  assert(nv2a_vsh_run(in,&b));assert(nv2a_vsh_run_interpreted(in,&a));check(&a,&b,v,101);
  nv2a_vsh_set_instruction(7,p->words[0]);
  /* Live component and stream uploads must not reuse stale constant values. */
  nv2a_vsh_constant_component(58,0,0x42000000u);
  nv2a_vsh_set_constant_load(96);nv2a_vsh_constant_word(0x3f000000u);
  assert(nv2a_vsh_run(in,&b));assert(nv2a_vsh_run_interpreted(in,&a));check(&a,&b,v,102);
 }
 assert(s_interpreted_runs>0);if(argc==1)assert(s_native_runs==count*193);
 printf("PASS: %u programs, %u random/IEEE-edge trials; exact finite/signed-zero outputs; NaN classification; masks; live constants; partial/reserved writes; context fallback; relocated starts; disabled=%u\n",count,checked,argc>1);
 /* Missing FINAL and invalid start preserve no-program behavior. */
 const uint32_t zero[4]={0};nv2a_vsh_set_instruction(135,zero);nv2a_vsh_set_start_slot(135);
 assert(!nv2a_vsh_run(in,&b));nv2a_vsh_set_start_slot(999);assert(!nv2a_vsh_run(in,&b));
 if(argc>1)return 0;
 double total[2]={0};
 for(unsigned v=0;v<count;v+=9){
  install(&nv_vsh_compiled[v],0);
  for(unsigned j=0;j<192;++j)for(unsigned k=0;k<4;++k)s_const[j][k]=sample(0);
  for(unsigned j=0;j<16;++j)for(unsigned k=0;k<4;++k)in[j][k]=sample(0);
  /* Same inputs/iterations, warm both paths, alternate timing order. */
  for(unsigned repeat=0;repeat<4;++repeat)for(unsigned path=0;path<2;++path){
   unsigned native=(path+repeat)%2;double begin=ms();
   for(unsigned j=0;j<5000;++j){in[0][0]+=.0000001f;
    if(native)nv2a_vsh_run(in,&b);else nv2a_vsh_run_interpreted(in,&b);sink=b.pos[0];}
   total[native]+=ms()-begin;
  }
 }
 printf("CPU shader microbenchmark (NOT race FPS): reference_ms=%.3f native_ms=%.3f speedup=%.2fx (20 evenly spaced programs, 400000 vertices/path)\n",total[0],total[1],total[0]/total[1]);
 nv2a_vsh_report();
}
'''

if __name__ == '__main__':
    with tempfile.TemporaryDirectory(prefix='qpr2-vertex-native-') as folder:
        p = Path(folder)
        (p / 'fixture.c').write_text(fixture)
        for sanitized in (False, True):
            exe = p / ('checked' if sanitized else 'benchmark')
            subprocess.run(['gcc', '-std=gnu11', '-O2', '-ffp-contract=off',
                *(['-fsanitize=undefined,float-cast-overflow', '-fno-sanitize-recover=all'] if sanitized else []),
                '-I' + str(source.parent), str(p / 'fixture.c'), '-lm', '-o', str(exe)], check=True)
            subprocess.run([str(exe)], check=True, timeout=120)
            subprocess.run([str(exe), 'disabled'], check=True, timeout=120)
