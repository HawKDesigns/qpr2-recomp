"""Real executor bridge + shared GPU pixel program using a deferred GPU mock.

This verifies portable arithmetic, tile ownership, fallback and completion
ordering. The Windows D3D11 hardware check is separately run at startup.
"""
import test_qpr2_raster_batches as batches
import test_qpr2_raster_workers as workers
from pathlib import Path
fixture = batches.fixture[:batches.fixture.index('int main(void)')]
fixture += r'''
static int mock_enabled, mock_active,mock_clear_enabled;
static unsigned mock_clears;
static uint8_t *mock_color;
static float *mock_depth;
static unsigned mock_stride;
static NvGpuState mock_state;
static uint32_t shadow_color[1024*1024];
static float shadow_depth[1024*1024];
static NvGpuCount mock_count;
static NvGpuTextureSource mock_sources[4];
static unsigned mock_submits,mock_syncs;
static uint32_t gpu_load_texel(NvGpuTexture meta,int st,int face,int x,int y) {
 (void)meta;uint32_t value;
 assert(mock_sources[st].decode(mock_sources[st].opaque,face,x,y,&value));return value;
}
#include "nv2a_gpu_pixel.inc"
#include "nv2a_gpu_shade.inc"
static size_t mock_offset(NvGpuState s,unsigned x,unsigned y) {
 if(((s.format>>8)&15)==2)return swizzle_offset(x,y,1u<<((s.format>>16)&15),1u<<((s.format>>24)&15))*s.bpp;
 return (size_t)y*s.pitch+(size_t)x*s.bpp;
}
static void mock_complete(void) {
 if(!mock_active)return;
 ++mock_syncs;
 for(unsigned y=0;y<mock_state.height;y++)for(unsigned x=0;x<mock_state.width;x++) {
  unsigned at=y*mock_state.width+x;uint32_t c=shadow_color[at];uint8_t*p=mock_color+mock_offset(mock_state,x,y);
  if(mock_state.bpp==4)memcpy(p,&c,4);
  else {uint16_t v=(uint16_t)(((c>>8)&0xF800)|((c>>5)&0x7E0)|((c>>3)&31));memcpy(p,&v,2);}
  if(mock_depth)mock_depth[(size_t)y*mock_stride+x]=shadow_depth[at];
 }
 mock_active=0;
}
int nv_gpu_geometry_available(void){return 0;}
int nv_gpu_draw_raw(const NvGpuState*s,const NvRawInput*i,uint8_t*c,float*d,unsigned stride,const NvGpuTextureSource t[4])
{(void)s;(void)i;(void)c;(void)d;(void)stride;(void)t;return 0;}
int nv_gpu_sync(NvGpuCount *count) {
 mock_complete();*count=mock_count;memset(&mock_count,0,sizeof mock_count);return 1;
}
int nv_gpu_sync_named(NvGpuCount *count,const char *reason){(void)reason;return nv_gpu_sync(count);}
int nv_gpu_reset_report(void){if(!mock_enabled)return 0;mock_count.zpass=0;return 1;}
/* This fixture keeps the conservative sync path; the production range proof
 * and executor integration are exercised by test_qpr2_disjoint_clear.py. */
int nv_gpu_cpu_write_disjoint(const void*p,size_t n){(void)p;(void)n;return 0;}
int nv_gpu_clear_color(const NvGpuState*s,uint8_t*c,unsigned l,unsigned t,unsigned r,unsigned b,uint32_t v)
{
 if(!mock_clear_enabled || !mock_active || c!=mock_color || s->width!=mock_state.width || s->height!=mock_state.height ||
    s->pitch!=mock_state.pitch || s->bpp!=mock_state.bpp || s->format!=mock_state.format || r>s->width || b>s->height)return 0;
 if(s->bpp==2)v=0xFF000000u|(v&0xF80000u)|(v&0xFC00u)|(v&0xF8u);
 for(unsigned y=t;y<b;y++)for(unsigned x=l;x<r;x++)shadow_color[y*s->width+x]=v;
 ++mock_clears;return 1;
}
int nv_gpu_clear_depth(float*d,unsigned stride,float v){
 if(!mock_clear_enabled || !mock_active || d!=mock_depth || stride!=mock_stride)return 0;
 for(unsigned i=0;i<mock_state.width*mock_state.height;i++)shadow_depth[i]=v;
 ++mock_clears;return 1;
}
void nv_gpu_report(void){}
void nv_gpu_capture_request(void){}
void nv_gpu_capture_present(const uint32_t*p,unsigned w,unsigned h){(void)p;(void)w;(void)h;}
int nv_gpu_submit(void){return 1;}
int nv_gpu_has_pending_work(void){return mock_active;}
int nv_gpu_guest_read(uint64_t a,uint64_t n){(void)a;(void)n;mock_complete();return 1;}
int nv_gpu_guest_access(uint64_t a,uint64_t n){(void)a;(void)n;return 1;}
int xbox_Nv2aResolveSoftwareEvent(uint32_t s,uint32_t v,uint32_t*a){(void)s;(void)v;(void)a;return 0;}
int xbox_Nv2aReleaseResolvedSoftwareEvent(uint32_t s,uint32_t v,uint32_t a){(void)s;(void)v;(void)a;return 0;}
int nv_gpu_available(void){return 1;}
int nv_gpu_draw(const NvGpuState*s,const NvGpuTri*tri,unsigned n,uint8_t*color,float*depth,unsigned stride,const NvGpuTextureSource sources[4]) {
 if(!mock_enabled)return 0;
 assert(s->width<=1024 && s->height<=1024);
 static NvGpuTile tiles[NV_GPU_MAX_TILES];static uint32_t refs[NV_GPU_MAX_REFERENCES];unsigned count,numrefs;
 assert(nv_gpu_bin(tri,n,s->width,s->height,tiles,&count,refs,NV_GPU_MAX_REFERENCES,&numrefs));
 if(mock_active && (mock_color!=color || mock_depth!=depth || mock_state.width!=s->width || mock_state.height!=s->height || mock_state.bpp!=s->bpp))mock_complete();
 if(!mock_active) {
  mock_color=color;mock_depth=depth;mock_stride=stride;mock_state=*s;
  for(unsigned y=0;y<s->height;y++)for(unsigned x=0;x<s->width;x++) {
   unsigned at=y*s->width+x;uint8_t*p=color+mock_offset(*s,x,y);uint32_t v;
   if(s->bpp==4)memcpy(&v,p,4);
   else {uint16_t c;memcpy(&c,p,2);v=0xff000000u|((c&0xf800)<<8)|((c&0x7e0)<<5)|((c&31)<<3);}
   shadow_color[at]=v;shadow_depth[at]=depth?depth[(size_t)y*stride+x]:3.4e38f;
  }
  mock_active=1;
 }
 memcpy(mock_sources,sources,sizeof mock_sources);++mock_submits;
 /* Process tiles in reverse order to stress independent pixel ownership.
  * Each tile's primitive references must still be in original draw order. */
 for(int tile=(int)count-1;tile>=0;tile--) {
  NvGpuTile q=tiles[tile];
  for(unsigned j=1;j<q.count;j++)assert(refs[q.first+j]>refs[q.first+j-1]);
  for(unsigned y=q.y;y<q.y+16 && y<s->height;y++)for(unsigned x=q.x;x<q.x+16 && x<s->width;x++) {
   unsigned at=y*s->width+x;
   for(unsigned j=0;j<q.count;j++) {
    NvGpuPixel p=gpu_pixel(*s,tri[refs[q.first+j]],x,y,shadow_color[at],shadow_depth[at]);
    shadow_color[at]=p.color;shadow_depth[at]=p.depth;
    mock_count.pixels+=p.pixels;mock_count.depth_fail+=p.depth_fail;mock_count.zpass+=p.zpass;
    if((p.pixel_max&0xFFFFFF)>(mock_count.pixel_max&0xFFFFFF))mock_count.pixel_max=p.pixel_max;
   }
  }
 }
 return 1;
}
int xbox_Nv2aReleaseFence(uint32_t a,uint32_t b,uint32_t c) {
 (void)a;(void)b;(void)c;assert(!mock_active);return 1;
}
int xbox_Nv2aResolveFence(uint32_t c,uint32_t o,uint32_t*a){(void)c;(void)o;(void)a;return 0;}
int xbox_Nv2aReleaseResolvedFence(uint32_t c,uint32_t o,uint32_t a,uint32_t v){(void)c;(void)o;(void)a;(void)v;assert(0);return 0;}
int xbox_Nv2aHandleSoftwareMethod(uint32_t a,uint32_t b) {
 (void)a;(void)b;assert(!mock_active);return 1;
}
static void counters(uint64_t *p) {
 p[0]=s_gpu.pixels;p[1]=s_gpu.xf_depth_fail;p[2]=s_gpu.zpass_count;p[3]=s_gpu.tris_drawn;
}
static void combinations(void) {
 for(unsigned i=0;i<20000;i++) {
  Nv2aCombiner rc={0};float a[4],b[4],f[4],t[4][4],expected[4],actual[4];
  for(int k=0;k<4;k++) {a[k]=(float)(random_word()%400)*0.01f-2;b[k]=(float)(random_word()%400)*0.01f-2;
   f[k]=(float)(random_word()%400)*0.01f-2;for(int st=0;st<4;st++)t[st][k]=(float)(random_word()%400)*0.01f-2;}
  rc.control=(i%9)|((i&7)<<8)|0x11000;rc.stage_program=i%32;
  for(int st=0;st<8;st++) {
   rc.color_icw[st]=random_word();rc.alpha_icw[st]=random_word();
   rc.color_ocw[st]=random_word();rc.alpha_ocw[st]=random_word();
   rc.factor0[st]=random_word();rc.factor1[st]=random_word();
  }
  if(i%2){rc.final0=random_word();rc.final1=random_word();rc.final_c0=random_word();rc.final_c1=random_word();}
  nv2a_rc_eval(&rc,a,b,f,(const float(*)[4])t,expected);gpu_rc_eval(rc,a,b,f,t,actual);
  assert(!memcmp(expected,actual,sizeof expected));
 }
}
int main(void) {
 setenv("RECOMP_RASTER_THREADS","1",1);setenv("RECOMP_GPU_RASTER","1",1);setenv("RECOMP_RENDER_TIMING","1",1);
 combinations();
 uint8_t *reference=malloc(256*256*4);float*depth=malloc(NV_ZBUF_W*NV_ZBUF_H*4);assert(reference && depth);
 for(int mode=0;mode<3;mode++) {
  mock_enabled=0;setup();
  if(mode==1){s_gpu.format=0x121;s_gpu.pitch=512;}
  if(mode==2){s_gpu.control0=1u<<12;}
  ordered_geometry(1);gpu_finish();
  size_t bytes=mode==1?256*256*2:256*256*4;
  memcpy(reference,ram+0x1000,bytes);memcpy(depth,zbuf_current(0),NV_ZBUF_W*NV_ZBUF_H*4);
  uint64_t counts[4],actual[4];counters(counts);
  setup();if(mode==1){s_gpu.format=0x121;s_gpu.pitch=512;}if(mode==2){s_gpu.control0=1u<<12;}
  mock_enabled=1;ordered_geometry(1);gpu_finish();counters(actual);
  assert(!memcmp(reference,ram+0x1000,bytes));assert(!memcmp(depth,zbuf_current(0),NV_ZBUF_W*NV_ZBUF_H*4));
  assert(!memcmp(counts,actual,sizeof counts));
 }
 for(unsigned mode=0;mode<3;mode++) {
  uint64_t expected_count[4],actual_count[4];
  for(unsigned hardware=0;hardware<2;hardware++) {
   mock_enabled=hardware;mock_clear_enabled=hardware;setup();
   if(mode==1){s_gpu.format=0x121;s_gpu.pitch=512;}
   if(mode==2)s_gpu.format=0x08080228; /* 256x256 swizzled color */
   ordered_geometry(1);
   s_gpu.color_mask=0x01010101;begin_batch();
   Nv2aVshOutput pa=point(1,1,20,1),pb=point(255,1,20,2),pc=point(1,255,20,3);
   raster_xf_clipped(&pa,&pb,&pc);end_batch();
   unsigned reads=mock_syncs,clears=mock_clears;
   s_gpu.clip_x=3;s_gpu.clip_w=250;s_gpu.clear_color=0x4D89ABCD;
   nv2a_pb_exec_method(0,NV097_CLEAR_SURFACE,NV097_CLEAR_COLOR_MASK|1);
   if(hardware)assert(mock_active && mock_syncs==reads && mock_clears==clears+2);
   s_gpu.clip_x=0;s_gpu.clip_w=256;ordered_geometry(1);
   gpu_finish();size_t bytes=mode==1?256*256*2:256*256*4;
   if(!hardware){memcpy(reference,ram+0x1000,bytes);memcpy(depth,zbuf_current(0),NV_ZBUF_W*NV_ZBUF_H*4);counters(expected_count);}
   else{counters(actual_count);assert(!memcmp(reference,ram+0x1000,bytes) && !memcmp(depth,zbuf_current(0),NV_ZBUF_W*NV_ZBUF_H*4) && !memcmp(expected_count,actual_count,sizeof expected_count));}
  }
 }
 mock_clear_enabled=0;
 puts("PASS: actual executor ordered partial color/full depth clears match CPU in ARGB8, RGB565 and swizzled targets without an intermediate readback");
 mock_enabled=0;seed=0x9e3779b9;textured_setup();terrain_draw(32);gpu_finish();memcpy(reference,ram+0x1000,256*256*4);
 uint64_t counts[4],actual[4];counters(counts);
 mock_enabled=1;seed=0x9e3779b9;textured_setup();terrain_draw(32);
 assert(mock_active); /* submitted GPU pixels are deliberately not in RAM yet */
 nv2a_pb_exec_method(0,0x1D70,9);assert(!mock_active);counters(actual);
 assert(!memcmp(reference,ram+0x1000,256*256*4) && !memcmp(counts,actual,sizeof counts));
 terrain_draw(16);assert(mock_active);nv2a_pb_exec_method(0,0x0100,5);assert(!mock_active);
 terrain_draw(16);assert(mock_active);nv2a_pb_exec_method(0,0x17C8,0);assert(mock_active && !s_gpu.zpass_count);
 terrain_draw(16);assert(mock_active);nv2a_pb_exec_method(0,0x17D0,0x50000);assert(!mock_active);
 assert(BRIDGE_MEM32(0x50008)==s_gpu.zpass_count && s_gpu.zpass_count==15*16*16);
 terrain_draw(16);assert(mock_active);nv2a_pb_exec_method(0,NV097_FLIP_INCREMENT_WRITE,0);assert(!mock_active);
 terrain_draw(16);assert(mock_active);gpu_finish();assert(!mock_active);
 terrain_draw(16);assert(mock_active);unsigned diagnostic_reads=mock_syncs;
 unsetenv("RECOMP_TEX_DUMP");unsetenv("RECOMP_FB_DUMP");
 dump_texture_bmp(0);dump_surface_bmp();
 assert(mock_active && mock_syncs==diagnostic_reads);gpu_finish();
 puts("PASS: disabled texture/frame dumps do not drain pending GPU work");
 /* CPU feedback stays serial and cannot observe pending GPU pixels. */
 feedback_draw(1);gpu_finish();assert(!mock_active);
 unsigned before=mock_submits;mock_enabled=0;setup();begin_batch();
 Nv2aVshOutput a=point(0,0,10,1),b=point(200,0,10,1),c=point(20,200,10,1);
 raster_xf_clipped(&a,&b,&c);end_batch();assert(mock_submits==before && s_gpu.pixels>0);
 /* Actual completed-frame snapshot consumes coherent GPU output. */
 setup();mock_enabled=1;s_gpu.clip_w=640;s_gpu.clip_h=480;s_gpu.pitch=2560;
 begin_batch();a=point(0,0,10,1);b=point(640,0,10,1);c=point(0,480,10,1);
 raster_xf_clipped(&a,&b,&c);end_batch();assert(mock_active);
 nv2a_pb_exec_method(0,NV097_FLIP_STALL,0);
 assert(!mock_active && s_completed_frame && s_completed_frame[0]==pack_color(a.d0));
 /* Guard capacity and edge tiles independently of the renderer. */
 NvGpuTile tile[NV_GPU_MAX_TILES];uint32_t refs[4];unsigned nt,nr;NvGpuTri q={0};q.maxx=q.maxy=17;
 assert(!nv_gpu_bin(&q,1,17,17,tile,&nt,refs,3,&nr));
 assert(nv_gpu_bin(&q,1,17,17,tile,&nt,refs,4,&nr) && nt==4 && nr==4);
 printf("PASS: actual executor GPU bridge; 20k combiner programs; ordered clipped/alpha/blend/depth color bytes, Z24/F24 and RGB565; bilinear DXT; deferred completion before fences/events/reports/flips; CPU fallback; tile capacity/ownership (%u submits, %u readbacks)\n",mock_submits,mock_syncs);
 free(reference);free(depth);
}
'''
if __name__ == '__main__':
    for registers in [[],['NV_GPU_FIXED_REGISTERS=1']]:
      workers.run_fixture(workers.root/'third_party/xboxrecomp/src/kernel/nv2a_pb_exec.c', fixture, True,
                        [workers.root/'third_party/xboxrecomp/src/kernel/nv2a_gpu_tiles.c'],
                        ['NV2A_ENABLE_GPU_RASTER=1']+registers)
