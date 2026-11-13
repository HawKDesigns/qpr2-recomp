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
