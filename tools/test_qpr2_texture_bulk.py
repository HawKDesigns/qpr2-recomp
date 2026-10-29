"""Compare production whole-image conversion against per-texel sample_tex.
Also exercises renderer cache hits, rewritten sources, opt-out and fallback.
No GPU performance is measured by this API shim.
"""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1]
base=root/'tools/test_qpr2_texture_bridge.py'
s=base.read_text();prefix,runner=s.split('\nwith tempfile.TemporaryDirectory(',1)
ns={'__file__':str(base)};exec(compile(prefix,str(base),'exec'),ns)
ns['fixture']=r'''
static unsigned texel_calls,bulk_calls;
static int count_texel(void *o,uint32_t f,uint32_t x,uint32_t y,uint32_t *c){++texel_calls;return gpu_texture_decode(o,f,x,y,c);}
static int count_bulk(void *o,const uint8_t *b,size_t n,uint32_t *p,size_t c){++bulk_calls;return gpu_texture_decode_image(o,b,n,p,c);}
int main(int argc,char **argv){
 ram=mmap(NULL,0x84000000ull,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS|MAP_NORESERVE,-1,0);assert(ram!=MAP_FAILED);g_memory_offset=(ptrdiff_t)ram;
 unsigned rng=1234567,images=0;uint64_t pixels=0;
 const unsigned formats[]={0x06,0x12,0x0C,0x0E,0x0F};
 const unsigned sizes[][2]={{1,1},{2,2},{4,8},{8,4},{16,16},{32,128},{256,64},{64,256},{512,512},{3,7},{17,5}};
 for(unsigned fmt=0;fmt<5;fmt++)for(unsigned size=0;size<11;size++)for(unsigned cube=0;cube<2;cube++)for(unsigned mip=1;mip<=3;mip+=2){
  Texture t={0};t.offset=0x100000;t.width=sizes[size][0];t.height=sizes[size][1];t.color=formats[fmt];t.pitch=t.width*4+(fmt==1?12:0);t.valid=1;t.addr_u=t.addr_v=1;t.cube=cube;t.levels=mip;
  if(fmt==0 && ((t.width&(t.width-1)) || (t.height&(t.height-1))))continue;
  size_t stride=tex_face_stride(&t),bytes=stride*(cube?6:1),n=(size_t)t.width*t.height*(cube?6:1);
  uint8_t *snapshot=malloc(bytes+1);uint32_t *out=malloc((n+2)*4);assert(snapshot && out);
  for(size_t i=0;i<bytes;i++){rng=rng*1664525u+1013904223u;ram[t.offset+i]=snapshot[1+i]=(uint8_t)(rng>>24);}
  out[0]=out[n+1]=0xBADCAFEu;
  assert(gpu_texture_decode_image(&t,snapshot+1,bytes,out+1,n));
  for(unsigned f=0;f<(cube?6u:1u);f++)for(unsigned y=0;y<t.height;y++)for(unsigned x=0;x<t.width;x++){
   uint32_t ref;assert(gpu_texture_decode(&t,f,x,y,&ref));assert(out[1+((size_t)f*t.height+y)*t.width+x]==ref);
  }
  assert(out[0]==0xBADCAFEu && out[n+1]==0xBADCAFEu);
  assert(!gpu_texture_decode_image(&t,snapshot+1,bytes,out+1,n-1));
  assert(!gpu_texture_decode_image(&t,snapshot+1,0,out+1,n));
  /* A copied source remains valid independently of later guest writes. */
  memset(ram+t.offset,0,bytes);uint32_t old=out[1];
  assert(gpu_texture_decode_image(&t,snapshot+1,bytes,out+1,n) && out[1]==old);
  free(snapshot);free(out);++images;pixels+=n;
 }
 printf("PASS: %u images / %llu texels match legacy sampler (all DXT types, ARGB, cube/mips, rectangular Morton, padded linear, partial blocks, unaligned input)\n",images,(unsigned long long)pixels);
 Texture t={0};t.offset=0x100000;t.width=t.height=16;t.pitch=64;t.color=0x12;t.valid=1;t.levels=1;
 uint32_t out[256];memset(out,0xDA,sizeof out);t.color=0x05;
 assert(!gpu_texture_decode_image(&t,ram+t.offset,1024,out,256) && out[0]==0xDADADADA);t.color=0x12;
 setenv("RECOMP_GPU_RASTER","1",1);setenv("RECOMP_GPU_ASYNC","0",1);setenv("RECOMP_GPU_DIAGNOSTIC_COUNTERS","0",1);
 int disabled=argc>1 && !strcmp(argv[1],"disabled");if(disabled)setenv("RECOMP_GPU_BULK_TEXTURE","0",1);
 native_supported=1;coverage_tier=3;assert(nv_gpu_available());assert(g.bulk_texture==!disabled);
 NvGpuTexture meta={0};meta.width=meta.height=16;meta.format=0x12;meta.valid=1;
 NvGpuTextureSource src={0};src.raw=ram+t.offset;src.bytes=1024;src.opaque=&t;src.decode=count_texel;src.decode_image=count_bulk;
 for(unsigned i=0;i<256;i++)memcpy(ram+t.offset+i*4,&i,4);
 ++g.tick;GpuTexture *slot=gpu_texture(&meta,&src);assert(slot && slot->decoded[125]==125);
 assert(bulk_calls==!disabled && texel_calls==(disabled?256u:0u));
 ++g.tick;assert(gpu_texture(&meta,&src)==slot && bulk_calls==!disabled && texel_calls==(disabled?256u:0u));
 uint32_t value=0xF0123456;memcpy(ram+t.offset+4,&value,4);++g.tick;
 slot=gpu_texture(&meta,&src);assert(slot && slot->decoded[1]==value);
 assert(bulk_calls==(disabled?0u:2u) && texel_calls==(disabled?512u:0u));
 /* Arbitrary test decoders remain authoritative when no bulk callback exists. */
 src.decode_image=NULL;memcpy(ram+t.offset+8,&value,4);++g.tick;
 slot=gpu_texture(&meta,&src);assert(slot && slot->decoded[2]==value && texel_calls==(disabled?768u:256u));
 nv_gpu_report();puts("PASS: real texture cache, source rewrite, scalar callback fallback and opt-out");return 0;
}
'''
runner=runner.replace("['default','quiet','rectangle','disabled','compile','create','corrupt','map']","['default','disabled']").replace('all 8 rendered-texture bridge modes','all 2 bulk texture integration modes')
exec(compile('\nwith tempfile.TemporaryDirectory('+runner,str(base),'exec'),ns)
