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

# Windows' py launcher can terminate with 0xC0000022 before a Python script
# starts. Resolve the interpreter and run python.exe directly when possible.
$script:Qpr2PythonChoice = $null
function Get-Qpr2PythonChoices {
    $choices = @()
    $pyCommand = Get-Command py.exe -ErrorAction SilentlyContinue
    if ($pyCommand) {
        try {
            $reported = & ($pyCommand.Source) -3 -c "import sys; print(sys.executable)" 2>$null
            if ($LASTEXITCODE -eq 0 -and $reported) {
                $path = [string]($reported | Select-Object -Last 1)
                if (Test-Path -LiteralPath $path -PathType Leaf) {
                    $choices += @{ Name = "py-selected python.exe"; Path = $path; Prefix = @() }
                }
            }
        } catch {
            Write-Host "[PYTHON] py -3 interpreter discovery failed: $($_.Exception.Message)"
        }
    }
    foreach ($name in @("python.exe", "python3.exe")) {
        foreach ($command in @(Get-Command $name -All -ErrorAction SilentlyContinue)) {
            if ($command -and $command.Source) {
                $choices += @{ Name = $name; Path = $command.Source; Prefix = @() }
            }
        }
    }
    if ($env:LOCALAPPDATA) {
        $pythonRoot = Join-Path $env:LOCALAPPDATA "Programs\Python"
        foreach ($file in @(Get-ChildItem -Path $pythonRoot -Filter python.exe -Recurse -File -ErrorAction SilentlyContinue)) {
            $choices += @{ Name = "installed python.exe"; Path = $file.FullName; Prefix = @() }
        }
    }
    if ($pyCommand) {
        $choices += @{ Name = "py -3"; Path = $pyCommand.Source; Prefix = @("-3") }
    }
    $seen = @{}
    foreach ($choice in $choices) {
        $key = ($choice.Path + " " + ($choice.Prefix -join " ")).ToLowerInvariant()
        if (-not $seen.ContainsKey($key)) {
            $seen[$key] = $true
            $choice
        }
    }
}
function Invoke-Qpr2Python {
    param([string[]]$Arguments, [string]$LogFile)
    $choices = if ($script:Qpr2PythonChoice) { @($script:Qpr2PythonChoice) } else { @(Get-Qpr2PythonChoices) }
    if ($choices.Count -eq 0) { throw "Python 3.10+ was not found. Check your Python installation and PATH." }
    $lastFailure = 0
    foreach ($choice in $choices) {
        $commandArgs = @($choice.Prefix) + $Arguments
        Write-Host "[PYTHON] $($choice.Name): $($choice.Path) $($Arguments[0])"
        try {
            $probeArgs = @($choice.Prefix) + @("-c", "import sys, zipfile, mmap; sys.exit(0 if sys.version_info >= (3, 10) else 3)")
            & ($choice.Path) @probeArgs 2>$null | Out-Null
            $probeExit = $LASTEXITCODE
        } catch {
            $probeExit = -1073741790
        }
        if ($probeExit -ne 0) {
            Write-Host "[PYTHON] Interpreter probe failed ($probeExit); trying the next installation."
            $lastFailure = $probeExit
            continue
        }
        $savedErrorAction = $ErrorActionPreference
        try {
            # Native stderr should remain visible, not become a terminating
            # PowerShell NativeCommandError before we inspect the exit code.
            $ErrorActionPreference = "Continue"
            if ($LogFile) {
                & ($choice.Path) @commandArgs | Tee-Object -FilePath $LogFile | Out-Host
            } else {
                & ($choice.Path) @commandArgs | Out-Host
            }
            $lastFailure = $LASTEXITCODE
        } catch {
            Write-Host "[PYTHON] Launch failed: $($_.Exception.Message)"
            $lastFailure = -1073741790
        } finally {
            $ErrorActionPreference = $savedErrorAction
        }
        if ($lastFailure -eq 0) {
            $script:Qpr2PythonChoice = $choice
            return 0
        }
        # Do not retry semantic Python errors with another interpreter.
        if ($lastFailure -ne -1073741790) { return $lastFailure }
        Write-Host "[PYTHON] Interpreter terminated with 0xC0000022 before completing; trying the next installation."
    }
    return $lastFailure
}

# Rebuild locally derived specializations before compiling. The first ordinary
# menu/preview/race run teaches the launcher the reviewed material states; no
# F8 capture or manual extractor command is required.
$nativeHeader = "third_party/xboxrecomp/src/kernel/nv2a_vsh_compiled.h"
$literalHeader = "third_party/xboxrecomp/src/kernel/nv2a_gpu_literal_programs.h"
$literalBody = "third_party/xboxrecomp/src/kernel/nv2a_gpu_literal.inc"
$materialFixture = "tools/fixtures/qpr2-literal-programs.json"

function Update-Qpr2MaterialFixture {
    if (Test-Path -LiteralPath $materialFixture -PathType Leaf) { return }
    $runLogs = @(Get-ChildItem -Path ".\logs\v126-run-*\one-hundred-twenty-sixth-run.stderr.log" -File -ErrorAction SilentlyContinue |
        Sort-Object FullName)
    if ($runLogs.Count -eq 0) { return }
    $logPaths = @($runLogs | ForEach-Object { $_.FullName })
    Write-Host "[SHADER-LEARN] Checking $($runLogs.Count) local run log(s) for reviewed material states..."
    $learnExit = Invoke-Qpr2Python -Arguments (@("tools/extract_qpr2_literal_programs.py", "--quiet-incomplete") + $logPaths)
    if ($learnExit -eq 0) {
        Write-Host "[SHADER-LEARN] Material fixture ready; the next build generates fast literal shaders."
    } else {
        Write-Host "[SHADER-LEARN] Still collecting material states. Play through a 3D menu, preview, and race normally."
    }
}
# Recover a fixture from existing runs when upgrading a checkout.
Update-Qpr2MaterialFixture
# Windows tools may rewrite LF as CRLF. Compare the actual shader text after
# normalizing line endings so a valid local file is not regenerated needlessly.
function Get-Qpr2ShaderTextHash([string]$shaderPath, [switch]$IgnoreArchiveMetadata) {
    $shaderText = [System.IO.File]::ReadAllText((Resolve-Path -LiteralPath $shaderPath).Path).Replace("`r`n", "`n")
    if ($IgnoreArchiveMetadata) {
        # ZIP timestamps/compression change the archive hash, not the shaders.
        $shaderText = [regex]::Replace($shaderText, "Archive SHA256: [0-9a-fA-F]{64}", "Archive SHA256: [archive]")
    }
    $shaderBytes = [System.Text.Encoding]::UTF8.GetBytes($shaderText)
    $hasher = [System.Security.Cryptography.SHA256]::Create()
    try {
        return ([System.BitConverter]::ToString($hasher.ComputeHash($shaderBytes))).Replace("-", "").ToLowerInvariant()
    } finally {
        $hasher.Dispose()
    }
}
if (-not (Test-Path $nativeHeader -PathType Leaf)) {
    # The extracted disc supplies ATV.BAD/ATV.BAH. The vendored MIT extractor
    # expands all files into game/ATV/, then a deterministic ZIP places their
    # contents (not an outer ATV folder) at the archive root.
    Write-Host "[ATV-PREP] Preparing game/ATV.zip from the local archive or extracted folder"
    $prepExit = Invoke-Qpr2Python -Arguments @("tools/prepare_qpr2_atv_archive.py")
    if ($prepExit -ne 0) {
        if ($prepExit -eq -1073741790) {
            throw "Python stopped with Windows 0xC0000022 before ATV archive preparation could inspect the game files. See the [PYTHON] interpreter attempts above; check Python execution permissions."
        }
        throw "ATV archive preparation exited $prepExit. Check the [ATV-PREP] error above and your local ATV.BAD/ATV.BAH or game/ATV/ folder."
    }
    Write-Host "[SHADER-BUILD] Compiling native vertex variants from game/ATV.zip"
    $nativeExit = Invoke-Qpr2Python -Arguments @("tools/compile_qpr2_vertex_programs.py", ".\\game\\ATV.zip")
    if ($nativeExit -ne 0) { throw "Native vertex generation failed: $nativeExit" }
}
# Generate the reviewed materials when absent. Preserve existing local variants
# for diagnosis instead of silently replacing a user's custom files.
$literalHeaderMissing = -not (Test-Path $literalHeader -PathType Leaf)
$literalBodyMissing = -not (Test-Path $literalBody -PathType Leaf)
$literalBodyPlaceholder = (-not $literalBodyMissing) -and ((Get-Item -LiteralPath $literalBody).Length -lt 256)
if ((Test-Path $materialFixture -PathType Leaf) -and
    ($literalHeaderMissing -or $literalBodyMissing -or $literalBodyPlaceholder)) {
    Write-Host "[SHADER-BUILD] classifier missing=$literalHeaderMissing body missing=$literalBodyMissing body placeholder=$literalBodyPlaceholder"
    Write-Host "[SHADER-BUILD] Generating literal material programs from locally learned fixture"
    $generationOut = Join-Path $runLogDir "shader-generation.stdout.log"
    $generationExit = Invoke-Qpr2Python -Arguments @("-u", "tools/generate_qpr2_literal_materials.py") -LogFile $generationOut
    $generationExit | Set-Content (Join-Path $runLogDir "shader-generation.exitcode.txt")
    if ($generationExit -ne 0) {
        $classifierReady = (Test-Path $literalHeader -PathType Leaf) -and
            ((Get-Qpr2ShaderTextHash $literalHeader) -eq "169bc9b0d0d421273d8705d924ce486afa310cba0edee5868d532bd1ef87e178")
        $bodyReady = (Test-Path $literalBody -PathType Leaf) -and
            ((Get-Qpr2ShaderTextHash $literalBody) -eq "e15bc49d65080a033479c3d065400543fa7e3970f0eb7942353abfd051e71f7c")
        if ($classifierReady -and $bodyReady) {
            Write-Warning "Python exited with $generationExit, but both generated files match the v126 hashes; continuing."
        } else {
            throw "Literal material generation exited $generationExit. See $generationOut and verify Python, your local fixture and the pinned submodule."
        }
    }
}

# A clean checkout still runs with generic shader fallbacks, but its race FPS
# is not comparable to the original v126 diagnostic until the local programs
# are generated. Check exact v126 fingerprints before starting the build.
$shaderSpecs = @(
    @{ Name = "native vertex"; Path = "third_party/xboxrecomp/src/kernel/nv2a_vsh_compiled.h"; Hash = "6347a41fa83a2dfae848e6398573eb5b46b37334b363c068c7b46119b7ff357d" },
    @{ Name = "literal classifier"; Path = "third_party/xboxrecomp/src/kernel/nv2a_gpu_literal_programs.h"; Hash = "169bc9b0d0d421273d8705d924ce486afa310cba0edee5868d532bd1ef87e178" },
    @{ Name = "literal GPU bodies"; Path = "third_party/xboxrecomp/src/kernel/nv2a_gpu_literal.inc"; Hash = "e15bc49d65080a033479c3d065400543fa7e3970f0eb7942353abfd051e71f7c" }
)
$shaderStatus = @()
foreach ($spec in $shaderSpecs) {
    $file = Join-Path $PWD $spec.Path
    $state = "missing"
    if (Test-Path -LiteralPath $file -PathType Leaf) {
        $actual = (Get-FileHash -LiteralPath $file -Algorithm SHA256).Hash.ToLowerInvariant()
        $normalized = Get-Qpr2ShaderTextHash $file
        $size = (Get-Item -LiteralPath $file).Length
        if ($actual -eq $spec.Hash) { $state = "ready" }
        elseif ($normalized -eq $spec.Hash) { $state = "ready (line endings normalized)" }
        elseif ($spec.Name -eq "native vertex" -and
                (Get-Qpr2ShaderTextHash $file -IgnoreArchiveMetadata) -eq "ac56f05ac9b1b7f3b0d65b39d331af7dd435542e26b30bf11016b0433eb20042") {
            $state = "ready (equivalent vertex programs; ZIP metadata differs)"
        }
        else { $state = "different hash; path=$file bytes=$size sha256=$actual normalized_sha256=$normalized" }
    }
    $shaderStatus += "$($spec.Name): $state"
}
$shaderStatus | Set-Content (Join-Path $runLogDir "shader-status.txt")
$shaderStatus | ForEach-Object { Write-Host "[SHADER-STATUS] $_" }
if ($shaderStatus -match "missing|different hash") {
    Write-Warning "Local v126 shader programs are incomplete; generic fallbacks can substantially lower race FPS."
    if (-not (Test-Path ".\game\ATV.zip" -PathType Leaf)) {
        Write-Host "Place your own ATV.BAD/ATV.BAH under game/ or a complete game/ATV/ folder; the launcher prepares game/ATV.zip automatically (see PUBLISHING.md)."
    }
    if (-not (Test-Path -LiteralPath $materialFixture -PathType Leaf)) {
        Write-Host "Play one ordinary run through a 3D menu, preview, and race. The launcher learns the material states on exit; the next launch uses them."
    }
}

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

# The original fast v126 build embedded the three literal bodies into this
# header. A copied .inc can be older than a previously built generic header;
# check the actual embedded bytes before spending time on a race comparison.
$expectedEmbeddedHash = "0e50cf80daa37174a94dc3c85feec8cf2744b3d3359eed8ab1e54a1b90a35465"
$embeddedShader = Join-Path $PWD "third_party/xboxrecomp/src/kernel/nv2a_gpu_shader.h"
$embeddedHash = "missing"
if (Test-Path -LiteralPath $embeddedShader -PathType Leaf) {
    $embeddedHash = (Get-FileHash -LiteralPath $embeddedShader -Algorithm SHA256).Hash.ToLowerInvariant()
}
$embeddedStatus = "path=$embeddedShader sha256=$embeddedHash expected=$expectedEmbeddedHash"
$embeddedStatus | Set-Content (Join-Path $runLogDir "shader-embed-status.txt")
Write-Host "[SHADER-EMBED] $embeddedStatus"
$allShadersReady = @($shaderStatus | Where-Object { $_ -match "missing|different hash" }).Count -eq 0
if ($allShadersReady -and $embeddedHash -ne $expectedEmbeddedHash) {
    throw "Embedded GPU shader differs from the original v126 build. See $runLogDir\shader-embed-status.txt; update the QPR2 xboxrecomp submodule and rebuild."
}

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

# The normal stderr records distinct combiner states once per program. Learn
# from all ordinary runs and install the fixture for the next build, without
# requiring timed F8 snapshots or a manual extraction command.
Update-Qpr2MaterialFixture

if (Test-Path ".\xbox_kernel.log") {
    Copy-Item -Force ".\xbox_kernel.log" "$runLogDir\one-hundred-twenty-sixth-kernel.log"
}
$results = @(Get-ChildItem "$runLogDir\diagnostic-v126-*", "$runLogDir\one-hundred-twenty-sixth-*", "$runLogDir\shader-status.txt", "$runLogDir\shader-embed-status.txt", "$runLogDir\shader-generation.*" -File)
if ($results.Count -gt 0) {
    Compress-Archive -LiteralPath $results.FullName -DestinationPath ".\logs\qpr2-v126-results.zip" -Force
    Write-Host "Results archived: $PWD\logs\qpr2-v126-results.zip"
}
