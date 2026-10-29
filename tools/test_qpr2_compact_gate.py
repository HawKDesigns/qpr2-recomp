"""Compact-family hardware admission and preservation of the validated V97 path."""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1];k=root/'third_party/xboxrecomp/src/kernel';path=root/'tools/test_qpr2_literal_gate.py'
s=path.read_text();ns={'__file__':str(path)};exec(compile(s[:s.index('\nwith tempfile.TemporaryDirectory(')],str(path),'exec'),ns)
api=ns['api'];fixture=ns['fixture'];main=ns['main']
main=main.replace('setenv("RECOMP_GPU_COMPACT_MATERIALS","0",1);','''setenv("RECOMP_GPU_COMPACT_MATERIALS",!strcmp(mode,"compact-off")?"0":"1",1);
 if(!strncmp(mode,"ccompile",8))compact_compile_fail=atoi(mode+8);
 if(!strncmp(mode,"ccreate",7))compact_create_fail=atoi(mode+7);
 if(!strncmp(mode,"ccolor",6))compact_color_bad=atoi(mode+6);
 if(!strncmp(mode,"cdepth",6))compact_depth_bad=atoi(mode+6);
 if(!strncmp(mode,"ccount",6))compact_count_bad=atoi(mode+6);
 if(!strncmp(mode,"cmap",4))compact_map_fail=atoi(mode+4);
 if(!strcmp(mode,"cvariant"))compact_variant_bad=1;''')
main=main.replace('if(literal_map_fail){','if(literal_map_fail || compact_map_fail){')
a=' assert(g.literal.ready==expected && g.literal.rejected==failed);';assert a in main
main=main.replace(a,a+'''
 unsigned compact_failed=0;
 if(g.literal.compact_enabled && g.native && g.native_batch && g.literal.enabled){
  if(compact_compile_fail==99 || compact_create_fail==99 || literal_compile_fail==99 || literal_create_fail==99)compact_failed=7;
  else for(unsigned id=1;id<=3;id++)if(compact_compile_fail==(int)id || compact_create_fail==(int)id || compact_color_bad==(int)id || compact_depth_bad==(int)id || compact_count_bad==(int)id || (compact_variant_bad && id==2) || (failed&(1u<<(id-1))))compact_failed|=1u<<(id-1);
 }
 unsigned expected_compact=g.literal.compact_enabled?expected&~compact_failed:0;
 assert(g.literal.compact_ready==expected_compact && g.literal.compact_rejected==compact_failed);
 for(unsigned id=1;id<=3;id++)if(expected&(1u<<(id-1)))assert(literal_is_compact[id-1]==((expected_compact>>(id-1))&1));
''')
main=main.replace('gpu_native_variant(g.array_registers?1u:0u)|(id<<12));','gpu_native_variant(g.array_registers?1u:0u)|(id<<12)|((expected_compact&(1u<<(id-1)))?32768u:0u));')
main=main.replace('"shader-cache/qpr2-rov-literal%u-%016llx.dxbc",id,(unsigned long long)key','"shader-cache/qpr2-rov-literal%u%s-%016llx.dxbc",id,(expected_compact&(1u<<(id-1)))?"-compact":"",(unsigned long long)key')
main=main.replace('g.literal.compiling=id;assert(gpu_rov_code','g.literal.compiling=id;g.literal.compact_compiling=(expected_compact>>(id-1))&1;assert(gpu_rov_code').replace('g.literal.compiling=0;\n  assert(compiler_calls','g.literal.compiling=g.literal.compact_compiling=0;\n  assert(compiler_calls')
# Clearing faults after admission proves fallback draw routing, not just readiness.
main=main.replace(' memset(literal_draws,0,sizeof literal_draws);',''' compact_color_bad=compact_depth_bad=compact_count_bad=compact_variant_bad=0;
 memset(compact_draws,0,sizeof compact_draws);
 memset(literal_draws,0,sizeof literal_draws);''')
main=main.replace(' if(expected)assert(g.literal.draws', ' for(unsigned id=0;id<3;id++)if(expected_compact&(1u<<id))assert(compact_draws[id]>0);else assert(!compact_draws[id]);\n if(expected)assert(g.literal.draws')
with tempfile.TemporaryDirectory(prefix='qpr2-compact-gate-') as folder:
 p=Path(folder)
 for name in ['windows.h','d3d11.h','d3d11_3.h','d3dcompiler.h','dxgi.h']:(p/name).write_text('/* strict fixture API */\n')
 (p/'test.c').write_text(api+'ptrdiff_t xbox_GetMemoryOffset(void){return 0;}\n#include "'+str(k/'nv2a_gpu_raster.c')+'"\n'+fixture+main)
 subprocess.run(['gcc','-std=gnu11','-O1','-ffp-contract=off','-fsanitize=undefined,float-cast-overflow','-fno-sanitize-recover=all','-Werror=implicit-function-declaration','-Werror=incompatible-pointer-types','-I'+str(p),'-I'+str(root/'tools'),'-I'+str(k),str(p/'test.c'),str(k/'nv2a_gpu_tiles.c'),str(k/'nv2a_vsh_interp.c'),'-pthread','-lm','-o',str(p/'test')],check=True)
 modes=['normal','warm','compact-off','disabled','no-batch','array','vector-off','usage-off','seed-off','empty-off','diagnostic','cvariant','ccompile99','ccreate99','compile99','create99','compute']+[kind+str(id) for kind in ['ccompile','ccreate','ccolor','cdepth','ccount','cmap'] for id in range(1,4)]
 for mode in modes:
  print('RUN compact gate '+mode,flush=True);cwd=p/('normal' if mode=='warm' else mode);cwd.mkdir(exist_ok=True);subprocess.run([str(p/'test'),mode],cwd=cwd,check=True,timeout=180)
 print('PASS: all '+str(len(modes))+' compact shader cache/failure/V97 fallback/mixed-publication modes')
