"""Compile actual Win32 publication/read helpers with pthread-backed API shims."""
from pathlib import Path
import tempfile,subprocess
root=Path(__file__).resolve().parents[1]
s=(root/'third_party/xboxrecomp/src/video/fb_present.c').read_text()
s=s[:s.index('/* Which keys are down')].replace('#if defined(_WIN32)','').replace('#include <windows.h>','')
shim='''#include <stdint.h>
#include <stddef.h>
#include <pthread.h>
#include <sched.h>
#include <assert.h>
typedef int32_t LONG;
typedef pthread_mutex_t CRITICAL_SECTION;
static LONG InterlockedCompareExchange(volatile LONG *p,LONG n,LONG old) { __atomic_compare_exchange_n(p,&old,n,0,__ATOMIC_SEQ_CST,__ATOMIC_SEQ_CST);return old; }
static LONG InterlockedExchange(volatile LONG *p,LONG n) {return __atomic_exchange_n(p,n,__ATOMIC_SEQ_CST);}
static void InitializeCriticalSection(CRITICAL_SECTION *p) {assert(!pthread_mutex_init(p,NULL));}
static void EnterCriticalSection(CRITICAL_SECTION *p) {assert(!pthread_mutex_lock(p));}
static void LeaveCriticalSection(CRITICAL_SECTION *p) {assert(!pthread_mutex_unlock(p));}
static void Sleep(int ms) {(void)ms;sched_yield();}
'''
fixture='''
ptrdiff_t xbox_GetMemoryOffset(void) {return 0;}
static volatile LONG done;
static void *produce(void *unused) {
 (void)unused;uint32_t *p=malloc(640*480*4);assert(p);
 for(uint32_t frame=1;frame<=300;frame++) {
  for(int i=0;i<640*480;i++)p[i]=frame;
  xbox_FramebufferWindowPresentPixels(p,640,480);
 }
 InterlockedExchange(&done,1);free(p);return NULL;
}
int main(void) {
 pthread_t thread;uint32_t *copy=malloc(640*480*4);unsigned reads=0;assert(copy);
 s_fb_running=1;assert(!fb_present_read(copy));assert(!pthread_create(&thread,NULL,produce,NULL));
 while(!InterlockedCompareExchange(&done,1,1))if(fb_present_read(copy)) {
  for(int i=0;i<640*480;i++)assert(copy[i]==copy[0]);++reads;
 }
 assert(!pthread_join(thread,NULL));assert(fb_present_read(copy));
 for(int i=0;i<640*480;i++)assert(copy[i]==300);
 assert(reads);free(copy);free(s_present[0]);free(s_present[1]);
 puts("PASS: actual window handoff/read helpers under concurrent rapid flips; no mixed frames, final frame published");
}
'''
with tempfile.TemporaryDirectory() as d:
 source=Path(d)/'test.c';binary=Path(d)/'test';source.write_text(shim+s+fixture)
 subprocess.run(['gcc','-std=gnu11','-O1','-fsanitize=undefined','-pthread',str(source),'-o',str(binary)],check=True)
 subprocess.run([str(binary)],check=True)
