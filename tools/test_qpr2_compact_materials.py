"""Compact SSA bodies vs independent frozen evaluator, with V97 routing proof."""
from pathlib import Path
import subprocess,sys
root=Path(__file__).resolve().parents[1];k=root/'third_party/xboxrecomp/src/kernel'
prior=(k/'nv2a_gpu_compact.inc').read_bytes()
subprocess.run([sys.executable,str(root/'tools/generate_qpr2_compact_materials.py')],check=True)
assert prior==(k/'nv2a_gpu_compact.inc').read_bytes()
p=root/'tools/test_qpr2_literal_materials.py';s=p.read_text()
s=s.replace("if id:flags+=f'", "if id:flags+=f'#define NV_GPU_COMPACT_MATERIALS 1\\n")
s=s.replace("if id:source+='#include", "if id:source+='#include")
s=s.replace("str(k/'nv2a_gpu_literal.inc')", "str(k/'nv2a_gpu_compact.inc')")
s=s.replace("'gpu_rc_literal' if id", "'gpu_rc_compact' if id")
# Exercise a broader input set, including signed zero and small finite values.
s=s.replace('return (int)(next()%8193)/2048.0f-2;', 'uint32_t u=next();if((u&7)==0){static const float edge[]={0.0f,-0.0f,0.5f,-0.5f,1.0f,-1.0f,0.0000001f,-0.0000001f};return edge[(u>>3)&7];}return (int)(next()%8193)/2048.0f-2;')
s=s.replace('65536','131072').replace('4096;scene++','8192;scene++').replace('196608','393216')
s=s.replace("prefix='qpr2-literal-materials-'","prefix='qpr2-compact-materials-'")
s=s.replace('bit-exact frozen combiner results','bit-exact frozen compact combiner results')
exec(compile(s,str(p),'exec'),{'__file__':str(p)})
# Every live operation is scalar SSA; no runtime virtual-register routing remains.
text=(k/'nv2a_gpu_compact.inc').read_text()
assert all(x not in text for x in ['GPU_RGET','GPU_RSET','gpu_reg_get','gpu_reg_set','gpu_read_rgb','gpu_read_alpha','gpu_map_out(s,'])
print('PASS: deterministic compact generation; direct scalar values, no virtual register lookup/write helpers')
