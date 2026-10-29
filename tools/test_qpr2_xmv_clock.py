"""Actual boot registration -> real display clock -> retail XMV clock reader.
The startup snippet comes from main.c; the fixture cannot supply its own offset.
"""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1]
base=root/'tools/test_qpr2_display_event.py';text=base.read_text();ns={'__file__':str(base)}
exec(compile(text[:text.index("fixture=r'''")],str(base),'exec'),ns)
gpu=ns['gpu'];main=(root/'src/main.c').read_text()
registration=gpu.between(main, '    /* QPR2\'s XMV player reads', '    g_esp = XBOX_STACK_TOP;')
reader=gpu.between((root/'src/recomp/gen/recomp_0033.c').read_text(),'void sub_001963B0(void)\n{','/**\n * sub_001963F0')
fixture=r'''
static uint32_t retail_counter(void){
 g_esp=0x180000;MEM32(g_esp)=0x12345;MEM32(g_esp+4)=0x190000;
 sub_001963B0();assert(g_esp==0x180008);return MEM32(0x190004);
}
static void init_device(uint32_t device,uint32_t initial){
 MEM32(device+0x1954)=0x00040000;MEM32(device+0x1974)=0x400000;
 MEM32(device+0x197C)=0xA1A2A3A4;MEM32(device+0x1980)=initial;MEM32(device+0x1984)=0xB1B2B3B4;
}
static void neighbors(uint32_t device){assert(MEM32(device+0x197C)==0xA1A2A3A4 && MEM32(device+0x1984)==0xB1B2B3B4);}
int main(void){
 g_xbox_mem_offset=g_memory_offset=(ptrdiff_t)ram;
 MEM32(0x1A50E8)=0x110000;init_device(0x110000,0x10203040);
 actual_boot_registration();
 assert(g_frame_counter_count==1 && g_frame_counters[0].device_ptr_va==0x1A50E8 && g_frame_counters[0].counter_off==0x1980 && g_frame_counters[0].event_off==0x1954);
 assert(xbox_Nv2aFrameCounter(0x1A50E8,0x1981)<0 && xbox_Nv2aFrameCounter(0x1A50E9,0x1980)<0 && g_frame_counter_count==1);
 frame_counters_tick();assert(retail_counter()==0x10203040);neighbors(0x110000);
 test_us=16000;frame_counters_tick();assert(retail_counter()==0x10203040);
 test_us=16700;frame_counters_tick();assert(retail_counter()==0x10203041);neighbors(0x110000);
 test_us=1001000;frame_counters_tick();assert(retail_counter()==0x10203040+60);
 xbox_Nv2aFrameCounterFlip();assert(retail_counter()==0x10203040+60);
 test_us=6006000;frame_counters_tick();assert(retail_counter()==0x10203040+360);neighbors(0x110000);
 puts("PASS: main.c's actual binding and retail clock reader agree: one tick adds one, 1.001 s adds 60, delayed polls retain elapsed ticks, flips add none, neighboring DWORDs stay intact");
 /* A device replacement and PAL mode switch re-seed the phase. */
 MEM32(0x1A50E8)=0x120000;init_device(0x120000,UINT32_MAX-1);
 frame_counters_tick();assert(retail_counter()==UINT32_MAX-1);
 test_us+=50050;frame_counters_tick();assert(retail_counter()==1);neighbors(0x120000);
 MEM32(0x121FE0)=0x01312D00;frame_counters_tick();assert(retail_counter()==1);
 test_us+=20000;frame_counters_tick();assert(retail_counter()==2);neighbors(0x120000);
 assert(MEM32(0x111980)==0x10203040+360);
 puts("PASS: actual guest reader observes counter wrap, device replacement and PAL/NTSC phases; misaligned registrations are rejected before clock writes");
 return 0;
}
'''
with tempfile.TemporaryDirectory(prefix='qpr2-xmv-clock-') as folder:
 tmp=Path(folder);source=tmp/'test.c';binary=tmp/'test'
 code=ns['shim']+gpu.shadow+gpu.lookup+gpu.wait_trace+gpu.events+gpu.fence_wait+gpu.wait+gpu.command_fences+gpu.registration+ns['clock']+ns['guest'].split('static unsigned retail_waits;')[0]
 code+='\nstatic void actual_boot_registration(void){\n'+registration+'\n}\n'+reader+fixture
 source.write_text(code)
 subprocess.run(['gcc','-std=gnu11','-O1','-ffunction-sections','-fdata-sections','-fsanitize=undefined','-fno-sanitize-recover=all','-I'+str(root/'src'),'-Werror=implicit-function-declaration','-I'+str(root/'third_party/xboxrecomp/src'),'-I'+str(root/'third_party/xboxrecomp/src/kernel'),'-I'+str(root/'third_party/xboxrecomp/include'),str(source),'-Wl,--gc-sections','-pthread','-lm','-o',str(binary)],check=True)
 subprocess.run([str(binary)],check=True,timeout=10)
 # The fractional clock arithmetic has its own full-duration/wrap reference.
 clock_binary=tmp/'clock'
 subprocess.run(['gcc','-std=gnu11','-O2','-fsanitize=undefined','-fno-sanitize-recover=all',str(root/'tools/test_qpr2_frame_clock.c'),'-o',str(clock_binary)],check=True)
 subprocess.run([str(clock_binary)],check=True,timeout=10)
 print('PASS: actual startup/movie-clock integration and fractional display-clock regression tests')
