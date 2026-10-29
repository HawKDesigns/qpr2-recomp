"""Check local ROV shader selection, global retry, and intact guest output."""
from pathlib import Path
import subprocess, tempfile, shutil
import test_qpr2_gpu_timing as t
from qpr2_shader_sources import parse_embedded
root=Path(__file__).resolve().parents[1];k=root/'third_party/xboxrecomp/src/kernel'
source=parse_embedded((k/'nv2a_gpu_shader.h').read_text())['nv_gpu_rov_source']
for local in (0,1):
 code=subprocess.check_output(['gcc','-E','-P','-x','c','-DNV_GPU_LOCAL_ROV='+str(local),'-'],
                              input=source,text=True)
 pixel=code[code.index('float4 pixel_main('):]
 assert pixel.count('DeviceMemoryBarrier();')==1
 assert ('globallycoherent RasterizerOrderedStructuredBuffer<uint2> surface_buffer' in code)==(local==0)
 assert code.count('RasterizerOrderedStructuredBuffer<uint2> surface_buffer')==1
api=t.api.replace('static HRESULT D3DCompile(', 'static int local_fail;\nstatic HRESULT D3DCompile(')
api=api.replace('int simple_macro=0;', 'int local_macro=0;for(D3D_SHADER_MACRO*m=b;m && m->Name;m++)if(!strcmp(m->Name,"NV_GPU_LOCAL_ROV"))local_macro=1;\n int simple_macro=0;')
api=api.replace(' || failed_compile || (literal_macro &&', ' || (local_fail && local_macro) || failed_compile || (literal_macro &&')
assert api!=t.api and 'local_fail && local_macro' in api
scene=t.main[t.main.index('static uint32_t color[1024]'):t.main.index('static void compute_scene(void)')]
main=r'''
int main(int argc,char**argv){
 setenv("RECOMP_GPU_RASTER","1",1);setenv("RECOMP_GPU_GEOMETRY","1",1);
 setenv("RECOMP_GPU_ASYNC","0",1);setenv("RECOMP_GPU_LOCAL_ROV","1",1);
 local_fail=argc>1;native_supported=1;coverage_tier=3;
 assert(nv_gpu_available() && g.native);
 assert(g.simple_pixel_shader);
 assert(g.local_rov==!local_fail && g.local_rov_rejected==local_fail);
 assert(gpu_native_variant(2u)&1024u ? !local_fail : local_fail);
 NvGpuCount count=scene();assert(count.zpass && color[5*32+5]==0xFFFFFFFF);
 puts(local_fail?"PASS: rejected local shader restored validated global shader and guest output":
                 "PASS: local simple shader admitted with exact guest output");
 return 0;
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-local-rov-') as d:
 p=Path(d);(p/'logs').mkdir()
 for name in ['windows.h','d3d11.h','d3d11_3.h','d3dcompiler.h','dxgi.h']:(p/name).write_text('/* API shim */\n')
 (p/'fixture.c').write_text(api+t.queries+'ptrdiff_t xbox_GetMemoryOffset(void){return 0;}\n#include "'+
                            str(k/'nv2a_gpu_raster.c')+'"\n'+t.geo.fixture+scene+main)
 subprocess.run(['gcc','-std=gnu11','-O1','-ffp-contract=off','-Werror=implicit-function-declaration',
                 '-Werror=incompatible-pointer-types','-fsanitize=undefined,float-cast-overflow',
                 '-fno-sanitize-recover=all','-I'+str(p),'-I'+str(root/'tools'),'-I'+str(k),
                 str(p/'fixture.c'),str(k/'nv2a_vsh_interp.c'),str(k/'nv2a_gpu_tiles.c'),
                 '-pthread','-lm','-o',str(p/'fixture')],check=True)
 for arg in ([],['fallback']):
  shutil.rmtree(p/'shader-cache',ignore_errors=True)
  subprocess.run([str(p/'fixture')]+arg,cwd=p,check=True,timeout=120)
print('PASS: embedded local/global HLSL preserve per-writer barrier; distinct variant keys')
