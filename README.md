# ATV: Quad Power Racing 2 — Xbox static recompilation

An experimental native Windows recompilation of the original Xbox version of
**ATV: Quad Power Racing 2**, built on
[XboxRecomp](https://github.com/sp00nznet/xboxrecomp). You must supply game data
from your own copy. The project does not distribute retail executable or asset
files.

## Current status (diagnostic overlay v126)

The matching North American retail XBE has been analyzed and lifted: 8,792
functions translated, with no failed functions in the initial lift. Windows
diagnostic builds boot into menus, play the intro, and run races. The v126 test
built and ran successfully. Its preview and race performance returned to the
pre-v125 range, but **sustained 30 FPS in races has not been achieved**. Frame
rate still varies considerably with the scene and nearby AI racers. Some
vehicle/rider sound effects and mounted rider/ATV shadows remain incomplete.

| Area | Status |
| --- | --- |
| Input and menus | Working for the current test path |
| Race and game simulation | Playable in diagnostic builds; performance work ongoing |
| Rendering | GPU raster/geometry paths with CPU fallbacks; output checks at startup |
| Audio | Mostly working; some effects still incorrect or absent |
| Race frame rate | Variable; the stable 30 FPS goal is open |

The renderer has conservative draw rejection, translated mesh reuse, shader
caching, GPU target handling, and profiling. These are **not** full AI vehicle
instancing, object occlusion culling, GPU particle simulation, or frame-rate
interpolation. See `tools/v126-feature-audit.md` for a more
specific implementation audit.

## Requirements

- Your legally obtained Xbox game files, extracted into `game/` with
  `game/default.xbe` present.
- Windows 10/11, Visual Studio 2022 with **Desktop development with C++**, and
  CMake 3.20 or newer for the host build.
- Python 3.10 or newer and Capstone (`py -3 -m pip install capstone`) if you
  run the analysis pipeline.
- A disc extraction tool such as `extract-xiso` or `xdvdfs` for your own disc.

The tested build identity is in [docs/KNOWN_BUILD.md](docs/KNOWN_BUILD.md).
Different releases may have different code addresses; verify the full XBE
SHA-256 before using build-specific overrides.

## Set up the source and game files

```powershell
git clone --recurse-submodules https://github.com/HawKDesigns/qpr2-recomp.git
cd qpr2-recomp
py -3 -m pip install capstone
```

For an existing Git checkout, update both the QPR2 repo and its pinned
XboxRecomp submodule:

```powershell
git pull --ff-only
git submodule update --init --recursive
```

If you downloaded ZIPs instead, use the fork's
`qpr2-v126` branch for `third_party/xboxrecomp/`; its default `main`
is the upstream v0.16 code. Keep your local generated shader files when
refreshing that folder.

Extract your disc into `game/`. The recompilation pipeline creates local files
in `analysis/`, `src/generated/`, and `src/recomp/gen/`; these are derived from
the supplied executable and are not checked in. If setting up from a fresh
source checkout rather than continuing an existing diagnostic installation,
run the pipeline before building:

```powershell
py -3 tools/qpr2_pipeline.py doctor
py -3 tools/qpr2_pipeline.py analyze
py -3 tools/qpr2_pipeline.py all
```

The pipeline examines all code-bearing XBE sections, not only `.text`. Do not
commit or share `game/`, generated translations, saves, memory dumps, or other
retail-derived artifacts.

To make `game/ATV.zip` for the native vertex shaders, follow the ISO extraction,
BAHHHD-Extract, and ZIP layout steps in [PUBLISHING.md](PUBLISHING.md#vertex-programs-from-your-extracted-archive). Keep the extracted folder and ZIP under
`game/` so they remain ignored by Git.


For comparable v126 race performance, prepare `game/ATV.zip` from your
own extracted disc. The launcher generates the 180 native vertex variants
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

## Run the current diagnostic build

The repository contains the cumulative v126 source. Clone with submodules,
prepare the game files from your own disc as described above, and run from the
repository root:

```powershell
.\rebuild-and-run.ps1
```

The launcher configures a separate `build-diagnostic-v126/`, builds Release,
starts the game, and writes per-run logs and `logs/qpr2-v126-results.zip` after
the run. It leaves previous run directories in `logs/` for comparison. If a
build fails, send the configure/build log and exit code from the new run
directory. For a performance run, include the results ZIP and say when you
were behind, alongside, or ahead of AI racers; scene differences matter.

The launcher normally enables the native GPU raster and GPU geometry paths.
No extra `RECOMP_GPU_NATIVE=1` setting is required for its default run. For a
same-build comparison of v126's point-bounds optimization, set this **before**
launching and remove it afterward:

```powershell
$env:RECOMP_GPU_POINT_V126 = "0"
.\rebuild-and-run.ps1
Remove-Item Env:RECOMP_GPU_POINT_V126
```

That switch changes one optimization, not the complete renderer. Avoid
enabling optional frame, vertex, texture, audio, or verbose render captures
when comparing FPS: their synchronous output can affect frame time.

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
