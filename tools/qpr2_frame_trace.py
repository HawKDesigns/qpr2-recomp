#!/usr/bin/env python3
"""Export and compare QPR2 frame pacing on a single host clock."""
from __future__ import annotations

import argparse
from bisect import bisect_left
from collections import Counter
import csv
from dataclasses import dataclass
import json
import math
from pathlib import Path
import statistics
import struct
import sys
from qpr2_debug_metadata import number

HEADER = struct.Struct("<4sIII")
RECORD = struct.Struct("<Q8I")
(FLIP, HANDOFF, DRAW, PHASE, CAPTURE_BEGIN, CAPTURE_END, CPU_METRIC, GPU_SAMPLE,
 GUEST_FENCE_WAIT, PAUSE, PROFILE_FRAME, THREAD_CPU, GUEST_PC, DEPTH_LAYOUT,
 DEPTH_DRAIN, SCENE_ACTIVITY, OVERLAY, VBLANK_EVENTS) = range(1, 19)
CPU_FIELDS = (
    "fifo_ms", "upload_ms", "geometry_submit_ms", "raster_submit_ms",
    "target_bind_ms", "current_map_ms", "queued_map_ms", "publication_ms",
    "completion_map_ms", "report_map_ms", "batches", "submitted_triangles",
    "draw_requests", "gpu_selected", "gpu_dropped", "gpu_invalid",
    "begin_end_sample_ms", "begin_end_samples", "fifo_words",
)
DEPENDENCIES = ("guest_access", "guest_read", "target_color", "target_depth", "texture_raw", "texture_palette")
TARGET_SIZES = ("640x480", "256x256", "128x128", "other")
GPU_CLASSES = ("prepared", "raw", "copy")
GPU_PHASES = ("vertex", "prepare", "pixels", "copy")
RAW_BOUNDARY = ("raw_boundary_numeric", "raw_boundary_w", "raw_boundary_xy",
                "raw_boundary_z", "raw_boundary_point_limit")
RAW_COUNTS = ("raw_boundary_draws", "raw_boundary_vertices")
DEPTH_CLEAR_COUNTS = ("depth_copies_retired_by_clear",)
COMPLETION_COUNTS = ("completion_markers", "completion_markers_reused",
                     "completion_early_releases", "completion_forced_markers")
PROFILE_COUNTS = DEPTH_CLEAR_COUNTS + COMPLETION_COUNTS
FOOTER = 0xFFFFFFFF
PRODUCERS = ("renderer", "window", "title")
PHASES = ("unmarked", "boot", "preview", "countdown", "ahead", "behind", "unmarked", "video",
          "rider_menu", "atv_menu")
THREAD_ROLES = ("guest_main", "fifo_executor", "gpu_startup", "presenter")
WINDOW_NS = 5_000_000_000
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
        if len(header) != HEADER.size:
            raise ValueError("missing frame trace header")
        magic, version, size, producers = HEADER.unpack(header)
        if (magic != b"QPTR" or version not in (1, 2, 3, 4) or size != RECORD.size or
                producers != len(PRODUCERS)):
            raise ValueError("unsupported frame trace header")
        last_event = (CAPTURE_END, PROFILE_FRAME, OVERLAY, VBLANK_EVENTS)[version - 1]
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
            if producer in dropped or event.kind not in range(FLIP, last_event + 1):
                raise ValueError("invalid frame trace event")
            if event.kind in (PHASE, SCENE_ACTIVITY) and not 0 < event.arg0 < len(PHASES):
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


def frame_rows(events: list[Event], exclude_segments=()) -> tuple[list[dict], list[dict]]:
    rows = {}
    phases = []
    phase = "unmarked"
    segment = 0
    exclusion_epoch = pause_epoch = overlay_epoch = 0
    capture = False
    paused = False
    overlay = False
    last_flip = None
    manual = False
    activity = None
    for event in events:
        if event.kind == PHASE:
            phase = PHASES[event.arg0]
            manual = bool(event.arg1) and phase != "unmarked"
            segment += 1
            phases.append({"timestamp_ns": event.timestamp, "segment": segment,
                           "phase": phase, "manual": bool(event.arg1)})
        elif event.kind == SCENE_ACTIVITY:
            activity = (PHASES[event.arg0], event.timestamp)
        elif event.kind in (CAPTURE_BEGIN, CAPTURE_END, OVERLAY):
            if event.kind == OVERLAY:
                overlay = bool(event.arg0)
                overlay_epoch += 1
            else:
                capture = event.kind == CAPTURE_BEGIN
                exclusion_epoch += 1
        elif event.kind == PAUSE:
            paused = bool(event.arg0)
            pause_epoch += 1
        elif event.kind == FLIP:
            if not manual and activity:
                active = activity[0] if event.timestamp - activity[1] <= 500_000_000 else "unmarked"
                if active != phase:
                    phase = active
                    segment += 1
                    phases.append({"timestamp_ns": event.timestamp, "segment": segment,
                                   "phase": phase, "manual": False, "source": "activity"})
                if active == "unmarked":
                    activity = None
            previous, previous_segment, previous_epoch, previous_pause, previous_overlay = (
                last_flip or (None, None, None, None, None))
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
                "excluded": ("manual_exclusion" if segment in exclude_segments else
                             "pause" if paused else "capture" if capture else "overlay" if overlay else
                             "first_frame" if previous is None else
                             "missing_flip" if difference(event.frame, previous.frame) != 1 else
                             "phase_boundary" if segment != previous_segment else
                             "pause_boundary" if pause_epoch != previous_pause else
                             "overlay_boundary" if overlay_epoch != previous_overlay else
                             "capture_boundary" if exclusion_epoch != previous_epoch else ""),
            }
            if previous is not None and difference(event.frame, previous.frame) == 1:
                row["frame_ms"] = (event.timestamp - previous.timestamp) / 1e6
                row["guest_ticks"] = difference(event.arg0, previous.arg0)
                row["emulated_vblanks"] = difference(event.arg1, previous.arg1)
                row["flip_commands"] = difference(event.arg2, previous.arg2)
            rows[event.frame] = row
            last_flip = (event, segment, exclusion_epoch, pause_epoch, overlay_epoch)
        elif event.kind in (HANDOFF, DRAW) and event.frame in rows:
            key = "handoff_ns" if event.kind == HANDOFF else "draw_ns"
            row = rows[event.frame]
            if row[key] is None:
                row[key] = event.timestamp
    return list(rows.values()), phases


def percentile(samples: list[float], q: float) -> float | None:
    return samples[math.ceil(len(samples) * q) - 1] if samples else None


def summarize(rows: list[dict], delivered_vblanks=None) -> dict:
    all_phases = {}
    for row in rows:
        all_phases.setdefault(row["phase"], []).append(row)
    phases = {}
    for phase, group in all_phases.items():
        measured = [row for row in group if not row["excluded"]]
        samples = sorted(row["frame_ms"] for row in measured)
        duration_ms = sum(samples)
        kernel_vblanks = sum(row["emulated_vblanks"] for row in measured)
        delivered = [delivered_vblanks[row["frame_id"]] for row in measured
                     if delivered_vblanks and row["frame_id"] in delivered_vblanks]
        display_rate = sum(row["guest_ticks"] for row in measured) * 1000 / duration_ms if duration_ms else None
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
            "max_ms": max(samples, default=None),
            "duration_ms": duration_ms,
            "sustained_30fps": bool(samples) and samples[-1] <= 1000 / 30,
            "sustained_60fps": bool(samples) and samples[-1] <= 1000 / 60,
            "over_16_67_ms": sum(x > 1000 / 60 for x in samples),
            "over_33_33_ms": sum(x > 1000 / 30 for x in samples),
            "over_50_ms": sum(x > 50 for x in samples),
            "over_100_ms": sum(x > 100 for x in samples),
            "host_handoffs": len(handoffs),
            "host_draws": len(draws),
            "flip_to_handoff_p95_ms": percentile(flip_to_handoff, .95),
            "handoff_to_draw_p95_ms": percentile(handoff_to_draw, .95),
            "guest_tick_hz": display_rate,
            "guest_display_vblank_hz": display_rate,
            "emulated_vblank_hz": display_rate,  # Compatibility with the research CLI.
            "delivered_vblank_events_hz": (sum(delivered) * 1000 / duration_ms
                                            if duration_ms and len(delivered) == len(measured) else None),
            "kernel_interrupt_vblank_hz": kernel_vblanks * 1000 / duration_ms if kernel_vblanks else None,
        }
    return phases


def diagnostic_windows(events: list[Event], rows: list[dict], sections: list[dict]) -> list[dict]:
    ranges = [(number(s["virtual_addr"]), number(s["virtual_addr"]) + s["virtual_size"], s["name"])
              for s in sections if s["executable"]]
    windows = {}
    previous = {}
    for event in events:
        if event.kind == THREAD_CPU and event.frame < len(THREAD_ROLES):
            role = THREAD_ROLES[event.frame]
            ticks = event.arg0 | (event.arg1 << 32)
            before = previous.get(role)
            previous[role] = (event.timestamp, ticks, event.arg2)
            if not before or before[2] != event.arg2 or ticks < before[1]:
                continue
            start, delta = before[0], ticks - before[1]
            span = event.timestamp - start
            if span <= 0:
                continue
            # Apportion an interval that straddles a report boundary. The
            # cumulative samples cannot locate the work within that interval.
            for index in range(start // WINDOW_NS, (event.timestamp - 1) // WINDOW_NS + 1):
                overlap = min(event.timestamp, (index + 1) * WINDOW_NS) - max(start, index * WINDOW_NS)
                window = windows.setdefault(index, {})
                cpu = window.setdefault("threads", {}).setdefault(role, {"cpu_ms": 0.0, "observed_ms": 0.0})
                cpu["cpu_ms"] += delta / 10_000 * overlap / span
                cpu["observed_ms"] += overlap / 1_000_000
        elif event.kind == GUEST_PC:
            bucket = next((name for start, end, name in ranges if start <= event.arg0 < end),
                          "outside_executable_sections" if event.arg0 else "unavailable")
            window = windows.setdefault(event.timestamp // WINDOW_NS, {})
            window.setdefault("guest_pc_samples", Counter())[bucket] += 1
    for row in rows:
        window = windows.setdefault(row["flip_ns"] // WINDOW_NS, {})
        key = "excluded_intervals" if row["excluded"] else "measured_scene_intervals"
        window.setdefault(key, Counter())[row["excluded"] or row["phase"]] += 1
    for window in windows.values():
        for cpu in window.get("threads", {}).values():
            cpu["percent_of_one_core"] = 100 * cpu["cpu_ms"] / cpu["observed_ms"]
    return [dict(start_ns=i * WINDOW_NS, end_ns=(i + 1) * WINDOW_NS, **window)
            for i, window in sorted(windows.items())]


def depth_dependencies(events: list[Event], rows: list[dict]) -> dict:
    frames = {row["frame_id"]: row for row in rows}
    layouts = {}
    drains = {}
    last = None
    for event in events:
        if event.kind == DEPTH_LAYOUT:
            layout = {
                "zeta_offset": f"0x{event.arg0:08X}", "width": event.arg1 & 0xFFFF,
                "height": event.arg1 >> 16, "surface_type": (event.arg2 >> 8) & 15,
                "depth_format": (event.arg2 >> 4) & 15, "float_depth": bool(event.arg2 & 0x8000),
                "zeta_pitch": event.arg2 >> 16,
            }
            last = tuple(layout.values())
            layouts[last] = layout
        elif event.kind == DEPTH_DRAIN:
            frame = frames.get(event.frame)
            scene = frame["excluded"] or frame["phase"] if frame else "no_flip"
            key = (scene, event.arg0, last, event.arg1 & 3, event.arg1 >> 8)
            drain = drains.setdefault(key, {"count": 0, "total_ms": 0.0, "max_ms": 0.0})
            drain["count"] += 1
            drain["total_ms"] += event.arg2 / 1000
            drain["max_ms"] = max(drain["max_ms"], event.arg2 / 1000)
    return {
        "observed_bindings": list(layouts.values()),
        "drains": [dict(scene=scene, producer_width=extent & 0xFFFF, producer_height=extent >> 16,
                        most_recent_binding=layouts.get(binding), publishes_depth=bool(flags & 1),
                        deferred_surface_copy=bool(flags & 2), queue_depth=queue, **stats)
                   for (scene, extent, binding, flags, queue), stats in drains.items()],
    }


def frame_profile(events: list[Event], rows: list[dict], integrity: dict) -> tuple[list[dict], dict]:
    frames = {row["frame_id"]: {**row, "profile_complete": False, "gpu_samples": 0,
                                "guest_fence_wait_ms": 0.0, "guest_fence_waits_ended": 0} for row in rows}
    dependencies = tuple(f"{name}_{size}_ms" for name in DEPENDENCIES for size in TARGET_SIZES)
    gpu_fields = tuple(f"gpu_{batch}_{phase}_sample_ms" for batch in GPU_CLASSES for phase in GPU_PHASES)
    drain_counts = tuple(f"{name}_drains" for name in DEPENDENCIES)
    gpu_counts = tuple(key.removesuffix("_ms") + "_count" for key in gpu_fields)
    guest_waits = []
    orphan_gpu_samples = 0
    for event in events:
        row = frames.get(event.frame)
        if event.kind == CPU_METRIC and row is not None:
            if event.arg0 < len(CPU_FIELDS):
                key = CPU_FIELDS[event.arg0]
            elif 24 <= event.arg0 < 24 + len(drain_counts):
                key = drain_counts[event.arg0 - 24]
            elif 32 <= event.arg0 < 32 + len(dependencies):
                key = dependencies[event.arg0 - 32]
            elif 30 <= event.arg0 < 32:
                key = RAW_COUNTS[event.arg0 - 30]
            elif 56 <= event.arg0 < 56 + len(RAW_BOUNDARY):
                key = RAW_BOUNDARY[event.arg0 - 56]
            elif 61 <= event.arg0 < 61 + len(PROFILE_COUNTS):
                key = PROFILE_COUNTS[event.arg0 - 61]
            else:
                continue
            row[key] = event.arg1 / 1000 if key.endswith("_ms") else event.arg1
        elif event.kind == PROFILE_FRAME and row is not None:
            row["profile_complete"] = True
        elif event.kind == GPU_SAMPLE and event.arg0 < len(gpu_fields):
            if row is None:
                orphan_gpu_samples += 1
                continue
            key = gpu_fields[event.arg0]
            row[key] = row.get(key, 0.0) + event.arg1 / 1000
            count_key = key.removesuffix("_ms") + "_count"
            row[count_key] = row.get(count_key, 0) + 1
            row["gpu_samples"] += 1
        elif event.kind == GUEST_FENCE_WAIT:
            guest_waits.append(event)

    # A guest wait can span multiple flips and overlap FIFO work on another
    # thread. Split the time at interval boundaries without adding it to FIFO.
    ends = [row["flip_ns"] for row in rows]
    for event in guest_waits:
        begin = event.timestamp - event.arg0 * 1000
        first = bisect_left(ends, begin)
        last = bisect_left(ends, event.timestamp)
        if last < len(rows):
            frames[rows[last]["frame_id"]]["guest_fence_waits_ended"] += 1
        for i in range(max(1, first), min(last + 1, len(rows))):
            overlap = min(ends[i], event.timestamp) - max(ends[i - 1], begin)
            frames[rows[i]["frame_id"]]["guest_fence_wait_ms"] += max(0, overlap) / 1e6

    data = list(frames.values())
    for row in data:
        if row["profile_complete"]:
            for key in CPU_FIELDS + dependencies + drain_counts + RAW_COUNTS + RAW_BOUNDARY + PROFILE_COUNTS:
                row.setdefault(key, 0)
    eligible = [row for row in data if row["profile_complete"] and not row["excluded"]]
    complete = (integrity["complete"] and not any(integrity["dropped"].values())
                and not any(integrity["sequence_gaps"].values()))
    cohorts = {}
    for phase in dict.fromkeys(row["phase"] for row in eligible):
        group = [row for row in eligible if row["phase"] == phase]
        slow = [row for row in group if row["frame_ms"] > 1000/30]
        fast = [row for row in group if row["frame_ms"] <= 1000/30]
        comparison_keys = CPU_FIELDS[:13] + ("guest_fence_wait_ms",)
        cohorts[phase] = {
            "frames": len(group),
            "over_33ms_frames": len(slow),
            "slow_and_fast_frame_means": {
                label: {key: statistics.mean(row[key] for row in sample)
                        for key in comparison_keys} if sample else None
                for label, sample in (("over_33ms", slow), ("within_33ms", fast))
            },
            "p95_ms": percentile(sorted(row["frame_ms"] for row in group), .95),
            "per_frame_mean": {key: statistics.mean(row[key] for row in group)
                               for key in CPU_FIELDS + dependencies + drain_counts + RAW_COUNTS + RAW_BOUNDARY +
                               PROFILE_COUNTS + ("guest_fence_wait_ms",)},
            "gpu_samples_by_frame_time": {
                label: {
                    key.removesuffix("_sample_ms"): {"total_ms": sum(row.get(key, 0) for row in sample),
                                                    "samples": sum(row.get(count, 0) for row in sample)}
                    for key, count in zip(gpu_fields, gpu_counts)
                }
                for label, sample in (("over_33ms", slow), ("within_33ms", fast))
            },
            "gpu_phase_samples": sum(row["gpu_samples"] for row in group),
        }
    worst = sorted(eligible, key=lambda row: row["frame_ms"], reverse=True)[:25]
    return data, {"schema_version": 1, "trace_complete_without_loss": complete,
                  "profiled_frames": sum(row["profile_complete"] for row in data),
                  "gpu_samples_without_flip": orphan_gpu_samples, "phases": cohorts,
                  "slowest_frames": worst,
                  "columns": list(CPU_FIELDS + dependencies + drain_counts + RAW_COUNTS + RAW_BOUNDARY +
                      PROFILE_COUNTS + gpu_fields + gpu_counts + ("profile_complete",
                      "gpu_samples", "guest_fence_wait_ms", "guest_fence_waits_ended"))}


def export(run_dir: Path, exclude_segments=()) -> dict:
    manifest = json.loads((run_dir / "run.json").read_text(encoding="utf-8-sig"))
    events, integrity = read_trace(run_dir / "frame-trace.bin")
    excluded = frozenset(exclude_segments)
    rows, phase_markers = frame_rows(events, excluded)
    delivered_vblanks = {}
    last_vblank = None
    for event in events:
        if event.kind == VBLANK_EVENTS:
            if last_vblank and difference(event.frame, last_vblank.frame) == 1:
                delivered_vblanks[event.frame] = difference(event.arg0, last_vblank.arg0)
            last_vblank = event
    if excluded - {marker["segment"] for marker in phase_markers} - {0}:
        raise ValueError("unknown phase segment to exclude")
    # Report source status alongside the data even if the writer reached EOF
    # after a watchdog exit. Runs with lost phase/capture markers are suspect.
    warnings = []
    if not any(mark["manual"] and mark["phase"] in ("countdown", "ahead", "behind")
               for mark in phase_markers):
        warnings.append("no manual phase marks: countdown, ahead and behind cannot be separated")
    summary = {"schema_version": 2, "run_id": manifest["run_id"], "mode": manifest["mode"],
               "status": manifest["status"], "integrity": integrity, "warnings": warnings,
               "phase_markers": phase_markers, "phases": summarize(rows, delivered_vblanks)}
    if excluded:
        summary["manually_excluded_segments"] = sorted(excluded)
    frame_columns = (
        "frame_id", "phase", "segment", "flip_ns", "handoff_ns", "draw_ns",
        "frame_ms", "guest_ticks", "emulated_vblanks", "flip_commands", "excluded")
    with (run_dir / "frames.csv").open("w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(stream, fieldnames=frame_columns)
        writer.writeheader()
        writer.writerows(rows)
    if any(event.kind in (THREAD_CPU, GUEST_PC) for event in events):
        memory_map = run_dir / "memory-map.json"
        sections = []
        if memory_map.is_file():
            mapping = json.loads(memory_map.read_text(encoding="utf-8-sig"))
            if mapping.get("xbe_sha256") == manifest.get("xbe_sha256"):
                sections = [{"name": section["name"], "virtual_addr": section["guest_va"],
                             "virtual_size": number(section["end_exclusive"]) - number(section["guest_va"]),
                             "executable": section["declared_executable"]}
                            for section in mapping["ranges"] if section["source"] == "xbe_section_table"]
        summary["diagnostic_windows"] = diagnostic_windows(events, rows, sections)
        summary["guest_pc_sections_available"] = bool(sections)
    if any(event.kind == OVERLAY for event in events):
        summary["overlay_pacing"] = summarize([
            {**row, "excluded": "" if row["excluded"] == "overlay" else row["excluded"]}
            for i, row in enumerate(rows)
            if i and row["excluded"] == rows[i - 1]["excluded"] == "overlay"
            and row["segment"] == rows[i - 1]["segment"] and row["frame_ms"] is not None
        ])
    if rows and any(event.kind == PROFILE_FRAME for event in events):
        summary["profile_exported"] = True
        profiled, profile_summary = frame_profile(events, rows, integrity)
        profile_summary["depth_dependencies"] = depth_dependencies(events, rows)
        columns = profile_summary.pop("columns")
        with (run_dir / "frame-profile.csv").open("w", newline="", encoding="utf-8") as stream:
            writer = csv.DictWriter(stream, fieldnames=list(frame_columns) + columns)
            writer.writeheader()
            writer.writerows(profiled)
        (run_dir / "profile-summary.json").write_text(
            json.dumps(profile_summary, indent=2) + "\n", encoding="utf-8")
    (run_dir / "frame-summary.json").write_text(json.dumps(summary, indent=2) + "\n", encoding="utf-8")
    return summary


def compare(reference: Path, candidate: Path, exclude_reference=(), exclude_candidate=()) -> dict:
    before, after = export(reference, exclude_reference), export(candidate, exclude_candidate)
    warnings = []
    for run in (before, after):
        integrity = run["integrity"]
        if run["mode"] != "Benchmark" or run["status"] != "exited":
            warnings.append(f'{run["run_id"]} mode={run["mode"]} status={run["status"]}')
        if (not integrity["complete"] or any(integrity["dropped"].values())
                or any(integrity["sequence_gaps"].values())):
            warnings.append(f'{run["run_id"]} has an incomplete trace or dropped records')
        warnings.extend(f'{run["run_id"]}: {text}' for text in run["warnings"])
    before_manifest = json.loads((reference / "run.json").read_text(encoding="utf-8-sig"))
    after_manifest = json.loads((candidate / "run.json").read_text(encoding="utf-8-sig"))
    changed = {}
    for key in ("mode", "guest_pc_tracking", "xbe_sha256", "embedded_shader_sha256",
                "title_revision", "runtime_revision", "runtime_pin", "executable_sha256",
                "nv2a_raster_source_sha256", "atv_archive_sha256", "raster_adapter", "cpu",
                "toolchain", "gpu_driver"):
        values = [before_manifest.get(key), after_manifest.get(key)]
        if values[0] != values[1]:
            changed[key] = values
    left, right = (manifest.get("environment", {}) for manifest in (before_manifest, after_manifest))
    environment = {key: [left.get(key), right.get(key)] for key in sorted(left.keys() | right.keys())
                   if key not in ("RECOMP_GPU_CAPTURE", "RECOMP_WATCHDOG_SECS")
                   and left.get(key) != right.get(key)}
    profiles = []
    for folder, summary in ((reference, before), (candidate, after)):
        path = folder / "profile-summary.json"
        profiles.append(json.loads(path.read_text(encoding="utf-8"))["phases"]
                        if summary.get("profile_exported") else {})
    phases = {}
    for phase in sorted(before["phases"].keys() | after["phases"].keys()):
        a, b = before["phases"].get(phase), after["phases"].get(phase)
        delta = None
        if a and b:
            delta = {k: b[k] - a[k] for k in ("mean_fps", "median_ms", "p95_ms", "p99_ms", "max_ms")
                     if a[k] is not None and b[k] is not None}
        phases[phase] = {"reference": a, "candidate": b, "delta": delta}
        for label, profile in zip(("reference", "candidate"), profiles):
            if phase in profile:
                mean = profile[phase]["per_frame_mean"]
                phases[phase][label + "_profile"] = {
                    key: mean[key] for key in ("draw_requests", "fifo_ms", "queued_map_ms",
                        "completion_map_ms", "target_depth_drains", *PROFILE_COUNTS)
                } | {"target_depth_ms": sum(mean[f"target_depth_{size}_ms"] for size in TARGET_SIZES)}
    return {"reference": before["run_id"], "candidate": after["run_id"], "warnings": warnings,
            "excluded_segments": {"reference": before.get("manually_excluded_segments", []),
                                  "candidate": after.get("manually_excluded_segments", [])},
            "manifest_differences": changed, "environment_differences": environment, "phases": phases}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    commands = parser.add_subparsers(dest="command", required=True)
    exporter = commands.add_parser("export")
    exporter.add_argument("run", type=Path)
    exporter.add_argument("--exclude-segment", action="append", default=[], type=int,
                          help="omit a mislabeled segment from statistics; use the segment column in frames.csv")
    comparison = commands.add_parser("compare")
    comparison.add_argument("reference", type=Path)
    comparison.add_argument("candidate", type=Path)
    comparison.add_argument("--exclude-reference-segment", action="append", default=[], type=int)
    comparison.add_argument("--exclude-candidate-segment", action="append", default=[], type=int)
    args = parser.parse_args()
    try:
        if args.command == "export":
            summary = export(args.run, args.exclude_segment)
            integrity = summary["integrity"]
            if not integrity["complete"] or any(integrity["dropped"].values()) or any(integrity["sequence_gaps"].values()):
                print("Frame trace is incomplete or has dropped records.", file=sys.stderr)
            print(f'Exported {sum(p["flip_count"] for p in summary["phases"].values())} flips from {args.run}')
        else:
            print(json.dumps(compare(args.reference, args.candidate, args.exclude_reference_segment,
                                     args.exclude_candidate_segment), indent=2))
    except (OSError, ValueError, KeyError) as error:
        print(f"Frame trace: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
