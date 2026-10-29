extern void qpr2_mesh_args_dump(void);
#include "qpr2_ui_trace.h"
#include "qpr2_log_buffer.h"
/*
 * ATV: Quad Power Racing 2 static-recompilation host.
 *
 * This contains host/runtime glue only. Game code is generated locally from
 * a user's legally owned default.xbe and is intentionally not distributed.
 */

#include <windows.h>
#include <dbghelp.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <xbox/xboxrecomp.h>
#include "qpr2_build_config.h"

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
extern bool apu_hook_handle_mmio(PCONTEXT context, uintptr_t fault_address,
                                 uint32_t xbox_va, int is_write);

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

static BOOL load_file(const char *path, void **out_data, size_t *out_size)
{
    FILE *file;
    long length;
    void *data;

    file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "[BOOT] Cannot open %s\n", path);
        return FALSE;
    }
    if (fseek(file, 0, SEEK_END) != 0 || (length = ftell(file)) <= 0 ||
        fseek(file, 0, SEEK_SET) != 0) {
        fclose(file);
        fprintf(stderr, "[BOOT] Cannot determine XBE size\n");
        return FALSE;
    }

    data = malloc((size_t)length);
    if (!data || fread(data, 1, (size_t)length, file) != (size_t)length) {
        free(data);
        fclose(file);
        fprintf(stderr, "[BOOT] Cannot read complete XBE\n");
        return FALSE;
    }

    fclose(file);
    *out_data = data;
    *out_size = (size_t)length;
    return TRUE;
}

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
        fprintf(stderr,
                "  guest regs: eax=%08X ecx=%08X edx=%08X ebx=%08X\n"
                "              esi=%08X edi=%08X esp=%08X\n",
                g_eax, g_ecx, g_edx, g_ebx, g_esi, g_edi, g_esp);
        fprintf(stderr, "  guest pc: %08X\n", (uint32_t)g_recomp_pc);
        if (g_xbox_mem_offset) {
            fprintf(stderr, "  possible guest VA: %08X\n",
                    (uint32_t)(fault - (uintptr_t)g_xbox_mem_offset));
        }
        qpr2_mesh_args_dump();
        qpr2_ui_trace_dump();
        qpr2_crash_stack_dump();
        describe_host_address((void *)ep->ContextRecord->Rip);
        fflush(stderr);
    }

    return EXCEPTION_CONTINUE_SEARCH;
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous,
                   LPSTR command_line, int show)
{
    void *xbe_data = NULL;
    size_t xbe_size = 0;

    (void)instance;
    (void)previous;
    (void)command_line;
    (void)show;

    setvbuf(stdout, NULL, _IONBF, 0);
    int log_buffered = qpr2_log_buffer_init(stderr);
    fprintf(stderr, "[LOG-POLICY] buffered=%d capacity=65536 periodic_flush=renderer-report\n", log_buffered);

    printf("=== ATV: Quad Power Racing 2 recomp bring-up ===\n");
    printf("[BOOT] diagnostic overlay v126 (fresh build + verified dispatcher/renderer)\n");
    unsigned renderer_revision = nv2a_pb_exec_overlay_revision();
    fprintf(stderr, "[OVERLAY-CHECK] renderer=v%u float_depth=1 swizzled_targets=1 texture_sizes=1\n",
            renderer_revision);
    if (renderer_revision != QPR2_RENDERER_REVISION || !qpr2_validate_level_callbacks()) {
        fprintf(stderr, "[BOOT] Overlay verification failed; stopping before guest startup.\n");
        return 4;
    }
    printf("Title ID: %s | entry: 0x%08X | XBE SHA-256: %s\n",
           QPR2_TITLE_ID_TEXT, QPR2_ENTRY_POINT, QPR2_XBE_SHA256);

    SymSetOptions(SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME);
    SymInitialize(GetCurrentProcess(), NULL, TRUE);
    AddVectoredExceptionHandler(1, qpr2_exception_handler);

    if (!load_file(QPR2_XBE_PATH, &xbe_data, &xbe_size)) {
        MessageBoxA(NULL,
                    "default.xbe was not found. Extract your legally owned "
                    "disc to the game folder first.",
                    "QPR2 recomp", MB_ICONERROR);
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
    xbox_WatchdogStart();

    printf("[BOOT] runtime initialized; entering recompiled code\n");
    xbe_entry_point();
    printf("[BOOT] guest returned\n");

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

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOW);
}

