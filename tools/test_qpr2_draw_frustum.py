"""Compare new conservative rejection with the unmodified exact clipper."""
import test_qpr2_raster_batches as batches
import test_qpr2_raster_workers as workers

fixture = batches.fixture[:batches.fixture.index('int main(void)')]+r'''
static void program(void){
 setup();s_gpu.pitch=128;s_gpu.clip_w=s_gpu.clip_h=32;s_gpu.prim=NV_PRIM_TRIANGLES;s_gpu.xf_mode=2;
 s_gpu.clip_z_seen=3;s_gpu.clip_z_min=10;s_gpu.clip_z_max=80;
 s_gpu.attr[0].offset=0x60000;s_gpu.attr[0].type=2;s_gpu.attr[0].size=4;s_gpu.attr[0].stride=16;
 uint32_t pos[4]={0,0x0020001b,0x08000000,0x0000f800},color[4]={0,0x0020001b,0x0c000000,0x0000f819};
 const float white[4]={1,0.3f,0.6f,1};nv2a_vsh_set_constant(0,white);
 nv2a_vsh_set_instruction(0,pos);nv2a_vsh_set_instruction(1,color);nv2a_vsh_set_start_slot(0);nv2a_vsh_set_cxt_write(0);
}
static uint32_t rng=76;
static float random_value(void){rng=rng*1664525u+1013904223u;return (int)(rng%256)/4.0f-16;}
static void scene(unsigned prim,const float *positions,unsigned n,int enabled){
 program();s_gpu.prim=prim;s_gpu.idx_count=n;s_frustum_enabled=enabled;
 for(unsigned i=0;i<n;i++)s_gpu.idx[i]=i;
 memcpy(ram+0x60000,positions,n*16);raster_batch_program();
}
int main(void){
 setenv("RECOMP_RASTER_THREADS","1",1);
 uint8_t color[32*128];float depth[32*32];uint32_t count;
 const unsigned prims[]={NV_PRIM_TRIANGLES,NV_PRIM_TRIANGLE_STRIP,NV_PRIM_TRIANGLE_FAN,NV_PRIM_POLYGON,NV_PRIM_QUADS,NV_PRIM_QUAD_STRIP};
 unsigned comparisons=0;
 for(unsigned trial=0;trial<90;trial++){
  float positions[12][4];
  for(unsigned i=0;i<12;i++){
   positions[i][0]=random_value();positions[i][1]=random_value();positions[i][2]=random_value()+40;positions[i][3]=0.5f+(i%4)*0.25f;
   switch(trial%15){
    case 0:positions[i][0]=-20-i;break;case 1:positions[i][0]=40+i;break;
    case 2:positions[i][1]=-20-i;break;case 3:positions[i][1]=40+i;break;
    case 4:positions[i][2]=0;break;case 5:positions[i][2]=100;break;
    case 6:positions[i][3]=-1;break;case 7:positions[i][3]=i%2?-1:1;break;
    case 8:positions[i][3]=NV_CLIP_W;break;case 9:positions[i][3]=i%2?0:1;break;
    case 10:positions[i][0]=i%2?NAN:4;break;case 11:positions[i][2]=INFINITY;break;
    case 12:positions[i][0]=(i&1)?32:0;positions[i][1]=(i&2)?32:0;break;
   }
  }
  unsigned prim=prims[trial%6];scene(prim,&positions[0][0],12,0);
  memcpy(color,ram+0x1000,sizeof color);float*z=zbuf_current(1);
  for(unsigned y=0;y<32;y++)memcpy(depth+y*32,z+y*1024,128);count=s_gpu.zpass_count;
  scene(prim,&positions[0][0],12,1);
  assert(!memcmp(color,ram+0x1000,sizeof color) && count==s_gpu.zpass_count);z=zbuf_current(1);
  for(unsigned y=0;y<32;y++)assert(!memcmp(depth+y*32,z+y*1024,128));++comparisons;
 }
 /* Uploaded context-writing programs must still execute for rejected draws. */
 for(unsigned enabled=0;enabled<2;enabled++){
  program();s_frustum_enabled=enabled;s_gpu.idx_count=6;
  uint32_t increment[4]={0,(3u<<21)|0x1B,(3u<<26)|(0x1Bu<<2),(3u<<28)|(15u<<12)|1u};
  nv2a_vsh_set_instruction(1,increment);nv2a_vsh_set_cxt_write(1);
  const float one[4]={1,1,1,1};nv2a_vsh_set_constant(0,one);
  for(unsigned i=0;i<6;i++){float p[4]={-10,0,30,1};memcpy(ram+0x60000+i*16,p,16);s_gpu.idx[i]=i;}
  raster_batch_program();assert(nv2a_vsh_constant(0)[0]==64 && !s_gpu.zpass_count);
 }
 nv2a_vsh_set_cxt_write(0);
 printf("PASS: %u exact framebuffer/depth/visibility comparisons across six primitive topologies, all planes, crossing/unknown/boundary cases; context writes preserved\n",comparisons);
 /* Real production batch benchmark: completely offscreen shared-index mesh.
  * This measures wrapper CPU work, not gameplay or a driver. */
 double best[2]={1e30,1e30};
 for(unsigned round=0;round<4;round++)for(unsigned pass=0;pass<2;pass++){
  unsigned enabled=pass^(round&1);program();s_frustum_enabled=enabled;
  s_gpu.idx_count=3072;for(unsigned i=0;i<3072;i++)s_gpu.idx[i]=i%3;
  const float p[3][4]={{-30,2,20,1},{-20,20,20,1},{-40,10,20,1}};memcpy(ram+0x60000,p,sizeof p);
  double begin=render_wall_ms();for(unsigned i=0;i<150;i++)raster_batch_program();
  double ms=render_wall_ms()-begin;if(ms<best[enabled])best[enabled]=ms;
 }
 printf("CPU BENCH: 150 offscreen 1024-triangle draws: V72_ms=%.3f culled_ms=%.3f speedup=%.3f (not game FPS)\n",best[0],best[1],best[0]/best[1]);
}
'''
if __name__=='__main__':
    workers.run_fixture(workers.root/'third_party/xboxrecomp/src/kernel/nv2a_pb_exec.c',fixture,True)
