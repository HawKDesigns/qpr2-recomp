# Known retail build

This file records the reproducible, non-secret results of running
`python tools/qpr2_pipeline.py all` against a legally dumped North American
retail executable. The XBE and generated translations are not distributed.

## Identity

| Field | Value |
| --- | --- |
| Certificate title | ATV: Quad Power Racing 2 |
| Title ID | `0x4143000A` (`AC-010`) |
| Full-file SHA-256 | `5e9788296601bae4401e31ae1f5e25a8a732cb0bd0588576182d3aa69f859383` |
| File size | 1,904,640 bytes |
| Region | North America |
| Image base | `0x00010000` |
| Entry point | `0x000CA76D` |
| Kernel thunk | `0x001ADDA0` |
| XDK version | 5120 |
| Sections | 13 |
| Kernel imports | 113 |
| Initial XboxRecomp analysis base | `193e2995ffaec871bb3be43a43bd66aa5fccfeb1` (v0.13.1) |
| Current QPR2 runtime fork | `e74078bb6a76441a611e3cdcfbe98df9325745db` (`qpr2-v126`) |

## Analysis result

| Stage | Result |
| --- | --- |
| Disassembly | 589,648 instructions; 8,792 functions; 102,271 cross-references |
| Reachability | 427,110 instructions (72.3%) |
| Function classification | 5,609 game-classified; 4,175 unknown; 11 CRT |
| Vtable recovery | 576 vtables; 11,552 virtual methods; 526 constructors |
| ABI recovery | 8,792 analyzed; 2,126 detected `thiscall` functions |
| Static lift | 8,792 translated; 0 failed; 36 generated chunks |
| Follow-up queue | 76 unresolved call targets; 483 unimplemented-opcode occurrences |

The unusual opcode results are candidates for code/data-boundary cleanup, not
proof that the game executes those instructions. A large cluster falls in the
gap immediately before the `D3D` section, so runtime traces should determine
which occurrences are reachable before manual implementations are added.

## Code-bearing sections

| Section | Virtual address | Analyzed bytes | Functions |
| --- | ---: | ---: | ---: |
| `.text` | `0x00011000` | 1,229,824 | 7,311 |
| `DSOUND` | `0x0013D400` | 52,292 | 472 |
| `XMV` | `0x0014A060` | 162,564 | 63 |
| `D3DX` | `0x00171B80` | 116,888 | 420 |
| `XGRPH` | `0x0018E420` | 19,536 | 96 |
| `D3D` | `0x00193080` | 79,004 | 206 |
| `XPP` | `0x001A6520` | 30,844 | 223 |
| `DOLBY` | `0x001F60A0` | 29,056 | 1 |

## Reproduce locally

Place the matching `default.xbe` at `game/default.xbe`, install Capstone, and
run:

```text
python tools/qpr2_pipeline.py all
```

Compare `analysis/qpr2_manifest.json` to the identity above before using any
address. Analysis reports and generated C remain ignored because they are
derived from the retail executable.
