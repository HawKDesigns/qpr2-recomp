# Linux build (x86-64)

Windows with the D3D11 native renderer is the performance target. The Linux
build exists to exercise the guest boot path, the runtime and the tooling
without a Windows machine. It uses the runtime's software NV2A raster, since
the D3D11 ROV path is Windows-only, so **its frame rate says nothing about
the Windows renderer**. Run manifests from this build carry
`"renderer": "software"`.

## What has been verified

| Check | Result |
| --- | --- |
| Runtime libraries build with GCC 13 | Pass |
| Title glue compiles and links against the runtime | Pass, with stub game code (`tools/qpr2_link_check.py`) |
| Start-up checks: renderer ABI, 70/70 callbacks, 52/52 level callbacks | Pass |
| Runtime init and shutdown with a synthetic five-function XBE: memory layout, APU, kernel bridge, OHCI, NV2A registrations | Pass, exit code 0 |
| Trapped register access through the title's SIGSEGV handler (APU read, AC'97 channel reset) | Pass |
| SDL window path, guest on its own thread (SDL dummy video driver) | Pass |
| Frame trace recorder and exporter | Pass |
| `[THREAD-CPU]` report | Pass |
| Lift of the retail XBE, boot to menus, race | **Not tested.** No game files were available where this port was written |

The untested row is the one that matters for a real run. Expect bring-up work
there: the 70 recovered callbacks and the lifted code have only ever executed
on Windows.

## Build and run

Dependencies: a C toolchain, CMake 3.20+, Ninja or Make, SDL2 and OpenSSL
development packages, Python 3.10+ with Capstone.

```sh
sudo apt install build-essential cmake ninja-build libsdl2-dev libssl-dev
python3 -m pip install capstone
python3 tools/qpr2_run_posix.py --mode Benchmark
```

`tools/qpr2_run_posix.py` is the counterpart of `rebuild-and-run.ps1` for the
parts that exist off Windows: XBE fingerprint check, lift, build, run with the
shared profile in `tools/qpr2_runtime.psd1`, a run folder with the same
`run.json` keys, frame-trace export, memory-map export and failure context.
With no display it runs headless. `--build-only`, `--timeout-seconds` and
`--no-frame-trace` match the Windows switches.

Differences from Windows:

- No D3D11 renderer, so no native ROV, raw GPU geometry or literal material
  shaders. The shader preparation steps are skipped.
- The generated native vertex header is not used; unsupported programs run in
  the interpreter.
- Audio goes through SDL2. Controllers go through SDL game controllers.
- F1, F4–F7 and F10 phase marks work in the SDL window. F3 excludes a pause
  interval. F2 overlay and F8 capture are Windows features.

## Checks that need no game files

```sh
python3 tools/qpr2_link_check.py
python3 tools/qpr2_link_check.py --toolchain tools/mingw-w64-x86_64.cmake
```

The first builds the title and runtime natively with stub game code, then
runs the two smoke boots listed in the table. The second cross-compiles and
links the **Windows** configuration (D3D11 renderer, Win32 presenter, VEH
handler) with MinGW-w64, which catches Windows build breaks from a Linux
host. It does not run the result, and MSVC remains the supported Windows
compiler.

## Runtime change made for this port

The AC'97 bus-master write trap never armed on x86-64 Linux. The arming code
named arm64 as the only POSIX host and sent x86-64 down the Win32 path, where
it failed with "could not arm the bus-master write trap; a channel reset will
spin". The store decoder for x86-64 Linux already existed. The guard now
covers both, and the link check's AC'97 step fails without the fix.
