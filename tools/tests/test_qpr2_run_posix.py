"""The POSIX launcher's profile reader and run stage, with a stand-in executable."""
import json
import os
from pathlib import Path
import stat
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
import qpr2_run_posix as launcher

FAKE_GAME = """#!/bin/sh
echo "[BOOT] stand-in started in $(pwd)"
echo "[ENV] carry=$RECOMP_GPU_FENCE_CARRY_MS rotate=$RECOMP_GPU_ROTATE_SURFACES timing=${RECOMP_GAME_TIMING:-unset} watchdog=${RECOMP_WATCHDOG_SECS:-unset} headless=${RECOMP_HEADLESS:-unset}" >&2
if [ "$1$FAKE_HANG" = "1" ]; then sleep 60; fi
exit ${FAKE_EXIT:-0}
"""


@unittest.skipIf(os.name == "nt", "POSIX launcher")
class PosixLauncherTest(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.root = Path(self.temp.name)
        (self.root / "game").mkdir()
        (self.root / "game" / "default.xbe").write_bytes(b"XBEH synthetic, not a retail image")
        self.exe = self.root / "fake-game"
        self.exe.write_text(FAKE_GAME)
        self.exe.chmod(self.exe.stat().st_mode | stat.S_IXUSR)

    def tearDown(self):
        self.temp.cleanup()

    def launch(self, *arguments, **environment):
        env = {k: v for k, v in os.environ.items() if not k.startswith("RECOMP_")}
        env.pop("DISPLAY", None)
        env.pop("WAYLAND_DISPLAY", None)
        env.update(environment)
        done = subprocess.run([sys.executable, str(ROOT / "tools" / "qpr2_run_posix.py"),
                               "--exe", str(self.exe), "--game-root", str(self.root),
                               "--logs-dir", str(self.root / "logs"), *arguments],
                              capture_output=True, text=True, env=env, timeout=60)
        runs = sorted((self.root / "logs").glob("clean-v1-*/run.json"))
        return done, json.loads(runs[-1].read_text()), runs[-1].parent

    def test_profile_matches_the_powershell_data_file(self):
        profile = launcher.read_profile()
        self.assertEqual(len(profile["XbeSha256"]), 64)
        self.assertEqual(profile["Runtime"]["RECOMP_GPU_FENCE_CARRY_MS"], "8")
        self.assertEqual(profile["Runtime"]["RECOMP_GPU_ROTATE_SURFACES"], "1")
        self.assertEqual(profile["Benchmark"]["RECOMP_GAME_TIMING"], "1")
        self.assertIn("RECOMP_MAIN_SAMPLES", profile["Debug"])
        # Every line of the real file is understood; nothing is silently skipped.
        text = (ROOT / "tools" / "qpr2_runtime.psd1").read_text(encoding="utf-8-sig")
        entries = sum(1 for line in text.splitlines() if '= "' in line)
        self.assertEqual(entries, 1 + sum(len(profile[name]) for name in ("Runtime", "Benchmark", "Debug")))

    def test_rejects_an_unknown_xbe_before_running(self):
        done, manifest, _ = self.launch()
        self.assertEqual(done.returncode, 1)
        self.assertEqual(manifest["status"], "failed")
        self.assertIn("Unexpected default.xbe SHA-256", manifest["error"])

    def test_benchmark_run_records_environment_and_exit(self):
        done, manifest, run_dir = self.launch("--mode", "Benchmark", "--allow-unknown-xbe",
                                              RECOMP_GPU_ROTATE_SURFACES="1")
        self.assertEqual(done.returncode, 0, done.stderr)
        self.assertEqual((manifest["status"], manifest["exit_code"], manifest["mode"]), ("exited", 0, "Benchmark"))
        self.assertEqual(manifest["renderer"], "software")
        self.assertFalse(manifest["watchdog_fired"])
        self.assertEqual(manifest["timeout_seconds"], 360)
        # The caller's override wins over the profile, and the manifest records what ran.
        self.assertEqual(manifest["environment"]["RECOMP_GPU_ROTATE_SURFACES"], "1")
        log = (run_dir / "run.stderr.log").read_text()
        self.assertIn("carry=8 rotate=1 timing=1 watchdog=360 headless=1", log)
        self.assertIn(str(self.root), (run_dir / "run.stdout.log").read_text())
        self.assertEqual(len(list((self.root / "logs").glob("qpr2-clean-v1-results-*.zip"))), 1)

    def test_play_mode_has_no_timeout_and_reports_failure_codes(self):
        done, manifest, run_dir = self.launch("--allow-unknown-xbe", FAKE_EXIT="7")
        self.assertEqual(done.returncode, 7)
        self.assertEqual((manifest["status"], manifest["exit_code"], manifest["timeout_seconds"]), ("failed", 7, 0))
        self.assertIn("timing=unset watchdog=unset", (run_dir / "run.stderr.log").read_text())

    def test_timeout_stops_the_process_group(self):
        done, manifest, _ = self.launch("--mode", "Benchmark", "--timeout-seconds", "1",
                                        "--allow-unknown-xbe", FAKE_HANG="1")
        self.assertEqual((manifest["status"], manifest["exit_code"]), ("timed_out", 124))
        self.assertEqual(done.returncode, 124)


if __name__ == "__main__":
    unittest.main()
