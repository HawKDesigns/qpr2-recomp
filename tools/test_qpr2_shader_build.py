"""Build real CMake shader dependency graph and change every shader input.
No Windows SDK, assets or HLSL compiler required; compiler-cache key checks
run in test_qpr2_gpu_device.py using the actual backend implementation.
"""
from pathlib import Path
import os,shutil,subprocess,tempfile
from qpr2_shader_sources import SHADERS,expand_shader,parse_embedded
root=Path(__file__).resolve().parents[1]
cmake=os.environ.get('CMAKE',shutil.which('cmake'))
assert cmake,'Set CMAKE to the CMake executable for this regression.'
text=(root/'CMakeLists.txt').read_text()
snippet=text[text.index('# Re-embed shader sources'):text.index('add_subdirectory(')]
inputs=['nv2a_gpu_raster.hlsl','nv2a_gpu_rov.hlsl','nv2a_gpu_report_reset.hlsl','nv2a_gpu_coverage.hlsl','nv2a_gpu_clear.hlsl','nv2a_gpu_types.h','nv2a_gpu_pixel.inc','nv2a_gpu_literal.inc','nv2a_gpu_compact.inc','nv2a_gpu_shade.inc','nv2a_gpu_source.inc','nv2a_gpu_geometry.hlsl','nv2a_gpu_texcopy.hlsl','nv2a_gpu_raw_types.h','nv2a_gpu_vsh.inc','nv2a_gpu_prepare.inc']
with tempfile.TemporaryDirectory(prefix='qpr2-shader-build-') as directory:
 p=Path(directory);k=p/'third_party/xboxrecomp/src/kernel';k.mkdir(parents=True)
 (p/'tools').mkdir()
 shutil.copy2(root/'tools/embed_qpr2_gpu_shader.cmake',p/'tools')
 for name in inputs:shutil.copy2(root/'third_party/xboxrecomp/src/kernel'/name,k/name)
 symbols=['nv_gpu_shader_source','nv_gpu_rov_source','nv_gpu_report_reset_source','nv_gpu_coverage_source','nv_gpu_clear_source','nv_gpu_geometry_source','nv_gpu_texcopy_source']
 code='#include <stdio.h>\n#include <stdint.h>\n#include "nv2a_gpu_shader.h"\nint main(void){uint64_t h=14695981039346656037ull;\n'
 for symbol in symbols:code+='for(unsigned i=0;i<sizeof('+symbol+')-1;i++){h^=(unsigned char)'+symbol+'[i];h*=1099511628211ull;}\n'
 code+='printf("%llx\\n",(unsigned long long)h);return 0;}\n'
 (k/'probe.c').write_text(code)
 (p/'CMakeLists.txt').write_text('cmake_minimum_required(VERSION 3.20)\nproject(shader_dependency C)\nset(XBOXRECOMP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/xboxrecomp")\n'+snippet+'add_executable(probe "${QPR2_SHADER_ROOT}/probe.c")\nadd_dependencies(probe qpr2_gpu_shader_sources)\n')
 subprocess.run([cmake,'-S',str(p),'-B',str(p/'build')],check=True,stdout=subprocess.DEVNULL)
 def build():
  subprocess.run([cmake,'--build',str(p/'build'),'--parallel','4'],check=True,stdout=subprocess.DEVNULL)
  actual=subprocess.check_output([str(p/'build/probe')],text=True).strip()
  # Compare compiled C array bytes with independently expanded source bytes,
  # including the large ROV source that cannot be a single MSVC literal.
  expected=14695981039346656037
  parsed=parse_embedded((k/'nv2a_gpu_shader.h').read_text())
  for file,symbol in SHADERS:
   source=expand_shader(k,file);assert parsed[symbol]==source
   if len(source.encode())>60000:assert ('static const char '+symbol+'[] =\n{') in (k/'nv2a_gpu_shader.h').read_text()
   for value in source.encode():expected=((expected^value)*1099511628211)&((1<<64)-1)
  assert actual==format(expected,'x')
  return actual
 prior=build()
 for i,name in enumerate(inputs):
  with (k/name).open('a') as f:f.write('\n/* dependency mutation '+str(i)+' */\n')
  # Accommodate build systems with coarse file timestamp granularity.
  os.utime(k/name,ns=(os.stat(k/name).st_atime_ns,os.stat(k/'nv2a_gpu_shader.h').st_mtime_ns+1000000))
  after=build();assert after!=prior,name;prior=after
 header=k/'nv2a_gpu_shader.h';mtime=header.stat().st_mtime_ns;content=header.read_bytes()
 with (k/'probe.c').open('a') as f:f.write('\n/* CPU-only edit */\n')
 assert build()==prior and header.read_bytes()==content and header.stat().st_mtime_ns==mtime
 print('PASS: real parallel CMake builds regenerate all 7 HLSL and 9 shared inputs; CPU-only edits retain shader strings/cache inputs')
