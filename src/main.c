extern void qpr2_mesh_args_dump(void);
#include "qpr2_ui_trace.h"
#include "qpr2_log_buffer.h"
/*
 * ATV: Quad Power Racing 2 static-recompilation host.
 *
 * This contains host/runtime glue only. Game code is generated locally from
 * a user's legally owned default.xbe and is intentionally not distributed.
 */

#if defined(_WIN32)
#include <windows.h>
#include <dbghelp.h>
#else
/* POSIX hosts (Linux first): the runtime's SDL window owns the process main
 * thread and the boot sequence below runs on its guest thread. */
#include <signal.h>
#include <ucontext.h>
#include <unistd.h>
#include "video/video_player.h"   /* xbox_HostWindowMain */
#endif
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <xbox/xboxrecomp.h>
#include "qpr2_build_config.h"
#include "qpr2_display.h"
#include "qpr2_input_config.h"
#include "platform/frame_trace.h"
#include "platform/thread_cpu.h"

extern RECOMP_TLS uint32_t g_eax, g_ecx, g_edx, g_esp;
extern RECOMP_TLS uint32_t g_ebx, g_esi, g_edi;
extern RECOMP_TLS volatile uint32_t g_recomp_pc;
extern ptrdiff_t g_xbox_mem_offset;

extern void xbe_entry_point(void);
extern int qpr2_validate_level_callbacks(void);
#include "nv2a_overlay_revision.h"
extern unsigned nv2a_pb_exec_overlay_revision(void);
extern int recomp_dispatch_init(void);

/* The emulated MCPX APU and its Win32 MMIO fault decoder.  MemoryLayoutInit
 * makes the real Xbox register window inaccessible when RECOMP_AC97_READY is
 * enabled; every access must therefore be routed through this model. */
typedef struct MCPXAPUState MCPXAPUState;
extern MCPXAPUState *mcpx_apu_init_standalone(uint8_t *ram_ptr);
extern void mcpx_apu_shutdown(MCPXAPUState *state);
extern MCPXAPUState *g_apu_state;
#if defined(_WIN32)
extern bool apu_hook_handle_mmio(PCONTEXT context, uintptr_t fault_address,
                                 uint32_t xbox_va, int is_write);
#endif

/* QPR2 talks directly to the MCPX OHCI controllers through its linked XAPI.
 * XboxRecomp's USB model protects those register pages so this host VEH must
 * decode their access violations, just as it does for the APU window. */
extern void xbox_OhciInit(void);
extern int xbox_OhciOwnsAddress(uint32_t xbox_va);
extern int xbox_OhciHandleMmio(void *ctx, uint32_t xbox_va);
extern void xbox_OhciReport(void);

/* MemoryLayoutInit currently traps the APU registers and voice processor,
 * leaving the GP/EP DSP RAM windows above 0xFE830000 directly mapped. */
#define QPR2_APU_TRAP_BASE 0xFE800000u
#define QPR2_APU_TRAP_END  0xFE830000u

static int load_file(const char *path, void **out_data, size_t *out_size)
{
    FILE *file;
    long length;
    void *data;

    file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "[BOOT] Cannot open %s\n", path);
        return 0;
    }
    if (fseek(file, 0, SEEK_END) != 0 || (length = ftell(file)) <= 0 ||
        fseek(file, 0, SEEK_SET) != 0) {
        fclose(file);
        fprintf(stderr, "[BOOT] Cannot determine XBE size\n");
        return 0;
    }

    data = malloc((size_t)length);
    if (!data || fread(data, 1, (size_t)length, file) != (size_t)length) {
        free(data);
        fclose(file);
        fprintf(stderr, "[BOOT] Cannot read complete XBE\n");
        return 0;
    }

    fclose(file);
    *out_data = data;
    *out_size = (size_t)length;
    return 1;
}

static void qpr2_report_guest_registers(void)
{
    fprintf(stderr,
            "  guest regs: eax=%08X ecx=%08X edx=%08X ebx=%08X\n"
            "              esi=%08X edi=%08X esp=%08X\n",
            g_eax, g_ecx, g_edx, g_ebx, g_esi, g_edi, g_esp);
    fprintf(stderr, "  guest pc: %08X\n", (uint32_t)g_recomp_pc);
}

#if defined(_WIN32)

static void describe_host_address(void *address)
{
    char storage[sizeof(SYMBOL_INFO) + 256];
    SYMBOL_INFO *symbol = (SYMBOL_INFO *)storage;
    DWORD64 displacement = 0;

    memset(storage, 0, sizeof(storage));
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    symbol->MaxNameLen = 255;
    if (SymFromAddr(GetCurrentProcess(), (DWORD64)(uintptr_t)address,
                    &displacement, symbol)) {
        fprintf(stderr, "  host symbol: %s+0x%llX\n", symbol->Name,
                (unsigned long long)displacement);
    }
}

static void qpr2_crash_stack_dump(void)
{
    unsigned i;
    fprintf(stderr, "[GUEST-STACK] from=%08X\n", g_esp);
    for (i = 0; i < 32; ++i) {
        uint32_t va = g_esp + i * 4u;
        uintptr_t native = (uintptr_t)va + g_xbox_mem_offset;
        MEMORY_BASIC_INFORMATION info;
        if (!VirtualQuery((const void *)native, &info, sizeof(info)) ||
            info.State != MEM_COMMIT || (info.Protect & (PAGE_GUARD | PAGE_NOACCESS)) ||
            native + 4u > (uintptr_t)info.BaseAddress + info.RegionSize) break;
        fprintf(stderr, "  %08X: %08X\n", va, *(const volatile uint32_t *)native);
    }
}

static LONG CALLBACK qpr2_exception_handler(PEXCEPTION_POINTERS ep)
{
    DWORD code = ep->ExceptionRecord->ExceptionCode;

    if (code == EXCEPTION_ACCESS_VIOLATION) {
        uintptr_t fault = ep->ExceptionRecord->ExceptionInformation[1];
        const char *operation =
            ep->ExceptionRecord->ExceptionInformation[0] ? "write" : "read";

        /* Translate the host fault back to the Xbox VA before reporting it as
         * a crash.  A handled access advances RIP past the native load/store,
         * exactly as if the register had been ordinary memory. */
        if (g_xbox_mem_offset) {
            uint32_t xbox_va =
                (uint32_t)(fault - (uintptr_t)g_xbox_mem_offset);
            if (xbox_OhciOwnsAddress(xbox_va)
                    && xbox_OhciHandleMmio(ep->ContextRecord, xbox_va)) {
                return EXCEPTION_CONTINUE_EXECUTION;
            }
            if (xbox_va >= QPR2_APU_TRAP_BASE
                    && xbox_va < QPR2_APU_TRAP_END
                    && apu_hook_handle_mmio(
                           ep->ContextRecord, fault, xbox_va,
                           ep->ExceptionRecord->ExceptionInformation[0]
                               ? 1 : 0)) {
                return EXCEPTION_CONTINUE_EXECUTION;
            }
        }

        fprintf(stderr,
                "[CRASH] access violation: %s at host=0x%llX fault=0x%llX\n",
                operation,
                (unsigned long long)ep->ContextRecord->Rip,
                (unsigned long long)fault);
        qpr2_report_guest_registers();
        if (g_xbox_mem_offset) {
            fprintf(stderr, "  possible guest VA: %08X\n",
                    (uint32_t)(fault - (uintptr_t)g_xbox_mem_offset));
        }
        qpr2_mesh_args_dump();
        qpr2_ui_trace_dump();
        qpr2_crash_stack_dump();
        describe_host_address((void *)ep->ContextRecord->Rip);
        fflush(stderr);
    } else if (code == EXCEPTION_INT_DIVIDE_BY_ZERO) {
        fprintf(stderr, "[CRASH] integer divide by zero at host=0x%llX\n",
                (unsigned long long)ep->ContextRecord->Rip);
        qpr2_report_guest_registers();
        qpr2_crash_stack_dump();
        describe_host_address((void *)ep->ContextRecord->Rip);
        fflush(stderr);
    }

    return EXCEPTION_CONTINUE_SEARCH;
}

static void qpr2_install_fault_handler(void)
{
    SymSetOptions(SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME);
    SymInitialize(GetCurrentProcess(), NULL, TRUE);
    AddVectoredExceptionHandler(1, qpr2_exception_handler);
}

static void qpr2_fatal_message(const char *text)
{
    MessageBoxA(NULL, text, "QPR2 recomp", MB_ICONERROR);
}

#else /* POSIX */

/* The guest main stack is one fixed arena, so the crash dump can bound its
 * reads without a VirtualQuery equivalent. Matches the loader's
 * "Stack: 8192 KB at Xbox VA 0x00780000" line. */
#define QPR2_STACK_ARENA_BASE 0x00780000u
#define QPR2_STACK_ARENA_END  0x00F80000u

static void qpr2_crash_stack_dump(void)
{
    unsigned i;
    fprintf(stderr, "[GUEST-STACK] from=%08X\n", g_esp);
    for (i = 0; i < 32; ++i) {
        uint32_t va = g_esp + i * 4u;
        if (va < QPR2_STACK_ARENA_BASE || va + 4u > QPR2_STACK_ARENA_END) break;
        fprintf(stderr, "  %08X: %08X\n", va,
                *(const volatile uint32_t *)((uintptr_t)va + g_xbox_mem_offset));
    }
}

/* SIGSEGV/SIGBUS: trapped register windows (APU, AC'97, OHCI) are emulated by
 * the runtime and the thread resumes. Anything else is a crash: report the
 * same context as the Windows handler, then let the default action run. */
static void qpr2_posix_fault_handler(int sig, siginfo_t *info, void *ucv)
{
    uintptr_t fault = (uintptr_t)info->si_addr;
    struct sigaction dfl;

    if (xbox_PosixMmioFault(ucv, fault))
        return;

    fprintf(stderr, "[CRASH] %s at fault=0x%llX\n",
            sig == SIGBUS ? "bus error" : "access violation",
            (unsigned long long)fault);
    qpr2_report_guest_registers();
    if (g_xbox_mem_offset)
        fprintf(stderr, "  possible guest VA: %08X\n",
                (uint32_t)(fault - (uintptr_t)g_xbox_mem_offset));
    qpr2_mesh_args_dump();
    qpr2_ui_trace_dump();
    qpr2_crash_stack_dump();
    fflush(stderr);

    memset(&dfl, 0, sizeof(dfl));
    dfl.sa_handler = SIG_DFL;
    sigaction(sig, &dfl, NULL);   /* returning re-raises under the default */
}

static void qpr2_install_fault_handler(void)
{
    /* A fault can arrive on any guest thread, including one whose own stack
     * is what overflowed, so handle it on a dedicated stack. */
    static uint8_t alt_stack[64 * 1024];
    stack_t ss;
    struct sigaction sa;

    memset(&ss, 0, sizeof(ss));
    ss.ss_sp = alt_stack;
    ss.ss_size = sizeof(alt_stack);
    sigaltstack(&ss, NULL);
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = qpr2_posix_fault_handler;
    sa.sa_flags = SA_SIGINFO | SA_ONSTACK | SA_NODEFER;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGSEGV, &sa, NULL);
    sigaction(SIGBUS, &sa, NULL);
}

static void qpr2_fatal_message(const char *text)
{
    fprintf(stderr, "[BOOT] %s\n", text);
}

#endif /* _WIN32 */

/* The boot sequence shared by every host. On Windows it runs on the process
 * main thread; on POSIX hosts xbox_HostWindowMain runs it on the guest thread
 * while the window keeps the main thread. */
static int qpr2_guest_main(void)
{
    void *xbe_data = NULL;
    size_t xbe_size = 0;

    setvbuf(stdout, NULL, _IONBF, 0);
    int log_buffered = qpr2_log_buffer_init(stderr);
    fprintf(stderr, "[LOG-POLICY] buffered=%d capacity=65536 periodic_flush=renderer-report\n", log_buffered);

    printf("=== ATV: Quad Power Racing 2 recomp bring-up ===\n");
    printf("[BOOT] QPR2 clean v1 on XboxRecomp v0.17.0\n");
    unsigned renderer_revision = nv2a_pb_exec_overlay_revision();
    fprintf(stderr, "[RUNTIME-CHECK] renderer_abi=%u float_depth=1 swizzled_targets=1 texture_sizes=1\n",
            renderer_revision);
    if (renderer_revision != QPR2_RENDERER_REVISION || !qpr2_validate_level_callbacks()) {
        fprintf(stderr, "[BOOT] Renderer or callback verification failed; stopping before guest startup.\n");
        return 4;
    }
    qpr2_input_config_init();
    printf("Title ID: %s | entry: 0x%08X | XBE SHA-256: %s\n",
           QPR2_TITLE_ID_TEXT, QPR2_ENTRY_POINT, QPR2_XBE_SHA256);

    qpr2_install_fault_handler();

    if (!load_file(QPR2_XBE_PATH, &xbe_data, &xbe_size)) {
        qpr2_fatal_message("default.xbe was not found. Extract your legally owned "
                           "disc to the game folder first.");
        return 1;
    }

    if (!xbox_MemoryLayoutInit(xbe_data, xbe_size)) {
        fprintf(stderr, "[BOOT] xbox_MemoryLayoutInit failed\n");
        free(xbe_data);
        return 2;
    }

    g_xbox_mem_offset = xbox_GetMemoryOffset();
    if (getenv("RECOMP_AC97_READY")) {
        g_apu_state =
            mcpx_apu_init_standalone((uint8_t *)xbox_GetMemoryBase());
        fprintf(stderr, "[BOOT] emulated APU %s\n",
                g_apu_state ? "up" : "FAILED to initialise");
        if (!g_apu_state) {
            xbox_MemoryLayoutShutdown();
            free(xbe_data);
            return 3;
        }
    }
    xbox_kernel_init();
    {
        extern void xbox_path_init(const char *game_dir, const char *save_dir);
        xbox_path_init(QPR2_GAME_DIR, QPR2_SAVE_DIR);
    }
    xbox_kernel_bridge_init();
    xbox_OhciInit();
    /* Miniport is embedded at device+17C0, so event +1A4 is device+1964.
     * Deliver the exact NOP 5 completion in executor order, not on a timer. */
    if(xbox_Nv2aSoftwareEvent(0x001A50E8u,0x1964u,5u)!=0)
        fprintf(stderr,"[BOOT] failed to register QPR2 GPU completion event\n");
    /* Consume the exact ring and release its notification DMA object only
     * at BACK_END_WRITE_SEMAPHORE_RELEASE, after preceding raster commands. */
    if(xbox_Nv2aPushBufferRing(0x001A50E8u,0x24u,0x28u)!=0 ||
       xbox_Nv2aCommandFence(0x001A50E8u,0x30u,8u,0x20u)!=0)
        fprintf(stderr,"[BOOT] failed to register QPR2 FIFO/fence state\n");
    /* QPR2's XMV player reads the D3D device's vblank/swap count at +0x1980
     * to pace decoded frames.  The retail ISR is the only writer on hardware;
     * register the exact field so the runtime supplies the missing clock. */
    if (xbox_Nv2aFrameCounter(0x001A50E8u, 0x1980u) != 0)
        fprintf(stderr, "[BOOT] failed to register QPR2 XMV frame counter\n");
    if(xbox_Nv2aVblankEvent(0x001A50E8u,0x1954u)!=0)
        fprintf(stderr,"[BOOT] failed to register QPR2 vblank event\n");
    g_esp = XBOX_STACK_TOP;

    if (!recomp_dispatch_init()) {
        fprintf(stderr,
                "[BOOT] flat dispatch unavailable; using binary-search dispatch\n");
    }
    xbox_frame_trace_thread(XBOX_TRACE_GUEST_MAIN);
#if defined(RECOMP_ENABLE_GUEST_PC)
    xbox_frame_trace_main_pc(&g_recomp_pc);
#endif
#if defined(_WIN32)
    wchar_t *trace_path = _wgetenv(L"QPR2_FRAME_TRACE_PATH");
    int trace_started = trace_path && *trace_path && xbox_frame_trace_start(trace_path);
#else
    const char *trace_path = getenv("QPR2_FRAME_TRACE_PATH");
    int trace_started = trace_path && *trace_path && xbox_frame_trace_start_utf8(trace_path);
#endif
    if (trace_path && *trace_path) {
        if (trace_started)
            xbox_frame_trace_note(XBOX_TRACE_TITLE, XBOX_TRACE_PHASE, 0, XBOX_PHASE_BOOT, 0, 0);
        else
            fprintf(stderr, "[FRAME-TRACE] could not start recorder\n");
    }
    xbox_WatchdogStart();
    qpr2_display_start();
    /* The thread that runs the title's main loop, for the [THREAD-CPU] report. */
    xbox_ThreadCpuRegister("guest-main");

    printf("[BOOT] runtime initialized; entering recompiled code\n");
    xbe_entry_point();
    printf("[BOOT] guest returned\n");
    xbox_frame_trace_stop();

    xbox_OhciReport();
    if (g_apu_state) {
        mcpx_apu_shutdown(g_apu_state);
        g_apu_state = NULL;
    }
    xbox_kernel_shutdown();
    xbox_MemoryLayoutShutdown();
    free(xbe_data);
    return 0;
}

#if defined(_WIN32)
int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous,
                   LPSTR command_line, int show)
{
    (void)instance;
    (void)previous;
    (void)command_line;
    (void)show;
    return qpr2_guest_main();
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOW);
}
#else
int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    /* Opens the SDL window when RECOMP_FB_WINDOW is set and a display exists;
     * otherwise (or with RECOMP_HEADLESS=1) the guest runs on this thread. */
    return xbox_HostWindowMain(qpr2_guest_main);
}
#endif

