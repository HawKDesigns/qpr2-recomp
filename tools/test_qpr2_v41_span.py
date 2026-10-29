"""Compare the actual row-span kernel with its dense bounding-box scan."""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1]
src=(root/'third_party/xboxrecomp/src/kernel/nv2a_pb_exec.c').read_text()
a=src.index('static void xf_rows(const XfTri');b=src.index('/* Worker threads',a)
dense=src[a:b].replace('static void xf_rows(', 'static void xf_rows_dense(').replace('int span=', 'int span=0 && ')
fixture=r'''
#include <time.h>
static void job(XfTri *t,Nv2aVshOutput *a,Nv2aVshOutput *b,Nv2aVshOutput *c) {
 memset(t,0,sizeof *t);t->mem=ram;t->bpp=4;t->a=a->pos;t->b=b->pos;t->c=c->pos;
 t->va=a;t->vb=b;t->vc=c;t->iw[0]=t->iw[1]=t->iw[2]=1;
 t->area=(b->pos[0]-a->pos[0])*(c->pos[1]-a->pos[1])-(b->pos[1]-a->pos[1])*(c->pos[0]-a->pos[0]);
 t->inv_area=1/t->area;t->maxx=640;t->maxy=480;
 t->inclusive[0]=nv2a_edge_inclusive(t->a,t->b,t->area);
 t->inclusive[1]=nv2a_edge_inclusive(t->b,t->c,t->area);
 t->inclusive[2]=nv2a_edge_inclusive(t->c,t->a,t->area);
}
int main(void) {
 setup();s_gpu.pitch=2560;s_gpu.clip_w=640;s_gpu.clip_h=480;assert(surface_begin_batch(ram));
 uint8_t *expected=malloc(640*480*4);assert(expected);
 for(int i=0;i<80;i++) {
  Nv2aVshOutput a=point(0,i%20),b=point(2+i%13,i%20),c=point(639,479-i%7);
  XfTri t;job(&t,&a,(i&1)?&c:&b,(i&1)?&b:&c);XfCount ref={0},now={0};
  memset(ram+0x1000,0,640*480*4);xf_rows_dense(&t,0,1,&ref);memcpy(expected,ram+0x1000,640*480*4);
  memset(ram+0x1000,0,640*480*4);xf_rows(&t,0,1,&now);
  assert(now.pixels==ref.pixels && !memcmp(expected,ram+0x1000,640*480*4));
 }
 Nv2aVshOutput a=point(0,0),b=point(2,0),c=point(639,480);XfTri t;job(&t,&a,&b,&c);
 XfCount count={0};clock_t start=clock();for(int i=0;i<100;i++)xf_rows_dense(&t,0,1,&count);
 double ref_ms=(double)(clock()-start)*1000/CLOCKS_PER_SEC;
 start=clock();for(int i=0;i<100;i++)xf_rows(&t,0,1,&count);
 double span_ms=(double)(clock()-start)*1000/CLOCKS_PER_SEC;
 printf("PASS: actual span kernel matches dense scan for 80 thin triangles/both windings; synthetic 100-triangle benchmark dense=%.2fms span=%.2fms (not a game FPS estimate)\n",ref_ms,span_ms);
 free(expected);
}
'''
with tempfile.TemporaryDirectory() as d:
 p=Path(d)/'test.c';exe=Path(d)/'test'
 p.write_text('#define main baseline_raster_main\n#include "'+str(root/'tools/test_qpr2_raster_capture.c')+'"\n#undef main\n'+dense+fixture)
 subprocess.run(['gcc','-std=gnu11','-O2','-ffunction-sections','-fdata-sections','-fsanitize=undefined,float-cast-overflow','-I'+str(root/'third_party/xboxrecomp/src'),'-I'+str(root/'third_party/xboxrecomp/include'),str(p),str(root/'third_party/xboxrecomp/src/kernel/nv2a_combiner.c'),'-Wl,--gc-sections','-lm','-o',str(exe)],check=True)
 subprocess.run([str(exe)],check=True,timeout=30)
