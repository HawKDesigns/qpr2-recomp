"""Actual FIFO scanner, pixel executor, fence registration/release and GPU
notification bridge. Sparse guest RAM and pthread native events only."""
import subprocess,tempfile
from pathlib import Path
import test_qpr2_gpu_event as gpu
root=gpu.root
shim=gpu.shim.replace('static uint8_t ram[0x200000];','static uint8_t *ram;').replace(
 'return va>=0x10000 && va<=sizeof(ram)-bytes;',
 'return va>=0x10000 && ((uint64_t)va+bytes<=0x200000 || (va>=0x80000000 && (uint64_t)va+bytes<=0x84000000ull));')
shim=shim.replace('assert(BRIDGE_MEM32(0x50000)==0xFF112233);',
 'assert(BRIDGE_MEM32(0x50000)==0xFF112233 && BRIDGE_MEM32(0x80018000)==7);')
reader=gpu.between(gpu.mem,'int xbox_Nv2aFifoRead(uint32_t va,uint32_t *word)\n{','/*\n * Two counters inside the device')
fixture=r'''
#include <sys/mman.h>
static void put(uint32_t va,uint32_t word) {BRIDGE_MEM32(va)=word;}
int main(void) {
 const size_t size=0x84000000ull;
 ram=mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS|MAP_NORESERVE,-1,0);
 assert(ram!=MAP_FAILED);g_memory_offset=g_xbox_mem_offset=(ptrdiff_t)ram;
 setenv("RECOMP_TITLE_KEVENTS","1",1);
 const uint32_t lo=0x80010000,hi=lo+0x80,base=0x80018000;
 put(0x1A50E8,0x110000);put(0x110024,lo);put(0x110028,hi);
 put(0x110030,base);put(base,3);put(0x11002C,9);put(0x111964,0x40000);
 assert(!xbox_Nv2aCommandFence(0x1A50E8,0x30,8,0x20));
 assert(!xbox_Nv2aPushBufferRing(0x1A50E8,0x24,0x28));
 assert(!xbox_Nv2aSoftwareEvent(0x1A50E8,0x1964,5));
 uint32_t first,last;assert(fifo_ring_bounds(&first,&last) && first==lo && last==hi);
 nv2a_pb_exec_method(0,0x1A4,8);nv2a_pb_exec_method(0,0x1D6C,0);
 nv2a_pb_exec_method(0,NV097_SET_SURFACE_FORMAT,0x128);
 nv2a_pb_exec_method(0,NV097_SET_SURFACE_COLOR_OFFSET,0x50000);
 nv2a_pb_exec_method(0,NV097_SET_SURFACE_PITCH,32);
 nv2a_pb_exec_method(0,NV097_SET_SURFACE_CLIP_HORIZONTAL,8u<<16);
 nv2a_pb_exec_method(0,NV097_SET_SURFACE_CLIP_VERTICAL,8u<<16);
 /* Tail release then actual wrap to +20, skipping a stale prefix JUMP. */
 put(lo,0x20010070);put(lo+0x70,0x41D70);put(lo+0x74,5);put(lo+0x78,0x20010020);
 uint32_t head[]={0x40000|NV097_SET_COLOR_CLEAR_VALUE,0xFF112233,
  0x40000|NV097_CLEAR_SURFACE,NV097_CLEAR_COLOR_MASK,0x41D70,7,0x40100,5};
 memcpy(ram+lo+0x20,head,sizeof head);
 assert(BRIDGE_MEM32(base)==3 && !BRIDGE_MEM32(0x111968));
 check_draw_order=1;
 uint32_t get=nv2a_pb_execute_fifo(lo+0x70,lo+0x40,first,last);
 check_draw_order=0;
 assert(get==lo+0x40 && BRIDGE_MEM32(base)==7 && BRIDGE_MEM32(0x11002C)==9);
 assert(BRIDGE_MEM32(0x111968)==1 && g_command_fences[0].releases==2);
 /* The latest submitted sequence is released only when its command arrives. */
 put(lo+0x40,0x41D70);put(lo+0x44,9);
 get=nv2a_pb_execute_fifo(get,lo+0x48,first,last);assert(get==lo+0x48 && BRIDGE_MEM32(base)==9);
 assert(!xbox_Nv2aReleaseFence(9,0,123) && !xbox_Nv2aReleaseFence(8,0x20,123));
 assert(!xbox_Nv2aReleaseFence(8,1,123) && BRIDGE_MEM32(base)==9);
 assert(xbox_Nv2aReleaseFence(8,0x1C,456) && BRIDGE_MEM32(base+0x1C)==456);
 /* An unread bad command cannot make GET or the completion fence catch PUT. */
 put(lo+0x48,0x60000003);get=nv2a_pb_execute_fifo(get,lo+0x50,first,last);
 assert(get==lo+0x48 && BRIDGE_MEM32(base)==9);
 munmap(ram,size);
 puts("PASS: actual FIFO wrap -> draw -> semaphore release -> event order; submitted sequence cannot prematurely complete; DMA bounds/offset guards; invalid commands stay unconsumed");
}
'''
with tempfile.TemporaryDirectory() as d:
 source=Path(d)/'test.c';binary=Path(d)/'test'
 renderer='#include "'+str(root/'third_party/xboxrecomp/src/kernel/nv2a_pb_exec.c')+'"\n'
 scan='#include "'+str(root/'third_party/xboxrecomp/src/kernel/nv2a_pb_scan.c')+'"\n'
 source.write_text(shim+gpu.shadow+gpu.lookup+gpu.events+gpu.command_fences+gpu.registration+reader+renderer+scan+fixture)
 subprocess.run(['gcc','-std=gnu11','-O1','-ffunction-sections','-fdata-sections','-fsanitize=undefined','-Werror=implicit-function-declaration','-I'+str(root/'third_party/xboxrecomp/src'),'-I'+str(root/'third_party/xboxrecomp/include'),str(source),str(root/'third_party/xboxrecomp/src/kernel/nv2a_vsh_interp.c'),str(root/'third_party/xboxrecomp/src/kernel/nv2a_combiner.c'),'-Wl,--gc-sections','-pthread','-lm','-o',str(binary)],check=True)
 subprocess.run([str(binary)],check=True,timeout=10)
