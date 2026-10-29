/* Bounded read-only observations. No guest register or memory writes. */
#include "qpr2_game_timing.h"
#include "recomp/gen/recomp_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#if defined(_WIN32)
#include <windows.h>
#define QPR2_TIMING_TLS __declspec(thread)
#else
#define QPR2_TIMING_TLS __thread
#endif
#define QPR2_TIMING_SLOTS 16
static QPR2_TIMING_TLS struct {
    uint32_t function;
    uint64_t calls, slow;
    double peak_ms;
    double window_started,window_ms,window_peak;
    uint64_t window_calls;
} s_timing[QPR2_TIMING_SLOTS];
static QPR2_TIMING_TLS int s_enabled = -1, s_verbose = -1;
static QPR2_TIMING_TLS double s_origin;
static double timing_wall_ms(void)
{
#if defined(_WIN32)
    static QPR2_TIMING_TLS LARGE_INTEGER frequency;
    LARGE_INTEGER now;
    if (!frequency.QuadPart) QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&now);
    return now.QuadPart * 1000.0 / frequency.QuadPart;
#else
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec * 1000.0 + now.tv_nsec / 1e6;
#endif
}
Qpr2GameTiming qpr2_game_timing_begin(uint32_t function, uint32_t object,
                                     uint32_t argument, uint32_t auxiliary)
{
    Qpr2GameTiming token = {0};
    unsigned i;
    if (s_enabled < 0) {
        const char *enabled = getenv("RECOMP_GAME_TIMING");
        s_enabled = enabled && *enabled && *enabled != '0';
    }
    if (s_verbose < 0) {
        const char *verbose = getenv("RECOMP_GAME_TIMING_VERBOSE");
        s_verbose = verbose && *verbose && *verbose != '0';
    }
    if (!s_enabled) return token;
    for (i = 0; i < QPR2_TIMING_SLOTS; ++i)
        if (!s_timing[i].function || s_timing[i].function == function) break;
    if (i == QPR2_TIMING_SLOTS) return token;
    s_timing[i].function = function;
    token.started = timing_wall_ms();
    if (!s_origin) s_origin = token.started;
    token.function = function; token.object = object;
    token.argument = argument; token.auxiliary = auxiliary; token.slot = i;
    ++s_timing[i].calls;
    if (s_verbose && s_timing[i].calls <= 2)
        fprintf(stderr, "[GAME-BEGIN] fn=%08X t_ms=%.3f this=%08X arg=%08X aux=%u\n",
            function, token.started - s_origin, object, argument, auxiliary);
    return token;
}
void qpr2_game_timing_end(const Qpr2GameTiming *token)
{
    double elapsed;
    unsigned i;
    uint64_t calls;
    float argument;
    if (!token->function) return;
    i = token->slot;
    elapsed = timing_wall_ms() - token->started;
    if(!s_timing[i].window_started)s_timing[i].window_started=token->started;
    s_timing[i].window_ms+=elapsed;++s_timing[i].window_calls;
    if(elapsed>s_timing[i].window_peak)s_timing[i].window_peak=elapsed;
    if(token->started+elapsed-s_timing[i].window_started>=5000.0) {
        fprintf(stderr,"[GAME-TOTAL] fn=%08X window_ms=%.3f calls=%llu inclusive_ms=%.3f mean_ms=%.6f peak_ms=%.3f window_start_host_ms=%.3f window_end_host_ms=%.3f\n",
            token->function,token->started+elapsed-s_timing[i].window_started,
            (unsigned long long)s_timing[i].window_calls,s_timing[i].window_ms,
            s_timing[i].window_ms/s_timing[i].window_calls,s_timing[i].window_peak,
            s_timing[i].window_started,token->started+elapsed);
        s_timing[i].window_started=token->started+elapsed;
        s_timing[i].window_calls=0;s_timing[i].window_ms=s_timing[i].window_peak=0;
    }
    if (elapsed > s_timing[i].peak_ms) s_timing[i].peak_ms = elapsed;
    calls = s_timing[i].calls;
    if (elapsed >= 50) ++s_timing[i].slow;
    if (s_verbose && (calls <= 2 || (calls % 128 == 0) ||
            (elapsed >= 50 && (s_timing[i].slow <= 32 ||
                              (s_timing[i].slow & (s_timing[i].slow - 1)) == 0)))) {
        memcpy(&argument, &token->argument, sizeof argument);
        fprintf(stderr, "[GAME-TIME] fn=%08X t_ms=%.3f calls=%llu wall_ms=%.3f peak_ms=%.3f "
            "this=%08X arg=%08X arg_float=%.9g aux=%u\n",
            token->function, token->started - s_origin, (unsigned long long)calls,
            elapsed, s_timing[i].peak_ms, token->object, token->argument,
            argument, token->auxiliary);
    }
}

/* Positions are passed from fields already used by the retail camera update.
 * This helper never dereferences guest pointers or alters the camera clock. */
void qpr2_camera_state(const Qpr2GameTiming *token,
                       float px,float py,float pz,float x,float y,float z)
{
    float dt;
    uint64_t calls;
    if(!token->function || s_verbose<=0)return;
    calls=s_timing[token->slot].calls;
    if(calls>8 && calls%32!=0)return;
    memcpy(&dt,&token->argument,sizeof dt);
    fprintf(stderr,"[CAMERA-STATE] this=%08X calls=%llu dt=%.9g previous=(%.9g %.9g %.9g) current=(%.9g %.9g %.9g)\n",
        token->object,(unsigned long long)calls,dt,px,py,pz,x,y,z);
}

/* The preview update dispatches animation nodes through 80995. Record the
 * live node/handler rather than inferring a shot from the chase camera alone.
 * Read-only and bounded: no change to guest time, state, registers or stack. */
void qpr2_camera_node_probe(uint32_t node,uint32_t callback,uint32_t dt)
{
    static QPR2_TIMING_TLS uint32_t targets[16],count;
    static QPR2_TIMING_TLS double last;
    double now;
    int fresh=1;
    float step;
    if(s_enabled<0){const char *e=getenv("RECOMP_GAME_TIMING");s_enabled=e&&*e&&*e!='0';}
    if(s_verbose<0){const char *e=getenv("RECOMP_GAME_TIMING_VERBOSE");s_verbose=e&&*e&&*e!='0';}
    if(!s_enabled || !s_verbose || !node)return;
    now=timing_wall_ms();
    for(uint32_t i=0;i<count;i++)if(targets[i]==callback)fresh=0;
    if(fresh && count<16)targets[count++]=callback;
    else if(now-last<2000)return;
#if defined(_WIN32)
    MEMORY_BASIC_INFORMATION info;
    uintptr_t native=XBOX_PTR(node);
    if(!VirtualQuery((void*)native,&info,sizeof info) || info.State!=MEM_COMMIT ||
       (info.Protect&(PAGE_GUARD|PAGE_NOACCESS)) ||
       native+0x30>(uintptr_t)info.BaseAddress+info.RegionSize)return;
#endif
    last=now;memcpy(&step,&dt,4);
    fprintf(stderr,"[CAMERA-NODE] node=%08X vtable=%08X callback=%08X dt=%.9g next=%08X words=",
        node,MEM32(node),callback,step,MEM32(node+8));
    for(uint32_t off=0xC;off<0x30;off+=4)fprintf(stderr," %08X",MEM32(node+off));
    fputc('\n',stderr);
}
