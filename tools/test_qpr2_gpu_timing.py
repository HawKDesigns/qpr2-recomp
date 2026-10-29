"""Real backend query integration: sparse, asynchronous, nonblocking GPU timing.
Strict query mock verifies flags, disjoint/frequency handling and bounded pools;
rendered raw geometry/color/depth/fences still use the existing D3D fixture.
"""
from pathlib import Path
import subprocess,tempfile
import test_qpr2_gpu_geometry as geo
root=geo.root;k=geo.k
api=geo.api.replace('#define NV_GPU_NO_TIMESTAMPS 1','')
queries=r'''
enum {D3D11_QUERY_TIMESTAMP=2,D3D11_QUERY_TIMESTAMP_DISJOINT=3,D3D11_ASYNC_GETDATA_DONOTFLUSH=1};
typedef struct {unsigned Query,MiscFlags;} D3D11_QUERY_DESC;
typedef struct {uint64_t Frequency;int Disjoint;} D3D11_QUERY_DATA_TIMESTAMP_DISJOINT;
typedef struct {unsigned type,begun,ended;uint64_t value;} ID3D11Query;
typedef ID3D11Query ID3D11Asynchronous;
static unsigned query_creates,query_releases,query_gets,query_begins,query_fail_at,query_live_scope;
static int query_not_ready,query_error,query_disjoint,query_zero_frequency;
static uint64_t query_clock;
static HRESULT ID3D11Device_CreateQuery(ID3D11Device*d,const D3D11_QUERY_DESC*desc,ID3D11Query**out){
 (void)d;assert(!desc->MiscFlags && (desc->Query==2 || desc->Query==3));++query_creates;
 if(query_fail_at==query_creates)return -1;*out=calloc(1,sizeof **out);assert(*out);(*out)->type=desc->Query;return 0;
}
static void ID3D11Query_Release(ID3D11Query*q){assert(!q->begun);++query_releases;free(q);}
static void ID3D11DeviceContext_Begin(ID3D11DeviceContext*c,ID3D11Asynchronous*q){
 (void)c;assert(q->type==3 && !q->begun && !query_live_scope);q->begun=1;q->ended=0;++query_begins;++query_live_scope;
}
static void ID3D11DeviceContext_End(ID3D11DeviceContext*c,ID3D11Asynchronous*q){
 (void)c;if(q->type==3){assert(q->begun && query_live_scope);q->begun=0;q->ended=1;--query_live_scope;}
 else {assert(query_live_scope);query_clock+=1000;q->value=query_clock;q->ended=1;}
}
static HRESULT ID3D11DeviceContext_GetData(ID3D11DeviceContext*c,ID3D11Asynchronous*q,void*out,unsigned bytes,unsigned flags){
 (void)c;assert(flags==D3D11_ASYNC_GETDATA_DONOTFLUSH && q->ended && !q->begun);++query_gets;
 if(query_error)return -1;if(query_not_ready)return 1;
 if(q->type==3){assert(bytes==sizeof(D3D11_QUERY_DATA_TIMESTAMP_DISJOINT));D3D11_QUERY_DATA_TIMESTAMP_DISJOINT*p=out;p->Frequency=query_zero_frequency?0:1000000;p->Disjoint=query_disjoint;}
 else {assert(bytes==8);memcpy(out,&q->value,8);}return 0;
}
'''
main=r'''
static uint32_t color[1024];static float depth[1024];
static NvGpuCount scene(void){
 float v[3][8]={{2,2,20,1,1,1,1,1},{27,3,20,1,1,1,1,1},{4,27,20,1,1,1,1,1}},constants[192][4]={{0}};uint32_t ids[3]={0,1,2};
 for(unsigned i=0;i<4;i++){constants[58][i]=constants[101][i]=1;constants[96+i][i]=1;}constants[186][1]=constants[186][3]=1;
 NvRawInput input={0};input.indices=ids;input.count=input.vertices=3;input.primitive=5;input.inputs=0x3d;input.position_kind=1;input.constants=constants;
 input.stream[0]=(NvRawStream){(const uint8_t*)v,80,32,2,4};input.stream[2]=(NvRawStream){(const uint8_t*)v+16,80,32,2,4};input.params.right=input.params.bottom=32;input.params.su=input.params.sv=1;
 NvGpuState state={0};state.width=state.height=32;state.pitch=128;state.bpp=4;state.format=0x128;state.color_mask=0x01010101;state.depth_available=state.depth_test=state.depth_mask=state.zpass_enable=1;state.depth_func=0x203;
 for(unsigned i=0;i<1024;i++){color[i]=0xFF102030;depth[i]=100;}
 NvGpuTextureSource textures[4]={0};NvGpuCount count={0};
 assert(nv_gpu_draw_raw(&state,&input,(uint8_t*)color,depth,32,textures)==1 && nv_gpu_sync(&count)==1);
 assert(color[5*32+5]==0xFFFFFFFF && depth[5*32+5]==20);return count;
}

static void compute_scene(void){
 NvGpuState state={0};state.width=state.height=32;state.pitch=128;state.bpp=4;state.format=0x128;state.color_mask=0x01010101;
 state.depth_available=state.depth_test=state.depth_mask=1;state.depth_func=0x203;NvGpuTri tri=triangle(32);NvGpuTextureSource textures[4]={0};NvGpuCount count={0};
 for(unsigned i=0;i<1024;i++){color[i]=0xFF102030;depth[i]=100;}
 assert(nv_gpu_draw(&state,&tri,1,(uint8_t*)color,depth,32,textures)==1 && nv_gpu_sync(&count)==1);
 assert(color[5*32+5]==0xFFFFFFFF && depth[5*32+5]==10);
}
static void advance(void){for(unsigned i=0;i<NV_GPU_TIME_FRAMES;i++)nv_gpu_capture_present(NULL,0,0);}
int main(int argc,char**argv){
 const char*mode=argc>1?argv[1]:"native";
 setenv("RECOMP_GPU_RASTER","1",1);setenv("RECOMP_GPU_ASYNC","0",1);setenv("RECOMP_GPU_GEOMETRY","1",1);setenv("RECOMP_GPU_PROFILE",strcmp(mode,"disabled")?"1":"0",1);
 native_supported=strcmp(mode,"compute")!=0;coverage_tier=3;if(!strcmp(mode,"create"))query_fail_at=7;
 assert(nv_gpu_available() && raw.ready==native_supported);
 if(!native_supported){assert(gpu_time.enabled);compute_scene();gpu_timing_poll();assert(gpu_time.phase[NV_GPU_TIME_PIXELS].samples==1 && !gpu_time.phase[0].samples);advance();compute_scene();gpu_timing_poll();assert(gpu_time.phase[NV_GPU_TIME_COPY].samples==1);puts("PASS: compute pixel fallback and paired color/depth copies also have valid nonblocking timing scopes");gpu_timing_release();return 0;}
 if(strcmp(mode,"native")){
  assert(!gpu_time.enabled);scene();assert(g.native && raw.ready);
  if(query_fail_at)assert(query_releases==query_creates-1);else assert(!query_creates);
  puts("PASS: unsupported query allocation / explicit opt-out leaves validated rendering active");return 0;
 }
 assert(gpu_time.enabled && query_creates==NV_GPU_TIME_SLOTS*(1+NV_GPU_TIME_KINDS*2));
 gpu_time.enabled=0;NvGpuCount baseline=scene();uint32_t reference[1024];float zref[1024];memcpy(reference,color,sizeof color);memcpy(zref,depth,sizeof depth);assert(!query_begins);
 gpu_time.enabled=1;uint64_t reads=g.readbacks;NvGpuCount profiled=scene();
 assert(g.readbacks==reads+1 && !memcmp(&baseline,&profiled,sizeof baseline) && !memcmp(reference,color,sizeof color) && !memcmp(zref,depth,sizeof depth));
 assert(query_begins==1 && !query_live_scope && gpu_time.slot[0].closed && gpu_time.slot[0].mask==7);
 query_not_ready=1;unsigned before=query_gets;gpu_timing_poll();assert(query_gets==before+1 && gpu_time.slot[0].closed && !gpu_time.phase[0].samples);
 query_not_ready=0;gpu_timing_poll();
 for(unsigned i=0;i<3;i++)assert(gpu_time.phase[i].samples==1 && gpu_time.phase[i].ms==1);
 assert(gpu_time.phase[0].work==3 && gpu_time.phase[1].work==1 && gpu_time.phase[2].work==1);
 advance();scene();gpu_timing_poll();assert(query_begins==2 && gpu_time.phase[NV_GPU_TIME_COPY].samples==1 && gpu_time.phase[NV_GPU_TIME_COPY].work==1024);
 /* Same present interval cannot start another disjoint query even if many
  * draws/copies occur. No query is a guest completion condition. */
 unsigned began=query_begins;scene();scene();assert(query_begins==began);
 puts("PASS: actual raw vertex/preparation/native pixels/copy query placement; exact pixels/depth/visibility and unchanged readback count; one disjoint scope per eight presents");
 query_not_ready=1;
 for(unsigned i=0;i<NV_GPU_TIME_SLOTS;i++){
  advance();unsigned h=gpu_timing_batch_begin(gpu_time.want_copy);assert(h);gpu_timing_phase_begin(NV_GPU_TIME_PIXELS,7);gpu_timing_phase_end(NV_GPU_TIME_PIXELS);gpu_timing_batch_end(h);
 }
 advance();assert(!gpu_timing_batch_begin(gpu_time.want_copy) && gpu_time.dropped==1 && !query_live_scope);
 query_not_ready=0;query_disjoint=1;gpu_timing_poll();assert(gpu_time.invalid==NV_GPU_TIME_SLOTS);query_disjoint=0;
 advance();unsigned h=gpu_timing_batch_begin(gpu_time.want_copy);assert(h);gpu_timing_phase_begin(NV_GPU_TIME_PIXELS,1);gpu_timing_phase_end(NV_GPU_TIME_PIXELS);gpu_timing_batch_end(h);
 query_zero_frequency=1;gpu_timing_poll();assert(gpu_time.invalid==NV_GPU_TIME_SLOTS+1);query_zero_frequency=0;
 advance();h=gpu_timing_batch_begin(gpu_time.want_copy);assert(h);gpu_timing_phase_begin(NV_GPU_TIME_PIXELS,1);gpu_timing_phase_end(NV_GPU_TIME_PIXELS);gpu_timing_batch_end(h);
 gpu_time.slot[h-1].query[2+NV_GPU_TIME_PIXELS*2]->value=0;gpu_timing_poll();assert(gpu_time.invalid==NV_GPU_TIME_SLOTS+2);
 gpu_timing_report();assert(!gpu_time.phase[0].samples && !gpu_time.invalid && !gpu_time.dropped);
 puts("PASS: bounded pending-query saturation drops samples without waits; disjoint, zero-frequency and reversed timestamps are discarded; reports reset completed sample statistics");
 advance();h=gpu_timing_batch_begin(gpu_time.want_copy);assert(h);gpu_timing_phase_begin(NV_GPU_TIME_PIXELS,1);gpu_timing_phase_end(NV_GPU_TIME_PIXELS);gpu_timing_batch_end(h);query_error=1;gpu_timing_poll();
 assert(!gpu_time.enabled && query_releases==query_creates);scene();assert(g.native && raw.ready);
 puts("PASS: runtime query failure disables diagnostics only; all query objects released; ordinary rendering and completion continue");return 0;
}
'''
if __name__=='__main__':
 with tempfile.TemporaryDirectory(prefix='qpr2-gpu-timing-') as d:
  p=Path(d);(p/'logs').mkdir()
  for name in ['windows.h','d3d11.h','d3d11_3.h','d3dcompiler.h','dxgi.h']:(p/name).write_text('/* API shim */\n')
  (p/'fixture.c').write_text(api+queries+'ptrdiff_t xbox_GetMemoryOffset(void){return 0;}\n#include "'+str(k/'nv2a_gpu_raster.c')+'"\n'+geo.fixture+main)
  subprocess.run(['gcc','-std=gnu11','-O1','-ffp-contract=off','-Werror=implicit-function-declaration','-Werror=incompatible-pointer-types','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-I'+str(p),'-I'+str(root/'tools'),'-I'+str(k),str(p/'fixture.c'),str(k/'nv2a_vsh_interp.c'),str(k/'nv2a_gpu_tiles.c'),'-pthread','-lm','-o',str(p/'fixture')],check=True)
  for mode in ['native','compute','create','disabled']:subprocess.run([str(p/'fixture'),mode],cwd=p,check=True,timeout=120)
