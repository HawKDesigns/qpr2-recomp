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
