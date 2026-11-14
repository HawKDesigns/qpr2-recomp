# Clean v1 bring-up

Use a separate Windows checkout for this branch and the last tested v126
reference. The clean branch pins a runtime that descends from upstream
XboxRecomp v0.17.0 (`343b6d8901723b37ce568b2418c5df4e1fe1d223`).
Keep the matching retail executable, assets, saves and generated shaders local.
See the [clean v1 requirements audit](CLEAN_V1_AUDIT.md) for evidence and
open acceptance items.

## Port boundaries

| Subsystem | Implementation in clean v1 |
| --- | --- |
| NV2A | QPR2's bounded FIFO execution, GET progress, deferred fences and events, ROV, GPU geometry, texture feedback and readback from v126. The fallback vertex interpreter uses v0.17's instruction decoding. |
| XMV and present | The elapsed guest clock drives the native decoder. Resolved frames go through v0.17's Windows presenter and frame lock. |
| Audio and input | QPR2's wide stereo mixer and rate conversion use v0.17's audio submission and pacing. The launcher asks for the v126 buffer sizes and keeps neutral controller fallback available. |
| Lift | The title enables source-addressed port I/O and WBINVD hooks. The ten reviewed direct-call overrides use v0.17's manual-function list. The title retains 70 manual dispatch entries, including the 52 level callbacks checked at startup. |
| Build and diagnostics | The launcher records both Git revisions, XBE and embedded shader hashes, settings, mode and exit status. Optimized MSVC builds emit PDBs. Debug mode enables lifted guest PC tracking. |

The upstream DMA walker, flip hold and software raster batching are deferred.
They overlap the FIFO path used by the parity build.

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

## Accepted Windows baseline

Benchmark run `20261112-150755-634-9fe8a99e` followed the `45656bf` race
callback fix and reported the v126 embedded shader hash. The user completed a race, reported
performance on par with v126, and confirmed that ATV and rider shadows
rendered during the race. The 53 five-second reports from the first
sustained in-race slowdown onward ranged from 12.6 to 29.4 FPS, with a
median of 18.1 FPS. These windows cover a mixed driving run.

The restored race callback at `0x000AB8B0` produced zero unresolved-call
reports. The run confirms the shadow result visually through the user's
test. The exact draw path responsible for that result is unverified.
The 360-second watchdog ended the capture, and the launcher recorded a
process-not-found error during timeout cleanup.

The Dunes comparison was accepted on November 13. Both follow-up archives
report native ROV, `ready_mask=7`, accepted raw GPU draws and window close.
Run `20261113-015231-241-e8b6f413` (`0`) was mostly with visible AI and had
clear track near the end. Run `20261113-015731-850-dba11550` (`1`) was mostly
behind the pack with no visible AI and rejoined it near the end. The player
confirmed the XboxRecomp update had yet to be copied into the ZIP install.
Leave the experiment off while finishing the v1 debugger and logging
requirements.

## Debugger foundations

Debug mode exports `guest-symbols.json` after the lift. It combines the
generated dispatch table, manual callback registry, disassembly, ABI
inference and reviewed aliases keyed by XBE hash and guest virtual address.
The catalog includes exact host symbol spellings for PDB lookups and labels
inferred names and reviewed roles separately.

Benchmark and Debug runs export `memory-map.json` from the XBE section
table and the current run's loader observations. Its ranges use exclusive
ends. It includes the observed contiguous/tiled mapping and alias check,
heap and stack arenas, apertures and registered D3D device field offsets.
Sections include the permissions declared by the XBE. These exports run
outside the game process and are included in the results ZIP.

```powershell
.\rebuild-and-run.ps1 -Mode Debug
$run = Get-ChildItem logs -Directory -Filter "clean-v1-*" | Sort-Object Name | Select-Object -Last 1
py -3 tools/qpr2_debug_metadata.py lookup "$($run.FullName)/guest-symbols.json" 0x000AB8B0
py -3 tools/qpr2_debug_metadata.py lookup "$($run.FullName)/memory-map.json" 0xF03D8000
```

The memory export is a startup snapshot. Live allocations, thread stacks,
host page permissions and GPU target ownership need runtime snapshots.
Frame events already use typed per-producer rings, and Benchmark runs export
distinct flip, handoff and draw records with phase marks. Remaining work
includes typed rings and crash history for the other producers, game-side
phase marks, debugger snapshots, pause/step and selective watches. F2 opens
the frame-time overlay on Windows. Historical cleanup and further tuning
follow those foundations.

## Where race time goes

Use `[GPU-FIFO] execute_ms` for executor occupancy and `[THREAD-CPU]` for
actual CPU time. Completion-marker and readback Map measurements include
time waiting for previously submitted GPU work. Compare those timings
against `profile-summary.json` for the same marked scenes.
