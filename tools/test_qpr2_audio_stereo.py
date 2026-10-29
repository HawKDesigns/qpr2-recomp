"""Production VP stereo routing, exact optimized filter differential and meter."""
from pathlib import Path
import subprocess,tempfile,os
root=Path(__file__).resolve().parents[1];src=root/'third_party/xboxrecomp/src'
with tempfile.TemporaryDirectory(prefix='qpr2-stereo-') as d:
 exe=Path(d)/'stereo'
 subprocess.run(['gcc','-std=gnu11','-O2','-ffp-contract=off','-ffunction-sections','-fdata-sections','-fsanitize=undefined','-fno-sanitize-recover=all','-Werror=implicit-function-declaration','-I'+str(src),str(root/'tools/test_qpr2_audio_stereo.c'),str(src/'apu/apu_dsp.c'),str(src/'platform/win32_compat.c'),'-Wl,--gc-sections','-lm','-lpthread','-o',str(exe)],check=True)
 for enabled in ['1','0']:
  subprocess.run([str(exe)],env=dict(os.environ,RECOMP_APU_HRTF=enabled),check=True,timeout=30)

 # Optimized-only benchmark: the filter is new enabled work relative to V79.
 subprocess.run(['gcc','-std=gnu11','-O2','-ffp-contract=off','-ffunction-sections','-fdata-sections','-I'+str(src),str(root/'tools/test_qpr2_audio_stereo.c'),str(src/'apu/apu_dsp.c'),str(src/'platform/win32_compat.c'),'-Wl,--gc-sections','-lm','-lpthread','-o',str(exe)],check=True)
 subprocess.run([str(exe),'benchmark'],check=True,timeout=30)
