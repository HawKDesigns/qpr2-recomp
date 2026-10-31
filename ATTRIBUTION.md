# Attribution and license scope

## Original game

**ATV: Quad Power Racing 2** (2003) was developed by **Climax Brighton** and
published by **Acclaim Entertainment** under its **AKA Acclaim** label. The
original game's executable, artwork, music, sound, video, trademarks, and
other assets belong to their respective rights holders. This independent fan
project is not affiliated with or endorsed by Climax, Acclaim, or Microsoft.
Users must supply game files from their own copy. This repository's MIT license
does not grant any rights to the original game.

## Project code

The independently authored QPR2 host code, build scripts, documentation, and
tests are under the repository's [MIT license](LICENSE), except where a file
states otherwise. The license does not extend to retail-derived translations,
game data, or separately licensed third-party code.

## Runtime and references

| Project | Role | Licensing and credit |
| --- | --- | --- |
| [XboxRecomp](https://github.com/sp00nznet/xboxrecomp) | Static recompiler and Xbox runtime | By sp00nz and contributors; predominantly MIT. Keep its own `LICENSE`, `NOTICE`, `CONTRIBUTORS.md`, and `LICENSES/` in any modified fork or vendored copy. |
| [xemu](https://github.com/xemu-project/xemu) | Source of portions of XboxRecomp's APU and NV2A definitions | The particular upstream files identified in XboxRecomp's `NOTICE` remain LGPL-2.1-or-later, including modifications to those files. Preserve their headers, the notice, and `LICENSES/LGPL-2.1.txt`. |
| [OpenTricky](https://github.com/GiZcesi/OpenTricky) | Bring-up and project-organization reference | OpenTricky's own code is GPL-3.0-only. A reference credit is not permission to copy that code into an MIT project; any actual adapted code needs a separate license review. |
| [BAHHHD-Extract](https://github.com/HawKDesigns/BAHHHD-Extract) | Bundled BAD/BAH archive extractor used to prepare local `game/ATV.zip` | By HawKDesigns, MIT licensed. The bundled copy is `tools/vendor/bahhhd_extract.py`; its license is preserved in `tools/vendor/BAHHHD_LICENSE`. Extracted game data remains under the original game's rights holders. |
| [Capstone](https://www.capstone-engine.org/), [CMake](https://cmake.org/), [Microsoft Visual Studio](https://visualstudio.microsoft.com/) | Analysis and build tools | Credited as tools used; their respective licenses continue to govern them. These tools are not bundled as part of QPR2's MIT-licensed source. |

XboxRecomp also credits xemu, Cxbx-Reloaded, and XboxDevWiki as technical
references in its own notice. Retain upstream copyright and license headers
when publishing patches to the runtime. If runtime patches are carried in a
fork, the main repository should point to a reachable fork commit rather than
an unmodified upstream submodule commit.

## Distribution boundary

Do not add `game/`, `analysis/`, `src/generated/`, `src/recomp/gen/`,
`rollback/`, saved data, memory dumps, or test captures from the original game
to a public commit. Ignore rules and a staged-file review are both necessary:
ignores do not remove files that Git already tracks.

In the modified runtime, `src/kernel/nv2a_vsh_compiled.h` is generated from
the game's extracted vertex shaders, and
`src/kernel/nv2a_gpu_literal_programs.h` contains game-observed combiner
instruction shapes. These are not covered by QPR2's MIT grant. Keep them local
until a publication approach for those inputs is resolved.
