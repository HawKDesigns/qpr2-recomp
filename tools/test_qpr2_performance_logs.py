"""Exercise production buffering, quiet timings and completed-frame accounting.

The slow sink simulates write latency; it is not a Windows/gameplay benchmark.
"""
from pathlib import Path
import os
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
fixture = r'''
#define _GNU_SOURCE
#include "qpr2_log_buffer.h"
#include "nv2a_frame_pacing.h"
#include <assert.h>
#include <string.h>
#include <time.h>
typedef struct { unsigned writes, bytes; char output[120000]; } Sink;
static ssize_t sink_write(void *opaque,const char *data,size_t size) {
    Sink *s=opaque;++s->writes;
    assert(size<=sizeof s->output-s->bytes);
    memcpy(s->output+s->bytes,data,size);s->bytes+=(unsigned)size;
    /* Small write latency models a redirected diagnostic destination. */
    struct timespec latency={0,200000};nanosleep(&latency,NULL);
    return (ssize_t)size;
}
static double wall_ms(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec*1000.0+t.tv_nsec/1e6;}
int main(void) {
    Sink sinks[2]={0};
    double elapsed[2];
    for(unsigned buffered=0;buffered<2;buffered++) {
        setenv("RECOMP_LOG_UNBUFFERED",buffered?"0":"1",1);
        cookie_io_functions_t io={0};io.write=sink_write;
        FILE *f=fopencookie(&sinks[buffered],"w",io);assert(f);
        assert(qpr2_log_buffer_init(f)==(int)buffered);
        double begin=wall_ms();
        for(unsigned i=0;i<2048;i++)fprintf(f,"[GAME-TIME] calls=%u wall_ms=0.001 this=0178ACBC\n",i);
        /* Periodic reports/fault paths flush explicitly. No bytes lost. */
        assert(!fflush(f));elapsed[buffered]=wall_ms()-begin;assert(!fclose(f));
    }
    assert(sinks[0].bytes==sinks[1].bytes && !memcmp(sinks[0].output,sinks[1].output,sinks[0].bytes));
    assert(sinks[0].writes==2048 && sinks[1].writes<=4);
    printf("PASS: production stderr policy: %u -> %u sink writes; %u identical bytes; simulated latency %.3f -> %.3f ms (not game FPS)\n",
        sinks[0].writes,sinks[1].writes,sinks[0].bytes,elapsed[0],elapsed[1]);
    NvFramePacing p={0};
    nv_frame_pacing_note(&p,0);
    for(unsigned i=1;i<=30;i++)nv_frame_pacing_note(&p,i*(1000.0/30.0));
    assert(p.intervals==30 && !p.over_33ms);
    nv_frame_pacing_note(&p,1034);assert(p.over_33ms==1 && !p.over_50ms);
    memset(&p,0,sizeof p);nv_frame_pacing_note(&p,0);
    for(unsigned i=1;i<=20;i++)nv_frame_pacing_note(&p,i*50.0);
    assert(p.intervals==20 && p.total_ms==1000 && p.max_ms==50 && !p.over_50ms && !p.over_100ms);
    nv_frame_pacing_reset_window(&p);
    assert(p.seeded && p.last_ms==1000 && !p.intervals);
    nv_frame_pacing_note(&p,1040);nv_frame_pacing_note(&p,1100);nv_frame_pacing_note(&p,1250);
    assert(p.intervals==3 && p.total_ms==250 && p.max_ms==150 && p.over_50ms==2 && p.over_100ms==1);
    /* Report boundaries include all elapsed time; backward clocks re-seed. */
    nv_frame_pacing_note(&p,1200);assert(p.intervals==3);
    nv_frame_pacing_note(&p,1250);assert(p.intervals==4 && p.total_ms==300);
    puts("PASS: 30/20 FPS thresholds, real interval counts, hitches, window continuity, backward-clock recovery; observations do not change guest time");
}
'''
with tempfile.TemporaryDirectory(prefix="qpr2-perf-logs-") as folder:
    tmp = Path(folder)
    (tmp / "test.c").write_text(fixture)
    includes = ["-I" + str(root / "src"), "-I" + str(root / "third_party/xboxrecomp/src/kernel")]
    flags = ["gcc", "-std=gnu11", "-O2", "-fsanitize=undefined", "-fno-sanitize-recover=all"]
    subprocess.run([*flags, *includes, str(tmp / "test.c"), "-o", str(tmp / "test")], check=True)
    subprocess.run([str(tmp / "test")], check=True)
    exe = tmp / "timing"
    subprocess.run([*flags, "-ffunction-sections", "-fdata-sections", str(root / "tools/test_qpr2_game_timing.c"), "-Wl,--gc-sections", "-lm", "-o", str(exe)], check=True)
    for verbose in (False, True):
        env = dict(os.environ, RECOMP_GAME_TIMING_VERBOSE="1" if verbose else "0")
        run = subprocess.run([str(exe)], env=env, check=True, capture_output=True, text=True)
        assert "[GAME-TOTAL]" in run.stderr
        assert ("[GAME-TIME]" in run.stderr) == verbose
        assert ("[GAME-BEGIN]" in run.stderr) == verbose
        print("PASS: actual game timing helper retains aggregates; verbose=" + str(int(verbose)))
    run = subprocess.run([str(exe), "disabled"], check=True, capture_output=True, text=True)
    assert not run.stderr
    print("PASS: disabled game timing produces no diagnostic output")
