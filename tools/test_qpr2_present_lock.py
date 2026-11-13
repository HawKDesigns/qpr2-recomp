"""Compile actual Win32 publication/read helpers with pthread-backed API shims."""
from pathlib import Path
import tempfile,subprocess
root=Path(__file__).resolve().parents[1]
s=(root/'third_party/xboxrecomp/src/video/fb_present.c').read_text()
state=s[s.index('static volatile LONG s_fb_running;'):s.index('/* The last PVIDEO frame shown.')]
helpers=s[s.index('/* Publish a resolved frame'):s.index('/* Title bar,')]
shim='''#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
typedef int32_t LONG;
typedef pthread_rwlock_t SRWLOCK;
#define SRWLOCK_INIT PTHREAD_RWLOCK_INITIALIZER
static LONG InterlockedCompareExchange(volatile LONG *p,LONG n,LONG old) { __atomic_compare_exchange_n(p,&old,n,0,__ATOMIC_SEQ_CST,__ATOMIC_SEQ_CST);return old; }
static LONG InterlockedExchange(volatile LONG *p,LONG n) {return __atomic_exchange_n(p,n,__ATOMIC_SEQ_CST);}
static void AcquireSRWLockShared(SRWLOCK *p) {assert(!pthread_rwlock_rdlock(p));}
static void AcquireSRWLockExclusive(SRWLOCK *p) {assert(!pthread_rwlock_wrlock(p));}
static void ReleaseSRWLockShared(SRWLOCK *p) {assert(!pthread_rwlock_unlock(p));}
static void ReleaseSRWLockExclusive(SRWLOCK *p) {assert(!pthread_rwlock_unlock(p));}
ptrdiff_t xbox_GetMemoryOffset(void);
'''
fixture='''
ptrdiff_t xbox_GetMemoryOffset(void) {return 0;}
static volatile LONG done;
static pthread_barrier_t first_frame;
static void *produce(void *unused) {
 (void)unused;uint32_t *p=malloc(640*480*4);assert(p);
 for(uint32_t frame=1;frame<=300;frame++) {
  for(int i=0;i<640*480;i++)p[i]=frame;
  xbox_FramebufferWindowPresentPixels(p,640,480);
  if(frame==1)pthread_barrier_wait(&first_frame);
 }
 InterlockedExchange(&done,1);free(p);return NULL;
}
int main(void) {
 pthread_t thread;uint32_t *copy=malloc(640*480*4);unsigned reads=0;assert(copy);
 s_fb_running=1;unsetenv("RECOMP_FB_VA");assert(!fb_copy_presented_frame(copy));
 assert(!pthread_barrier_init(&first_frame,NULL,2));assert(!pthread_create(&thread,NULL,produce,NULL));
 pthread_barrier_wait(&first_frame);
 do {
  assert(fb_copy_presented_frame(copy));
  for(int i=0;i<640*480;i++)assert(copy[i]==copy[0]);
  ++reads;
 } while(!InterlockedCompareExchange(&done,1,1));
 assert(!pthread_join(thread,NULL));assert(!pthread_barrier_destroy(&first_frame));
 assert(fb_copy_presented_frame(copy));
 for(int i=0;i<640*480;i++)assert(copy[i]==300);
 assert(reads);free(copy);free(s_present[0]);free(s_present[1]);
 puts("PASS: actual window handoff/read helpers under concurrent rapid flips; no mixed frames, final frame published");
}
'''
with tempfile.TemporaryDirectory() as d:
 source=Path(d)/'test.c';binary=Path(d)/'test';source.write_text(shim+state+helpers+fixture)
 subprocess.run(['gcc','-std=gnu11','-O1','-fsanitize=undefined','-pthread',str(source),'-o',str(binary)],check=True)
 subprocess.run([str(binary)],check=True)
