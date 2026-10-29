"""Compile the actual startup gate and actual renderer revision function.
Catch version drift before packaging while retaining rejection of stale objects
and missing recovered callbacks. Does not start the game or require Windows.
"""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1]
k=root/'third_party/xboxrecomp/src/kernel'
main=(root/'src/main.c').read_text()
gate=main[main.index('    unsigned renderer_revision = nv2a_pb_exec_overlay_revision();'):main.index('    printf("Title ID:')]
renderer=(k/'nv2a_pb_exec.c').read_text()
revision=renderer[renderer.index('unsigned nv2a_pb_exec_overlay_revision(void)'):renderer.index('\n#if defined(_WIN32)',renderer.index('unsigned nv2a_pb_exec_overlay_revision(void)'))]
source=r'''
#include <stdio.h>
#include <assert.h>
#include "nv2a_overlay_revision.h"
static int callbacks_ok=1,callback_calls;
static unsigned old_revision;
static int qpr2_validate_level_callbacks(void){++callback_calls;return callbacks_ok;}
'''+revision+r'''
static unsigned linked_revision(void){return old_revision?old_revision:nv2a_pb_exec_overlay_revision();}
#define nv2a_pb_exec_overlay_revision linked_revision
static int boot_gate(void){
'''+gate+r'''
return 0;
}
int main(void){
 assert(boot_gate()==0 && callback_calls==1);
 callbacks_ok=0;assert(boot_gate()==4 && callback_calls==2);
 callbacks_ok=1;old_revision=QPR2_RENDERER_REVISION-1;
 assert(boot_gate()==4 && callback_calls==2);
 puts("PASS: actual startup gate accepts linked renderer; still rejects stale renderer and missing callbacks");
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-startup-gate-') as d:
 p=Path(d);(p/'fixture.c').write_text(source)
 subprocess.run(['gcc','-std=c11','-Wall','-Wextra','-Werror','-I'+str(k),str(p/'fixture.c'),'-o',str(p/'fixture')],check=True)
 subprocess.run([str(p/'fixture')],check=True,timeout=10)
