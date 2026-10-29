"""Retail preview updater and effect enqueue regression; path sampler is a fixture.

Compiles extracted production bodies in a temporary directory. Never part of
the game build or launch script, and does not change game files or allocations.
"""
from pathlib import Path
import re
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
bodies = {}
for p in (root / 'src/recomp/gen').glob('recomp_*.c'):
    s = p.read_text()
    for m in re.finditer(r'void sub_([0-9A-F]{8})\(void\)\n\{', s):
        end = s.find('\n/**', m.start())
        bodies[int(m[1], 16)] = s[m.start():end if end != -1 else len(s)]

stubs = (root / 'src/recomp/gen/recomp_stubs_unresolved.c').read_text()
wrapper = re.search(r'void sub_0007F9F0\(void\) \{ qpr2_recovered_0007F9F0\(\); \}', stubs)
assert wrapper, 'ordinary tail call must reach the recovered preview body'
assert re.search(r'void sub_0007FEF0\(void\) \{ qpr2_recovered_0007FEF0\(\); \}', stubs)
manual = (root / 'src/recomp_manual.c').read_text()
for a in (0x7f9f0, 0x7fef0, 0x119360):
    assert f'if (xbox_va == 0x{a:08X}u) return qpr2_recovered_{a:08X};' in manual

# This live action/effect call captures its fallback ESP AFTER pushing all five
# arguments. Retaining that old fallback reproduces the 20-byte stack error.
caller = bodies[0xa71a0]
start = caller.index('loc_000A74FD: ;')
end = caller.index('loc_000A7514: ;', start)
callsite = caller[start:end]
assert callsite.count('PUSH32') == 6
assert callsite.index('_icall_esp = g_esp') > callsite.index('PUSH32(esp, ebp)')

shim = r'''
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define RECOMP_GENERATED_CODE
#include "recomp/gen/recomp_types.h"
#include "recomp/gen/recomp_funcs.h"
ptrdiff_t g_xbox_mem_offset;
uint32_t g_xbox_code_lo=0x10000,g_xbox_code_hi=0x1c0000;
RECOMP_TLS uint32_t g_eax,g_ecx,g_edx,g_esp,g_ebx,g_esi,g_edi,g_ebp,g_seh_ebp;
RECOMP_TLS volatile uint32_t g_recomp_pc;
RECOMP_TLS int g_df,g_fp_top,g_fp_cmp;
RECOMP_TLS uint16_t g_fp_control_word,g_fp_cc;
RECOMP_TLS double g_fp_stack[8];
volatile uint32_t g_icall_trace[16],g_icall_trace_idx;
volatile uint64_t g_icall_count;
void qpr2_recovered_0007F9F0(void),qpr2_recovered_0007FEF0(void),qpr2_recovered_00119360(void);
void sub_0007FEF0(void){qpr2_recovered_0007FEF0();}
static int old_preview,old_queue,missing;
static float last_sample;
static unsigned samples;
static void sample(void) {
 uint32_t out=MEM32(g_esp+4);float t=MEMF(g_esp+8);
 MEMF(out)=100+t;MEMF(out+4)=200+2*t;MEMF(out+8)=300+3*t;
 last_sample=t;++samples;g_eax=out;g_esp+=12;
}
static void duration(void) {
 g_fp_top=(g_fp_top+7)&7;g_fp_stack[g_fp_top]=3;g_esp+=4;
}
recomp_func_t recomp_lookup_manual(uint32_t va){
 if(va==0x119360)return old_queue?NULL:qpr2_recovered_00119360;
 if(va==0x7f9f0)return qpr2_recovered_0007F9F0;
 return NULL;
}
recomp_func_t recomp_lookup(uint32_t va){
 if(va==0x110000)return sample;
 if(va==0x110010)return duration;
 return NULL;
}
recomp_func_t recomp_lookup_kernel(uint32_t va){(void)va;return NULL;}
void recomp_icall_not_code_log(uint32_t va){(void)va;assert(0);}
void recomp_icall_fail_log(uint32_t va){assert(va==0x119360 && old_queue);++missing;}
void sub_00080130(void){assert(0);}
'''
wrapper = wrapper[0].replace(' qpr2_recovered_0007F9F0();',
                            ' if(old_preview)g_esp+=4;else qpr2_recovered_0007F9F0();')
callsite = 'static void actual_effect_callsite(void) { uint32_t ebp=g_ebp;\n' + callsite + '\n}\n'
fixture = r'''
static void saved(uint32_t sp){assert(g_esp==sp && g_ebx==91 && g_esi==92 && g_edi==93 && g_fp_top==0);}
static void enter(uint32_t object){g_ecx=object;g_esp=0x280000;MEM32(g_esp)=0x98765;
 g_ebx=91;g_esi=92;g_edi=93;g_ebp=g_seh_ebp=94;g_fp_top=0;}
static void queue_setup(void){
 memset((void*)XBOX_PTR(0x200000),0,0x1000);
 memset((void*)XBOX_PTR(0x210000),0,0x1000);
 MEM32(0x200000)=0x220000;MEM32(0x22001c)=0x119360;
 MEM32(0x200004)=4;MEM32(0x200030)=3;MEM32(0x20008c)=0x210000;
 MEM32(0x230008)=0x200000;
}
static void direct_enqueue(int kind){
 enter(0x200000);MEM32(g_esp+4)=kind;MEM32(g_esp+8)=0x240000;MEM32(g_esp+12)=0x240010;
 MEM32(g_esp+16)=0x123;MEM32(g_esp+20)=0x456;
 qpr2_recovered_00119360();saved(0x280018);
}
int main(void){
 void *ram=calloc(1,0x300000);assert(ram);g_xbox_mem_offset=(ptrdiff_t)ram;
 MEMF(0x1c91d0)=1;MEMF(0x1bbc74)=1;MEMF(0x1ae140)=10;
 MEM32(0x220008)=0x110000;MEM32(0x220004)=0x110010;
 uint32_t node=0x200000,camera=0x201000;
 MEM32(node+4)=camera;MEM32(node+12)=0x202000;MEM32(0x202000)=0x220000;
 MEMF(node+0x10)=10;MEMF(node+0x7c)=2;
 for(unsigned i=0;i<3;i++){
  MEMF(node+0x4c+i*4)=10+i;MEMF(node+0x58+i*4)=20+i;
  MEMF(node+0x64+i*4)=3+i;MEMF(node+0x70+i*4)=4+i;
 }
 old_preview=1;enter(node);MEMF(g_esp+4)=0.1f;sub_000800B0();
 assert(g_esp==0x280004 && MEMF(camera+0x10)==0 && MEM32(node+0x88)==0);
 old_preview=0;MEMF(node+0x14)=0;float previous=0;
 for(unsigned frame=0;frame<12;frame++){
  float t=MEMF(node+0x80);enter(node);MEMF(g_esp+4)=0.1f;sub_000800B0();saved(0x280008);
  float u=t+MEMF(node+0x14)/2*0.1f;
  for(unsigned i=0;i<3;i++){
   float expected=10+i+(20+i)*u+(3+i)*u*u+(4+i)*u*u*u;
   assert(fabsf(MEMF(camera+0x10+i*4)-expected)<0.0001f);
  }
  assert(MEMF(camera+0x10)>previous);previous=MEMF(camera+0x10);
 }
 puts("PASS: actual preview updater -> production direct wrapper -> recovered cubic path advances position; old stub stays fixed and leaves dt on stack");
 MEMF(node+0x80)=0.99f;MEMF(node+0x14)=1;enter(node);MEMF(g_esp+4)=0.1f;
 sub_000800B0();saved(0x280008);
 assert(MEM32(node+0x88)==1 && samples==1 && fabsf(last_sample-0.09f)<0.00001f);
 assert(fabsf(MEMF(camera+0x10)-(100+last_sample))<0.00001f);
 enter(node);MEMF(g_esp+4)=0.1f;sub_000800B0();saved(0x280008);
 assert(samples==2 && fabsf(last_sample-0.21f)<0.00001f);
 MEMF(node+0x14)=10;MEMF(node+0x84)=2.9f;enter(node);MEMF(g_esp+4)=0.1f;
 sub_000800B0();saved(0x280008);assert(MEM32(camera+0x60)==1 && last_sample==3);
 puts("PASS: path boundary changes stage, samples residual time, advances next segment and signals completion; saved registers/FPU/ESP balanced");
 MEMF(0x240000)=1;MEMF(0x240004)=2;MEMF(0x240008)=3;
 MEMF(0x240010)=4;MEMF(0x240014)=5;MEMF(0x240018)=6;
 queue_setup();
 for(unsigned n=0;n<6;n++){
  direct_enqueue(2);unsigned row=n%4;
  assert(MEM32(0x210000+row*0x38+0x34)==2);
  for(unsigned i=0;i<3;i++)assert(MEMF(0x210000+row*0x38+i*4)==1+i);
  for(unsigned i=0;i<3;i++)assert(MEMF(0x210000+row*0x38+12+i*4)==4+i);
  assert(MEM32(0x210000+row*0x38+0x18)==0x123 && MEM32(0x210000+row*0x38+0x1c)==0x456);
 }
 assert(MEM32(0x200094)==2 && MEM32(0x200090)==3 && MEM32(0x200098)==3);
 uint32_t head=MEM32(0x200094),count=MEM32(0x200098);
 direct_enqueue(-1);direct_enqueue(3);assert(MEM32(0x200094)==head && MEM32(0x200098)==count);
 puts("PASS: actual effect enqueue + retail wrap helper copy both vectors/scalars, wrap head, evict oldest at capacity, reject invalid kinds, preserve ABI");
 for(int old=1;old>=0;old--){
  queue_setup();old_queue=old;enter(0);g_edx=0x230000;g_ebp=1;
  for(unsigned i=0;i<3;i++){MEMF(g_esp+0x1c+i*4)=11+i;MEMF(g_esp+0x40+i*4)=21+i;}
  actual_effect_callsite();
  if(old){assert(missing==1 && g_esp==0x280000-20 && MEM32(0x200094)==0);}
  else{saved(0x280000);assert(MEM32(0x200094)==1 && MEM32(0x210034)==1);}
 }
 puts("PASS: actual A7511 action/effect callsite reproduces 20-byte fallback stack error; recovered dispatch queues the effect and restores caller ESP");
 free(ram);return 0;
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-preview-effects-') as d:
    source = Path(d) / 'fixture.c'
    source.write_text(shim + '\n' + wrapper + '\n' + '\n'.join(
        bodies[a] for a in (0x13760, 0x28ef0, 0x800b0)) + '\n' + callsite + fixture)
    exe = Path(d) / 'fixture'
    subprocess.run(['gcc', '-O1', '-g', '-fsanitize=undefined,float-cast-overflow',
                    '-fno-sanitize-recover=all', '-I' + str(root / 'src'), str(source),
                    str(root / 'src/qpr2_v43_callbacks.c'), '-lm', '-o', str(exe)], check=True)
    subprocess.run([str(exe)], check=True)
