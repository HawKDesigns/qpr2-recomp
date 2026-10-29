"""Compile the production HLSL pre-ROV control flow as scalar C.

Compare exact guest color/depth/visibility over mixed ordered scenes, with the
feature on/off and full diagnostics. This is a correctness proof, not GPU FPS.
"""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1];k=root/'third_party/xboxrecomp/src/kernel'
path=root/'tools/test_qpr2_depth_seed.py';ns={'__file__':str(path)};s=path.read_text()
exec(compile(s[:s.index('with tempfile.TemporaryDirectory(')],str(path),'exec'),ns)
source=ns['source']
shader=(k/'nv2a_gpu_rov.hlsl').read_text()
a=shader.index('    int x=(int)input.position.x',shader.index('float4 pixel_main('))
b=shader.index('    /* Surviving viewport candidates',a)
prefix=shader[a:b]
assert prefix.index('if(!shade)return 0;')<prefix.index('source=gpu_source_color')<prefix.index('if(!source.keep)return 0;')
assert 'surface_buffer' not in prefix and 'DeviceMemoryBarrier' not in prefix
# Surviving invocations retain the exact previously validated transaction.
frozen=root/'tools/fixtures/v94-ordered-transaction.hlsl'
tail=shader[shader.index('    uint2 before=surface_buffer[at];'):]
# V101 changes only the store/fence block, separately executed by its proof.
store_block="    uint2 after=uint2(p.color,asuint(p.depth));\n#if NV_GPU_SKIP_UNCHANGED\n    /* Compare storage bits, including signed zero and NaN payloads. A read-only\n     * invocation has nothing to publish; every actual writer keeps V54's fence.\n     * Do not return here: unchanged output can still count as guest zpass. */\n    [branch] if(after.x!=before.x || after.y!=before.y)\n#endif\n    {\n        surface_buffer[at]=after;\n        /* Explicit visibility is still required after EVERY surface write. */\n        DeviceMemoryBarrier();\n    }\n"
legacy_block="    surface_buffer[at]=uint2(p.color,asuint(p.depth));\n    /* Explicit device-wide visibility for the next ordered pixel invocation.\n     * V54's trace observed stale input despite otherwise correct ROV ordering\n     * declarations. Keep this non-group-synchronizing fence unconditional. */\n    DeviceMemoryBarrier();\n"
assert tail.count(store_block)==1
assert tail.replace(store_block,legacy_block)==frozen.read_text()
helpers='''
typedef uint32_t uint;
typedef struct {float x,y;} Position;
typedef struct {Position position;uint primitive;} RasterInput;
typedef struct {uint x,y;} SeedPixel;
#define asfloat GPU_ASFLOAT
#define NV_GPU_DEPTH_SEED 1
'''
for name,enabled,diagnostics in [('reject_quiet',1,0),('keep_diagnostic',1,1),('disabled_quiet',0,0)]:
 helpers+=f'\n#define NV_GPU_EMPTY_REJECT {enabled}\n#define NV_GPU_DIAGNOSTIC_COUNTERS {diagnostics}\n'
 helpers+='static int '+name+'(NvGpuSource *output,const NvGpuState *configuration,const NvGpuTri *triangles,const SeedPixel *batch_depth_seed,const uint *depth_seed_control,RasterInput input){\n'+prefix+'    *output=source;return 1;\n}\n'
 helpers+='#undef NV_GPU_EMPTY_REJECT\n#undef NV_GPU_DIAGNOSTIC_COUNTERS\n'
source=source.replace('static uint32_t seed=0x94171513u;',helpers+'static uint32_t seed=0x95171513u;')
source=source.replace('unsigned skipped=0,checks=0,safe_batches=0,unsafe_batches=0;', 'unsigned skipped=0,checks=0,safe_batches=0,unsafe_batches=0,empty_before=0,empty_alpha=0,prefix_checks=0;')
a='  for(unsigned draw=0;draw<16;draw++){'
b='''  SeedPixel seeds[64];for(unsigned at=0;at<64;at++){seeds[at].x=0;memcpy(&seeds[at].y,&frozen[at],4);}
  uint control[1]={(uint)enabled};
'''+a
assert a in source;source=source.replace(a,b,1)
a='    color[at]=old.color;z[at]=old.depth;actual[at]=out.color;az[at]=out.depth;++checks;'
b='''    for(unsigned variant=0;variant<3;variant++){
     NvGpuSource src={0};RasterInput input={{(float)x+0.5f,(float)y+0.5f},0};
     int commit=variant==0?reject_quiet(&src,state,&t,seeds,control,input):variant==1?keep_diagnostic(&src,state,&t,seeds,control,input):disabled_quiet(&src,state,&t,seeds,control,input);
     NvGpuPixel p={0};p.color=color[at];p.depth=z[at];
     if(commit)p=gpu_commit(*state,bary,src,color[at],z[at]);
     else {assert(!variant);if(!shade)++empty_before;else ++empty_alpha;}
     NvGpuPixel reference=old;if(variant!=1){reference.depth_fail=reference.pixels=reference.pixel_max=0;p.depth_fail=p.pixels=p.pixel_max=0;}
     assert(!memcmp(&p,&reference,sizeof p));++prefix_checks;
    }
'''+a
assert a in source;source=source.replace(a,b,1)
a=' assert(checks==4194304 && skipped>10000 && safe_batches && unsafe_batches);'
b=a+'\n assert(prefix_checks==12582912 && empty_before>10000 && empty_alpha>10000);\n printf("PASS: %u production-prefix comparisons; %u coverage/hidden and %u alpha/clip transactions omitted; diagnostics/opt-out exact; pre-store math, statistics and writer barrier retained\\n",prefix_checks,empty_before,empty_alpha);'
assert a in source;source=source.replace(a,b,1)
with tempfile.TemporaryDirectory(prefix='qpr2-empty-reject-proof-') as folder:
 p=Path(folder);(p/'test.c').write_text(source)
 subprocess.run(['gcc','-std=c11','-O2','-ffp-contract=off','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-I'+str(k),str(p/'test.c'),'-lm','-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True,timeout=180)
