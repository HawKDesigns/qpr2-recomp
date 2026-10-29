"""Run an archive-derived program through the actual CPU renderer wrapper."""
import re
import test_qpr2_raster_batches as batches
import test_qpr2_raster_workers as workers

header = (workers.root / 'third_party/xboxrecomp/src/kernel/nv2a_vsh_compiled.h').read_text()
words = re.search(r'nv_vsh_words_0\[\]\[4\]=\{(.*?)\n\};', header, re.S)[1]
fixture = batches.fixture[:batches.fixture.index('int main(void)')] + '\nstatic const uint32_t asset_program[][4]={' + words + r'''
};
static void asset_draw(unsigned prim,unsigned scene,int interpreted){
 setup();s_pool.n=1;s_gpu.pitch=128;s_gpu.clip_w=s_gpu.clip_h=32;
 s_gpu.prim=prim;s_gpu.xf_mode=2;s_gpu.idx_count=24;
 s_gpu.clip_z_seen=3;s_gpu.clip_z_min=10;s_gpu.clip_z_max=80;
 const float one[4]={1,1,1,1},zero[4]={0},fog[4]={0,1,0,1};
 nv2a_vsh_set_constant(58,one);nv2a_vsh_set_constant(59,zero);
 nv2a_vsh_set_constant(101,one);nv2a_vsh_set_constant(186,fog);
 for(unsigned row=0;row<4;++row){float c[4]={0};c[row]=1;nv2a_vsh_set_constant(96+row,c);}
 for(unsigned j=0;j<sizeof asset_program/sizeof asset_program[0];++j)nv2a_vsh_set_instruction(j+5,asset_program[j]);
 nv2a_vsh_set_start_slot(5);nv2a_vsh_set_cxt_write(interpreted);
 assert(nv2a_vsh_input_mask()==(interpreted?0xffffu:0x3du));
 s_gpu.attr[0].offset=0x60000;s_gpu.attr[0].type=2;s_gpu.attr[0].size=4;s_gpu.attr[0].stride=16;
 s_gpu.attr[2].offset=0x61000;s_gpu.attr[2].type=2;s_gpu.attr[2].size=4;s_gpu.attr[2].stride=16;
 for(unsigned i=0;i<12;++i){
  float p[4]={(float)((i*13+scene*3)%60)-14,(float)((i*17+scene*5)%60)-14,20+(float)(i*3),1};
  if(scene%4==1)p[0]=-30-(float)i;
  if(scene%4==2)p[3]=(i%2)?-.5f:1;
  if(scene%4==3){p[0]=(i&1)?32:0;p[1]=(i&2)?32:0;}
  const float color[4]={i/12.0f,1-i/12.0f,.5f,.75f};
  memcpy(ram+0x60000+i*16,p,16);memcpy(ram+0x61000+i*16,color,16);
 }
 for(unsigned i=0;i<24;++i)s_gpu.idx[i]=i%12;
 s_gpu.blend_enable=1;s_gpu.blend_sfactor=0x302;s_gpu.blend_dfactor=0x303;s_gpu.blend_equation=0x8006;
 raster_batch_program();
}
int main(void){
 setenv("RECOMP_RASTER_THREADS","1",1);setenv("RECOMP_RENDER_TIMING","1",1);
 const unsigned prims[]={NV_PRIM_TRIANGLES,NV_PRIM_TRIANGLE_STRIP,NV_PRIM_TRIANGLE_FAN,NV_PRIM_POLYGON,NV_PRIM_QUADS,NV_PRIM_QUAD_STRIP};
 uint8_t color[32*128];float depth[32*32];Nv2aVshOutput vertices[24];unsigned checked=0;
 for(unsigned scene=0;scene<24;++scene){
  asset_draw(prims[scene%6],scene,1);memcpy(color,ram+0x1000,sizeof color);memcpy(vertices,s_xf,sizeof vertices);
  float*z=zbuf_current(1);for(unsigned y=0;y<32;++y)memcpy(depth+y*32,z+y*1024,128);
  uint64_t pixels=s_gpu.xf_pixels;uint32_t count=s_gpu.zpass_count;
  asset_draw(prims[scene%6],scene,0);
  assert(!memcmp(vertices,s_xf,sizeof vertices));assert(!memcmp(color,ram+0x1000,sizeof color));
  z=zbuf_current(1);for(unsigned y=0;y<32;++y)assert(!memcmp(depth+y*32,z+y*1024,128));
  assert(s_gpu.xf_pixels==pixels && s_gpu.zpass_count==count);++checked;
 }
 /* Include the actual attribute fetches, program selection and outcodes.
  * Cache reuse is disabled in both paths, so it cannot explain this result. */
 asset_draw(NV_PRIM_TRIANGLES,0,0);double best[2]={1e30,1e30};
 for(unsigned round=0;round<4;++round)for(unsigned pass=0;pass<2;++pass){
  unsigned native=(pass+round)%2;nv2a_vsh_set_cxt_write(!native);uint32_t runs;
  double begin=render_wall_ms();
  for(unsigned j=0;j<5000;++j)assert(transform_vertices(24,0,&runs) && runs==24);
  double elapsed=render_wall_ms()-begin;if(elapsed<best[native])best[native]=elapsed;
 }
 nv2a_vsh_set_cxt_write(0);
 printf("CPU full-vertex-stage microbenchmark (NOT race FPS): 120000 vertices/path, reference_ms=%.3f native_ms=%.3f speedup=%.2fx; fetch, selection and frustum outcodes included\n",best[0],best[1],best[0]/best[1]);
 printf("PASS: %u asset-program wrapper scenes; six topologies; exact transformed vertices/color/depth/visibility; repeated indices, clipping, blend, constant snapshots, native input masks\n",checked);
 nv2a_vsh_report();
}
'''

if __name__ == '__main__':
    workers.run_fixture(workers.root / 'third_party/xboxrecomp/src/kernel/nv2a_pb_exec.c', fixture, True)
