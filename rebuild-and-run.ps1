$ErrorActionPreference = "Stop"

if (-not (Test-Path ".\CMakeLists.txt")) {
    throw "Run this script from the QPR2 recomp repository root."
}

New-Item -ItemType Directory -Force ".\logs" | Out-Null
# Retain each attempt, including a failed boot, across consecutive launches.
$runId = (Get-Date -Format "yyyyMMdd-HHmmss-fff") + "-" + [guid]::NewGuid().ToString("N").Substring(0, 8)
$runLogDir = Join-Path $PWD ("logs\v126-run-" + $runId)
New-Item -ItemType Directory -Force $runLogDir | Out-Null

# Isolate every object from the partial v30/v31 builds. Do not rely on ZIP
# timestamps to invalidate previously compiled MSBuild objects.
$diagnosticBuildDir = Join-Path $PWD "build-diagnostic-v126"

# V53 builds the supplied generation and bounded recovered callbacks.
# Do not run the V22 rollback or regenerate the full lift here.
Write-Host "Configuring QPR2 diagnostic overlay v126..."
& cmake -S . -B $diagnosticBuildDir 2>&1 | Tee-Object -FilePath "$runLogDir\diagnostic-v126-configure.log"
if ($LASTEXITCODE -ne 0) { throw "CMake configure failed: $LASTEXITCODE" }

Write-Host "Building QPR2 diagnostic overlay v126..."
& cmake --build $diagnosticBuildDir --config Release --target qpr2_recomp --parallel 2>&1 |
    Tee-Object -FilePath "$runLogDir\diagnostic-v126-build.log"
$buildExit = $LASTEXITCODE
$buildExit | Set-Content "$runLogDir\diagnostic-v126-build.exitcode.txt"
if ($buildExit -ne 0) { throw "Build failed: $buildExit. Send diagnostic-v126-build.log." }

# V108 removes the V107 live-depth experiment; clear an inherited override.
$env:RECOMP_GPU_LIVE_DEPTH = "0"
if (-not (Test-Path Env:RECOMP_GPU_BULK_TEXTURE)) { $env:RECOMP_GPU_BULK_TEXTURE = "1" }
$env:RECOMP_WATCHDOG_SECS = "360"
$env:RECOMP_UNIMPL_TRAP = "1"
$env:RECOMP_KERNEL_LOG_BUDGET = "0"
$env:RECOMP_TRACE_BUDGET = "0"

# Report the AC97 codec as ready and enable XboxRecomp's APU MMIO model.
# The seventeenth run proved this path; clear the verbose trace left by v11.
$env:RECOMP_AC97_READY = "1"
Remove-Item Env:RECOMP_APU_TRACE -ErrorAction SilentlyContinue

# Reclaim virtual-memory blocks allocated from the Xbox heap. Without this,
# QPR2's thousands of MEM_DECOMMIT calls fall through to the 64-bit host API,
# fail with error 87, and leak every temporary movie/asset allocation.
$env:RECOMP_HEAP_RECLAIM = "1"

# Start the two MCPX OHCI controllers and expose one virtual Xbox controller.
# The virtual controller reads Windows XInput controller 0. Keep every
# scripted-input source disabled. Controller and keyboard fallback both
# remain available to exercise the natural intro and menu transition.
$env:RECOMP_USB = "1"
$env:RECOMP_USB_HC = "0"
$env:RECOMP_USB_PORT = "0"
$env:RECOMP_USB_NDP = "2"
$env:RECOMP_USB_PADS = "1"
$env:RECOMP_VIRTUAL_PAD = "1"
$env:RECOMP_USB_STATS = "1"
Remove-Item Env:RECOMP_USB_TRACE -ErrorAction SilentlyContinue
$env:RECOMP_INPUT_DIAG = "1"
Remove-Item Env:RECOMP_PAD_PRESS -ErrorAction SilentlyContinue
Remove-Item Env:RECOMP_PAD_SCRIPT -ErrorAction SilentlyContinue
Remove-Item Env:RECOMP_PAD_LIVE -ErrorAction SilentlyContinue
$env:RECOMP_KEYBOARD = "1"
$env:RECOMP_AUDIO_DIAG = "1"
if ($env:RECOMP_AUDIO_DIAGNOSTICS -eq "1") {
    $env:RECOMP_VOICE_DIAG = "1"
} else {
    Remove-Item Env:RECOMP_VOICE_DIAG -ErrorAction SilentlyContinue
}
# Audio sample exports decode/copy and write files synchronously. Keep normal
# playback and timing diagnostics; captures require an explicit opt-in.
if ($env:RECOMP_AUDIO_DIAGNOSTICS -eq "1") {
    $env:RECOMP_VOICE_DUMP = "$runLogDir\one-hundred-twenty-sixth-voice-"
} else {
    Remove-Item Env:RECOMP_VOICE_DUMP -ErrorAction SilentlyContinue
}
# Full vertex dumps are synchronous and trigger on the first extreme 3D
# vertices; keep normal performance runs free of those first-use dumps.
if ($env:RECOMP_VERTEX_DIAGNOSTICS -eq "1") {
    $env:RECOMP_VSH_TRACE = "12"
} else {
    $env:RECOMP_VSH_TRACE = "0"
}
Remove-Item Env:RECOMP_VSH_DUMP -ErrorAction SilentlyContinue

# Execute the NV2A pushbuffer, show its framebuffer, and record just enough
# detail to identify the next missing method if the title stops again.
$env:RECOMP_PB_EXEC = "1"
# QPR2 D3D constructs its GPU KEVENT directly in guest memory.
$env:RECOMP_TITLE_KEVENTS = "1"
$env:RECOMP_FB_WINDOW = "1"
$env:RECOMP_PB_REPORT_MS = "5000"
$env:RECOMP_RENDER_TIMING = "1"
# Exact asset-program CPU specializations; honor an explicit interpreter A/B run.
if (-not (Test-Path Env:RECOMP_VERTEX_NATIVE)) { $env:RECOMP_VERTEX_NATIVE = "1" }
# Submit required current and saved readback copies before blocking.
if (-not (Test-Path Env:RECOMP_GPU_BATCH_READBACK)) { $env:RECOMP_GPU_BATCH_READBACK = "1" }
# Hardware pixel rendering; the working CPU path remains the fallback.
# Set RECOMP_GPU_RASTER=0 before launching for an explicit CPU comparison.
if (-not (Test-Path Env:RECOMP_GPU_RASTER)) { $env:RECOMP_GPU_RASTER = "1" }
# Finish batched guest semaphores with a four-byte GPU marker. Use the
# version-specific opt-out for an A/B run without rebuilding.
if ($env:RECOMP_GPU_COMPLETION_V126 -eq "0" -or $env:RECOMP_GPU_COMPLETION_V125 -eq "0" -or $env:RECOMP_GPU_COMPLETION_V124 -eq "0" -or $env:RECOMP_GPU_COMPLETION_V123 -eq "0") {
    $env:RECOMP_GPU_COMPLETION_TOKEN = "0"
} else {
    $env:RECOMP_GPU_COMPLETION_TOKEN = "1"
}
Write-Host "GPU completion token=$env:RECOMP_GPU_COMPLETION_TOKEN"
# Isolated same-build comparison of the exact GPU feedback snapshot path.
$env:RECOMP_GPU_FEEDBACK_SNAPSHOT = if ($env:RECOMP_GPU_FEEDBACK_V126 -eq "0" -or $env:RECOMP_GPU_FEEDBACK_V125 -eq "0" -or $env:RECOMP_GPU_FEEDBACK_V124 -eq "0") { "0" } else { "1" }
Write-Host "GPU feedback snapshot=$env:RECOMP_GPU_FEEDBACK_SNAPSHOT"
# Per-vertex interval proof for uncertain raw mesh boxes; isolated opt-out.
$env:RECOMP_GPU_POINT_BOUNDS = if ($env:RECOMP_GPU_POINT_V126 -eq "0" -or $env:RECOMP_GPU_POINT_V125 -eq "0") { "0" } else { "1" }
Write-Host "GPU point bounds=$env:RECOMP_GPU_POINT_BOUNDS"
# Compiled shaders live in shader-cache; changed compiler inputs get new keys.
# The CMake dependency regenerates embedded HLSL/shared includes automatically.
New-Item -ItemType Directory -Force ".\shader-cache" | Out-Null
# Cold shader preparation runs independently while the guest uses CPU rendering.
if (-not (Test-Path Env:RECOMP_GPU_ASYNC)) { $env:RECOMP_GPU_ASYNC = "1" }
# V115 defaults to the V113 source-first pixel shader. Preserve explicit A/B
# overrides; V114 accidentally deleted RECOMP_GPU_EARLY_DEPTH=0 before launch.
if (-not (Test-Path Env:RECOMP_GPU_EARLY_DEPTH)) { $env:RECOMP_GPU_EARLY_DEPTH = "0" }
if (-not (Test-Path Env:RECOMP_GPU_REPORT_ONLY)) { $env:RECOMP_GPU_REPORT_ONLY = "1" }
if (-not (Test-Path Env:RECOMP_GPU_STATE_INTERN)) { $env:RECOMP_GPU_STATE_INTERN = "1" }
Write-Host "GPU pixel mode: early depth=$env:RECOMP_GPU_EARLY_DEPTH; counter-only reports=$env:RECOMP_GPU_REPORT_ONLY"
Write-Host "GPU exact material-state reuse=$env:RECOMP_GPU_STATE_INTERN"
Remove-Item Env:RECOMP_GPU_PARTIAL_READBACK -ErrorAction SilentlyContinue
# Prefer the ordered graphics backend when supported; keep compute fallback.
$env:RECOMP_GPU_NATIVE = "1" # This build measures native graphics even after a compute A/B run.
# Separate small compute/native shaders handle exact supported one-stage materials.
if (-not (Test-Path Env:RECOMP_GPU_SIMPLE)) { $env:RECOMP_GPU_SIMPLE = "1" }
# Gate the dedicated native pixel shader independently; keep validated compute on failure.
if (-not (Test-Path Env:RECOMP_GPU_NATIVE_SIMPLE)) { $env:RECOMP_GPU_NATIVE_SIMPLE = "1" }
# Share uploads and use hardware triangle coverage after independent comparisons.
if (-not (Test-Path Env:RECOMP_GPU_BATCH_UPLOADS)) { $env:RECOMP_GPU_BATCH_UPLOADS = "1" }
if (-not (Test-Path Env:RECOMP_GPU_COVERAGE)) { $env:RECOMP_GPU_COVERAGE = "1" }
# Raw known-program vertices/lighting/preparation run on GPU only after
# private offscreen comparisons. Set RECOMP_GPU_GEOMETRY=0 for an A/B run.
if (-not (Test-Path Env:RECOMP_GPU_GEOMETRY)) { $env:RECOMP_GPU_GEOMETRY = "1" }
if (-not (Test-Path Env:RECOMP_GPU_MESH_CACHE_2Q)) { $env:RECOMP_GPU_MESH_CACHE_2Q = "1" }
if (-not (Test-Path Env:RECOMP_GPU_MESH_CACHE_BALANCE)) { $env:RECOMP_GPU_MESH_CACHE_BALANCE = "1" }
if (-not (Test-Path Env:RECOMP_GPU_SKIP_UNCHANGED)) { $env:RECOMP_GPU_SKIP_UNCHANGED = "1" }
if (-not (Test-Path Env:RECOMP_GPU_STREAM_UNION)) { $env:RECOMP_GPU_STREAM_UNION = "1" }
if (-not (Test-Path Env:RECOMP_GPU_GEOMETRY_PROFILE)) { $env:RECOMP_GPU_GEOMETRY_PROFILE = "1" }
if (-not (Test-Path Env:RECOMP_GPU_PROFILE_MATERIALS)) { $env:RECOMP_GPU_PROFILE_MATERIALS = "1" }
if (-not (Test-Path Env:RECOMP_GPU_PROFILE_SHADE)) { $env:RECOMP_GPU_PROFILE_SHADE = "1" }
$env:RECOMP_GPU_LOCAL_ROV = "0" # Local scope failed the exact v111 startup comparison.
if (-not (Test-Path Env:RECOMP_GPU_MESH_WORD_HASH)) { $env:RECOMP_GPU_MESH_WORD_HASH = "1" }
if (-not (Test-Path Env:RECOMP_GPU_MESH_CACHE_SLOTS)) { $env:RECOMP_GPU_MESH_CACHE_SLOTS = "32768" }
# Rotate raw constants and keep transform/preparation job inputs separate.
if (-not (Test-Path Env:RECOMP_GPU_RAW_UPLOAD_STREAM)) { $env:RECOMP_GPU_RAW_UPLOAD_STREAM = "1" }
# Merge nearby CPU-prepared upload runs before GPU raw-slot preparation.
if (-not (Test-Path Env:RECOMP_GPU_MIXED_UPLOAD_COALESCE)) { $env:RECOMP_GPU_MIXED_UPLOAD_COALESCE = "1" }
# Exact SSE2 color/depth readback deinterleave; retain scalar opt-out.
if (-not (Test-Path Env:RECOMP_GPU_READBACK_SIMD)) { $env:RECOMP_GPU_READBACK_SIMD = "1" }
if (-not (Test-Path Env:RECOMP_GPU_DEFER_READBACK)) { $env:RECOMP_GPU_DEFER_READBACK = "1" }
# Sparse nonblocking GPU timeline samples distinguish vertex/pixel/copy cost.
if (-not (Test-Path Env:RECOMP_GPU_PROFILE)) { $env:RECOMP_GPU_PROFILE = "1" }
# Spread samples across batch positions and classify prepared/raw/copy scopes.
if (-not (Test-Path Env:RECOMP_GPU_PROFILE_SPREAD)) { $env:RECOMP_GPU_PROFILE_SPREAD = "1" }
# Queue independent native target copies after their hardware comparison.
# Guest/CPU dependencies still complete all required target output.
if (-not (Test-Path Env:RECOMP_GPU_READBACK_QUEUE)) { $env:RECOMP_GPU_READBACK_QUEUE = "1" }
# GPU-only handoff for exact ARGB render-target textures; startup comparison gated.
if (-not (Test-Path Env:RECOMP_GPU_TEXTURE_BRIDGE)) { $env:RECOMP_GPU_TEXTURE_BRIDGE = "1" }
# Resume only exact pending targets whose host pixels/depth are unchanged.
if (-not (Test-Path Env:RECOMP_GPU_TARGET_RESUME)) { $env:RECOMP_GPU_TARGET_RESUME = "1" }
# Later exact GPU versions inherit dirty pixels; retain older counters only.
if (-not (Test-Path Env:RECOMP_GPU_RETIRE_RESUMED_PIXELS)) { $env:RECOMP_GPU_RETIRE_RESUMED_PIXELS = "1" }
# Reuse completed surface-buffer allocations and validate UAV-only graphics.
if (-not (Test-Path Env:RECOMP_GPU_TARGET_POOL)) { $env:RECOMP_GPU_TARGET_POOL = "1" }
if (-not (Test-Path Env:RECOMP_GPU_UAV_ONLY)) { $env:RECOMP_GPU_UAV_ONLY = "1" }
# Named combiner registers reduce general shader indexing; failed checks retry arrays.
if (-not (Test-Path Env:RECOMP_GPU_FIXED)) { $env:RECOMP_GPU_FIXED = "1" }
# Exact grouped RGB combiner register writes, independently hardware validated.
if (-not (Test-Path Env:RECOMP_GPU_VECTOR_COMBINERS)) { $env:RECOMP_GPU_VECTOR_COMBINERS = "1" }
if (-not (Test-Path Env:RECOMP_GPU_QUAD_SAMPLING)) { $env:RECOMP_GPU_QUAD_SAMPLING = "1" }
if (-not (Test-Path Env:RECOMP_GPU_DEPTH_SEED)) { $env:RECOMP_GPU_DEPTH_SEED = "1" }
if (-not (Test-Path Env:RECOMP_GPU_SEED_REUSE)) { $env:RECOMP_GPU_SEED_REUSE = "1" }
Write-Host "GPU exact depth snapshot reuse=$env:RECOMP_GPU_SEED_REUSE"
if (-not (Test-Path Env:RECOMP_GPU_MICRO_FUSE)) { $env:RECOMP_GPU_MICRO_FUSE = "1" }
Write-Host "GPU short material-run fusion=$env:RECOMP_GPU_MICRO_FUSE"
if (-not (Test-Path Env:RECOMP_GPU_RAW_FAST_RESERVE)) { $env:RECOMP_GPU_RAW_FAST_RESERVE = "1" }
Write-Host "GPU raw ordered slot reservation=$env:RECOMP_GPU_RAW_FAST_RESERVE"
# Ignore a 16 ms value inherited from the prior v121 script in this shell.
# A deliberate comparison can set the version-specific override before launch.
if (Test-Path Env:RECOMP_GPU_FENCE_CARRY_V126) {
    $env:RECOMP_GPU_FENCE_CARRY_MS = $env:RECOMP_GPU_FENCE_CARRY_V126
} elseif (Test-Path Env:RECOMP_GPU_FENCE_CARRY_V125) {
    $env:RECOMP_GPU_FENCE_CARRY_MS = $env:RECOMP_GPU_FENCE_CARRY_V125
} elseif (Test-Path Env:RECOMP_GPU_FENCE_CARRY_V124) {
    $env:RECOMP_GPU_FENCE_CARRY_MS = $env:RECOMP_GPU_FENCE_CARRY_V124
} else {
    $env:RECOMP_GPU_FENCE_CARRY_MS = "8"
}
Write-Host "GPU fence carry budget (ms)=$env:RECOMP_GPU_FENCE_CARRY_MS"
if (-not (Test-Path Env:RECOMP_GPU_EMPTY_REJECT)) { $env:RECOMP_GPU_EMPTY_REJECT = "1" }
if (-not (Test-Path Env:RECOMP_GPU_STAGE_LIVENESS)) { $env:RECOMP_GPU_STAGE_LIVENESS = "1" }
if (-not (Test-Path Env:RECOMP_GPU_LITERAL_MATERIALS)) { $env:RECOMP_GPU_LITERAL_MATERIALS = "1" }
if (-not (Test-Path Env:RECOMP_GPU_COMPACT_MATERIALS)) { $env:RECOMP_GPU_COMPACT_MATERIALS = "1" }
# Pixel/depth-failure/brightness atomics are logging only. Keep guest visibility
# counters and full output checks, with diagnostics opt-in for comparisons.
if ($env:RECOMP_RENDER_DIAGNOSTICS -eq "1") {
    $env:RECOMP_GPU_DIAGNOSTIC_COUNTERS = "1"
} elseif (-not (Test-Path Env:RECOMP_GPU_DIAGNOSTIC_COUNTERS)) {
    $env:RECOMP_GPU_DIAGNOSTIC_COUNTERS = "0"
}
# Reset visibility counters on GPU after an independent hardware comparison.
if (-not (Test-Path Env:RECOMP_GPU_REPORT_RESET)) { $env:RECOMP_GPU_REPORT_RESET = "1" }
# Keep color/depth clears on the active GPU target after an output comparison.
if (-not (Test-Path Env:RECOMP_GPU_CLEAR)) { $env:RECOMP_GPU_CLEAR = "1" }
# Skip CPU-clear readback only after proving both output mirrors disjoint.
if (-not (Test-Path Env:RECOMP_GPU_DISJOINT_CLEAR)) { $env:RECOMP_GPU_DISJOINT_CLEAR = "1" }
# Batch completion waits with V62's completed-fence notification.
# Dependent memory accesses, reports and idle streams still drain.
# Reset an inherited V62 default; opt out with RECOMP_DISABLE_FENCE_BATCH=1.
if ($env:RECOMP_DISABLE_FENCE_BATCH -eq "1") {
    $env:RECOMP_GPU_FENCE_BATCH = "0"
} else {
    $env:RECOMP_GPU_FENCE_BATCH = "1"
}
# Registered software events share ordered GPU completion groups.
if (-not (Test-Path Env:RECOMP_GPU_EVENT_BATCH)) { $env:RECOMP_GPU_EVENT_BATCH = "1" }
# GET keeps its 4096-word budget; completion groups span up to 16 slices/8 ms.
if (-not (Test-Path Env:RECOMP_GPU_SLICE_PIPELINE)) { $env:RECOMP_GPU_SLICE_PIPELINE = "1" }
# V71's menu-batch choice was slower in races. Clear its inherited default;
# re-enable only through the explicit experiment setting.
if ($env:RECOMP_GPU_AUTOTUNE_EXPERIMENT -eq "1") {
    $env:RECOMP_GPU_AUTOTUNE = "1"
} else {
    $env:RECOMP_GPU_AUTOTUNE = "0"
}
# Omit statistics traffic only when all draws prove those counters unused.
if (-not (Test-Path Env:RECOMP_GPU_EMPTY_STATS)) { $env:RECOMP_GPU_EMPTY_STATS = "1" }
# Conservative active-camera draw rejection, bounded input-buffer rotation,
# and byte-exact reuse of resident translated geometry/material payloads.
if (-not (Test-Path Env:RECOMP_DRAW_FRUSTUM)) { $env:RECOMP_DRAW_FRUSTUM = "1" }
if (-not (Test-Path Env:RECOMP_GPU_UPLOAD_STREAM)) { $env:RECOMP_GPU_UPLOAD_STREAM = "1" }
if (-not (Test-Path Env:RECOMP_GPU_UPLOAD_CACHE)) { $env:RECOMP_GPU_UPLOAD_CACHE = "1" }
# Reuse repeated indices within one draw; context-writing programs bypass it.
if (-not (Test-Path Env:RECOMP_VERTEX_CACHE)) { $env:RECOMP_VERTEX_CACHE = "1" }
$env:RECOMP_GPU_CAPTURE = "$runLogDir\one-hundred-twenty-sixth-gpu-capture.bin"
# Buffer stderr before startup; set RECOMP_LOG_UNBUFFERED=1 only for crash tracing.
# Keep aggregate timings; frequent per-call output must be explicitly requested.
$env:RECOMP_GAME_TIMING = "1"
if ($env:RECOMP_RENDER_DIAGNOSTICS -eq "1") {
    $env:RECOMP_GAME_TIMING_VERBOSE = "1"
    $env:RECOMP_PB_PROGRESS = "1"
} else {
    Remove-Item Env:RECOMP_GAME_TIMING_VERBOSE -ErrorAction SilentlyContinue
    Remove-Item Env:RECOMP_PB_PROGRESS -ErrorAction SilentlyContinue
}
$env:RECOMP_MESH_TRACE = "1"
$env:RECOMP_MAIN_SAMPLES = "1"
$env:RECOMP_KERNEL_SLOW_MS = "100"
Remove-Item Env:RECOMP_PB_EXEC_VERBOSE -ErrorAction SilentlyContinue
Remove-Item Env:RECOMP_NV2A_TRACE -ErrorAction SilentlyContinue
Remove-Item Env:RECOMP_FENCE_TRACE -ErrorAction SilentlyContinue

# QPR2's own XMV decoder remains in control.  Its D3D swap/vblank counter is
# supplied by xbox_Nv2aFrameCounter; no conversion, sidecar, or host player is
# involved.  Clear inherited host-video settings from older diagnostics.
Remove-Item Env:RECOMP_FMV_HOST -ErrorAction SilentlyContinue
Remove-Item Env:RECOMP_FMV_DUMP -ErrorAction SilentlyContinue
# Automatic screenshots write synchronously on the FIFO thread. Keep ordinary
# timing logs and F8 captures, but opt in to periodic images when needed.
if ($env:RECOMP_FRAME_DIAGNOSTICS -eq "1") {
    $env:RECOMP_FB_DUMP = "$runLogDir\one-hundred-twenty-sixth-frame-"
} else {
    Remove-Item Env:RECOMP_FB_DUMP -ErrorAction SilentlyContinue
    Remove-Item Env:RECOMP_FB_DUMP_FLIPS -ErrorAction SilentlyContinue
}
# First-use texture BMPs are synchronous and can stall entry into a new menu.
# F8 still captures rendering inputs/output. Opt in only when texture images
# are specifically needed; clear a value inherited from an older launcher.
if ($env:RECOMP_TEXTURE_DIAGNOSTICS -eq "1") {
    $env:RECOMP_TEX_DUMP = "$runLogDir\one-hundred-twenty-sixth-texture-"
} else {
    Remove-Item Env:RECOMP_TEX_DUMP -ErrorAction SilentlyContinue
}
$env:RECOMP_TEX_DUMP_EVERY = "0"
# Bounded three-snapshot method survey to trace mounted-racer shadow state.
$env:RECOMP_SHADOW_METHODS = "1"
# Full texture/method tables are diagnostic disk writes, not frame timings.
if ($env:RECOMP_RENDER_DIAGNOSTICS -eq "1") {
    $env:RECOMP_TEX_STATE = "1"
    $env:RECOMP_RENDER_STATE = "1"
} else {
    Remove-Item Env:RECOMP_TEX_STATE -ErrorAction SilentlyContinue
    Remove-Item Env:RECOMP_RENDER_STATE -ErrorAction SilentlyContinue
}

$tempLogDir = Join-Path $env:TEMP ("qpr2-recomp-v126-" + [guid]::NewGuid())
New-Item -ItemType Directory -Force $tempLogDir | Out-Null
$tempStdout = Join-Path $tempLogDir "one-hundred-twenty-sixth-run.stdout.log"
$tempStderr = Join-Path $tempLogDir "one-hundred-twenty-sixth-run.stderr.log"

Write-Host "Starting QPR2 from the isolated v126 build with dispatcher and renderer verification..."
Write-Host "The diagnostic will run for up to 360 seconds."
Write-Host "Use your controller; keyboard fallback: Enter = Start, Z = A, arrows = D-pad (focus the game window)."
Write-Host "Live logs are on the local temp volume: $tempLogDir"
Write-Host "If a visible artifact returns, press F8 once to capture two rendering frames."
$process = Start-Process `
    -FilePath (Join-Path $diagnosticBuildDir "Release\qpr2_recomp.exe") `
    -WorkingDirectory "$PWD" `
    -Wait -PassThru `
    -RedirectStandardOutput $tempStdout `
    -RedirectStandardError $tempStderr

Copy-Item -Force $tempStdout "$runLogDir\one-hundred-twenty-sixth-run.stdout.log"
Copy-Item -Force $tempStderr "$runLogDir\one-hundred-twenty-sixth-run.stderr.log"
$process.ExitCode | Set-Content "$runLogDir\one-hundred-twenty-sixth-run.exitcode.txt"
Write-Host "Run finished with exit code $($process.ExitCode)."
Write-Host "Kept this run in $runLogDir (earlier attempts remain in their own folders)."


if (Test-Path ".\xbox_kernel.log") {
    Copy-Item -Force ".\xbox_kernel.log" "$runLogDir\one-hundred-twenty-sixth-kernel.log"
}
$results = @(Get-ChildItem "$runLogDir\diagnostic-v126-*", "$runLogDir\one-hundred-twenty-sixth-*" -File)
if ($results.Count -gt 0) {
    Compress-Archive -LiteralPath $results.FullName -DestinationPath ".\logs\qpr2-v126-results.zip" -Force
    Write-Host "Results archived: $PWD\logs\qpr2-v126-results.zip"
}
