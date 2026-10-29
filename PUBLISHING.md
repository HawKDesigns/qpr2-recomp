# Reproduce QPR2 shader specializations locally

The public fork keeps runtime code and generic fallbacks. Public shader-state
metadata is limited to hashes, numeric identifiers, counts, and generalized
technical descriptors; see [SHADER_STATES.md](docs/SHADER_STATES.md). Original
Xbox shader instruction words and captured GPU state stay in each user's
local checkout.

## Vertex programs from your extracted archive

Extract the game's BAD/BAH resources from your own copy until you have the
original `ATV.zip` with `VertexShader/Assembled/VS*` entries. From the QPR2
repo root, run:

```powershell
py -3 tools/compile_qpr2_vertex_programs.py "path\to\ATV.zip"
```

This writes `third_party/xboxrecomp/src/kernel/nv2a_vsh_compiled.h`.
It matches complete shader instruction words and uses live constants.

## Literal combiner programs from a GPU capture

The second generator **does exist**: `tools/generate_qpr2_literal_materials.py`.
It requires `tools/fixtures/qpr2-literal-programs.json`, captured from the
running game. That JSON contains complete instruction-word arrays, unlike
the public hashes and numeric identifiers. Keep the JSON and its generated
program bodies local.

1. Run the QPR2 build with your own game files. `rebuild-and-run.ps1` sets
   `RECOMP_GPU_CAPTURE` to a local path in the run's `logs/` directory.
   Press **F8** in a 3D menu, preview, and race to capture GPU states. A capture
   closes after two frames. Copy/rename each completed `.bin` capture before
   pressing F8 again; the next request overwrites the configured path.
2. From the QPR2 repo root, pass all saved captures to the extractor. Add
   further captures if it says any of the three exact shapes or the three
   variants of class 2 are missing:

   ```powershell
   py -3 tools/extract_qpr2_literal_programs.py `
     "logs\menu-capture.bin" "logs\preview-capture.bin" "logs\race-capture.bin"
   ```

3. Generate the two local files from the exact-shape fixture:

   ```powershell
   py -3 tools/generate_qpr2_literal_materials.py
   ```

   This writes `third_party/xboxrecomp/src/kernel/nv2a_gpu_literal_programs.h`
   (CPU classifier and fixtures) **and**
   `third_party/xboxrecomp/src/kernel/nv2a_gpu_literal.inc` (literal GPU
   material bodies). The build then regenerates the ignored
   `nv2a_gpu_shader.h` from local HLSL and include sources.

The extractor checks SHA-256 fingerprints of the three v126 instruction shapes
and their final variants. It fails on incomplete or different captures rather
than silently selecting a different program. More than one capture is usually
needed; a single F8 request is only two frames. Exact v126 output SHA-256s:

| Generated file | SHA-256 |
| --- | --- |
| `nv2a_vsh_compiled.h` | `6347a41fa83a2dfae848e6398573eb5b46b37334b363c068c7b46119b7ff357d` |
| `nv2a_gpu_literal_programs.h` | `169bc9b0d0d421273d8705d924ce486afa310cba0edee5868d532bd1ef87e178` |
| `nv2a_gpu_literal.inc` | `e15bc49d65080a033479c3d065400543fa7e3970f0eb7942353abfd051e71f7c` |

The vertex hash assumes the same extracted archive and generator version.
The literal hashes assume the same v126 shapes and generator version.
Captured `.bin` files contain more than combiner words; keep them private.

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
