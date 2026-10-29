"""Real GPU methods + transformed triangle clipping/depth occlusion regression."""
from pathlib import Path
import subprocess
import tempfile
import test_qpr2_gpu_event as gpu
root = gpu.root
fixture = r'''
static void setup(void) {
 memset(ram,0,sizeof ram);memset(&s_gpu,0,sizeof s_gpu);
 s_gpu.format=0x128;s_gpu.color_offset=0x1000;s_gpu.pitch=32;
 s_gpu.clip_w=s_gpu.clip_h=8;s_gpu.color_mask=0x01010101;
 s_gpu.depth_test=s_gpu.depth_mask=1;s_gpu.depth_func=0x203;
 s_gpu.zeta_offset=0x3000;
 for(int i=0;i<4;i++)if(s_zbufs[i].z)
  for(size_t k=0;k<(size_t)NV_ZBUF_W*NV_ZBUF_H;k++)s_zbufs[i].z[k]=3.4e38f;
}
static Nv2aVshOutput point(float x,float y,float z,float w) {
 Nv2aVshOutput p={0};p.pos[0]=x;p.pos[1]=y;p.pos[2]=z;p.pos[3]=w;
 p.d0[0]=p.d0[3]=1;return p;
}
static void range(float low,float high) {
 uint32_t u;memcpy(&u,&low,4);nv2a_pb_exec_method(0,NV097_SET_CLIP_MIN,u);
 memcpy(&u,&high,4);nv2a_pb_exec_method(0,NV097_SET_CLIP_MAX,u);
 assert(s_gpu.clip_z_seen==3 && s_gpu.clip_z_min==low && s_gpu.clip_z_max==high);
}
int main(void) {
 setup();range(0,100);assert(!s_gpu.unhandled_total);
 Nv2aVshOutput a=point(0,0,-10,1),b=point(8,0,-10,1),c=point(0,8,-10,1);
 raster_xf_triangle(&a,&b,&c); /* Reproduce the old un-clipped entry. */
 a.pos[2]=b.pos[2]=c.pos[2]=50;a.d0[0]=b.d0[0]=c.d0[0]=0;
 a.d0[1]=b.d0[1]=c.d0[1]=1;raster_xf_triangle(&a,&b,&c);
 assert(((uint32_t*)(ram+0x1000))[0]==0xFFFF0000 && s_gpu.xf_depth_fail==28);
 setup();range(0,100);a=point(0,0,-10,1);b=point(8,0,-10,1);c=point(0,8,-10,1);
 raster_xf_clipped(&a,&b,&c);
 assert(!s_gpu.xf_pixels && s_gpu.xf_zclip_outside==1);
 /* Old renderer clamps the invalid negative-Z polygon to zero and lets it
  * occlude terrain that should be visible. Range clipping removes it. */
 a.pos[2]=b.pos[2]=c.pos[2]=50;a.d0[0]=b.d0[0]=c.d0[0]=0;
 a.d0[1]=b.d0[1]=c.d0[1]=1;raster_xf_clipped(&a,&b,&c);
 assert(((uint32_t*)(ram+0x1000))[0]==0xFF00FF00 && s_gpu.xf_pixels==28);
 setup();range(0,100);a=point(0,0,-50,1);b=point(8,0,50,1);c=point(0,8,50,1);
 raster_xf_clipped(&a,&b,&c);
 for(int y=0;y<8;y++)for(int x=0;x<8;x++) {
  int visible=x+y>=3 && x+y<7;
  assert((((uint32_t*)(ram+0x1000))[y*8+x]!=0)==visible);
 }
 assert(s_gpu.xf_zclip_partial==1 && !s_gpu.xf_zclip_outside);
 setup();range(0,100);a=point(0,0,150,1);b=point(8,0,50,1);c=point(0,8,50,1);
 raster_xf_clipped(&a,&b,&c);
 for(int y=0;y<8;y++)for(int x=0;x<8;x++)
  assert((((uint32_t*)(ram+0x1000))[y*8+x]!=0)==(x+y>=3 && x+y<7));
 setup();range(0,1e36f);s_gpu.control0=1u<<12;
 a=point(0,0,-1.84541873e33f,1);
 b=point(8,0,5e35f,10);c=point(0,8,5e35f,10);
 raster_xf_clipped(&a,&b,&c);
 assert(s_gpu.xf_pixels>0);
 float *z=zbuf_current(0);assert(z);
 for(int y=0;y<8;y++)for(int x=0;x<8;x++)
  if(((uint32_t*)(ram+0x1000))[y*8+x])assert(isfinite(z[y*NV_ZBUF_W+x]) && z[y*NV_ZBUF_W+x]>=0 && z[y*NV_ZBUF_W+x]<=1e36f);
 setup();range(0,100);a=point(0,0,101,1);b=point(8,0,101,1);c=point(0,8,101,1);
 raster_xf_clipped(&a,&b,&c);assert(!s_gpu.xf_pixels && s_gpu.xf_zclip_outside==1);
 setup();range(0,100);a=point(0,0,0,1);b=point(8,0,0,1);c=point(0,8,0,1);
 raster_xf_clipped(&a,&b,&c);assert(s_gpu.xf_pixels==28);
 puts("PASS: real clip-limit methods, negative-depth false occlusion, partial near/far polygons, inclusive zero depth, huge floating-depth case and depth-buffer safety");
 return 0;
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-depth-clip-') as d:
    source=Path(d)/'fixture.c';exe=Path(d)/'fixture'
    renderer='#include "'+str(root/'third_party/xboxrecomp/src/kernel/nv2a_pb_exec.c')+'"\n'
    source.write_text(gpu.shim+gpu.shadow+gpu.lookup+gpu.events+gpu.command_fences+gpu.registration+renderer+fixture)
    subprocess.run(['gcc','-std=gnu11','-O1','-ffunction-sections','-fdata-sections',
        '-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all',
        '-Werror=implicit-function-declaration','-I'+str(root/'third_party/xboxrecomp/src'),
        '-I'+str(root/'third_party/xboxrecomp/include'),str(source),
        str(root/'third_party/xboxrecomp/src/kernel/nv2a_vsh_interp.c'),
        str(root/'third_party/xboxrecomp/src/kernel/nv2a_combiner.c'),
        '-Wl,--gc-sections','-pthread','-lm','-o',str(exe)],check=True)
    subprocess.run([str(exe)],check=True,timeout=15)
