[CmdletBinding()]
param(
    [ValidateSet("Play", "Benchmark", "Debug")]
    [string]$Mode = "Play",
    [ValidateRange(0, 86400)]
    [int]$TimeoutSeconds = 0,
    [switch]$BuildOnly,
    [switch]$NoFrameTrace,
    [switch]$SampleGuestPc
)

$ErrorActionPreference = "Stop"
$root = $PSScriptRoot
$runtimePath = "third_party/xboxrecomp"
$runId = (Get-Date -Format "yyyyMMdd-HHmmss-fff") + "-" + [guid]::NewGuid().ToString("N").Substring(0, 8)
$runLogDir = Join-Path $root "logs\clean-v1-$runId"
$trackGuestPc = $Mode -eq "Debug" -or $SampleGuestPc
$buildVariant = if ($trackGuestPc) { "build-clean-v1-debug" } else { "build-clean-v1" }
$buildDir = Join-Path $root $buildVariant
$runtimeSettings = Import-PowerShellDataFile (Join-Path $root "tools/qpr2_runtime.psd1")

function Invoke-Qpr2Command {
    param([string]$Command, [string[]]$Arguments, [string]$LogFile)
    $savedErrorAction = $ErrorActionPreference
    try {
        $ErrorActionPreference = "Continue"
        & $Command @Arguments 2>&1 | Tee-Object -FilePath $LogFile | Out-Host
        $exitCode = $LASTEXITCODE
    } finally {
        $ErrorActionPreference = $savedErrorAction
    }
    $exitCode | Set-Content "$LogFile.exitcode"
    if ($exitCode -ne 0) { throw "$Command exited $exitCode. See $LogFile." }
}

function Get-Qpr2Toolchain {
    $cache = Get-Content -LiteralPath (Join-Path $buildDir "CMakeCache.txt")
    $compiler = Get-ChildItem -LiteralPath (Join-Path $buildDir "CMakeFiles") -Recurse -Filter "CMakeCCompiler.cmake" -File |
        Sort-Object LastWriteTimeUtc -Descending | Select-Object -First 1
    $toolchain = [ordered]@{}
    foreach ($line in $cache) {
        if ($line -match '^CMAKE_(GENERATOR|CACHE_(?:MAJOR|MINOR|PATCH)_VERSION|GENERATOR_PLATFORM):[^=]+=(.*)$') {
            $toolchain[$Matches[1].ToLowerInvariant()] = $Matches[2]
        }
    }
    if ($compiler) {
        foreach ($line in (Get-Content -LiteralPath $compiler.FullName)) {
            if ($line -match '^set\(CMAKE_(C_COMPILER_ID|C_COMPILER_VERSION) "([^"]+)"\)$') {
                $toolchain[$Matches[1].ToLowerInvariant()] = $Matches[2]
            }
        }
    }
    $configureLog = Get-Content -LiteralPath (Join-Path $runLogDir "configure.log")
    foreach ($line in $configureLog) {
        if ($line -match 'Selecting Windows SDK version ([\d.]+)') { $toolchain["windows_sdk"] = $Matches[1] }
    }
    $buildLog = Get-Content -LiteralPath (Join-Path $runLogDir "build.log") -TotalCount 20
    foreach ($line in $buildLog) {
        if ($line -match 'MSBuild version ([^ ]+)') { $toolchain["msbuild"] = $Matches[1] }
    }
    $project = Join-Path $buildDir "qpr2_recomp.vcxproj"
    if (-not $toolchain.Contains("windows_sdk") -and (Test-Path -LiteralPath $project)) {
        $match = Select-String -LiteralPath $project -Pattern '<WindowsTargetPlatformVersion>([\d.]+)<' | Select-Object -First 1
        if ($match) { $toolchain["windows_sdk"] = $match.Matches[0].Groups[1].Value }
    }
    return $toolchain
}

function Get-Qpr2Cim([string]$Class, [string[]]$Properties) {
    try { Get-CimInstance -ClassName $Class -Property $Properties -OperationTimeoutSec 5 -ErrorAction Stop }
    catch { Write-Verbose "System metadata for $Class is unavailable." }
}

function Add-Qpr2RunHardware($Manifest, [string]$RuntimeLog) {
    $processors = @(Get-Qpr2Cim "Win32_Processor" @("Name", "NumberOfCores", "NumberOfLogicalProcessors"))
    $system = Get-Qpr2Cim "Win32_ComputerSystem" @("TotalPhysicalMemory") | Select-Object -First 1
    $Manifest["cpu"] = @($processors | ForEach-Object { @{
        name = $_.Name.Trim()
        cores = $_.NumberOfCores
        logical_processors = $_.NumberOfLogicalProcessors
    } })
    if ($system) { $Manifest["ram_bytes"] = [long]$system.TotalPhysicalMemory }
    $id = Select-String -LiteralPath $RuntimeLog -Pattern '^\[GPU-ADAPTER-ID\] vendor=([0-9A-F]+) device=([0-9A-F]+) subsystem=([0-9A-F]+) revision=([0-9A-F]+)' |
        Select-Object -First 1
    $size = Select-String -LiteralPath $RuntimeLog -Pattern '^\s*\[FBWIN\] framebuffer window open \((\d+)x(\d+)\)' | Select-Object -First 1
    if ($size) {
        $groups = $size.Matches[0].Groups
        $Manifest["framebuffer"] = @{ width = [int]$groups[1].Value; height = [int]$groups[2].Value }
    }
    if ($id) {
        $groups = $id.Matches[0].Groups
        $prefix = "PCI\VEN_$($groups[1].Value)&DEV_$($groups[2].Value)&SUBSYS_$($groups[3].Value)&REV_$($groups[4].Value)"
        if ($Manifest["raster_adapter"]) { $Manifest["raster_adapter"]["pci_id"] = $prefix }
        $versions = @(Get-Qpr2Cim "Win32_VideoController" @("PNPDeviceID", "DriverVersion") |
            Where-Object { $_.PNPDeviceID -like "$prefix*" -and $_.DriverVersion } |
            Select-Object -ExpandProperty DriverVersion -Unique)
        if ($versions.Count -eq 1) { $Manifest["gpu_driver"] = $versions[0] }
    }
    $parity = Select-String -LiteralPath $RuntimeLog -Pattern '^\[(?:GPU-RASTER|GPU-TARGET-RESUME|GPU-TEXTURE-BRIDGE|GPU-GEOMETRY)\].*(?:comparison|mismatch|fallback|retained)' |
        Where-Object { $_.Line -notmatch '\bdraws=' } | Select-Object -First 64
    $Manifest["renderer_admission"] = @($parity | ForEach-Object { $_.Line })
}

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
                & ($choice.Path) @commandArgs 2>&1 | Tee-Object -FilePath $LogFile | Out-Host
            } else {
                & ($choice.Path) @commandArgs 2>&1 | Out-Host
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


# Host facts for the run manifest. Every query is optional: a machine where CIM
# is unavailable still builds and runs, and the field is recorded as null.
function Get-Qpr2RuntimeSource([string]$PinnedRevision, [bool]$TitleIsArchive) {
    $lifter = Join-Path $runtimePath "tools/recomp/__main__.py"
    if (-not (Test-Path -LiteralPath $lifter -PathType Leaf)) {
        if (-not $TitleIsArchive) {
            throw "Run git submodule update --init --recursive to install XboxRecomp."
        }
        $bootstrapLog = Join-Path $runLogDir "bootstrap.log"
        $exitCode = Invoke-Qpr2Python -Arguments @("tools/bootstrap_xboxrecomp.py") -LogFile $bootstrapLog
        if ($exitCode -ne 0) { throw "XboxRecomp setup exited $exitCode. See $bootstrapLog." }
    }

    if (Test-Path (Join-Path $runtimePath ".git")) {
        $revision = & git -C $runtimePath rev-parse HEAD
        if ($LASTEXITCODE -ne 0 -or $revision -ne $PinnedRevision) {
            $fix = if ($TitleIsArchive) { "py -3 tools/bootstrap_xboxrecomp.py" } else { "git submodule update --init --recursive" }
            throw "Expected XboxRecomp $PinnedRevision. Run $fix."
        }
        return @{ Kind = "git"; Revision = $revision; LiftIdentity = $revision }
    }

    $rendererHeader = Join-Path $runtimePath "src/kernel/nv2a_overlay_revision.h"
    $frameTraceSource = Join-Path $runtimePath "src/platform/frame_trace.c"
    $frameTraceHeader = Join-Path $runtimePath "src/platform/frame_trace.h"
    $profileHeader = Join-Path $runtimePath "src/kernel/nv2a_frame_profile.h"
    $threadCpuSource = Join-Path $runtimePath "src/platform/thread_cpu.c"
    if (-not (Test-Path -LiteralPath $threadCpuSource -PathType Leaf) -or
        -not (Test-Path -LiteralPath $rendererHeader -PathType Leaf) -or
        -not (Test-Path -LiteralPath $frameTraceSource -PathType Leaf) -or
        -not (Test-Path -LiteralPath $frameTraceHeader -PathType Leaf) -or
        -not (Select-String -LiteralPath $frameTraceHeader -Quiet -SimpleMatch 'XBOX_TRACE_DEPTH_DRAIN') -or
        -not (Test-Path -LiteralPath $profileHeader -PathType Leaf) -or
        -not (Select-String -LiteralPath $profileHeader -Quiet -SimpleMatch 'NV_FRAME_COMPLETION_REUSED') -or
        -not (Select-String -LiteralPath $rendererHeader -Quiet -Pattern '^#define QPR2_RENDERER_REVISION 1704u$') -or
        -not (Select-String -LiteralPath $lifter -Quiet -SimpleMatch '--runtime-instruction-hooks')) {
        throw "The XboxRecomp source in $runtimePath is incompatible. Use the QPR2 runtime archive for $PinnedRevision."
    }

    # A runtime ZIP has no commit ID. Use its actual lifter inputs for cache
    # invalidation, including when a user replaces the unpacked source.
    $directories = @("tools", "templates/runtime") | ForEach-Object { Join-Path $runtimePath $_ }
    $inputs = Get-ChildItem -LiteralPath $directories -Recurse -File |
        Where-Object { $_.Extension -in ".py", ".h" } |
        Sort-Object FullName | ForEach-Object {
            "$(Resolve-Path -LiteralPath $_.FullName -Relative) $((Get-FileHash -LiteralPath $_.FullName).Hash)"
        }
    $sha = [System.Security.Cryptography.SHA256]::Create()
    try {
        $bytes = [System.Text.Encoding]::UTF8.GetBytes(($inputs -join "`n"))
        $identity = ([System.BitConverter]::ToString($sha.ComputeHash($bytes))).Replace("-", "").ToLowerInvariant()
    } finally {
        $sha.Dispose()
    }
    return @{ Kind = "archive"; Revision = $null; LiftIdentity = $identity }
}

function Invoke-Qpr2BuildAndRun {
    $titleIsArchive = -not (Test-Path ".git")
    $titleRevision = $null
    if ($titleIsArchive) {
        $runtimePin = (Get-Content "tools/xboxrecomp-pin.txt" -Raw).Trim()
    } else {
        $titleRevision = & git rev-parse HEAD
        if ($LASTEXITCODE -ne 0) { throw "Could not read the QPR2 Git revision." }
        $runtimePin = & git rev-parse "HEAD:$runtimePath"
        if ($LASTEXITCODE -ne 0) { throw "Could not read the pinned XboxRecomp revision." }
    }

    if (-not (Test-Path "game/default.xbe" -PathType Leaf)) {
        throw "Extract your matching Xbox disc into game/ so game/default.xbe is present."
    }

    $xbeHash = (Get-FileHash "game/default.xbe" -Algorithm SHA256).Hash.ToLowerInvariant()
    if ($xbeHash -ne $runtimeSettings.XbeSha256) { throw "Unexpected default.xbe SHA-256 $xbeHash. See docs/KNOWN_BUILD.md." }
    $runtime = Get-Qpr2RuntimeSource -PinnedRevision $runtimePin -TitleIsArchive $titleIsArchive
    $manifest = [ordered]@{
        schema_version = 1
        run_id = $runId
        started_utc = [DateTime]::UtcNow.ToString("o")
        mode = $Mode
        title_source = if ($titleIsArchive) { "archive" } else { "git" }
        title_revision = $titleRevision
        runtime_source = $runtime.Kind
        runtime_revision = $runtime.Revision
        runtime_pin = $runtimePin
        runtime_lift_identity = $runtime.LiftIdentity
        xbe_sha256 = $xbeHash
        configuration = "Release"
        guest_pc_tracking = $trackGuestPc
        os = [Environment]::OSVersion.VersionString
        status = "preparing"
    }
    $manifestPath = Join-Path $runLogDir "run.json"
    $manifest | ConvertTo-Json -Depth 6 | Set-Content -Encoding UTF8 $manifestPath
    $pipelineHash = (Get-FileHash "tools/qpr2_pipeline.py" -Algorithm SHA256).Hash
    $liftKey = "$($runtime.LiftIdentity)/$xbeHash/$pipelineHash"
    $liftStamp = "analysis/clean-v1-lift.txt"
    $lifted = (Test-Path $liftStamp) -and ((Get-Content $liftStamp -Raw).Trim() -eq $liftKey) -and
        (Test-Path "src/generated/qpr2_build_config.h") -and
        (Test-Path "src/recomp/gen/recomp_dispatch.c") -and
        (Test-Path "src/recomp/gen/qpr2_manual_entries.c")
    if (-not $lifted) {
        Remove-Item $liftStamp -ErrorAction SilentlyContinue
        $liftLog = Join-Path $runLogDir "lift.log"
        $exitCode = Invoke-Qpr2Python -Arguments @("tools/qpr2_pipeline.py", "all") -LogFile $liftLog
        $exitCode | Set-Content "$liftLog.exitcode"
        if ($exitCode -ne 0) { throw "Lift exited $exitCode. See $liftLog." }
        Set-Content $liftStamp $liftKey
    }
    if ($Mode -eq "Debug") {
        $debugLog = Join-Path $runLogDir "debug-metadata.log"
        $symbols = Join-Path $runLogDir "guest-symbols.json"
        $exitCode = Invoke-Qpr2Python -Arguments @("tools/qpr2_debug_metadata.py", "symbols", "--output", $symbols) -LogFile $debugLog
        if ($exitCode -ne 0) { Write-Warning "Guest symbol export exited $exitCode. See $debugLog." }
    }

    $nativeHeader = "third_party/xboxrecomp/src/kernel/nv2a_vsh_compiled.h"
    $literalHeader = "third_party/xboxrecomp/src/kernel/nv2a_gpu_literal_programs.h"
    $literalBody = "third_party/xboxrecomp/src/kernel/nv2a_gpu_literal.inc"
    $materialFixture = "tools/fixtures/qpr2-literal-programs.json"

    function Update-Qpr2MaterialFixture {
        if (Test-Path -LiteralPath $materialFixture -PathType Leaf) { return }
        $runLogs = @(Get-ChildItem -Path ".\logs\clean-v1-*\run.stderr.log" -File -ErrorAction SilentlyContinue |
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
    if (-not (Test-Path $nativeHeader -PathType Leaf) -or -not (Test-Path ".\game\ATV.zip" -PathType Leaf)) {
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
    }
    if (-not (Test-Path $nativeHeader -PathType Leaf)) {
        Write-Host "[SHADER-BUILD] Compiling native vertex variants from game/ATV.zip"
        $nativeExit = Invoke-Qpr2Python -Arguments @("tools/compile_qpr2_vertex_programs.py", "game/ATV.zip")
        if ($nativeExit -ne 0) { throw "Native vertex generation failed: $nativeExit" }
    }
    # Generate the reviewed materials when absent. Preserve existing local variants
    # for diagnosis instead of silently replacing a user's custom files.
    $literalHeaderMissing = -not (Test-Path $literalHeader -PathType Leaf)
    $literalBodyMissing = -not (Test-Path $literalBody -PathType Leaf)
    if ((Test-Path $materialFixture -PathType Leaf) -and ($literalHeaderMissing -or $literalBodyMissing)) {
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


    $trackPc = if ($trackGuestPc) { "ON" } else { "OFF" }
    Invoke-Qpr2Command -Command cmake -Arguments @(
        "-S", ".", "-B", $buildDir, "-G", "Visual Studio 17 2022", "-A", "x64",
        "-DQPR2_TRACK_GUEST_PC=$trackPc"
    ) -LogFile (Join-Path $runLogDir "configure.log")
    Invoke-Qpr2Command -Command cmake -Arguments @(
        "--build", $buildDir, "--config", "Release", "--target", "qpr2_recomp", "--parallel"
    ) -LogFile (Join-Path $runLogDir "build.log")
    $manifest["toolchain"] = Get-Qpr2Toolchain

    $embeddedShader = Join-Path $runtimePath "src/kernel/nv2a_gpu_shader.h"
    $embeddedHash = (Get-FileHash $embeddedShader -Algorithm SHA256).Hash.ToLowerInvariant()
    $expectedHash = "0e50cf80daa37174a94dc3c85feec8cf2744b3d3359eed8ab1e54a1b90a35465"
    $allShadersReady = @($shaderStatus | Where-Object { $_ -match "missing|different hash" }).Count -eq 0
    $embedStatus = "sha256=$embeddedHash reference_v126=$expectedHash"
    Write-Host "[SHADER-EMBED] $embedStatus"
    $embedStatus | Set-Content (Join-Path $runLogDir "shader-embed-status.txt")
    if ($allShadersReady -and $embeddedHash -ne $expectedHash) {
        throw "The embedded shader hash differs from v126 despite matching local shader inputs."
    }
    $manifest["shader_status"] = $shaderStatus
    $manifest["embedded_shader_sha256"] = $embeddedHash
    $exe = Join-Path $buildDir "Release\qpr2_recomp.exe"
    $manifest["executable_sha256"] = (Get-FileHash -LiteralPath $exe -Algorithm SHA256).Hash.ToLowerInvariant()
    $manifest["nv2a_raster_source_sha256"] = (Get-FileHash -LiteralPath (Join-Path $runtimePath "src/kernel/nv2a_gpu_raster.c") -Algorithm SHA256).Hash.ToLowerInvariant()
    if (Test-Path -LiteralPath "game/ATV.zip" -PathType Leaf) {
        $manifest["atv_archive_sha256"] = (Get-FileHash -LiteralPath "game/ATV.zip" -Algorithm SHA256).Hash.ToLowerInvariant()
    }
    $manifest["status"] = "built"
    $manifest | ConvertTo-Json -Depth 6 | Set-Content -Encoding UTF8 $manifestPath
    if ($BuildOnly) { return }

    $timeout = $TimeoutSeconds
    if ($timeout -eq 0 -and $Mode -ne "Play") { $timeout = 360 }
    $settings = @{} + $runtimeSettings.Runtime
    if ($Mode -ne "Play") { $settings += $runtimeSettings.Benchmark }
    if ($Mode -eq "Debug") { $settings += $runtimeSettings.Debug }
    $settings["RECOMP_GPU_CAPTURE"] = Join-Path $runLogDir "gpu-capture.bin"
    if ($Mode -ne "Play" -and -not $NoFrameTrace) {
        $settings["QPR2_FRAME_TRACE_PATH"] = Join-Path $runLogDir "frame-trace.bin"
    } else {
        $settings["QPR2_FRAME_TRACE_PATH"] = $null
    }
    if ($timeout -gt 0) { $settings["RECOMP_WATCHDOG_SECS"] = [string]$timeout }

    $previous = @{}
    $effective = [ordered]@{}
    $tempLogDir = Join-Path $env:TEMP "qpr2-clean-v1-$runId"
    New-Item -ItemType Directory -Path $tempLogDir | Out-Null
    $tempStdout = Join-Path $tempLogDir "run.stdout.log"
    $tempStderr = Join-Path $tempLogDir "run.stderr.log"
    try {
        foreach ($name in ($settings.Keys | Sort-Object)) {
            $previous[$name] = [Environment]::GetEnvironmentVariable($name)
            if ($null -eq $previous[$name] -or $name -in "RECOMP_GPU_CAPTURE", "QPR2_FRAME_TRACE_PATH") {
                [Environment]::SetEnvironmentVariable($name, $settings[$name])
            }
        }
        Get-ChildItem Env: | Where-Object {
            $_.Name -like "RECOMP_*" -or $_.Name -like "QPR2_PAD_*" -or
            $_.Name -in "QPR2_MONITOR", "QPR2_FULLSCREEN"
        } |
            Sort-Object Name | ForEach-Object { $effective[$_.Name] = $_.Value }
        $manifest["environment"] = $effective
        $manifest["frame_trace_requested"] = [bool]$settings["QPR2_FRAME_TRACE_PATH"]
        $manifest["timeout_seconds"] = $timeout
        $manifest["status"] = "running"
        $manifest | ConvertTo-Json -Depth 6 | Set-Content -Encoding UTF8 $manifestPath
        Write-Host "Starting QPR2 clean v1. Live logs are in $tempLogDir"
        $process = Start-Process -FilePath $exe `
            -WorkingDirectory $root -PassThru -RedirectStandardOutput $tempStdout -RedirectStandardError $tempStderr
        # Keep the original process handle open through ExitCode, including
        # when the window thread terminates the process before Refresh().
        try { $null = $process.Handle } catch {
            if (-not $process.HasExited) { throw }
        }
        if ($timeout -eq 0) {
            $process.WaitForExit()
        } elseif (-not $process.WaitForExit($timeout * 1000)) {
            if (-not $process.HasExited) {
                $killOutput = & cmd.exe /d /c "taskkill /F /T /PID $($process.Id) 2>&1"
                $killExit = $LASTEXITCODE
                $killOutput | Set-Content (Join-Path $runLogDir "taskkill.log")
                if ($killExit -eq 0 -or -not $process.HasExited) {
                    $process.WaitForExit(10000) | Out-Null
                    $manifest["status"] = "timed_out"
                    $manifest["exit_code"] = 124
                }
            }
        }
        if ($manifest["status"] -eq "running") {
            $process.Refresh()
            $code = $process.ExitCode
            if ($null -eq $code) {
                $manifest["status"] = "exit_status_unavailable"
                $manifest["exit_code"] = $null
            } else {
                $manifest["exit_code"] = [int]$code
                $manifest["status"] = if ($code -eq 0) { "exited" } else { "failed" }
            }
        }
    } finally {
        foreach ($name in $previous.Keys) {
            [Environment]::SetEnvironmentVariable($name, $previous[$name])
        }
        Copy-Item -Path (Join-Path $tempLogDir "*.log") -Destination $runLogDir -ErrorAction SilentlyContinue
        if (Test-Path "xbox_kernel.log") {
            Copy-Item "xbox_kernel.log" (Join-Path $runLogDir "kernel.log")
        }
        $manifest["finished_utc"] = [DateTime]::UtcNow.ToString("o")
        if (Test-Path -LiteralPath $tempStderr -PathType Leaf) {
            $watchdog = Select-String -LiteralPath $tempStderr -Pattern '^\[WATCHDOG\]' | Select-Object -First 1
            $manifest["watchdog_fired"] = [bool]$watchdog
            $adapter = Select-String -LiteralPath $tempStderr -Pattern '^\[GPU-RASTER\] hardware adapter=(.+) feature_level=([0-9A-F]+) dedicated_mib=(\d+)' | Select-Object -First 1
            if ($adapter) {
                $groups = $adapter.Matches[0].Groups
                $manifest["raster_adapter"] = @{
                    name = $groups[1].Value
                    feature_level = $groups[2].Value
                    dedicated_mib = [long]$groups[3].Value
                }
            }
            if ($Mode -ne "Play") { Add-Qpr2RunHardware $manifest $tempStderr }
        }
        $manifest | ConvertTo-Json -Depth 6 | Set-Content -Encoding UTF8 $manifestPath
    }

    if ($Mode -ne "Play" -and (Test-Path -LiteralPath (Join-Path $runLogDir "run.stderr.log"))) {
        $mapLog = Join-Path $runLogDir "memory-map.log"
        $map = Join-Path $runLogDir "memory-map.json"
        $exitCode = Invoke-Qpr2Python -Arguments @("tools/qpr2_debug_metadata.py", "memory-map", "--run", $runLogDir, "--output", $map) -LogFile $mapLog
        if ($exitCode -ne 0) { Write-Warning "Memory map export exited $exitCode. See $mapLog." }
    }
    $tracePath = Join-Path $runLogDir "frame-trace.bin"
    if (Test-Path -LiteralPath $tracePath -PathType Leaf) {
        $traceLog = Join-Path $runLogDir "frame-trace.log"
        $exitCode = Invoke-Qpr2Python -Arguments @("tools/qpr2_frame_trace.py", "export", $runLogDir) -LogFile $traceLog
        if ($exitCode -ne 0) { Write-Warning "Frame trace export exited $exitCode. See $traceLog." }
    } elseif ($manifest["frame_trace_requested"]) {
        Write-Warning "Frame trace was requested but no trace file was created."
    }
    if ((Test-Path -LiteralPath (Join-Path $runLogDir "run.stderr.log")) -and
        ($manifest["status"] -ne "exited" -or $manifest["watchdog_fired"])) {
        $failure = Join-Path $runLogDir "failure.json"
        $failureLog = Join-Path $runLogDir "failure.log"
        $exitCode = Invoke-Qpr2Python -Arguments @("tools/qpr2_debug_metadata.py", "failure", "--run", $runLogDir, "--output", $failure) -LogFile $failureLog
        if ($exitCode -ne 0) { Write-Warning "Failure context export exited $exitCode. See $failureLog." }
    }
    Update-Qpr2MaterialFixture
    $results = Join-Path $root "logs\qpr2-clean-v1-results-$runId.zip"
    Compress-Archive -Path (Join-Path $runLogDir "*") -DestinationPath $results
    if ($null -eq $manifest["exit_code"]) {
        Write-Warning "The game ended without a readable process exit code. Results: $results"
        exit 125
    }
    Write-Host "Run ended with exit code $($manifest['exit_code']). Results: $results"
    if ($manifest["exit_code"] -ne 0) { exit $manifest["exit_code"] }
}

Push-Location $root
try {
    New-Item -ItemType Directory -Force $runLogDir | Out-Null
    Invoke-Qpr2BuildAndRun
} catch {
    $runManifest = Join-Path $runLogDir "run.json"
    if (Test-Path -LiteralPath $runManifest) {
        $failedRun = Get-Content -LiteralPath $runManifest -Raw | ConvertFrom-Json
        $failedRun.status = "failed"
        $failedRun | Add-Member -NotePropertyName error -NotePropertyValue $_.Exception.Message -Force
        $failedRun | Add-Member -NotePropertyName finished_utc -NotePropertyValue ([DateTime]::UtcNow.ToString("o")) -Force
        $failedRun | ConvertTo-Json -Depth 6 | Set-Content -Encoding UTF8 $runManifest
    }
    throw
} finally {
    Pop-Location
}
