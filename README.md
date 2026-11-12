# ATV: Quad Power Racing 2 — Xbox static recompilation

An experimental native Windows recompilation of the original Xbox version of
**ATV: Quad Power Racing 2**, built on
[XboxRecomp](https://github.com/sp00nznet/xboxrecomp). You must supply game data
from your own copy. The project does not distribute retail executable or asset
files.

## Clean v1 on XboxRecomp v0.17.0

This branch ports the working v126 QPR2 runtime onto upstream XboxRecomp
v0.17.0. It keeps the v126 native D3D11 ROV renderer, raw GPU geometry,
ordered FIFO completion, XMV clock and recovered title callbacks. The retail
boot-to-race path and performance parity still need Windows hardware tests.

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
For a clean performance comparison, use the same Dunes1 setup and note where
you drove with visible AI and where the track ahead was clear. Keep F8 capture
and other verbose diagnostics outside measured intervals.

The launcher requests native ROV and raw GPU geometry. Confirm what the
runtime actually admitted in the logs. Runtime defaults are in
`tools/qpr2_runtime.psd1` and can be overridden before launch. For example,
`RECOMP_GPU_POINT_BOUNDS=0` restores the prior AABB decision for a same-build
comparison.

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
