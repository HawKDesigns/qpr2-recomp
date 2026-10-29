#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif
#include "recomp/gen/recomp_types.h"
#include "qpr2_xmv_probe.h"

/* Read-only diagnostics. Never changes a guest register, packet or status. */
static uint32_t probe_read(uint32_t va)
{
    if (!va) return 0;
#ifdef _WIN32
    MEMORY_BASIC_INFORMATION info;
    uintptr_t native = XBOX_PTR(va);
    if (!VirtualQuery((const void *)native, &info, sizeof(info))
        || info.State != MEM_COMMIT
        || (info.Protect & (PAGE_NOACCESS | PAGE_GUARD))
        || native + 4u > (uintptr_t)info.BaseAddress + info.RegionSize)
        return 0xBAD00000u;
#endif
    return MEM32(va);
}

void qpr2_xmv_probe(uint32_t object, uint32_t result, uint32_t hr, uint32_t stage,
                    const QPR2_XMV_TIMING *timing)
{
    static RECOMP_TLS uint64_t calls;
    static RECOMP_TLS uint32_t last_ms;
    static RECOMP_TLS uint32_t first_ms, last_object, last_base, previous_vblank;
    uint32_t now, dev, audio_status, tracks, i, base, vblank, delta_ms;
#ifdef _WIN32
    now = GetTickCount();
#else
    now = (uint32_t)((uint64_t)clock() * 1000u / CLOCKS_PER_SEC);
#endif
    ++calls;
    if (calls > 8 && (uint32_t)(now - last_ms) < 5000u) return;
    dev = probe_read(0x001A50E8u);
    base = probe_read(object + 0xB8u);
    vblank = dev ? probe_read(dev + 0x1980u) : 0;
    if (object != last_object || base != last_base) {
        first_ms = now;
        last_ms = now;
        previous_vblank = vblank;
        last_object = object;
        last_base = base;
    }
    delta_ms = now - last_ms;
    last_ms = now;
    fprintf(stderr,
        "[XMV-STATE] calls=%llu object=%08X hr=%08X result=%u stage=%08X "
        "vblank=%u base=%u tracks=%u frames=%u "
        "next_packet_bytes=%u packet60=%08X packet64=%08X ready68=%u ready6c=%u "
        "audio_done74=%u time_b0=%08X:%08X video_time_f8=%08X video_ready_e8=%u\n",
        (unsigned long long)calls, object, hr, probe_read(result), stage,
        dev ? probe_read(dev + 0x1980u) : 0, probe_read(object + 0xB8u),
        probe_read(object + 0x48u), probe_read(object + 0x3Cu),
        probe_read(object + 0x50u), probe_read(object + 0x60u),
        probe_read(object + 0x64u), probe_read(object + 0x68u),
        probe_read(object + 0x6Cu), probe_read(object + 0x74u),
        probe_read(object + 0xB4u), probe_read(object + 0xB0u),
        probe_read(object + 0xF8u), probe_read(object + 0xE8u));
    fprintf(stderr,
        "[XMV-TIMING] host_elapsed_ms=%u vblank_rate=%.3f "
        "elapsed_valid=%u elapsed_ticks=%u "
        "ready_valid=%u ready_due_ticks=%u ready_ms=%u ready_sync_ms=%d ready_interval_16_16=%u "
        "decode_valid=%u decode_due_ticks=%u decode_ms=%u decode_sync_ms=%d decode_interval_16_16=%u "
        "live_sync_ms=%d live_interval_16_16=%u\n",
        now - first_ms, delta_ms ? (double)(uint32_t)(vblank - previous_vblank) * 1000.0 / delta_ms : 0.0,
        timing->elapsed_valid, timing->elapsed_ticks,
        timing->ready_valid, timing->ready_due_ticks, timing->ready_ms, (int32_t)timing->ready_sync_ms, timing->ready_interval,
        timing->decode_valid, timing->decode_due_ticks, timing->decode_ms, (int32_t)timing->decode_sync_ms, timing->decode_interval,
        (int32_t)probe_read(object + 0xC8u), probe_read(object + 0xBCu));
    previous_vblank = vblank;
    audio_status = probe_read(object + 0x148u);
    tracks = probe_read(object + 0x48u);
    fprintf(stderr, "[XMV-AUDIO] status_array=%08X", audio_status);
    if (audio_status && tracks <= 16u)
        for (i = 0; i < tracks && i < 4u; ++i)
            fprintf(stderr, " track%u=%08X", i, probe_read(audio_status + i * 4u));
    fputc('\n', stderr);
    fflush(stderr);
}
