"""Exercise the production texture cache with an 80-texture warm working set."""
from pathlib import Path
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
source = root / 'tools/test_qpr2_gpu_device.py'
scope = {'__file__': str(source)}
exec(compile(source.read_text().split('with tempfile.TemporaryDirectory(')[0], str(source), 'exec'), scope)
api = scope['api']
fixture = scope['fixture'].split('int main(')[0]
kernel = root / 'third_party/xboxrecomp/src/kernel'
main = r'''
int main(int argc,char **argv){
 int enabled=argc==1;
 setenv("RECOMP_GPU_RASTER","1",1);setenv("RECOMP_GPU_ASYNC","0",1);
 setenv("RECOMP_GPU_TEXTURE_HOT",enabled?"1":"0",1);
 native_supported=1;coverage_tier=3;
 assert(nv_gpu_available() && g.native && g.texture_hot_enabled==enabled);
 uint32_t pixels[80][256];NvGpuTextureSource sources[80]={0};
 NvGpuTexture meta={0};meta.valid=1;meta.width=meta.height=16;meta.format=0x12;
 for(unsigned i=0;i<80;i++){
  for(unsigned p=0;p<256;p++)pixels[i][p]=0xFF010000u+i*256u+p;
  sources[i].raw=(const uint8_t*)pixels[i];sources[i].bytes=sizeof pixels[i];
  sources[i].opaque=pixels[i];sources[i].decode=decode;
  ++g.tick;GpuTexture *slot=gpu_texture(&meta,&sources[i]);
  assert(slot && slot->decoded[0]==pixels[i][0]);
 }
 const unsigned loops=100000;double begin=gpu_ms();
 for(unsigned i=0;i<loops;i++){
  unsigned at=i%80;++g.tick;
  GpuTexture *slot=gpu_texture(&meta,&sources[at]);
  assert(slot && slot->decoded[0]==pixels[at][0]);
 }
 double elapsed=gpu_ms()-begin;
 assert(g.texture_updates==80 && g.texture_hits==loops);
 if(enabled)assert(g.texture_index_hits>loops/2 && g.texture_full_scans<loops/2);
 else assert(g.texture_full_scans>=loops);
 printf("PASS: hot=%d calls=%u elapsed_ms=%.3f index_hits=%llu full_scans=%llu\n",
   enabled,loops,elapsed,(unsigned long long)g.texture_index_hits,
   (unsigned long long)g.texture_full_scans);
 return 0;
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-texture-lookup-') as folder:
    p = Path(folder)
    for name in ['windows.h', 'd3d11.h', 'd3d11_3.h', 'd3dcompiler.h', 'dxgi.h']:
        (p / name).write_text('/* mock API */\n')
    test = p / 'fixture.c'
    test.write_text(api + 'ptrdiff_t xbox_GetMemoryOffset(void){return 0;}\n#include "' +
                    str(kernel / 'nv2a_gpu_raster.c') + '"\n' + fixture + main)
    exe = p / 'fixture'
    subprocess.run(['gcc', '-std=gnu11', '-O2', '-Werror=implicit-function-declaration',
                    '-Werror=incompatible-pointer-types', '-I' + str(p), '-I' + str(root / 'tools'),
                    '-I' + str(kernel), str(test), str(kernel / 'nv2a_vsh_interp.c'),
                    str(kernel / 'nv2a_gpu_tiles.c'), '-pthread', '-lm', '-o', str(exe)], check=True)
    for name, args in [('off', ['off']), ('on', [])]:
        cwd = p / name
        cwd.mkdir()
        subprocess.run([str(exe), *args], cwd=cwd, check=True, timeout=120)
