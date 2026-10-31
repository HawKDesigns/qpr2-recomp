# Reproduce QPR2 shader specializations locally

The public fork keeps runtime code and generic fallbacks. Public shader-state
metadata is limited to hashes, numeric identifiers, counts, and generalized
technical descriptors; see [SHADER_STATES.md](docs/SHADER_STATES.md). Original
Xbox shader instruction words and captured GPU state stay in each user's
local checkout.

## Vertex programs from your extracted archive

`ATV.zip` is made locally from **all files inside the extracted ATV folder**.
The ZIP root contains `VertexShader/Assembled/VS*` and other archive files;
it does not contain an outer `ATV/` directory. The retail files and generated
ZIP stay under ignored `game/`.

1. Extract your own original Xbox ISO into `game/` with an Xbox ISO
   extraction utility. Leave its matching `ATV.BAD` and `ATV.BAH` there
   (or together in a nested directory under `game/`).
2. Run `rebuild-and-run.ps1`. If
   `third_party/xboxrecomp/src/kernel/nv2a_vsh_compiled.h` is missing, the
   launcher runs `tools/prepare_qpr2_atv_archive.py`. That script uses
   the MIT licensed [BAHHHD-Extract](https://github.com/HawKDesigns/BAHHHD-Extract)
   copy at `tools/vendor/bahhhd_extract.py` to extract the **entire**
   BAD/BAH pair into `game/ATV/`, then packages every file inside the
   folder as `game/ATV.zip`. ZIP entry dates and attributes are fixed for
   repeatable packaging. The launcher then compiles the native vertex header.
3. Existing complete `game/ATV/` and `game/ATV.zip` are reused. If the
   folder is incomplete but the BAD/BAH pair is present, the tool re-extracts
   from the pair, retains the old folder as `game/ATV.backup-*/`, and writes
   a new ZIP. An existing ZIP is checked against the BAH inventory when that
   pair is available.

You can run the preparation independently to inspect the result:

```powershell
py -3 tools/prepare_qpr2_atv_archive.py
py -3 -c "import zipfile; z=zipfile.ZipFile('game/ATV.zip'); print(len([i for i in z.infolist() if not i.is_dir()]), 'files'); assert any(n.startswith('VertexShader/Assembled/VS') for n in z.namelist())"
```

If you change the local source archive after generating the native header,
remove that header and rerun the launcher so the vertex specialization can be
regenerated. The launcher checks the known v126 shader fingerprint before
building; differing game releases need their own verification.

## Literal combiner programs from ordinary play

The complete shader instruction words and captured GPU state stay in each
user's checkout. The launcher learns the three reviewed material shapes from
its own ordinary stderr logs; no F8 snapshots, timed captures, or manual
extractor commands are required.

1. Extract your game files under `game/` and launch `rebuild-and-run.ps1`.
   The launcher prepares `ATV.zip` and the vertex header. This first run
   uses the generic material fallback if no local fixture exists. Play
   normally through a 3D menu, preview, and race, then close the game.
2. After the game exits, the launcher scans its saved run logs and writes
   `tools/fixtures/qpr2-literal-programs.json` only after all three exact
   shapes and the three class-2 final variants have been observed. Earlier
   runs count too, so a partial first run is fine.
3. Launch `rebuild-and-run.ps1` again. It generates both local files
   automatically: `third_party/xboxrecomp/src/kernel/nv2a_gpu_literal_programs.h`
   and `third_party/xboxrecomp/src/kernel/nv2a_gpu_literal.inc`. CMake
   re-embeds the GPU shader and recompiles code that previously used the
   fallback. The second run should report all three `[SHADER-STATUS]`
   entries as `ready`.

This is a one-time learning pass per matching game build. If you have old
normal stderr logs from before this launcher update, the next launch scans
them before building, so it can skip the learning pass. If a class has not
appeared yet, keep playing normally; the launcher reports how many variants
it still needs. For advanced diagnosis, the extractor still accepts logs or
F8 `.bin` files manually:

```powershell
py -3 tools/extract_qpr2_literal_programs.py ".\\logs\\v126-run-...\\one-hundred-twenty-sixth-run.stderr.log"
py -3 tools/generate_qpr2_literal_materials.py
```

The extractor checks SHA-256 fingerprints of the three v126 instruction shapes
and their final variants. These words are program instructions, not frame
coordinates or live constants, so the same tested game build executing the
same materials produces the same shapes. Other game revisions need their own
review; the extractor fails on different instruction shapes rather than
silently selecting an untested program. A normal run log from
menu through race contained all three classes and all three class-2 variants
in the v126 test. Exact v126 output SHA-256s:

| Generated file | SHA-256 |
| --- | --- |
| `nv2a_vsh_compiled.h` | `6347a41fa83a2dfae848e6398573eb5b46b37334b363c068c7b46119b7ff357d` |
| `nv2a_gpu_literal_programs.h` | `169bc9b0d0d421273d8705d924ce486afa310cba0edee5868d532bd1ef87e178` |
| `nv2a_gpu_literal.inc` | `e15bc49d65080a033479c3d065400543fa7e3970f0eb7942353abfd051e71f7c` |

The vertex file hash assumes the same extracted ZIP bytes and generator
version. The launcher also recognizes the original vertex program output
after replacing only the ZIP SHA-256 comment with a stable marker; this
accepts equivalent programs repacked with different ZIP metadata.
The literal hashes assume the same v126 shapes, fork source and generator
version. The original v126 embedded shader header has SHA-256
`0e50cf80daa37174a94dc3c85feec8cf2744b3d3359eed8ab1e54a1b90a35465`.
The pinned fork now checks embedded shader source content on each build,
including when ZIP extraction preserves older file timestamps.
Run logs and optional captured `.bin` files contain game-derived state; keep them private.

If you already have the three generated shader files in your local v126
checkout, you may copy them into a fresh checkout. Do not copy the old
`nv2a_gpu_shader.h`: the build creates it from the selected local includes.

## Before publishing

The fork tracks a **generic placeholder** `nv2a_gpu_literal.inc`; local
generation modifies it. Do not stage that local change. The two local
headers, fixture, and generated embedded shader header are ignored, but an
ignore rule does not protect a file deliberately force-added. Check the
staged diff **in both repositories**. Exclude `game/`, `src/generated/`,
`src/recomp/gen/`, `analysis/`, saves, captures, shader caches, and logs
containing game data. Retain XboxRecomp's original licenses and notices.

Older public fork commits contained generated literal bodies and an embedded
shader header. Removing files from the latest tree does not erase those
earlier Git objects. Treat that history as requiring a separate publication
review before advertising its history as free of embedded program words.
