/* Exercise the actual transformed batch/raster path with a deterministic
 * vertex-program fixture, including finite geometry beyond int range. */
#include "../third_party/xboxrecomp/src/kernel/nv2a_pb_exec.c"
#include <assert.h>
static uint8_t ram[0x100000];
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
int main(void) {
 setenv("RECOMP_RENDER_TIMING","1",1);
 memset(&s_gpu,0,sizeof s_gpu);
 s_gpu.format=0x128;s_gpu.color_offset=0x1000;s_gpu.pitch=32;
 s_gpu.clip_w=8;s_gpu.clip_h=4;s_gpu.color_mask=0x01010101;
 s_gpu.prim=NV_PRIM_TRIANGLES;s_gpu.idx_count=6;
 for(int k=0;k<6;k++)s_gpu.idx[k]=(uint16_t)(k%3);
 s_gpu.attr[0].offset=0x2000;s_gpu.attr[0].type=2;s_gpu.attr[0].size=4;s_gpu.attr[0].stride=16;
 const float points[3][4]={{-1e10f,0,0,1},{1e10f,0,0,1},{0,1e10f,0,1}};
 memcpy(ram+0x2000,points,sizeof points);
 raster_batch_program();
 assert(runs==3 && s_render_time.batches==1 && s_render_time.vertices==6 && s_render_time.vertex_runs==3 && s_render_time.vertex_reuses==3);
 assert(s_render_time.raster_jobs==4 && s_render_time.parallel_jobs==0);
 assert(s_render_time.bbox_pixels==128 && s_gpu.xf_drawn==4);
 assert(s_render_time.transform_ms>=0 && s_render_time.raster_ms>=0);
 assert(xf_viewport_bound(-1e30f,0,8,0)==0 && xf_viewport_bound(1e30f,0,8,1)==8);
 assert(xf_viewport_bound(2.25f,0,8,0)==2 && xf_viewport_bound(2.25f,0,8,1)==3);
 Nv2aVshOutput bad=s_xf[0];bad.pos[0]=INFINITY;
 raster_xf_triangle(&bad,&s_xf[1],&s_xf[2]);
 assert(s_gpu.xf_degenerate==1 && s_render_time.raster_jobs==4);
 puts("PASS: actual batch wall-time accounting, finite huge triangle viewport clipping before int conversion, nonfinite rejection, bounded raster work");
}
