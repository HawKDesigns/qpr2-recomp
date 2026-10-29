"""Candidate hashing: exact tails, mutation coverage, forced collision queues.
Synthetic hashing speed is not game FPS; full-byte equality remains mandatory.
"""
from pathlib import Path
import subprocess,tempfile,os,json
import test_qpr2_gpu_geometry as geo
r=geo.root;k=geo.k
ns={'__file__':str(r/'tools/test_qpr2_stream_union.py')}
s=(r/'tools/test_qpr2_stream_union.py').read_text();exec(compile(s[:s.index('with tempfile.TemporaryDirectory(')],ns['__file__'],'exec'),ns)
helpers=ns['helpers']+r'''
static void hash_benchmark(NvRawInput *in){
 NvRawStreamPlan plan;assert(gpu_raw_stream_plan(in,&plan,raw.stream_union));
 unsigned saved=raw.word_hash;volatile uint64_t sink=0;double times[2][5];
 /* Same key payload and iteration count; alternate order to reduce bias. */
 for(unsigned run=0;run<5;run++)for(unsigned order=0;order<2;order++){
  unsigned variant=order^(run&1);raw.word_hash=variant;double start=gpu_ms();
  for(unsigned n=0;n<20000;n++)sink^=gpu_raw_key(in,&plan);
  times[variant][run]=gpu_ms()-start;
 }
 raw.word_hash=saved;
 for(unsigned v=0;v<2;v++)for(unsigned a=0;a<5;a++)for(unsigned b=a+1;b<5;b++)if(times[v][b]<times[v][a]){double t=times[v][a];times[v][a]=times[v][b];times[v][b]=t;}
 printf("BENCH key_lookup iterations=20000 logical_bytes=%llu unique_bytes=%llu legacy_median_ms=%.6f word_median_ms=%.6f ratio=%.3f sink=%llu\n",(unsigned long long)plan.logical,(unsigned long long)plan.unique,times[0][2],times[1][2],times[0][2]/times[1][2],(unsigned long long)sink);
 double lookup[2][5];
 for(unsigned run=0;run<5;run++)for(unsigned order=0;order<2;order++){
  unsigned variant=order^(run&1);union_clear();raw.word_hash=variant;
  NvRawMesh *expected=gpu_raw_mesh(in);assert(expected);uint64_t misses=raw.misses;
  double start=gpu_ms();for(unsigned n=0;n<20000;n++)assert(gpu_raw_mesh(in)==expected);
  lookup[variant][run]=gpu_ms()-start;assert(raw.misses==misses);
 }
 union_clear();raw.word_hash=saved;
 for(unsigned v=0;v<2;v++)for(unsigned a=0;a<5;a++)for(unsigned b=a+1;b<5;b++)if(lookup[v][b]<lookup[v][a]){double t=lookup[v][a];lookup[v][a]=lookup[v][b];lookup[v][b]=t;}
 printf("BENCH warm_mesh_lookup iterations=20000 legacy_median_ms=%.6f word_median_ms=%.6f ratio=%.3f forced_collision=%d\n",lookup[0][2],lookup[1][2],lookup[0][2]/lookup[1][2],hash_force_collision);
}
'''
main=ns['main'].replace(' state.zpass_enable=1;union_clear();',' state.zpass_enable=1;union_clear();hash_force_collision=!strcmp(mode,"collision");',1)
main=main.replace(' assert(nv_gpu_available());',' setenv("RECOMP_GPU_MESH_WORD_HASH",!strcmp(mode,"oldhash")?"0":"1",1);\n assert(nv_gpu_available());',1)
main=main.replace(' nv_gpu_report();return 0;',' hash_benchmark(&input);\n nv_gpu_report();return 0;',1)
api=ns['api']+'\nstatic int hash_force_collision;\n'
impl=(k/'nv2a_gpu_geometry_impl.inc').read_text();needle='uint64_t key=gpu_raw_key(in,&plan);';assert impl.count(needle)==1
# Test-only fault injection: every complete mesh key collides. Production
# equality, cache buckets, pinned versions and GPU command paths remain real.
impl=impl.replace(needle,'uint64_t key=hash_force_collision?7:gpu_raw_key(in,&plan);',1)
pure=r'''
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include "nv2a_gpu_mesh_word_hash.inc"
/* Independently assemble native words byte by byte; little-endian Windows
 * target. No production memcpy or tail loop is used for the reference. */
static uint64_t reference(uint64_t h,const uint8_t *p,size_t n){
 if(!n)return h;h^=(uint64_t)n*0x9e3779b185ebca87ull;size_t at=0;
 while(n-at>=8){uint64_t v=0;for(unsigned j=0;j<8;j++)v|=(uint64_t)p[at+j]<<(8*j);h^=v;h*=0x9e3779b185ebca87ull;h=(h<<27)|(h>>37);at+=8;}
 if(at<n){uint64_t v=0;for(size_t j=at;j<n;j++)v|=(uint64_t)p[j]<<(8*(j-at));h^=v;h*=0x9e3779b185ebca87ull;}
 h^=h>>30;h*=0xbf58476d1ce4e5b9ull;h^=h>>27;h*=0x94d049bb133111ebull;return h^(h>>31);
}
int main(void){
 uint16_t endian=1;assert(*(uint8_t*)&endian==1);assert(gpu_raw_word_hash(123,NULL,0)==123);
 uint8_t data[4096];for(unsigned i=0;i<sizeof data;i++)data[i]=(uint8_t)(i*137u+i/17u);
 for(unsigned n=0;n<=2048;n++)for(unsigned off=0;off<16;off++)assert(gpu_raw_word_hash(0x145679023ull+n,data+off,n)==reference(0x145679023ull+n,data+off,n));
 size_t page=(size_t)sysconf(_SC_PAGESIZE);uint8_t *guard=mmap(NULL,page*2,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);assert(guard!=MAP_FAILED && !mprotect(guard+page,page,PROT_NONE));
 for(unsigned n=1;n<=2048;n++){uint8_t *p=guard+page-n;memcpy(p,data,n);assert(gpu_raw_word_hash(n,p,n)==reference(n,p,n));}
 assert(!munmap(guard,page*2));
 for(unsigned n=1;n<257;n++){uint64_t base=gpu_raw_word_hash(1,data,n);for(unsigned j=0;j<n;j++){data[j]^=0x80;assert(gpu_raw_word_hash(1,data,n)!=base);data[j]^=0x80;}}
 unsigned buckets[65536]={0},used=0,peak=0;uint64_t payload[8]={0};
 for(unsigned i=0;i<65536;i++){payload[5]=(uint64_t)i<<32;unsigned b=(unsigned)gpu_raw_word_hash(1,payload,sizeof payload)&65535u;++buckets[b];}
 for(unsigned i=0;i<65536;i++){used+=buckets[i]!=0;if(buckets[i]>peak)peak=buckets[i];}assert(used>35000 && peak<16);
 printf("PASS: independent reference, all alignments/lengths, guarded tails, every-byte mutations, bucket spread used=%u peak=%u\n",used,peak);
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-mesh-word-hash-') as directory:
 p=Path(directory)
 (p/'pure.c').write_text(pure)
 subprocess.run(['gcc','-std=gnu11','-O2','-no-pie','-fsanitize=address,undefined','-fno-sanitize-recover=all','-I'+str(k),str(p/'pure.c'),'-o',str(p/'pure')],check=True)
 env=dict(os.environ);env['ASAN_OPTIONS']='detect_leaks=0';subprocess.run([str(p/'pure')],env=env,check=True)
 for name in ['windows.h','d3d11.h','d3d11_3.h','d3dcompiler.h','dxgi.h']:(p/name).write_text('/* strict API */\n')
 (p/'nv2a_gpu_raster.c').write_bytes((k/'nv2a_gpu_raster.c').read_bytes());(p/'nv2a_gpu_geometry_impl.inc').write_text(impl)
 (p/'test.c').write_text(api+'ptrdiff_t xbox_GetMemoryOffset(void){return 0;}\n#define calloc union_test_calloc\n#include "nv2a_gpu_raster.c"\n#undef calloc\n'+ns['fixture']+helpers+main)
 subprocess.run(['gcc','-std=gnu11','-O2','-ffp-contract=off','-Werror=implicit-function-declaration','-Werror=incompatible-pointer-types','-I'+str(p),'-I'+str(r/'tools'),'-I'+str(k),str(p/'test.c'),str(k/'nv2a_vsh_interp.c'),str(k/'nv2a_gpu_tiles.c'),'-pthread','-lm','-o',str(p/'test')],check=True)
 for mode in ['merged','oldhash','legacy','collision']:
  print('RUN hash '+mode,flush=True);cwd=p/mode;cwd.mkdir();subprocess.run([str(p/'test'),mode],cwd=cwd,check=True,timeout=180)
print('PASS: four hash/cache modes, forced complete-key collisions, independent pixels/depth/visibility and delayed publication')
