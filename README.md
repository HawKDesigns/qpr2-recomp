# ATV: Quad Power Racing 2 — Xbox static recompilation

An experimental native Windows recompilation of the original Xbox version of
**ATV: Quad Power Racing 2**, built on
[XboxRecomp](https://github.com/sp00nznet/xboxrecomp). You must supply game data
from your own copy. The project does not distribute retail executable or asset
files.

## Clean v1 on XboxRecomp v0.17.0

This branch ports the working v126 QPR2 runtime onto upstream XboxRecomp
v0.17.0. It keeps the v126 native D3D11 ROV renderer, raw GPU geometry,
ordered FIFO completion, XMV clock and recovered title callbacks. The clean
Dunes1 baseline is accepted. See the [requirements audit](docs/CLEAN_V1_AUDIT.md)
for the remaining checks.

| Reference | QPR2 | XboxRecomp |
| --- | --- | --- |
| Last tested v126 | `0f55e19f3e04d7d7587920f2d6a34618db48ee51` | `92a5f030d0e9442d35e7fdfb8cb86344716c677f` |
| v126 source with dependency attribution | `b32020286812c2c9336a550b83b1f0dcc4f18d5f` | `f39fdf00ab65ffb1f6c3ccc18280885f1a5e6f09` |

The v126 build reached menus and completed races. About 15 FPS at the Dunes1
countdown is the historical check. Compare against a fresh v126 run on the
same machine for race pacing. Mounted-racer shadows and some audio effects
were already incomplete in v126. See the [renderer audit](tools/v126-feature-audit.md)
and [migration notes](docs/CLEAN_V1.md).

## Requirements

- Your legally obtained Xbox game files, extracted into `game/` with
  `game/default.xbe` present.
- Windows 10/11, Visual Studio 2022 with **Desktop development with C++**, and
  CMake 3.20 or newer for the host build.
- Python 3.10 or newer for local archive and shader preparation; Capstone
  (`py -3 -m pip install capstone`) for the analysis pipeline.
- A disc extraction tool such as `extract-xiso` or `xdvdfs` for your own disc.

The tested build identity is in [docs/KNOWN_BUILD.md](docs/KNOWN_BUILD.md).
Different releases may have different code addresses; verify the full XBE
SHA-256 before using build-specific overrides.

## Set up the source and game files

```powershell
git clone --branch clean-v1-v017 --recurse-submodules https://github.com/HawKDesigns/qpr2-recomp.git qpr2-clean-v1
cd qpr2-clean-v1
py -3 -m pip install capstone
```

For an existing Git checkout, update both the QPR2 repo and its pinned
XboxRecomp submodule:

```powershell
git pull --ff-only
git submodule update --init --recursive
```

GitHub's source ZIP contains the QPR2 project. The launcher installs the
pinned XboxRecomp runtime into `third_party/xboxrecomp` when that folder is
empty and Git is available. You can also extract the pinned runtime source
there yourself. The runtime pin is in [tools/xboxrecomp-pin.txt](tools/xboxrecomp-pin.txt).
An existing compatible runtime source folder is reused.

Extract your disc into `game/`. The launcher checks the XBE fingerprint and
regenerates local analysis and translated C on the first build or when the
runtime or lift pipeline changes. Run `py -3 tools/qpr2_pipeline.py doctor`
to check the toolchain and inputs. Run `py -3 tools/qpr2_pipeline.py all`
to exercise the lift independently.

The pipeline examines all code-bearing XBE sections, not only `.text`. Do not
commit or share `game/`, generated translations, saves, memory dumps, or other
retail-derived artifacts.

Keep the `ATV.BAD` and `ATV.BAH` from your extracted ISO under
`game/` (a nested game directory is also supported). When the native vertex
header is missing, the launcher uses the bundled BAHHHD-Extract tool to
extract **every** ATV file into `game/ATV/`, creates `game/ATV.zip` with
those files at the ZIP root, and generates the native vertex variants.
An existing complete `game/ATV/` folder or `game/ATV.zip` is reused.
The extractor and ZIP preparation work offline; no separate clone or manual
PowerShell compression command is needed. See [PUBLISHING.md](PUBLISHING.md#vertex-programs-from-your-extracted-archive)
for the source layout and checks. The launcher probes Python installations
and prefers the actual `python.exe` when available. If Windows returns
`0xC0000022` before `[ATV-PREP] Python started`, check the printed
`[PYTHON]` interpreter attempts: the archive tool did not reach the
BAD/BAH files.

For comparable v126 race performance, leave those locally extracted game
files under `game/`. The launcher generates the 180 native vertex variants
automatically. During the first ordinary playthrough of a 3D menu, preview,
and race, it learns the reviewed material shapes from its own stderr log.
Close the game and launch `rebuild-and-run.ps1` again: it generates all three
literal material classes and rebuilds with the fast path. **No F8 capture
or manual extraction command is required.** If the first run does not reach
all required material states, further ordinary runs accumulate them.

The pinned XboxRecomp fork rechecks the embedded shader content and recompiles
the optional program users when the local headers appear after an earlier
generic build. Check all three `[SHADER-STATUS]` lines for `ready` on the
second launch; `shader-status.txt` is included in the results ZIP. After
building, `[SHADER-EMBED]` verifies the GPU shader against the original v126
source. The capture fixture and generated shader files stay local.
See [PUBLISHING.md](PUBLISHING.md) for validation and troubleshooting.

## Run clean v1

On Windows, double-click **`QPR2 Launcher.bat`** in the project root. The
launcher uses the same build script as PowerShell. Monitor selection, button
mapping, and stick and trigger settings are built into the game. See the
[launcher guide](launcher/README.md) for settings and upgrades from the
standalone ZIP.

```powershell
.\rebuild-and-run.ps1
.\rebuild-and-run.ps1 -Mode Benchmark
.\rebuild-and-run.ps1 -Mode Debug
```

Play mode starts an optimized Release build with ordinary logging and no
launcher timeout. Benchmark mode enables the existing phase and GPU timing
counters and defaults to a six-minute timeout. Debug mode adds guest PC
tracking and additional diagnostics in a separate Release build tree.
Use `-TimeoutSeconds` to extend a diagnostic run or `-BuildOnly` to stop
after the build.

Each attempt writes logs and a `run.json` manifest under
`logs/clean-v1-*/`. A completed run also creates a time-stamped results ZIP.
Benchmark and Debug runs include a compact frame trace, `frames.csv`, and
`frame-summary.json`. The export distinguishes completed NV2A flips, successful
framebuffer handoffs, and distinct frames drawn by the host. Host draw timing
describes the GDI/D3D11 presenter, independent of display scanout. The trace also
records guest display ticks and emulated vblanks.

Use the same Dunes1 setup across runs. With the game window focused, mark
countdown with **F5**, clear track ahead with **F6**, and following visible AI
with **F7**. Press **F4** when those labels no longer apply. The report
excludes intervals that cross phase changes or overlap F8 capture, and shows
missing records. Use
`-NoFrameTrace` for a same-build recorder overhead comparison.

```powershell
py -3 tools/qpr2_frame_trace.py compare logs/clean-v1-REFERENCE logs/clean-v1-CANDIDATE
```

Failed runs with runtime logs include `failure.json` with available guest
registers, stack words, watchdog FIFO pointers and active waits. The export
resolves guest addresses against the matching symbol catalog when available
and adds the last recorded flip, handoff and draw from the frame trace.

The launcher requests native ROV and raw GPU geometry. Confirm what the
runtime actually admitted in the logs. Runtime defaults are in
`tools/qpr2_runtime.psd1` and can be overridden before launch. For example,
`RECOMP_GPU_POINT_BOUNDS=0` restores the prior AABB decision for a same-build
comparison.

**Performance → Reuse queued GPU surfaces** is on by default. Turn it off
in the launcher or set `$env:RECOMP_GPU_ROTATE_SURFACES = '0'` for a same-build
comparison. The `[GPU-SURFACE-ROTATION]` line reports the snapshot copies and
logical bytes avoided.

## Diagnose race frame times

Benchmark exports `frame-profile.csv` and `profile-summary.json` alongside the
existing pacing files. The profile compares slow and fast frames within each
marked scene. It reports FIFO wall time, upload and submission work, render-target
setup, current and queued staging Map waits, completion/report waits, guest
memory publication, draw/triangle counts, and the producer size for dependency
drains. CPU Map time includes waiting for earlier GPU work. Dependency drain
time includes its nested stages. Guest fence waits may overlap the renderer.
The profile also records raw-geometry boundary fallback counts and observed
ZETA bindings and dependency drains. The latest binding is contextual evidence
and can differ from the actual producer. The GPU columns are sampled execution
times for the submitting frame. Use
the sample counts and drop/invalid counters to assess coverage. Benchmark
leaves the shader replay probe off so its extra GPU work stays out of the timing.

`frame-summary.json` includes the worst interval, counts over 100 ms, measured
duration, the guest display clock, and five-second Windows thread CPU summaries.
The optional kernel interrupt schedule has its own rate when observed.
For section-bucketed guest main PC samples, use
`./rebuild-and-run.ps1 -Mode Benchmark -SampleGuestPc`. This uses the separate
PC-tracking build and should be compared with captures using the same build.

Video and preview-camera activity can label phases automatically. Use **F5**
at countdown, **F6** on clear track, **F7** behind visible AI, and
**F4** outside those scenes. Press **F3** before pausing and again after
resuming. F3 marks an exclusion and leaves the current scene label in place.
An interval crossing the marker is excluded as well. **F2** toggles the
in-game frame-time graph. The exporter excludes intervals with the graph open
and intervals crossing its toggle. Keep **F8** capture outside the timed part
of the run. Use the same track, rider, and camera for
comparisons. For an overhead check, `RECOMP_FRAME_PROFILE=0` keeps basic frame
pacing while omitting the detailed CPU/GPU profile. Compare alternating runs
from the same build and scene.

The profiling design follows [OpenTricky’s grouped GPU timestamp scopes](https://github.com/GiZcesi/OpenTricky/blob/61a03bc/xboxrecomp/src/d3d/d3d8_gpuprof.c)
and [Takedawn’s same-scene frame-time comparisons](https://github.com/xenophobentx/takedawn/blob/9f7c371/docs/testing.md#frame-times-and-pacing).

If a scene key was pressed by mistake, find its segment in `frames.csv`.
`py -3 tools/qpr2_frame_trace.py export RUN --exclude-segment N` regenerates
the summaries with that interval excluded and preserves the raw trace.
`compare` accepts `--exclude-reference-segment` and `--exclude-candidate-segment`.

For a same-build depth-clear comparison, launch Benchmark once with
`$env:RECOMP_GPU_DEPTH_CLEAR_RETIRE = '1'` and once with `'0'`. The candidate
can omit an older queued depth write after a full overwrite of its host depth
cache. The profile counts retired depth publications and reports target-depth
drains, queued Map time and completion Map time. Start both runs with the same
track, camera and graphics settings, and check shadows, terrain and race flow.

## Shader cache and generated files

Compiled GPU shaders (`*.dxbc`) are stored in `shader-cache/`, separate from
`logs/`. They are compiled shader binaries, **not GPU logging**. Cache keys
include the shader/compiler inputs; a changed input produces a new entry.
The build also regenerates embedded shader sources when their tracked inputs
change. Keep the cache across ordinary builds for faster startup. If you
suspect stale or corrupted cache contents, close the game, delete
`shader-cache/`, and rerun; the shaders will be rebuilt. `shader-cache/` is
local, disposable, and ignored by Git.

Diagnostic build trees, logs, saved game data, generated translations, and
packaged run ZIPs are also ignored. The `.gitignore` protects common paths,
but check `git status` before a commit in case data was placed elsewhere.

## Build identity and project boundaries

The known executable has title ID `0x4143000A` (`AC-010`); its full fingerprint,
entry point, section list, and original analysis totals are in
[docs/KNOWN_BUILD.md](docs/KNOWN_BUILD.md). The source repository contains host
glue, tooling, tests, and build scripts. The executable, generated retail code,
and game assets must remain local to each user.

## License and attribution

This project's independently authored code is offered under the [MIT license](LICENSE).
That license does not cover the original game or third-party components. The
original game was developed by **Climax Brighton** and published by **Acclaim
Entertainment** under the **AKA Acclaim** label. This is an independent fan
project, unaffiliated with and not endorsed by those companies or Microsoft.

See [ATTRIBUTION.md](ATTRIBUTION.md) for the tool, reference-project, and
third-party notices. If publishing a modified `xboxrecomp` runtime, preserve
its own MIT notice, the xemu-derived LGPL notices, and the corresponding
source and license text. The repository should contain no original game files
or generated translations of the retail executable. The optional v126 shader specializations are generated locally from your
own extracted archive and ordinary run logs. See [PUBLISHING.md](PUBLISHING.md)
for the repeatable steps and publication review.
