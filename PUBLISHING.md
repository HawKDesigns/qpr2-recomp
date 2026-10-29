# Publishing and local game-derived data

This project publishes source for an independent Xbox recompilation. It does
not grant permission to redistribute the retail game, its extracted assets,
translated retail code, or data captured from its shaders. Review the complete
working tree and generated outputs before publishing a new branch or release.

## Shader headers in the XboxRecomp fork

The QPR2 runtime can use two optional game-specific headers:

| Local path under `third_party/xboxrecomp/src/kernel/` | Source | Public fork |
| --- | --- | --- |
| `nv2a_vsh_compiled.h` | Generated from the user's extracted `ATV.zip`; includes exact shader instruction words and native specializations | Omitted and ignored |
| `nv2a_gpu_literal_programs.h` | Captured game-specific combiner/literal programs | Omitted and ignored |

These files were present in the local v126 development runtime, but were
**not committed** to `HawKDesigns/xboxrecomp` on `qpr2-v126` or
`qpr2-v016-experimental`. The public runtime has generic fallback paths.
A build or standalone test that passes without these files does not establish
the same rendering behavior or frame rate as a local v126 race.

For a comparison using your own game data, clone the experimental QPR2 branch
in a separate directory and copy both headers from your existing local v126
checkout into the same relative `third_party/xboxrecomp/src/kernel/` directory.
On Windows PowerShell, from the **experimental QPR2 repo root**:

```powershell
$sourceKernel = "S:\ATV qpr 2 recomp\third_party\xboxrecomp\src\kernel"
$targetKernel = ".\third_party\xboxrecomp\src\kernel"
Copy-Item (Join-Path $sourceKernel "nv2a_vsh_compiled.h") $targetKernel
Copy-Item (Join-Path $sourceKernel "nv2a_gpu_literal_programs.h") $targetKernel
```

Adjust `$sourceKernel` to your actual working v126 checkout. If you need to
regenerate the first header from **your own extracted archive**, run this from
the QPR2 root:

```powershell
py -3 tools/compile_qpr2_vertex_programs.py "path\to\your\ATV.zip"
```

That script does not generate `nv2a_gpu_literal_programs.h`. Restore the
second header only from your own local v126 development files; if it is
unavailable, use the public fallback and label any performance result as a
fallback run. Neither header belongs in a public commit, release asset, or
support ZIP. They are ignored by the runtime fork's `.gitignore`; keep the
local copies out of your published patch as well.

## Before publishing

- Check `git status` and the staged diff in both the QPR2 repository and its
  `third_party/xboxrecomp` submodule. Verify that the two shader headers are
  untracked/ignored and absent from the commit tree. Ignore rules do not
  protect files already force-added.
- Exclude `game/`, `src/generated/`, `src/recomp/gen/`, `analysis/`,
  retail executables, archives, saves, RAM dumps, and logs containing
  extracted game content. Recheck any new paths outside the usual directories.
- Preserve the MIT and third-party notices in the XboxRecomp fork, including
  the xemu-derived LGPL material and corresponding source. See
  [ATTRIBUTION.md](ATTRIBUTION.md) and the fork's licenses.
- Test a clean public clone separately from the game-specific local setup;
  state which one was tested when reporting build success or race FPS.

The public fork contains the runtime modifications, not the two optional
shader tables. The separate QPR2 v0.16 trial branch pins the experimental
runtime commit and does not change `main`.
