"""Actual Win32 batch workers: order/lifetime/queue limits and textured timing.

An optional previous renderer path measures the same small-triangle workload.
Reported times use Linux pthread API shims, not the Windows game or its FPS.
"""
from pathlib import Path
import sys
import test_qpr2_raster_workers as workers

args = sys.argv[1:]
sanitize = '--sanitize' in args
if sanitize:
    args.remove('--sanitize')
renderer = Path(args[0]).resolve() if args else workers.root/'third_party/xboxrecomp/src/kernel/nv2a_pb_exec.c'
fixture = r'''
static void begin_batch(void) {
#ifdef NV_RASTER_BATCH_JOBS
 assert(!s_xf_job_count && !s_xf_batch_active);s_xf_batch_active=1;
#endif
}
static void end_batch(void) {
#ifdef NV_RASTER_BATCH_JOBS
 xf_flush_jobs();s_xf_batch_active=0;assert(!s_xf_job_count);
#endif
}
static void setup(void) {
 memset(ram,0,sizeof ram);memset(&s_gpu,0,sizeof s_gpu);
 s_gpu.format=0x128;s_gpu.color_offset=0x1000;s_gpu.pitch=1024;
 s_gpu.clip_w=s_gpu.clip_h=256;s_gpu.color_mask=0x01010101;
 s_gpu.depth_test=s_gpu.depth_mask=1;s_gpu.depth_func=0x203;s_gpu.zeta_offset=0x180000;
 s_gpu.zpass_enable=1;
 float*z=zbuf_current(1);for(size_t i=0;i<(size_t)NV_ZBUF_W*NV_ZBUF_H;i++)z[i]=100;
}
static Nv2aVshOutput point(float x,float y,float z,unsigned i) {
 Nv2aVshOutput v={0};v.pos[0]=x;v.pos[1]=y;v.pos[2]=z;v.pos[3]=0.5f+(i%7)*0.25f;
 v.d0[0]=(i%5)*0.2f;v.d0[1]=(i%3)*0.4f;v.d0[2]=(i%7)*0.15f;v.d0[3]=0.25f+(i%3)*0.25f;
 return v;
}
static void ordered_geometry(int queued) {
 s_gpu.blend_enable=1;s_gpu.blend_sfactor=0x302;s_gpu.blend_dfactor=0x303;s_gpu.blend_equation=0x8006;
 s_gpu.alpha_test=1;s_gpu.alpha_func=0x204;s_gpu.alpha_ref=63;
 s_gpu.clip_z_seen=3;s_gpu.clip_z_min=12;s_gpu.clip_z_max=48;
 if(queued)begin_batch();
 /* More than 512 jobs, unequal miny, overlapping translucent/depth-tested
  * pixels and clipped fan vertices whose stack storage expires immediately. */
 for(unsigned i=0;i<1100;i++) {
  int x=(int)(i*37%283)-25,y=(int)(i*19%277)-21;
  Nv2aVshOutput a=point(x,y,8+i%31,i),b=point(x+49,y+1,20+i%39,i+1),c=point(x+7,y+57,30+i%27,i+2);
  if(i%13==0)a.pos[3]=-0.25f;
  if(i%11==0)a.d0[3]=b.d0[3]=c.d0[3]=0;
  raster_xf_clipped(&a,&b,&c);
 }
 if(queued)end_batch();
 /* A later state change/draw must see every pixel/count of the completed
  * queue; workers cannot escape its end into this following depth-only pass. */
 s_gpu.blend_enable=s_gpu.alpha_test=0;s_gpu.color_mask=0;s_gpu.depth_mask=1;
 Nv2aVshOutput a=point(2,1,10,0),b=point(230,3,30,1),c=point(99,243,40,2);
 raster_xf_clipped(&a,&b,&c);
}
static uint32_t hash_bytes(const void*p,size_t n) {
 const uint8_t*b=p;uint32_t h=2166136261u;for(size_t i=0;i<n;i++)h=(h^b[i])*16777619u;return h;
}
static uint32_t seed=0x9e3779b9;
static uint32_t random_word(void) {seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed;}
static Texture texture(unsigned i) {
 Texture t={0};t.offset=0x100000+i*0x40000;t.width=t.height=512;
 t.color=0xf;t.addr_u=t.addr_v=1;t.filter=0x02020000;t.valid=1;return t;
}
static void textured_setup(void) {
 setup();s_gpu.depth_test=s_gpu.depth_mask=0;
 for(size_t i=0x100000;i<0x180000;i++)ram[i]=(uint8_t)random_word();
 s_gpu.rc_seen=1;s_gpu.texs[0]=texture(0);s_gpu.texs[1]=texture(1);
 s_gpu.rc.control=2;s_gpu.rc.stage_program=1|(1u<<5);
 s_gpu.rc.color_icw[0]=0x08040000;s_gpu.rc.color_icw[1]=0x090c0000;
 s_gpu.rc.alpha_icw[0]=0x18140000;s_gpu.rc.alpha_icw[1]=0x191c0000;
 s_gpu.rc.color_ocw[0]=s_gpu.rc.color_ocw[1]=0xc0;
 s_gpu.rc.alpha_ocw[0]=s_gpu.rc.alpha_ocw[1]=0xc0;
}
static Nv2aVshOutput textured_vertex(float x,float y,float u,float v) {
 Nv2aVshOutput p=point(x,y,10,0);p.pos[3]=1;
 for(int k=0;k<4;k++)p.d0[k]=1;
 p.tex[0][0]=u;p.tex[0][1]=v;p.tex[0][3]=1;
 p.tex[1][0]=v;p.tex[1][1]=u;p.tex[1][3]=1;return p;
}
static void terrain_draw(int size) {
 begin_batch();
 for(int i=0;i<15;i++) {
  float x=(i%5)*size,y=(i/5)*size,u=x/64,v=y/64,du=size/64.0f;
  Nv2aVshOutput a=textured_vertex(x,y,u,v),b=textured_vertex(x+size,y,u+du,v);
  Nv2aVshOutput c=textured_vertex(x+size,y+size,u+du,v+du),d=textured_vertex(x,y+size,u,v+du);
  raster_xf_clipped(&a,&b,&c);raster_xf_clipped(&a,&c,&d);
 }
 end_batch();
}
static void actual_program_draw(void) {
 setup();s_gpu.depth_test=s_gpu.depth_mask=0;s_gpu.xf_mode=2;s_gpu.prim=NV_PRIM_TRIANGLES;
 s_gpu.attr[0].offset=0x60000;s_gpu.attr[0].type=2;s_gpu.attr[0].size=4;s_gpu.attr[0].stride=16;
 uint32_t pos[4]={0,0x0020001b,0x08000000,0x0000f800},color[4]={0,0x0020001b,0x0c000000,0x0000f819};
 const float white[4]={1,1,1,1};nv2a_vsh_set_instruction(0,pos);nv2a_vsh_set_instruction(1,color);
 nv2a_vsh_set_constant(0,white);nv2a_vsh_set_start_slot(0);
 for(unsigned i=0;i<32;i++) {
  float x=(i%8)*16,y=(i/8)*16;
  const float p[3][4]={{x,y,10,1},{x+16,y,10,1},{x,y+16,10,1}};
  memcpy(ram+0x60000+i*48,p,sizeof p);
  for(unsigned j=0;j<3;j++)s_gpu.idx[i*3+j]=i*3+j;
 }
 s_gpu.idx_count=96;memset(&s_render_time,0,sizeof s_render_time);raster_batch_program();
 assert(s_gpu.xf_pixels>0 && s_gpu.tris_drawn==32);
#ifdef NV_RASTER_BATCH_JOBS
 assert(s_render_time.batch_flushes==1 && s_render_time.queued_jobs==32);
 if(s_pool.n>1)assert(s_render_time.parallel_jobs==1 && s_render_time.parallel_triangles==32);
 assert(!s_xf_job_count && !s_xf_batch_active);
#endif
}
static void feedback_draw(int queued) {
 setup();s_gpu.depth_test=s_gpu.depth_mask=0;
 for(unsigned i=0;i<256*256;i++)((uint32_t*)(ram+0x1000))[i]=0xff000000u|(i*19);
 Texture t={0};t.offset=0x1000;t.color=0x12;t.width=t.height=256;t.pitch=1024;
 t.addr_u=t.addr_v=3;t.valid=1;s_gpu.texs[0]=t;
 if(queued)begin_batch();
 for(int i=0;i<8;i++) {
  Nv2aVshOutput a=textured_vertex(2,2+i*3,0,1),b=textured_vertex(140,3+i*3,100,2);
  Nv2aVshOutput c=textured_vertex(30,160+i*3,10,80);
  raster_xf_clipped(&a,&b,&c);
 }
 if(queued)end_batch();
}
int main(void) {
 setenv("RECOMP_RASTER_THREADS","8",1);setenv("RECOMP_RENDER_TIMING","1",1);
 assert(raster_pool_size()==8);
 setup();s_pool.n=1;ordered_geometry(0);
 uint8_t*reference=malloc(256*256*4);float*depth=malloc(NV_ZBUF_W*NV_ZBUF_H*4);assert(reference && depth);
 memcpy(reference,ram+0x1000,256*256*4);memcpy(depth,zbuf_current(0),NV_ZBUF_W*NV_ZBUF_H*4);
 uint64_t pixels=s_gpu.xf_pixels,fails=s_gpu.xf_depth_fail;uint32_t zpass=s_gpu.zpass_count,max=s_gpu.pixel_max,tris=s_gpu.tris_drawn;
 for(int pass=0;pass<8;pass++) {
  setup();s_pool.n=1<<(pass%4);ordered_geometry(1);
  assert(!memcmp(reference,ram+0x1000,256*256*4));
  assert(!memcmp(depth,zbuf_current(0),NV_ZBUF_W*NV_ZBUF_H*4));
  assert(s_gpu.xf_pixels==pixels && s_gpu.xf_depth_fail==fails && s_gpu.zpass_count==zpass);
  assert(s_gpu.pixels==pixels && s_gpu.pixel_max==max && s_gpu.tris_drawn==tris);
 }
 s_pool.n=1;actual_program_draw();memcpy(reference,ram+0x1000,256*256*4);
 uint64_t program_pixels=s_gpu.xf_pixels;
 s_pool.n=8;actual_program_draw();
 assert(!memcmp(reference,ram+0x1000,256*256*4) && s_gpu.xf_pixels==program_pixels);
 s_pool.n=1;feedback_draw(0);memcpy(reference,ram+0x1000,256*256*4);
 s_pool.n=8;memset(&s_render_time,0,sizeof s_render_time);feedback_draw(1);
 assert(!memcmp(reference,ram+0x1000,256*256*4) && !s_render_time.parallel_jobs);
 textured_setup();s_pool.n=1;terrain_draw(32);memcpy(reference,ram+0x1000,256*256*4);
 uint64_t texture_pixels=s_gpu.xf_pixels;uint32_t texture_max=s_gpu.pixel_max;
 setup(); /* reset caches/state before reconstructing the same deterministic texture */
 seed=0x9e3779b9;textured_setup();s_pool.n=8;terrain_draw(32);
 assert(!memcmp(reference,ram+0x1000,256*256*4));
 assert(s_gpu.xf_pixels==texture_pixels && s_gpu.pixel_max==texture_max);
 /* Warm persistent workers and TLS decode caches before measuring. */
 for(int workload=0;workload<2;workload++) {
  int size=workload?32:16,draws=workload?200:800;
  for(int i=0;i<5;i++)terrain_draw(size);
  memset(&s_render_time,0,sizeof s_render_time);s_gpu.pixels=s_gpu.xf_pixels=0;
  double begin=render_wall_ms();for(int i=0;i<draws;i++)terrain_draw(size);double elapsed=render_wall_ms()-begin;
  assert(s_gpu.xf_pixels==(uint64_t)draws*15*size*size && s_gpu.pixels==s_gpu.xf_pixels);
  printf("{\"triangle_size\":%d,\"terrain_ms\":%.3f,\"dispatches\":%llu,\"threads_dispatched\":%llu,\"pixel_checksum\":%u,\"pixels\":%llu}\n",
   size,elapsed,(unsigned long long)s_render_time.parallel_jobs,(unsigned long long)s_render_time.worker_threads,
   hash_bytes(ram+0x1000,256*256*4),(unsigned long long)s_gpu.xf_pixels);
 }
 free(reference);free(depth);
 puts("PASS: ordered overlapping/clipped/alpha/depth geometry, owned clip vertices, 512-job queue flushes, 1/2/4/8 thread transitions, actual program draw, framebuffer-texture feedback, bilinear DXT/combiner bytes and counts");
}
'''
if __name__ == '__main__':
    workers.run_fixture(renderer, fixture, sanitize)
