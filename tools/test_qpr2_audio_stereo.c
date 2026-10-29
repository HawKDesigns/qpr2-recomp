/* Real VP register/sample/mix routing with isolated RAM and host-output metering. */
#include "../third_party/xboxrecomp/src/apu/apu_vp.c"
#include "../third_party/xboxrecomp/src/apu/apu_stereo_meter.h"
#include "../third_party/xboxrecomp/src/apu/apu_output_limit.h"
#include <stdio.h>
#include <time.h>
uint8_t *g_apu_ram_ptr;
struct McpxApuDebug g_dbg,g_dbg_cache;
int g_dbg_voice_monitor=-1;
uint64_t g_dbg_muted_voices[4];
uint8_t *mcpx_apu_phys(uint64_t a){assert(a<0x40000);return g_apu_ram_ptr+a;}
static unsigned rng=1;
static float random_float(void){rng=rng*1664525u+1013904223u;return ((rng>>8)&65535)/32768.0f-1;}
/* The previous ring indexing, independent of the wrapped-decrement path. */
static void reference_filter(HrtfFilter *f,float in[][2],float out[][2]){
 for(int n=0;n<32;n++){
  hrtf_filter_step_parameters(f);
  for(int ch=0;ch<2;ch++){
   float *buf=f->ch[ch].buf,*coeff=f->ch[ch].hrir_coeff_cur;
   buf[f->buf_pos]=in[n][ch];float delay=f->itd_cur*(ch?-1.0f:1.0f);
   if(delay<0)delay=0;int di=(int)delay;float frac=delay-di,acc=0;
   for(int k=0;k<31;k++){
    int index=(f->buf_pos-di-k+HRTF_BUFLEN)%HRTF_BUFLEN;
    float sample=buf[index];
    if(frac>0){int next=(index-1+HRTF_BUFLEN)%HRTF_BUFLEN;sample=sample*(1-frac)+buf[next]*frac;}
    acc+=coeff[k]*sample;
   }
   out[n][ch]=acc;
  }
  f->buf_pos=(f->buf_pos+1)%HRTF_BUFLEN;
 }
}
static void check_filters(void){
 HrtfFilter fast,slow;hrtf_filter_init(&fast);
 float coeff[2][31]={{0}};coeff[0][0]=1;coeff[1][3]=0.5f;
 hrtf_filter_set_target_params(&fast,coeff,0);
 assert(fast.has_target && fast.ch[0].hrir_coeff_tar[0]==1 && fast.ch[1].hrir_coeff_tar[3]==1);
 memset(coeff,0,sizeof coeff);coeff[1][1]=1;
 hrtf_filter_set_target_params(&fast,coeff,0);
 assert(!fast.ch[0].hrir_coeff_tar[0] && fast.ch[1].hrir_coeff_tar[1]==1);
 slow=fast;
 for(int block=0;block<1500;block++){
  if(block%7==0){
   for(int ch=0;ch<2;ch++)for(int k=0;k<31;k++)coeff[ch][k]=random_float();
   float delay=(block%3==0)?42.0f:((block%3==1)?-42.0f:random_float()*42.0f);
   hrtf_filter_set_target_params(&fast,coeff,delay);hrtf_filter_set_target_params(&slow,coeff,delay);
  }
  float in[32][2],a[32][2],b[32][2],alias[32][2];
  for(int i=0;i<32;i++)for(int ch=0;ch<2;ch++)in[i][ch]=random_float();
  memcpy(alias,in,sizeof in);HrtfFilter aliased=fast;
  hrtf_filter_process(&fast,in,a);reference_filter(&slow,in,b);hrtf_filter_process(&aliased,alias,alias);
  assert(!memcmp(a,b,sizeof a) && !memcmp(a,alias,sizeof a));
  assert(!memcmp(&fast,&slow,sizeof fast) && !memcmp(&fast,&aliased,sizeof fast));
 }
 hrtf_filter_clear_history(&fast);
 for(int ch=0;ch<2;ch++)for(int k=0;k<HRTF_BUFLEN;k++)assert(fast.ch[ch].buf[k]==0);
 assert(fast.has_target);
 puts("PASS: both ears update for zero/unity left coefficients; 48000 stereo frames match the reference exactly including in-place, delay edges and history wrap");
}
static void setup_voice(MCPXAPUState *d,unsigned v,int stereo){
 memset(d,0,sizeof *d);memset(g_apu_ram_ptr,0,0x40000);
 qemu_mutex_init(&d->lock);
 d->regs[NV_PAPU_VPVADDR]=0x10000;d->regs[NV_PAPU_VPSGEADDR]=0x1000;
 stl_le_phys(address_space_memory,0x1000,0x20000);
 voice_set_mask(d,v,NV_PAVS_VOICE_CFG_FMT,NV_PAVS_VOICE_CFG_FMT_CONTAINER_SIZE,1);
 voice_set_mask(d,v,NV_PAVS_VOICE_CFG_FMT,NV_PAVS_VOICE_CFG_FMT_SAMPLE_SIZE,1);
 voice_set_mask(d,v,NV_PAVS_VOICE_CFG_FMT,NV_PAVS_VOICE_CFG_FMT_SAMPLES_PER_BLOCK,stereo?1:0);
 voice_set_mask(d,v,NV_PAVS_VOICE_CFG_FMT,NV_PAVS_VOICE_CFG_FMT_STEREO,stereo);
 voice_set_mask(d,v,NV_PAVS_VOICE_CFG_FMT,NV_PAVS_VOICE_CFG_FMT_LOOP,1);
 voice_set_mask(d,v,NV_PAVS_VOICE_PAR_STATE,NV_PAVS_VOICE_PAR_STATE_ACTIVE_VOICE,1);
 voice_set_mask(d,v,NV_PAVS_VOICE_PAR_NEXT,NV_PAVS_VOICE_PAR_NEXT_EBO,255);
 voice_set_mask(d,v,NV_PAVS_VOICE_CFG_VBIN,NV_PAVS_VOICE_CFG_VBIN_V1BIN,1);
 voice_set_mask(d,v,NV_PAVS_VOICE_TAR_VOLA,~0u,~0u);
 voice_set_mask(d,v,NV_PAVS_VOICE_TAR_VOLB,~0u,~0u);
 voice_set_mask(d,v,NV_PAVS_VOICE_TAR_VOLC,~0u,~0u);
 voice_set_mask(d,v,NV_PAVS_VOICE_TAR_VOLA,NV_PAVS_VOICE_TAR_VOLA_VOLUME0,0);
 voice_set_mask(d,v,NV_PAVS_VOICE_TAR_VOLA,NV_PAVS_VOICE_TAR_VOLA_VOLUME1,0);
 for(int i=0;i<4;i++)d->vp.hrtf_submix[i]=(uint8_t)i;
 d->regs[NV_PAPU_FECV]=v;
}
static void finish_voice(MCPXAPUState *d){qemu_mutex_destroy(&d->lock);}
static void check_routes(void){
 MCPXAPUState *d=calloc(1,sizeof *d);assert(d);
 float bins[32][32],monitor[32][2]={{0}};
 /* 2D stereo and mono keep their encoded source-channel mapping. */
 for(int stereo=0;stereo<2;stereo++){
  setup_voice(d,64,stereo);int16_t *pcm=(int16_t*)(g_apu_ram_ptr+0x20000);
  for(int i=0;i<256;i++){pcm[i*(stereo+1)]=1000;if(stereo)pcm[i*2+1]=-2000;}
  memset(bins,0,sizeof bins);voice_process(d,bins,monitor,64,0);
  assert(bins[0][0]==int16_to_float(1000));
  assert(bins[1][0]==int16_to_float(stereo?-2000:1000));
  assert((stereo?d->vp.stereo.stereo_2d:d->vp.stereo.mono_2d)==1);
  for(int b=2;b<32;b++)assert(bins[b][0]==0);
  finish_voice(d);
 }
 /* Guest HRIR writes and voice binding produce separate ears for a mono source. */
 setup_voice(d,0,0);
 ((int16_t*)(g_apu_ram_ptr+0x20000))[0]=16000;
 mcpx_apu_vp_write(d,NV1BA0_PIO_SET_CURRENT_HRTF_ENTRY,0,4);
 mcpx_apu_vp_write(d,NV1BA0_PIO_SET_HRIR,0x00000040,4); /* left tap 0 */
 mcpx_apu_vp_write(d,NV1BA0_PIO_SET_HRIR+4,0x40000000,4); /* right tap 3 */
 mcpx_apu_vp_write(d,NV1BA0_PIO_SET_VOICE_TAR_HRTF,0,4);
 assert(d->vp.filters[0].hrtf.has_target);
 memset(bins,0,sizeof bins);voice_process(d,bins,monitor,0,1);
 if(voice_hrtf_enabled()){
  assert(bins[0][0]!=0 && bins[1][0]==0 && bins[1][3]!=0);
  assert(d->vp.stereo.filtered_3d==1 && !d->vp.stereo.bypass_3d);
 }else{
  assert(!memcmp(bins[0],bins[1],sizeof bins[0]));assert(d->vp.stereo.bypass_3d==1 && !d->vp.stereo.filtered_3d);
 }
 /* Actual DSP mixdown and final limiter preserve the two routed ears. */
 mcpx_apu_dsp_frame(d,bins);
 APUOutputLimit limiter={0};limiter.gain=1;int16_t output[32][2];
 apu_output_limit(&limiter,d->monitor.mix_buf,output,32,1);
 APUStereoMeter meter={0};apu_stereo_meter_add(&meter,output,32);
 assert(meter.nonzero_frames && !limiter.hard_clipped);
 if(voice_hrtf_enabled())assert(meter.unequal_frames && meter.difference_energy>0 && output[0][0]>0 && output[0][1]==0);
 else assert(!meter.unequal_frames && !meter.difference_energy);
 finish_voice(d);
 /* Missing, null and invalid targets keep the established unfiltered route. */
 for(unsigned mode=0;mode<3;mode++){
  setup_voice(d,0,0);((int16_t*)(g_apu_ram_ptr+0x20000))[0]=16000;
  mcpx_apu_vp_write(d,NV1BA0_PIO_SET_VOICE_TAR_HRTF,mode==0?0:mode==1?0xffff:0xfffe,4);
  memset(bins,0,sizeof bins);voice_process(d,bins,monitor,0,1);
  assert(!memcmp(bins[0],bins[1],sizeof bins[0]) && bins[0][0]!=0 && d->vp.stereo.bypass_3d==1);
  finish_voice(d);
 }
 /* Explicitly written zero coefficients are a valid silent filter, not missing data. */
 setup_voice(d,0,0);((int16_t*)(g_apu_ram_ptr+0x20000))[0]=16000;
 mcpx_apu_vp_write(d,NV1BA0_PIO_SET_HRIR,0,4);
 mcpx_apu_vp_write(d,NV1BA0_PIO_SET_VOICE_TAR_HRTF,0,4);
 assert(d->vp.filters[0].hrtf.has_target);
 memset(bins,0,sizeof bins);voice_process(d,bins,monitor,0,1);
 if(voice_hrtf_enabled())for(int i=0;i<32;i++)assert(!bins[0][i] && !bins[1][i]);
 finish_voice(d);free(d);
 puts("PASS: real PCM fetch, guest HRIR writes, mono-to-stereo routing, 2D channel preservation, muted sends, missing/null/invalid targets, written silence and opt-out");
}
static void check_meter(void){
 APUStereoMeter m={0};int16_t stereo[4][2]={{1000,-2000},{-1000,2000},{2000,-4000},{-2000,4000}};
 apu_stereo_meter_add(&m,stereo,4);assert(m.frames==4 && m.unequal_frames==4 && m.nonzero_frames==4);
 assert(fabs(apu_stereo_meter_similarity(&m)+1)<1e-12);
 memset(&m,0,sizeof m);for(int i=0;i<4;i++)stereo[i][1]=stereo[i][0];
 apu_stereo_meter_add(&m,stereo,4);assert(!m.unequal_frames && !m.difference_energy && fabs(apu_stereo_meter_similarity(&m)-1)<1e-12);
 memset(&m,0,sizeof m);memset(stereo,0,sizeof stereo);apu_stereo_meter_add(&m,stereo,4);
 assert(!m.nonzero_frames && !apu_stereo_meter_similarity(&m) && !apu_stereo_meter_rms(m.left_energy,m.frames));
 puts("PASS: meter distinguishes dual mono, opposite polarity stereo and silence without changing samples");
}
static void benchmark_filter(void){
 HrtfFilter initial;hrtf_filter_init(&initial);float coeff[2][31],input[32][2];
 for(int ch=0;ch<2;ch++)for(int k=0;k<31;k++)coeff[ch][k]=random_float();
 for(int i=0;i<32;i++)for(int ch=0;ch<2;ch++)input[i][ch]=random_float();
 hrtf_filter_set_target_params(&initial,coeff,3.5f);
 double seconds[2];float checksum[2]={0};
 for(int mode=0;mode<2;mode++){
  HrtfFilter f=initial;float output[32][2];clock_t start=clock();
  for(int block=0;block<4096;block++){
   if(mode)hrtf_filter_process(&f,input,output);else reference_filter(&f,input,output);
   checksum[mode]+=output[0][0]+output[31][1];
  }
  seconds[mode]=(double)(clock()-start)/CLOCKS_PER_SEC;
 }
 assert(checksum[0]==checksum[1]);
 printf("CPU filter microbenchmark: modulo_ms=%.3f wrapped_ms=%.3f ratio=%.3f (not gameplay FPS)\n",seconds[0]*1000,seconds[1]*1000,seconds[0]/seconds[1]);
}
int main(int argc,char **argv){
 if(argc>1){(void)argv;benchmark_filter();return 0;}
 g_apu_ram_ptr=calloc(1,0x40000);assert(g_apu_ram_ptr);
 check_filters();check_routes();check_meter();free(g_apu_ram_ptr);
}
