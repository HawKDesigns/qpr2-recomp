#!/usr/bin/env python3
"""Export and query local QPR2 debugger metadata outside the game process."""

from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
DISPATCH_ENTRY = re.compile(r"^\s*\{ 0x([\da-fA-F]{8})u, \(recomp_func_t\)(\w+) \},$", re.M)
CALLBACK_ENTRY = re.compile(r"^QPR2_CALLBACK\(([\da-fA-F]{8}), ([01])\)$", re.M)


def read_json(path: Path):
    return json.loads(path.read_text(encoding="utf-8-sig"))


def va(value: int) -> str:
    return f"0x{value:08X}"


def number(value: int | str) -> int:
    return int(value, 0) if isinstance(value, str) else value


def address_index(rows, key):
    return {number(row[key]): row for row in rows}


def export_symbols(analysis_dir: Path, generated_dir: Path, source_dir: Path) -> dict:
    game = read_json(analysis_dir / "qpr2_manifest.json")["game"]
    digest = game["xbe_sha256"]
    base = number(game["base_address"])
    entry = number(game["entry_point"])
    reviewed = read_json(ROOT / "tools/qpr2_debug_aliases.json")
    aliases = {int(key, 16): value for key, value in reviewed.get(digest, {}).items()}
    functions = address_index(read_json(analysis_dir / "disasm/functions.json"), "start")
    identified = address_index(read_json(analysis_dir / "func_id/identified_functions.json"), "start")
    abi = address_index(read_json(analysis_dir / "abi/abi_functions.json"), "address")
    dispatch = {int(address, 16): name for address, name in DISPATCH_ENTRY.findall(
        (generated_dir / "recomp_dispatch.c").read_text(encoding="utf-8"))}
    if not dispatch:
        raise ValueError("No entries found in the generated XboxRecomp dispatch table")
    overrides = {int(key, 16): name for key, name in
                 read_json(analysis_dir / "recomp/manual-functions.json").items()}
    callbacks = {}
    if digest in reviewed:
        callbacks = {int(address, 16): level == "1" for address, level in CALLBACK_ENTRY.findall(
            (source_dir / "qpr2_callbacks.def").read_text(encoding="utf-8"))}
        if not callbacks or overrides.keys() - callbacks.keys():
            raise ValueError("Manual overrides do not match the QPR2 callback registry")
    sections = read_json(analysis_dir / "qpr2_analysis.json")["sections"]
    sections = [(number(s["virtual_addr"]), number(s["virtual_addr"]) + s["virtual_size"], s["name"])
                for s in sections]

    result = []
    for address in sorted(functions.keys() | dispatch.keys() | callbacks.keys() | {entry}):
        fn = functions.get(address, {})
        identity = identified.get(address, {})
        alias = aliases.get(address)
        section = fn.get("section") or next((name for start, end, name in sections if start <= address < end), None)
        name = alias["name"] if alias else identity.get("identified_name")
        kind = "inferred" if name else "unknown"
        evidence = identity.get("method") if name else None
        confidence = identity.get("confidence") if name else None
        if alias:
            kind, evidence, confidence = alias["kind"], alias["evidence"], None
        name = name or f"sub_{address:08X}"
        row = {
            "guest_va": va(address),
            "end_exclusive": fn.get("end"),
            "rva": va(address - base) if address >= base else None,
            "section": section,
            "display_name": f"{name} [{va(address)}]",
            "name_kind": kind,
            "name_evidence": evidence,
            "name_confidence": confidence,
            "subsystem": alias["subsystem"] if alias else identity.get("category"),
            "generated_dispatch_symbol": dispatch.get(address),
            "implementation": "generated_dispatch" if address in dispatch else "no_dispatch_entry",
            "discovery_method": fn.get("detection_method"),
            "range_confidence": fn.get("confidence"),
            "abi": abi.get(address),
            "callers": fn.get("called_by", []),
            "vtable_reference": identity.get("vtable_addr"),
            "vtable_slot": identity.get("vtable_index"),
        }
        if address in callbacks:
            row.update(implementation="manual_callback", host_symbol=f"qpr2_recovered_{address:08X}",
                       level_callback=callbacks[address], direct_override_symbol=overrides.get(address))
        elif address == entry:
            row.update(host_entry_symbol="xbe_entry_point", host_symbol=dispatch.get(address, "xbe_entry_point"))
        else:
            row["host_symbol"] = dispatch.get(address)
        result.append(row)
    return {"schema_version": 1, "address_space": "guest_va", "xbe_sha256": digest, "functions": result}


def read_log(path: Path) -> str:
    data = path.read_bytes()
    return data.decode("utf-16" if data.startswith((b"\xff\xfe", b"\xfe\xff")) else "utf-8-sig",
                       errors="replace")


def export_memory_map(analysis_dir: Path, run_dir: Path) -> dict:
    game = read_json(analysis_dir / "qpr2_manifest.json")["game"]
    run = read_json(run_dir / "run.json")
    if game["xbe_sha256"] != run["xbe_sha256"]:
        raise ValueError("Run and XBE analysis have different SHA-256 fingerprints")
    image = read_json(analysis_dir / "qpr2_analysis.json")
    log = read_log(run_dir / "run.stderr.log")
    ranges = []

    def add(name, start, end, source, **details):
        ranges.append(dict(name=name, guest_va=va(start), end_exclusive=va(end), source=source, **details))

    for section in image["sections"]:
        start = number(section["virtual_addr"])
        add(section["name"], start, start + section["virtual_size"], "xbe_section_table",
            declared_writable=section["writable"], declared_executable=section["executable"],
            section_flags=section["flags"])

    mapped = re.search(r"xbox_MemoryLayoutInit: mapped (\d+) KB at", log)
    if mapped:
        add("Low RAM", 0, int(mapped[1]) * 1024, "loader_log", mapping="backed_span")

    alias = re.search(r"Tiled aperture alias verified \(tiled (0x[\dA-Fa-f]+) == contiguous (0x[\dA-Fa-f]+)\)", log)
    for label in ("Contiguous window", "Tiled aperture", "NV2A register aperture", "MCPX device aperture", "Flash ROM aperture"):
        found = re.search(rf"{label}: (\d+) MB at Xbox VA (0x[\dA-Fa-f]+)", log)
        if found:
            start = number(found[2])
            details = {}
            if label == "Tiled aperture":
                details["alias_verified"] = bool(alias and number(alias[1]) == start)
                if details["alias_verified"]:
                    details["alias_guest_va"] = va(number(alias[2]))
            add(label, start, start + int(found[1]) * 1024 * 1024, "loader_log", **details)

    stack = re.search(r"Stack: (\d+) KB at Xbox VA (0x[\dA-Fa-f]+)", log)
    if stack:
        start = number(stack[2])
        add("Stack arena", start, start + int(stack[1]) * 1024, "loader_log")
    heap = re.search(r"Heap: \d+ MB at Xbox VA (0x[\dA-Fa-f]+)-(0x[\dA-Fa-f]+)", log)
    if heap:
        add("Heap arena", number(heap[1]), number(heap[2]), "loader_log")
    apu = re.search(r"APU: (0x[\dA-Fa-f]+)\.\.(0x[\dA-Fa-f]+) trapped for MMIO", log)
    if apu:
        add("APU MMIO trap", number(apu[1]), number(apu[2]), "loader_log", mapping="trapped")

    landmarks = {"XBE image base": image["base_address"], "XBE entry point": image["entry_point"],
                 "Kernel thunk table": image["kernel_thunk_addr"]}
    for label, pattern in (
        ("Main TIB", r"TIB: fake TIB at VA (0x[\dA-Fa-f]+)"),
        ("Main image TLS", r"TLS: \d+-byte block at (0x[\dA-Fa-f]+)"),
        ("Kernel data exports", r"Kernel data exports: initialized at Xbox VA (0x[\dA-Fa-f]+)"),
        ("Fake kernel header", r"Kernel: fake PE header at Xbox VA (0x[\dA-Fa-f]+)"),
        ("D3D device pointer slot", r"\[PB-FIFO\] registered device=([\dA-Fa-f]{8})"),
    ):
        match = re.search(pattern, log)
        if match:
            landmarks[label] = va(int(match[1], 16))
    device_fields = {}
    for label, pattern in (
        ("FIFO base", r"\[PB-FIFO\] registered device=[\dA-Fa-f]+ bounds=\+([\dA-Fa-f]+)"),
        ("FIFO limit", r"\[PB-FIFO\] registered device=[\dA-Fa-f]+ bounds=\+[\dA-Fa-f]+/\+([\dA-Fa-f]+)"),
        ("Fence pointer", r"\[GPU-FENCE\] registered device=[\dA-Fa-f]+ pointer_offset=([\dA-Fa-f]+)"),
        ("Completion event", r"\[GPU-EVENT\] registered device=[\dA-Fa-f]+ event_offset=([\dA-Fa-f]+)"),
        ("Vblank event", r"\[VBLANK-EVENT\] registered device=[\dA-Fa-f]+ event_offset=([\dA-Fa-f]+)"),
        ("XMV clock", r"Frame counter: device at 0x[\dA-Fa-f]+, count \+0x([\dA-Fa-f]+)"),
    ):
        match = re.search(pattern, log)
        if match:
            device_fields[label] = va(int(match[1], 16))

    return {
        "schema_version": 1, "address_space": "guest_va", "xbe_sha256": game["xbe_sha256"],
        "run_id": run["run_id"], "runtime_pin": run.get("runtime_pin"),
        "snapshot": "startup_observations", "loader_complete": "xbox_MemoryLayoutInit: complete" in log,
        "ranges": sorted(ranges, key=lambda row: (number(row["guest_va"]), number(row["end_exclusive"]))),
        "landmarks": landmarks, "d3d_device_field_offsets": device_fields,
    }


def lookup(path: Path, query: str) -> list[dict]:
    data = read_json(path)
    if "functions" in data:
        if re.fullmatch(r"(?:0x[0-9a-f]{1,8}|[0-9a-f]{8})", query, re.I):
            address = int(query, 16)
        else:
            term = query.casefold()
            return [row for row in data["functions"] if term in row["display_name"].casefold()
                    or term in (row.get("host_symbol") or "").casefold()]
        exact = [row for row in data["functions"] if number(row["guest_va"]) == address]
        if exact:
            return exact
        rows = data["functions"]
    else:
        address = int(query, 16)
        rows = data["ranges"]
    matches = [row for row in rows if row["end_exclusive"] is not None and
               number(row["guest_va"]) <= address < number(row["end_exclusive"])]
    if "ranges" in data:
        matches = [dict(row, canonical_guest_va=va(number(row["alias_guest_va"]) + address - number(row["guest_va"])))
                   if row.get("alias_verified") else row for row in matches]
    return sorted(matches, key=lambda row: number(row["end_exclusive"]) - number(row["guest_va"]))


def export_failure(run_dir: Path) -> dict:
    manifest = read_json(run_dir / "run.json")
    log = read_log(run_dir / "run.stderr.log")
    fault = re.search(r"^\[CRASH\] access violation: (\w+) at host=(0x[0-9A-F]+) fault=(0x[0-9A-F]+)", log, re.M)
    watchdog = re.search(r"^\[WATCHDOG\] no exit after (\d+)s; guest pc=(0x[0-9A-F]+) esp=(0x[0-9A-F]+)", log, re.M)
    if not (fault or watchdog or manifest.get("status") in ("failed", "timed_out", "exit_status_unavailable")):
        return {}
    start = watchdog.start() if watchdog else fault.start() if fault else len(log)
    excerpt = log[start:]
    symbol_path = run_dir / "guest-symbols.json"
    catalog = read_json(symbol_path) if symbol_path.is_file() else {}
    functions = catalog.get("functions", []) if catalog.get("xbe_sha256") == manifest.get("xbe_sha256") else []
    exact = {number(row["guest_va"]): row for row in functions}
    ranges = [(number(row["guest_va"]), number(row["end_exclusive"]), row) for row in functions
              if row.get("end_exclusive") is not None]

    def resolve(address: str) -> dict:
        value = int(address, 16)
        row = exact.get(value)
        if row is None:
            row = min((item for item in ranges if item[0] <= value < item[1]),
                      key=lambda item: item[1] - item[0], default=(0, 0, None))[2]
        return {"guest_va": va(value), "symbol": row["display_name"] if row else None}

    result = {
        "schema_version": 1, "run_id": manifest["run_id"], "status": manifest["status"],
        "exit_code": manifest.get("exit_code"), "guest_pc_tracking": manifest.get("guest_pc_tracking"),
        "reason": "watchdog" if watchdog else "access_violation" if fault else manifest["status"],
    }
    if watchdog:
        result.update(timeout_seconds=int(watchdog[1]), guest_esp=watchdog[3],
                      guest_pc=resolve(watchdog[2]) if int(watchdog[2], 16) else None)
    elif fault:
        pc = re.search(r"^  guest pc: ([0-9A-F]{8})$", excerpt, re.M)
        host_symbol = re.search(r"^  host symbol: (.+)$", excerpt, re.M)
        result.update(access=fault[1], host_pc=fault[2], fault_address=fault[3],
                      host_symbol=host_symbol[1] if host_symbol else None,
                      guest_pc=resolve(pc[1]) if pc and int(pc[1], 16) else None)

    regs = re.search(r"^ +(?:guest )?regs: (.+(?:\n +esi=[^\n]+)?)", excerpt, re.M)
    if regs:
        result["guest_registers"] = {name: va(int(value, 16)) for name, value in
                                     re.findall(r"(\w+)=([0-9A-F]{8})", regs[1])}
    icalls = re.search(r"^  recent ICALL targets:((?: [0-9A-F]{8})+)", excerpt, re.M)
    if icalls:
        result["recent_indirect_calls"] = [resolve(address) for address in icalls[1].split() if int(address, 16)]
    result["stack"] = [{"slot": va(int(slot, 16)), "word": va(int(word, 16))}
                       for slot, word in re.findall(r"^ +(?:GS )?([0-9A-F]{8}):? ([0-9A-F]{8})$",
                                                   excerpt, re.M)[:64]]
    fifo = re.search(r"^  NV2A USER  PUT=([0-9A-F]+) GET=([0-9A-F]+)\s+"
                     r"NV2A PFIFO PUT=([0-9A-F]+) GET=([0-9A-F]+) REF=([0-9A-F]+) SUBR=([0-9A-F]+)",
                     excerpt, re.M)
    if fifo:
        result["fifo"] = dict(zip(("user_put", "user_get", "pfifo_put", "pfifo_get", "ref", "subroutine"),
                                  (va(int(value, 16)) for value in fifo.groups())))
    waits = re.findall(r"^\[WAIT-ACTIVE\] thread=(\d+) ordinal=(\d+) caller=([0-9A-F]+) "
                       r"object=([0-9A-F]+) host=\S+ elapsed_ms=(\d+)", excerpt, re.M)
    result["active_waits"] = [{"thread": int(thread), "ordinal": int(ordinal), "caller": resolve(caller),
                               "guest_object": va(int(obj, 16)), "elapsed_ms": int(elapsed)}
                              for thread, ordinal, caller, obj, elapsed in waits[:32]]
    trace_path = run_dir / "frame-trace.bin"
    if trace_path.is_file():
        from qpr2_frame_trace import DRAW, FLIP, HANDOFF, read_trace
        try:
            events, integrity = read_trace(trace_path)
            result["frame_trace_integrity"] = integrity
            progress = {}
            for event in events:
                if event.kind in (FLIP, HANDOFF, DRAW):
                    label = {FLIP: "completed_flip", HANDOFF: "host_handoff", DRAW: "host_draw"}[event.kind]
                    progress[label] = {"frame_id": event.frame, "timestamp_ns": event.timestamp}
            result["frame_progress"] = progress
        except (OSError, ValueError) as error:
            result["frame_trace_error"] = str(error)
    return result


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    commands = parser.add_subparsers(dest="command", required=True)
    symbols = commands.add_parser("symbols", help="export the lifted guest symbol catalog")
    symbols.add_argument("--generated", type=Path, default=ROOT / "src/recomp/gen")
    memory = commands.add_parser("memory-map", help="export XBE and observed startup mappings")
    memory.add_argument("--run", type=Path, required=True)
    for command in (symbols, memory):
        command.add_argument("--analysis", type=Path, default=ROOT / "analysis")
        command.add_argument("--output", type=Path, required=True)
    failure = commands.add_parser("failure", help="export observed crash or watchdog context")
    failure.add_argument("--run", type=Path, required=True)
    failure.add_argument("--output", type=Path, required=True)
    find = commands.add_parser("lookup", help="search an exported file by hex guest VA or symbol name")
    find.add_argument("file", type=Path)
    find.add_argument("query")
    args = parser.parse_args()
    try:
        if args.command == "lookup":
            print(json.dumps(lookup(args.file, args.query), indent=2))
            return 0
        if args.command == "symbols":
            data = export_symbols(args.analysis, args.generated, ROOT / "src")
        elif args.command == "memory-map":
            data = export_memory_map(args.analysis, args.run)
        else:
            data = export_failure(args.run)
            if not data:
                print("No crash, watchdog, or failed process status in this run.")
                return 0
        args.output.write_text(json.dumps(data, indent=2) + "\n", encoding="utf-8")
        print(f"Wrote {args.output}")
        return 0
    except (OSError, ValueError, KeyError) as exc:
        print(f"Debugger metadata: {exc}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
