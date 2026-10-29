"""Read/expand checked-in shader inputs, including large MSVC-safe byte arrays."""
from pathlib import Path
import ast,re
SHADERS=[('nv2a_gpu_raster.hlsl','nv_gpu_shader_source'),('nv2a_gpu_rov.hlsl','nv_gpu_rov_source'),('nv2a_gpu_report_reset.hlsl','nv_gpu_report_reset_source'),('nv2a_gpu_coverage.hlsl','nv_gpu_coverage_source'),('nv2a_gpu_clear.hlsl','nv_gpu_clear_source'),('nv2a_gpu_geometry.hlsl','nv_gpu_geometry_source'),('nv2a_gpu_texcopy.hlsl','nv_gpu_texcopy_source')]
INCLUDES=['nv2a_gpu_types.h','nv2a_gpu_pixel.inc','nv2a_gpu_literal.inc','nv2a_gpu_compact.inc','nv2a_gpu_shade.inc','nv2a_gpu_source.inc','nv2a_gpu_raw_types.h','nv2a_gpu_vsh.inc','nv2a_gpu_prepare.inc']
def expand_shader(root,name):
 source=(Path(root)/name).read_text()
 for include in INCLUDES:source=source.replace('#include "'+include+'"',(Path(root)/include).read_text())
 return source

def parse_embedded(text):
 result={}
 for name,body in re.findall(r'static const char (\w+)\[\] =\n(.*?);\n',text,re.S):
  if body.lstrip().startswith('{'):
   values=[int(x,16) for x in re.findall(r'0x([0-9A-Fa-f]{2})\b',body)]
   assert values and values[-1]==0 and not any(v==0 for v in values[:-1])
   result[name]=bytes(values[:-1]).decode('utf-8')
  else:result[name]=''.join(ast.literal_eval(x) for x in re.findall(r'"(?:[^"\\]|\\.)*"',body))
 return result
