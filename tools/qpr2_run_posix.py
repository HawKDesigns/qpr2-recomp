#!/usr/bin/env python3
"""Build and run QPR2 clean v1 on a POSIX host (Linux x86-64).

The POSIX counterpart of rebuild-and-run.ps1 for the parts that exist off
Windows: lift, CMake build, run with the shared runtime profile, a run folder
with the same run.json keys, frame-trace export, memory-map export and
failure context.  The native D3D11 renderer is Windows-only, so this build
uses the runtime's software NV2A raster: use it for boot, correctness and
tooling work.  Its frame rate is not comparable with a Windows run and the
manifest says so ("renderer": "software").

    python3 tools/qpr2_run_posix.py                    # Play
    python3 tools/qpr2_run_posix.py --mode Benchmark   # counters, trace, 360 s limit
    python3 tools/qpr2_run_posix.py --build-only
"""
from __future__ import annotations

import argparse
import datetime as dt
import hashlib
import json
import os
import platform
import re
import shutil
import signal
import subprocess
import sys
import uuid
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
RUNTIME = ROOT / "third_party" / "xboxrecomp"
PROFILE = ROOT / "tools" / "qpr2_runtime.psd1"
SECTION = re.compile(r"^\s*(\w+)\s*=\s*@\{\s*$")
ENTRY = re.compile(r'^\s*(\w+)\s*=\s*"([^"]*)"\s*$')


def read_profile(path: Path = PROFILE) -> dict:
    """Read the PowerShell data file the Windows launcher uses.

    Only the shape that file has is accepted: top-level `Name = "value"`
    entries and one level of `Name = @{ ... }` tables of string values.
    """
    profile: dict = {}
    section = None
    for number, line in enumerate(path.read_text(encoding="utf-8-sig").splitlines(), 1):
        text = line.split("#", 1)[0].rstrip()
        if not text.strip() or text.strip() in ("@{", "}"):
            if text.strip() == "}" and section is not None:
                section = None
            continue
        opened = SECTION.match(text)
        entry = ENTRY.match(text)
        if opened and section is None:
            section = profile.setdefault(opened.group(1), {})
        elif entry:
            (section if section is not None else profile)[entry.group(1)] = entry.group(2)
        else:
            raise ValueError(f"{path.name}:{number}: unsupported profile line: {line.strip()}")
    return profile


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1 << 20), b""):
            digest.update(block)
    return digest.hexdigest()


def git_revision(path: Path) -> str | None:
    if not (path / ".git").exists():
        return None
    done = subprocess.run(["git", "-C", str(path), "rev-parse", "HEAD"], capture_output=True, text=True)
    return done.stdout.strip() if done.returncode == 0 else None


def lift_identity(runtime_revision: str | None) -> str:
    """The lifter inputs, so a replaced runtime archive invalidates the lift."""
    if runtime_revision:
        return runtime_revision
    digest = hashlib.sha256()
    for folder in (RUNTIME / "tools", RUNTIME / "templates" / "runtime"):
        for path in sorted(folder.rglob("*")):
            if path.is_file() and path.suffix in (".py", ".h"):
                digest.update(f"{path.relative_to(RUNTIME).as_posix()} {sha256(path)}\n".encode())
    return digest.hexdigest()


def host_facts() -> dict:
    facts = {"os": f"{platform.system()} {platform.release()}", "host_platform": sys.platform,
             "machine": platform.machine(), "logical_processors": os.cpu_count()}
    try:
        for line in Path("/proc/cpuinfo").read_text().splitlines():
            if line.startswith("model name"):
                facts["cpu"] = {"name": line.split(":", 1)[1].strip()}
                break
        for line in Path("/proc/meminfo").read_text().splitlines():
            if line.startswith("MemTotal"):
                facts["memory_mib"] = int(line.split()[1]) // 1024
                break
    except OSError:
        pass
    return facts


def toolchain_facts(build_dir: Path) -> dict:
    facts: dict = {}
    for name, command in (("cmake", ["cmake", "--version"]), ("c_compiler", ["cc", "--version"])):
        try:
            done = subprocess.run(command, capture_output=True, text=True)
            facts[name] = done.stdout.splitlines()[0].strip() if done.returncode == 0 and done.stdout else None
        except OSError:
            facts[name] = None
    cache = build_dir / "CMakeCache.txt"
    if cache.exists():
        for line in cache.read_text(errors="replace").splitlines():
            if line.startswith("CMAKE_GENERATOR:") or line.startswith("CMAKE_BUILD_TYPE:"):
                key, value = line.split("=", 1)
                facts[key.split(":")[0].lower()] = value
    return facts


def logged(command: list[str], log: Path, **kwargs) -> int:
    with log.open("w", encoding="utf-8") as stream:
        done = subprocess.run(command, stdout=stream, stderr=subprocess.STDOUT, **kwargs)
    Path(str(log) + ".exitcode").write_text(f"{done.returncode}\n")
    return done.returncode


def write_manifest(path: Path, manifest: dict) -> None:
    path.write_text(json.dumps(manifest, indent=4) + "\n", encoding="utf-8")


def run_game(exe: Path, cwd: Path, env: dict, run_dir: Path, timeout: int) -> tuple[str, int | None]:
    """Start the game in its own process group; on timeout stop the whole group."""
    with (run_dir / "run.stdout.log").open("wb") as out, (run_dir / "run.stderr.log").open("wb") as err:
        process = subprocess.Popen([str(exe)], cwd=cwd, env=env, stdout=out, stderr=err,
                                   start_new_session=True)
        try:
            code = process.wait(timeout=timeout or None)
        except subprocess.TimeoutExpired:
            for sig in (signal.SIGTERM, signal.SIGKILL):
                try:
                    os.killpg(process.pid, sig)
                except ProcessLookupError:
                    break
                try:
                    process.wait(timeout=10)
                    break
                except subprocess.TimeoutExpired:
                    continue
            return "timed_out", 124
    if code == 0:
        return "exited", 0
    # A negative return code is the signal that ended the process.
    return "failed", code


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    parser.add_argument("--mode", choices=("Play", "Benchmark", "Debug"), default="Play")
    parser.add_argument("--timeout-seconds", type=int, default=0)
    parser.add_argument("--build-only", action="store_true")
    parser.add_argument("--no-frame-trace", action="store_true")
    parser.add_argument("--sample-guest-pc", action="store_true")
    parser.add_argument("--exe", type=Path, help="run this binary instead of building (tests, bisecting)")
    parser.add_argument("--game-root", type=Path, default=ROOT,
                        help="directory holding game/ and hdd/ (default: the project root)")
    parser.add_argument("--logs-dir", type=Path, default=ROOT / "logs",
                        help="where run folders and result ZIPs are written")
    parser.add_argument("--allow-unknown-xbe", action="store_true",
                        help="skip the retail fingerprint check (synthetic test XBEs only)")
    args = parser.parse_args(argv)
    if os.name == "nt":
        parser.error("use rebuild-and-run.ps1 on Windows")

    profile = read_profile()
    run_id = dt.datetime.now().strftime("%Y%m%d-%H%M%S-%f")[:-3] + "-" + uuid.uuid4().hex[:8]
    logs_dir = args.logs_dir.resolve()
    run_dir = logs_dir / f"clean-v1-{run_id}"
    run_dir.mkdir(parents=True)
    manifest_path = run_dir / "run.json"
    track_guest_pc = args.mode == "Debug" or args.sample_guest_pc
    build_dir = ROOT / ("build-clean-v1-posix-debug" if track_guest_pc else "build-clean-v1-posix")
    game_root = args.game_root.resolve()
    xbe = game_root / "game" / "default.xbe"
    title_revision = git_revision(ROOT)
    runtime_revision = git_revision(RUNTIME)
    pin = ROOT / "tools" / "xboxrecomp-pin.txt"
    manifest: dict = {
        "schema_version": 1, "run_id": run_id,
        "started_utc": dt.datetime.now(dt.timezone.utc).isoformat(),
        "mode": args.mode, "configuration": "Release", "renderer": "software",
        "title_source": "git" if title_revision else "archive", "title_revision": title_revision,
        "runtime_source": "git" if runtime_revision else "archive", "runtime_revision": runtime_revision,
        "runtime_pin": pin.read_text().strip() if pin.exists() else None,
        "guest_pc_tracking": track_guest_pc if not args.exe else None, "status": "preparing",
    }
    manifest.update(host_facts())

    def fail(message: str, code: int = 1) -> int:
        manifest.update(status="failed", error=message,
                        finished_utc=dt.datetime.now(dt.timezone.utc).isoformat())
        write_manifest(manifest_path, manifest)
        print(f"qpr2: {message}", file=sys.stderr)
        return code

    if not xbe.is_file():
        return fail("Extract your matching Xbox disc into game/ so game/default.xbe is present.")
    manifest["xbe_sha256"] = sha256(xbe)
    if manifest["xbe_sha256"] != profile["XbeSha256"] and not args.allow_unknown_xbe:
        return fail(f"Unexpected default.xbe SHA-256 {manifest['xbe_sha256']}. See docs/KNOWN_BUILD.md.")
    write_manifest(manifest_path, manifest)

    exe = args.exe.resolve() if args.exe else build_dir / "qpr2_recomp"
    if not args.exe:
        if not (RUNTIME / "CMakeLists.txt").exists():
            return fail("third_party/xboxrecomp is empty. Run git submodule update --init --recursive "
                        "or python3 tools/bootstrap_xboxrecomp.py.")
        if runtime_revision and runtime_revision != manifest["runtime_pin"]:
            return fail("XboxRecomp does not match the runtime pin. Run git submodule update --init --recursive.")
        manifest["runtime_lift_identity"] = lift_identity(runtime_revision)
        key = f"{manifest['runtime_lift_identity']}/{manifest['xbe_sha256']}/{sha256(ROOT / 'tools/qpr2_pipeline.py')}"
        stamp = ROOT / "analysis" / "clean-v1-lift.txt"
        lifted = (stamp.exists() and stamp.read_text().strip() == key
                  and (ROOT / "src/generated/qpr2_build_config.h").exists()
                  and (ROOT / "src/recomp/gen/recomp_dispatch.c").exists())
        if not lifted:
            stamp.unlink(missing_ok=True)
            if logged([sys.executable, "tools/qpr2_pipeline.py", "all"], run_dir / "lift.log", cwd=ROOT):
                return fail(f"Lift failed. See {run_dir / 'lift.log'}.")
            stamp.parent.mkdir(exist_ok=True)
            stamp.write_text(key + "\n")
        generator = ["-G", "Ninja"] if shutil.which("ninja") else []
        configure = ["cmake", "-S", str(ROOT), "-B", str(build_dir), *generator, "-DCMAKE_BUILD_TYPE=Release",
                     f"-DQPR2_TRACK_GUEST_PC={'ON' if track_guest_pc else 'OFF'}"]
        if logged(configure, run_dir / "configure.log"):
            return fail(f"Configure failed. See {run_dir / 'configure.log'}.")
        if logged(["cmake", "--build", str(build_dir), "--target", "qpr2_recomp", "--parallel"],
                  run_dir / "build.log"):
            return fail(f"Build failed. See {run_dir / 'build.log'}.")
        manifest["toolchain"] = toolchain_facts(build_dir)
        if args.mode == "Debug":
            logged([sys.executable, "tools/qpr2_debug_metadata.py", "symbols", "--output",
                    str(run_dir / "guest-symbols.json")], run_dir / "debug-metadata.log")
    if not exe.is_file():
        return fail(f"{exe} was not found.")
    manifest["executable_sha256"] = sha256(exe)
    manifest["status"] = "built"
    write_manifest(manifest_path, manifest)
    if args.build_only:
        return 0

    timeout = args.timeout_seconds or (360 if args.mode != "Play" else 0)
    settings = dict(profile["Runtime"])
    if args.mode != "Play":
        settings.update(profile["Benchmark"])
    if args.mode == "Debug":
        settings.update(profile["Debug"])
    trace_path = run_dir / "frame-trace.bin"
    if args.mode != "Play" and not args.no_frame_trace:
        settings["QPR2_FRAME_TRACE_PATH"] = str(trace_path)
    if timeout:
        settings["RECOMP_WATCHDOG_SECS"] = str(timeout)
    if not (os.environ.get("DISPLAY") or os.environ.get("WAYLAND_DISPLAY")):
        settings["RECOMP_HEADLESS"] = "1"          # no display: run without the SDL window
        settings.setdefault("SDL_AUDIODRIVER", "dummy")
    env = dict(os.environ)
    env.pop("QPR2_FRAME_TRACE_PATH", None)
    # As on Windows, a value already set in the caller's environment wins, so a
    # same-build comparison can override one setting before launching.
    for name, value in settings.items():
        if name not in env:
            env[name] = value
    manifest["environment"] = {name: env[name] for name in sorted(env)
                               if name.startswith(("RECOMP_", "QPR2_PAD_")) or name in ("QPR2_MONITOR", "QPR2_FULLSCREEN")}
    manifest["frame_trace_requested"] = "QPR2_FRAME_TRACE_PATH" in settings
    manifest["timeout_seconds"] = timeout
    manifest["status"] = "running"
    write_manifest(manifest_path, manifest)

    status, code = run_game(exe, game_root, env, run_dir, timeout)
    manifest.update(status=status, exit_code=code,
                    finished_utc=dt.datetime.now(dt.timezone.utc).isoformat())
    stderr_log = run_dir / "run.stderr.log"
    log_text = stderr_log.read_text(errors="replace") if stderr_log.exists() else ""
    manifest["watchdog_fired"] = bool(re.search(r"^\[WATCHDOG\]", log_text, re.M))
    kernel_log = game_root / "xbox_kernel.log"
    if kernel_log.exists():
        shutil.copyfile(kernel_log, run_dir / "kernel.log")
    write_manifest(manifest_path, manifest)

    def tool(name: str, arguments: list[str], log: str) -> None:
        if logged([sys.executable, str(ROOT / "tools" / name), *arguments], run_dir / log, cwd=ROOT):
            print(f"qpr2: {name} {arguments[0]} failed. See {run_dir / log}.", file=sys.stderr)

    if args.mode != "Play" and (ROOT / "analysis" / "qpr2_manifest.json").exists():
        tool("qpr2_debug_metadata.py", ["memory-map", "--run", str(run_dir), "--output",
                                        str(run_dir / "memory-map.json")], "memory-map.log")
    if trace_path.exists():
        tool("qpr2_frame_trace.py", ["export", str(run_dir)], "frame-trace.log")
    elif manifest["frame_trace_requested"]:
        print("qpr2: a frame trace was requested but no trace file was created.", file=sys.stderr)
    if (status != "exited" or manifest["watchdog_fired"]) and (ROOT / "analysis" / "qpr2_manifest.json").exists():
        tool("qpr2_debug_metadata.py", ["failure", "--run", str(run_dir), "--output",
                                        str(run_dir / "failure.json")], "failure.log")
    archive = shutil.make_archive(str(logs_dir / f"qpr2-clean-v1-results-{run_id}"), "zip", run_dir)
    print(f"Run ended with status {status} (exit code {code}). Results: {archive}")
    return 0 if status == "exited" else (code if isinstance(code, int) and 0 < code < 256 else 1)


if __name__ == "__main__":
    raise SystemExit(main())
