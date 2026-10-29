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
