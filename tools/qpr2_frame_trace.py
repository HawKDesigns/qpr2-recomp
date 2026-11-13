#!/usr/bin/env python3
"""Export and compare QPR2 frame pacing on a single host clock."""
from __future__ import annotations

import argparse
from collections import Counter
import csv
from dataclasses import dataclass
import json
import math
from pathlib import Path
import statistics
import struct
import sys

HEADER = struct.Struct("<4sIII")
RECORD = struct.Struct("<Q8I")
FLIP, HANDOFF, DRAW, PHASE, CAPTURE_BEGIN, CAPTURE_END = range(1, 7)
FOOTER = 0xFFFFFFFF
PRODUCERS = ("renderer", "window", "title")
PHASES = ("unmarked", "boot", "preview", "countdown", "ahead", "behind", "unmarked")
UINT32_MASK = 0xFFFFFFFF


@dataclass(frozen=True)
class Event:
    timestamp: int
    sequence: int
    thread: int
    kind: int
    producer: int
    frame: int
    arg0: int
    arg1: int
    arg2: int


def read_trace(path: Path) -> tuple[list[Event], dict]:
    with path.open("rb") as stream:
        header = stream.read(HEADER.size)
        if len(header) != HEADER.size or HEADER.unpack(header) != (b"QPTR", 1, RECORD.size, len(PRODUCERS)):
            raise ValueError("missing or unsupported frame trace header")
        events = []
        dropped = {}
        last_sequence = {}
        sequence_gaps = Counter()
        tail_bytes = 0
        while data := stream.read(RECORD.size):
            if len(data) != RECORD.size:
                tail_bytes = len(data)
                break
            event = Event(*RECORD.unpack(data))
            if event.producer >= len(PRODUCERS):
                raise ValueError("invalid frame trace producer")
            producer = PRODUCERS[event.producer]
            if event.kind == FOOTER:
                dropped[producer] = event.arg0
                continue
            if producer in dropped or event.kind not in range(FLIP, CAPTURE_END + 1):
                raise ValueError("invalid frame trace event")
            if event.kind == PHASE and not 0 < event.arg0 < len(PHASES):
                raise ValueError("invalid phase marker")
            last = last_sequence.get(producer, 0)
            if event.sequence <= last:
                raise ValueError("frame trace producer sequence is out of order")
            sequence_gaps[producer] += event.sequence - last - 1
            last_sequence[producer] = event.sequence
            events.append(event)
    # Each producer's timestamps are ordered. Break clock ties so a phase
    # mark applies to a simultaneous flip and a flip precedes its handoff.
    events.sort(key=lambda e: (e.timestamp, 0 if e.kind == PHASE else 1,
                               e.producer, e.sequence))
    integrity = {
        "complete": len(dropped) == len(PRODUCERS) and not tail_bytes,
        "trailing_bytes": tail_bytes,
        "dropped": {name: dropped.get(name) for name in PRODUCERS},
        "sequence_gaps": {name: sequence_gaps[name] for name in PRODUCERS},
    }
    return events, integrity


def difference(new: int, old: int) -> int:
    return (new - old) & UINT32_MASK


def frame_rows(events: list[Event]) -> tuple[list[dict], list[dict]]:
    rows = {}
    phases = []
    phase = "unmarked"
    segment = 0
    exclusion_epoch = 0
    capture = False
    last_flip = None
    for event in events:
        if event.kind == PHASE:
            phase = PHASES[event.arg0]
            segment += 1
            phases.append({"timestamp_ns": event.timestamp, "segment": segment,
                           "phase": phase, "manual": bool(event.arg1)})
        elif event.kind in (CAPTURE_BEGIN, CAPTURE_END):
            capture = event.kind == CAPTURE_BEGIN
            exclusion_epoch += 1
        elif event.kind == FLIP:
            previous, previous_segment, previous_epoch = last_flip or (None, None, None)
            row = {
                "frame_id": event.frame,
                "phase": phase,
                "segment": segment,
                "flip_ns": event.timestamp,
                "handoff_ns": None,
                "draw_ns": None,
                "frame_ms": None,
                "guest_ticks": None,
                "emulated_vblanks": None,
                "flip_commands": None,
                "excluded": ("capture" if capture else "first_frame" if previous is None else
                             "missing_flip" if difference(event.frame, previous.frame) != 1 else
                             "phase_boundary" if segment != previous_segment else
                             "capture_boundary" if exclusion_epoch != previous_epoch else ""),
            }
            if previous is not None and difference(event.frame, previous.frame) == 1:
                row["frame_ms"] = (event.timestamp - previous.timestamp) / 1e6
                row["guest_ticks"] = difference(event.arg0, previous.arg0)
                row["emulated_vblanks"] = difference(event.arg1, previous.arg1)
                row["flip_commands"] = difference(event.arg2, previous.arg2)
            rows[event.frame] = row
            last_flip = (event, segment, exclusion_epoch)
        elif event.frame in rows:
            key = "handoff_ns" if event.kind == HANDOFF else "draw_ns"
            row = rows[event.frame]
            if row[key] is None:
                row[key] = event.timestamp
    return list(rows.values()), phases


def percentile(samples: list[float], q: float) -> float | None:
    return samples[math.ceil(len(samples) * q) - 1] if samples else None


def summarize(rows: list[dict]) -> dict:
    all_phases = {}
    for row in rows:
        all_phases.setdefault(row["phase"], []).append(row)
    phases = {}
    for phase, group in all_phases.items():
        measured = [row for row in group if not row["excluded"]]
        samples = sorted(row["frame_ms"] for row in measured)
        duration_ms = sum(samples)
        handoffs = [row for row in measured if row["handoff_ns"] is not None]
        draws = [row for row in measured if row["draw_ns"] is not None]
        flip_to_handoff = sorted((row["handoff_ns"] - row["flip_ns"]) / 1e6 for row in handoffs)
        handoff_to_draw = sorted((row["draw_ns"] - row["handoff_ns"]) / 1e6
                                 for row in draws if row["handoff_ns"] is not None)
        phases[phase] = {
            "flip_count": len(group),
            "measured_intervals": len(samples),
            "excluded_intervals": dict(Counter(row["excluded"] for row in group if row["excluded"])),
            "mean_fps": len(samples) * 1000 / duration_ms if duration_ms else None,
            "median_ms": statistics.median(samples) if samples else None,
            "p95_ms": percentile(samples, .95),
            "p99_ms": percentile(samples, .99),
            "over_33_33_ms": sum(x > 1000 / 30 for x in samples),
            "over_50_ms": sum(x > 50 for x in samples),
            "host_handoffs": len(handoffs),
            "host_draws": len(draws),
            "flip_to_handoff_p95_ms": percentile(flip_to_handoff, .95),
            "handoff_to_draw_p95_ms": percentile(handoff_to_draw, .95),
            "guest_tick_hz": sum(row["guest_ticks"] for row in measured) * 1000 / duration_ms if duration_ms else None,
            "emulated_vblank_hz": sum(row["emulated_vblanks"] for row in measured) * 1000 / duration_ms if duration_ms else None,
        }
    return phases


def export(run_dir: Path) -> dict:
    manifest = json.loads((run_dir / "run.json").read_text(encoding="utf-8-sig"))
    events, integrity = read_trace(run_dir / "frame-trace.bin")
    rows, phase_markers = frame_rows(events)
    # Report source status alongside the data even if the writer reached EOF
    # after a watchdog exit. Runs with lost phase/capture markers are suspect.
    summary = {"schema_version": 1, "run_id": manifest["run_id"], "mode": manifest["mode"],
               "status": manifest["status"], "integrity": integrity,
               "phase_markers": phase_markers, "phases": summarize(rows)}
    with (run_dir / "frames.csv").open("w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(stream, fieldnames=(
            "frame_id", "phase", "segment", "flip_ns", "handoff_ns", "draw_ns",
            "frame_ms", "guest_ticks", "emulated_vblanks", "flip_commands", "excluded"))
        writer.writeheader()
        writer.writerows(rows)
    (run_dir / "frame-summary.json").write_text(json.dumps(summary, indent=2) + "\n", encoding="utf-8")
    return summary


def compare(reference: Path, candidate: Path) -> dict:
    before, after = export(reference), export(candidate)
    warnings = []
    for run in (before, after):
        integrity = run["integrity"]
        if run["mode"] != "Benchmark" or run["status"] != "exited":
            warnings.append(f'{run["run_id"]} mode={run["mode"]} status={run["status"]}')
        if (not integrity["complete"] or any(integrity["dropped"].values())
                or any(integrity["sequence_gaps"].values())):
            warnings.append(f'{run["run_id"]} has an incomplete trace or dropped records')
    before_manifest = json.loads((reference / "run.json").read_text(encoding="utf-8-sig"))
    after_manifest = json.loads((candidate / "run.json").read_text(encoding="utf-8-sig"))
    changed = {}
    for key in ("mode", "guest_pc_tracking", "xbe_sha256", "embedded_shader_sha256",
                "title_revision", "runtime_revision", "runtime_pin", "executable_sha256",
                "nv2a_raster_source_sha256", "atv_archive_sha256", "raster_adapter"):
        values = [before_manifest.get(key), after_manifest.get(key)]
        if values[0] != values[1]:
            changed[key] = values
    left, right = (manifest.get("environment", {}) for manifest in (before_manifest, after_manifest))
    environment = {key: [left.get(key), right.get(key)] for key in sorted(left.keys() | right.keys())
                   if key not in ("RECOMP_GPU_CAPTURE", "RECOMP_WATCHDOG_SECS")
                   and left.get(key) != right.get(key)}
    phases = {}
    for phase in sorted(before["phases"].keys() | after["phases"].keys()):
        a, b = before["phases"].get(phase), after["phases"].get(phase)
        delta = None
        if a and b:
            delta = {k: b[k] - a[k] for k in ("mean_fps", "median_ms", "p95_ms", "p99_ms")
                     if a[k] is not None and b[k] is not None}
        phases[phase] = {"reference": a, "candidate": b, "delta": delta}
    return {"reference": before["run_id"], "candidate": after["run_id"], "warnings": warnings,
            "manifest_differences": changed, "environment_differences": environment, "phases": phases}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    commands = parser.add_subparsers(dest="command", required=True)
    commands.add_parser("export").add_argument("run", type=Path)
    comparison = commands.add_parser("compare")
    comparison.add_argument("reference", type=Path)
    comparison.add_argument("candidate", type=Path)
    args = parser.parse_args()
    try:
        if args.command == "export":
            summary = export(args.run)
            integrity = summary["integrity"]
            if not integrity["complete"] or any(integrity["dropped"].values()) or any(integrity["sequence_gaps"].values()):
                print("Frame trace is incomplete or has dropped records.", file=sys.stderr)
            print(f'Exported {sum(p["flip_count"] for p in summary["phases"].values())} flips from {args.run}')
        else:
            print(json.dumps(compare(args.reference, args.candidate), indent=2))
    except (OSError, ValueError, KeyError) as error:
        print(f"Frame trace: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
