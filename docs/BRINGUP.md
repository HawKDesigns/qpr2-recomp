# QPR2 bring-up plan

The first useful target is not “the whole game works.” It is a sequence of
small, measurable gates. Every address recorded during this work must be paired
with the XBE SHA-256 from `analysis/qpr2_manifest.json`.

## Gate 0 — binary inventory

Status for the documented retail build: **complete**. See
[KNOWN_BUILD.md](KNOWN_BUILD.md).

Run `tools/qpr2_pipeline.py analyze` and retain:

- full XBE SHA-256;
- title ID and certificate title;
- entry point and base address;
- XDK version and linked-library table;
- section names, executable flags, and sizes;
- kernel import list.

The known retail title ID is `0x4143000A` (`AC-010`). An alternate ID is a hard
stop unless the disc/build is independently verified. `--allow-unknown-title`
exists for intentional prototype or regional research, not convenience.

## Gate 1 — complete lift

Status for the documented retail build: **lift complete; generated-code compile
pending on Windows**.

Success criteria:

- disassembly completes across all code-bearing sections;
- function classification and ABI analysis complete;
- generated C contains an `xbe_entry_point` and dispatch table;
- unimplemented instruction summary is reviewed before building;
- the generated files compile with the pinned XboxRecomp revision.

Do not begin game-specific patches until the pipeline is deterministic on the
same XBE hash.

## Gate 2 — entry point to first hardware wait

Run with `RECOMP_WATCHDOG_SECS=10` and capture stderr. Classify the first stop
as one of:

1. missing kernel import;
2. unresolved indirect call;
3. untranslated x86 instruction;
4. invalid guest/native pointer conversion;
5. GPU or APU MMIO access;
6. title-specific file/path assumption;
7. legitimate hardware polling loop.

Add a manual override only after identifying the original function and its
calling convention. Each override should document the XBE hash, guest VA,
observed failure, expected Xbox behavior, and removal condition.

## Gate 3 — logo and front end

QPR2 previously exposed an NV2A `Z24S8` floating-depth clear path in xemu. The
current emulator reports the title playable, but that history makes depth/
stencil clear behavior an early graphics checkpoint for XboxRecomp's D3D11
translator.

Success criteria:

- stable window/device creation;
- first visible frame;
- correct color, depth, and stencil clears;
- movies can be skipped or played without blocking boot;
- controller input reaches the profile/menu flow;
- title-data/save directories map under local `hdd/`.

## Gate 4 — load one race

Trace the title's file opens and produce an asset inventory before writing any
format-specific loader. Confirm case behavior and Xbox path separators on the
host. Then validate:

- course geometry and terrain materials;
- rider and quad meshes;
- fixed-function render states and texture-stage combiners;
- physics tick stability;
- input dead zones and analog trigger mapping;
- race timer and opponent update cadence.

## Gate 5 — representative gameplay

Use one repeatable race as the test scene. Compare against real hardware or a
known-good xemu run for start position, lap timing, vehicle acceleration,
camera, effects, audio, and finish flow. Only after this gate should widescreen,
frame-rate, or quality-of-life enhancements be considered.

## OpenTricky lessons applied here

- Keep toolkit/runtime code separate from title-specific host glue.
- Preserve a fast manual-override path for verified problem functions.
- Log indirect-call history and guest registers at the actual failure.
- Keep extracted disc files and writable save data in separate roots.
- Derive addresses from each XBE instead of inheriting another project's
  constants.
- Treat “boots” and “renders correctly” as different milestones.

