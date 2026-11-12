/* Game-specific overrides and diagnostics live here as bring-up advances. */

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(_MSC_VER)
#include <intrin.h>
#endif

#include <xbox/xboxrecomp.h>

/* Guest-memory macros used by the stack probe live in this header. */
#include "recomp/gen/recomp_types.h"


extern volatile uint32_t g_icall_trace[16];
extern volatile uint32_t g_icall_trace_idx;
extern volatile uint64_t g_icall_count;
extern RECOMP_TLS uint32_t g_eax, g_ecx, g_edx, g_esp;
extern RECOMP_TLS uint32_t g_ebx, g_esi, g_edi;
extern RECOMP_TLS volatile uint32_t g_recomp_pc;
extern void xbox_KernelProgressDump(void);

/* QPR2 reaches WBINVD after publishing GPU/contiguous-memory state during
 * D3D device setup. A user-mode host cannot execute that privileged
 * instruction; translated guest memory is already coherent, so retain the
 * ordering effect with a full host fence. */
void recomp_memory_fence(uint32_t va)
{
    static int logged;
#if defined(_MSC_VER)
    static volatile long fence_word;
    (void)_InterlockedExchangeAdd(&fence_word, 0);
#elif defined(__GNUC__) || defined(__clang__)
    __sync_synchronize();
#else
    volatile uint32_t fence_word = 0;
    (void)fence_word;
#endif

    if (!logged) {
        logged = 1;
        fprintf(stderr,
                "[CPU] wbinvd at 0x%08X -> host memory fence "
                "(guest memory is coherent)\n",
                va);
        fflush(stderr);
    }
}

/* PFB's write-combine cache flush is independent of FIFO/raster completion.
 * Host guest RAM is coherent. Complete the flush with an atomic full barrier
 * on its own register, without advancing DMA_GET or signalling a GPU fence.
 * Waiting for the renderer thread to poll this bit made every flush wait
 * behind its outstanding software draw/report work. */
void recomp_nv2a_cache_flush(uint32_t register_va)
{
    static unsigned logged;
    if (register_va != 0xFD100410u) return;
#if defined(_MSC_VER)
    (void)_InterlockedAnd((volatile long *)XBOX_PTR(register_va), ~0x10000L);
#elif defined(__GNUC__) || defined(__clang__)
    (void)__sync_fetch_and_and((volatile uint32_t *)XBOX_PTR(register_va), ~0x10000u);
#else
    recomp_memory_fence(register_va);
    MEM32(register_va) &= ~0x10000u;
#endif
    if (!logged++) fprintf(stderr, "[PFB-FLUSH] coherent host cache flush at %08X; DMA_GET and GPU fences unchanged\n", register_va);
}

/* USER DMA_PUT and PFIFO DMA_PUT are views of the same submitted pointer.
 * Publish the PFIFO view before retail's immediate idle checks; completion
 * (both GET views) stays with the executor. */
void recomp_nv2a_submit(uint32_t register_va,uint32_t put)
{
    if(register_va!=0xFD800040u)return;
#if defined(_MSC_VER)
    (void)_InterlockedExchange((volatile long*)XBOX_PTR(0xFD003240u),(long)put);
#elif defined(__GNUC__) || defined(__clang__)
    __atomic_store_n((volatile uint32_t*)XBOX_PTR(0xFD003240u),put,__ATOMIC_SEQ_CST);
#else
    recomp_memory_fence(register_va);MEM32(0xFD003240u)=put;
#endif
}

/* The retail interrupt patch can miss an already-consumed NOP on the host's
 * independent FIFO thread. Keep its event wakeup, but decide completion from
 * the same ordered guest semaphore word that retail checks on entry. */
extern int xbox_KeWaitGuestFence(uint32_t,uint32_t,uint32_t,uint32_t);
int recomp_nv2a_fence_wait(uint32_t device,uint32_t requested)
{
    if(device<0x10000u || device>0x04000000u-0x1968u)return 0;
    return xbox_KeWaitGuestFence(device+0x1964u,device+0x2Cu,
                               MEM32(device+0x30u),requested);
}

/* Raw mesh getter arguments only: the crash handler must not dereference a
 * potentially corrupt mesh/object while explaining an invalid index. */
typedef struct { uint32_t caller, object, section, point, corner, output; } Qpr2MeshArgs;
static RECOMP_TLS Qpr2MeshArgs s_mesh_args[32];
static RECOMP_TLS uint32_t s_mesh_next, s_mesh_count;
void qpr2_mesh_args_record(uint32_t caller,uint32_t object,uint32_t section,
                          uint32_t point,uint32_t corner,uint32_t output)
{
    static int enabled=-1;
    if(enabled<0)enabled=getenv("RECOMP_MESH_TRACE")!=NULL;
    if(!enabled)return;
    Qpr2MeshArgs *slot=&s_mesh_args[s_mesh_next];
    slot->caller=caller;slot->object=object;slot->section=section;
    slot->point=point;slot->corner=corner;slot->output=output;
    s_mesh_next=(s_mesh_next+1)&31u;if(s_mesh_count<32)++s_mesh_count;
}
void qpr2_mesh_args_dump(void)
{
    fprintf(stderr,"[MESH-ARGS] last %u vertex getter calls (oldest first)\n",s_mesh_count);
    for(uint32_t i=0;i<s_mesh_count;i++) {
        const Qpr2MeshArgs *a=&s_mesh_args[(s_mesh_next+32-s_mesh_count+i)&31u];
        fprintf(stderr,"  caller=%08X this=%08X section=%08X point=%08X corner=%08X output=%08X\n",
                a->caller,a->object,a->section,a->point,a->corner,a->output);
    }
}

/* Minimal shadow for the one direct I/O register QPR2's Xbox D3D runtime uses
 * during device setup and vertical-status bookkeeping. */
static uint8_t g_qpr2_io_port_80c0;

#define QPR2_CALLBACK(address, level) extern void qpr2_recovered_##address(void);
#include "qpr2_callbacks.def"
#undef QPR2_CALLBACK

recomp_func_t recomp_lookup_manual(uint32_t xbox_va)
{
    switch (xbox_va) {
#define QPR2_CALLBACK(address, level) case 0x##address##u: return qpr2_recovered_##address;
#include "qpr2_callbacks.def"
#undef QPR2_CALLBACK
    default: return NULL;
    }
}

int qpr2_validate_level_callbacks(void)
{
    unsigned resolved = 0;
    unsigned level_resolved = 0;
    unsigned total = 0;
#define QPR2_CALLBACK(address, level) \
    do { \
        unsigned valid = recomp_lookup_manual(0x##address##u) == qpr2_recovered_##address; \
        resolved += valid; \
        level_resolved += (level) * valid; \
        ++total; \
    } while (0);
#include "qpr2_callbacks.def"
#undef QPR2_CALLBACK
    fprintf(stderr, "[CALLBACK-CHECK] resolved=%u/%u level_callbacks=%u/52\n",
            resolved, total, level_resolved);
    return resolved == total && level_resolved == 52;
}

/* Direct stubs bypass indirect-call failure reporting. Observe their first
 * two uses per thread without reading guest pointers or changing registers. */
void recomp_direct_stub_log(uint32_t va)
{
    fprintf(stderr,"[DIRECT-STUB] target=%08X caller_pc=%08X esp=%08X eax=%08X ecx=%08X edx=%08X\n",
        va,(uint32_t)g_recomp_pc,g_esp,g_eax,g_ecx,g_edx);
}

void recomp_icall_fail_log(uint32_t va)
{
    int i;
    static uint64_t xpp_completion_skips;

    /* The rollback still stalls with this callback absent. Preserve
     * V19's stack-safe unresolved-call behavior while recording the live ABI
     * on the first few calls. Throttle later messages: V19 emitted more than
     * 5,000 identical lines in five minutes, which added avoidable I/O to the
     * already slow software-rendering run. */
    if (va == 0x001A7D43u) {
        uint64_t n = ++xpp_completion_skips;

        if (n <= 8) {
            fprintf(stderr,
                    "[XPP-PROBE] skipped 0x001A7D43 #%llu pc=0x%08X "
                    "esp=0x%08X eax=%08X ecx=%08X edx=%08X "
                    "ebx=%08X esi=%08X edi=%08X\n",
                    (unsigned long long)n, (uint32_t)g_recomp_pc, g_esp,
                    g_eax, g_ecx, g_edx, g_ebx, g_esi, g_edi);
            fprintf(stderr, "  stack:");
            for (i = 0; i < 16; ++i)
                fprintf(stderr, " %08X", MEM32(g_esp + (uint32_t)i * 4u));
            fputc('\n', stderr);
            fflush(stderr);
        }
        else if ((n & (n - 1)) == 0) {
            fprintf(stderr,
                    "[XPP-PROBE] skipped 0x001A7D43 %llu times\n",
                    (unsigned long long)n);
            fflush(stderr);
        }
        return;
    }

    fprintf(stderr,
            "[ICALL] unresolved target 0x%08X at call #%llu\n",
            va, (unsigned long long)g_icall_count);
    fprintf(stderr, "  recent targets:");
    for (i = 0; i < 16; ++i) {
        uint32_t index = (g_icall_trace_idx + (uint32_t)i) & 15u;
        fprintf(stderr, " %08X", g_icall_trace[index]);
    }
    fputc('\n', stderr);
    fprintf(stderr,"  caller_pc=%08X esp=%08X eax=%08X ecx=%08X edx=%08X ebx=%08X esi=%08X edi=%08X\n",
        (uint32_t)g_recomp_pc,g_esp,g_eax,g_ecx,g_edx,g_ebx,g_esi,g_edi);
    fflush(stderr);
}

void recomp_icall_not_code_log(uint32_t va)
{
    static uint32_t last_va;
    static uint64_t repeated;

    if (last_va != va) {
        last_va = va;
        repeated = 0;
    }
    ++repeated;
    if (repeated == 1 || repeated == 10 || repeated == 100 ||
        repeated == 1000 || repeated == 10000) {
        fprintf(stderr,
                "[ICALL] 0x%08X is not code; skipped %llu time(s) "
                "pc=%08X esp=%08X eax=%08X ecx=%08X edx=%08X\n",
                va, (unsigned long long)repeated, (uint32_t)g_recomp_pc,
                g_esp, g_eax, g_ecx, g_edx);
        fflush(stderr);
    }
}

void recomp_unimpl(const char *text, uint32_t va)
{
    static unsigned printed;
    static unsigned xpp_interrupt_guard_logged;
    const char *trap = getenv("RECOMP_UNIMPL_TRAP");
    int should_trap = trap && *trap && *trap != '0';

    /* QPR2's XPP interrupt handler brackets one MCPX status read with CLI/STI.
     * Guest interrupts are delivered synchronously by the host runtime, so
     * there is no host IF bit to change here.  Ignore only this verified pair;
     * all other privileged instructions remain trap-capable diagnostics. */
    if ((va == 0x001ABB3Du && strcmp(text, "cli") == 0)
            || (va == 0x001ABB4Eu && strcmp(text, "sti") == 0)) {
        unsigned bit = va == 0x001ABB3Du ? 1u : 2u;
        if (!(xpp_interrupt_guard_logged & bit)) {
            xpp_interrupt_guard_logged |= bit;
            fprintf(stderr,
                    "[CPU] XPP %s at 0x%08X -> synchronous interrupt guard "
                    "no-op\n", text, va);
            fflush(stderr);
        }
        return;
    }

    if (printed < 50 || should_trap) {
        ++printed;
        fprintf(stderr,
                "[UNIMPL] `%s` reached at guest VA 0x%08X%s\n",
                text, va, should_trap ? " (trapping)" : "");
        fflush(stderr);
    }
    if (should_trap) {
        xbox_KernelProgressDump();
        abort();
    }
}

/* QPR2 uses this byte port while bringing up the Xbox D3D/NV2A path and later
 * reads it for status bookkeeping. The host renderer owns display hardware
 * state, so preserve coherent software-visible state in a shadow register.
 * Every other direct port remains a loud, trap-capable blocker. */
void recomp_io_out8(uint16_t port, uint8_t value, uint32_t va)
{
    static int logged_80c0_write;

    if (port == 0x80C0u) {
        g_qpr2_io_port_80c0 = value;
        if (!logged_80c0_write) {
            logged_80c0_write = 1;
            fprintf(stderr,
                    "[IO] out8 port=0x80C0 value=0x%02X at 0x%08X "
                    "(host display shadow)\n", value, va);
            fflush(stderr);
        }
        return;
    }

    fprintf(stderr,
            "[IO] unsupported out8 port=0x%04X value=0x%02X at 0x%08X\n",
            port, value, va);
    fflush(stderr);
    recomp_unimpl("out8 to unsupported I/O port", va);
}

void recomp_io_out16(uint16_t port, uint16_t value, uint32_t va)
{
    fprintf(stderr,
            "[IO] unsupported out16 port=0x%04X value=0x%04X at 0x%08X\n",
            port, value, va);
    fflush(stderr);
    recomp_unimpl("out16 to unsupported I/O port", va);
}

void recomp_io_out32(uint16_t port, uint32_t value, uint32_t va)
{
    fprintf(stderr,
            "[IO] unsupported out32 port=0x%04X value=0x%08X at 0x%08X\n",
            port, value, va);
    fflush(stderr);
    recomp_unimpl("out32 to unsupported I/O port", va);
}

uint8_t recomp_io_in8(uint16_t port, uint32_t va)
{
    static int logged_80c0_read;

    if (port == 0x80C0u) {
        if (!logged_80c0_read) {
            logged_80c0_read = 1;
            fprintf(stderr,
                    "[IO] in8 port=0x80C0 -> 0x%02X at 0x%08X "
                    "(host display shadow)\n",
                    g_qpr2_io_port_80c0, va);
            fflush(stderr);
        }
        return g_qpr2_io_port_80c0;
    }

    fprintf(stderr, "[IO] unsupported in8 port=0x%04X at 0x%08X\n",
            port, va);
    fflush(stderr);
    recomp_unimpl("in8 from unsupported I/O port", va);
    return 0;
}

uint16_t recomp_io_in16(uint16_t port, uint32_t va)
{
    fprintf(stderr, "[IO] unsupported in16 port=0x%04X at 0x%08X\n",
            port, va);
    fflush(stderr);
    recomp_unimpl("in16 from unsupported I/O port", va);
    return 0;
}

uint32_t recomp_io_in32(uint16_t port, uint32_t va)
{
    fprintf(stderr, "[IO] unsupported in32 port=0x%04X at 0x%08X\n",
            port, va);
    fflush(stderr);
    recomp_unimpl("in32 from unsupported I/O port", va);
    return 0;
}

