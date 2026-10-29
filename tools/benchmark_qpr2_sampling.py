"""Sample-cache and two-texture raster benchmark; optional renderer source arg.

Runs a deterministic workload and reports checksums and timings. These are local
CPU measurements, not a prediction of Windows game FPS. Tests the actual cache
and raster entry points, including rewriting compressed blocks at the same VA.
"""
from pathlib import Path
import subprocess
import sys
import tempfile
root=Path(__file__).resolve().parents[1]
renderer=Path(sys.argv[1]).resolve() if len(sys.argv)>1 else root/'third_party/xboxrecomp/src/kernel/nv2a_pb_exec.c'
fixture=r'''
#include <assert.h>
static uint8_t ram[0x200000];
uint32_t g_xbox_image_lo,g_xbox_image_hi;
ptrdiff_t xbox_GetMemoryOffset(void){return (ptrdiff_t)ram;}
uint32_t xbox_ContiguousAllocatedBytes(void){return 0;}
void xbox_FramebufferWindowSet(uint32_t a,uint32_t b){(void)a;(void)b;}
void xbox_FramebufferWindowStart(void){}
void xbox_FramebufferWindowPresentPixels(const uint32_t*p,uint32_t w,uint32_t h){(void)p;(void)w;(void)h;}
int nv2a_vsh_cacheable(void){return 1;}
uint32_t nv2a_vsh_input_mask(void){return 0xffffu;}
void nv2a_vsh_report(void){}
int nv2a_vsh_run(const float in[NV2A_VSH_INPUTS][4],Nv2aVshOutput*out){(void)in;(void)out;return 0;}
const float*nv2a_vsh_constant(uint32_t i){static float c[4];(void)i;return c;}
void nv2a_vsh_trace_state(void){}
static uint32_t seed=0x9e3779b9;
static uint32_t random_word(void){seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed;}
static Texture texture(unsigned i){
 Texture t={0};t.offset=0x100000+i*0x40000;t.width=t.height=512;
 t.color=0xf;t.addr_u=t.addr_v=1;t.filter=0x02020000;t.valid=1;return t;
}
static uint32_t hash_bytes(const void *p,size_t n){
 const uint8_t*b=p;uint32_t h=2166136261u;for(size_t i=0;i<n;i++)h=(h^b[i])*16777619u;return h;
}
static Nv2aVshOutput vertex(float x,float y,float u,float v){
 Nv2aVshOutput p={0};p.pos[0]=x;p.pos[1]=y;p.pos[2]=10;p.pos[3]=1;
 for(int k=0;k<4;k++)p.d0[k]=1;
 p.tex[0][0]=u;p.tex[0][1]=v;p.tex[0][3]=1;
 p.tex[1][0]=v;p.tex[1][1]=u;p.tex[1][3]=1;return p;
}
int main(void){
 for(size_t i=0x100000;i<0x180000;i++)ram[i]=(uint8_t)random_word();
 Texture t=texture(0);uint32_t got,expected;
 for(unsigned f=0;f<3;f++){
  t.color=f==0?0xc:f==1?0xe:0xf;
  for(unsigned n=0;n<2000;n++){
   uint32_t u=random_word()%512,v=random_word()%512;
   assert(sample_tex(&t,0,u,v,&got));
   assert(d3d8_dxt_decode_texel(ram+t.offset,t.color,u,v,t.width,&expected) && got==expected);
   if(n%64==0){
    uint32_t bb=d3d8_format_dxt_block_bytes(t.color);
    ram[t.offset+((v/4)*128+u/4)*bb]^=0x55;
    assert(sample_tex(&t,0,u,v,&got));
    assert(d3d8_dxt_decode_texel(ram+t.offset,t.color,u,v,t.width,&expected) && got==expected);
   }
  }
 }
 t=texture(0);seed=0x12345678;uint32_t sum=0;
 double begin=render_wall_ms();
 for(unsigned i=0;i<400000;i++){
  uint32_t u=random_word()%512,v=random_word()%512;sample_tex(&t,0,u,v,&got);sum=(sum*33)^got;
 }
 double sampling_ms=render_wall_ms()-begin;
 memset(&s_gpu,0,sizeof s_gpu);s_gpu.format=0x128;s_gpu.color_offset=0x1000;s_gpu.pitch=1024;
 s_gpu.clip_w=s_gpu.clip_h=256;s_gpu.color_mask=0x01010101;s_gpu.rc_seen=1;
 s_gpu.texs[0]=texture(0);s_gpu.texs[1]=texture(1);
 s_gpu.rc.control=2;s_gpu.rc.stage_program=1|(1u<<5);
 s_gpu.rc.color_icw[0]=0x08040000;s_gpu.rc.color_icw[1]=0x090c0000;
 s_gpu.rc.alpha_icw[0]=0x18140000;s_gpu.rc.alpha_icw[1]=0x191c0000;
 s_gpu.rc.color_ocw[0]=s_gpu.rc.color_ocw[1]=0xc0;
 s_gpu.rc.alpha_ocw[0]=s_gpu.rc.alpha_ocw[1]=0xc0;
 Nv2aVshOutput a=vertex(0,0,0,0),b=vertex(256,0,4,0),c=vertex(256,256,4,4),d=vertex(0,256,0,4);
 begin=render_wall_ms();
 for(int i=0;i<8;i++){raster_xf_clipped(&a,&b,&c);raster_xf_clipped(&a,&c,&d);}
 double raster_ms=render_wall_ms()-begin;
 assert(s_gpu.xf_pixels==8*256*256);
 printf("{\"sampling_ms\":%.3f,\"raster_ms\":%.3f,\"sample_checksum\":%u,\"pixel_checksum\":%u,\"pixels\":%llu}\n",
  sampling_ms,raster_ms,sum,hash_bytes(ram+0x1000,256*256*4),(unsigned long long)s_gpu.xf_pixels);
 puts("PASS: actual DXT cache matches scalar reference, observes same-address rewrites, and full two-stage bilinear raster workload completes");
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-sampling-benchmark-') as d:
    src=Path(d)/'fixture.c';exe=Path(d)/'fixture'
    src.write_text('#include "'+str(renderer)+'"\n'+fixture)
    subprocess.run(['gcc','-std=gnu11','-O2','-ffunction-sections','-fdata-sections',
        '-I'+str(root/'third_party/xboxrecomp/src/kernel'),
        '-I'+str(root/'third_party/xboxrecomp/src'),
        '-I'+str(root/'third_party/xboxrecomp/include'),str(src),
        str(root/'third_party/xboxrecomp/src/kernel/nv2a_combiner.c'),
        '-Wl,--gc-sections','-pthread','-lm','-o',str(exe)],check=True)
    subprocess.run([str(exe)],check=True,timeout=30)
