/* Compare the production command inventory with the prior linear inventory.
 * CPU timings describe this synthetic lookup workload, never gameplay FPS. */
#include "../third_party/xboxrecomp/src/kernel/nv2a_pb_scan.c"
#include <assert.h>
ptrdiff_t xbox_GetMemoryOffset(void){return 0;}
void nv2a_pb_exec_method(uint32_t s,uint32_t m,uint32_t p){(void)s;(void)m;(void)p;}
void nv2a_pb_exec_report(void){}
uint32_t nv2a_pb_exec_fifo_begin(void){return 4096;}
void nv2a_pb_exec_fifo_end(void){}
void nv2a_pb_exec_guest_access(uint64_t a,uint64_t n){(void)a;(void)n;}
int xbox_Nv2aFifoRead(uint32_t p,uint32_t*w){(void)p;(void)w;return 0;}
static struct {uint32_t method,subch,count;} reference[PB_MAX_METHODS];
static unsigned reference_count;
static void linear_note(uint32_t subch,uint32_t method)
{
    for(unsigned i=0;i<reference_count;i++)if(reference[i].method==method && reference[i].subch==subch){++reference[i].count;return;}
    if(reference_count<PB_MAX_METHODS){reference[reference_count].method=method;reference[reference_count].subch=subch;reference[reference_count++].count=1;}
}
static void verify(void)
{
    assert((unsigned)s_seen_count==reference_count);
    for(unsigned i=0;i<reference_count;i++)assert(s_seen[i].method==reference[i].method && s_seen[i].subch==reference[i].subch && s_seen[i].count==reference[i].count);
}
static uint32_t random_word(uint32_t *s){*s^=*s<<13;*s^=*s>>17;*s^=*s<<5;return *s;}
int main(void)
{
    setenv("RECOMP_PB_SCAN","1",1);
    for(unsigned i=0;i<1200;i++){note(0,i*4);linear_note(0,i*4);}
    uint32_t seed=0x51973;double begin=pb_wall_ms();
    for(unsigned i=0;i<2000000;i++)linear_note(0,(random_word(&seed)%1200)*4);
    double linear_ms=pb_wall_ms()-begin;seed=0x51973;begin=pb_wall_ms();
    for(unsigned i=0;i<2000000;i++)note(0,(random_word(&seed)%1200)*4);
    double direct_ms=pb_wall_ms()-begin;verify();
    /* Full table, repeated entries after saturation and malformed-range keys
     * retain the original counts and insertion order without aliasing. */
    for(unsigned sub=0;sub<8;sub++)for(unsigned m=0;m<2048;m++){note(sub,m*4);linear_note(sub,m*4);}
    for(unsigned i=0;i<10000;i++){uint32_t sub=random_word(&seed)%12,m=random_word(&seed)%0x5000;note(sub,m);linear_note(sub,m);}
    verify();
    memset(s_seen,0,sizeof s_seen);memset(s_seen_index,0,sizeof s_seen_index);s_seen_count=0;
    memset(reference,0,sizeof reference);reference_count=0;
    uint32_t methods[]={3,0x2000,0x1FFF,0xFFFFFFFC,0,4,0x1FFC};
    for(unsigned repeat=0;repeat<10;repeat++)for(unsigned i=0;i<7;i++){note(i%3,methods[i]);linear_note(i%3,methods[i]);}
    verify();s_inventory_enabled=0;begin=pb_wall_ms();
    for(unsigned i=0;i<2000000;i++)note(0,(random_word(&seed)%1200)*4);
    double disabled_ms=pb_wall_ms()-begin;verify();
    printf("PASS: exact legacy method counts/order, all subchannels, full-table repeats, malformed keys and opt-in gate; 2M updates linear_ms=%.3f direct_ms=%.3f disabled_ms=%.3f (synthetic CPU workload)\n",linear_ms,direct_ms,disabled_ms);
}
