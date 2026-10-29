"""Stress the production bridge handle table with concurrent native workers.

An optional old kernel_bridge.c plus --expect-race reproduces token aliasing.
No game files or Windows SDK are needed; SRW locks use pthread rwlocks.
"""
from pathlib import Path
import subprocess,sys,tempfile
root=Path(__file__).resolve().parents[1]
args=[x for x in sys.argv[1:] if x!='--expect-race']
source=Path(args[0]) if args else root/'third_party/xboxrecomp/src/kernel/kernel_bridge.c'
bridge=source.read_text()
body=bridge[bridge.index('#define BRIDGE_HANDLE_TAG'):bridge.index('static void bridge_build_oa(')]
shim=r'''
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
typedef void *HANDLE;
#define INVALID_HANDLE_VALUE ((HANDLE)(intptr_t)-1)
typedef pthread_rwlock_t SRWLOCK;
#define SRWLOCK_INIT PTHREAD_RWLOCK_INITIALIZER
static void AcquireSRWLockExclusive(SRWLOCK*p){assert(!pthread_rwlock_wrlock(p));}
static void ReleaseSRWLockExclusive(SRWLOCK*p){assert(!pthread_rwlock_unlock(p));}
static void AcquireSRWLockShared(SRWLOCK*p){assert(!pthread_rwlock_rdlock(p));}
static void ReleaseSRWLockShared(SRWLOCK*p){assert(!pthread_rwlock_unlock(p));}
static uint32_t ram[65536];
#define BRIDGE_MEM32(a) ram[(uint32_t)(a)/4]
'''
fixture=r'''
enum {THREADS=16,EACH=128};
static pthread_barrier_t barrier;
static uint32_t tokens[THREADS][EACH];
static HANDLE expected[THREADS][EACH];
static void *produce(void *raw){
 unsigned id=(unsigned)(uintptr_t)raw;
 int rc=pthread_barrier_wait(&barrier);assert(rc==0 || rc==PTHREAD_BARRIER_SERIAL_THREAD);
 for(unsigned j=0;j<EACH;j++)tokens[id][j]=bridge_handle_token(expected[id][j]);
 return NULL;
}
static void *consume(void *raw){
 unsigned id=(unsigned)(uintptr_t)raw;
 for(unsigned j=0;j<EACH;j++){
  uint32_t token=tokens[id][j];uint32_t va=(id*EACH+j+1)*4;
  bridge_write_handle(va,expected[id][j]);assert(BRIDGE_MEM32(va)==token);
  assert(bridge_read_handle(va)==expected[id][j]);
  assert(bridge_resolve_handle(token)==expected[id][j]);
  assert(bridge_take_handle(token)==expected[id][j]);
  assert(!bridge_resolve_handle(token) && !bridge_take_handle(token));
 }
 return NULL;
}
int main(int argc,char **argv){
 (void)argv;unsigned failures=0;
 for(unsigned round=0;round<20;round++){
  memset(s_handle_table,0,sizeof s_handle_table);
  for(unsigned i=0;i<THREADS;i++)for(unsigned j=0;j<EACH;j++)
   expected[i][j]=(HANDLE)(uintptr_t)(0x100000u+(i*EACH+j)*16u);
  pthread_t threads[THREADS];assert(!pthread_barrier_init(&barrier,NULL,THREADS));
  for(unsigned i=0;i<THREADS;i++)assert(!pthread_create(threads+i,NULL,produce,(void*)(uintptr_t)i));
  for(unsigned i=0;i<THREADS;i++)assert(!pthread_join(threads[i],NULL));
  assert(!pthread_barrier_destroy(&barrier));
  unsigned errors=0;
  for(unsigned i=0;i<THREADS;i++)for(unsigned j=0;j<EACH;j++)
   if(bridge_resolve_handle(tokens[i][j])!=expected[i][j])++errors;
  if(errors){failures+=errors;if(argc==1){fprintf(stderr,"lost %u handle mappings\n",errors);return 1;}}
  else {
   for(unsigned i=0;i<THREADS;i++)assert(!pthread_create(threads+i,NULL,consume,(void*)(uintptr_t)i));
   for(unsigned i=0;i<THREADS;i++)assert(!pthread_join(threads[i],NULL));
   for(unsigned i=1;i<BRIDGE_HANDLE_MAX;i++)assert(!s_handle_table[i]);
  }
 }
 if(argc>1){assert(failures);printf("PASS: previous unlocked production table reproduced %u lost/aliased mappings\n",failures);return 0;}
 assert(bridge_resolve_handle(0xFFFFFFFEu)==(HANDLE)(intptr_t)-2);
 assert(bridge_resolve_handle(0xDEAD0001u)==(HANDLE)(uintptr_t)0xDEAD0001u);
 assert(!bridge_resolve_handle(BRIDGE_HANDLE_TAG) && !bridge_take_handle(BRIDGE_HANDLE_TAG|BRIDGE_HANDLE_MAX));
 puts("PASS: 40,960 concurrent creates preserve unique handles; duplicate publication, parallel reads/removal, pseudo-handles and invalid tokens");
}
'''
with tempfile.TemporaryDirectory() as d:
 p=Path(d);c=p/'test.c';exe=p/'test';c.write_text(shim+body+fixture)
 subprocess.run(['gcc','-std=gnu11','-O2','-fsanitize=undefined','-fno-sanitize-recover=all','-Werror=implicit-function-declaration',str(c),'-pthread','-o',str(exe)],check=True)
 subprocess.run([str(exe),*(['baseline'] if '--expect-race' in sys.argv else [])],check=True,timeout=60)
