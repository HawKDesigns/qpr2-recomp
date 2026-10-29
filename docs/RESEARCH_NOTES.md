# Initial research notes

## Upstream baselines

- XboxRecomp: <https://github.com/sp00nznet/xboxrecomp>
  - pinned commit: `193e2995ffaec871bb3be43a43bd66aa5fccfeb1`
  - release: v0.13.1
- OpenTricky: <https://github.com/GiZcesi/OpenTricky>
  - inspected commit: `2db800a9eefef1e5d2689eba8ebe7bc884195347`
- xemu QPR2 entry: <https://xemu.app/titles/4143000a/>
- historical xemu depth-clear issue:
  <https://github.com/xemu-project/xemu/issues/322>

## Known public identity

- Xbox title code: `AC-010`
- title ID: `0x4143000A`
- xemu report's XBE-header SHA-256:
  `f9c34e1219940713cb0ea8572a457da59eb89aef3a32bac6e55466f2679698a6`

The xemu value hashes the XBE headers, not necessarily the full file. The local
pipeline records a full-file SHA-256 and does not treat those hashes as
interchangeable.

## Findings from the supplied XBE

- The build links XDK 5120 libraries.
- Executable code exists outside `.text`; the analysis covers `DSOUND`, `XMV`,
  `D3DX`, `XGRPH`, `D3D`, `XPP`, and `DOLBY` in addition to `.text`.
- Rendering path and use of direct NV2A register access.
- Audio path (DirectSound API layer versus direct APU access).
- Retail file layout, archive/container formats, and case sensitivity.
- Differences between the AKA Acclaim release and later physical reissues.

The build-specific addresses in [KNOWN_BUILD.md](KNOWN_BUILD.md) are known only
for that exact full-file SHA-256. They must not be reused for other releases.

