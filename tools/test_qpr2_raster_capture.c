/* Exercise the actual transformed batch/raster path with a deterministic
 * vertex-program fixture, including finite geometry beyond int range. */
#include "../third_party/xboxrecomp/src/kernel/nv2a_pb_exec.c"
#include <assert.h>
static uint8_t ram[0x200000];
static unsigned runs;
uint32_t g_xbox_image_lo,g_xbox_image_hi;
ptrdiff_t xbox_GetMemoryOffset(void) {return (ptrdiff_t)ram;}
uint32_t xbox_ContiguousAllocatedBytes(void) {return 0;}
void xbox_FramebufferWindowSet(uint32_t base,uint32_t pitch) {(void)base;(void)pitch;}
void xbox_FramebufferWindowStart(void) {}
int nv2a_vsh_cacheable(void){return 1;}
uint32_t nv2a_vsh_input_mask(void){return 0xffffu;}
void nv2a_vsh_report(void){}
int nv2a_vsh_run(const float in[NV2A_VSH_INPUTS][4],Nv2aVshOutput *out) {
 ++runs;memset(out,0,sizeof(*out));memcpy(out->pos,in[0],sizeof out->pos);
 for(int k=0;k<4;k++)out->d0[k]=1;
 return 1;
}
const float *nv2a_vsh_constant(uint32_t index) {static const float c[4]={1,1,1,1};(void)index;return c;}
void nv2a_vsh_trace_state(void) {}

static void setup(void) {
 memset(ram,0,sizeof ram);memset(&s_gpu,0,sizeof s_gpu);
 s_gpu.format=0x128;s_gpu.color_offset=0x1000;s_gpu.pitch=32;
 s_gpu.clip_w=8;s_gpu.clip_h=8;s_gpu.color_mask=0x01010101;
}
static Nv2aVshOutput point(float x,float y) {
 Nv2aVshOutput p={0};p.pos[0]=x;p.pos[1]=y;p.pos[3]=1;
 p.d0[0]=p.d0[3]=1;return p;
}
void xbox_FramebufferWindowPresentPixels(const uint32_t *p,uint32_t w,uint32_t h) {(void)p;(void)w;(void)h;}
int main(void) {
 const float a[2]={0,0},b[2]={8,0},c[2]={8,8},d[2]={0,8};
 for(int reverse=0;reverse<2;reverse++) {
  setup();s_gpu.blend_enable=1;s_gpu.blend_sfactor=0x302;s_gpu.blend_dfactor=0x303;
  raster_triangle(a,reverse?c:b,reverse?b:c,0x80FF0000,NULL);
  raster_triangle(a,reverse?d:c,reverse?c:d,0x80FF0000,NULL);
  assert(s_gpu.pixels==64);
  for(int i=0;i<64;i++)assert(((uint32_t*)(ram+0x1000))[i]==((uint32_t*)(ram+0x1000))[0]);
 }
 Nv2aVshOutput p[4]={point(0,0),point(8,0),point(8,8),point(0,8)};
 for(int reverse=0;reverse<2;reverse++) {
  setup();s_gpu.blend_enable=1;s_gpu.blend_sfactor=0x302;s_gpu.blend_dfactor=0x303;
  raster_xf_triangle(p,reverse?p+2:p+1,reverse?p+1:p+2);
  raster_xf_triangle(p,reverse?p+3:p+2,reverse?p+2:p+3);
  assert(s_gpu.pixels==64);
  for(int i=0;i<64;i++)assert(((uint32_t*)(ram+0x1000))[i]==((uint32_t*)(ram+0x1000))[0]);
 }
 for(int front=0;front<2;front++)for(int face=0;face<3;face++)for(int reverse=0;reverse<2;reverse++) {
  setup();s_gpu.cull_enable=1;s_gpu.front_face=front?0x901:0x900;
  s_gpu.cull_face=face==0?0x404:face==1?0x405:0x408;
  int culled=face==2 || (face==0 ? reverse==front : reverse!=front);
  raster_triangle(a,reverse?c:b,reverse?b:c,0xFFFFFFFF,NULL);
  assert(s_gpu.tris_culled==(unsigned)culled && s_gpu.tris_drawn==(unsigned)!culled);
  s_gpu.tris_culled=0;raster_xf_triangle(p,reverse?p+2:p+1,reverse?p+1:p+2);
  assert(s_gpu.tris_culled==(unsigned)culled);
 }
 setup();s_gpu.cull_enable=1;s_gpu.cull_face=0x405;s_gpu.front_face=0x900;
 s_gpu.prim=NV_PRIM_TRIANGLE_STRIP;s_gpu.idx_count=4;
 s_gpu.attr[0].offset=0x2000;s_gpu.attr[0].type=2;s_gpu.attr[0].size=4;s_gpu.attr[0].stride=16;
 const float strip[4][4]={{0,0,0,1},{8,0,0,1},{0,8,0,1},{8,8,0,1}};
 memcpy(ram+0x2000,strip,sizeof strip);for(int i=0;i<4;i++)s_gpu.idx[i]=i;
 raster_batch_program();assert(s_gpu.xf_drawn==2 && s_gpu.tris_culled==0 && s_gpu.pixels==64);
 s_gpu.xf_mode=0;s_gpu.tris_drawn=0;s_gpu.tris_culled=0;s_gpu.pixels=0;
 raster_batch();assert(s_gpu.tris_drawn==2 && s_gpu.tris_culled==0 && s_gpu.pixels==64);
 /* Exact finished-frame snapshot stays immutable when the guest target changes. */
 memset(ram+0x3000,0,640*480*4);((uint32_t*)(ram+0x3000))[0]=0xFF123456;
 s_gpu.flips=7;assert(snapshot_presented_frame(0x3000,2560,4));
 memset(ram+0x3000,0,640*480*4);assert(s_completed_frame[0]==0xFF123456);
 setenv("RECOMP_FB_DUMP","/tmp/qpr2-v37-capture-",1);dump_surface_bmp();
 FILE *f=fopen("/tmp/qpr2-v37-capture-00000.bmp","rb");assert(f);
 assert(fseek(f,54+479*640*3,SEEK_SET)==0);unsigned char rgb[3];assert(fread(rgb,1,3,f)==3);
 assert(rgb[0]==0x56 && rgb[1]==0x34 && rgb[2]==0x12);fclose(f);
 uint16_t rgb565[4]={0xF800,0x07E0,0x001F,0xFFFF};uint32_t copy[4];
 assert(nv2a_frame_copy(copy,(uint8_t*)rgb565,4,2,2,2));
 assert(copy[0]==0xFF0000 && copy[1]==0x00FF00 && copy[2]==0x0000FF && copy[3]==0xFFFFFF);
 assert(!nv2a_frame_copy(copy,(uint8_t*)rgb565,3,2,2,2));
 assert(nv2a_frame_write_bmp("/tmp/qpr2-v37-pad.bmp",copy,1,2));
 f=fopen("/tmp/qpr2-v37-pad.bmp","rb");assert(f);fseek(f,0,SEEK_END);assert(ftell(f)==62);fclose(f);
 puts("PASS: actual plain/programmed raster coverage, shared alpha edges once, both cull orientations/faces, strip winding, immutable completed BMP capture, 565 conversion/padding");
}
