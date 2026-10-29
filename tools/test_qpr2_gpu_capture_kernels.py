"""Compare general/specialized scalar kernels on an actual F8 capture.
Historical GPU differences are reported separately; this tests kernel equivalence,
not execution of the updated HLSL on real hardware.
Usage: python tools/test_qpr2_gpu_capture_kernels.py path/to/capture.bin
"""
from pathlib import Path
import subprocess,sys,tempfile
import replay_qpr2_gpu_capture as replay
root=Path(__file__).resolve().parents[1]
simple=r'''
#include "nv2a_gpu_types.h"
uint32_t test_load_texel(NvGpuTexture,int,int,int,int);
static uint32_t gpu_load_texel(NvGpuTexture t,int st,int face,int x,int y){return test_load_texel(t,st,face,x,y);}
#define NV_GPU_SIMPLE_MATERIAL 1
#include "nv2a_gpu_pixel.inc"
#include "nv2a_gpu_shade.inc"
NvGpuPixel test_simple_pixel(NvGpuState s,NvGpuTri t,int x,int y,uint32_t c,float z){return gpu_pixel(s,t,x,y,c,z);}
'''
fixed=simple.replace('#define NV_GPU_SIMPLE_MATERIAL 1','#define NV_GPU_FIXED_REGISTERS 1').replace('test_simple_pixel','test_fixed_pixel')
fixed+=r'''
NvGpuPixel test_native_pixel(NvGpuState s,NvGpuTri t,int x,int y,uint32_t c,float z){
 NvGpuBary bary=gpu_bary(s,t,x,y);NvGpuSource source={0};
 if(bary.covered)source=gpu_source_color(s,t,bary.l0,bary.l1,bary.l2);
 return gpu_commit(s,bary,source,c,z);
}
'''
source=replay.source.replace('static uint32_t gpu_load_texel(', 'uint32_t test_load_texel(')
source=source.replace('#include "nv2a_gpu_pixel.inc"', '''
static uint32_t gpu_load_texel(NvGpuTexture t,int st,int face,int x,int y){return test_load_texel(t,st,face,x,y);}
#include "qpr2_gpu_coverage_fixture.h"
NvGpuPixel test_simple_pixel(NvGpuState,NvGpuTri,int,int,uint32_t,float);
NvGpuPixel test_fixed_pixel(NvGpuState,NvGpuTri,int,int,uint32_t,float);
NvGpuPixel test_native_pixel(NvGpuState,NvGpuTri,int,int,uint32_t,float);
#include "nv2a_gpu_pixel.inc"''')
source=source.replace('unsigned batches=0,checks=0,frames=0,mismatches=0;',
    'unsigned batches=0,checks=0,frames=0,mismatches=0;uint64_t eligible=0,candidate_count=0,bbox_count=0;')
source=source.replace('color[at]=p.color;depth[at]=p.depth;', '''
     int candidate=coverage_candidate(&t[i],x,y);assert(candidate || !gpu_bary(state,t[i],x,y).covered);candidate_count+=candidate;++bbox_count;
     NvGpuPixel fixed=test_fixed_pixel(state,t[i],x,y,color[at],depth[at]);
     NvGpuPixel native=test_native_pixel(state,t[i],x,y,color[at],depth[at]);
     assert(!memcmp(&p,&fixed,sizeof p) && !memcmp(&p,&native,sizeof p));
     if(!t[i].use_rc || gpu_rc_simple_kind(state.rc)){
      NvGpuPixel other=test_simple_pixel(state,t[i],x,y,color[at],depth[at]);
      assert(p.color==other.color && p.depth==other.depth && p.depth_fail==other.depth_fail &&
             p.pixels==other.pixels && p.zpass==other.zpass && p.pixel_max==other.pixel_max);++eligible;
     }
     color[at]=p.color;depth[at]=p.depth;''')
source=source.replace('if(mismatches<=4)', 'if(0)')
source=source.replace('return mismatches?1:0;',
    'printf("PASS: %llu exact array/named/split-native/specialized pixel comparisons from captured geometry/state/textures\\n",(unsigned long long)eligible);printf("Independent coverage model: %llu candidates / %llu bounding-box pixels\\n",(unsigned long long)candidate_count,(unsigned long long)bbox_count);return 0;')
if __name__=='__main__':
    if len(sys.argv)!=2:raise SystemExit(__doc__)
    with tempfile.TemporaryDirectory(prefix='qpr2-capture-kernels-') as d:
        p=Path(d);(p/'fixture.c').write_text(source);(p/'simple.c').write_text(simple);(p/'fixed.c').write_text(fixed)
        subprocess.run(['gcc','-std=c11','-O2','-ffp-contract=off','-fsanitize=undefined,float-cast-overflow',
            '-fno-sanitize-recover=all','-I'+str(root/'third_party/xboxrecomp/src/kernel'),'-I'+str(root/'tools'),
            str(p/'fixture.c'),str(p/'simple.c'),str(p/'fixed.c'),'-lm','-o',str(p/'fixture')],check=True)
        subprocess.run([str(p/'fixture'),str(Path(sys.argv[1]).resolve())],check=True,timeout=600)
