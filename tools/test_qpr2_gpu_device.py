"""Compile/run the actual Win32 D3D11 backend with a strict native API mock.

Checks host resource/cache/readback code and its real startup self-test. The
mock executes the shared pixel kernel, not a hardware/HLSL compiler. Actual
D3D11 compilation and offscreen GPU comparison happen on Windows startup.
"""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1]
api=r'''
#define _WIN32 1
#define NV_GPU_NO_TIMESTAMPS 1
#define COBJMACROS
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include <sys/stat.h>
#include <errno.h>
/* Windows SDK RPC headers expose this legacy type macro. Keep it active
 * while compiling the production backend to catch identifier collisions. */
#define small char
static int CreateDirectoryA(const char*p,void*q){(void)q;return mkdir(p,0700)==0 || errno==EEXIST;}

typedef int32_t LONG;typedef uint32_t DWORD;typedef void*LPVOID;typedef void*HANDLE;
typedef pthread_mutex_t SRWLOCK;typedef pthread_cond_t CONDITION_VARIABLE;
#define SRWLOCK_INIT PTHREAD_MUTEX_INITIALIZER
#define CONDITION_VARIABLE_INIT PTHREAD_COND_INITIALIZER
#define ERROR_TIMEOUT 1460
static __thread DWORD fixture_error;
static DWORD GetLastError(void){return fixture_error;}
static void AcquireSRWLockExclusive(SRWLOCK*p){assert(!pthread_mutex_lock(p));}
static void ReleaseSRWLockExclusive(SRWLOCK*p){assert(!pthread_mutex_unlock(p));}
static void WakeAllConditionVariable(CONDITION_VARIABLE*p){assert(!pthread_cond_broadcast(p));}
static int SleepConditionVariableSRW(CONDITION_VARIABLE*c,SRWLOCK*m,DWORD ms,unsigned flags){
 assert(ms==2 && flags==0);struct timespec end;clock_gettime(CLOCK_REALTIME,&end);
 /* Deliberately coarse timeout proves notification wakes before a poll tick. */
 end.tv_nsec+=250000000;if(end.tv_nsec>=1000000000){++end.tv_sec;end.tv_nsec-=1000000000;}
 int status=pthread_cond_timedwait(c,m,&end);if(status==ETIMEDOUT){fixture_error=ERROR_TIMEOUT;return 0;}
 assert(!status);return 1;
}

#define WINAPI
static LONG InterlockedCompareExchange(volatile LONG*p,LONG n,LONG old){__atomic_compare_exchange_n(p,&old,n,0,__ATOMIC_SEQ_CST,__ATOMIC_SEQ_CST);return old;}
static LONG InterlockedExchange(volatile LONG*p,LONG n){return __atomic_exchange_n(p,n,__ATOMIC_SEQ_CST);}
static __thread DWORD thread_id=1;
static DWORD GetCurrentThreadId(void){return thread_id;}
static int startup_gate,create_thread_fail;
static pthread_mutex_t gate_lock=PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t gate_cond=PTHREAD_COND_INITIALIZER;
typedef struct {DWORD(*fn)(LPVOID);LPVOID data;} StartupThread;
static void*startup_thunk(void*raw){StartupThread*t=raw;thread_id=2;
 pthread_mutex_lock(&gate_lock);while(startup_gate)pthread_cond_wait(&gate_cond,&gate_lock);pthread_mutex_unlock(&gate_lock);
 t->fn(t->data);free(t);return NULL;}
static HANDLE CreateThread(void*a,size_t b,DWORD(*fn)(LPVOID),LPVOID data,DWORD flags,DWORD*id){
 (void)a;(void)b;(void)flags;(void)id;if(create_thread_fail)return NULL;
 StartupThread*t=malloc(sizeof*t);assert(t);t->fn=fn;t->data=data;pthread_t thread;
 assert(!pthread_create(&thread,NULL,startup_thunk,t));pthread_detach(thread);return (HANDLE)1;
}
static int CloseHandle(HANDLE h){(void)h;return 1;}
typedef int HRESULT;typedef unsigned UINT;typedef size_t SIZE_T;typedef void *HMODULE;
typedef struct {int64_t QuadPart;} LARGE_INTEGER;
static void QueryPerformanceCounter(LARGE_INTEGER*p){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);p->QuadPart=(int64_t)t.tv_sec*1000000000+t.tv_nsec;}
static void QueryPerformanceFrequency(LARGE_INTEGER*p){p->QuadPart=1000000000;}
#define FAILED(x) ((x)<0)
#define SUCCEEDED(x) ((x)>=0)
#define CP_UTF8 65001
static int WideCharToMultiByte(unsigned a,unsigned b,const uint16_t*c,int d,char*e,int f,void*h,void*i){(void)a;(void)b;(void)c;(void)d;(void)h;(void)i;strncpy(e,"D3D11 API fixture",f);return 1;}
enum {D3D11_USAGE_DEFAULT=0,D3D11_USAGE_IMMUTABLE=1,D3D11_USAGE_DYNAMIC=2,D3D11_USAGE_STAGING=3,
 D3D11_BIND_INDEX_BUFFER=2,DXGI_FORMAT_R32_UINT=42,D3D11_BIND_SHADER_RESOURCE=8,D3D11_BIND_UNORDERED_ACCESS=128,D3D11_RESOURCE_MISC_BUFFER_STRUCTURED=64,
 D3D11_BIND_RENDER_TARGET=32,DXGI_FORMAT_R8G8B8A8_UNORM=28,
 D3D11_CPU_ACCESS_READ=0x20000,D3D11_MAP_READ=1,DXGI_FORMAT_UNKNOWN=0,
 D3D11_UAV_DIMENSION_BUFFER=1,D3D11_SRV_DIMENSION_BUFFER=1,D3D_FEATURE_LEVEL_11_0=0xb000,
 D3D_FEATURE_LEVEL_11_1=0xb100,D3D_FEATURE_LEVEL_12_0=0xc000,D3D_FEATURE_LEVEL_12_1=0xc100,
 D3D_DRIVER_TYPE_HARDWARE=1,D3D11_SDK_VERSION=7,D3DCOMPILE_IEEE_STRICTNESS=8192,D3DCOMPILE_OPTIMIZATION_LEVEL3=32768};
typedef unsigned D3D_FEATURE_LEVEL;
typedef struct {unsigned ByteWidth,Usage,BindFlags,CPUAccessFlags,MiscFlags,StructureByteStride;} D3D11_BUFFER_DESC;
typedef struct {const void*pSysMem;unsigned SysMemPitch,SysMemSlicePitch;} D3D11_SUBRESOURCE_DATA;
typedef struct {unsigned FirstElement,NumElements,Flags;} BufferView;
typedef struct {unsigned Format,ViewDimension;BufferView Buffer;} D3D11_UNORDERED_ACCESS_VIEW_DESC;
typedef struct {unsigned Format,ViewDimension;BufferView Buffer;} D3D11_SHADER_RESOURCE_VIEW_DESC;
typedef struct {unsigned left,top,front,right,bottom,back;} D3D11_BOX;
typedef struct {void*pData;unsigned RowPitch,DepthPitch;} D3D11_MAPPED_SUBRESOURCE;
typedef struct {uint16_t Description[128];size_t DedicatedVideoMemory;} DXGI_ADAPTER_DESC;
typedef struct {int unused;} ID3D11Device;
typedef struct {int unused;} ID3D11DeviceContext;
typedef struct {int unused;} ID3D11ComputeShader;
typedef struct {int unused;} ID3D11VertexShader;
typedef struct {int unused;} ID3D11Device3;
typedef struct {int unused;} ID3D11PixelShader;
typedef struct {int unused;} ID3D11RasterizerState;
typedef ID3D11RasterizerState ID3D11RasterizerState2;
typedef struct {const char*Name,*Definition;} D3D_SHADER_MACRO;
typedef struct {int PSSpecifiedStencilRefSupported,TypedUAVLoadAdditionalFormats,ROVsSupported,ConservativeRasterizationTier,TiledResourcesTier,MapOnDefaultTextures,StandardSwizzle,UnifiedMemoryArchitecture;} D3D11_FEATURE_DATA_D3D11_OPTIONS2;
typedef struct {unsigned FillMode,CullMode;int FrontCounterClockwise;int DepthBias;float DepthBiasClamp,SlopeScaledDepthBias;int DepthClipEnable,ScissorEnable,MultisampleEnable,AntialiasedLineEnable;} D3D11_RASTERIZER_DESC;
typedef struct {unsigned FillMode,CullMode;int FrontCounterClockwise;int DepthBias;float DepthBiasClamp,SlopeScaledDepthBias;int DepthClipEnable,ScissorEnable,MultisampleEnable,AntialiasedLineEnable;unsigned ForcedSampleCount,ConservativeRaster;} D3D11_RASTERIZER_DESC2;
enum {D3D11_CONSERVATIVE_RASTERIZATION_TIER_2=2,D3D11_CONSERVATIVE_RASTERIZATION_MODE_ON=1};
typedef struct {float TopLeftX,TopLeftY,Width,Height,MinDepth,MaxDepth;} D3D11_VIEWPORT;
enum {D3D11_FEATURE_D3D11_OPTIONS2=14,D3D11_FILL_SOLID=3,D3D11_CULL_NONE=1,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST=4};
static ID3D11VertexShader vertex_shader,coverage_vertex_shader;
static ID3D11VertexShader *bound_vertex;
static ID3D11Device3 modern_device;
static int coverage_tier,coverage_query_fail,coverage_state_fail,coverage_compile_fail,coverage_create_fail,coverage_corrupt,coverage_thin_corrupt;
static unsigned coverage_draws,coverage_thin_draws;
static int native_batch_corrupt,index_create_fail;
static uint64_t mock_reject_before_source,mock_reject_after_source,mock_ordered_transactions;
static ID3D11PixelShader pixel_shader,simple_pixel_shader,literal_pixel_shader[3];
static int literal_compile_fail,literal_create_fail,literal_color_bad,literal_depth_bad,literal_count_bad,literal_variant_bad,literal_map_fail;
static unsigned literal_draws[3],compact_draws[3];
static unsigned literal_is_compact[3];
static int compact_compile_fail,compact_create_fail,compact_color_bad,compact_depth_bad,compact_count_bad,compact_variant_bad,compact_map_fail;
static unsigned mock_literal_id(ID3D11PixelShader *s){for(unsigned i=0;i<3;i++)if(s==literal_pixel_shader+i)return i+1;return 0;}
static ID3D11PixelShader *bound_pixel;
static int native_simple_compile_fail,native_simple_create_fail,native_simple_corrupt,native_simple_large_corrupt;
static unsigned native_simple_draws;
static int native_simple_readback_fail,fail_next_map;
static ID3D11RasterizerState rasterizer,coverage_rasterizer;
static ID3D11RasterizerState *bound_rasterizer;
static int quiet_zpass_corrupt;
static int native_supported,native_corrupt,native_compile_fail,native_create_fail,native_stale,native_arithmetic;
static int early_compile_fail,early_simple_compile_fail;
static unsigned mock_early_depth_reject;
static unsigned native_draws,vs_bound;
static int simple_compile_fail,simple_corrupt,fixed_compile_fail,fixed_corrupt;
static D3D11_VIEWPORT native_view;
typedef struct {LONG left,top,right,bottom;} D3D11_RECT;
static D3D11_RECT bound_scissor;
typedef struct {unsigned char*data;D3D11_BUFFER_DESC desc;int mapped;} ID3D11Buffer;
typedef ID3D11Buffer ID3D11Resource;
typedef struct {unsigned Count,Quality;} SampleDesc;
typedef struct {unsigned Width,Height,MipLevels,ArraySize,Format;SampleDesc SampleDesc;unsigned Usage,BindFlags,CPUAccessFlags,MiscFlags;} D3D11_TEXTURE2D_DESC;
typedef struct {D3D11_TEXTURE2D_DESC desc;} ID3D11Texture2D;
typedef struct {ID3D11Texture2D*texture;} ID3D11RenderTargetView;
static ID3D11RenderTargetView *bound_raster;
static int raster_create_fail,uav_only_corrupt,uav_only_map_fail;
static unsigned uav_only_draws,raster_creates,buffer_creates,buffer_releases,target_create_fail_bytes;
typedef struct {ID3D11Buffer*buffer;} ID3D11ShaderResourceView;
typedef struct {ID3D11Buffer*buffer;} ID3D11UnorderedAccessView;
typedef struct {const char*data;size_t bytes;int owned;} ID3DBlob;
typedef struct {int unused;} IDXGIDevice;
typedef struct {int unused;} IDXGIAdapter;
static int IID_IDXGIDevice,IID_ID3D11Device3;
static ID3D11Device device;
static ID3D11DeviceContext context;
static ID3D11ComputeShader shader;
static IDXGIDevice dxgi;
static IDXGIAdapter adapter;
static int failed_device,failed_compile,corrupt_shader,report_compile_fail,report_corrupt;
static ID3D11ComputeShader report_shader,clear_shader,bridge_shader;
static int bridge_compile_fail,bridge_create_fail,bridge_corrupt,bridge_map_fail;
static unsigned bridge_dispatches;
static int clear_compile_fail,clear_create_fail,clear_corrupt,clear_map_fail;
static unsigned clear_dispatches;
static ID3D11ComputeShader*bound_compute;
static unsigned device_calls,legacy_runtime,selected_feature_level,compiler_calls;
#define E_INVALIDARG ((HRESULT)0x80070057u)
static ID3D11ShaderResourceView *bound_srv[5];
static ID3D11ShaderResourceView *bound_seed_srv[2];
static ID3D11ShaderResourceView *bound_material_srv;
static int mock_apply_stage_masks;
static unsigned mock_masked_sources;
static uint32_t mock_texture_stage_mask(unsigned state){
 if(!mock_apply_stage_masks)return 15;
 assert(bound_material_srv && (state+1)*4<=bound_material_srv->buffer->desc.ByteWidth);
 ++mock_masked_sources;return ((uint32_t*)bound_material_srv->buffer->data)[state];
}
#define NV_GPU_STAGE_LIVENESS 1
#define GPU_TEXTURE_STAGE_MASK(tri) mock_texture_stage_mask((tri).state_index)

static ID3D11UnorderedAccessView *bound_uav[4];
static HRESULT D3D11CreateDevice(IDXGIAdapter*a,unsigned b,HMODULE c,unsigned d,const D3D_FEATURE_LEVEL*e,unsigned f,unsigned h,ID3D11Device**i,D3D_FEATURE_LEVEL*j,ID3D11DeviceContext**k){
 (void)a;(void)c;(void)d;assert(b==D3D_DRIVER_TYPE_HARDWARE && h==D3D11_SDK_VERSION);++device_calls;
 assert((f==4 && e[0]==D3D_FEATURE_LEVEL_12_1 && e[1]==D3D_FEATURE_LEVEL_12_0 && e[2]==D3D_FEATURE_LEVEL_11_1 && e[3]==D3D_FEATURE_LEVEL_11_0) ||
        (f==2 && e[0]==D3D_FEATURE_LEVEL_11_1 && e[1]==D3D_FEATURE_LEVEL_11_0) ||
        (f==1 && e[0]==D3D_FEATURE_LEVEL_11_0));
 if(failed_device)return -1;
 if((legacy_runtime && f==4) || (legacy_runtime==2 && f==2))return E_INVALIDARG;
 *i=&device;*j=selected_feature_level=native_supported?e[0]:D3D_FEATURE_LEVEL_11_0;*k=&context;return 0;
}
static HRESULT ID3D11Device_QueryInterface(ID3D11Device*d,const void*i,void**out){(void)d;if(i==&IID_ID3D11Device3){if(coverage_query_fail)return -1;*out=&modern_device;return 0;}assert(i==&IID_IDXGIDevice);*out=&dxgi;return 0;}
static HRESULT IDXGIDevice_GetAdapter(IDXGIDevice*d,IDXGIAdapter**out){(void)d;*out=&adapter;return 0;}
static HRESULT IDXGIAdapter_GetDesc(IDXGIAdapter*a,DXGI_ADAPTER_DESC*d){(void)a;memset(d,0,sizeof *d);d->DedicatedVideoMemory=1024u*1024u*1024u;return 0;}
static void IDXGIAdapter_Release(IDXGIAdapter*a){(void)a;}
static void IDXGIDevice_Release(IDXGIDevice*a){(void)a;}
static HRESULT D3DCompile(const void*s,size_t n,const char*a,void*b,void*c,const char*d,const char*e,unsigned f,unsigned h,ID3DBlob**out,ID3DBlob**error){
 (void)b;(void)c;(void)h;assert(n>20000 || (!strcmp(a,"qpr2_gpu_texcopy") && n>200) || (!strcmp(a,"qpr2_report_reset") && n>200) || (!strcmp(a,"qpr2_gpu_clear") && n>200) || (!strcmp(a,"qpr2_gpu_coverage") && n>2000));
 assert((!strcmp(d,"main") && !strcmp(e,"cs_5_0")) ||
        (!strcmp(d,"vertex_main") && !strcmp(e,"vs_5_0")) ||
        (!strcmp(d,"pixel_main") && !strcmp(e,"ps_5_0")));
 assert(f==(D3DCOMPILE_IEEE_STRICTNESS|D3DCOMPILE_OPTIMIZATION_LEVEL3));
 ++compiler_calls;ID3DBlob*blob=calloc(1,sizeof *blob);assert(blob);
 int simple_macro=0;
 for(D3D_SHADER_MACRO*m=b;m && m->Name;m++)if(!strcmp(m->Name,"NV_GPU_SIMPLE_MATERIAL"))simple_macro=1;
 int early_macro=0;for(D3D_SHADER_MACRO*m=b;m && m->Name;m++)if(!strcmp(m->Name,"NV_GPU_EARLY_DEPTH"))early_macro=atoi(m->Definition);
 int literal_macro=0;for(D3D_SHADER_MACRO*m=b;m && m->Name;m++)if(!strcmp(m->Name,"NV_GPU_LITERAL_CLASS"))literal_macro=atoi(m->Definition);
 int compact_macro=0;for(D3D_SHADER_MACRO*m=b;m && m->Name;m++)if(!strcmp(m->Name,"NV_GPU_COMPACT_MATERIALS"))compact_macro=atoi(m->Definition);
 if(failed_compile || (literal_macro && compact_macro && (compact_compile_fail==literal_macro || compact_compile_fail==99)) || (literal_macro && (literal_compile_fail==literal_macro || literal_compile_fail==99)) || (bridge_compile_fail && !strcmp(a,"qpr2_gpu_texcopy")) || (clear_compile_fail && !strcmp(a,"qpr2_gpu_clear")) || (coverage_compile_fail && !strcmp(a,"qpr2_gpu_coverage")) || (report_compile_fail && !strcmp(a,"qpr2_report_reset")) || (fixed_compile_fail && strcmp(a,"qpr2_report_reset") && !b && !strcmp(e,"cs_5_0")) || (simple_compile_fail && simple_macro && !strcmp(e,"cs_5_0")) || (native_simple_compile_fail && simple_macro && !strcmp(e,"ps_5_0")) || (early_macro && !strcmp(e,"ps_5_0") && (early_compile_fail || (simple_macro && early_simple_compile_fail))) || (native_compile_fail && strcmp(e,"cs_5_0"))){blob->data="fixture compiler failure";blob->bytes=strlen(blob->data);*error=blob;return -1;}
 if(simple_macro && !strcmp(e,"ps_5_0")){char*data=malloc(n+16);assert(data);memcpy(data,"//native-simple\n",16);memcpy(data+16,s,n);blob->data=data;blob->bytes=n+16;blob->owned=1;}
 else {blob->data=s;blob->bytes=n;}
 if(literal_macro){assert(!strcmp(e,"ps_5_0") && !simple_macro && literal_macro>=1 && literal_macro<=3);char marker[17]="//literal-0!!!!\n";marker[10]=(char)('0'+literal_macro);marker[11]=(char)('0'+compact_macro);char*data=malloc(blob->bytes+16);assert(data);memcpy(data,marker,16);memcpy(data+16,blob->data,blob->bytes);if(blob->owned)free((void*)blob->data);blob->data=data;blob->bytes+=16;blob->owned=1;}
 *out=blob;*error=NULL;return 0;
}
static void*ID3D10Blob_GetBufferPointer(ID3DBlob*b){return (void*)b->data;}
static size_t ID3D10Blob_GetBufferSize(ID3DBlob*b){return b->bytes;}
static void ID3D10Blob_Release(ID3DBlob*b){if(b->owned)free((void*)b->data);free(b);}
static HRESULT ID3D11Device_CreateComputeShader(ID3D11Device*d,const void*p,size_t n,void*a,ID3D11ComputeShader**out){(void)d;(void)a;assert(n>0);int clear=n>20 && !memcmp(p,"/* Ordered native",17);int handoff=n>24 && !memcmp(p,"/* GPU-resident ARGB",19);if(handoff && bridge_create_fail)return -1;if(clear && clear_create_fail)return -1;*out=handoff?&bridge_shader:clear?&clear_shader:n<1000?&report_shader:&shader;return 0;}
static void ID3D11ComputeShader_Release(ID3D11ComputeShader*s){assert(s==&shader || s==&report_shader || s==&clear_shader || s==&bridge_shader);}

static HRESULT ID3D11Device_CheckFeatureSupport(ID3D11Device*d,unsigned f,void*data,unsigned n){(void)d;assert(f==D3D11_FEATURE_D3D11_OPTIONS2 && n==sizeof(D3D11_FEATURE_DATA_D3D11_OPTIONS2));((D3D11_FEATURE_DATA_D3D11_OPTIONS2*)data)->ROVsSupported=native_supported;((D3D11_FEATURE_DATA_D3D11_OPTIONS2*)data)->ConservativeRasterizationTier=coverage_tier;return 0;}
static HRESULT ID3D11Device_CreateVertexShader(ID3D11Device*d,const void*p,size_t n,void*a,ID3D11VertexShader**out){(void)d;(void)a;assert(n>0);const char marker[]="/* Hardware creates";int coverage=n>=sizeof marker-1 && !memcmp(p,marker,sizeof marker-1);if(native_create_fail || (coverage && coverage_create_fail))return -1;*out=coverage?&coverage_vertex_shader:&vertex_shader;return 0;}
static HRESULT ID3D11Device_CreatePixelShader(ID3D11Device*d,const void*p,size_t n,void*a,ID3D11PixelShader**out){(void)d;(void)a;assert(n>0);int simple=n>16 && !memcmp(p,"//native-simple\n",16);if(n>16 && !memcmp(p,"//literal-",10)){unsigned id=((const char*)p)[10]-'0';assert(id>=1 && id<=3);unsigned compact=((const char*)p)[11]=='1';if(literal_create_fail==(int)id || literal_create_fail==99 || (compact && (compact_create_fail==(int)id || compact_create_fail==99)))return -1;literal_is_compact[id-1]=compact;*out=literal_pixel_shader+id-1;return 0;}if(simple && native_simple_create_fail)return -1;*out=simple?&simple_pixel_shader:&pixel_shader;return 0;}
static void ID3D11PixelShader_Release(ID3D11PixelShader*s){assert(s==&pixel_shader || s==&simple_pixel_shader || mock_literal_id(s));}
static HRESULT ID3D11Device_CreateRasterizerState(ID3D11Device*d,const D3D11_RASTERIZER_DESC*r,ID3D11RasterizerState**out){(void)d;assert(r->FillMode==D3D11_FILL_SOLID && r->CullMode==D3D11_CULL_NONE && r->DepthClipEnable && !r->ScissorEnable && !r->MultisampleEnable);*out=&rasterizer;return 0;}
static HRESULT ID3D11Device3_CreateRasterizerState2(ID3D11Device3*d,const D3D11_RASTERIZER_DESC2*r,ID3D11RasterizerState2**out){assert(d==&modern_device && r->FillMode==D3D11_FILL_SOLID && r->CullMode==D3D11_CULL_NONE && r->DepthClipEnable && r->ScissorEnable && !r->ForcedSampleCount && r->ConservativeRaster==D3D11_CONSERVATIVE_RASTERIZATION_MODE_ON);if(coverage_state_fail)return -1;*out=&coverage_rasterizer;return 0;}
static void ID3D11Device3_Release(ID3D11Device3*d){assert(d==&modern_device);}
static void ID3D11RasterizerState2_Release(ID3D11RasterizerState2*s){assert(s==&coverage_rasterizer);}
static void ID3D11DeviceContext_IASetInputLayout(ID3D11DeviceContext*c,void*layout){(void)c;assert(!layout);}
static void ID3D11DeviceContext_IASetPrimitiveTopology(ID3D11DeviceContext*c,unsigned topology){(void)c;assert(topology==D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);}
static void ID3D11DeviceContext_RSSetViewports(ID3D11DeviceContext*c,unsigned n,const D3D11_VIEWPORT*v){(void)c;assert(n==1 && v->TopLeftX==0 && v->TopLeftY==0 && v->MinDepth==0 && v->MaxDepth==1);native_view=*v;}
static void ID3D11DeviceContext_RSSetScissorRects(ID3D11DeviceContext*c,unsigned n,const D3D11_RECT*r){(void)c;assert(n==1 && !r->left && !r->top && r->right==(int)native_view.Width && r->bottom==(int)native_view.Height);bound_scissor=*r;}
static void ID3D11DeviceContext_RSSetState(ID3D11DeviceContext*c,ID3D11RasterizerState*r){(void)c;assert(r==&rasterizer || r==&coverage_rasterizer);bound_rasterizer=r;}
static void ID3D11DeviceContext_VSSetShader(ID3D11DeviceContext*c,ID3D11VertexShader*s,void*a,unsigned n){(void)c;assert((s==&vertex_shader || s==&coverage_vertex_shader) && !a && !n);bound_vertex=s;}
static void ID3D11DeviceContext_PSSetShader(ID3D11DeviceContext*c,ID3D11PixelShader*s,void*a,unsigned n){(void)c;assert((s==&pixel_shader || s==&simple_pixel_shader || mock_literal_id(s)) && !a && !n);bound_pixel=s;}
static void ID3D11DeviceContext_VSSetShaderResources(ID3D11DeviceContext*c,unsigned first,unsigned n,ID3D11ShaderResourceView*const*v){(void)c;assert(!first && n==5);vs_bound=v[0]!=NULL;}
static void ID3D11DeviceContext_PSSetShaderResources(ID3D11DeviceContext*c,unsigned first,unsigned n,ID3D11ShaderResourceView*const*v){(void)c;if(first==7){assert(n==1);bound_material_srv=v[0];}else if(first==5){assert(n==2);memcpy(bound_seed_srv,v,sizeof bound_seed_srv);}else{assert(!first && n==5);memcpy(bound_srv,v,sizeof bound_srv);}}
static void ID3D11DeviceContext_OMSetRenderTargetsAndUnorderedAccessViews(ID3D11DeviceContext*c,unsigned n,ID3D11RenderTargetView*const*rtv,void*dsv,unsigned first,unsigned count,ID3D11UnorderedAccessView*const*uav,const unsigned*initial){
 (void)c;assert(!dsv && !initial);if(n){assert(n==1 && rtv && *rtv && first==1 && count==3);bound_raster=*rtv;bound_uav[0]=NULL;bound_uav[1]=uav[0];bound_uav[2]=uav[1];bound_uav[3]=uav[2];}
 else {assert(!rtv && !first && count==4);bound_raster=NULL;memcpy(bound_uav,uav,sizeof bound_uav);}
}
static ID3D11Buffer *bound_indices;
static void ID3D11DeviceContext_IASetIndexBuffer(ID3D11DeviceContext*c,ID3D11Buffer*b,unsigned format,unsigned offset){(void)c;assert(b && b->desc.BindFlags==D3D11_BIND_INDEX_BUFFER && format==DXGI_FORMAT_R32_UINT && !offset);bound_indices=b;}
static HRESULT ID3D11Device_CreateTexture2D(ID3D11Device*d,const D3D11_TEXTURE2D_DESC*desc,const void*data,ID3D11Texture2D**out){
 (void)d;assert(!data && desc->Width && desc->Height && desc->MipLevels==1 && desc->ArraySize==1 &&
 desc->SampleDesc.Count==1 && !desc->SampleDesc.Quality && desc->Format==DXGI_FORMAT_R8G8B8A8_UNORM &&
 desc->Usage==D3D11_USAGE_DEFAULT && desc->BindFlags==D3D11_BIND_RENDER_TARGET && !desc->CPUAccessFlags && !desc->MiscFlags);
 ++raster_creates;if(raster_create_fail)return -1;*out=calloc(1,sizeof **out);assert(*out);(*out)->desc=*desc;return 0;
}
static HRESULT ID3D11Device_CreateRenderTargetView(ID3D11Device*d,ID3D11Resource*resource,const void*desc,ID3D11RenderTargetView**out){
 (void)d;assert(resource && !desc);*out=calloc(1,sizeof **out);assert(*out);(*out)->texture=(ID3D11Texture2D*)resource;return 0;
}
static void ID3D11RenderTargetView_Release(ID3D11RenderTargetView*v){assert(v!=bound_raster);free(v);}
static void ID3D11Texture2D_Release(ID3D11Texture2D*t){assert(!bound_raster || bound_raster->texture!=t);free(t);}
static void ID3D11DeviceContext_Draw(ID3D11DeviceContext*,unsigned,unsigned);
static void ID3D11DeviceContext_DrawIndexed(ID3D11DeviceContext*,unsigned,unsigned,int);
static HRESULT ID3D11Device_CreateBuffer(ID3D11Device*d,const D3D11_BUFFER_DESC*s,const D3D11_SUBRESOURCE_DATA*i,ID3D11Buffer**out){
 (void)d;++buffer_creates;if(target_create_fail_bytes && s->ByteWidth==target_create_fail_bytes && s->Usage==D3D11_USAGE_DEFAULT)return -1;assert(s->ByteWidth && s->ByteWidth%4==0);
 if(s->BindFlags==D3D11_BIND_INDEX_BUFFER){assert(s->Usage==D3D11_USAGE_IMMUTABLE && !s->MiscFlags && !s->StructureByteStride && !s->CPUAccessFlags && i && i->pSysMem && s->ByteWidth==8192u*6u*4u);if(index_create_fail)return -1;for(unsigned n=0;n<s->ByteWidth/4;n++)assert(((const uint32_t*)i->pSysMem)[n]==n);}
 else if(s->Usage==D3D11_USAGE_STAGING)assert(!s->BindFlags && !s->MiscFlags && !s->StructureByteStride && s->CPUAccessFlags==D3D11_CPU_ACCESS_READ);
 else assert(s->Usage==D3D11_USAGE_DEFAULT && !s->CPUAccessFlags && s->MiscFlags==D3D11_RESOURCE_MISC_BUFFER_STRUCTURED && s->StructureByteStride && s->ByteWidth%s->StructureByteStride==0);
 ID3D11Buffer*b=calloc(1,sizeof *b);assert(b);b->desc=*s;b->data=calloc(1,s->ByteWidth);assert(b->data);
 if(i)memcpy(b->data,i->pSysMem,s->ByteWidth);*out=b;return 0;
}
static HRESULT ID3D11Device_CreateShaderResourceView(ID3D11Device*d,ID3D11Resource*b,const D3D11_SHADER_RESOURCE_VIEW_DESC*v,ID3D11ShaderResourceView**out){
 (void)d;assert((b->desc.BindFlags&D3D11_BIND_SHADER_RESOURCE) && v->Format==DXGI_FORMAT_UNKNOWN && v->Buffer.NumElements==b->desc.ByteWidth/b->desc.StructureByteStride);
 ID3D11ShaderResourceView*q=calloc(1,sizeof *q);assert(q);q->buffer=b;*out=q;return 0;
}
static HRESULT ID3D11Device_CreateUnorderedAccessView(ID3D11Device*d,ID3D11Resource*b,const D3D11_UNORDERED_ACCESS_VIEW_DESC*v,ID3D11UnorderedAccessView**out){
 (void)d;assert((b->desc.BindFlags&D3D11_BIND_UNORDERED_ACCESS) && v->Format==DXGI_FORMAT_UNKNOWN && v->Buffer.NumElements==b->desc.ByteWidth/b->desc.StructureByteStride);
 ID3D11UnorderedAccessView*q=calloc(1,sizeof *q);assert(q);q->buffer=b;*out=q;return 0;
}
static void ID3D11ShaderResourceView_Release(ID3D11ShaderResourceView*v){assert(bound_material_srv!=v);for(int i=0;i<2;i++)assert(bound_seed_srv[i]!=v);for(int i=0;i<5;i++)assert(bound_srv[i]!=v);free(v);}
static void ID3D11UnorderedAccessView_Release(ID3D11UnorderedAccessView*v){for(int i=0;i<3;i++)assert(bound_uav[i]!=v);free(v);}
static void ID3D11Buffer_Release(ID3D11Buffer*b){++buffer_releases;assert(!b->mapped);free(b->data);free(b);}
static void ID3D11DeviceContext_UpdateSubresource(ID3D11DeviceContext*c,ID3D11Resource*b,unsigned sub,const D3D11_BOX*box,const void*data,unsigned pitch,unsigned dep){
 (void)c;(void)pitch;(void)dep;assert(!sub && b->desc.Usage==D3D11_USAGE_DEFAULT);
 assert(!bound_material_srv || bound_material_srv->buffer!=b);
 for(int i=0;i<5;i++)assert(!bound_srv[i] || bound_srv[i]->buffer!=b);
 for(int i=0;i<2;i++)assert(!bound_seed_srv[i] || bound_seed_srv[i]->buffer!=b);
 for(int i=0;i<4;i++)assert(!bound_uav[i] || bound_uav[i]->buffer!=b);
 unsigned left=box?box->left:0,right=box?box->right:b->desc.ByteWidth;
 assert(right<=b->desc.ByteWidth && right>=left);
 if(box)assert(box->top==0 && box->front==0 && box->bottom==1 && box->back==1 && left%b->desc.StructureByteStride==0 && right%b->desc.StructureByteStride==0);
 memcpy(b->data+left,data,right-left);
}
static void ID3D11DeviceContext_CopyResource(ID3D11DeviceContext*c,ID3D11Resource*dst,ID3D11Resource*src){
 (void)c;assert(dst->desc.ByteWidth==src->desc.ByteWidth);for(int i=0;i<2;i++)assert(!bound_seed_srv[i] || (bound_seed_srv[i]->buffer!=src && bound_seed_srv[i]->buffer!=dst));assert(dst->desc.Usage==D3D11_USAGE_STAGING || (dst->desc.Usage==D3D11_USAGE_DEFAULT && dst->desc.StructureByteStride==src->desc.StructureByteStride));
 for(int i=0;i<4;i++)assert(!bound_uav[i] || bound_uav[i]->buffer!=src);memcpy(dst->data,src->data,src->desc.ByteWidth);
}
static void ID3D11DeviceContext_CopySubresourceRegion(ID3D11DeviceContext*c,ID3D11Resource*dst,unsigned ds,unsigned x,unsigned y,unsigned z,ID3D11Resource*src,unsigned ss,const D3D11_BOX*box){
 (void)c;assert(!ds && !ss && !y && !z && box && !box->top && !box->front && box->bottom==1 && box->back==1);
 assert(!dst->mapped && !src->mapped && dst->desc.StructureByteStride==4 && src->desc.StructureByteStride==4);
 assert(box->right>=box->left && box->right<=src->desc.ByteWidth && x+box->right-box->left<=dst->desc.ByteWidth);
 assert(!(x%4) && !(box->left%4) && !(box->right%4));
 for(unsigned i=0;i<5;i++)assert(!bound_srv[i] || (bound_srv[i]->buffer!=src && bound_srv[i]->buffer!=dst));
 for(unsigned i=0;i<4;i++)assert(!bound_uav[i] || (bound_uav[i]->buffer!=src && bound_uav[i]->buffer!=dst));
 memcpy(dst->data+x,src->data+box->left,box->right-box->left);
}
static HRESULT ID3D11DeviceContext_Map(ID3D11DeviceContext*c,ID3D11Resource*b,unsigned sub,unsigned mode,unsigned flags,D3D11_MAPPED_SUBRESOURCE*out){
 (void)c;if(fail_next_map){fail_next_map=0;return -1;}assert(!sub && mode==D3D11_MAP_READ && !flags && b->desc.Usage==D3D11_USAGE_STAGING && !b->mapped);b->mapped=1;out->pData=b->data;return 0;
}
static void ID3D11DeviceContext_Unmap(ID3D11DeviceContext*c,ID3D11Resource*b,unsigned sub){(void)c;assert(!sub && b->mapped);b->mapped=0;}
static HRESULT ID3D11Device_GetDeviceRemovedReason(ID3D11Device*d){(void)d;return 0;}
static void ID3D11DeviceContext_CSSetShader(ID3D11DeviceContext*c,ID3D11ComputeShader*s,void*p,unsigned n){(void)c;assert((s==&shader || s==&report_shader || s==&clear_shader || s==&bridge_shader) && !p && !n);bound_compute=s;}
static void ID3D11DeviceContext_CSSetShaderResources(ID3D11DeviceContext*c,unsigned start,unsigned n,ID3D11ShaderResourceView*const*v){(void)c;assert(!start && n==5);memcpy(bound_srv,v,sizeof bound_srv);}
static void ID3D11DeviceContext_CSSetUnorderedAccessViews(ID3D11DeviceContext*c,unsigned start,unsigned n,ID3D11UnorderedAccessView*const*v,const unsigned*counts){(void)c;assert(!start && n==3 && !counts);memcpy(bound_uav,v,3*sizeof(*v));}
static void ID3D11DeviceContext_Dispatch(ID3D11DeviceContext*,unsigned,unsigned,unsigned);
'''
fixture=r'''
#define NV_GPU_LITERAL_CLASS 1
#define NV_GPU_COMPACT_MATERIALS 1
#define gpu_rc_compact mock_compact_eval_1
#include "nv2a_gpu_compact.inc"
#define gpu_source_color mock_compact_source_1
#include "nv2a_gpu_source.inc"
#undef gpu_source_color
#undef gpu_rc_compact
#undef NV_GPU_COMPACT_MATERIALS
#undef NV_GPU_LITERAL_CLASS
#define NV_GPU_LITERAL_CLASS 2
#define NV_GPU_COMPACT_MATERIALS 1
#define gpu_rc_compact mock_compact_eval_2
#include "nv2a_gpu_compact.inc"
#define gpu_source_color mock_compact_source_2
#include "nv2a_gpu_source.inc"
#undef gpu_source_color
#undef gpu_rc_compact
#undef NV_GPU_COMPACT_MATERIALS
#undef NV_GPU_LITERAL_CLASS
#define NV_GPU_LITERAL_CLASS 3
#define NV_GPU_COMPACT_MATERIALS 1
#define gpu_rc_compact mock_compact_eval_3
#include "nv2a_gpu_compact.inc"
#define gpu_source_color mock_compact_source_3
#include "nv2a_gpu_source.inc"
#undef gpu_source_color
#undef gpu_rc_compact
#undef NV_GPU_COMPACT_MATERIALS
#undef NV_GPU_LITERAL_CLASS
#define NV_GPU_LITERAL_CLASS 1
#define gpu_rc_literal mock_literal_eval_1
#include "nv2a_gpu_literal.inc"
#define gpu_source_color mock_literal_source_1
#include "nv2a_gpu_source.inc"
#undef gpu_source_color
#undef gpu_rc_literal
#undef NV_GPU_LITERAL_CLASS
#define NV_GPU_LITERAL_CLASS 2
#define gpu_rc_literal mock_literal_eval_2
#include "nv2a_gpu_literal.inc"
#define gpu_source_color mock_literal_source_2
#include "nv2a_gpu_source.inc"
#undef gpu_source_color
#undef gpu_rc_literal
#undef NV_GPU_LITERAL_CLASS
#define NV_GPU_LITERAL_CLASS 3
#define gpu_rc_literal mock_literal_eval_3
#include "nv2a_gpu_literal.inc"
#define gpu_source_color mock_literal_source_3
#include "nv2a_gpu_source.inc"
#undef gpu_source_color
#undef gpu_rc_literal
#undef NV_GPU_LITERAL_CLASS
static void ID3D11DeviceContext_Dispatch(ID3D11DeviceContext*ctx,unsigned n,unsigned y,unsigned z){
 (void)ctx;assert(y==1 && z==1);
 if(bound_compute==&bridge_shader){
  ++bridge_dispatches;assert(bound_srv[0] && bound_srv[1] && bound_uav[0] && !bound_uav[1] && !bound_uav[2]);
  for(unsigned i=2;i<5;i++)assert(!bound_srv[i]);
  assert(bound_srv[0]->buffer->desc.StructureByteStride==8 && bound_srv[1]->buffer->desc.ByteWidth==16 && bound_uav[0]->buffer->desc.StructureByteStride==4);
  uint32_t count=*(uint32_t*)bound_srv[1]->buffer->data;assert(n==(count+63)/64 && count*8<=bound_srv[0]->buffer->desc.ByteWidth && count*4<=bound_uav[0]->buffer->desc.ByteWidth);
  GpuSurface*src=(GpuSurface*)bound_srv[0]->buffer->data;uint32_t*dst=(uint32_t*)bound_uav[0]->buffer->data;
  for(unsigned i=0;i<count;i++)dst[i]=src[i].color;
  if(bridge_corrupt)dst[0]^=0xFF00;if(bridge_map_fail)fail_next_map=1;
  return;
 }
 if(bound_compute==&clear_shader){
  ++clear_dispatches;assert(bound_srv[0] && bound_uav[0] && !bound_uav[1] && !bound_uav[2]);
  for(unsigned i=1;i<5;i++)assert(!bound_srv[i]);
  uint32_t*p=(uint32_t*)bound_srv[0]->buffer->data;GpuSurface*surface=(GpuSurface*)bound_uav[0]->buffer->data;
  assert(bound_srv[0]->buffer->desc.ByteWidth==36 && bound_uav[0]->buffer->desc.StructureByteStride==8);
  assert(n==(unsigned)(((p[4]-p[2])*(p[5]-p[3])+63)/64));
  for(unsigned row=p[3];row<p[5];row++)for(unsigned x=p[2];x<p[4];x++){
   GpuSurface*v=surface+row*p[0]+x;if(p[6]&1)v->color=p[7];if(p[6]&2)v->depth_bits=p[8];
  }
  /* One injected write must remain observable; repeating XOR can cancel
   * itself across the two clears without exercising rejection at all. */
  if(clear_corrupt && clear_dispatches==1)surface[0].color^=0xFFFFFF;
  if(clear_map_fail)fail_next_map=1;
  return;
 }
 assert(n%4==0);
 if(bound_compute==&report_shader){
  for(int i=0;i<5;i++)assert(!bound_srv[i]);assert(!bound_uav[0] && !bound_uav[1] && bound_uav[2]);
  uint32_t*stats=(uint32_t*)bound_uav[2]->buffer->data;unsigned elements=bound_uav[2]->buffer->desc.ByteWidth/4;
  assert(n*64>=elements/4);
  for(unsigned i=0;i<elements;i++)if(report_corrupt || i%4==2)stats[i]=0;
  return;
 }
 NvGpuState*s=(NvGpuState*)bound_srv[3]->buffer->data;

 NvGpuTri*tri=(NvGpuTri*)bound_srv[0]->buffer->data;
 for(unsigned primitive=0;primitive<g.queued_count;primitive++) {
  NvGpuState*q=&s[tri[primitive].state_index];
  for(int st=0;st<4;st++)if(q->tex[st].valid) {
   NvGpuTexture meta=q->tex[st];GpuTexture*cached=&g.cache[meta.cache_slot];
   assert((size_t)meta.texel_base*4+cached->decoded_bytes<=bound_srv[4]->buffer->desc.ByteWidth);
   if(!cached->gpu_serial)assert(!memcmp(bound_srv[4]->buffer->data+(size_t)meta.texel_base*4,cached->decoded,cached->decoded_bytes));
  }
 }

 NvGpuTile*tiles=(NvGpuTile*)bound_srv[2]->buffer->data;
 uint32_t*refs=(uint32_t*)bound_srv[1]->buffer->data;
 uint32_t*color=(uint32_t*)bound_uav[0]->buffer->data,*stats=(uint32_t*)bound_uav[2]->buffer->data;
 float*depth=(float*)bound_uav[1]->buffer->data;
 assert(bound_srv[0]->buffer->desc.StructureByteStride==sizeof(NvGpuTri));
 assert(bound_srv[3]->buffer->desc.StructureByteStride==sizeof(NvGpuState));
 for(unsigned group=0;group<n;group++) {
  unsigned tile=group/4,quadrant=group&3;NvGpuTile q=tiles[tile];
  unsigned left=q.x+(quadrant&1)*8,top=q.y+(quadrant>>1)*8;
  for(unsigned row=top;row<top+8 && row<s->height;row++)for(unsigned x=left;x<left+8 && x<s->width;x++) {
   unsigned at=row*s->width+x;
   for(unsigned j=0;j<q.count;j++) {
    NvGpuPixel p=gpu_pixel(s[tri[refs[q.first+j]].state_index],tri[refs[q.first+j]],x,row,color[at],depth[at]);
    color[at]=p.color;depth[at]=p.depth;
    unsigned base=tile*4;
    stats[base]+=p.depth_fail;stats[base+1]+=p.pixels;stats[base+2]+=p.zpass;
    if((p.pixel_max&0xFFFFFF)>(stats[base+3]&0xFFFFFF))stats[base+3]=p.pixel_max;
   }
  }
 }
 if(corrupt_shader || (fixed_corrupt && !g.array_registers && !g.queue_simple) || (simple_corrupt && g.queue_simple))color[17]^=0xFF00;
}

#include "qpr2_gpu_coverage_fixture.h"
static void mock_native_draw(ID3D11DeviceContext*ctx,unsigned vertices,unsigned first_vertex){
 /* The CPU fixture samples the actual simulated GPU atlas for GPU-only
  * entries. Never substitute their intentionally empty host placeholders. */
 uint32_t*saved_decoded[128];
 for(unsigned i=0;i<128;i++){saved_decoded[i]=g.cache[i].decoded;if(g.cache[i].gpu_serial)g.cache[i].decoded=(uint32_t*)g.atlas.buffer->data+g.cache[i].atlas_base;}
 (void)ctx;unsigned factor=bound_vertex==&coverage_vertex_shader?3:6;
 assert((factor==3)==(bound_rasterizer==&coverage_rasterizer));
 assert(vertices%factor==0 && first_vertex%factor==0 && vs_bound);unsigned first=first_vertex/factor,n=vertices/factor;++native_draws;
 if(factor==3){++coverage_draws;assert(!bound_scissor.left && !bound_scissor.top && bound_scissor.right==(int)native_view.Width && bound_scissor.bottom==(int)native_view.Height);}
 unsigned literal=mock_literal_id(bound_pixel);int compact=literal && literal_is_compact[literal-1];if(literal)++literal_draws[literal-1];if(compact)++compact_draws[literal-1];
 if(bound_pixel==&simple_pixel_shader){++native_simple_draws;if(native_simple_readback_fail && g.target.width==256)fail_next_map=1;}
 NvGpuTri*tri=(NvGpuTri*)bound_srv[0]->buffer->data;NvGpuState*state=(NvGpuState*)bound_srv[3]->buffer->data;
 GpuSurface*surface=(GpuSurface*)bound_uav[1]->buffer->data;
 uint32_t*stats=(uint32_t*)bound_uav[2]->buffer->data;
 assert(!bound_uav[0] && bound_uav[1]->buffer->desc.StructureByteStride==8);
 if(g.uav_only){assert(!bound_raster);++uav_only_draws;if(uav_only_map_fail)fail_next_map=1;}
 else assert(bound_raster && bound_raster->texture->desc.Width==state->width && bound_raster->texture->desc.Height==state->height);
 assert(native_view.Width==state->width && native_view.Height==state->height && first+n<=g.queued_count);
 /* Native rectangles only generate candidates; exact shared edges and depth
  * remain authoritative. Visit all primitives in submission order per pixel. */
 for(unsigned primitive=first;primitive<first+n;primitive++){
  NvGpuTri t=tri[primitive];NvGpuState s=state[t.state_index];
  if(bound_pixel==&simple_pixel_shader && t.use_rc)assert(gpu_rc_simple_kind(s.rc));
  if(literal && t.use_rc)assert(nv_gpu_literal_program(&s.rc)==literal);
  int left=t.minx>0?t.minx-1:0,top=t.miny>0?t.miny-1:0;
  int right=t.maxx<(int)s.width?t.maxx+1:s.width,bottom=t.maxy<(int)s.height?t.maxy+1:s.height;
  for(int y=top;y<bottom;y++)for(int x=left;x<right;x++){
   if(factor==3 && !coverage_candidate(&t,x,y))continue;
   unsigned at=(unsigned)y*s.width+x;float before_depth;uint32_t before_color=surface[at].color,before_z=surface[at].depth_bits;
   if(native_stale && primitive==3 && x==10 && y==10){before_color=0xFF112233u;float stale_z=100;memcpy(&before_z,&stale_z,4);}
   memcpy(&before_depth,&before_z,4);
   NvGpuBary bary=gpu_bary(s,t,x,y);NvGpuSource source={0};
   int shade=bary.covered;
   if(shade && bound_seed_srv[1] && *(uint32_t*)bound_seed_srv[1]->buffer->data){
    assert(bound_seed_srv[0] && bound_seed_srv[0]->buffer!=bound_uav[1]->buffer);
    GpuSurface *seed=(GpuSurface*)bound_seed_srv[0]->buffer->data;float z;memcpy(&z,&seed[at].depth_bits,4);
    shade=gpu_seed_source_visible(s,bary,z);
   }
   /* Frozen visibility is settled before source work or ordered writes. */
   if(g.empty_reject && !g.native_diagnostics && !shade){
    NvGpuPixel ref=gpu_pixel(s,t,x,y,before_color,before_depth);uint32_t zbits;memcpy(&zbits,&ref.depth,4);
    assert(ref.color==before_color && zbits==before_z && !ref.zpass);++mock_reject_before_source;continue;
   }
   if(g.early_depth && !g.native_diagnostics && shade && s.depth_available && s.depth_test &&
      !gpu_compare(bary.depth,before_depth,s.depth_func)){
    NvGpuPixel ref=gpu_pixel(s,t,x,y,before_color,before_depth);
    assert(ref.color==before_color && !ref.zpass);
    ++mock_early_depth_reject;continue;
   }
   mock_apply_stage_masks=g.material.enabled;
   if(shade && compact && literal==1)source=mock_compact_source_1(s,t,bary.l0,bary.l1,bary.l2);
   else if(shade && compact && literal==2)source=mock_compact_source_2(s,t,bary.l0,bary.l1,bary.l2);
   else if(shade && compact && literal==3)source=mock_compact_source_3(s,t,bary.l0,bary.l1,bary.l2);
   else if(shade && literal==1)source=mock_literal_source_1(s,t,bary.l0,bary.l1,bary.l2);
   else if(shade && literal==2)source=mock_literal_source_2(s,t,bary.l0,bary.l1,bary.l2);
   else if(shade && literal==3)source=mock_literal_source_3(s,t,bary.l0,bary.l1,bary.l2);
   else
   if(shade)source=gpu_source_color(s,t,bary.l0,bary.l1,bary.l2);
   mock_apply_stage_masks=0;
   if(g.empty_reject && !g.native_diagnostics && !source.keep){
    NvGpuPixel ref=gpu_pixel(s,t,x,y,before_color,before_depth);uint32_t zbits;memcpy(&zbits,&ref.depth,4);
    assert(ref.color==before_color && zbits==before_z && !ref.zpass);++mock_reject_after_source;continue;
   }
   ++mock_ordered_transactions;
   NvGpuPixel p=gpu_commit(s,bary,source,before_color,before_depth);
   NvGpuPixel reference=gpu_pixel(s,t,x,y,before_color,before_depth);
   assert(!memcmp(&p,&reference,sizeof p));
   if(literal){
    if(literal_color_bad==(int)literal)p.color^=0x40;
    if(literal_depth_bad==(int)literal)p.depth=-123;
    if(literal_count_bad==(int)literal)p.zpass^=1;
    if(literal_variant_bad && literal==2 && g.literal.variant==1)p.color^=0x40;
    if(literal_map_fail==(int)literal)fail_next_map=1;
    if(compact){
     if(compact_color_bad==(int)literal)p.color^=0x40;
     if(compact_depth_bad==(int)literal)p.depth=-123;
     if(compact_count_bad==(int)literal)p.zpass^=1;
     if(compact_variant_bad && literal==2 && g.literal.variant==1)p.color^=0x40;
     if(compact_map_fail==(int)literal)fail_next_map=1;
    }
   }
   if(native_arithmetic && primitive==3 && x==10 && y==10)p.color^=0xFF00u;
   if(s.width==32 && s.height==32 && primitive<8) {
    uint32_t*trace=(uint32_t*)bound_uav[3]->buffer->data+(primitive*1024+at)*4;
    trace[0]=before_color;trace[1]=before_z;trace[2]=p.color;memcpy(trace+3,&p.depth,4);
   }
   uint32_t depth_bits;memcpy(&depth_bits,&p.depth,4);
   surface[at].color=p.color;surface[at].depth_bits=depth_bits;
   unsigned base=((unsigned)y/16*((s.width+15)/16)+(unsigned)x/16)*4;
   if(!g.native_diagnostics){p.depth_fail=p.pixels=p.pixel_max=0;if(quiet_zpass_corrupt)p.zpass=0;}
   stats[base]+=p.depth_fail;stats[base+1]+=p.pixels;stats[base+2]+=p.zpass;
   if((p.pixel_max&0xFFFFFF)>(stats[base+3]&0xFFFFFF))stats[base+3]=p.pixel_max;
  }
 }
 if(native_batch_corrupt && first_vertex && g.native_batch)surface[17].color^=0xFF00;
 if(factor==3 && fabsf(tri[0].area)<0.02f)++coverage_thin_draws;
 if(factor==3 && (coverage_corrupt || (coverage_thin_corrupt && fabsf(tri[0].area)<0.02f)))surface[17].color^=0xFF00;
 if(g.uav_only && uav_only_corrupt)surface[17].color^=0xFF00;
 if(native_corrupt || (bound_pixel==&simple_pixel_shader && (native_simple_corrupt || (native_simple_large_corrupt && state->width==256))))surface[state->width==256?180*256+40:17].color^=0xFF00;
 for(unsigned i=0;i<128;i++)g.cache[i].decoded=saved_decoded[i];
}
static void ID3D11DeviceContext_Draw(ID3D11DeviceContext*ctx,unsigned count,unsigned first){assert(!first);mock_native_draw(ctx,count,0);}
static void ID3D11DeviceContext_DrawIndexed(ID3D11DeviceContext*ctx,unsigned count,unsigned first,int base){assert(bound_indices && !base && first+count<=bound_indices->desc.ByteWidth/4);const uint32_t*ids=(const uint32_t*)bound_indices->data;for(unsigned i=0;i<count;i++)assert(ids[first+i]==first+i);mock_native_draw(ctx,count,ids[first]);}
static int decode(void*raw,uint32_t face,uint32_t x,uint32_t y,uint32_t*value){(void)face;*value=((uint32_t*)raw)[y*16+x];return 1;}
static NvGpuTri triangle(int width){
 NvGpuTri t={0};t.v[1].pos[0]=width;t.v[2].pos[1]=width;t.area=(float)(width*width);t.inv_area=1/t.area;
 t.maxx=t.maxy=width;
 for(int k=0;k<3;k++) {
  t.v[k].pos[2]=10;t.v[k].pos[3]=t.iw[k]=1;
  for(int j=0;j<4;j++)t.v[k].d0[j]=1;
  t.uv[k][0]=t.v[k].pos[0];t.uv[k][1]=t.v[k].pos[1];
 }
 t.inclusive[0]=nv2a_edge_inclusive(t.v[0].pos,t.v[1].pos,t.area);
 t.inclusive[1]=nv2a_edge_inclusive(t.v[1].pos,t.v[2].pos,t.area);
 t.inclusive[2]=nv2a_edge_inclusive(t.v[2].pos,t.v[0].pos,t.area);return t;
}
int main(int argc,char**argv){
 int restored_defaults=argc>1 && !strcmp(argv[1],"native-restored-defaults");
 int quiet_mode=argc>1 && (restored_defaults || !strcmp(argv[1],"native-quiet") || !strcmp(argv[1],"quiet-compute") || !strcmp(argv[1],"native-quiet-zpass-bad"));
 if(restored_defaults){
  unsetenv("RECOMP_GPU_DIAGNOSTIC_COUNTERS");
  setenv("RECOMP_GPU_EARLY_DEPTH","1",1);setenv("RECOMP_GPU_PARTIAL_READBACK","1",1);
 }else setenv("RECOMP_GPU_DIAGNOSTIC_COUNTERS",quiet_mode?"0":"1",1);
 quiet_zpass_corrupt=argc>1 && !strcmp(argv[1],"native-quiet-zpass-bad");
 setenv("RECOMP_GPU_RASTER","1",1);setenv("RECOMP_GPU_ASYNC","0",1);
 setenv("RECOMP_GPU_CAPTURE","capture.bin",1);
 int coverage_mode=argc>1 && !strncmp(argv[1],"coverage",8);
 int native_mode=coverage_mode || (argc>1 && !strncmp(argv[1],"native",6));
 if(argc>1){
  if(!strcmp(argv[1],"native-uav-disabled"))setenv("RECOMP_GPU_UAV_ONLY","0",1);
  uav_only_corrupt=!strcmp(argv[1],"native-uav-bad");uav_only_map_fail=!strcmp(argv[1],"native-uav-readback");
  if(!strcmp(argv[1],"native-pool-disabled"))setenv("RECOMP_GPU_TARGET_POOL","0",1);
  if(!strcmp(argv[1],"native-disjoint-disabled"))setenv("RECOMP_GPU_DISJOINT_CLEAR","0",1);
 }
 int clear_mode=argc>1 && strstr(argv[1],"-clear-");
 if(clear_mode){clear_compile_fail=!strcmp(argv[1],"native-clear-compile");clear_create_fail=!strcmp(argv[1],"native-clear-create");
  clear_corrupt=!strcmp(argv[1],"native-clear-bad");clear_map_fail=!strcmp(argv[1],"native-clear-readback");
  if(!strcmp(argv[1],"native-clear-disabled"))setenv("RECOMP_GPU_CLEAR","0",1);}
 int simple_mode=argc>1 && !strncmp(argv[1],"simple-",7);
 int report_mode=argc>1 && !strncmp(argv[1],"report-",7);
 if(report_mode){report_compile_fail=!strcmp(argv[1],"report-compile");report_corrupt=!strcmp(argv[1],"report-bad");
  if(!strcmp(argv[1],"report-disabled"))setenv("RECOMP_GPU_REPORT_RESET","0",1);}
 int fixed_mode=argc>1 && !strncmp(argv[1],"fixed-",6);
 if(fixed_mode){fixed_compile_fail=!strcmp(argv[1],"fixed-compile");fixed_corrupt=!strcmp(argv[1],"fixed-bad");
  if(!strcmp(argv[1],"fixed-disabled"))setenv("RECOMP_GPU_FIXED","0",1);}
 int legacy_mode=argc>1 && !strncmp(argv[1],"legacy-",7);
 if(legacy_mode)legacy_runtime=!strcmp(argv[1],"legacy-11")?2:1;
 if(simple_mode){simple_compile_fail=!strcmp(argv[1],"simple-compile");simple_corrupt=!strcmp(argv[1],"simple-bad");}
 if(coverage_mode){coverage_tier=3;
  if(!strcmp(argv[1],"coverage-tier1"))coverage_tier=1;
  if(!strcmp(argv[1],"coverage-tier2"))coverage_tier=2;
  if(!strcmp(argv[1],"coverage-unsupported"))coverage_tier=0;
  if(!strcmp(argv[1],"coverage-disabled"))setenv("RECOMP_GPU_COVERAGE","0",1);
  if(!strcmp(argv[1],"coverage-general"))setenv("RECOMP_GPU_NATIVE_SIMPLE","0",1);
  if(!strcmp(argv[1],"coverage-array"))setenv("RECOMP_GPU_FIXED","0",1);
  coverage_query_fail=!strcmp(argv[1],"coverage-query");coverage_state_fail=!strcmp(argv[1],"coverage-state");coverage_compile_fail=!strcmp(argv[1],"coverage-compile");coverage_create_fail=!strcmp(argv[1],"coverage-create");coverage_corrupt=!strcmp(argv[1],"coverage-bad");coverage_thin_corrupt=!strcmp(argv[1],"coverage-thin-bad");
 }
 if(native_mode){if(!strcmp(argv[1],"native-array"))setenv("RECOMP_GPU_FIXED","0",1);
  index_create_fail=!strcmp(argv[1],"native-index-fail") || !strcmp(argv[1],"coverage-index-fail");
  native_batch_corrupt=!strcmp(argv[1],"native-batch-bad") || !strcmp(argv[1],"coverage-batch-bad");
  if(!strcmp(argv[1],"native-batch-disabled"))setenv("RECOMP_GPU_BATCH_UPLOADS","0",1);
  native_simple_readback_fail=!strcmp(argv[1],"native-simple-readback");
  native_simple_compile_fail=!strcmp(argv[1],"native-simple-compile");native_simple_create_fail=!strcmp(argv[1],"native-simple-create");native_simple_corrupt=!strcmp(argv[1],"native-simple-bad");native_simple_large_corrupt=!strcmp(argv[1],"native-simple-large-bad");
  if(!strcmp(argv[1],"native-simple-disabled"))setenv("RECOMP_GPU_NATIVE_SIMPLE","0",1);
  if(!strcmp(argv[1],"native-early-off"))setenv("RECOMP_GPU_EARLY_DEPTH","0",1);
  if(!strcmp(argv[1],"native-early-default"))unsetenv("RECOMP_GPU_EARLY_DEPTH");
  early_compile_fail=!strcmp(argv[1],"native-early-compile");
  early_simple_compile_fail=!strcmp(argv[1],"native-early-simple-compile");
  native_stale=!strcmp(argv[1],"native-order");native_arithmetic=!strcmp(argv[1],"native-math");native_supported=1;native_corrupt=!strcmp(argv[1],"native-bad");native_compile_fail=!strcmp(argv[1],"native-compile");native_create_fail=!strcmp(argv[1],"native-create");raster_create_fail=!strcmp(argv[1],"native-target");}
 if(argc>1){failed_device=!strcmp(argv[1],"device");failed_compile=!strcmp(argv[1],"compile");corrupt_shader=!strcmp(argv[1],"shader");}
 NvGpuState s={0};s.width=s.height=16;s.pitch=64;s.bpp=4;s.format=0x128;s.color_mask=0x01010101;
 s.depth_available=s.depth_test=s.depth_mask=s.zpass_enable=1;s.depth_func=0x203;
 NvGpuTri t=triangle(16);uint32_t color[256]={0},other[256]={0};float depth[256];
 for(int i=0;i<256;i++)depth[i]=100;
 NvGpuTextureSource source[4]={0};

 if(argc>1 && (!strcmp(argv[1],"async") || !strcmp(argv[1],"thread"))) {
  setenv("RECOMP_GPU_ASYNC","1",1);startup_gate=1;create_thread_fail=!strcmp(argv[1],"thread");
  assert(!nv_gpu_available());NvGpuCount pending={0};
  assert(nv_gpu_sync(&pending)==1 && !pending.pixels && !color[0]);nv_gpu_report();
  s_capture=fopen("blocked-startup-capture.bin","wb");assert(s_capture);s_capture_bytes=0;s_capture_presents=0;
  nv_gpu_capture_present(color,16,16);nv_gpu_capture_present(color,16,16);
  assert(s_capture && ftell(s_capture)==0 && !s_capture_presents && !s_capture_bytes);
  fclose(s_capture);s_capture=NULL;
  puts("PASS: guest CPU presentation cannot append or close the setup worker's private capture");
  if(create_thread_fail){assert(s_startup==3);puts("PASS: setup-thread failure retains CPU renderer");return 0;}
  assert(s_startup==1 && g.init==0);
  pthread_mutex_lock(&gate_lock);startup_gate=0;pthread_cond_signal(&gate_cond);pthread_mutex_unlock(&gate_lock);
  while(InterlockedCompareExchange(&s_startup,0,0)==1)sched_yield();
  assert(nv_gpu_available() && s_startup==2 && g.init==3 && !g.active && !color[0]);
  assert(nv_gpu_draw(&s,&t,1,(uint8_t*)color,depth,16,source)==1);
  assert(nv_gpu_sync(&pending)==1 && color[0]==0xFFFFFFFF);
  puts("PASS: delayed background initialization leaves guest pixels/completion untouched; CPU-to-GPU handoff after checks");return 0;
 }
 int result=nv_gpu_draw(&s,&t,1,(uint8_t*)color,depth,16,source);
 if(clear_map_fail || uav_only_map_fail){assert(result==0 && s_startup==3 && !g.active && !g.host_color && !g.host_depth && !color[0]);puts("PASS: surface-clear readback failure cannot publish hardware readiness");return 0;}
 if(native_simple_readback_fail){assert(result==0 && s_startup==3 && g.init==1 && !g.active && !g.host_color && !g.host_depth && !color[0]);puts("PASS: large native fixture readback failure cannot publish hardware readiness or retain freed CPU buffers");return 0;}
 if(argc>1 && !native_mode && !simple_mode && !legacy_mode && !fixed_mode && !report_mode && !quiet_mode){assert(result==0 && !g.active && g.init==1);puts("PASS: hardware/device/compiler/real-selftest failure leaves guest pixels untouched and uses CPU fallback");return 0;}
 if(report_mode){assert(!g.report_shader && nv_gpu_reset_report()==0);puts("PASS: report-reset compiler/comparison failure or explicit disable retains synchronous reset path");}
 if(fixed_mode){assert(g.array_registers);puts("PASS: named-register compile/comparison failure or explicit disable retains validated array compute");}
 if(legacy_mode){assert(device_calls==legacy_runtime+1 && selected_feature_level==D3D_FEATURE_LEVEL_11_0);puts("PASS: newer feature-level negotiation retries unsupported runtime enumerations and retains 11_0 compute support");}
 if(simple_mode){assert(!g.simple_shader && !g.native);puts("PASS: specialized compiler/self-test failure retains validated general compute backend");}
 assert(result==1 && g.init==3 && g.active && !color[0]);
 if(quiet_mode){
  NvGpuCount initial;assert(nv_gpu_sync(&initial)==1);
  assert(g.native==(restored_defaults || !strcmp(argv[1],"native-quiet")));
  if(restored_defaults)assert(!g.native_diagnostics);
  assert(initial.zpass && (g.native?!initial.pixels:initial.pixels>0));
  if(g.native && g.report_shader){
   uint32_t fast[256]={0};float fast_depth[256];
   for(unsigned i=0;i<256;i++)fast_depth[i]=100;
   NvGpuCount first,second,final;
   uint64_t readbacks=g.readbacks;
   assert(nv_gpu_draw(&s,&t,1,(uint8_t*)fast,fast_depth,16,source)==1);
   assert(nv_gpu_sync_report(&first)==1 && first.zpass>0 && g.active &&
          g.readbacks==readbacks && fast[0]==0 && fast_depth[0]==100);
   assert(nv_gpu_draw(&s,&t,1,(uint8_t*)fast,fast_depth,16,source)==1);
   assert(nv_gpu_sync_report(&second)==1 && second.zpass==first.zpass &&
          g.active && g.readbacks==readbacks && fast[0]==0);
   assert(nv_gpu_sync(&final)==1 && fast[0]==0xFFFFFFFF && fast_depth[0]==10 &&
          final.zpass==0 && g.report_only>=2);
   assert(nv_gpu_sync_report(&final)==0);
   puts("PASS: visibility report reads counters twice without copying live pixels; later full completion publishes exact pixels and no duplicate zpass");
  }
  for(unsigned pass=0;pass<3;pass++){
   uint32_t expected[256];float reference_depth[256];NvGpuCount reference={0},count;
   for(unsigned i=0;i<256;i++){color[i]=expected[i]=0xFF123456;depth[i]=reference_depth[i]=100;}
   s.blend_enable=pass==1;s.blend_sfactor=0x302;s.blend_dfactor=0x303;s.blend_equation=0x8006;
   s.alpha_test=pass==2;s.alpha_func=0x204;s.alpha_ref=100;
   for(unsigned draw=0;draw<20;draw++){
    NvGpuTri next=t;s.zpass_enable=draw%3!=0;
    for(unsigned i=0;i<3;i++){
     next.v[i].pos[2]=draw%4==0?110.0f:50.0f-(float)draw;
     next.v[i].d0[0]=0.1f+(float)(draw%6)*0.15f;
     next.v[i].d0[3]=(float)(draw%3)*0.5f;
    }
    assert(nv_gpu_draw(&s,&next,1,(uint8_t*)color,depth,16,source)==1);
    for(unsigned y=0;y<16;y++)for(unsigned x=0;x<16;x++){
     unsigned at=y*16+x;NvGpuPixel px=gpu_pixel(s,next,x,y,expected[at],reference_depth[at]);
     expected[at]=px.color;reference_depth[at]=px.depth;reference.pixels+=px.pixels;
     reference.depth_fail+=px.depth_fail;reference.zpass+=px.zpass;
    }
    if(draw==7){assert(nv_gpu_reset_report()==1);reference.zpass=0;}
   }
   assert(nv_gpu_sync(&count)==1 && !memcmp(color,expected,sizeof color) && !memcmp(depth,reference_depth,sizeof depth));
   assert(reference.zpass && count.zpass==reference.zpass);
   if(g.native)assert(!count.depth_fail && !count.pixels && !count.pixel_max);
   else assert(count.depth_fail==reference.depth_fail && count.pixels==reference.pixels);
  }
  if(g.native && g.state_intern_enabled)assert(g.state_intern_hits>0 && g.state_intern_bytes_saved>0);
  if(g.native && g.seed.enabled){
   NvGpuTri repeated[128];uint32_t flat[256]={0};float flat_depth[256];NvGpuCount completed;
   for(unsigned i=0;i<128;i++)repeated[i]=t;
   for(unsigned i=0;i<256;i++)flat_depth[i]=100;
   NvGpuState color_only=s;color_only.depth_mask=0;color_only.depth_test=1;color_only.depth_func=0x203;
   color_only.alpha_test=0;color_only.blend_enable=0;color_only.zpass_enable=1;
   uint64_t copies=g.seed.copies,reuses=g.seed.reuses;
   for(unsigned batch=0;batch<3;batch++){
    assert(nv_gpu_draw(&color_only,repeated,128,(uint8_t*)flat,flat_depth,16,source)==1);
    assert(nv_gpu_submit()==1);
   }
   assert(nv_gpu_sync(&completed)==1 && completed.zpass>0);
   assert(flat[0]==0xFFFFFFFF && flat_depth[0]==100);
   if(g.seed.reuse_enabled)assert(g.seed.reuses>reuses && g.seed.copies==copies+1);
   else assert(g.seed.reuses==reuses && g.seed.copies>=copies+2);
   puts("PASS: color-only batches preserve exact depth and reuse only an unchanged depth snapshot");
  }
  if(g.native && g.early_depth)assert(mock_early_depth_reject>0);
  if(g.native && g.micro_fuse.enabled && g.native_batch){
   NvGpuTri tiny[24];uint32_t fused[256],expected[256];float z[256],reference_z[256];NvGpuCount result;
   NvGpuState part=s;part.alpha_test=part.blend_enable=0;part.zpass_enable=0;
   for(unsigned i=0;i<256;i++){fused[i]=expected[i]=0xFF123456;z[i]=reference_z[i]=100;}
   for(unsigned i=0;i<24;i++)tiny[i]=t;
   uint64_t draws=g.dispatches,saved=g.micro_fuse.runs_saved;
   assert(nv_gpu_draw(&part,tiny,24,(uint8_t*)fused,z,16,source)==1 && g.queued_count==24);
   /* Exercise the same indexed mixed-kind queue using a single state: the
    * general shader must consume both kinds in original primitive order. */
   for(unsigned i=0;i<24;i++)g.queued_kind[i]=(uint8_t)(i&1);
   for(unsigned i=0;i<24;i++)for(unsigned y=0;y<16;y++)for(unsigned x=0;x<16;x++){
    unsigned at=y*16+x;NvGpuPixel px=gpu_pixel(part,tiny[i],x,y,expected[at],reference_z[at]);
    expected[at]=px.color;reference_z[at]=px.depth;
   }
   assert(nv_gpu_sync(&result)==1 && g.dispatches==draws+1 && g.micro_fuse.runs_saved>=saved+23);
   assert(!memcmp(fused,expected,sizeof fused) && !memcmp(z,reference_z,sizeof z));
   puts("PASS: short mixed material runs fuse into one indexed general draw with exact ordered color and depth");
  }
  puts("PASS: visibility-only native counters preserve exact color/depth, alpha/blending, guest visibility enable/reset/report; compute fallback retains full counters");return 0;
 }

 /* An unrelated CPU clear cannot force a submit/readback. Address inequality
  * alone is insufficient: shifted aliases, depth and wrapping spans reject. */
 uint8_t unrelated[2048];memset(unrelated,0x25,sizeof unrelated);
 unsigned pending=g.queued_count;uint64_t reads_before=g.readbacks;
 int disjoint_disabled=argc>1 && !strcmp(argv[1],"native-disjoint-disabled");
 assert(nv_gpu_cpu_write_disjoint(unrelated,sizeof unrelated)==!disjoint_disabled);
 assert(!nv_gpu_cpu_write_disjoint(color,1));
 assert(!nv_gpu_cpu_write_disjoint((uint8_t*)color+16*16*4-1,2));
 assert(!nv_gpu_cpu_write_disjoint((uint8_t*)depth+3,1));
 assert(!nv_gpu_cpu_write_disjoint((void*)(uintptr_t)(UINTPTR_MAX-3),8));
 assert(!nv_gpu_cpu_write_disjoint(NULL,1) && !nv_gpu_cpu_write_disjoint(unrelated,0));
 assert(g.queued_count==pending && g.active && g.readbacks==reads_before && !color[0]);
 memset(unrelated,0x91,sizeof unrelated);
 if(clear_mode)assert(!g.clear_shader && g.native);
 NvGpuCount count;assert(nv_gpu_sync_named(&count,"test-fence")==1 && !g.active && color[0]==0xFFFFFFFF && count.pixels>0);
 for(unsigned i=0;i<sizeof unrelated;i++)assert(unrelated[i]==0x91);
 assert(!nv_gpu_cpu_write_disjoint(unrelated,sizeof unrelated));
 puts("PASS: disjoint CPU writes retain pending rendering; shifted color/depth aliases, overflow and empty spans require sync; fence output preserved");
 if(g.report_shader) {
  unsigned covered=(unsigned)count.pixels;uint32_t a[256]={0},b[256]={0};float az[256],bz[256];
  for(int at=0;at<256;at++)az[at]=bz[at]=100;
  assert(nv_gpu_draw(&s,&t,1,(uint8_t*)a,az,16,source)==1);
  assert(nv_gpu_draw(&s,&t,1,(uint8_t*)b,bz,16,source)==1);
  if(g.readback_queue)assert(!a[0] && !b[0] && g.pending_count==1 && !g.completed.zpass);
  if(g.pending_count)assert(nv_gpu_sync_report(&count)==0);
  else assert(a[0]==0xFFFFFFFF && !b[0] && g.completed.pixels==covered && g.completed.zpass==covered);
  uint64_t reset_reads=g.readbacks;assert(nv_gpu_reset_report()==1);
  assert(g.active && !b[0] && g.readbacks==reset_reads+(g.readback_queue?1u:0u) && !g.completed.zpass && g.completed.pixels==covered);
  assert(nv_gpu_draw(&s,&t,1,(uint8_t*)b,bz,16,source)==1 && nv_gpu_sync(&count)==1);
  assert(count.pixels==covered*3 && count.zpass==covered && b[0]==0xFFFFFFFF);
  reset_reads=g.readbacks;assert(nv_gpu_reset_report()==1 && g.readbacks==reset_reads);
  puts("PASS: GPU-only visibility reset preserves pending pixels, depth and target-change diagnostics; results contain only post-reset zpass; queued pre-reset copies drain before reset");
 }
 assert(!strcmp(g.sync_reason[0].name,"test-fence") && g.sync_reason[0].count==1);
 if(g.clear_shader){
  uint32_t colors[256];float zs[16*20];for(unsigned i=0;i<256;i++)colors[i]=0xFF112233;
  for(unsigned i=0;i<16*20;i++)zs[i]=100;
  uint64_t reads=g.readbacks;unsigned commands=clear_dispatches;
  nv_gpu_capture_request();assert(nv_gpu_draw(&s,&t,1,(uint8_t*)colors,zs,20,source)==1);
  assert(nv_gpu_clear_color(&s,(uint8_t*)colors,2,3,13,15,0xFF1267AB)==1);
  /* The executor still performs its CPU clears, while GPU writes stay pending. */
  for(unsigned y=3;y<15;y++)for(unsigned x=2;x<13;x++)colors[y*16+x]=0xFF1267AB;
  assert(nv_gpu_clear_depth(zs,20,75)==1);for(unsigned i=0;i<16*20;i++)zs[i]=75;
  assert(g.active && g.readbacks==reads && clear_dispatches==commands+2 && colors[0]==0xFF112233);
  assert(nv_gpu_clear_color(&s,(uint8_t*)color,0,0,16,16,0)==0 && g.active);
  NvGpuState wrong=s;wrong.pitch+=4;assert(!nv_gpu_clear_color(&wrong,(uint8_t*)colors,0,0,16,16,0));
  assert(!nv_gpu_clear_color(&s,(uint8_t*)colors,0,0,17,16,0));assert(!nv_gpu_clear_depth(depth,16,0));
  assert(nv_gpu_draw(&s,&t,1,(uint8_t*)colors,zs,20,source)==1 && nv_gpu_sync_named(&count,"clear-test")==1);
  for(unsigned y=0;y<16;y++)for(unsigned x=0;x<20;x++){
   if(x>=16){assert(zs[y*20+x]==75);continue;}
   NvGpuPixel pixel=gpu_pixel(s,t,x,y,0xFF112233,100);int covered=pixel.pixels!=0;
   uint32_t expected=covered?0xFFFFFFFF:(x>=2 && x<13 && y>=3 && y<15)?0xFF1267AB:0xFF112233;
   assert(colors[y*16+x]==expected && zs[y*20+x]==(covered?10:75));
  }
  assert(g.readbacks==reads+1 && count.pixels==240 && count.zpass==240);
  uint64_t cu=g.color_uploads,du=g.depth_uploads;
  assert(nv_gpu_draw(&s,&t,1,(uint8_t*)colors,zs,20,source)==1 && nv_gpu_sync(&count)==1);
  assert(g.color_uploads==cu && g.depth_uploads==du);
  nv_gpu_capture_present(colors,16,16);nv_gpu_capture_present(colors,16,16);assert(!s_capture);
  assert(!rename("capture.bin","clear-capture.bin"));
  assert(!nv_gpu_clear_depth(zs,20,0) && !nv_gpu_clear_color(&s,(uint8_t*)colors,0,0,16,16,0));
  puts("PASS: ordered color/depth clears preserve deferred draws, counters, CPU mirrors, padding, target identity and resident reuse; capture contains clears");
  /* Restore the original resident target for the following reuse checks. */
  assert(nv_gpu_draw(&s,&t,1,(uint8_t*)color,depth,16,source)==1 && nv_gpu_sync(&count)==1);
 }
 /* Completed GPU storage is reusable only while CPU bytes still match it. */
 uint64_t cu=g.color_uploads,du=g.depth_uploads,cr=g.color_reuses,dr=g.depth_reuses;
 assert(nv_gpu_draw(&s,&t,1,(uint8_t*)color,depth,16,source)==1);
 assert(nv_gpu_sync(&count)==1 && g.color_uploads==cu && g.depth_uploads==du &&
        g.color_reuses==cr+1 && g.depth_reuses==dr+1);
 color[255]=0xFF654321;depth[255]=99;
 assert(nv_gpu_draw(&s,&t,1,(uint8_t*)color,depth,16,source)==1);
 assert(nv_gpu_sync(&count)==1 && g.color_uploads==cu+1 && g.depth_uploads==du+1 &&
        color[255]==0xFF654321 && depth[255]==99);
 puts("PASS: identical completed targets reuse GPU color/depth; CPU edits force uploads and survive untouched pixels");
 if(g.native?g.simple_pixel_shader!=NULL:g.simple_shader!=NULL){
  uint64_t dispatched=g.dispatches,specialized=g.simple_dispatches,uploads=g.queue_uploads;
  NvGpuTri general=t;general.use_rc=1;
  assert(nv_gpu_draw(&s,&t,1,(uint8_t*)color,depth,16,source)==1);
  assert(nv_gpu_draw(&s,&general,1,(uint8_t*)color,depth,16,source)==1);
  assert(nv_gpu_draw(&s,&t,1,(uint8_t*)color,depth,16,source)==1);
  assert(nv_gpu_sync(&count)==1 && color[0]==0xFFFFFFFF);
  if(g.native && g.native_batch && g.micro_fuse.enabled){
   assert(g.dispatches==dispatched+1 && g.simple_dispatches==specialized && g.micro_fuse.runs_saved>=2);
  }else assert(g.dispatches==dispatched+3 && g.simple_dispatches==specialized+2);
  assert(g.queue_uploads==uploads+(g.native && g.native_batch?1:3));
  puts("PASS: simple/general/simple transitions preserve order; shared native upload combines all three runs");
 }
 /* Resident consecutive draws defer readback until completion. */
 assert(nv_gpu_draw(&s,&t,1,(uint8_t*)color,depth,16,source)==1);
 uint64_t reads=g.readbacks;
 assert(nv_gpu_draw(&s,&t,1,(uint8_t*)color,depth,16,source)==1 && g.readbacks==reads);
 /* Target change must complete color before decoding that old target as a
  * texture; otherwise reflections/rendered textures upload stale pixels. */
 for(int k=0;k<3;k++)t.v[k].d0[0]=0.3f;
 assert(nv_gpu_draw(&s,&t,1,(uint8_t*)color,depth,16,source)==1);
 t.textured=1;
 s.tex[0].valid=1;s.tex[0].width=s.tex[0].height=16;s.tex[0].format=0x12;s.tex[0].addr_u=s.tex[0].addr_v=3;
 source[0].raw=(const uint8_t*)color;source[0].bytes=sizeof color;source[0].opaque=color;source[0].decode=decode;
 assert(nv_gpu_draw(&s,&t,1,(uint8_t*)other,depth,16,source)==1 && g.readbacks>reads);
 assert(g.bound[0]->decoded[0]==color[0] && color[0]!=0xFFFFFFFF);
 assert(nv_gpu_sync(&count)==1);uint64_t uploads=g.uploads,hot_hits=g.texture_hot_hits;
 assert(nv_gpu_draw(&s,&t,1,(uint8_t*)other,depth,16,source)==1 && g.uploads==uploads);
 if(g.texture_hot_enabled)assert(g.texture_hot_hits>hot_hits);
 assert(nv_gpu_sync(&count)==1);
 color[0]^=0x5555;assert(nv_gpu_draw(&s,&t,1,(uint8_t*)other,depth,16,source)==1 && g.uploads==uploads+1);
 assert(nv_gpu_sync(&count)==1 && g.bound[0]->decoded[0]==color[0]);
 /* Five immutable identities evict the first MRU entry. The indexed hint
  * must find it without a full scan, while the original byte check remains. */
 if(g.texture_hot_enabled){
  uint32_t texels[16][256];NvGpuTextureSource ts[16]={0};NvGpuTexture tm=s.tex[0];
  unsigned chosen[5],used=0;uint8_t buckets[256]={0};
  for(unsigned i=0;i<16 && used<5;i++){
   for(unsigned p=0;p<256;p++)texels[i][p]=0xFF000000u+i*0x100u+p;
   ts[i].raw=(const uint8_t*)texels[i];ts[i].bytes=sizeof texels[i];
   ts[i].opaque=texels[i];ts[i].decode=decode;
   unsigned bucket=gpu_texture_bucket(&tm,&ts[i]);
   if(!buckets[bucket]){buckets[bucket]=1;chosen[used++]=i;}
  }
  assert(used==5);
  for(unsigned i=0;i<5;i++){
   unsigned k=chosen[i];++g.tick;GpuTexture *hit=gpu_texture(&tm,&ts[k]);
   assert(hit && hit->decoded[0]==texels[k][0]);
  }
  uint64_t index_hits=g.texture_index_hits,scans=g.texture_full_scans;
  unsigned k=chosen[0];++g.tick;GpuTexture *hit=gpu_texture(&tm,&ts[k]);
  assert(hit && hit->decoded[0]==texels[k][0]);
  assert(g.texture_index_hits==index_hits+1 && g.texture_full_scans==scans);
 }

 /* Sixty heterogeneous guest draws must become one real backend dispatch.
  * Reference each state at the time of submission; old texture bytes may
  * change later, but queued draws must retain the old version. */
 uint32_t merged[256]={0},expected[256]={0},tex_a[256],tex_b[256];float mz[256];
 for(int i=0;i<256;i++){mz[i]=100;tex_a[i]=0xFF2255AA;tex_b[i]=0x80CC4422;}
 NvGpuState multi=s;multi.depth_test=multi.depth_mask=0;multi.blend_enable=1;
 multi.blend_sfactor=0x302;multi.blend_dfactor=0x303;multi.blend_equation=0x8006;
 multi.tex[0].width=multi.tex[0].height=16;multi.tex[0].valid=1;
 multi.tex[0].format=0x12;multi.tex[0].addr_u=multi.tex[0].addr_v=3;
 uint64_t dispatch=g.dispatches;
 for(int draw=0;draw<60;draw++) {
  NvGpuTri mt=triangle(16);mt.textured=1;
  multi.color_mask=draw%3?0x01010101:0x01010001;
  for(int v=0;v<3;v++)mt.v[v].d0[0]=0.2f+(draw%4)*0.15f;
  NvGpuTextureSource ms[4]={0};ms[0].raw=(const uint8_t*)(draw%2?tex_a:tex_b);
  ms[0].bytes=sizeof tex_a;ms[0].opaque=(void*)ms[0].raw;ms[0].decode=decode;
  assert(nv_gpu_draw(&multi,&mt,1,(uint8_t*)merged,mz,16,ms)==1);
  NvGpuState copy=g.queued_state[g.queued[g.queued_count-1].state_index];
  for(int y=0;y<16;y++)for(int x=0;x<16;x++){
   unsigned at=y*16+x;expected[at]=gpu_pixel(copy,mt,x,y,expected[at],100).color;
  }
 }
 assert(g.dispatches==dispatch && g.state_count>0 && g.state_count<=60);
 if(g.state_intern_enabled)assert(g.state_count<60 && g.state_intern_hits>0);
 else assert(g.state_count==60);
 assert(nv_gpu_sync(&count)==1 && g.dispatches==dispatch+1);
 assert(!memcmp(merged,expected,sizeof merged));
 /* Rewrite a pinned texture: dispatch old draws before the same-address
  * texture update; the new draw uses the newly decoded bytes. */
 NvGpuTri mt=triangle(16);mt.textured=1;NvGpuTextureSource ms[4]={0};
 ms[0].raw=(const uint8_t*)tex_a;ms[0].bytes=sizeof tex_a;ms[0].opaque=tex_a;ms[0].decode=decode;
 assert(nv_gpu_draw(&multi,&mt,1,(uint8_t*)merged,mz,16,ms)==1);
 NvGpuState copy=g.queued_state[g.queued[g.queued_count-1].state_index];
 for(int y=0;y<16;y++)for(int x=0;x<16;x++){unsigned at=y*16+x;expected[at]=gpu_pixel(copy,mt,x,y,expected[at],100).color;}
 dispatch=g.dispatches;for(int i=0;i<256;i++)tex_a[i]=0xFF77AA33;
 assert(nv_gpu_draw(&multi,&mt,1,(uint8_t*)merged,mz,16,ms)==1 && g.dispatches==dispatch+1);
 copy=g.queued_state[g.state_count-1];
 for(int y=0;y<16;y++)for(int x=0;x<16;x++){unsigned at=y*16+x;expected[at]=gpu_pixel(copy,mt,x,y,expected[at],100).color;}
 assert(nv_gpu_sync(&count)==1 && !memcmp(merged,expected,sizeof merged));

 /* More queued texture versions than SRV slots forces submission before LRU
  * eviction; all 140 original colors must still blend in order. */
 uint32_t many[140][256];dispatch=g.dispatches;
 for(int draw=0;draw<140;draw++){
  for(int at=0;at<256;at++)many[draw][at]=0x80A00000u+(unsigned)draw*0x0307u;
  ms[0].raw=(const uint8_t*)many[draw];ms[0].opaque=many[draw];
  assert(nv_gpu_draw(&multi,&mt,1,(uint8_t*)merged,mz,16,ms)==1);
  copy=g.queued_state[g.state_count-1];
  for(int y=0;y<16;y++)for(int x=0;x<16;x++){unsigned at=y*16+x;expected[at]=gpu_pixel(copy,mt,x,y,expected[at],100).color;}
 }
 assert(g.dispatches>dispatch);assert(nv_gpu_sync(&count)==1);
 assert(!memcmp(merged,expected,sizeof merged));

 uint64_t warm_uploads=g.uploads,warm_index_hits=g.texture_index_hits;
 for(int draw=60;draw<140;draw++){
  ms[0].raw=(const uint8_t*)many[draw];ms[0].opaque=many[draw];
  assert(nv_gpu_draw(&multi,&mt,1,(uint8_t*)merged,mz,16,ms)==1);
 }
 assert(nv_gpu_sync(&count)==1 && g.uploads==warm_uploads);
 if(g.texture_hot_enabled)assert(g.texture_index_hits>warm_index_hits);
 puts("PASS: 80-texture warm working set produces zero texture reuploads");
 puts("PASS: LRU eviction of 140 queued texture versions preserves submission order and original bytes");

 /* A small menu viewport copies only its changed rectangle, including depth
  * rows; CPU-owned pixels outside that rectangle must survive completion. */
 uint32_t partial[256]={0};float pz[256];for(int at=0;at<256;at++)pz[at]=100;
 NvGpuState ps=s;memset(ps.tex,0,sizeof ps.tex);NvGpuTri pt=triangle(8);NvGpuTextureSource empty[4]={0};
 for(int v=0;v<3;v++){pt.v[v].pos[0]+=4;pt.v[v].pos[1]+=4;}
 pt.minx=pt.miny=4;pt.maxx=pt.maxy=12;
 assert(nv_gpu_draw(&ps,&pt,1,(uint8_t*)partial,pz,16,empty)==1);
 partial[255]=0x12345678;pz[255]=77;
 assert(nv_gpu_sync(&count)==1 && partial[5*16+5]==0xFFFFFFFF);
 assert(partial[255]==0x12345678 && pz[255]==77 && pz[5*16+5]==10);
 puts("PASS: dirty-rectangle row copies preserve pixels/depth outside the menu draw");

 setenv("RECOMP_GPU_CAPTURE","capture.bin",1);nv_gpu_capture_request();
 for(unsigned frame=0;frame<2;frame++){
  assert(nv_gpu_draw(&ps,&pt,1,(uint8_t*)partial,pz,16,empty)==1);
  if(g.report_shader){assert(nv_gpu_reset_report()==1);assert(nv_gpu_draw(&ps,&pt,1,(uint8_t*)partial,pz,16,empty)==1);}
  assert(nv_gpu_sync(&count)==1);nv_gpu_capture_present(partial,16,16);
 }
 assert(!s_capture && s_capture_presents==2);
 FILE*capture=fopen("capture.bin","rb");assert(capture);uint32_t cap_header[4];assert(fread(cap_header,sizeof cap_header,1,capture)==1);
 assert(cap_header[0]==0x51475235u && cap_header[2]==sizeof(NvGpuState) && cap_header[3]==sizeof(NvGpuTri));fclose(capture);
 unsetenv("RECOMP_GPU_CAPTURE");puts("PASS: F8 bounded capture records exact input/state and completes after two frames");
 /* Valid compiler cache, source mismatch, truncated/corrupt payload. */
 const char *cache="shader-fixture.dxbc";ID3DBlob blob={nv_gpu_shader_source,sizeof(nv_gpu_shader_source)-1};
 uint64_t key=gpu_compile_key(nv_gpu_shader_source,sizeof(nv_gpu_shader_source)-1,"main","cs_5_0",0);gpu_shader_save(cache,key,&blob);
 assert(gpu_shader_cached(cache,key));assert(!gpu_shader_cached(cache,key^1));
 assert(key==gpu_compile_key(nv_gpu_shader_source,sizeof(nv_gpu_shader_source)-1,"main","cs_5_0",0));
 assert(key!=gpu_compile_key(nv_gpu_shader_source,sizeof(nv_gpu_shader_source)-1,"changed_entry","cs_5_0",0));
 assert(key!=gpu_compile_key(nv_gpu_shader_source,sizeof(nv_gpu_shader_source)-1,"main","ps_5_0",0));
 assert(key!=gpu_compile_key(nv_gpu_shader_source,sizeof(nv_gpu_shader_source)-1,"main","cs_5_0",1));
 char *changed_source=malloc(sizeof(nv_gpu_shader_source));assert(changed_source);memcpy(changed_source,nv_gpu_shader_source,sizeof(nv_gpu_shader_source));changed_source[0]^=1;
 assert(key!=gpu_compile_key(changed_source,sizeof(nv_gpu_shader_source)-1,"main","cs_5_0",0));free(changed_source);
 puts("PASS: shader cache rejects changed source/shared includes, entry point, profile, macro variant, wrong header and truncated payload");
 FILE*f=fopen(cache,"wb");assert(f);fputs("bad",f);fclose(f);assert(!gpu_shader_cached(cache,key));remove(cache);
 puts("PASS: 60 changing states/two texture slots merge into one dispatch; pinned same-address rewrite preserves old pixels; compiler-cache guards");
 if(native_mode){assert(native_draws>0 || native_compile_fail || native_create_fail || raster_create_fail);assert(g.native==(!native_corrupt && !native_stale && !native_arithmetic && !native_compile_fail && !native_create_fail && !raster_create_fail && !native_simple_compile_fail && !native_simple_create_fail && !native_simple_corrupt && !native_simple_large_corrupt));if(g.native && strcmp(argv[1],"native-simple-disabled") && strcmp(argv[1],"coverage-general"))assert(g.simple_pixel_shader && native_simple_draws);if(early_compile_fail || early_simple_compile_fail || (argc>1 && (!strcmp(argv[1],"native-early-off") || !strcmp(argv[1],"native-early-default"))))assert(!g.early_depth && g.native);puts("PASS: actual graphics API path/ordered candidate coverage and native self-test/compiler/create/target failure fallback");}
 if(g.native){
  int wanted=!uav_only_corrupt && !(argc>1 && !strcmp(argv[1],"native-uav-disabled"));assert(g.uav_only==wanted);
  if(g.uav_only)assert(!g.raster_view && !g.raster_target && uav_only_draws);
  assert(!g.color.buffer && !g.depth.buffer && g.surface.buffer);
  puts("PASS: native output uses only paired surface storage; UAV-only activation/fallback retains existing pixel shaders");
 }
 if(index_create_fail || native_batch_corrupt || (argc>1 && !strcmp(argv[1],"native-batch-disabled")))assert(g.native && !g.native_batch);
 if(coverage_mode){int enabled=strcmp(argv[1],"coverage-disabled") && coverage_tier>=2 && !coverage_query_fail && !coverage_state_fail && !coverage_compile_fail && !coverage_create_fail && !coverage_corrupt && !coverage_thin_corrupt;
  assert(g.coverage==enabled);if(enabled){
   assert(coverage_draws && coverage_thin_draws);
   uint64_t key=gpu_compile_key(nv_gpu_coverage_source,sizeof(nv_gpu_coverage_source)-1,"vertex_main","vs_5_0",4u|(g.array_registers?1u:0u));
   char path[128];snprintf(path,sizeof path,"shader-cache/qpr2-coverage-vs-%016llx.dxbc",(unsigned long long)key);unsigned calls=compiler_calls;ID3D11VertexShader *rectangle=g.vertex_shader;
   assert(gpu_rov_code(2,0,path,key) && compiler_calls==calls && g.coverage_vertex_shader==&coverage_vertex_shader && g.vertex_shader==rectangle);
   FILE *cached=fopen(path,"rb");uint64_t h[4];assert(cached && fread(h,sizeof h,1,cached)==1 && h[2]==3);fclose(cached);
  }
  puts("PASS: conservative coverage, viewport scissor, thin cases, separate vertex cache and independent fallback");
 }
 if(g.native && g.simple_pixel_shader){
  uint64_t key=gpu_compile_key(nv_gpu_rov_source,sizeof(nv_gpu_rov_source)-1,"pixel_main","ps_5_0",gpu_native_variant(2u|(g.array_registers?1u:0u)));
  char path[128];snprintf(path,sizeof path,"shader-cache/qpr2-rov-simple-%016llx.dxbc",(unsigned long long)key);
  unsigned calls=compiler_calls;ID3D11PixelShader *general=g.pixel_shader;
  assert(gpu_rov_code(0,1,path,key) && compiler_calls==calls && g.simple_pixel_shader==&simple_pixel_shader && g.pixel_shader==general);
  FILE *cached=fopen(path,"r+b");uint64_t header[4];assert(cached && fread(header,sizeof header,1,cached)==1 && header[2]==2);
  header[2]=0;rewind(cached);assert(fwrite(header,sizeof header,1,cached)==1);fclose(cached);
  assert(gpu_rov_code(0,1,path,key) && compiler_calls==calls+1 && g.pixel_shader==general);
  puts("PASS: dedicated native shader cache reuses its own program, rejects general-stage headers and preserves general shader binding");
 }
 /* Alternating real target sizes must reuse allocations after warmup. */
 {
  uint32_t pixels[48*48];float depths[48*48];NvGpuTextureSource no_textures[4]={0};NvGpuState ps={0};ps.bpp=4;ps.format=0x128;ps.color_mask=0x01010101;
  ps.depth_available=ps.depth_test=ps.depth_mask=ps.zpass_enable=1;ps.depth_func=0x203;
  ps.pitch=0;unsigned sizes[4]={16,24,32,40};uint64_t allocations=0,hits=0;unsigned rtvs=0;
  int pool_disabled=argc>1 && !strcmp(argv[1],"native-pool-disabled");
  for(unsigned iteration=0;iteration<16;iteration++){
   unsigned side=sizes[iteration%4];ps.width=ps.height=side;ps.pitch=side*4;NvGpuTri pt=triangle(side);
   for(unsigned at=0;at<side*side;at++){pixels[at]=0xFF112233u+iteration;depths[at]=100;}
   if(iteration==4){allocations=g.target_creates;hits=g.target_pool_hits;rtvs=raster_creates;}
   assert(nv_gpu_draw(&ps,&pt,1,(uint8_t*)pixels,depths,side,no_textures)==1 && nv_gpu_sync(&count)==1);
   for(unsigned y=0;y<side;y++)for(unsigned x=0;x<side;x++){
    NvGpuPixel expected=gpu_pixel(ps,pt,x,y,0xFF112233u+iteration,100);
    assert(pixels[y*side+x]==expected.color && depths[y*side+x]==expected.depth);
   }
  }
  if(!pool_disabled)assert(g.target_creates==allocations && g.target_pool_hits==hits+12);
  else assert(g.target_creates==allocations+12 && g.target_pool_hits==hits);
  if(g.native && g.uav_only)assert(raster_creates==rtvs);
  size_t bytes=g.color.bytes+g.depth.bytes+g.surface.bytes;
  for(unsigned i=0;i<3;i++)bytes+=g.target_spare[i].color.bytes+g.target_spare[i].depth.bytes+g.target_spare[i].surface.bytes;
  assert(bytes*2<=64u*1024u*1024u);
  for(unsigned at=0;at<48*48;at++){pixels[at]=0xFF123456;depths[at]=100;}
  ps.width=ps.height=48;ps.pitch=48*4;NvGpuTri pt=triangle(48);target_create_fail_bytes=48*48*(g.native?8:4);
  assert(!nv_gpu_draw(&ps,&pt,1,(uint8_t*)pixels,depths,48,no_textures) && !g.active);
  target_create_fail_bytes=0;
  assert(nv_gpu_draw(&ps,&pt,1,(uint8_t*)pixels,depths,48,no_textures)==1 && nv_gpu_sync(&count)==1);
  puts("PASS: four alternating target sizes reuse allocations with exact color/depth; UAV-only needs zero RTV allocations; bounded cache and allocation-failure recovery");
 }
 uint64_t reason_total=0;for(unsigned i=0;i<20;i++)reason_total+=g.sync_reason[i].count;assert(reason_total==g.readbacks);
 nv_gpu_report();puts("PASS: actual Win32 GPU backend resource creation/layout/boxed updates, real offscreen selftest, deferred readback, target-to-texture coherence, cache reuse and same-address edits");
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-gpu-device-') as d:
    p=Path(d)
    (p/'logs').mkdir()
    for name in ['windows.h','d3d11.h','d3d11_3.h','d3dcompiler.h','dxgi.h']:
        (p/name).write_text('/* Fixture API is declared above. */\n')
    src=p/'fixture.c';exe=p/'fixture'
    src.write_text(api+'ptrdiff_t xbox_GetMemoryOffset(void){return 0;}\n#include "'+str(root/'third_party/xboxrecomp/src/kernel/nv2a_gpu_raster.c')+'"\n'+fixture)
    subprocess.run(['gcc','-std=gnu11','-O1','-Werror=implicit-function-declaration',
                    '-Werror=incompatible-pointer-types','-fsanitize=undefined,float-cast-overflow',
                    '-fno-sanitize-recover=all','-I'+str(p),'-I'+str(root/'tools'),'-I'+str(root/'third_party/xboxrecomp/src/kernel'),
                    str(src),str(root/'third_party/xboxrecomp/src/kernel/nv2a_vsh_interp.c'),str(root/'third_party/xboxrecomp/src/kernel/nv2a_gpu_tiles.c'),'-pthread','-lm','-o',str(exe)],check=True)
    mode_count=0
    import sys
    modes=[[],['device'],['compile'],['shader'],['async'],['thread'],['native'],['native-array'],['native-bad'],['native-order'],['native-math'],['native-compile'],['native-create'],['native-target'],['native-simple-compile'],['native-simple-create'],['native-simple-bad'],['native-simple-large-bad'],['native-simple-readback'],['native-simple-disabled'],['native-early-off'],['native-early-default'],['native-early-compile'],['native-early-simple-compile'],['simple-compile'],['simple-bad'],['legacy-111'],['legacy-11'],['fixed-compile'],['fixed-bad'],['fixed-disabled'],['report-compile'],['report-bad'],['report-disabled'],['coverage'],['coverage-array'],['coverage-tier1'],['coverage-tier2'],['coverage-unsupported'],['coverage-query'],['coverage-state'],['coverage-compile'],['coverage-create'],['coverage-bad'],['coverage-thin-bad'],['coverage-disabled'],['coverage-general'],['native-batch-bad'],['native-batch-disabled'],['coverage-batch-bad'],['native-index-fail'],['coverage-index-fail'],['native-clear-compile'],['native-clear-create'],['native-clear-bad'],['native-clear-disabled'],['native-clear-readback'],['native-uav-disabled'],['native-uav-bad'],['native-uav-readback'],['native-pool-disabled'],['native-disjoint-disabled'],['native-quiet'],['quiet-compute'],['native-quiet-zpass-bad'],['native-restored-defaults']]
    if len(sys.argv)>1:modes=[m for m in modes if (m[0] if m else 'compute') in sys.argv[1:]]
    assert modes
    for mode in modes:
        print('RUN host/device '+(mode[0] if mode else 'compute'),flush=True)
        mode_count+=1
        cap=p/'capture.bin'
        if cap.exists():cap.unlink()
        clear_cap=p/'clear-capture.bin'
        if clear_cap.exists():clear_cap.unlink()
        for failure in p.glob('capture.bin.selftest-*.bin'):failure.unlink()
        for cache in (p/'shader-cache').glob('*'):cache.unlink()
        for cache in p.glob('capture.bin.failed-native-*.dxbc-cache'):cache.unlink()
        subprocess.run([str(exe),*mode],check=True,timeout=60,cwd=d)
        if cap.exists():subprocess.run(['python3',str(root/'tools/replay_qpr2_gpu_capture.py'),str(cap)],check=True,timeout=60,cwd=d)
        if clear_cap.exists():subprocess.run(['python3',str(root/'tools/replay_qpr2_gpu_capture.py'),str(clear_cap)],check=True,timeout=60,cwd=d)
        failures=list(p.glob('capture.bin.selftest-*.bin'))
        if mode in [['shader'],['native-bad'],['simple-bad'],['fixed-bad'],['native-order'],['native-math'],['native-simple-bad'],['native-simple-large-bad'],['coverage-bad'],['coverage-thin-bad'],['native-batch-bad'],['coverage-batch-bad']]:assert failures
        if mode in [['native-bad'],['native-order'],['native-math']]:
            assert (p/'capture.bin.failed-native-vs.dxbc-cache').is_file()
            assert (p/'capture.bin.failed-native-ps.dxbc-cache').is_file()
        if mode in [['native-simple-bad'],['native-simple-large-bad']]:
            assert (p/'capture.bin.failed-native-simple-ps.dxbc-cache').is_file()
        if mode in [["coverage-bad"],["coverage-thin-bad"]]:assert (p/"capture.bin.failed-coverage-vs.dxbc-cache").is_file()
        for failure in failures:
            result=subprocess.run(['python3',str(root/'tools/replay_qpr2_gpu_capture.py'),str(failure)],timeout=60,cwd=d,capture_output=True,text=True)
            print(result.stdout,end='');print(result.stderr,end='')
            if mode==['native-order']:assert '1 nonsequential inputs, 0 arithmetic differences' in result.stdout
            if mode==['native-math']:assert '0 nonsequential inputs, 1 arithmetic differences' in result.stdout
            assert result.returncode==(0 if mode==['native-quiet-zpass-bad'] else 1),'counter-only fault has matching color/depth; other private startup failures must replay as output mismatches'

    print(f"PASS: all {mode_count} host/device modes completed; injected failures rejected as expected")
