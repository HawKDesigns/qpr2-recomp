"""Bounded mixed CPU/raw upload ranges and real render/failure integration.
Strict API checks byte ranges/canaries. Scalar kernels execute shader commands;
this verifies correctness and call reduction, not Windows driver performance.
"""
from pathlib import Path
import subprocess,tempfile
base=Path(__file__).with_name('test_qpr2_raw_upload_stream.py');s=base.read_text();ns={'__file__':str(base)}
exec(compile(s[:s.index('\nwith tempfile.TemporaryDirectory(')],str(base),'exec'),ns)
root=ns['root'];k=ns['k'];api=ns['api'];fixture=ns['fixture'];main=ns['main']
api=api.replace('static void ID3D11DeviceContext_UpdateSubresource(', 'static void mixed_update_probe(ID3D11Resource*,const D3D11_BOX*);\nstatic void ID3D11DeviceContext_UpdateSubresource(',1)
needle=' input_update_probe(b);';assert needle in api;api=api.replace(needle,' mixed_update_probe(b,box);'+needle,1)
helpers=r'''
static unsigned property_checking,mixed_calls;static uint8_t written[NV_GPU_MAX_TRIANGLES];
static ID3D11Buffer *property_buffer;
static void mixed_update_probe(ID3D11Resource *b,const D3D11_BOX *box){
 if(b->desc.StructureByteStride!=sizeof(NvGpuTri))return;
 if(property_checking){
  assert(b==property_buffer && box && !box->top && !box->front && box->bottom==1 && box->back==1);
  assert(box->left%sizeof(NvGpuTri)==0 && box->right%sizeof(NvGpuTri)==0);
  unsigned first=box->left/sizeof(NvGpuTri),end=box->right/sizeof(NvGpuTri);
  assert(first<end && end<=g.queued_count && !raw.slots[first] && !raw.slots[end-1]);
  for(unsigned i=first;i<end;i++){assert(!written[i]);written[i]=1;}
 }else if(checking)++mixed_calls;
}
static uint32_t random_word(uint32_t *s){*s=*s*1664525u+1013904223u;return *s;}
static void range_properties(void){
 GpuBuffer private={0};assert(gpu_buffer_create(&private,NV_GPU_MAX_TRIANGLES*sizeof(NvGpuTri),sizeof(NvGpuTri),2));
 property_buffer=private.buffer;uint32_t seed=0x90abc123u;unsigned cases=0;
 for(unsigned policy=0;policy<2;policy++)for(unsigned test=0;test<256;test++){
  raw.upload_coalesce=policy;unsigned n=test<8?NV_GPU_MAX_TRIANGLES:1+random_word(&seed)%512;
  g.queued_count=n;unsigned cpu=0,runs=0;
  for(unsigned i=0;i<n;i++){
   raw.slots[i]=test==0?1:test==1?0:test==2?(i&1):test==3?((i%129)<65):test==4?(i>0 && i<n-1):((random_word(&seed)>>24)%3==0);
   if(!raw.slots[i]){++cpu;if(!i || raw.slots[i-1])++runs;}
   uint32_t *words=(uint32_t*)(g.queued+i);for(unsigned j=0;j<sizeof(NvGpuTri)/4;j++)words[j]=random_word(&seed);
  }
  memset(private.buffer->data,0xCD,private.bytes);memset(written,0,sizeof written);
  memset(&raw.mixed_upload,0,sizeof raw.mixed_upload);property_checking=1;
  assert(gpu_raw_upload(&private));property_checking=0;
  assert(raw.mixed_upload.batches==1 && raw.mixed_upload.runs==runs);
  assert(raw.mixed_upload.writes<=runs && raw.mixed_upload.cpu_bytes==(uint64_t)cpu*sizeof(NvGpuTri));
  assert(raw.mixed_upload.padding_bytes<=(uint64_t)(cpu<1024?cpu:1024)*sizeof(NvGpuTri));
  if(!policy)assert(raw.mixed_upload.writes==runs && !raw.mixed_upload.padding_bytes);
  if(test==2)assert(raw.mixed_upload.writes==(policy?3072u:4096u)); /* Global padding cap. */
  unsigned padded=0;
  for(unsigned i=0;i<NV_GPU_MAX_TRIANGLES;i++){
   const uint8_t *got=private.buffer->data+(size_t)i*sizeof(NvGpuTri);
   if(i<n && !raw.slots[i])assert(written[i] && !memcmp(got,g.queued+i,sizeof(NvGpuTri)));
   else if(written[i]){++padded;assert(i<n && !memcmp(got,g.queued+i,sizeof(NvGpuTri)));}
   else for(unsigned j=0;j<sizeof(NvGpuTri);j++)assert(got[j]==0xCD);
  }
  assert(raw.mixed_upload.padding_bytes==(uint64_t)padded*sizeof(NvGpuTri));++cases;
 }
 gpu_buffer_release(&private);g.queued_count=0;memset(raw.slots,0,sizeof raw.slots);memset(&raw.mixed_upload,0,sizeof raw.mixed_upload);
 printf("PASS: %u byte-exact range/canary cases; empty/all-CPU/alternating/large gaps/tails/random patterns, both policies; bounded padding\n",cases);
}
'''
# Helpers need the existing integration fixture's globals and callbacks first.
needle='int main(int argc,char **argv){';assert needle in main;main=main.replace(needle,helpers+'\n'+needle,1)
needle='native_supported=1;coverage_tier=3;assert(nv_gpu_available() && raw.ready);';assert needle in main
main=main.replace(needle,'''setenv("RECOMP_GPU_MIXED_UPLOAD_COALESCE",!strcmp(mode,"unmerged")?"0":"1",1);
 '''+needle+'''
 if(!strcmp(mode,"stream"))range_properties();raw.upload_coalesce=strcmp(mode,"unmerged")!=0;''',1)
a='''  if(batch%3==0){NvGpuTri cpu=triangle(32);for(unsigned v=0;v<3;v++)cpu.v[v].pos[2]=30;assert(nv_gpu_draw(&s,&cpu,1,(uint8_t*)color,z,32,sources)==1);reference_draw(s,cpu,expected,expected_z,&ref);}'''
b='''  NvGpuTri cpu[16];for(unsigned j=0;j<16;j++){cpu[j]=triangle(32);for(unsigned v=0;v<3;v++){cpu[j].v[v].pos[2]=30;cpu[j].v[v].d0[0]=.75f;cpu[j].v[v].d0[1]=.25f;cpu[j].v[v].d0[2]=.125f;cpu[j].v[v].d0[3]=.5f;}}
  assert(nv_gpu_draw(&s,cpu,16,(uint8_t*)color,z,32,sources)==1);
  for(unsigned j=0;j<16;j++)reference_draw(s,cpu[j],expected,expected_z,&ref);''';assert a in main;main=main.replace(a,b,1)
main=main.replace('s.depth_available=s.depth_test=s.depth_mask=s.zpass_enable=1;', 's.depth_available=s.depth_mask=s.zpass_enable=1;s.depth_test=0;',1)
needle='  int submitted=nv_gpu_submit();';assert needle in main
main=main.replace(needle,'''  for(unsigned j=0;j<16;j++)for(unsigned v=0;v<3;v++){cpu[j].v[v].d0[0]=.125f;cpu[j].v[v].d0[1]=.25f;cpu[j].v[v].d0[2]=.75f;}
  assert(nv_gpu_draw(&s,cpu,16,(uint8_t*)color,z,32,sources)==1);
  for(unsigned j=0;j<16;j++)reference_draw(s,cpu[j],expected,expected_z,&ref);
'''+needle,1)
needle=' nv_gpu_report();assert(';assert needle in main
main=main.replace(needle,''' assert(raw.mixed_upload.batches==32 && raw.mixed_upload.runs==64);
 assert(mixed_calls==(raw.upload_coalesce?32u:64u) && raw.mixed_upload.writes==mixed_calls);
 assert(raw.mixed_upload.padding_bytes==(raw.upload_coalesce?528u*sizeof(NvGpuTri):0u));
 nv_gpu_report();assert(!raw.mixed_upload.batches && !raw.mixed_upload.runs && !raw.mixed_upload.writes && !raw.mixed_upload.cpu_bytes && !raw.mixed_upload.padding_bytes && raw.mixed_upload.ms==0);assert(''',1)
with tempfile.TemporaryDirectory(prefix='qpr2-mixed-upload-') as folder:
 p=Path(folder);(p/'logs').mkdir()
 for name in ['windows.h','d3d11.h','d3d11_3.h','d3dcompiler.h','dxgi.h']:(p/name).write_text('/* strict API above */\n')
 (p/'fixture.c').write_text(api+'ptrdiff_t xbox_GetMemoryOffset(void){return 0;}\n#include "'+str(k/'nv2a_gpu_raster.c')+'"\n'+fixture+main)
 subprocess.run(['gcc','-std=gnu11','-O1','-ffp-contract=off','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-Werror=implicit-function-declaration','-Werror=incompatible-pointer-types','-I'+str(p),'-I'+str(root/'tools'),'-I'+str(k),str(p/'fixture.c'),str(k/'nv2a_vsh_interp.c'),str(k/'nv2a_gpu_tiles.c'),'-pthread','-lm','-o',str(p/'fixture')],check=True)
 for mode in ['stream','unmerged','legacy','no-cache','draw-fail','vertex-jobs-fail','prepare-jobs-fail','fallback-fail']:
  print('RUN mixed upload '+mode,flush=True);subprocess.run([str(p/'fixture'),mode],check=True,cwd=p,timeout=120)
 print('PASS: all eight mixed-upload modes; exact final pixels/depth/counts, bounded extra bytes and accepted-work failures')
