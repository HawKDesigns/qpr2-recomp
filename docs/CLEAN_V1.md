# Clean v1 bring-up

Use a separate Windows checkout for this branch and the last tested v126
reference. The clean branch pins a runtime that descends from upstream
XboxRecomp v0.17.0 (`343b6d8901723b37ce568b2418c5df4e1fe1d223`).
Keep the matching retail executable, assets, saves and generated shaders local.

## Port boundaries

| Subsystem | Implementation in clean v1 |
| --- | --- |
| NV2A | QPR2's bounded FIFO execution, GET progress, deferred fences and events, ROV, GPU geometry, texture feedback and readback from v126. The fallback vertex interpreter uses v0.17's instruction decoding. |
| XMV and present | The elapsed guest clock drives the native decoder. Resolved frames go through v0.17's Windows presenter and frame lock. |
| Audio and input | QPR2's wide stereo mixer and rate conversion use v0.17's audio submission and pacing. The launcher asks for the v126 buffer sizes and keeps neutral controller fallback available. |
| Lift | The title enables source-addressed port I/O and WBINVD hooks. The ten reviewed direct-call overrides use v0.17's manual-function list. The title retains 70 manual dispatch entries, including the 52 level callbacks checked at startup. |
| Build and diagnostics | The launcher records both Git revisions, XBE and embedded shader hashes, settings, mode and exit status. Optimized MSVC builds emit PDBs. Debug mode enables lifted guest PC tracking. |

The upstream DMA walker, flip hold and software raster batching are deferred.
They overlap the FIFO path used by the parity build. Guest PC tracking and
the existing statistics are the debugger foundations. The live debugger UI,
pause/step, symbol catalog and live memory map are follow-on work.

## First hardware comparison

1. Run `rebuild-and-run.ps1 -Mode Benchmark` with a matching North American
   `game/default.xbe` and its disc files. If this is the first shader-learning
   run, play a 3D menu, preview and race, close the game and run it again.
2. Confirm the logs show all three shader inputs ready, the reference
   embedded shader hash, 180 vertex variants, native ROV admission,
   `ready_mask=7` and nonzero accepted raw GPU geometry.
3. Check native intro and Press Start, name entry, rider and ATV selection,
   preview, automatic countdown, Dunes1 race, results and exit. Check
   terrain, tires, controls and audio during that run.
4. Compare the warmed build with the v126 reference on the same machine.
   Record countdown, behind visible AI and clear-view segments separately.
   Keep resolution, camera, rider, vehicle and grid size fixed. Include both
   run ZIPs and the approximate times for the visibility transitions.

For a build-only check run `rebuild-and-run.ps1 -BuildOnly`. For a stall
or incorrect picture, use Debug mode and keep the associated run folder.
F8 captures render state when requested. Debug capture changes the timing
and should be kept outside a benchmark.
