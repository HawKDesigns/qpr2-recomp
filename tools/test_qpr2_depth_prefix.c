#include <assert.h>
#include <stdint.h>
#include <stdio.h>
typedef struct {
    unsigned depth_available,depth_mask,depth_test,depth_func;
} NvGpuState;
typedef struct { unsigned state_index; } NvGpuTri;
#include "../third_party/xboxrecomp/src/kernel/nv2a_gpu_depth_seed.h"
int main(void)
{
    NvGpuState states[]={
        {1,1,1,0x201u}, /* monotone and useful */
        {1,1,1,0x207u}, /* may increase depth */
        {1,0,1,0x207u}, /* read-only is safe */
        {1,1,1,0x203u}
    };
    NvGpuTri full[]={{0},{2},{3}},partial[]={{0},{2},{1},{0}},early[]={{1},{0}};
    int useful=-1;
    assert(nv_gpu_depth_seed_prefix(states,4,full,3,&useful)==3 && useful);
    assert(nv_gpu_depth_seed_prefix(states,4,partial,4,&useful)==2 && useful);
    assert(nv_gpu_depth_seed_prefix(states,4,early,2,&useful)==0 && !useful);
    NvGpuTri unused[]={{2},{1}};
    assert(nv_gpu_depth_seed_prefix(states,4,unused,2,&useful)==1 && !useful);
    NvGpuTri invalid[]={{0},{4},{3}};
    assert(nv_gpu_depth_seed_prefix(states,4,invalid,3,&useful)==1 && useful);
    assert(nv_gpu_depth_seed_batch_safe(states,4)==0);
    assert(nv_gpu_depth_seed_batch_useful(states,4)==1);
    puts("PASS: safe leading depth draws retain seed; unsafe and invalid indices stop it");
    return 0;
}
