#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "recomp/gen/recomp_types.h"
#include "qpr2_ui_trace.h"

enum { UI_TRACE_COUNT = 64 };
typedef struct UITrace {
    uint32_t phase, site, target, object, caller, argument, expected_esp;
    uint32_t eax, ecx, edx, ebx, esi, edi, esp, old_ebx, old_esi, old_edi;
} UITrace;
static RECOMP_TLS UITrace history[UI_TRACE_COUNT];
static RECOMP_TLS uint64_t total;
static RECOMP_TLS unsigned violations;
enum { LOADER_TRACE_COUNT = 256 };
static RECOMP_TLS UITrace loader_history[LOADER_TRACE_COUNT];
static RECOMP_TLS uint64_t loader_total;
static RECOMP_TLS unsigned loader_violations;
static RECOMP_TLS struct { uint32_t object, vtable, child70, child74; unsigned draws; } academy[3];

static uint32_t academy_read(uint32_t address)
{
#ifdef _WIN32
    MEMORY_BASIC_INFORMATION info;
    uintptr_t native = XBOX_PTR(address);
    if (!VirtualQuery((const void *)native, &info, sizeof info) ||
        info.State != MEM_COMMIT || (info.Protect & (PAGE_NOACCESS | PAGE_GUARD)) ||
        native + 4u > (uintptr_t)info.BaseAddress + info.RegionSize) return 0xBAD00000u;
#endif
    return MEM32(address);
}

void qpr2_academy_register(uint32_t kind, uint32_t object)
{
    if (kind >= 3 || !object) return;
    academy[kind].object = object; academy[kind].draws = 0;
    academy[kind].vtable = academy_read(object);
    academy[kind].child70 = academy_read(object + 0x70);
    academy[kind].child74 = academy_read(object + 0x74);
    fprintf(stderr, "[ACADEMY-OBJECT] kind=%u object=%08X vtable=%08X child70=%08X child74=%08X\n",
        kind, object, academy[kind].vtable, academy[kind].child70, academy[kind].child74);
}

static void academy_draw(uint32_t phase, uint32_t object, uint32_t target)
{
    if (phase != 1 && phase != 2) return;
    for (unsigned kind = 0; kind < 3; ++kind) {
        uint32_t root = academy[kind].object;
        if (!root) continue;
        uint32_t child70 = academy[kind].child70, child74 = academy[kind].child74;
        if (object != root && object != child70 && object != child74) continue;
        if (academy_read(root) != academy[kind].vtable) continue;
        unsigned n = academy[kind].draws;
        if (phase == 1) n = ++academy[kind].draws;
        if (n > 8 && (n % 120 || n > 2400)) continue;
        fprintf(stderr, "[ACADEMY-DRAW] kind=%u count=%u phase=%u root=%08X object=%08X target=%08X "
            "vt=%08X state48=%08X field4c=%08X field24=%08X rect=%08X,%08X,%08X,%08X "
            "child70=%08X child74=%08X\n", kind, n, phase, root, object, target,
            academy_read(object), academy_read(object + 0x48), academy_read(object + 0x4C), academy_read(object + 0x24),
            academy_read(object + 0xC), academy_read(object + 0x10), academy_read(object + 0x14), academy_read(object + 0x18),
            child70, child74);
    }
}

static void print_record(const UITrace *r)
{
    fprintf(stderr, "%s phase=%u site=%08X target=%08X this=%08X "
        "caller=%08X arg=%08X eax=%08X ecx=%08X edx=%08X "
        "ebx=%08X/%08X esi=%08X/%08X edi=%08X/%08X esp=%08X expected=%08X\n",
        r->phase >= 4 ? "[LOAD-CALL]" : "[UI-CALL]",
        r->phase, r->site, r->target, r->object, r->caller, r->argument,
        r->eax, r->ecx, r->edx, r->ebx, r->old_ebx, r->esi, r->old_esi,
        r->edi, r->old_edi, r->esp, r->expected_esp);
}

static void dump_loader_history(void)
{
    uint64_t start = loader_total > LOADER_TRACE_COUNT ?
        loader_total - LOADER_TRACE_COUNT : 0;
    fprintf(stderr, "[LOAD-HISTORY] last %llu of %llu events on faulting thread\n",
        (unsigned long long)(loader_total - start), (unsigned long long)loader_total);
    for (; start < loader_total; ++start)
        print_record(&loader_history[start & (LOADER_TRACE_COUNT - 1)]);
}

void qpr2_loader_trace(uint32_t phase, uint32_t site, uint32_t target,
                      uint32_t expected_esp, uint32_t old_ebx,
                      uint32_t old_esi, uint32_t old_edi)
{
    UITrace *r = &loader_history[loader_total++ & (LOADER_TRACE_COUNT - 1)];
    r->phase = phase; r->site = site; r->target = target;
    r->object = g_ecx; r->caller = 0; r->argument = 0;
    r->expected_esp = expected_esp;
    r->eax = g_eax; r->ecx = g_ecx; r->edx = g_edx;
    r->ebx = g_ebx; r->esi = g_esi; r->edi = g_edi; r->esp = g_esp;
    r->old_ebx = old_ebx; r->old_esi = old_esi; r->old_edi = old_edi;
    if ((phase == 5 || phase == 7) && (g_esp != expected_esp ||
        g_ebx != old_ebx || g_esi != old_esi || g_edi != old_edi)) {
        if (loader_violations++ < 16) {
            fprintf(stderr, "[LOAD-ABI] level setup call changed saved registers or stack\n");
            print_record(r);
        }
        if (loader_violations == 1) dump_loader_history();
    }
}

void qpr2_ui_trace(uint32_t phase, uint32_t site, uint32_t target,
                   uint32_t object, uint32_t caller, uint32_t argument,
                   uint32_t expected_esp, uint32_t old_ebx,
                   uint32_t old_esi, uint32_t old_edi)
{
    academy_draw(phase, object, target);
    UITrace *r = &history[total++ & (UI_TRACE_COUNT - 1)];
    r->phase = phase; r->site = site; r->target = target;
    r->object = object; r->caller = caller; r->argument = argument;
    r->expected_esp = expected_esp;
    r->eax = g_eax; r->ecx = g_ecx; r->edx = g_edx;
    r->ebx = g_ebx; r->esi = g_esi; r->edi = g_edi; r->esp = g_esp;
    r->old_ebx = old_ebx; r->old_esi = old_esi; r->old_edi = old_edi;
    if (phase == 3 && argument && argument < 0x10000u) {
        fprintf(stderr, "[UI-FREE] invalid low allocation address; caller recorded before CRT/SEH\n");
        print_record(r);
    }
    if (phase == 2 && (g_ebx != old_ebx || g_esi != old_esi ||
        g_edi != old_edi || g_esp != expected_esp) && violations++ < 16) {
        fprintf(stderr, "[UI-ABI] drawing callback changed saved registers or stack\n");
        print_record(r);
    }
}

void qpr2_ui_trace_dump(void)
{
    dump_loader_history();
    uint64_t start = total > UI_TRACE_COUNT ? total - UI_TRACE_COUNT : 0;
    fprintf(stderr, "[UI-HISTORY] last %llu of %llu events on faulting thread\n",
        (unsigned long long)(total - start), (unsigned long long)total);
    for (; start < total; ++start) print_record(&history[start & (UI_TRACE_COUNT - 1)]);
}
