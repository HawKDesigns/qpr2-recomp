"""Retail HUD factories + recovered constructors on reused/poisoned heap.
Resource services are fixtures; base construction and coordinate math are real.
"""
from pathlib import Path
import re,subprocess,tempfile
root=Path(__file__).resolve().parents[1]
bodies={}
for p in (root/'src/recomp/gen').glob('*.c'):
 s=p.read_text()
 for m in re.finditer(r'void sub_([0-9A-F]{8})\(void\)\n\{',s):
  end=s.find('\n/**',m.start())
  bodies[int(m[1],16)]=s[m.start():end if end!=-1 else len(s)]
real=[0x426e0,0x3cfd0,0x367a0,0x367b0,0x367c0,0x367e0,0x970f0,0x97ca0,0x98ec0,0x9b710,0x9c460,0x9cc10]
new=(root/'src/qpr2_v42_callbacks.c').read_text()
needed=set(int(x,16) for x in re.findall(r'RECOMP_ABI_CALL\(0x([0-9A-F]+)u',new))|{0x35f80,0x152a0,0xe16a0,0x426f0}
special={0x12b267,0x42bc0,0x10faa0,0x4e570,0x35f10,0x35f80,0x110b60,0xe0830,0x66260}
mock=[]
for a in sorted(needed-set(real)-special):
 clean=set(int(x) for x in re.findall(r'esp \+= (\d+); return;',bodies[a]))
 assert len(clean)==1,(hex(a),clean)
 mock.append(f'void sub_{a:08X}(void) {{g_eax=g_ecx;g_esp+={clean.pop()};}}')
shim=r'''
#include <assert.h>
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
static uint32_t next_alloc,allocated,last_root,root_size;
static int old_constructor,fail_root,links,base_count;
static void style(void){g_esp+=12;}
recomp_func_t recomp_lookup_manual(uint32_t va){(void)va;return NULL;}
recomp_func_t recomp_lookup_kernel(uint32_t va){(void)va;return NULL;}
recomp_func_t recomp_lookup(uint32_t va){assert(va==0x110000);return style;}
void recomp_icall_not_code_log(uint32_t va){(void)va;assert(0);}
void recomp_icall_fail_log(uint32_t va){(void)va;assert(0);}
void sub_0012B267(void) {
 uint32_t bytes=MEM32(g_esp+4);++allocated;
 if(allocated==1){root_size=bytes;if(fail_root){g_eax=0;g_esp+=4;return;}}
 g_eax=next_alloc;next_alloc+=(bytes+0xfff)&~0xfff;assert(next_alloc<0x200000);
 memset((void*)XBOX_PTR(g_eax),0xA0,bytes+16);
 if(allocated==1){last_root=g_eax;MEM32(g_eax)=0xA05A305C;}
 g_esp+=4;
}
void sub_00035F80(void){++base_count;memset((void*)XBOX_PTR(g_ecx),0,0x160);MEM32(g_ecx)=0x1b0d80;g_eax=g_ecx;g_esp+=16;}
void sub_00035F10(void){memset((void*)XBOX_PTR(g_ecx),0,0x40);MEM32(g_ecx)=0x1b0d80;g_eax=g_ecx;g_esp+=36;}
void sub_00110B60(void){MEM32(g_ecx)=0x1b0d80;g_eax=g_ecx;g_esp+=12;}
void sub_0010FAA0(void){assert(MEM32(g_esp+4)>0);++links;g_esp+=12;}
void sub_0004E570(void){g_eax=0x210000;g_esp+=4;}
void sub_000E0830(void){g_eax=0x210000;g_esp+=8;}
void sub_00042BC0(void){g_fp_top=(g_fp_top+7)&7;g_fp_stack[g_fp_top]=0.1;g_esp+=4;}
void sub_00066260(void){assert(0);}
void qpr2_recovered_00044010(void),qpr2_recovered_0003D060(void),qpr2_recovered_00043BE0(void);
void qpr2_recovered_00045060(void),qpr2_recovered_0006B950(void),qpr2_recovered_00045510(void);
'''
# Use the actual production direct-call wrappers, rather than a manual-only
# dispatch mock which could hide the original tail-jump bug.
stubs=(root/'src/recomp/gen/recomp_stubs_unresolved.c').read_text()
wrappers=[]
for a in [0x44010,0x3d060,0x43be0,0x45060,0x6b950,0x45510]:
 pat=rf'void sub_{a:08X}\(void\) \{{ qpr2_recovered_{a:08X}\(\); \}}'
 match=re.search(pat,stubs);assert match,(hex(a),'direct wrapper missing')
 wrappers.append(match[0].replace(f' qpr2_recovered_{a:08X}();',f' if(old_constructor)g_esp+=4;else qpr2_recovered_{a:08X}();'))
fixture=r'''
int main(void) {
 void *ram=calloc(1,0x300000);assert(ram);g_xbox_mem_offset=(ptrdiff_t)ram;
 MEMF(0x1c91d0)=1;MEMF(0x1b1fb0)=1.0f/640;MEMF(0x1b1fb4)=1.0f/480;
 MEM32(0x1d9fb8)=0x230000;MEM32(0x230010)=2;
 MEM32(0x230014)=0x231000;MEM32(0x230018)=0x232000;
 MEMF(0x1d8084)=0.05;MEMF(0x1d8090)=0.1;MEM32(0x1db454)=0x210000;
 MEM32(0x1b0d80+0x30)=MEM32(0x1b0d80+0x34)=0x110000;
 void (*getters[])(void)={sub_000970F0,sub_00097CA0,sub_0009B710,sub_0009C460,sub_0009CC10,sub_00098EC0};
 uint32_t expected[]={0x1b3a10,0x1b2a58,0x1b3970,0x1b3ad8,0x1bad68,0x1b3b80};
 for(int mode=0;mode<6;mode++)for(int variant=0;variant<3;variant++) {
  next_alloc=0x100000;allocated=last_root=links=base_count=0;
  old_constructor=variant==0;fail_root=variant==2;
  g_ecx=0x240000;MEM32(g_ecx+0x68)=mode==5?4:10;
  g_esp=0x280000;MEM32(g_esp)=0x98765;
  g_ebx=91;g_esi=92;g_edi=93;g_ebp=g_seh_ebp=94;g_fp_top=0;
  getters[mode]();

  assert(g_esp==0x280004 && g_ebx==91 && g_esi==92 && g_edi==93 && g_fp_top==0);
  if(fail_root){assert(g_eax==0 && !last_root && !base_count);continue;}
  assert(g_eax==last_root && g_eax==0x100000);
  if(old_constructor){assert(MEM32(g_eax)==0xA05A305C && !base_count);continue;}
  assert(MEM32(g_eax)==expected[mode] && base_count==1);
  for(unsigned i=0;i<16;i++)assert(MEM8(last_root+root_size+i)==0xA0);
  printf("PASS: retail factory mode %d initializes poisoned root vtable=%08X size=%u links=%d and preserves ABI\n",mode,MEM32(g_eax),root_size,links);
 }
 free(ram);puts("PASS: old return-only constructors reproduce V41 invalid vtable; all six actual factory/direct-wrapper/recovered-constructor chains initialize roots, child loops and handle failed root allocation");
}
'''
with tempfile.TemporaryDirectory() as d:
 p=Path(d)/'test.c';exe=Path(d)/'test'
 p.write_text(shim+'\n'.join(mock)+'\n'+'\n'.join(wrappers)+'\n'+'\n'.join(bodies[a] for a in real)+fixture)
 subprocess.run(['gcc','-std=gnu11','-O1','-ffunction-sections','-fdata-sections','-fsanitize=undefined','-Werror=implicit-function-declaration','-I'+str(root/'src'),str(p),str(root/'src/qpr2_v42_callbacks.c'),'-Wl,--gc-sections','-lm','-o',str(exe)],check=True)
 subprocess.run([str(exe)],check=True,timeout=15)
