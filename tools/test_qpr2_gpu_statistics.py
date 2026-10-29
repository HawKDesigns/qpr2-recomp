"""Check compiled-out native diagnostics in the exact production shader source.
HLSL hardware arithmetic/order checks remain the Windows offscreen gate;
API fixtures separately compare pixels/depth and guest visibility behavior.
"""
from pathlib import Path
import subprocess,ast,re
root=Path(__file__).resolve().parents[1];k=root/'third_party/xboxrecomp/src/kernel'
s=(k/'nv2a_gpu_shader.h').read_text()
from qpr2_shader_sources import parse_embedded
source=parse_embedded(s)['nv_gpu_rov_source']
for simple in [0,1]:
 body=[]
 for diagnostics in [0,1]:
  code=subprocess.check_output(['gcc','-E','-P','-x','c','-DNV_GPU_DIAGNOSTIC_COUNTERS='+str(diagnostics),'-DNV_GPU_SIMPLE_MATERIAL='+str(simple),'-'],input=source,text=True)
  pixel=code[code.index('float4 pixel_main('):]
  assert pixel.count('DeviceMemoryBarrier();')==1
  assert pixel.count('InterlockedAdd(')==(3 if diagnostics else 1)
  assert pixel.count('InterlockedCompareExchange(')==diagnostics
  assert 'InterlockedAdd(statistics[base+2],p.zpass)' in pixel
  body.append(pixel[:pixel.index('uint base=')])
 assert body[0]==body[1], 'diagnostic policy may not change color/depth/order/coverage/texture code'
print('PASS: both native shader variants remove diagnostic atomics and brightness reads, preserve the one visibility atomic, and retain identical pixel/ordering code')
