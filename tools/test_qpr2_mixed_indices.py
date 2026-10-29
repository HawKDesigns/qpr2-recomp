"""Real index commands + interpreter + raster; odd strip tail and full indices."""
from pathlib import Path
import subprocess,tempfile
import test_qpr2_gpu_event as gpu
root=gpu.root
fixture=r'''
static void setup(void){
 memset(ram,0,sizeof ram);memset(&s_gpu,0,sizeof s_gpu);
 s_gpu.format=0x128;s_gpu.color_offset=0x1000;s_gpu.pitch=32;
 s_gpu.clip_w=s_gpu.clip_h=8;s_gpu.color_mask=0x01010101;s_gpu.xf_mode=2;
 s_gpu.attr[0].offset=0x20000;s_gpu.attr[0].type=2;s_gpu.attr[0].size=4;s_gpu.attr[0].stride=16;
 const float points[5][4]={{0,0,10,1},{8,0,10,1},{0,4,10,1},{8,4,10,1},{0,8,10,1}};
 memcpy(ram+0x20000,points,sizeof points);
 uint32_t pos[4]={0,0x0020001b,0x08000000,0x0000f800};
 uint32_t color[4]={0,0x0020001b,0x0c000000,0x0000f819};
 const float white[4]={1,1,1,1};
 nv2a_vsh_set_instruction(0,pos);nv2a_vsh_set_instruction(1,color);
 nv2a_vsh_set_constant(0,white);nv2a_vsh_set_start_slot(0);
}
static void method(uint32_t m,uint32_t p){nv2a_pb_exec_method(0,m,p);}
static void triangle(uint32_t a,uint32_t b,uint32_t c){
 method(NV097_SET_BEGIN_END,NV_PRIM_TRIANGLES);
 method(NV097_ARRAY_ELEMENT32,a);method(NV097_ARRAY_ELEMENT32,b);method(NV097_ARRAY_ELEMENT32,c);
 method(NV097_SET_BEGIN_END,0);
}
int main(void){
 setup();method(NV097_SET_BEGIN_END,NV_PRIM_TRIANGLE_STRIP);
 method(NV097_ARRAY_ELEMENT16,1u<<16);
 method(NV097_SET_BEGIN_END,0);assert(!s_gpu.xf_pixels); /* old ignored tail */
 setup();method(NV097_SET_BEGIN_END,NV_PRIM_TRIANGLE_STRIP);
 method(NV097_ARRAY_ELEMENT16,1u<<16);method(NV097_ARRAY_ELEMENT32,2);
 assert(s_gpu.idx_count==3 && s_gpu.idx[2]==2);
 method(NV097_SET_BEGIN_END,0);assert(s_gpu.xf_pixels==16 && s_gpu.xf_drawn==1 && !s_gpu.unhandled_total);
 setup();method(NV097_SET_BEGIN_END,NV_PRIM_TRIANGLE_STRIP);
 method(NV097_ARRAY_ELEMENT16,1u<<16);method(NV097_ARRAY_ELEMENT16,2|(3u<<16));
 method(NV097_SET_BEGIN_END,0);assert(s_gpu.xf_pixels==32);
 setup();method(NV097_SET_BEGIN_END,NV_PRIM_TRIANGLE_STRIP);
 method(NV097_ARRAY_ELEMENT16,1u<<16);method(NV097_ARRAY_ELEMENT16,2|(3u<<16));method(NV097_ARRAY_ELEMENT32,4);
 method(NV097_SET_BEGIN_END,0);assert(s_gpu.xf_pixels==48 && s_gpu.xf_drawn==3);
 uint8_t reference[256];memcpy(reference,ram+0x1000,sizeof reference);
 setup();triangle(0,1,2);triangle(2,1,3);triangle(2,3,4);
 assert(s_gpu.xf_pixels==48 && !memcmp(reference,ram+0x1000,sizeof reference));
 setup();memcpy(ram+0x20000+70000*16,ram+0x20000+2*16,16);
 triangle(0,1,70000);assert(s_gpu.idx[2]==70000 && s_gpu.xf_pixels==16);
 setup();memcpy(ram+0x20000+65536*16,ram+0x20000,3*16);
 method(NV097_SET_BEGIN_END,NV_PRIM_TRIANGLES);method(NV097_DRAW_ARRAYS,(2u<<24)|65536);
 assert(s_gpu.idx[0]==65536 && s_gpu.idx[2]==65538);
 method(NV097_SET_BEGIN_END,0);assert(s_gpu.xf_pixels==16);
 setup();method(NV097_ARRAY_ELEMENT32,123);assert(!s_gpu.idx_count);
 method(NV097_SET_BEGIN_END,NV_PRIM_TRIANGLE_STRIP);
 for(uint32_t i=0;i<NV_MAX_INDICES;i++)method(NV097_ARRAY_ELEMENT32,i);
 method(NV097_ARRAY_ELEMENT32,0xffffffff);assert(s_gpu.idx_count==NV_MAX_INDICES && s_gpu.idx_dropped==1);
 puts("PASS: actual mixed 16/32-bit commands restore odd strip-tail triangle; bytes equal explicit triangle reference; index 70000 and DRAW_ARRAYS above 65535 retain full width; bounds and inactive commands guarded");
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-mixed-indices-') as d:
 src=Path(d)/'fixture.c';exe=Path(d)/'fixture'
 renderer='#include "'+str(root/'third_party/xboxrecomp/src/kernel/nv2a_pb_exec.c')+'"\n'
 src.write_text(gpu.shim+gpu.shadow+gpu.lookup+gpu.events+gpu.command_fences+gpu.registration+renderer+fixture)
 subprocess.run(['gcc','-std=gnu11','-O1','-ffunction-sections','-fdata-sections',
  '-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all',
  '-Werror=implicit-function-declaration','-I'+str(root/'third_party/xboxrecomp/src'),
  '-I'+str(root/'third_party/xboxrecomp/include'),str(src),
  str(root/'third_party/xboxrecomp/src/kernel/nv2a_vsh_interp.c'),
  str(root/'third_party/xboxrecomp/src/kernel/nv2a_combiner.c'),
  '-Wl,--gc-sections','-pthread','-lm','-o',str(exe)],check=True)
 subprocess.run([str(exe)],check=True,timeout=15)
