/* Actual survey and persistent executor decoder: expose the old wrap loss,
 * parameter continuation, partial publication, CALL/RETURN and error GET. */
#include "../third_party/xboxrecomp/src/kernel/nv2a_pb_scan.c"
#include <assert.h>
static uint8_t ram[0x20000];
static uint32_t emitted[10000][3],n;
ptrdiff_t xbox_GetMemoryOffset(void) {return (ptrdiff_t)ram;}
void nv2a_pb_exec_report(void) {}
uint32_t nv2a_pb_exec_fifo_begin(void){return 4096;}
void nv2a_pb_exec_fifo_end(void){}
void nv2a_pb_exec_guest_access(uint64_t a,uint64_t n){(void)a;(void)n;}
void nv2a_pb_exec_method(uint32_t sub,uint32_t method,uint32_t param)
{assert(n<10000);emitted[n][0]=sub;emitted[n][1]=method;emitted[n++][2]=param;}
int xbox_Nv2aFifoRead(uint32_t va,uint32_t *word) {
 uint32_t at=va&0x0FFFFFFFu;if(at+4>sizeof ram)return 0;
 memcpy(word,ram+at,4);return 1;
}
static void put(uint32_t va,uint32_t word) {memcpy(ram+(va&0x0FFFFFFFu),&word,4);}
int main(void) {
 setenv("RECOMP_PB_EXEC","1",1);
 const uint32_t lo=0x80010000,hi=lo+0x80;
 /* Real JUMP targets +20, not the smallest previously observed PUT. The
  * old prefix scan encounters a stale JUMP and loses the fresh NOP 5. */
 put(lo,0x20010070);put(lo+0x20,0x40100);put(lo+0x24,5);
 put(lo+0x70,0x40110);put(lo+0x74,0);put(lo+0x78,0x20010020);
 /* Low-RAM aliases of the same bytes for the legacy direct reader. */
 nv2a_pb_scan(0x10070,0x10078);nv2a_pb_scan(0x10000,0x10028);
 assert(n==1 && emitted[0][1]==0x110);
 n=0;uint32_t get=nv2a_pb_execute_fifo(lo+0x70,lo+0x28,lo,hi);
 assert(get==lo+0x28 && n==2 && emitted[1][1]==0x100 && emitted[1][2]==5);
 /* Increasing method packet split across submissions, including words
  * whose low bits look like JUMPs/CALLs when incorrectly parsed as headers. */
 memset(&s_fifo,0,sizeof s_fifo);n=0;
 put(lo,(3u<<18)|0x100);put(lo+4,0x20000001);put(lo+8,0x00020000);put(lo+12,0x00000002);
 get=nv2a_pb_execute_fifo(lo,lo+8,lo,hi);assert(get==lo+8 && n==1 && s_fifo.remaining==2);
 assert(nv2a_pb_execute_fifo(get,lo+8,lo,hi)==get && n==1);
 get=nv2a_pb_execute_fifo(get,lo+16,lo,hi);assert(n==3 && !s_fifo.remaining);
 assert(emitted[0][1]==0x100 && emitted[1][1]==0x104 && emitted[2][1]==0x108);
 assert(emitted[1][2]==0x20000 && emitted[2][2]==2);
 /* Non-increasing packet crosses the per-poll word budget. */
 memset(&s_fifo,0,sizeof s_fifo);n=0;
 put(lo,0x40000000|(3u<<18)|0x1800);put(lo+4,1);put(lo+8,2);put(lo+12,3);
 get=nv2a_fifo_run(&s_fifo,lo,lo+16,lo,hi,xbox_Nv2aFifoRead,fifo_emit,2);
 assert(get==lo+8 && n==1 && s_fifo.remaining==2);
 get=nv2a_fifo_run(&s_fifo,get,lo+16,lo,hi,xbox_Nv2aFifoRead,fifo_emit,8);
 assert(n==3 && emitted[2][1]==0x1800 && emitted[2][2]==3);
 /* CALL is pending even when its return address is the published PUT. */
 memset(&s_fifo,0,sizeof s_fifo);n=0;
 put(lo,0x00011002);put(0x80011000,0x40100);put(0x80011004,5);put(0x80011008,0x20000);
 get=nv2a_fifo_run(&s_fifo,lo,lo+4,lo,hi,xbox_Nv2aFifoRead,fifo_emit,1);
 assert(s_fifo.in_call && nv2a_pb_fifo_subroutine()==0x10005);
 get=nv2a_pb_execute_fifo(get,lo+4,lo,hi);assert(get==lo+4 && n==1 && !s_fifo.in_call);
 /* Invalid opcode remains unconsumed; GET never jumps to PUT. */
 memset(&s_fifo,0,sizeof s_fifo);put(lo,0x60000003);
 get=nv2a_pb_execute_fifo(lo,lo+4,lo,hi);assert(get==lo && s_fifo.error);
 puts("PASS: reproduce guessed-wrap command loss; actual FIFO jump target, split packets, budget continuation, CALL/RETURN and no false GET on error");
}
