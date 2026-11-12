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

## November 12, 2026 Windows baseline

Benchmark run `20261112-150755-634-9fe8a99e` used title commit `45656bf`
and the v126 embedded shader hash. The user completed a race, reported
performance on par with v126, and confirmed that ATV and rider shadows
rendered during the race. The 53 five-second reports from the first
sustained in-race slowdown onward ranged from 12.6 to 29.4 FPS, with a
median of 18.1 FPS. These windows cover a mixed driving run.

The restored race callback at `0x000AB8B0` produced zero unresolved-call
reports. The run confirms the shadow result visually through the user's
test. The exact draw path responsible for that result is unverified.
The 360-second watchdog ended the capture, and the launcher recorded a
process-not-found error during timeout cleanup.

## Shared depth experiment

In the same race windows, a five-second report spent a median of 897 ms
waiting for staging buffers to map and 127 ms publishing pixels to CPU
memory. Shared depth dependencies triggered 34.35 seconds of queue drains
across those reports, including time spent mapping and publishing.

The opt-in `RECOMP_GPU_READONLY_DEPTH` experiment keeps a queued color
pass pending when the next target uses the same depth buffer and the old
pass could not change depth. Depth-writing draws, depth clears and aliased
color/depth destinations retain the existing dependency. The default is off.

Run the same race with each setting on the updated runtime:

```powershell
$env:RECOMP_GPU_READONLY_DEPTH = "0"
.\rebuild-and-run.ps1 -Mode Benchmark
$env:RECOMP_GPU_READONLY_DEPTH = "1"
.\rebuild-and-run.ps1 -Mode Benchmark
Remove-Item Env:RECOMP_GPU_READONLY_DEPTH
```

Compare countdown FPS, both visible-AI and clear-view race segments, and
ATV and rider shadows. `GPU-READONLY-DEPTH` reports color-only snapshots
queued without depth publication and shared-depth binds that skipped a
dependency. `GPU-DEPENDENCY-SOURCE` and `GPU-READBACK-TIME` show whether
the skipped dependencies reduce time spent waiting across the whole run.
