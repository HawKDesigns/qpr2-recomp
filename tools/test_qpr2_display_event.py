"""Exercise the retail boot display routine, real frame-clock event dispatch
and kernel wait bridge. Native event/video services use deterministic shims."""
from pathlib import Path
import subprocess,tempfile
import test_qpr2_gpu_event as gpu
root=gpu.root
clock=gpu.between(gpu.mem,'#define XBOX_MAX_FRAME_COUNTERS','static void fence_mirrors_tick')
display=gpu.between((root/'src/recomp/gen/recomp_0034.c').read_text(),
    'void sub_001A00E0(void)\n{','/**\n * sub_001A0270')
shim=gpu.shim.replace('void xbox_Nv2aFrameCounterFlip(void) {}','').replace(
    'InterlockedExchange(&waiting,1);','__atomic_add_fetch(&waiting,1,__ATOMIC_SEQ_CST);')
shim+=r'''
static uint64_t test_us;
static int QueryPerformanceCounter(LARGE_INTEGER *p) {p->QuadPart=test_us;return 1;}
static int QueryPerformanceFrequency(LARGE_INTEGER *p) {p->QuadPart=1000000;return 1;}
'''
guest=gpu.guest.replace('recomp_func_t recomp_lookup_kernel(uint32_t va) {assert(va==0xFE000100);return kernel_wait;}',r'''
static volatile LONG display_calls;
static void kernel_display(void) {
 /* The first native request asks retail to retry at the next vblank. */
 LONG n=__atomic_add_fetch(&display_calls,1,__ATOMIC_SEQ_CST);g_eax=n==1;g_esp+=28;
}
static void kernel_encoder(void) {
 if(STACK_ARG(1)==15)BRIDGE_MEM32(STACK_ARG(3))=0;
 g_eax=0;g_esp+=20;
}
static void kernel_irql(void) {g_eax=0;g_esp+=4;}
recomp_func_t recomp_lookup_kernel(uint32_t va) {
 switch(va) {
 case 0xFE000100:return kernel_wait;
 case 0xFE000104:return kernel_display;
 case 0xFE000108:return kernel_encoder;
 case 0xFE00010C:return kernel_irql;
 default:assert(0);return NULL;
 }
}
''')
fixture=r'''
static void *display_thread(void *unused) {
 (void)unused;g_esp=0x180000;MEM32(g_esp)=0x12345;
 g_edi=0x110000;g_esi=91;g_ebx=93;g_ebp=g_seh_ebp=94;
 sub_001A00E0();
 assert(g_esp==0x180004 && g_edi==0x110000 && g_esi==91 && g_ebx==93);
 return NULL;
}
int main(void) {
 setenv("RECOMP_TITLE_KEVENTS","1",1);
 g_xbox_mem_offset=g_memory_offset=(ptrdiff_t)ram;
 MEM32(0x1A50E8)=0x110000;MEM32(0x111954)=MEM32(0x111964)=0x00040000;
 MEM32(0x111974)=0x400000;MEM32(0x11002C)=100;
 MEM32(0x110030)=0x130200;MEM32(0x130200)=100;
 MEM32(0x1115C4)=0x150000;MEM32(0x150004)=0;
 MEM32(0x1ADEB0)=0xFE000100;MEM32(0x1ADEC0)=0xFE000104;
 MEM32(0x1ADEB8)=0xFE000108;MEM32(0x1ADEB4)=0xFE00010C;
 MEM32(0x1ADE78)=0x150100;MEM32(0x150100)=0x200; /* skip MMIO mode override */
 assert(xbox_Nv2aFrameCounter(0x1A50E8,0x1980)==0);
 assert(xbox_Nv2aVblankEvent(0x1A50E8,0x1954)==0);
 assert(xbox_Nv2aSoftwareEvent(0x1A50E8,0x1964,5)==0);
 frame_counters_tick();assert(!MEM32(0x111958) && !nmade);
 pthread_t thread;assert(!pthread_create(&thread,NULL,display_thread,NULL));
 while(InterlockedCompareExchange(&waiting,0,0)<1)sched_yield();
 /* V38 stays here without a vblank notification; no display API is reached. */
 assert(!display_calls && !MEM32(0x111958));
 test_us=16000;frame_counters_tick();assert(!display_calls && !MEM32(0x111958));
 test_us=16700;frame_counters_tick();
 while(InterlockedCompareExchange(&waiting,0,0)<2)sched_yield();
 assert(display_calls==1 && !MEM32(0x111958));
 test_us=33400;frame_counters_tick();assert(!pthread_join(thread,NULL));
 assert(display_calls==2 && retail_waits==2);
 assert(!MEM32(0x111968) && !g_software_events[0].signals);
 uint32_t count=MEM32(0x111980);
 nv2a_pb_exec_method(0,0x100,5);
 assert(MEM32(0x111968)==1 && MEM32(0x111980)==count);
 puts("PASS: actual retail boot display setup unblocks on elapsed vblank; native retry waits for another tick; RET/register ABI; display and GPU completion events remain separate");
}
'''
with tempfile.TemporaryDirectory() as d:
 source=Path(d)/'test.c';binary=Path(d)/'test'
 renderer='#include "'+str(root/'third_party/xboxrecomp/src/kernel/nv2a_pb_exec.c')+'"\n'
 scan='#include "'+str(root/'third_party/xboxrecomp/src/kernel/nv2a_pb_scan.c')+'"\n'
 source.write_text(shim+gpu.shadow+gpu.lookup+gpu.wait_trace+gpu.events+gpu.fence_wait+gpu.wait+gpu.command_fences+gpu.registration+clock+renderer+scan+guest+gpu.fence_wrapper+gpu.guest_body+display+fixture)
 subprocess.run(['gcc','-std=gnu11','-O1','-ffunction-sections','-fdata-sections','-fsanitize=undefined','-I'+str(root/'src'),'-Werror=implicit-function-declaration','-I'+str(root/'third_party/xboxrecomp/src'),'-I'+str(root/'third_party/xboxrecomp/src/kernel'),'-I'+str(root/'third_party/xboxrecomp/include'),str(source),str(root/'third_party/xboxrecomp/src/kernel/nv2a_vsh_interp.c'),str(root/'third_party/xboxrecomp/src/kernel/nv2a_combiner.c'),'-Wl,--gc-sections','-pthread','-lm','-o',str(binary)],check=True)
 subprocess.run([str(binary)],check=True,timeout=10)
