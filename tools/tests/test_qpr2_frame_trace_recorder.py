"""Exercise the actual recorder and offline reader with a small Win32 shim."""
import json
import os
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

from test_qpr2_frame_trace import trace

ROOT = Path(__file__).resolve().parents[2]
PLATFORM = ROOT / "third_party/xboxrecomp/src/platform"

WINDOWS_SHIM = r"""
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <wchar.h>
typedef int32_t LONG;
typedef uint32_t DWORD;
typedef pthread_t *HANDLE;
typedef void *LPVOID;
#define WINAPI
static inline LONG InterlockedCompareExchange(volatile LONG *ptr, LONG value, LONG expected) {
    __atomic_compare_exchange_n(ptr, &expected, value, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    return expected;
}
static inline LONG InterlockedExchange(volatile LONG *ptr, LONG value) {
    return __atomic_exchange_n(ptr, value, __ATOMIC_SEQ_CST);
}
static inline LONG InterlockedExchangeAdd(volatile LONG *ptr, LONG value) {
    return __atomic_fetch_add(ptr, value, __ATOMIC_SEQ_CST);
}
static inline void InterlockedIncrement(volatile LONG *ptr) {
    InterlockedExchangeAdd(ptr, 1);
}
DWORD GetCurrentThreadId(void);
HANDLE CreateThread(void *, size_t, DWORD (*)(LPVOID), LPVOID, DWORD, DWORD *);
DWORD WaitForSingleObject(HANDLE, DWORD);
void Sleep(DWORD);
FILE *_wfopen(const wchar_t *, const wchar_t *);
"""

DRIVER = r"""
#include <assert.h>
#include "windows.h"
#include "frame_trace.h"
#include "nv2a_frame_profile.h"
#include "host_time.h"

static pthread_t writer;
static DWORD (*write_proc)(LPVOID);
static LPVOID write_arg;
static uint64_t tick;

static void *run_writer(void *unused)
{
    (void)unused;
    assert(write_proc(write_arg) == 0);
    return NULL;
}

DWORD GetCurrentThreadId(void) { return 3; }
HANDLE CreateThread(void *security, size_t stack, DWORD (*proc)(LPVOID),
                    LPVOID arg, DWORD flags, DWORD *id)
{
    (void)security; (void)stack; (void)flags; (void)id;
    write_proc = proc;
    write_arg = arg;
    return &writer;
}

DWORD WaitForSingleObject(HANDLE handle, DWORD timeout)
{
    (void)timeout;
    /* Delay draining until after producers have filled the ring. */
    assert(!pthread_create(handle, NULL, run_writer, NULL));
    assert(!pthread_join(*handle, NULL));
    return 0;
}

void Sleep(DWORD ms) { (void)ms; }
uint64_t xbox_HostNowNs(void) { return ++tick * 1000000; }
FILE *_wfopen(const wchar_t *path, const wchar_t *mode)
{
    assert(!wcscmp(path, L"frame-trace.bin") && !wcscmp(mode, L"wb"));
    return fopen("frame-trace.bin", "wb");
}

int main(int argc, char **argv)
{
    (void)argv;
    assert(!xbox_frame_trace_enabled());
    assert(xbox_frame_trace_start(L"frame-trace.bin"));
    xbox_frame_trace_note(XBOX_TRACE_TITLE, XBOX_TRACE_PHASE, 0, XBOX_PHASE_BOOT, 0, 0);
    if (argc > 1) {
        assert(nv2a_frame_profile_enabled());
        nv2a_frame_profile_fifo_enter(5);
        xbox_frame_trace_note(XBOX_TRACE_RENDERER, XBOX_TRACE_FLIP, 1, 0, 0, 0);
        nv2a_frame_profile_flip(1, 8, 15);
        assert(nv2a_frame_profile_next_id() == 2);
        nv2a_frame_profile_add(NV_FRAME_UPLOAD, 3.5);
        nv2a_frame_profile_fifo_leave(20);
        xbox_frame_trace_note(XBOX_TRACE_RENDERER, XBOX_TRACE_FLIP, 2, 0, 0, 0);
        nv2a_frame_profile_flip(2, 4, 25);
        xbox_frame_trace_stop();
        return 0;
    }
    for (uint32_t frame = 1; frame <= 1100; ++frame) {
        xbox_frame_trace_guest_ticks(2);
        xbox_frame_trace_note(XBOX_TRACE_RENDERER, XBOX_TRACE_FLIP, frame,
                              xbox_frame_trace_guest_tick_count(), frame, frame);
    }
    xbox_frame_trace_note(XBOX_TRACE_WINDOW, XBOX_TRACE_PHASE, 0, XBOX_PHASE_AHEAD, 1, 0);
    xbox_frame_trace_stop();
    assert(!xbox_frame_trace_enabled());
    return 0;
}
"""


@unittest.skipUnless(shutil.which("gcc"), "requires gcc")
class FrameTraceRecorderTest(unittest.TestCase):
    def test_recorder_flushes_all_producers_and_reports_ring_overflow(self):
        with tempfile.TemporaryDirectory() as name:
            run = Path(name)
            (run / "windows.h").write_text(WINDOWS_SHIM, encoding="utf-8")
            (run / "driver.c").write_text(DRIVER, encoding="utf-8")
            (run / "run.json").write_text(json.dumps({
                "run_id": "fixture", "mode": "Benchmark", "status": "exited",
            }), encoding="utf-8")
            subprocess.run([
                "gcc", "-std=c11", "-pthread", "-O2", "-Wall", "-Wextra", "-Werror",
                "-DXBOX_FRAME_TELEMETRY=1", "-I", str(run), "-I", str(PLATFORM),
                "-I", str(PLATFORM.parent), "-I", str(PLATFORM.parent / "kernel"),
                str(PLATFORM / "frame_trace.c"),
                str(PLATFORM.parent / "kernel/nv2a_frame_profile.c"), str(run / "driver.c"), "-o", str(run / "driver"),
            ], check=True)
            subprocess.run([str(run / "driver")], cwd=run, check=True, timeout=10)
            result = trace.export(run)
            self.assertTrue(result["integrity"]["complete"])
            self.assertEqual(result["integrity"]["dropped"]["renderer"], 76)
            self.assertEqual(result["phases"]["boot"]["flip_count"], 1024)
            self.assertEqual(result["phases"]["boot"]["guest_tick_hz"], 2000)
            self.assertEqual([mark["phase"] for mark in result["phase_markers"]], ["boot", "ahead"])
            subprocess.run([str(run / "driver"), "profile"], cwd=run, check=True, timeout=10,
                           env={**os.environ, "RECOMP_FRAME_PROFILE": "1"})
            trace.export(run)
            profile = json.loads((run / "profile-summary.json").read_text())
            self.assertEqual(profile["profiled_frames"], 2)
            frame = profile["phases"]["boot"]["per_frame_mean"]
            self.assertEqual((frame["fifo_ms"], frame["upload_ms"], frame["draw_requests"]), (5, 3.5, 4))
