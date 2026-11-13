# QPR2 Launcher

A couch-friendly launcher for the ATV: Quad Power Racing 2 recomp. Pick up a controller, press **Play**.

## Use it

Double-click **`QPR2 Launcher.bat`** in the project root. It opens the launcher in an Edge or Chrome app window. The project builds monitor selection and controller options by default. Run `"QPR2 Launcher.bat" debug` from Command Prompt to see the launcher log in a console.

The launcher uses a fresh browser profile and disables DirectComposition for that window to avoid persistent fullscreen artifacts on multi-monitor Windows setups. Fullscreen entered with the launcher's **Toggle fullscreen** button exits when the window loses focus.

The launcher **wraps `rebuild-and-run.ps1`**; it does not replace it. A normal launch runs that script, so the build, shader learning, `run.json` manifests and results ZIPs all behave exactly as before.

### Controls

| | Controller | Keyboard |
|---|---|---|
| Move | D-pad / left stick | Arrow keys |
| Select / change value | A | Enter |
| Back | B | Esc |
| Previous / next page | LB / RB | PgUp / PgDn |
| Reset a setting | Y | Delete |
| Launch the game | Menu (Start) | P |
| Help | View | F1 |

Left/Right change a setting's value. The right stick scrolls long pages. Mouse works too. Button prompts follow the controller you last touched (Xbox or PlayStation).

### Music

The launcher plays the soundtrack from your local game files. It looks for
`TrackN.wav` under `game/Licensed`, `game/ATV/Licensed`, and then under
`Licensed/` inside `game/ATV.zip`, using the first match for each track number.
The launcher comes with track labels. It reads the audio from your game files.

Open **Launcher → Music** to choose a track, set the volume, turn shuffle on
or off, or change the Xbox ADPCM layout. Music pauses when the game starts
and resumes when it exits.

| Action | Controller | Keyboard |
|---|---|---|
| Pause or resume | X | M |
| Previous track | LT | B |
| Next track | RT | N |

To change the labels, edit `launcher/music/tracks.xml`. Each
`<song number="1"><title>Song title</title><artist>Artist</artist></song>`
entry labels the corresponding `Track1.wav`. Tracks without a label appear as
**Track N**.

## What it does

- **Setup check** - verifies `default.xbe` against the supported SHA-256, the ATV archive, runtime, translated code, shaders and build, with plain-language fixes. First launch shows progress with live log lines.
- **Run modes** - Play, Benchmark, Debug, plus an optional time limit.
- **Save protection** - backs up `hdd/` before each launch (only when something changed; backups live in `launcher/backups/`), keeps the newest N automatic backups, one-click manual backup and restore. Restoring takes a safety copy first.
- **Housekeeping** - shader-cache size and clear, run-log cleanup (held back while logs are still teaching the launcher your shaders), open-folder shortcuts, recent runs.
- **Return to launcher** - minimises while you play, comes back when you quit with a summary (clean exit, time limit, crash code, build failure with the real error lines).
- **Play time** tracking, controller tester, desktop shortcut, "skip launcher" (hold Shift to open it), adaptive accent colour, interface size, menu sounds, reduce motion.

## Settings are real or absent

Every game setting maps to something that exists and was checked against the code. A setting left at its default is **not exported**, so the runtime and `rebuild-and-run.ps1` keep control of their own defaults. Settings are saved to `launcher/launcher.settings.json`.

**Display** (the runtime reads these itself): window size (`RECOMP_WINDOW_SCALE`), start fullscreen (`RECOMP_FULLSCREEN`), the Direct3D 11 presenter (`RECOMP_D3D_PRESENT`), V-Sync (`RECOMP_PRESENT_VSYNC`) and picture filtering (`RECOMP_PRESENT_FILTER`). V-Sync and filtering only apply with the Direct3D 11 presenter, and the launcher dims them when it is off. **Monitor** is host code (`src/qpr2_display.c`): the runtime always opens on the primary monitor, so the host finds its window, moves it, and then triggers the runtime's own F11 handler.

**Performance** controls the game renderer independently of Display. Clean v1
uses native D3D11 rasterization with ROV and eligible GPU geometry. CPU raster
worker count controls the software path. The comparison switches cover draw
bounds and target readbacks. The native-vs-compute trial uses both runtime
flags and starts off. A saved v3 `gpu_autotune` value is discarded since it
did not enable the trial. The new trial control requires a deliberate opt-in.

**Debugger exports** come from the QPR2 title's full runner. Benchmark and
Debug use a full launch even if Quick is selected for Play. Update the title
source if Setup reports missing debugger exports. Open the completed run from
Saves & Data. Look for `memory-map.json` in Benchmark and Debug and for
`guest-symbols.json` in Debug. The same files are included in the results ZIP.
Debug enables guest PC tracking and extra diagnostics. Use Benchmark for
countdown or track-area performance comparisons.

**Controls**: vibration (`RECOMP_RUMBLE`, runtime). **Stick deadzones, response curves, trigger threshold, invert, swap sticks and button mapping are host code** (`src/qpr2_input_config.c`). The runtime's Windows pad path passes XInput straight through, so the host transforms the 18-byte input block the game has just published, inside `qpr2_recovered_001A7D43`, before the game's own analog threshold. Order: remap, trigger threshold, deadzone and curve per stick, inversion, swap. Mapping means "this game control takes its value from that button"; the source keeps its own job unless you change it too (the launcher offers the swap when you bind). Mappings apply to the keyboard as well as controllers.

**Not available** because nothing in the runtime or host can do it today: a frame-rate cap, widescreen or other aspect ratios (the picture is 640x480, letterboxed), and rebinding individual *keys* (the runtime's keyboard table is fixed; its `input_map` library supports bindings but exposes no setter). The Buttons page shows the fixed keyboard layout.

## Upgrading from the standalone launcher

For a source ZIP installation, extract the branch source over the existing project folder. Retain your `game/`, `hdd/`, and `launcher/` settings and backups. Use **Full** for the first launch so the integrated game build is compiled. Settings continue to live in `launcher/launcher.settings.json`.

For a Git checkout with the standalone launcher already applied, preserve your local source edits before merging the branch. Use the integrated sources for `CMakeLists.txt`, `src/main.c`, and `src/qpr2_callbacks.c`, merging any unrelated edits you want to keep. The old patch script is superseded by the build integration.

Run `python3 -m unittest discover -s tools/tests -v` for the native input and retail callback fixtures (GCC required).

### Adding a setting

Add one entry to `settings-schema.json`; the backend validates and exports it and the UI renders it. No other launcher code changes (the game still has to read the variable).

```json
{ "id": "my_flag", "scope": "game", "page": "performance", "group": "Experiments",
  "type": "toggle", "env": "RECOMP_MY_FLAG", "default": "0",
  "label": "My flag", "help": "What it does." }
```

`type` is `toggle` or `choice` (with `choices`). `scope` is `game` (exported as env), `run` (launch parameter) or `ui` (launcher only). A choice may carry an `env` map to set several variables at once (see `audio_latency`). `env_values` maps a selected value to a group of environment variables. `requires` names a toggle that must be on for the launcher to export the override.

## Artwork

On first run the launcher downloads the logo and menu background once into `launcher/assets/` (see `ArtUrls` in `Launcher.ps1`). To use your own, drop `logo.png` and `background.jpg` there. The folder is git-ignored; do not commit artwork you do not own. If a logo is very dark it gets a soft glow so it stays readable, and the accent colour is sampled from the background (change it under Launcher).

## How it works

`Launcher.ps1` serves `ui/` on `127.0.0.1` at a random port and opens it as an app window. Every request needs a per-run token (cookie), a matching `Host`, and for POSTs an `X-QPR2` header and same-origin `Origin`, so a web page in your browser cannot drive it. Only whitelisted settings, folders and backup names are accepted. Written for Windows PowerShell 5.1.

Developing the UI without Windows: serve `launcher/` with any static server and open `/ui/index.html?demo` (also `?demo=firstrun`, `?demo=broken`, `?demo=fail`).

## Known limits

- Tested end-to-end on a Linux PowerShell 7 test rig with a stand-in project; the Windows-only parts (Edge window control, `explorer`, `taskkill`, Shift detection, shortcuts, monitor detection) are written to the documented APIs but need a run on real Windows.
- Monitor selection uses the Windows framebuffer window. The Direct3D 11 presenter remains opt-in and falls back to GDI when initialization fails.
- Browsers list every HID device with buttons as a gamepad (headsets, dongles). The launcher scores each one and ignores non-controllers; the Controls page lists what it ignored.
- Quick launch skips the build, shader generation and shader learning. Use Full to keep those current.
- The launcher is idle while the game runs; it does not read the game's FPS.
