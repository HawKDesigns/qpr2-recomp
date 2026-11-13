# QPR2 clean v1 requirements audit

The clean runtime descends from XboxRecomp v0.17.0 (`343b6d8`). The title
submodule and `tools/xboxrecomp-pin.txt` must identify the same runtime. The
last tested v126 remains available at title `0f55e19` and runtime `92a5f03`.

## Baseline and settings

The Dunes1 comparison is accepted. The Windows acceptance checklist also
covers intro, input, menus, preview, terrain, audio and race finish.
See [CLEAN_V1.md](CLEAN_V1.md).

QPR2 enables queued surface reuse by default. The renderer can reuse a
finished surface as a queued snapshot and report the logical copy savings.
The launcher and environment overrides remain available for comparisons.

## Source and runtime requirements

| Requirement | Status | Evidence or remaining work |
| --- | --- | --- |
| Verified XBE, local shader/lift inputs, callback registry and separate v126 history | Implemented | The runner checks the XBE hash and runtime pin. The title generates lifted C and shader inputs locally. Numbered recovery units and scattered title address tables still need migration. |
| Native raster and ordered guest completion | Baseline verified | The Windows admission checks and local queue, fence and publication fixtures cover the accepted path. Retest after changing depth ownership. |
| Reproducible run identity | Implemented with hardware fields to verify | `run.json` records the executable, shader and archive hashes, runtime pin, effective environment, toolchain, framebuffer size, CPU, RAM, matching driver version when available, and renderer admission messages. |
| Phase and frame-time comparison | Implemented with partial automatic labeling | `frames.csv`, `frame-summary.json`, manual F4–F7 and a folder comparison cover flip, handoff, draw, guest display vblank, maximum, duration and over-budget intervals. Video and preview activity supply provisional automatic labels. Rider/ATV menus, countdown and racer visibility need validated game-side transitions. |
| Attributed performance measurements | Implemented with Windows verification pending | Benchmark exports per-frame FIFO, target, CPU wait and GPU sample data, boundary fallback counts and observed ZETA layouts. The five-second report adds per-role Windows thread CPU time. `-SampleGuestPc` adds section-bucketed last guest-main PC samples. CPU wall scopes overlap GPU work and other threads. |
| Shared depth layout and ownership | Candidate available | The depth cache uses one float buffer per ZETA offset. The opt-in full-clear path can retire older queued depth writes when the clear covers their entire cache extent. A Windows comparison must establish uptake, pacing and rendering parity. Other layout transitions still use the existing dependency path. |
| Bounded events and failure context | Partial | Frame, wait, dependency and sample events use the existing asynchronous recorder. Failure export can include watchdog registers, recent calls and FIFO progress. Live wait age and target generation, and targeted subsystem/address capture, remain open. |
| Live stats and debugger | Partial | F2 displays flip FPS, the recent frame-time graph, p95, budget misses and draw count. Additional snapshot fields, safe pause/step, watch controls and live debugger views remain open. F3 marks a pause in the trace. |
| Symbols and memory map | Offline foundation | Debug exports a hash-keyed symbol catalog. Benchmark and Debug export the startup memory map. Executed coverage, unresolved-callsite export, live allocations, stacks and bus ownership remain open. |
| Mode overhead and optimization cleanup | Open | Compare tracing disabled, basic pacing, profiling and the open overlay on the same optimized build. Use matching scenes and integrity counters before removing active cache, batching or readback paths. |

## Next Windows capture

Run Benchmark with the default renderer settings. Mark countdown, clear track
and visible AI. Keep F2 closed during the measured intervals. The next trace
should show whether FIFO wall time is active CPU time or blocked, and which
ZETA bindings coincide with the expensive drains. Once those results are
available, use the same scenes for the depth-clear comparison. A separate
PC-tracked run can narrow down guest-main work if it accounts for a material
share of a core.
