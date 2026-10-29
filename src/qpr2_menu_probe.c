#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif
#include "recomp/gen/recomp_types.h"
#include "qpr2_menu_probe.h"

static uint32_t menu_read(uint32_t address)
{
    if (!address) return 0;
#ifdef _WIN32
    MEMORY_BASIC_INFORMATION info;
    uintptr_t native = XBOX_PTR(address);
    if (!VirtualQuery((const void *)native, &info, sizeof(info)) ||
        info.State != MEM_COMMIT || (info.Protect & (PAGE_NOACCESS | PAGE_GUARD)) ||
        native + 4u > (uintptr_t)info.BaseAddress + info.RegionSize) return 0xBAD00000u;
#endif
    return MEM32(address);
}
static float menu_float(uint32_t bits)
{
    float value;
    memcpy(&value, &bits, sizeof(value));
    return value;
}

/* Read-only snapshots at the retail splash, legal-page and transition gates.
 * No guest registers, FPU state, timers or pages are changed. */
void qpr2_menu_probe(uint32_t stage, uint32_t object, uint32_t value)
{
    static RECOMP_TLS struct { uint32_t object, last_ms; uint64_t calls; } slots[4];
    unsigned index = stage == 0x3FCE0 ? 0 : stage == 0xB5D80 ? 1 : stage == 0xB5DA0 ? 2 : 3;
    uint32_t now, linked, vt, raw, linked_raw;
#ifdef _WIN32
    now = GetTickCount();
#else
    now = (uint32_t)((uint64_t)clock() * 1000u / CLOCKS_PER_SEC);
#endif
    ++slots[index].calls;
    if (object == slots[index].object && slots[index].calls > 4 &&
        (uint32_t)(now - slots[index].last_ms) < 2000u) return;
    slots[index].last_ms = now;
    slots[index].object = object;
    vt = menu_read(object);
    linked = object ? menu_read(object + 4) : 0;
    raw = object ? menu_read(object + 0x4C) : 0;
    linked_raw = linked && linked != 0xBAD00000u ? menu_read(linked + 0x4C) : 0;
    fprintf(stderr, "[MENU-GATE] stage=%08X calls=%llu object=%08X vtable=%08X "
        "update68=%08X state48=%u field4c=%08X/%.9g linked=%08X linked4c=%08X/%.9g "
        "value=%08X/%.9g auto_threshold=%.9g input_threshold=%.9g blink=%.9g\n",
        stage, (unsigned long long)slots[index].calls, object, vt,
        vt && vt != 0xBAD00000u ? menu_read(vt + 0x68) : 0,
        object ? menu_read(object + 0x48) : 0, raw, menu_float(raw),
        linked, linked_raw, menu_float(linked_raw), value, menu_float(value),
        menu_float(menu_read(0x1C00A8)), menu_float(menu_read(0x1C00A4)),
        object ? menu_float(menu_read(object + 0xB4)) : 0.0);
}
