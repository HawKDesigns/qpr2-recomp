"""Execute the production native store/fence block in ordered scalar scenes.

Proves bit-exact publication and semantic counters, not GPU synchronization or
FPS. Real hardware admission also runs exact writer/read-only/writer sequences.
"""
from pathlib import Path
import subprocess,tempfile
r=Path(__file__).resolve().parents[1];k=r/'third_party/xboxrecomp/src/kernel'
p=r/'tools/test_qpr2_depth_seed.py';ns={'__file__':str(p)};s=p.read_text();exec(compile(s[:s.index('with tempfile.TemporaryDirectory(')],str(p),'exec'),ns)
source=ns['source'];shader=(k/'nv2a_gpu_rov.hlsl').read_text()
a=shader.index('    uint2 before=surface_buffer[at];');b=shader.index('    if(state.width==32',a);block=shader[a:b]
assert block.count('surface_buffer[at]=')==1 and block.count('DeviceMemoryBarrier();')==1
assert '[branch] if(after.x!=before.x || after.y!=before.y)' in block
assert shader.index('if(p.zpass)InterlockedAdd',b)>b # No return skips semantic visibility.
block=block.replace('uint2(p.color,asuint(p.depth))','(uint2){p.color,asuint(p.depth)}').replace('[branch] ','')
helpers='''
typedef struct {uint32_t x,y;} uint2;
static uint2 surface_buffer[1];static unsigned fences;
#define asuint GPU_ASUINT
#define asfloat GPU_ASFLOAT
#define DeviceMemoryBarrier() (++fences)
'''
for name,on in [('skip_store',1),('legacy_store',0)]:
 helpers+=f'\n#define NV_GPU_SKIP_UNCHANGED {on}\n'
 helpers+='static NvGpuPixel '+name+'(NvGpuState state,NvGpuBary bary,NvGpuSource source,uint32_t color,float depth){\nint at=0;surface_buffer[0]=(uint2){color,GPU_ASUINT(depth)};fences=0;\n'+block+'return p;\n}\n#undef NV_GPU_SKIP_UNCHANGED\n'
helpers+='''
static void bit_edges(void){
 NvGpuState s={0};s.depth_available=s.depth_mask=s.zpass_enable=1;
 NvGpuBary b={0};b.covered=1;NvGpuSource src={0};src.keep=1;
 uint32_t bits[]={0,0x80000000u,0x7f800000u,0xff800000u,0x7fc12345u,0x7fc54321u};
 for(unsigned x=0;x<6;x++)for(unsigned y=0;y<6;y++){
  b.depth=GPU_ASFLOAT(bits[x]);float before=GPU_ASFLOAT(bits[y]);
  NvGpuPixel p=skip_store(s,b,src,0,before);
  assert(fences==(bits[x]!=bits[y]));assert(surface_buffer[0].y==bits[x] && p.zpass==1);
 }
 puts("PASS: signed zero, infinities and distinct NaN payloads compare by storage bits; unchanged zpass retained");
}
'''
source=source.replace('static uint32_t seed=0x94171513u;',helpers+'static uint32_t seed=0x10171513u;')
source=source.replace('int main(void){','int main(void){bit_edges();',1)
source=source.replace('unsigned skipped=0,checks=0,safe_batches=0,unsafe_batches=0;', 'unsigned skipped=0,checks=0,safe_batches=0,unsafe_batches=0,writes=0,readonly=0,readonly_zpass=0,quiet_eligible_readonly=0;')
a='    color[at]=old.color;z[at]=old.depth;actual[at]=out.color;az[at]=out.depth;++checks;'
b='''    int changed=old.color!=color[at] || GPU_ASUINT(old.depth)!=GPU_ASUINT(z[at]);
    NvGpuPixel got=skip_store(*state,bary,source,color[at],z[at]);
    assert(!memcmp(&got,&old,sizeof got));assert(fences==(unsigned)changed);
    assert(surface_buffer[0].x==old.color && surface_buffer[0].y==GPU_ASUINT(old.depth));
    if(changed)++writes;else {++readonly;if(old.zpass)++readonly_zpass;if(shade && source.keep)++quiet_eligible_readonly;}
    got=legacy_store(*state,bary,source,color[at],z[at]);assert(fences==1 && !memcmp(&got,&old,sizeof got));
    assert(surface_buffer[0].x==old.color && surface_buffer[0].y==GPU_ASUINT(old.depth));
'''+a
assert a in source;source=source.replace(a,b,1)
a=' assert(checks==4194304 && skipped>10000 && safe_batches && unsafe_batches);';assert a in source
source=source.replace(a,a+'\n assert(writes>10000 && readonly>10000 && readonly_zpass>1000 && quiet_eligible_readonly>1000);\n printf("PASS: %u production store-block comparisons; changed=%u unchanged=%u unchanged_zpass=%u quiet_eligible_unchanged=%u; exact color/depth/all counters, every writer fenced, legacy opt-out exact\\n",checks,writes,readonly,readonly_zpass,quiet_eligible_readonly);')
with tempfile.TemporaryDirectory(prefix='qpr2-skip-store-') as directory:
 p=Path(directory);(p/'test.c').write_text(source)
 subprocess.run(['gcc','-std=c11','-O2','-ffp-contract=off','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-I'+str(k),str(p/'test.c'),'-lm','-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True,timeout=180)
