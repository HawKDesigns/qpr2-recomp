#!/usr/bin/env python3
"""Reproducible XboxRecomp pipeline for ATV: Quad Power Racing 2.

The script never downloads or distributes game content. It operates on a
default.xbe extracted by the user from a legally owned Xbox disc.
"""

from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
import shutil
import subprocess
import sys
from datetime import datetime, timezone
from pathlib import Path
from typing import Any


EXPECTED_TITLE_IDS = {0x4143000A}  # AC-010
KNOWN_FUNCTION_SPLITS = {
    # The vtable at 0x001AFF70 points at 0x000261A0, immediately after a
    # short predicate ending at 0x00026198. Generic discovery merged both
    # routines because the second starts with `sub esp, 0x40` rather than a
    # canonical frame prologue. Tie the correction to the verified build so
    # an address can never leak into a regional or reissue executable.
    "5e9788296601bae4401e31ae1f5e25a8a732cb0bd0588576182d3aa69f859383": (
        0x000261A0,
        # The vtable scanner independently identifies 0x000BCAA0 as a method,
        # and the runtime called that exact address.  The base disassembler
        # nevertheless merged it into the destructor at 0x000BCA80 even
        # though the latter returns at 0x000BCA9B and padding separates the
        # two.  Without this split RECOMP_ICALL treats the live method call as
        # unresolved and silently skips it, leaving invalid object state.
        0x000BCAA0,
    ),
}
KNOWN_FUNCTION_RANGES = {
    # D3D's device-method dispatch table at guest VA 0x001A2A48 contains
    # these entry points. Generic discovery sees their machine code in the
    # executable D3D section, but misses the data-pointer references and
    # leaves the routines as gaps between directly-called functions. At
    # runtime RECOMP_ICALL consequently skipped 18 of them during device
    # setup; 0x00193800 is the remaining table member not reached in that
    # trace. Ends are the instruction immediately after each terminal ret or
    # tail jump, excluding alignment NOPs. Hash-lock this just like the split
    # above: these are addresses in one verified retail executable, not a
    # heuristic suitable for another build.
    "5e9788296601bae4401e31ae1f5e25a8a732cb0bd0588576182d3aa69f859383": (
        (0x00193110, 0x0019314E),
        (0x00193230, 0x00193273),
        (0x00193280, 0x001932C2),
        (0x001934E0, 0x0019354E),
        (0x00193610, 0x0019367B),
        (0x00193680, 0x001936EB),
        (0x001936F0, 0x00193701),
        (0x00193710, 0x00193721),
        (0x00193730, 0x00193741),
        (0x00193750, 0x001937F8),
        (0x00193800, 0x00193834),
        (0x00193840, 0x00193874),
        (0x00193900, 0x00193959),
        (0x00193960, 0x001939B7),
        (0x001939C0, 0x00193A1B),
        (0x00193A70, 0x00193ACA),
        (0x00193AD0, 0x00193B3C),
        (0x00193B40, 0x00193B8A),
        (0x00193B90, 0x00193BB2),
        # The runtime reaches this XPP routine through an indirect callback
        # immediately before XAPI connects the USB interrupt. It allocates and
        # clears the USB host-state block (pool tag 'USBH') and initializes its
        # timer. Generic discovery omitted the packed leaf because it begins
        # directly after the preceding ret and has no conventional prologue.
        # Skipping it leaves the OHCI driver inert even though its ISR exists.
        (0x001A7450, 0x001A74BC, "XPP", "verified_runtime_target"),
        # XPP calls this packed routine as the SET_CONFIGURATION completion
        # callback.  It starts immediately after sub_001A8400's `ret 8`, has
        # no conventional prologue, and was therefore left in the discovery
        # gap before sub_001A87A2.  The routine builds the controller's input
        # and output endpoint state and queues the follow-up work that exposes
        # the periodic interrupt pipe.  Skipping it leaves a fully enumerated
        # USB device with no input reports scheduled.
        (0x001A864E, 0x001A87A2, "XPP", "verified_runtime_target"),
    ),
}
KNOWN_RUNTIME_PATCHES = {
    # QPR2's linked DirectSound writes command 3 to the GP DSP doorbell and
    # waits for the DSP to clear it at 0x0014124A. XboxRecomp's GP/EP DSPs are
    # deliberately passthrough stubs, so no DSP56300 program consumes the
    # command and the title otherwise spins forever after loading ATV.BAD.
    # Completing the doorbell immediately is the stub's honest success path:
    # it does not synthesize audio results or alter the command block. Keep
    # this address-specific edit locked to the verified retail XBE hash.
    "5e9788296601bae4401e31ae1f5e25a8a732cb0bd0588576182d3aa69f859383": (
        0x0014124A,
        # XPP brackets one MCPX status read with CLI/STI. Guest interrupts are
        # delivered synchronously by the host runtime, which has no guest IF
        # bit to manipulate. Remove only this verified instruction pair.
        0x001ABB3D,
        0x001ABB4E,
    ),
}

# These entry points have recovered host implementations. A fresh lift still
# emits empty return stubs for direct calls, which corrupts title state.
# Addresses are valid only for this verified retail executable.
KNOWN_RECOVERED_STUBS = {
    "5e9788296601bae4401e31ae1f5e25a8a732cb0bd0588576182d3aa69f859383": (
        0x00027E00, 0x00037F90, 0x0003D060, 0x00043BE0, 0x00044010,
        0x00045060, 0x00045510, 0x0006B950, 0x0007F9F0, 0x0007FEF0,
    ),
}

PROJECT_ROOT = Path(__file__).resolve().parents[1]
DEFAULT_XBE = PROJECT_ROOT / "game" / "default.xbe"
DEFAULT_TOOLKIT = PROJECT_ROOT / "third_party" / "xboxrecomp"
ANALYSIS_ROOT = PROJECT_ROOT / "analysis"
ANALYSIS_JSON = ANALYSIS_ROOT / "qpr2_analysis.json"
MANIFEST_JSON = ANALYSIS_ROOT / "qpr2_manifest.json"
CONFIG_HEADER = PROJECT_ROOT / "src" / "generated" / "qpr2_build_config.h"
GENERATED_C = PROJECT_ROOT / "src" / "recomp" / "gen"


class PipelineError(RuntimeError):
    """A user-actionable pipeline failure."""


def parse_int(value: Any) -> int:
    if isinstance(value, int):
        return value
    return int(str(value), 0)


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def load_analysis(path: Path) -> dict[str, Any]:
    try:
        data = json.loads(path.read_text(encoding="utf-8"))
    except FileNotFoundError as exc:
        raise PipelineError(f"analysis output was not created: {path}") from exc
    except json.JSONDecodeError as exc:
        raise PipelineError(f"invalid analysis JSON: {exc}") from exc

    required = ("title", "title_id", "entry_point", "base_address", "sections")
    missing = [key for key in required if key not in data]
    if missing:
        raise PipelineError("analysis JSON is missing: " + ", ".join(missing))
    return data


def validate_title(data: dict[str, Any], allow_unknown: bool = False) -> int:
    title_id = parse_int(data["title_id"])
    if title_id not in EXPECTED_TITLE_IDS and not allow_unknown:
        expected = ", ".join(f"0x{x:08X}" for x in sorted(EXPECTED_TITLE_IDS))
        raise PipelineError(
            f"title ID 0x{title_id:08X} is not QPR2 ({expected}). "
            "Use --allow-unknown-title only when intentionally analyzing a "
            "verified regional/prototype build."
        )
    return title_id


def c_string(value: str) -> str:
    return value.replace("\\", "\\\\").replace('"', '\\"')


def render_config_header(data: dict[str, Any], xbe_sha256: str) -> str:
    title_id = parse_int(data["title_id"])
    entry_point = parse_int(data["entry_point"])
    title = c_string(str(data["title"]))
    return f"""/* Auto-generated by tools/qpr2_pipeline.py. Do not edit. */
#ifndef QPR2_BUILD_CONFIG_H
#define QPR2_BUILD_CONFIG_H

#define QPR2_GAME_TITLE       \"{title}\"
#define QPR2_TITLE_ID         0x{title_id:08X}u
#define QPR2_TITLE_ID_TEXT    \"0x{title_id:08X}\"
#define QPR2_ENTRY_POINT      0x{entry_point:08X}u
#define QPR2_XBE_SHA256       \"{xbe_sha256}\"
#define QPR2_XBE_PATH         \"game/default.xbe\"
#define QPR2_GAME_DIR         \"game\"
#define QPR2_SAVE_DIR         \"hdd\"

#endif
"""


def toolkit_commit(toolkit: Path) -> str | None:
    try:
        result = subprocess.run(
            ["git", "rev-parse", "HEAD"],
            cwd=toolkit,
            check=True,
            capture_output=True,
            text=True,
        )
    except (OSError, subprocess.CalledProcessError):
        return None
    return result.stdout.strip() or None


def run_module(toolkit: Path, module: str, *arguments: object) -> None:
    command = [sys.executable, "-m", module, *(str(item) for item in arguments)]
    print("+", " ".join(command))
    result = subprocess.run(command, cwd=toolkit)
    if result.returncode:
        raise PipelineError(f"{module} failed with exit code {result.returncode}")


def apply_known_function_splits(functions_path: Path, xbe_sha256: str) -> int:
    """Install verified function starts missed by generic discovery."""
    starts = KNOWN_FUNCTION_SPLITS.get(xbe_sha256, ())
    if not starts:
        return 0

    functions = json.loads(functions_path.read_text(encoding="utf-8"))
    changed = 0
    for split_at in starts:
        if any(parse_int(item["start"]) == split_at for item in functions):
            continue
        owner = next(
            (item for item in functions
             if parse_int(item["start"]) < split_at < parse_int(item["end"])),
            None,
        )
        if owner is None:
            raise PipelineError(
                f"verified function split 0x{split_at:08X} has no containing range"
            )

        original_end = parse_int(owner["end"])
        owner_start = parse_int(owner["start"])
        owner["end"] = f"0x{split_at:08X}"
        owner["size"] = split_at - owner_start
        # These discovery summaries covered the old combined range. ABI and
        # translation derive fresh data; stale counts/calls are worse than no
        # advisory field here.
        owner.pop("num_instructions", None)
        owner.pop("calls_to", None)

        functions.append({
            "start": f"0x{split_at:08X}",
            "end": f"0x{original_end:08X}",
            "size": original_end - split_at,
            "name": f"sub_{split_at:08X}",
            "section": owner["section"],
            "confidence": 1.0,
            "detection_method": "verified_build_split",
            "has_prologue": False,
            "called_by": [],
        })
        changed += 1

    if changed:
        functions.sort(key=lambda item: parse_int(item["start"]))
        functions_path.write_text(
            json.dumps(functions, indent=2) + "\n", encoding="utf-8")
        print(f"Applied {changed} verified build-specific function split(s)")
    return changed


def apply_known_function_ranges(functions_path: Path, xbe_sha256: str) -> int:
    """Install verified functions omitted as discovery gaps.

    Two-field entries retain the original D3D pointer-table defaults. A
    four-field entry supplies its section and evidence explicitly for gaps
    proven through another source, such as a captured runtime target.
    """
    ranges = KNOWN_FUNCTION_RANGES.get(xbe_sha256, ())
    if not ranges:
        return 0

    functions = json.loads(functions_path.read_text(encoding="utf-8"))
    changed = 0
    for entry in ranges:
        if len(entry) == 2:
            start, end = entry
            section = "D3D"
            detection_method = "verified_pointer_table"
        elif len(entry) == 4:
            start, end, section, detection_method = entry
        else:
            raise PipelineError(
                f"invalid verified function-range entry {entry!r}"
            )
        if end <= start:
            raise PipelineError(
                f"invalid verified function range 0x{start:08X}-0x{end:08X}"
            )
        existing = next(
            (item for item in functions if parse_int(item["start"]) == start),
            None,
        )
        if existing is not None:
            if parse_int(existing["end"]) != end:
                raise PipelineError(
                    f"verified function 0x{start:08X} already has unexpected "
                    f"end {existing['end']}"
                )
            continue

        overlap = next(
            (item for item in functions
             if start < parse_int(item["end"])
             and end > parse_int(item["start"])),
            None,
        )
        if overlap is not None:
            raise PipelineError(
                f"verified function range 0x{start:08X}-0x{end:08X} overlaps "
                f"{overlap['start']}-{overlap['end']}"
            )

        functions.append({
            "start": f"0x{start:08X}",
            "end": f"0x{end:08X}",
            "size": end - start,
            "name": f"sub_{start:08X}",
            "section": section,
            "confidence": 1.0,
            "detection_method": detection_method,
            "has_prologue": False,
            "called_by": [],
        })
        changed += 1

    if changed:
        functions.sort(key=lambda item: parse_int(item["start"]))
        functions_path.write_text(
            json.dumps(functions, indent=2) + "\n", encoding="utf-8")
        print(f"Applied {changed} verified function range(s)")
    return changed


def apply_known_runtime_patches(generated_dir: Path, xbe_sha256: str) -> int:
    """Bridge verified hardware waits that the current runtime cannot service."""
    patches = KNOWN_RUNTIME_PATCHES.get(xbe_sha256, ())
    if not patches:
        return 0

    changed = 0
    for guest_pc in patches:
        if guest_pc in (0x001ABB3D, 0x001ABB4E):
            instruction = "cli" if guest_pc == 0x001ABB3D else "sti"
            needle = (
                f'    RECOMP_UNIMPL("{instruction}", '
                f'0x{guest_pc:08X}u); /* TODO: {instruction} */'
            )
            injected = (
                "    /* QPR2 XPP interrupt guard; host interrupt delivery is "
                "synchronous. */"
                if instruction == "cli" else
                "    /* End QPR2 XPP interrupt guard. */"
            )
            candidates = []
            for path in generated_dir.glob("recomp_*.c"):
                source = path.read_text(encoding="utf-8")
                if needle in source or injected in source:
                    candidates.append(path)
            if len(candidates) != 1:
                raise PipelineError(
                    f"runtime patch 0x{guest_pc:08X} matched "
                    f"{len(candidates)} generated files"
                )
            path = candidates[0]
            source = path.read_text(encoding="utf-8")
            if injected in source:
                continue
            if source.count(needle) != 1:
                raise PipelineError(
                    f"runtime patch 0x{guest_pc:08X} did not match its "
                    "verified instruction sequence"
                )
            path.write_text(source.replace(needle, injected), encoding="utf-8")
            changed += 1
            continue

        if guest_pc != 0x0014124A:
            raise PipelineError(
                f"unknown verified runtime patch at 0x{guest_pc:08X}"
            )
        marker = f"    RECOMP_PC(0x{guest_pc:08X}u);"
        candidates = [
            path for path in generated_dir.glob("recomp_*.c")
            if marker in path.read_text(encoding="utf-8")
        ]
        if len(candidates) != 1:
            raise PipelineError(
                f"runtime patch 0x{guest_pc:08X} matched {len(candidates)} "
                "generated files"
            )

        path = candidates[0]
        source = path.read_text(encoding="utf-8")
        injected = (
            "    /* XboxRecomp's GP DSP is a passthrough stub. The hardware "
            "clears\n"
            "     * this command doorbell after consuming it; complete it "
            "immediately. */\n"
            "    MEM32(ebx) = 0;\n\n"
        )
        if injected in source:
            continue
        needle = (
            "    MEM32(ebx) = eax;\n\n"
            f"loc_{guest_pc:08X}: ;\n"
            f"{marker}\n"
        )
        if source.count(needle) != 1:
            raise PipelineError(
                f"runtime patch 0x{guest_pc:08X} did not match its verified "
                "instruction sequence"
            )
        source = source.replace(
            needle,
            "    MEM32(ebx) = eax;\n" + injected
            + f"loc_{guest_pc:08X}: ;\n{marker}\n",
        )
        path.write_text(source, encoding="utf-8")
        changed += 1

    if changed:
        print(f"Applied {changed} verified build-specific runtime patch(es)")
    return changed



def apply_known_generated_bridges(generated_dir: Path, xbe_sha256: str) -> int:
    """Restore verified host bridges after each clean lift of the retail XBE."""
    addresses = KNOWN_RECOVERED_STUBS.get(xbe_sha256, ())
    if not addresses:
        return 0

    changed = 0
    stubs_path = generated_dir / "recomp_stubs_unresolved.c"
    if not stubs_path.is_file():
        raise PipelineError(f"recovered call stubs missing: {stubs_path}")
    stubs = stubs_path.read_text(encoding="utf-8")
    for address in addresses:
        name = f"{address:08X}"
        empty = (f"void sub_{name}(void) {{ g_esp += 4; "
                 f"/* 0x{name}: not detected */ }}")
        wrapper = (f"extern void qpr2_recovered_{name}(void);\n"
                   f"void sub_{name}(void) {{ qpr2_recovered_{name}(); }}")
        if stubs.count(wrapper) == 1:
            continue
        if stubs.count(empty) != 1:
            raise PipelineError(
                f"recovered call 0x{name} did not match its empty stub; "
                "inspect generated code and the linked host implementation"
            )
        stubs = stubs.replace(empty, wrapper)
        changed += 1
    stubs_path.write_text(stubs, encoding="utf-8")

    header_path = generated_dir / "recomp_types.h"
    if not header_path.is_file():
        raise PipelineError(f"generated bridge declarations missing: {header_path}")
    header = header_path.read_text(encoding="utf-8")
    anchor = "void recomp_memory_fence(uint32_t va);"
    declarations = (
        "\nvoid recomp_nv2a_cache_flush(uint32_t register_va);"
        "\nvoid recomp_nv2a_submit(uint32_t register_va, uint32_t put);"
        "\nint recomp_nv2a_fence_wait(uint32_t device, uint32_t requested);"
    )
    if declarations not in header:
        if header.count(anchor) != 1 or any(
            declaration in header for declaration in declarations.splitlines()
            if declaration
        ):
            raise PipelineError("generated NV2A bridge declarations have drifted")
        header_path.write_text(
            header.replace(anchor, anchor + declarations), encoding="utf-8"
        )
        changed += 1

    # Locate the functions by guest address so --split can change TU numbering.
    candidates = [
        path for path in generated_dir.glob("recomp_*.c")
        if "void sub_00196CF0(void)" in path.read_text(encoding="utf-8")
    ]
    if len(candidates) != 1:
        raise PipelineError(
            f"NV2A bridge function matched {len(candidates)} generated files"
        )
    bridge_path = candidates[0]
    source = bridge_path.read_text(encoding="utf-8")
    patches = (
        (
            "void sub_00196CF0(void)\n{\n",
            "void sub_00196CF0(void)\n{\n"
            "    const uint32_t requested_fence = MEM32(g_esp + 4);\n",
            1,
        ),
        (
            "    RECOMP_PC(0x00196DF4u);\n",
            "    RECOMP_PC(0x00196DF4u);\n"
            "    if (recomp_nv2a_fence_wait(esi, requested_fence)) "
            "goto loc_00196E0F;\n",
            1,
        ),
        (
            "    MEM32(eax + 0x100410) = ecx;\n",
            "    MEM32(eax + 0x100410) = ecx;\n"
            "    recomp_nv2a_cache_flush(eax + 0x100410);\n",
            3,
        ),
        (
            "    MEM32(ecx + 0x40) = edx;\n",
            "    MEM32(ecx + 0x40) = edx;\n"
            "    recomp_nv2a_submit(ecx + 0x40, edx);\n",
            2,
        ),
        (
            "    MEM32(edx + 0x40) = eax;\n",
            "    MEM32(edx + 0x40) = eax;\n"
            "    recomp_nv2a_submit(edx + 0x40, eax);\n",
            1,
        ),
    )
    for needle, replacement, expected in patches:
        if source.count(replacement) == expected:
            continue
        if source.count(replacement) or source.count(needle) != expected:
            raise PipelineError(
                f"NV2A bridge in {bridge_path.name} changed shape: "
                f"expected {expected} instances of {needle.strip()}"
            )
        source = source.replace(needle, replacement)
        changed += expected
    bridge_path.write_text(source, encoding="utf-8")


    # Keep the faulting UI path observable in a clean build. The earlier
    # generated files carried these probes by hand, leaving UI-HISTORY empty
    # after 'all' replaced them.
    ui_candidates = [
        path for path in generated_dir.glob("recomp_*.c")
        if "void sub_0010FEE0(void)" in path.read_text(encoding="utf-8")
    ]
    if len(ui_candidates) != 1:
        raise PipelineError(
            f"UI trace function matched {len(ui_candidates)} generated files"
        )
    ui_path = ui_candidates[0]
    ui = ui_path.read_text(encoding="utf-8")
    include = '#include "../../qpr2_ui_trace.h"\n'
    if include not in ui:
        anchor = '#include "recomp_funcs.h"\n'
        if ui.count(anchor) != 1:
            raise PipelineError("UI trace include location has drifted")
        ui = ui.replace(anchor, anchor + include)
        changed += 1
    marker = "    RECOMP_PC(0x0010FEE0u);\n"
    observed = (
        "    qpr2_ui_trace(0, 0x0010FEE0u, 0, ecx, MEM32(esp), "
        "MEM32(esp + 4), esp, ebx, esi, edi);\n"
    )
    if observed not in ui:
        if ui.count(marker) != 1:
            raise PipelineError("UI trace entry has drifted")
        ui = ui.replace(marker, observed + marker)
        changed += 1
    for site in (0x0010F45C, 0x0010FF29, 0x0010FF55, 0x0010FF61):
        original = (
            f"RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x{site:08X}u)"
        )
        traced = f"QPR2_UI_ICALL(_icall_target, _icall_esp, 0x{site:08X}u)"
        if traced in ui:
            continue
        if ui.count(original) != 1:
            raise PipelineError(f"UI trace call at 0x{site:08X} has drifted")
        ui = ui.replace(original, traced)
        changed += 1
    ui_path.write_text(ui, encoding="utf-8")

    if changed:
        print(f"Applied {changed} verified generated host bridge(s)")
    return changed


def verify_inputs(toolkit: Path, xbe: Path, require_xbe: bool = True) -> None:
    if not (toolkit / "tools" / "xbe_parser").is_dir():
        raise PipelineError(
            f"XboxRecomp was not found at {toolkit}. Run "
            "'git submodule update --init --recursive'."
        )
    if require_xbe and not xbe.is_file():
        raise PipelineError(
            f"No XBE found at {xbe}. Extract your own QPR2 disc into game/."
        )
    if require_xbe and xbe.read_bytes()[:4] != b"XBEH":
        raise PipelineError(f"{xbe} does not have an XBE header")


def analyze(toolkit: Path, xbe: Path, allow_unknown: bool) -> dict[str, Any]:
    verify_inputs(toolkit, xbe)
    ANALYSIS_ROOT.mkdir(parents=True, exist_ok=True)
    CONFIG_HEADER.parent.mkdir(parents=True, exist_ok=True)

    run_module(toolkit, "tools.xbe_parser", xbe, "--json", ANALYSIS_JSON)
    data = load_analysis(ANALYSIS_JSON)
    title_id = validate_title(data, allow_unknown)
    digest = sha256_file(xbe)

    CONFIG_HEADER.write_text(render_config_header(data, digest), encoding="utf-8")
    manifest = {
        "schema_version": 1,
        "generated_at": datetime.now(timezone.utc).isoformat(),
        "game": {
            "title": data["title"],
            "title_id": f"0x{title_id:08X}",
            "entry_point": data["entry_point"],
            "base_address": data["base_address"],
            "xdk_version": data.get("xdk_version", 0),
            "section_count": len(data["sections"]),
            "kernel_import_count": len(data.get("kernel_imports", [])),
            "xbe_size": xbe.stat().st_size,
            "xbe_sha256": digest,
        },
        "toolkit": {
            "repository": "https://github.com/HawKDesigns/xboxrecomp",
            "commit": toolkit_commit(toolkit),
        },
    }
    MANIFEST_JSON.write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")

    print(
        f"Analyzed {data['title']} ({manifest['game']['title_id']}), "
        f"entry {data['entry_point']}, {len(data['sections'])} sections."
    )
    print(f"Wrote {CONFIG_HEADER.relative_to(PROJECT_ROOT)}")
    return data


def lift(toolkit: Path, xbe: Path, allow_unknown: bool, split: int) -> None:
    analyze(toolkit, xbe, allow_unknown)

    disasm = ANALYSIS_ROOT / "disasm"
    func_id = ANALYSIS_ROOT / "func_id"
    abi = ANALYSIS_ROOT / "abi"
    recomp = ANALYSIS_ROOT / "recomp"
    for directory in (disasm, func_id, abi, recomp, GENERATED_C):
        directory.mkdir(parents=True, exist_ok=True)

    # Analyze all code-bearing sections. QPR2's exact section layout is
    # build-derived; restricting discovery to .text this early is unjustified.
    run_module(
        toolkit,
        "tools.disasm",
        xbe,
        "--analysis-json",
        ANALYSIS_JSON,
        "--output",
        disasm,
        "--force",
        "-v",
    )
    run_module(
        toolkit,
        "tools.func_id",
        xbe,
        "--functions",
        disasm / "functions.json",
        "--strings",
        disasm / "strings.json",
        "--xrefs",
        disasm / "xrefs.json",
        "--output",
        func_id,
        "-v",
    )
    digest = sha256_file(xbe)
    apply_known_function_splits(disasm / "functions.json", digest)
    apply_known_function_ranges(disasm / "functions.json", digest)
    run_module(
        toolkit,
        "tools.abi_analysis",
        xbe,
        "--disasm-dir",
        disasm,
        "--func-id-dir",
        func_id,
        "--output-dir",
        abi,
        "-v",
    )
    run_module(
        toolkit,
        "tools.recomp",
        xbe,
        "--game-name",
        "ATV Quad Power Racing 2",
        "--all",
        "--split",
        split,
        "--gen-dir",
        GENERATED_C,
        "--output-dir",
        recomp,
        "--disasm-dir",
        disasm,
        "--func-id-dir",
        func_id,
        "--abi-dir",
        abi,
        "-v",
    )
    apply_known_runtime_patches(GENERATED_C, digest)
    apply_known_generated_bridges(GENERATED_C, digest)
    generated = len(list(GENERATED_C.glob("*.c")))
    print(f"Lift complete: {generated} C files in {GENERATED_C.relative_to(PROJECT_ROOT)}")


def doctor(toolkit: Path, xbe: Path) -> int:
    checks = []
    checks.append(("Python 3.10+", sys.version_info >= (3, 10), sys.version.split()[0]))
    checks.append(("Git", shutil.which("git") is not None, shutil.which("git") or "missing"))
    checks.append(("CMake", shutil.which("cmake") is not None, shutil.which("cmake") or "missing"))
    checks.append(("Capstone", importlib.util.find_spec("capstone") is not None,
                   "installed" if importlib.util.find_spec("capstone") else "missing"))
    checks.append(("XboxRecomp", (toolkit / "tools" / "xbe_parser").is_dir(), str(toolkit)))
    checks.append(("QPR2 default.xbe", xbe.is_file(), str(xbe)))

    failed = False
    for label, ok, detail in checks:
        print(f"[{'OK' if ok else 'MISSING'}] {label}: {detail}")
        failed |= not ok

    if xbe.is_file():
        magic_ok = xbe.read_bytes()[:4] == b"XBEH"
        print(f"[{'OK' if magic_ok else 'INVALID'}] XBE header: {xbe}")
        failed |= not magic_ok
    print("Runtime build target: Windows 10/11, VS 2022 x64, D3D11")
    return 1 if failed else 0


def status() -> int:
    if MANIFEST_JSON.is_file():
        manifest = json.loads(MANIFEST_JSON.read_text(encoding="utf-8"))
        game = manifest["game"]
        print(f"Analyzed: {game['title']} {game['title_id']}")
        print(f"Entry point: {game['entry_point']}")
        print(f"XBE SHA-256: {game['xbe_sha256']}")
        print(f"XboxRecomp commit: {manifest['toolkit'].get('commit') or 'unknown'}")
    else:
        print("Analyzed: no")
    print(f"Generated C files: {len(list(GENERATED_C.glob('*.c')))}")
    return 0


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("doctor", "analyze", "lift", "all", "status"))
    parser.add_argument("--toolkit", type=Path, default=DEFAULT_TOOLKIT)
    parser.add_argument("--xbe", type=Path, default=DEFAULT_XBE)
    parser.add_argument(
        "--allow-unknown-title",
        action="store_true",
        help="allow a non-AC-010 title ID for a verified alternate build",
    )
    parser.add_argument(
        "--split",
        type=int,
        default=250,
        help="functions per generated C translation unit (default: 250)",
    )
    return parser


def main(argv: list[str] | None = None) -> int:
    args = build_parser().parse_args(argv)
    toolkit = args.toolkit.resolve()
    xbe = args.xbe.resolve()

    try:
        if args.command == "doctor":
            return doctor(toolkit, xbe)
        if args.command == "status":
            return status()
        if args.command == "analyze":
            analyze(toolkit, xbe, args.allow_unknown_title)
            return 0
        if args.split < 25:
            raise PipelineError("--split must be at least 25")
        lift(toolkit, xbe, args.allow_unknown_title, args.split)
        return 0
    except PipelineError as exc:
        print(f"ERROR: {exc}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())

