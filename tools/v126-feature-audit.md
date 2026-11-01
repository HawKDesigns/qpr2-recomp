# QPR2 v126 feature audit

## V125 regression

V125 built and exited normally. The user observed worse preview and race
performance. Its last six five-second race windows had a 15.58 FPS median.
The v124 run's final ten had a 19.49 FPS median, although AI and camera
positions differ and those medians cannot establish a controlled effect.

The new `[GPU-POINT-BOUNDS]` counters were decisive: about 26,000 draws
per late race window passed the point proof, but sampled
`[GPU-GEOMETRY-CPU] phase=bounds` time extrapolates to roughly 0.8–1.1
seconds per five-second window. The complete draw submission path rose to
about 3.1 seconds per window despite less CPU clipping. V125's per-vertex
rounding operations erased its benefit.

## V126 correction

The point proof now bounds float rounding error for each position dot
product using the sum of absolute products, then bounds reciprocal, scale
and offset. It keeps inward and outward screen margins. NaN, overflow,
reciprocal limits and near-boundary cases use CPU clipping; only draws
provably inside or wholly beyond one plane change path. The mesh byte cache
and existing GPU geometry execution are unchanged.

In a local C/D3D shim benchmark of 150,000 correlated four-vertex checks,
the new proof took about 160 ms versus 462 ms for v125's exact interval
implementation. The renderer fixtures passed with the feature enabled and
disabled, including 1,800 random plus 1,800 correlated trials spanning all
180 known vertex program forms. This benchmark does not establish a Windows
FPS gain. `RECOMP_GPU_POINT_V126=0` restores the v124 AABB decision in the
same build; an inherited `RECOMP_GPU_POINT_V125=0` also disables it.

Stable 30 FPS remains open. Mounted ATV/rider shadows remain unresolved.

## Readback staging experiment (after v126 baseline)

The latest v126 race log confirms that the automatic ATV and shader preparation
returns to the expected performance range. Race intervals still vary with the
scene. In several five-second windows, GPU target readback mapping takes about
0.7–1.5 seconds while the GPU pixel phase is short in sampled batches. The
mesh cache evicts many entries, but its miss count stays high even in faster
race intervals; the log does not make a case for simply enlarging that cache.

The pinned XboxRecomp fork now transfers only the dirty target's containing
row span from GPU surface to CPU staging at current, queued, and deferred
readbacks. Guest color and depth publication still reads the same dirty
rectangle, and the complete GPU snapshot used by texture feedback and target
resume is unchanged. The new `[GPU-READBACK-ROWS]` counter records transfers,
partial transfers, and bytes saved. A 400-case synthetic dirty rectangle test
matched full-copy pixel/depth publication. Windows compilation, output checks,
and the FPS effect need a user race run; this is an experiment, not a measured
frame-rate improvement. Compare the new default against `RECOMP_GPU_PARTIAL_READBACK=0` in the same
build if the Windows run shows a regression. The existing `RECOMP_GPU_NATIVE`
setting does not need to be changed.

## FIFO dependency lookup experiment (following the dirty-row result)

The next race run showed no FPS improvement from dirty-row staging:
`[GPU-READBACK-ROWS]` reported about 4–9 MB avoided from roughly
400–630 MB transferred in typical five-second race windows. Most large
targets are fully dirty, so that transfer change cannot resolve the
frame-rate limit.

The FIFO executor reads approximately 6–9 million pushbuffer words per
five-second race window. Each read checks for overlap with queued GPU
color/depth targets before the guest can consume their memory. The fork now
caches the canonical address and exact size of each queued target once and
reuses those ranges for the per-word dependency check. When no target is
pending and no active deferred publication exists, it skips the target-memory
lookup. Missing cached metadata retains the original overlap implementation.
The check still drains the queued readback on a matching range. The new
`[GPU-PENDING-ACCESS]` line reports the fast path's use.

A randomized 100,000-case comparison covered direct addresses, tiled
aliases, zero sizes, and overflowing ranges against the original overlap
predicate. Windows build and race FPS remain unverified. Set
`RECOMP_GPU_FAST_PENDING_ACCESS=0` for a same-build comparison.

## Sampled batch timing pass

The run after the FIFO pending-range cache showed no visible FPS change.
`[GPU-PENDING-ACCESS]` counts roughly 8–12 million empty checks and around
100,000 actual range checks in representative five-second windows; that
lookup was not the main cost. `[RENDER-TIME] batch_ms` remains about
2.3–2.9 seconds per window, while CPU fallback transform/raster totals
are often around one second.

The fork now reports 1/64 sampled CPU times for FIFO word reads and
method execution (`[GPU-FIFO-CPU]`), raw draw setup versus GPU submission
(`[PB-RAW-CPU]`), and raw geometry reserve, bookkeeping and total
(`[GPU-GEOMETRY-CPU]`). Each line includes exact call counts and sampled
time; `decode_residual_est_ms` is a noisy difference estimate. The
existing `[GPU-FLUSH-CPU]` measures full CPU upload/dispatch timings.
These nested measurements must not be added together.

GPU pixel timestamps in `[GPU-PASS-TIME]` are also samples. For example,
22 ms across 42 sampled batches in this run is not 22 ms for the whole
five-second window: roughly 2,400 total flush batches at that mean would
represent about 1.3 seconds of GPU pixel work, potentially overlapped
with CPU submission and waits. This profiling pass changes no rendering
algorithm and does not claim an FPS gain.

## Requested features

| Feature | Status in v126 | Outstanding |
|---|---|---|
| Strict frustum and occlusion culling | Conservative AABB and bounded per-vertex proofs; uncertain cases retain CPU clipping. | Object-wide AI/track occlusion needs reliable identities and depth evidence. |
| AI ATV instancing | Ordered cross-mesh batching exists. | Per-vehicle matrix capture and identical-mesh instancing unimplemented. |
| Static geometry in VRAM | Byte-verified immutable versions in a 64 MiB GPU atlas and host cache. | Zone-level lifetime and measured benefit. |
| Vehicle physics/collision | CPU. | Retain on CPU. |
| AI pathfinding and logic | CPU. | Retain on CPU. |
| Audio mixing/buffering | CPU. | Retain on CPU. |
| GPU transforms and lighting | Supported vertex programs use GPU transform and triangle preparation; bounded proofs can admit additional draws. | Unsupported/boundary programs use CPU fallback. |
| GPU shadow maps/CSM | Unimplemented. | Identify mounted shadow pass; mounted shadows still absent. |
| Compute particles | Unimplemented. | Locate update loop and data structures first. |
| Persistent mapping and dynamic indices | Bounded rotating uploads. | No persistent host-visible mapping. |
| Tick/render interpolation | Unimplemented; game timing retained. | Establish tick dependence first. |
| Geometry batching and material binding | Ordered batching, native materials, state interning, short-run fusion. | CPU command processing and GPU synchronization remain costly. |
| Texture and shader caches | Keyed shaders, byte-verified texture reuse and GPU feedback bridge. | Bulk conversion has no measured FPS case. |

## Results (7): reservation and target flow diagnosis

The run reports no noticeable FPS change, matching the previous measurement. In
62 high-draw five-second windows, the median was 91,609 raster batches per
window. About 78,000 raw draws were accepted, 10,000 used the CPU boundary
fallback, and 2,700 were rejected by bounds in a representative window.
Sampled accepted raw submission extrapolates to about 1.4 seconds per window,
of which the sampled geometry reserve phase accounts for roughly 1.0 second.
The whole accepted geometry path extrapolates to about 1.5 seconds. These
measurements are nested and cannot be added. FIFO method emission estimates
fluctuate, with a roughly 3.6-second median; its residual is often negative
in short windows, so it is not a reliable independent time budget.

The exact raster counters show around 1.1 seconds in target handling and 0.7
seconds in texture handling in many race windows, with hundreds of target
readbacks and up to about 300 dependency drains. The source of the reserve
cost could be target transitions, texture/source comparisons, state setup, or
queue writes. The fork now reports 1/64 sampled accepted reservation phases in
`[GPU-RESERVE-CPU]`, sampled FIFO method groups in `[GPU-FIFO-KIND]`, and
exact target size transitions, queued copies and synchronous reads in
`[GPU-TARGET-FLOW]`. These counters should identify which path to change next.
They do not change rendering behavior or claim a frame-rate gain. The FIFO
scanner passes GCC C11 syntax checks; the Windows renderer still needs a
Windows build and race validation.

The user's ATV archive survey estimates around 65 submeshes per rider/ATV and
up to eight vehicles. This plausibly explains the rise in draw requests but
has not yet been verified against per-vehicle pushbuffer identities. The
reported baked visibility data makes a second object occlusion layer a lower
priority. The pixel shader table alone cannot rule out GPU shading time:
sampled GPU timestamp totals need extrapolation and can overlap CPU waits.
The next performance change should follow the measured reservation phase and
target transition matrix rather than assume the asset count itself locates the
host bottleneck.

## Results (8): FIFO draw methods

The latest run again reports no noticeable performance gain. Across 63
high-draw five-second windows, the median raster request count is 90,212 and
the executor is occupied for about 4.92 seconds. Accepted raw reservation
samples extrapolate to a median of approximately 0.92 seconds: about 0.31
seconds for target handling and 0.58 seconds for texture handling in its
sampled accepted path. The unsampled raster counters, which include other
paths, show about 1.19 seconds target time and 0.74 seconds texture time.
These figures overlap other reported times.

The prior `[GPU-FIFO-KIND] kind=draw_data` group extrapolates to a median of
2.90 seconds and about 3.9 million calls per window. Its classification was
too broad: it includes BEGIN_END, which submits a draw, and misses INLINE_ARRAY
(at 0x1818). Therefore that total does not prove index copying costs 2.9
seconds. The new report separates index16, index32, inline, draw_arrays and
begin_end so that the next run can attribute the time correctly.

The fork also takes a direct early path for the four payload methods
ARRAY_ELEMENT16, ARRAY_ELEMENT32, INLINE_ARRAY and DRAW_ARRAYS. It preserves
the original index limits, overflow counters, primitive gating and recorded
values; the original switch remains reachable with
`RECOMP_PB_FAST_ARRAYS=0` for an exact same-build comparison. This change is
an optimization experiment, not a proven FPS increase. The FIFO scan and
method executor both pass Linux GCC C11 syntax checks. Windows build, render
behavior and frame-rate effects require a user run.

`[GPU-TARGET-FLOW]` shows that most 640x480-to-128x128 and
256x256-to-640x480 changes are queued, with virtually no synchronous
`target-change` readback in the switch itself. Roughly 300 dependency drains
per window still force later publication. The size matrix does not identify
which game shader or texture consumes those targets; log binding identity only
if the dependency drain remains a primary cost after the FIFO experiment.

## Results (9): countdown baseline and X/Y crossing experiment

The player reports roughly the same race performance and a repeatable
~15 FPS during the starting countdown. Across 59 high-draw windows, the median
executor time is 4.91 seconds per five seconds, with about 91,800 batch
requests. The corrected FIFO method groups show approximately 2.88 seconds
at BEGIN_END (which performs draw submission), but only 82 ms for index16
payload methods. The preceding index shortcut therefore cannot solve this
frame-rate limit. Draws using CPU boundary fallback remain around 10,200 per
window, with roughly 0.93 seconds in CPU raster work. Reservation samples
attribute roughly 0.60 seconds to texture handling and 0.36 seconds to target
handling per window. Those are nested and/or sampled estimates.

The fork now admits an X/Y crossing triangle to GPU preparation when its
conservative whole-mesh bounds prove strictly positive W, finite bounded
screen coordinates and, where enabled, fully interior depth. The existing
hardware pixel pass clamps triangle coverage to the viewport. Full rejection
outside one plane remains ahead of this change; near-plane, uncertain depth,
nonfinite and extreme screen bounds still use the CPU clipper. It reports
`[GPU-XY-CROSSING] enabled=1 promoted=...`, which lets the next run measure
whether any meaningful fraction of the ~10,200 fallbacks is eligible. Set
`RECOMP_GPU_XY_CROSSING=0` for an exact same-build fallback if visual output
or frame pacing regresses. The conservative box/guard function passes a C11
fixture for wholly inside, X/Y crossing, disabled, near-W, near-Z, outside,
and extreme-bounds cases. The existing startup proof fixture now expects the
new X/Y behavior. Windows compilation, pixel-equivalence and FPS impact still
require a race run. The countdown is a useful before/after comparison point.

This is a bounded viewport crossing experiment. It does not implement general
GPU near-plane/depth clipping, AI instancing, or texture memory write tracking.

## Results (10): startup proof regression repaired

The user observed better ATV selection and a small rider selection gain, but
slightly worse race FPS. The decisive log is
`[GPU-GEOMETRY] unavailable or comparison failed`, followed by zero raw GPU
draws and zero X/Y promotions for the entire run. The previous run typically
had roughly 78,000 raw GPU draws and ~10,000 CPU boundary fallbacks per
high-draw five-second window. With raw geometry disabled, CPU raster time
rose from roughly 0.93 to 2.31 seconds per high-draw window. The menu gains
cannot be credited to the X/Y crossing path because it never ran.

The new bounding-box proof legitimately classifies the startup fixture's
first loose box as X/Y crossing even though all its actual vertices are
inside. Its old assertion still required the box to be uncertain, so startup
rejected the entire raw geometry path. The fork now expects the new box result
and retains the point and outside cases. A separate C11 fixture reproduced
the actual three startup inputs and checked results 1, 1 and 0 with edge
admission, plus 0 with the original mode. The startup path now retries the
full raw GPU comparison with X/Y admission disabled if its new proof fails.
That keeps the established raw GPU path available rather than silently
forcing all race vertex work onto the CPU. Windows startup and actual pixel
behavior remain to be verified in the next run. Confirm
`[GPU-GEOMETRY] enabled=1`, nonzero raw draws, and then examine
`[GPU-XY-CROSSING] promoted=` before interpreting the countdown FPS.

## Screen-edge experiment withdrawn after freeze

A run with the repaired proof hung after ATV selection and the game process
would not close normally. The subsequent race run reached gameplay, but the
menu gain disappeared and race start remained about 15 FPS. This latter log
shows raw geometry enabled and only around 424 X/Y promotions per five-second
race window, against roughly 8,700 remaining boundary fallbacks; CPU raster
time was still about 0.99 seconds per window. The edge route had little
leverage on the race bottleneck and introduced a hang risk.

The fork's geometry implementation and state are restored byte-for-byte to
the pre-edge commit `b020858c`, preserving the FIFO and reservation
profiling but removing the crossing policy, fixture change and startup retry.
The launcher now enforces its stated 360-second runtime limit using
`WaitForExit(360000)`, then kills the game process tree and copies the
redirected logs. For an immediate manual stop from another PowerShell window,
run `taskkill /F /T /IM qpr2_recomp.exe`. Windows build and runtime recovery
need confirmation on the user's machine. Stable 30 FPS remains open; the next
optimization should target per-draw submission or a properly validated GPU
clipper, not this small bounding-box exception.
