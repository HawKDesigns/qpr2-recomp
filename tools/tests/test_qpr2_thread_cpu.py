"""Build the runtime's per-thread CPU accounting natively and check its report."""
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
PLATFORM = ROOT / "third_party/xboxrecomp/src/platform"

DRIVER = r"""
#define _GNU_SOURCE
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include "thread_cpu.h"
#include "host_time.h"

static volatile int stop;
static volatile uint64_t sink;

static void *busy(void *role)
{
    xbox_ThreadCpuRegister((const char *)role);
    xbox_ThreadCpuRegister("ignored-second-name");   /* no-op for this thread */
    while (!stop) sink += 1;
    return NULL;
}

static void *idle(void *role)
{
    struct timespec nap = {0, 5 * 1000 * 1000};
    xbox_ThreadCpuRegister((const char *)role);
    while (!stop) nanosleep(&nap, NULL);
    return NULL;
}

static void *brief(void *role)
{
    xbox_ThreadCpuRegister((const char *)role);
    return NULL;                                     /* ends without unregistering */
}

int main(void)
{
    pthread_t a, b, c, d;
    struct timespec window = {0, 300 * 1000 * 1000};
    xbox_ThreadCpuRegister("main");
    pthread_create(&a, NULL, busy, "worker");
    pthread_create(&b, NULL, busy, "worker");
    pthread_create(&c, NULL, idle, "sleeper");
    pthread_create(&d, NULL, brief, "brief");
    pthread_join(d, NULL);
    nanosleep(&window, NULL);
    xbox_ThreadCpuReport(stdout);                    /* baseline: prints nothing */
    puts("--- window ---");
    nanosleep(&window, NULL);
    xbox_ThreadCpuReport(stdout);
    stop = 1;
    pthread_join(a, NULL); pthread_join(b, NULL); pthread_join(c, NULL);
    return 0;
}
"""


@unittest.skipUnless(shutil.which("gcc") and (PLATFORM / "thread_cpu.c").exists(),
                     "requires gcc and the pinned runtime source")
class ThreadCpuTest(unittest.TestCase):
    def test_reports_busy_threads_by_role_and_a_process_total(self):
        with tempfile.TemporaryDirectory() as name:
            run = Path(name)
            (run / "driver.c").write_text(DRIVER, encoding="utf-8")
            subprocess.run([
                # gnu11, as the runtime's CMake build compiles host_time.c.
                "gcc", "-std=gnu11", "-pthread", "-O1", "-Wall", "-Wextra", "-Werror",
                "-DXBOX_THREAD_CPU=1", "-I", str(PLATFORM), str(PLATFORM / "thread_cpu.c"), str(PLATFORM / "host_time.c"),
                str(run / "driver.c"), "-o", str(run / "driver"),
            ], check=True)
            out = subprocess.run([str(run / "driver")], check=True, timeout=20,
                                 capture_output=True, text=True).stdout
        baseline, window = out.split("--- window ---")
        self.assertEqual(baseline.strip(), "")
        rows = [dict(re.findall(r"(\w+)=(\S+)", line)) for line in window.strip().splitlines()]
        self.assertTrue(all(line.startswith("[THREAD-CPU] ") for line in window.strip().splitlines()))
        workers = [row for row in rows if row["role"] == "worker"]
        # Two threads share one role and are told apart by ordinal.
        self.assertEqual(sorted(row["ordinal"] for row in workers), ["1", "2"])
        total_worker_ms = sum(float(row["cpu_ms"]) for row in workers)
        process = rows[-1]
        self.assertEqual(process["role"], "process")
        window_ms = float(process["window_ms"])
        self.assertGreater(window_ms, 250)
        # The spinning threads account for most of the process's CPU time; how
        # much each gets depends on how many cores the host has.
        self.assertGreater(total_worker_ms, 0.5 * float(process["cpu_ms"]))
        self.assertGreater(float(process["cpu_ms"]), 0.5 * window_ms)
        self.assertAlmostEqual(float(process["named_ms"]) + float(process["unnamed_ms"]),
                               float(process["cpu_ms"]), delta=1.0)
        # A sleeping thread and one that already ended add no busy rows.
        for row in rows[:-1]:
            if row["role"] in ("sleeper", "main"):
                self.assertLess(float(row["busy_pct"]), 20.0)
        self.assertNotIn("brief", [row["role"] for row in rows])
        self.assertNotIn("ignored-second-name", [row["role"] for row in rows])


if __name__ == "__main__":
    unittest.main()
