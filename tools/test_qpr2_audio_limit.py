"""Actual APU core/VP/DSP compile, end-to-end output and wide-mix checks."""
from pathlib import Path
import subprocess,tempfile,os
root=Path(__file__).resolve().parents[1];src=root/'third_party/xboxrecomp/src'
limit=r'''
#include "apu/apu_output_limit.h"
#include <assert.h>
#include <float.h>
#include <stdio.h>
int main(void){
 APUOutputLimit s={0};s.gain=1;
 float in[48000][2]={{0}};int16_t out[48000][2];
 in[0][0]=1234;in[0][1]=-2468;
 apu_output_limit(&s,in,out,1,1);assert(out[0][0]==1234 && out[0][1]==-2468 && s.gain==1 && !s.overload_frames);
 in[0][0]=120000;in[0][1]=-60000;
 apu_output_limit(&s,in,out,1,1);assert(out[0][0]>=32759 && out[0][0]<=32760 && out[0][1]==-16380 && s.overload_frames==1 && !s.hard_clipped);
 float low=s.gain;in[0][0]=10000;in[0][1]=-5000;
 apu_output_limit(&s,in,out,1,1);assert(s.gain>low && s.gain<low+0.001f && abs(out[0][0]+2*out[0][1])<=1);
 float gain=s.gain;apu_output_limit_reset_counts(&s);assert(s.gain==gain && !s.frames && !s.input_peak);
 apu_output_limit(&s,in,out,48000,1);assert(s.gain==1.0f && !s.hard_clipped);
 in[0][0]=INFINITY;in[0][1]=NAN;apu_output_limit(&s,in,out,1,1);assert(!out[0][0] && !out[0][1] && s.nonfinite==2);
 in[0][0]=FLT_MAX;in[0][1]=-FLT_MAX;apu_output_limit(&s,in,out,1,1);assert(!s.hard_clipped && out[0][0]<=32767 && out[0][1]>=-32768);
 in[0][0]=120000;in[0][1]=-120000;apu_output_limit(&s,in,out,1,0);assert(out[0][0]==32767 && out[0][1]==-32768 && s.hard_clipped==2 && s.gain==1);
 puts("PASS: unity quiet output, overload protection, stereo balance, release, counter reset, nonfinite/huge input and opt-out");
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-audio-limit-') as d:
 p=Path(d);(p/'limit.c').write_text('#include <stdlib.h>\n'+limit)
 flags=['gcc','-std=gnu11','-O2','-ffunction-sections','-fdata-sections','-fsanitize=undefined','-fno-sanitize-recover=all','-Werror=implicit-function-declaration','-I'+str(src),'-I'+str(src/'apu')]
 def run(cmd,**kw):subprocess.run([str(x) for x in cmd],check=True,**kw)
 for name in ['apu_core','apu_vp','apu_dsp']:
  run(flags+['-c',src/'apu'/f'{name}.c','-o',p/f'{name}.o'])
 run(flags+[p/'limit.c','-lm','-o',p/'limit']);run([p/'limit'])
 run(flags+[root/'tools/test_qpr2_audio_output.c',p/'apu_dsp.o',src/'platform/win32_compat.c','-Wl,--gc-sections','-lm','-lpthread','-o',p/'output']);run([p/'output'],env=dict(os.environ,RECOMP_AUDIO_LIMITER='1'));run([p/'output'],env=dict(os.environ,RECOMP_AUDIO_LIMITER='0'))
 run(flags+[root/'third_party/xboxrecomp/tests/apu_mixdown/test_main.c',p/'apu_dsp.o',p/'apu_core.o','-Wl,--gc-sections','-lm','-o',p/'mixdown'])
 for enabled in ['0','1']:run([p/'mixdown'],env=dict(os.environ,RECOMP_APU_MIXDOWN_ALL=enabled,APU_MIXDOWN_EXPECT=enabled))
 for name,export in [('apu_vp','vp'),('apu_dsp','dsp')]:
  run(flags+['-Dmcpx_apu_'+export+'_state_layout=qpr2_test_'+export+'_layout','-c',src/'apu'/f'{name}.c','-o',p/f'{name}-layout.o'])
 run(flags+[root/'tools/test_qpr2_apu_layout.c',p/'apu_core.o',p/'apu_vp-layout.o',p/'apu_dsp-layout.o','-Wl,--gc-sections','-lm','-o',p/'layout']);run([p/'layout'])
 run(['python3',root/'third_party/xboxrecomp/tests/audio_mixer/run.py'])
 print('PASS: production core/VP/DSP compile; output, mixdown arms and existing software playback/cancellation tests')
