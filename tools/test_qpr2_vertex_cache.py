"""Compare draw-local vertex reuse with the actual uncached NV2A interpreter."""
import test_qpr2_raster_batches as batches
import test_qpr2_raster_workers as workers
fixture=batches.fixture[:batches.fixture.index('int main(void)')]+r'''
int main(void){
 setenv("RECOMP_RASTER_THREADS","1",1);setenv("RECOMP_RENDER_TIMING","1",1);
 actual_program_draw();assert(nv2a_vsh_cacheable());
 s_gpu.idx_count=4096;
 for(unsigned i=0;i<4096;i++)s_gpu.idx[i]=i%3;
 Nv2aVshOutput*reference=malloc(sizeof s_xf);assert(reference);uint32_t runs;
 assert(transform_vertices(4096,0,&runs) && runs==4096);memcpy(reference,s_xf,sizeof s_xf);
 assert(transform_vertices(4096,1,&runs) && runs==3 && !memcmp(reference,s_xf,sizeof s_xf));
 /* New draw, constants and source vertex bytes must be read again. */
 const float red[4]={1,0,0,1};nv2a_vsh_set_constant(0,red);
 float x=123;memcpy(ram+0x60000,&x,4);
 assert(transform_vertices(4096,1,&runs) && runs==3 && s_xf[0].pos[0]==123 && s_xf[0].d0[1]==0);
 assert(memcmp(reference,s_xf,sizeof s_xf));
 memcpy(reference,s_xf,sizeof s_xf);assert(transform_vertices(4096,0,&runs) && !memcmp(reference,s_xf,sizeof s_xf));
 /* Context writes can make repeated invocations observe different constants. */
 nv2a_vsh_set_cxt_write(1);assert(!nv2a_vsh_cacheable());
 assert(transform_vertices(4096,1,&runs) && runs==4096);nv2a_vsh_set_cxt_write(0);
 /* Colliding keys can reduce reuse, never substitute the wrong vertex. */
 unsigned collision=1;while(((collision*2654435761u)>>22)!=0)++collision;
 assert(0x60000u+collision*16u+16u<sizeof ram);
 const float point[4]={17,23,10,1};memcpy(ram+0x60000+collision*16,point,16);
 for(unsigned i=0;i<4096;i++)s_gpu.idx[i]=(i&1)?collision:0;
 assert(transform_vertices(4096,0,&runs));memcpy(reference,s_xf,sizeof s_xf);
 assert(transform_vertices(4096,1,&runs) && runs==4096 && !memcmp(reference,s_xf,sizeof s_xf));
 nv2a_vsh_set_start_slot(NV2A_VSH_SLOTS-1);const uint32_t empty[4]={0};
 nv2a_vsh_set_instruction(NV2A_VSH_SLOTS-1,empty);assert(!transform_vertices(4096,1,&runs));
 free(reference);puts("PASS: 4096 repeated indices use 3 actual vertex runs with exact output; fresh constants/data per draw; context-write bypass; collision fallback; missing-program behavior");
}
'''
if __name__=='__main__':
 workers.run_fixture(workers.root/'third_party/xboxrecomp/src/kernel/nv2a_pb_exec.c',fixture,True)
