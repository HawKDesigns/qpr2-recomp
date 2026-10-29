#include "../src/qpr2_game_timing.c"
#include "../third_party/xboxrecomp/src/kernel/main_pc_samples.h"
#include <assert.h>
int main(int argc,char **argv) {
 (void)argv;
 if(argc>1) {
  setenv("RECOMP_GAME_TIMING","0",1);
  Qpr2GameTiming token=qpr2_game_timing_begin(0x6f5f0,0xDEADBEEF,0x3C23D70A,4);
  assert(!token.function);qpr2_game_timing_end(&token);
 } else {
  setenv("RECOMP_GAME_TIMING","1",1);
  Qpr2GameTiming outer=qpr2_game_timing_begin(0x6f5f0,0xDEADBEEF,0x3C23D70A,4);
  Qpr2GameTiming inner=qpr2_game_timing_begin(0xd1970,0xDEADBEEF,0x3C23D70A,2);
  assert(outer.function==0x6f5f0 && outer.object==0xDEADBEEF && outer.auxiliary==4);
  assert(inner.slot!=outer.slot);qpr2_game_timing_end(&inner);qpr2_game_timing_end(&outer);
  qpr2_camera_state(&outer,1,2,3,4,5,6);
  assert(s_timing[outer.slot].calls==1 && s_timing[inner.slot].calls==1);
  assert(s_timing[outer.slot].peak_ms>=s_timing[inner.slot].peak_ms);
  /* Aggregate output survives quiet mode without per-call prints. */
  s_timing[outer.slot].window_started=outer.started-5001;
  outer=qpr2_game_timing_begin(0x6f5f0,0xDEADBEEF,0x3C23D70A,4);
  qpr2_game_timing_end(&outer);
 }
 XboxMainPcSamples window={0};
 for(unsigned i=0;i<20;i++)xbox_main_sample_add(&window,i<12?0x6f6d3:(i<17?0xd1970:0x9e940));
 assert(window.total==20 && window.used==3);
 int best=xbox_main_sample_best(&window,0);assert(window.pc[best]==0x6f6d3 && window.hits[best]==12);
 best=xbox_main_sample_best(&window,1);assert(window.pc[best]==0xd1970 && window.hits[best]==5);
 assert(xbox_main_sample_best(&window,7)==-1);
 memset(&window,0,sizeof window);for(unsigned i=0;i<20;i++)xbox_main_sample_add(&window,i);
 assert(window.used==20 && window.total==20 && xbox_main_sample_best(&window,0)==0);
 puts("PASS: nested game wall timings, disabled mode, no guest-memory reads, bounded PC sample window and top-hit ranking");
}
