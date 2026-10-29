/* Deterministic test of the actual registration/tick/flip runtime code.
 * GCC: -ffunction-sections -fdata-sections -Wl,--gc-sections; no game needed. */
#define QueryPerformanceCounter test_QueryPerformanceCounter
#define QueryPerformanceFrequency test_QueryPerformanceFrequency
#define VirtualQuery test_VirtualQuery
#include "../third_party/xboxrecomp/src/kernel/xbox_memory_layout.c"
#include <assert.h>

static uint64_t test_us;
static unsigned char *test_memory;
static unsigned event_signals;
static uint32_t last_event;
int xbox_KeSignalGuestEvent(uint32_t va) { ++event_signals;last_event=va;return 1; }
#define TEST_MEMORY_BYTES 0x300000u
BOOL test_QueryPerformanceCounter(LARGE_INTEGER *out)
{ out->QuadPart = (LONGLONG)test_us; return TRUE; }
BOOL test_QueryPerformanceFrequency(LARGE_INTEGER *out)
{ out->QuadPart = 1000000; return TRUE; }
SIZE_T test_VirtualQuery(LPCVOID address, MEMORY_BASIC_INFORMATION *info, SIZE_T size)
{
    uintptr_t a = (uintptr_t)address, b = (uintptr_t)test_memory;
    (void)size;
    if (a < b || a >= b + TEST_MEMORY_BYTES) return 0;
    memset(info, 0, sizeof(*info));
    info->BaseAddress = test_memory;
    info->RegionSize = TEST_MEMORY_BYTES;
    info->State = MEM_COMMIT;
    info->Protect = PAGE_READWRITE;
    return sizeof(*info);
}
static void put(uint32_t va, uint32_t value)
{ *(uint32_t *)(test_memory + va) = value; }
static uint32_t get(uint32_t va)
{ return *(uint32_t *)(test_memory + va); }

int main(void)
{
    test_memory = calloc(1, TEST_MEMORY_BYTES);
    assert(test_memory);
    g_memory_offset = (ptrdiff_t)test_memory;
    g_memory_base = test_memory;
    g_memory_size = TEST_MEMORY_BYTES;
    assert(xbox_Nv2aVblankEvent(0x100000,0x1954)==-1);
    assert(xbox_Nv2aFrameCounter(0x100000, 0x1980) == 0);
    assert(xbox_Nv2aVblankEvent(0x100000,0x1954)==0);
    frame_counters_tick(); /* device is not allocated yet */
    put(0x100000, 0x10000);
    put(0x10000 + 0x1974, 0x400000);
    put(0x10000 + 0x1980, 17);
    frame_counters_tick();
    assert(!event_signals);
    test_us = 50050;
    frame_counters_tick();
    assert(get(0x10000 + 0x1980) == 20);
    assert(event_signals==1 && last_event==0x11954);
    xbox_Nv2aFrameCounterFlip();
    xbox_Nv2aFrameCounterFlip();
    assert(get(0x10000 + 0x1980) == 20);
    assert(event_signals==1 && last_event==0x11954);
    test_us = 5005000;
    xbox_Nv2aFrameCounterFlip();
    assert(get(0x10000 + 0x1980) == 317);
    assert(event_signals==2);
    frame_counters_tick();
    assert(get(0x10000 + 0x1980) == 317);
    assert(event_signals==2);
    /* A replacement device starts from its own count and phase. */
    put(0x100000, 0x20000);
    put(0x20000 + 0x1980, 900);
    put(0x20000 + 0x1FE0, 0x1312D00); /* PAL explicit display period */
    frame_counters_tick();
    assert(get(0x20000 + 0x1980) == 900 && event_signals==2);
    test_us += 1000000;
    xbox_Nv2aFrameCounterFlip();
    assert(get(0x20000 + 0x1980) == 950 && event_signals==3 && last_event==0x21954);
    assert(get(0x10000 + 0x1980) == 317);
    assert(event_signals==3);
    free(test_memory);
    puts("PASS: actual runtime device registration, missing device, delayed ticks, vblank event dispatch only on elapsed periods, flips, device replacement and PAL selection");
    return 0;
}
