"""Recreate QPR2's three exact literal material shapes from local run logs.

Pass the v126 stderr log from a normal run through a 3D menu, preview, and
race. [GPU-MATERIAL-PROGRAM] records include each distinct combiner state;
F8 captures (.bin) remain supported but are not required. Logs, captures,
and the resulting JSON contain game-derived state and must remain local.
The fingerprints are of exact state words, so incomplete or different input
fails rather than silently selecting an unreviewed material.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct


MAGIC = 0x51475235
FIELDS = ("rgb_in", "alpha_in", "rgb_out", "alpha_out")
# SHA-256 of control, stage program, final1 and the active words of each field,
# in little-endian uint32 order. These select the three reviewed v126 classes.
SHAPES = {
    "7d1599e1dc0ba85f48471d0661729bb3cd4df012b93b068ec55e148d507e1d7a": 1,
    "11458e542c8664c1fea8d61df35c63922071e0093b99f61671198ad71ea39bc0": 2,
    "8df78d1449c6ed59b4508c7efdfb3486f25d70d1d7f685762200aa52cba80db2": 3,
}
FINAL_SIGNATURES = {
    1: "4e1ceb7cd6e56a5f049fcfddf019fff7508322e6ec03c030af5e65cde34a41cb",
    2: "05efbd5949d1a131542d031ce556e6aab7ccaed3b2f92ca39e69b89f9c7bab0f",
    3: "4e1ceb7cd6e56a5f049fcfddf019fff7508322e6ec03c030af5e65cde34a41cb",
}
COUNTS = {1: 1, 2: 3, 3: 1}


def digest(words):
    return hashlib.sha256(struct.pack("<%dI" % len(words), *words)).hexdigest()


def log_states(path):
    """Read per-program combiner snapshots emitted by the normal v126 run."""
    keys = ("id", "control", "stages", "final0", "final1", *FIELDS)
    seen = 0
    with path.open("r", encoding="utf-8", errors="replace") as stream:
        for line_number, line in enumerate(stream, 1):
            if not line.startswith("[GPU-MATERIAL-PROGRAM] "):
                continue
            seen += 1
            try:
                record = dict(part.split("=", 1) for part in line.split()[1:])
                if not all(key in record for key in keys):
                    raise ValueError("missing field")
                words = [0] * 54
                for i, key in enumerate(FIELDS):
                    values = record[key].split(",")
                    if len(values) != 8 or any(len(value) != 8 for value in values):
                        raise ValueError(f"{key} must contain eight instruction words")
                    words[i * 8:i * 8 + 8] = [int(value, 16) for value in values]
                for index, key in ((48, "final0"), (49, "final1"),
                                   (52, "control"), (53, "stages")):
                    if len(record[key]) != 8:
                        raise ValueError(f"{key} must be an eight-digit word")
                    words[index] = int(record[key], 16)
            except (ValueError, KeyError) as exc:
                raise ValueError(f"{path}:{line_number}: invalid material program: {exc}") from exc
            yield words
    if not seen:
        raise ValueError(f"{path}: no GPU-MATERIAL-PROGRAM records; use the v126 stderr log after a 3D scene")


def states(path):
    if path.suffix.lower() != ".bin":
        yield from log_states(path)
        return
    with path.open("rb") as stream:
        header = stream.read(16)
        if len(header) != 16:
            raise ValueError(f"{path}: missing capture header")
        magic, version, state_size, triangle_size = struct.unpack("<4I", header)
        if magic != MAGIC or version not in (1, 2) or state_size < 216 or triangle_size not in (448, 688):
            raise ValueError(f"{path}: unsupported GPU capture format")
        while prefix := stream.read(8):
            if len(prefix) != 8:
                raise ValueError(f"{path}: truncated record header")
            kind, length = struct.unpack("<2I", prefix)
            if length > 192 * 1024 * 1024:
                raise ValueError(f"{path}: oversized record")
            payload = stream.read(length)
            if len(payload) != length:
                raise ValueError(f"{path}: truncated record payload")
            if kind != 3:
                continue
            if length < 12:
                raise ValueError(f"{path}: short draw record")
            _, count, triangles = struct.unpack_from("<3I", payload)
            if length != 12 + count * state_size + triangles * triangle_size:
                raise ValueError(f"{path}: inconsistent draw record length")
            for i in range(count):
                yield struct.unpack_from("<54I", payload, 12 + i * state_size)


def extract(paths):
    found = {}
    for path in paths:
        for words in states(path):
            control, stage_program = words[52:54]
            n = control & 255
            if not 0 < n <= 8:
                continue
            arrays = [list(words[i * 8:i * 8 + 8]) for i in range(4)]
            shape = digest([control, stage_program, words[49]] +
                           [item for array in arrays for item in array[:n]])
            cls = SHAPES.get(shape)
            if cls is None:
                continue
            item = found.setdefault(cls, {
                "control": control, "stages": stage_program, "final0": words[48],
                "final1": words[49], "id": 0,
                **dict(zip(FIELDS, arrays)), "class": cls, "finals": set(),
            })
            item["finals"].add(words[48])
    missing = []
    for cls in (1, 2, 3):
        item = found.get(cls)
        if item is None or len(item["finals"]) < COUNTS[cls]:
            missing.append(f"class {cls}: {len(item['finals']) if item else 0}/{COUNTS[cls]} final variants")
            continue
        item["finals"] = sorted(item["finals"])
        if len(item["finals"]) != COUNTS[cls] or digest(item["finals"]) != FINAL_SIGNATURES[cls]:
            raise ValueError(f"class {cls}: unexpected final variants; check capture and game version")
        item["final0"] = item["finals"][0]
    if missing:
        raise ValueError("Incomplete input; pass stderr logs from menu, preview and race: " + ", ".join(missing))
    return [found[cls] for cls in (1, 2, 3)]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("captures", nargs="+", type=Path, help="local v126 stderr .log files or F8 .bin captures")
    parser.add_argument("--output", type=Path, default=Path("tools/fixtures/qpr2-literal-programs.json"))
    parser.add_argument("--quiet-incomplete", action="store_true",
                        help="report missing states briefly when learning from ordinary runs")
    args = parser.parse_args()
    try:
        programs = extract(args.captures)
    except ValueError as exc:
        if not args.quiet_incomplete:
            raise
        print(f"[SHADER-LEARN] {exc}")
        return 2
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(programs, indent=2) + "\n")
    print(f"Recovered {len(programs)} exact material shapes from {len(args.captures)} input(s): {args.output}")


if __name__ == "__main__":
    raise SystemExit(main())
