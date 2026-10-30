# Reproduce QPR2 shader specializations locally

The public fork keeps runtime code and generic fallbacks. Public shader-state
metadata is limited to hashes, numeric identifiers, counts, and generalized
technical descriptors; see [SHADER_STATES.md](docs/SHADER_STATES.md). Original
Xbox shader instruction words and captured GPU state stay in each user's
local checkout.

## Vertex programs from your extracted archive

The `ATV.zip` used here is **made locally from the contents of an
extracted ATV folder**. It is not a ZIP found directly on the disc.

1. Extract your own original Xbox game ISO into this repo's `game/` folder
   with an Xbox ISO extraction utility. Find the matching `ATV.BAD` and
   `ATV.BAH` files in those extracted game files. The commands below assume
   `game/ATV.BAD` and `game/ATV.BAH`; adjust the paths if your ISO extractor
   placed them elsewhere.
2. Clone [BAHHHD-Extract](https://github.com/HawKDesigns/BAHHHD-Extract)
   beside this QPR2 repository. From the **QPR2 repository root**, list and
   then extract the pair into `game/ATV/`:

   ```powershell
   git clone https://github.com/HawKDesigns/BAHHHD-Extract.git ..\BAHHHD-Extract
   py -3 ..\BAHHHD-Extract\bad_extract.py ".\game\ATV.BAD" --bah ".\game\ATV.BAH" --list
   py -3 ..\BAHHHD-Extract\bad_extract.py ".\game\ATV.BAD" --bah ".\game\ATV.BAH" -o ".\game\ATV"
   ```

   If you already cloned the tool, skip the clone command. Resolve any
   reported extraction failures before making the ZIP.
3. **Go into `game/ATV/` and ZIP its contents**, naming the result
   `game/ATV.zip`:

   ```powershell
   Push-Location .\game\ATV
   Compress-Archive -Path .\* -DestinationPath ..\ATV.zip -Force
   Pop-Location
   ```

   The ZIP root must contain `VertexShader/Assembled/VS*` entries, **without**
   an outer `ATV/` directory. Check its layout:

   ```powershell
   py -3 -c "import zipfile; names=zipfile.ZipFile('game/ATV.zip').namelist(); assert any(n.startswith('VertexShader/Assembled/VS') for n in names), 'Shader entries are not at the ZIP root'; print('ATV.zip layout OK')"
   ```

4. From the QPR2 repository root, compile the local vertex programs:

   ```powershell
   py -3 tools/compile_qpr2_vertex_programs.py ".\game\ATV.zip"
   ```

This writes `third_party/xboxrecomp/src/kernel/nv2a_vsh_compiled.h`.
It matches complete shader instruction words and uses live constants. Once
`game/ATV.zip` exists, the current launcher can also generate this file before
building.

## Literal combiner programs from a normal run log

`tools/generate_qpr2_literal_materials.py` requires the local
`tools/fixtures/qpr2-literal-programs.json`. The fixture contains complete
instruction-word arrays, unlike the public hashes and numeric identifiers.
Keep the JSON and generated shader files local.

1. Run your own game through a 3D menu, preview, and race. The normal v126
   stderr log records each distinct `[GPU-MATERIAL-PROGRAM]` state. You do
   **not** need to time F8 snapshots. The launcher saves each run under its
   own `logs/v126-run-*/` folder.
2. From the QPR2 repo root, extract the exact shapes from that run's log:

   ```powershell
   $log = Get-ChildItem .\logs\v126-run-*\one-hundred-twenty-sixth-run.stderr.log |
     Sort-Object LastWriteTime -Descending | Select-Object -First 1
   py -3 tools/extract_qpr2_literal_programs.py $log.FullName
   ```

   If the newest log lacks any required state, pass additional completed
   menu/preview/race stderr logs to the same command. The extractor also
   accepts saved F8 `.bin` captures as optional inputs.
3. Generate **both** local files from the fixture:

   ```powershell
   py -3 tools/generate_qpr2_literal_materials.py
   ```

   This writes `third_party/xboxrecomp/src/kernel/nv2a_gpu_literal_programs.h`
   (CPU classifier and fixtures) **and**
   `third_party/xboxrecomp/src/kernel/nv2a_gpu_literal.inc` (literal GPU
   material bodies). The build then regenerates the ignored
   `nv2a_gpu_shader.h` from local HLSL and include sources. Once the exact-shape
   JSON fixture exists, the current launcher can generate these two files
   before building. Verify that **both** files exist and the `.inc` is
   generated rather than the 94-byte placeholder; their expected hashes
   appear below.

The extractor checks SHA-256 fingerprints of the three v126 instruction shapes
and their final variants. It fails on incomplete or different logs/captures
rather than silently selecting a different program. A normal run log from
menu through race contained all three classes and all three class-2 variants
in the v126 test. Exact v126 output SHA-256s:

| Generated file | SHA-256 |
| --- | --- |
| `nv2a_vsh_compiled.h` | `6347a41fa83a2dfae848e6398573eb5b46b37334b363c068c7b46119b7ff357d` |
| `nv2a_gpu_literal_programs.h` | `169bc9b0d0d421273d8705d924ce486afa310cba0edee5868d532bd1ef87e178` |
| `nv2a_gpu_literal.inc` | `e15bc49d65080a033479c3d065400543fa7e3970f0eb7942353abfd051e71f7c` |

The vertex hash assumes the same extracted archive and generator version.
The literal hashes assume the same v126 shapes and generator version.
Run logs and optional captured `.bin` files contain game-derived state; keep them private.

If you already have the three files in your local v126 checkout, copy them
into the same paths in a separate v0.16 trial clone. Do not copy the old
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
