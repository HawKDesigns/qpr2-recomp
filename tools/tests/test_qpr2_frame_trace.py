import csv
import json
from pathlib import Path
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import qpr2_frame_trace as trace


def record(ms, sequence, kind, producer=0, frame=0, args=(0, 0, 0)):
    return trace.RECORD.pack(ms * 1_000_000, sequence, 17, kind, producer, frame, *args)


def fixture(folder, *, mode="Benchmark", frame_ms=40, truncate=False):
    (folder / "run.json").write_text(json.dumps({
        "run_id": folder.name, "mode": mode, "status": "exited", "environment": {},
    }), encoding="utf-8")
    records = [
        record(0, 1, trace.PHASE, producer=2, args=(1, 0, 0)),
        record(10, 1, trace.FLIP, frame=1, args=(0xFFFFFFFF, 1, 1)),
        record(11, 2, trace.HANDOFF, frame=1),
        record(10 + frame_ms, 3, trace.FLIP, frame=2, args=(2, 3, 2)),
        record(95, 4, trace.CAPTURE_BEGIN),
        record(100, 5, trace.FLIP, frame=3, args=(5, 5, 3)),
        record(120, 6, trace.CAPTURE_END),
        record(140, 7, trace.FLIP, frame=4, args=(8, 7, 4)),
        record(160, 8, trace.FLIP, frame=5, args=(9, 9, 5)),
        record(175, 10, trace.FLIP, frame=7, args=(10, 10, 7)),
        record(12, 1, trace.DRAW, producer=1, frame=1),
        record(170, 2, trace.PHASE, producer=1, args=(5, 1, 0)),
        *(record(180, 0, trace.FOOTER, producer=i, args=(i == 0, 0, 0)) for i in range(3)),
    ]
    data = trace.HEADER.pack(b"QPTR", 1, trace.RECORD.size, 3) + b"".join(records)
    (folder / "frame-trace.bin").write_bytes(data[:-8] if truncate else data)


class FrameTraceTest(unittest.TestCase):
    def test_export_correlates_producers_and_excludes_capture_and_missing_flips(self):
        with tempfile.TemporaryDirectory() as name:
            run = Path(name)
            fixture(run)
            summary = trace.export(run)
            self.assertTrue(summary["integrity"]["complete"])
            self.assertEqual(summary["integrity"]["dropped"]["renderer"], 1)
            self.assertEqual(summary["integrity"]["sequence_gaps"]["renderer"], 1)
            boot = summary["phases"]["boot"]
            self.assertEqual(boot["measured_intervals"], 2)
            self.assertEqual(boot["median_ms"], 30)
            self.assertEqual(boot["p95_ms"], 40)
            self.assertEqual((boot["duration_ms"], boot["max_ms"], boot["over_100_ms"]), (60, 40, 0))
            self.assertEqual(boot["guest_display_vblank_hz"], 4 * 1000 / 60)
            self.assertEqual(summary["phases"]["behind"]["excluded_intervals"], {"missing_flip": 1})
            with (run / "frames.csv").open(newline="") as stream:
                rows = list(csv.DictReader(stream))
            self.assertEqual(rows[0]["handoff_ns"], "11000000")
            self.assertEqual(rows[0]["draw_ns"], "12000000")
            self.assertEqual(rows[1]["guest_ticks"], "3")
            self.assertEqual(rows[2]["excluded"], "capture")
            self.assertEqual(rows[3]["excluded"], "capture_boundary")
            self.assertTrue(summary["phase_markers"][-1]["manual"])

    def test_compare_flags_a_partial_debug_run(self):
        with tempfile.TemporaryDirectory() as name:
            reference = Path(name) / "reference"
            candidate = Path(name) / "candidate"
            reference.mkdir()
            candidate.mkdir()
            fixture(reference)
            fixture(candidate, mode="Debug", frame_ms=60, truncate=True)
            result = trace.compare(reference, candidate)
            self.assertIn("candidate has an incomplete trace or dropped records", result["warnings"])
            self.assertEqual(result["manifest_differences"]["mode"], ["Benchmark", "Debug"])
            self.assertEqual(result["phases"]["boot"]["delta"]["p95_ms"], 20)
            corrected = trace.compare(reference, candidate, exclude_reference=[1])
            self.assertEqual(corrected["excluded_segments"]["reference"], [1])
            self.assertEqual(corrected["phases"]["boot"]["reference"]["measured_intervals"], 0)
            self.assertEqual(corrected["phases"]["boot"]["reference"]["excluded_intervals"],
                             {"manual_exclusion": 5})
            with self.assertRaisesRegex(ValueError, "unknown phase segment"):
                trace.export(reference, [99])


    def test_profile_joins_delayed_samples_and_excludes_pause(self):
        with tempfile.TemporaryDirectory() as name:
            run = Path(name)
            (run / "run.json").write_text(json.dumps({
                "run_id": "profile", "mode": "Benchmark", "status": "exited",
            }))
            records = [
                record(0, 1, trace.PHASE, producer=1, args=(5, 1, 0)),
                record(10, 1, trace.FLIP, frame=1),
                record(25, 2, trace.PAUSE, producer=1, args=(1, 0, 0)),
                record(40, 2, trace.FLIP, frame=2),
                record(50, 3, trace.PAUSE, producer=1, args=(0, 0, 0)),
                record(60, 3, trace.FLIP, frame=3),
                record(100, 4, trace.FLIP, frame=4),
                record(100, 5, trace.CPU_METRIC, frame=4, args=(0, 19000, 0)),
                record(100, 6, trace.CPU_METRIC, frame=4, args=(6, 9000, 0)),
                record(100, 7, trace.CPU_METRIC, frame=4, args=(45, 11000, 0)),
                record(100, 8, trace.PROFILE_FRAME, frame=4),
                record(110, 1, trace.GUEST_FENCE_WAIT, producer=2, args=(30000, 0, 1)),
                record(130, 9, trace.FLIP, frame=5),
                record(130, 10, trace.PROFILE_FRAME, frame=5),
                record(145, 11, trace.GPU_SAMPLE, frame=4, args=(6, 3100, 50)),
                *(record(150, 0, trace.FOOTER, producer=i) for i in range(3)),
            ]
            (run / "frame-trace.bin").write_bytes(
                trace.HEADER.pack(b"QPTR", 2, trace.RECORD.size, 3) + b"".join(records))
            trace.export(run)
            with (run / "frame-profile.csv").open(newline="") as stream:
                rows = list(csv.DictReader(stream))
            self.assertEqual([row["excluded"] for row in rows[1:3]], ["pause", "pause_boundary"])
            self.assertEqual(rows[3]["gpu_raw_pixels_sample_ms"], "3.1")
            self.assertEqual(rows[3]["guest_fence_wait_ms"], "20.0")
            self.assertEqual(rows[4]["guest_fence_wait_ms"], "10.0")
            self.assertEqual(rows[3]["fifo_ms"], "19.0")
            self.assertEqual(rows[3]["gpu_raw_pixels_sample_count"], "1")
            self.assertEqual(rows[3]["target_depth_256x256_ms"], "11.0")
            data = json.loads((run / "profile-summary.json").read_text())
            self.assertTrue(data["trace_complete_without_loss"])
            self.assertEqual(data["phases"]["behind"]["slow_and_fast_frame_means"]
                             ["over_33ms"]["queued_map_ms"], 9)

    def test_version_three_diagnostics(self):
        with tempfile.TemporaryDirectory() as name:
            run = Path(name)
            (run / "run.json").write_text(json.dumps({
                "run_id": "profile-v3", "mode": "Benchmark", "status": "exited", "xbe_sha256": "test",
            }))
            (run / "memory-map.json").write_text(json.dumps({
                "xbe_sha256": "test", "ranges": [{
                    "source": "xbe_section_table", "name": "D3D", "guest_va": "0x100",
                    "end_exclusive": "0x200", "declared_executable": True,
                }],
            }))
            records = [
                record(10, 1, trace.SCENE_ACTIVITY, producer=2, args=(2, 0, 0)),
                record(20, 1, trace.FLIP, frame=1),
                record(35, 2, trace.DEPTH_LAYOUT, frame=2, args=(0x3BD8000, 640 | (480 << 16), 0x120)),
                record(36, 3, trace.DEPTH_DRAIN, frame=2, args=(256 | (256 << 16), 0x203, 2000)),
                record(130, 4, trace.FLIP, frame=2, args=(7, 0, 0)),
                record(130, 5, trace.CPU_METRIC, frame=2, args=(58, 1, 0)),
                record(130, 6, trace.CPU_METRIC, frame=2, args=(62, 3, 0)),
                record(130, 7, trace.CPU_METRIC, frame=2, args=(63, 2, 0)),
                record(130, 8, trace.PROFILE_FRAME, frame=2),
                record(140, 1, trace.OVERLAY, producer=1, args=(1, 0, 0)),
                record(160, 9, trace.FLIP, frame=3),
                record(170, 2, trace.OVERLAY, producer=1),
                record(190, 10, trace.FLIP, frame=4),
                record(600, 11, trace.FLIP, frame=5),
                record(650, 3, trace.PHASE, producer=1, args=(8, 1, 0)),
                record(660, 12, trace.FLIP, frame=6),
                record(690, 13, trace.FLIP, frame=7),
                record(700, 4, trace.PHASE, producer=1, args=(9, 1, 0)),
                record(720, 14, trace.FLIP, frame=8),
                record(740, 15, trace.FLIP, frame=9),
                record(4850, 2, trace.GUEST_PC, producer=2, args=(0x101, 17, 0)),
                record(4900, 3, trace.THREAD_CPU, producer=2, frame=1, args=(10_000_000, 0, 42)),
                record(5900, 4, trace.THREAD_CPU, producer=2, frame=1, args=(15_000_000, 0, 42)),
                *(record(6000, 0, trace.FOOTER, producer=i) for i in range(3)),
            ]
            (run / "frame-trace.bin").write_bytes(
                trace.HEADER.pack(b"QPTR", 3, trace.RECORD.size, 3) + b"".join(records))
            summary = trace.export(run)
            preview = summary["phases"]["preview"]
            self.assertEqual(preview["over_100_ms"], 1)
            self.assertIsNone(preview["kernel_interrupt_vblank_hz"])
            self.assertEqual(preview["excluded_intervals"],
                             {"first_frame": 1, "overlay": 1, "overlay_boundary": 1})
            first, second = summary["diagnostic_windows"]
            self.assertEqual(first["guest_pc_samples"], {"D3D": 1})
            self.assertEqual(first["threads"]["fifo_executor"]["percent_of_one_core"], 50)
            self.assertEqual(second["threads"]["fifo_executor"]["cpu_ms"], 450)
            profile = json.loads((run / "profile-summary.json").read_text())
            drain, = profile["depth_dependencies"]["drains"]
            self.assertEqual((drain["producer_width"], drain["most_recent_binding"]["width"],
                              drain["total_ms"], drain["publishes_depth"]), (256, 640, 2, True))
            self.assertEqual(profile["phases"]["preview"]["per_frame_mean"]["raw_boundary_xy"], 1)
            self.assertEqual(profile["phases"]["preview"]["per_frame_mean"]["completion_markers"], 3)
            self.assertEqual(profile["phases"]["preview"]["per_frame_mean"]["completion_markers_reused"], 2)
            self.assertEqual(summary["phases"]["rider_menu"]["median_ms"], 30)
            self.assertEqual(summary["phases"]["atv_menu"]["median_ms"], 20)

    def test_a_run_without_manual_marks_is_flagged(self):
        with tempfile.TemporaryDirectory() as name:
            run = Path(name)
            (run / "run.json").write_text(json.dumps({
                "run_id": "unmarked", "mode": "Benchmark", "status": "exited", "environment": {},
            }), encoding="utf-8")
            records = [
                record(0, 1, trace.PHASE, producer=2, args=(1, 0, 0)),     # automatic boot mark only
                record(10, 1, trace.FLIP, frame=1, args=(0, 0, 1)),
                record(10, 2, trace.VBLANK_EVENTS, frame=1, args=(10, 0, 0)),
                record(40, 3, trace.FLIP, frame=2, args=(2, 0, 2)),
                record(40, 4, trace.VBLANK_EVENTS, frame=2, args=(11, 0, 0)),
                record(70, 5, trace.FLIP, frame=3, args=(4, 0, 3)),
                record(70, 6, trace.VBLANK_EVENTS, frame=3, args=(12, 0, 0)),
                *(record(80, 0, trace.FOOTER, producer=i) for i in range(3)),
            ]
            (run / "frame-trace.bin").write_bytes(
                trace.HEADER.pack(b"QPTR", 4, trace.RECORD.size, 3) + b"".join(records))
            summary = trace.export(run)
            self.assertEqual(len(summary["warnings"]), 1)
            self.assertIn("no manual phase marks", summary["warnings"][0])
            boot = summary["phases"]["boot"]
            self.assertTrue(boot["sustained_30fps"])       # two 30 ms frames
            self.assertFalse(boot["sustained_60fps"])
            self.assertAlmostEqual(boot["emulated_vblank_hz"], 4 / 0.06)
            self.assertAlmostEqual(boot["delivered_vblank_events_hz"], 2 / 0.06)
            self.assertIsNone(boot["kernel_interrupt_vblank_hz"])
