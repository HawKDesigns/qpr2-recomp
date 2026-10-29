#define main baseline_raster_main
#include "test_qpr2_raster_capture.c"
#undef main
static PbUnhandled reference[PB_EXEC_MAX_UNHANDLED];
static unsigned reference_count;
static void legacy_unhandled(uint32_t method,uint32_t param)
{
    for(unsigned i=0;i<reference_count;i++)if(reference[i].method==method){++reference[i].count;reference[i].last_param=param;return;}
    if(reference_count<PB_EXEC_MAX_UNHANDLED){reference[reference_count].method=method;reference[reference_count].count=1;reference[reference_count++].last_param=param;}
}
static void verify_unhandled(void)
{
    assert((unsigned)s_unhandled_count==reference_count);
    for(unsigned i=0;i<reference_count;i++) {
        unsigned j;for(j=0;j<(unsigned)s_unhandled_count;j++)if(s_unhandled[j].method==reference[i].method)break;
        assert(j<(unsigned)s_unhandled_count && s_unhandled[j].count==reference[i].count && s_unhandled[j].last_param==reference[i].last_param);
    }
}
int main(void)
{
    setup();
    for(unsigned round=0;round<3;round++) {
        for(unsigned i=0;i<10000;i++){
            uint32_t method=((i*37u+round*163u)%501)*4;
            note_unhandled(method,i);legacy_unhandled(method,i);
        }
        verify_unhandled();nv2a_pb_exec_report();verify_unhandled();
    }
    for(unsigned i=0;i<3000;i++){uint32_t method=i%3==0?0x2000+i:i*4;note_unhandled(method,i);legacy_unhandled(method,i);}
    for(unsigned i=0;i<10000;i++){uint32_t method=i%3==0?0x2000+i:i*4;note_unhandled(method,i+100);legacy_unhandled(method,i+100);}
    verify_unhandled();assert(s_gpu.unhandled_total==43000);
    puts("PASS: unhandled method counts/last values match legacy, including diagnostic sorting and lookup rebuild, malformed keys and capacity saturation");
}
