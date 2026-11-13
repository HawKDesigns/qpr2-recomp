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
