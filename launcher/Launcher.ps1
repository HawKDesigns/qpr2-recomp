#requires -Version 5.1
<#
.SYNOPSIS
    Couch-friendly launcher for the ATV: Quad Power Racing 2 recomp.

.DESCRIPTION
    Starts a tiny web server on 127.0.0.1 (random port, per-run token), opens the
    launcher UI in an Edge/Chrome "app" window, and drives the project's own
    rebuild-and-run.ps1. Works on stock Windows PowerShell 5.1; nothing to install.

    Settings are passed to the game as RECOMP_* environment variables. A setting left
    at its default is never exported, so rebuild-and-run.ps1 and tools/qpr2_runtime.psd1
    keep control of their own defaults.

.PARAMETER NoBrowser
    Do not open the UI; print the URL instead (testing).
.PARAMETER Console
    Echo the launcher log to the console.
.PARAMETER NoSkip
    Ignore the "skip launcher" setting for this run.
#>
[CmdletBinding()]
param(
    [string]$Root,
    [int]$Port = 0,
    [switch]$NoBrowser,
    [switch]$Console,
    [switch]$NoSkip
)

$ErrorActionPreference = 'Stop'
$ProgressPreference = 'SilentlyContinue'
$script:LauncherVersion = '1.1.1'
$script:IsWin = ($env:OS -eq 'Windows_NT')
$script:Here = $PSScriptRoot
if (-not $Root) { $Root = Split-Path -Parent $script:Here }
$script:Root = (Resolve-Path -LiteralPath $Root).Path
$script:UiDir = Join-Path $script:Here 'ui'
$script:AssetDir = Join-Path $script:Here 'assets'
$script:SettingsFile = Join-Path $script:Here 'launcher.settings.json'
$script:StateFile = Join-Path $script:Here 'launcher.state.json'
$script:LogFile = Join-Path $script:Here 'launcher.log'
$script:SchemaFile = Join-Path $script:Here 'settings-schema.json'
$script:ProfileDir = Join-Path $script:Here '.ui-profile'
$script:UiProfile = $null
$script:UiProcess = $null
$script:ArtUrls = [ordered]@{
    logo       = 'https://cdn2.steamgriddb.com/logo/15d54085c87a67c0ff3c1f01b1c31eb9.png'
    background = 'https://www.video-games-museum.com/en/screenshots/Playstation%202/2/49662-menu-ATV-Quad-Power-Racing-2.jpg'
}

# ---------------------------------------------------------------- logging
function Write-Log([string]$Message) {
    $line = '{0} {1}' -f (Get-Date -Format 'HH:mm:ss'), $Message
    try {
        if ((Test-Path -LiteralPath $script:LogFile) -and (Get-Item -LiteralPath $script:LogFile).Length -gt 524288) {
            Remove-Item -LiteralPath $script:LogFile -Force -ErrorAction SilentlyContinue
        }
        Add-Content -LiteralPath $script:LogFile -Value $line -Encoding UTF8
    } catch { }
    if ($Console) { Write-Host $line }
}

# ---------------------------------------------------------------- generic helpers
function ConvertTo-Hashtable($obj) {
    if ($null -eq $obj) { return $null }
    if ($obj -is [System.Collections.IDictionary]) {
        $h = @{}
        foreach ($k in $obj.Keys) { $h[[string]$k] = ConvertTo-Hashtable $obj[$k] }
        return $h
    }
    if ($obj -is [System.Management.Automation.PSCustomObject]) {
        $h = @{}
        foreach ($p in $obj.PSObject.Properties) { $h[$p.Name] = ConvertTo-Hashtable $p.Value }
        return $h
    }
    if (($obj -is [System.Collections.IEnumerable]) -and ($obj -isnot [string])) {
        $list = New-Object System.Collections.ArrayList
        foreach ($i in $obj) { [void]$list.Add((ConvertTo-Hashtable $i)) }
        return , $list.ToArray()
    }
    return $obj
}

function Read-JsonFile([string]$Path, $Fallback) {
    try {
        if (Test-Path -LiteralPath $Path -PathType Leaf) {
            $raw = [System.IO.File]::ReadAllText($Path)
            if ($raw.Trim().Length -gt 0) { return (ConvertTo-Hashtable (ConvertFrom-Json $raw)) }
        }
    } catch { Write-Log "Could not read $Path : $($_.Exception.Message)" }
    return $Fallback
}

function Write-JsonFile([string]$Path, $Object) {
    $json = ConvertTo-Json -InputObject $Object -Depth 10
    [System.IO.File]::WriteAllText($Path, $json, (New-Object System.Text.UTF8Encoding($false)))
}

function Get-IsoNow { return [DateTime]::UtcNow.ToString('o') }
function As-Iso($v) { if ($v -is [datetime]) { return $v.ToUniversalTime().ToString('o') }; return [string]$v }
function Get-IsoOf([datetime]$d) { return $d.ToUniversalTime().ToString('o') }

function Join-P([string]$a, [string]$b) { return (Join-Path $a $b) }

function Get-DirSize([string]$Path) {
    if (-not (Test-Path -LiteralPath $Path -PathType Container)) { return [int64]0 }
    $sum = (Get-ChildItem -LiteralPath $Path -Recurse -File -Force -ErrorAction SilentlyContinue | Measure-Object -Property Length -Sum).Sum
    if ($null -eq $sum) { return [int64]0 }
    return [int64]$sum
}

function Find-Tool([string[]]$Names) {
    foreach ($n in $Names) {
        $c = Get-Command $n -ErrorAction SilentlyContinue | Select-Object -First 1
        if ($c) { return $c.Source }
    }
    return $null
}

# ---------------------------------------------------------------- schema / settings
$script:Schema = Read-JsonFile $script:SchemaFile $null
if (-not $script:Schema) { throw "settings-schema.json is missing or invalid: $script:SchemaFile" }
$script:Defs = @{}
foreach ($d in $script:Schema.settings) {
    # Large shared lists (the controller button sources) live once under choice_sets.
    if ($d.ContainsKey('choices_ref')) { $d['choices'] = $script:Schema.choice_sets[[string]$d.choices_ref] }
    $script:Defs[$d.id] = $d
}

function Get-Psd {
    $p = Join-P $script:Root 'tools\qpr2_runtime.psd1'
    if (-not (Test-Path -LiteralPath $p -PathType Leaf)) { return @{ Runtime = @{}; Benchmark = @{}; Debug = @{}; XbeSha256 = '' } }
    try { return (ConvertTo-Hashtable (Import-PowerShellDataFile -LiteralPath $p)) }
    catch { Write-Log "psd1 unreadable: $($_.Exception.Message)"; return @{ Runtime = @{}; Benchmark = @{}; Debug = @{}; XbeSha256 = '' } }
}

function Get-EffectiveDefaults {
    $psd = Get-Psd
    $out = @{}
    foreach ($d in $script:Schema.settings) {
        $v = [string]$d.default
        if ($d.env -and $psd.Runtime -and $psd.Runtime.ContainsKey([string]$d.env)) { $v = [string]$psd.Runtime[[string]$d.env] }
        $out[$d.id] = $v
    }
    return $out
}

function Test-SettingValue($Def, [string]$Value) {
    if ($Def.type -eq 'toggle') { return ($Value -eq '0' -or $Value -eq '1') }
    # Monitors are discovered at run time. Accept a bare device name so a monitor that is unplugged
    # today is still remembered; the game falls back to the primary monitor if it is missing.
    if ($Def.ContainsKey('choices_from') -and $Def.choices_from -eq 'monitors') { return ($Value -eq '' -or $Value -match '^DISPLAY[0-9]{1,2}$') }
    foreach ($c in $Def.choices) { if ([string]$c.value -eq $Value) { return $true } }
    return $false
}

function Get-Settings {
    $s = Read-JsonFile $script:SettingsFile @{}
    if (-not $s.ContainsKey('values')) { $s['values'] = @{} }
    if (-not $s.ContainsKey('ui')) { $s['ui'] = @{} }
    # Drop anything that is no longer valid for the current schema.
    foreach ($bag in @('values', 'ui')) {
        foreach ($k in @($s[$bag].Keys)) {
            $def = $script:Defs[$k]
            if (-not $def -or -not (Test-SettingValue $def ([string]$s[$bag][$k]))) { $s[$bag].Remove($k) }
        }
    }
    return $s
}

function Save-Settings($s) { Write-JsonFile $script:SettingsFile $s }

function Get-SettingValue($s, [string]$Id, $Defaults) {
    $def = $script:Defs[$Id]
    $bag = if ($def.scope -eq 'ui') { 'ui' } else { 'values' }
    if ($s[$bag].ContainsKey($Id)) { return [string]$s[$bag][$Id] }
    return [string]$Defaults[$Id]
}

function Set-SettingValue([string]$Id, [string]$Value) {
    $def = $script:Defs[$Id]
    if (-not $def) { throw "Unknown setting '$Id'." }
    if (-not (Test-SettingValue $def $Value)) { throw "Invalid value for '$Id'." }
    $defaults = Get-EffectiveDefaults
    $s = Get-Settings
    $bag = if ($def.scope -eq 'ui') { 'ui' } else { 'values' }
    if ($Value -eq [string]$defaults[$Id]) { $s[$bag].Remove($Id) } else { $s[$bag][$Id] = $Value }
    Save-Settings $s
}

# Build the RECOMP_* overrides requested by the player. Only non-default values are exported.
function Get-GameEnv($s) {
    $env2 = [ordered]@{}
    $defaults = Get-EffectiveDefaults
    foreach ($id in @($s['values'].Keys | Sort-Object)) {
        $def = $script:Defs[$id]
        if (-not $def -or $def.scope -ne 'game') { continue }
        $parent = $def.requires
        while ($parent -and (Get-SettingValue $s $parent $defaults) -eq '1') {
            $parent = $script:Defs[$parent].requires
        }
        if ($parent) { continue }
        $val = [string]$s['values'][$id]
        if ($def.env) {
            if ($val -ne '') { $env2[[string]$def.env] = $val }
        } elseif ($def.env_values) {
            $mapped = $def.env_values[$val]
            foreach ($k in $mapped.Keys) { $env2[$k] = [string]$mapped[$k] }
        } else {
            foreach ($c in $def.choices) {
                if ([string]$c.value -eq $val -and $c.env) {
                    foreach ($k in $c.env.Keys) { $env2[[string]$k] = [string]$c.env[$k] }
                }
            }
        }
    }
    # With a chosen monitor the game's own display thread moves the window first and then asks the
    # runtime for fullscreen there; the runtime would otherwise go fullscreen on the primary monitor.
    if ($env2.Contains('QPR2_MONITOR') -and $env2['RECOMP_FULLSCREEN'] -eq '1') {
        $env2['RECOMP_FULLSCREEN'] = '0'
        $env2['QPR2_FULLSCREEN'] = '1'
    }
    return $env2
}

# ---------------------------------------------------------------- persistent launcher state
function Get-LState {
    $st = Read-JsonFile $script:StateFile @{}
    if (-not $st.ContainsKey('sessions')) { $st['sessions'] = 0 }
    if (-not $st.ContainsKey('seconds')) { $st['seconds'] = 0 }
    return $st
}
function Save-LState($st) { Write-JsonFile $script:StateFile $st }

# ---------------------------------------------------------------- environment / health checks
$script:Cache = @{}
function Get-Cached([string]$Key, [int]$TtlSeconds, [scriptblock]$Producer) {
    $now = [DateTime]::UtcNow
    if ($script:Cache.ContainsKey($Key)) {
        $e = $script:Cache[$Key]
        if (($now - $e.At).TotalSeconds -lt $TtlSeconds) { return $e.Value }
    }
    $v = & $Producer
    $script:Cache[$Key] = @{ At = $now; Value = $v }
    return $v
}
function Clear-Cache { $script:Cache = @{} }

function Get-ToolInfo {
    return (Get-Cached 'tools' 600 {
        $vs = $null
        if ($script:IsWin) {
            $pf86 = ${env:ProgramFiles(x86)}
            if ($pf86) {
                $vw = Join-P $pf86 'Microsoft Visual Studio\Installer\vswhere.exe'
                if (Test-Path -LiteralPath $vw) {
                    try {
                        $vs = (& $vw -latest -products '*' -requires 'Microsoft.VisualStudio.Component.VC.Tools.x86.x64' -property installationPath 2>$null | Select-Object -First 1)
                    } catch { }
                }
            }
        }
        @{
            git    = [bool](Find-Tool @('git'))
            cmake  = [bool](Find-Tool @('cmake'))
            python = [bool](Find-Tool @('python', 'python3', 'py'))
            vs     = [bool]$vs
        }
    })
}

# Monitors by Windows device name (DISPLAY1, DISPLAY2...), the same name the game matches on.
function Get-Monitors {
    return (Get-Cached 'monitors' 30 {
        $list = New-Object System.Collections.ArrayList
        if ($script:IsWin) {
            try {
                Add-Type -AssemblyName System.Windows.Forms -ErrorAction Stop
                $n = 0
                foreach ($m in [System.Windows.Forms.Screen]::AllScreens) {
                    $n++
                    $dev = ([string]$m.DeviceName).Split([char]92)[-1]   # device path -> DISPLAY2
                    if ($dev -notmatch '^DISPLAY[0-9]{1,2}$') { continue }
                    $label = 'Display {0}: {1} x {2}' -f $dev.Substring(7), $m.Bounds.Width, $m.Bounds.Height
                    if ($m.Primary) { $label += ' (primary)' }
                    [void]$list.Add(@{ value = $dev; label = $label })
                }
            } catch { Write-Log "Monitor detection failed: $($_.Exception.Message)" }
        }
        , $list.ToArray()
    })
}

function Get-XbeInfo {
    $xbe = Join-P (Join-P $script:Root 'game') 'default.xbe'
    if (-not (Test-Path -LiteralPath $xbe -PathType Leaf)) { return @{ exists = $false; match = $false; sha = '' } }
    $fi = Get-Item -LiteralPath $xbe
    $key = 'xbe-{0}-{1}' -f $fi.Length, $fi.LastWriteTimeUtc.Ticks
    return (Get-Cached $key 86400 {
        $sha = (Get-FileHash -LiteralPath $xbe -Algorithm SHA256).Hash.ToLowerInvariant()
        $want = [string](Get-Psd).XbeSha256
        @{ exists = $true; match = ($sha -eq $want.ToLowerInvariant()); sha = $sha }
    })
}

function Get-BuildExe([string]$Mode) {
    $variant = if ($Mode -eq 'Debug') { 'build-clean-v1-debug' } else { 'build-clean-v1' }
    return (Join-P (Join-P (Join-P $script:Root $variant) 'Release') 'qpr2_recomp.exe')
}

function Get-RunDirs([bool]$HistoryOnly = $false) {
    $logs = Join-P $script:Root 'logs'
    if (-not (Test-Path -LiteralPath $logs -PathType Container)) { return @() }
    $dirs = @(Get-ChildItem -LiteralPath $logs -Directory -ErrorAction SilentlyContinue |
        Where-Object { $_.Name -match '^(clean-v1|launcher)-' } |
        Sort-Object LastWriteTimeUtc -Descending)
    if ($HistoryOnly) {
        # Full-mode launcher folders only hold console output; the real record is the clean-v1-* run.
        $dirs = @($dirs | Where-Object { $_.Name -like 'clean-v1-*' -or (Test-Path -LiteralPath (Join-P $_.FullName 'run.json') -PathType Leaf) })
    }
    return $dirs
}

function Read-RunSummary($Dir) {
    $m = Read-JsonFile (Join-P $Dir.FullName 'run.json') $null
    $id = $Dir.Name
    $zip = Join-P (Join-P $script:Root 'logs') ('qpr2-clean-v1-results-' + ($id -replace '^(clean-v1|launcher)-', '') + '.zip')
    $r = [ordered]@{
        id = $id; modifiedUtc = (Get-IsoOf $Dir.LastWriteTimeUtc)
        mode = ''; status = 'unknown'; exitCode = $null; startedUtc = ''; finishedUtc = ''; error = ''
        hasResultsZip = (Test-Path -LiteralPath $zip -PathType Leaf)
    }
    if ($m) {
        if ($m.ContainsKey('mode')) { $r.mode = [string]$m.mode }
        if ($m.ContainsKey('status')) { $r.status = [string]$m.status }
        if ($m.ContainsKey('exit_code')) { $r.exitCode = $m.exit_code }
        if ($m.ContainsKey('started_utc')) { $r.startedUtc = As-Iso $m.started_utc }
        if ($m.ContainsKey('finished_utc')) { $r.finishedUtc = As-Iso $m.finished_utc }
        if ($m.ContainsKey('error')) { $r.error = [string]$m.error }
    }
    return $r
}

function Get-ShaderStatus {
    $kernel = Join-P $script:Root 'third_party\xboxrecomp\src\kernel'
    $items = @(
        @{ key = 'native'; label = 'Native vertex programs'; file = 'nv2a_vsh_compiled.h' },
        @{ key = 'classifier'; label = 'Material classifier'; file = 'nv2a_gpu_literal_programs.h' },
        @{ key = 'bodies'; label = 'Material shader bodies'; file = 'nv2a_gpu_literal.inc' }
    )
    $present = 0
    $detail = @()
    foreach ($i in $items) {
        $ok = Test-Path -LiteralPath (Join-P $kernel $i.file) -PathType Leaf
        if ($ok) { $present++ }
        $detail += @{ key = $i.key; label = $i.label; present = $ok }
    }
    $fixture = Test-Path -LiteralPath (Join-P $script:Root 'tools\fixtures\qpr2-literal-programs.json') -PathType Leaf
    $verified = ''
    foreach ($d in (Get-RunDirs $true | Select-Object -First 8)) {
        $f = Join-P $d.FullName 'shader-status.txt'
        if (Test-Path -LiteralPath $f -PathType Leaf) {
            $lines = @(Get-Content -LiteralPath $f -ErrorAction SilentlyContinue)
            $verified = if (@($lines | Where-Object { $_ -match 'missing|different hash' }).Count -eq 0) { 'ready' } else { 'fallback' }
            break
        }
    }
    return @{ present = $present; total = 3; items = $detail; fixture = $fixture; lastRun = $verified }
}

function Get-Checks {
    $checks = @()
    $psd = Get-Psd
    $xbe = Get-XbeInfo
    $gameDir = Join-P $script:Root 'game'

    # Game files
    if (-not $xbe.exists) {
        $checks += @{ id = 'xbe'; label = 'Game files'; status = 'bad'; detail = 'game\default.xbe not found. Extract your own disc into the game folder.'; action = 'open-game' }
    } elseif (-not $xbe.match) {
        $checks += @{ id = 'xbe'; label = 'Game files'; status = 'bad'; detail = 'default.xbe is a different release than the one this build supports (see docs/KNOWN_BUILD.md).'; action = 'open-game' }
    } else {
        $checks += @{ id = 'xbe'; label = 'Game files'; status = 'ok'; detail = 'default.xbe found and verified.' }
    }

    # ATV archive / vertex data
    $hasZip = Test-Path -LiteralPath (Join-P $gameDir 'ATV.zip') -PathType Leaf
    $hasBad = $false
    if (Test-Path -LiteralPath $gameDir -PathType Container) {
        $hasBad = [bool](Get-ChildItem -LiteralPath $gameDir -Recurse -Depth 3 -File -Filter 'ATV.BAD' -ErrorAction SilentlyContinue | Select-Object -First 1)
        if (-not $hasBad) { $hasBad = Test-Path -LiteralPath (Join-P (Join-P $gameDir 'ATV') 'VertexShader') }
    }
    if ($hasZip) {
        $checks += @{ id = 'archive'; label = 'ATV archive'; status = 'ok'; detail = 'game\ATV.zip is ready.' }
    } elseif ($hasBad) {
        $checks += @{ id = 'archive'; label = 'ATV archive'; status = 'todo'; detail = 'Will be prepared from your ATV.BAD / ATV.BAH on first launch.' }
    } else {
        $checks += @{ id = 'archive'; label = 'ATV archive'; status = 'bad'; detail = 'ATV.BAD and ATV.BAH were not found under game\.'; action = 'open-game' }
    }

    # Runtime source
    $rt = Test-Path -LiteralPath (Join-P $script:Root 'third_party\xboxrecomp\CMakeLists.txt') -PathType Leaf
    $tools = Get-ToolInfo
    if ($rt) { $checks += @{ id = 'runtime'; label = 'XboxRecomp runtime'; status = 'ok'; detail = 'Pinned runtime source is in place.' } }
    elseif ($tools.git) { $checks += @{ id = 'runtime'; label = 'XboxRecomp runtime'; status = 'todo'; detail = 'Will be downloaded on first launch.' } }
    else { $checks += @{ id = 'runtime'; label = 'XboxRecomp runtime'; status = 'warn'; detail = 'Missing, and Git was not found to download it. See the README.' } }

    # Translated code
    $lifted = (Test-Path -LiteralPath (Join-P $script:Root 'src\generated\qpr2_build_config.h') -PathType Leaf) -and
              (Test-Path -LiteralPath (Join-P $script:Root 'analysis\clean-v1-lift.txt') -PathType Leaf)
    if ($lifted) { $checks += @{ id = 'lift'; label = 'Translated game code'; status = 'ok'; detail = 'Generated from your executable.' } }
    else { $checks += @{ id = 'lift'; label = 'Translated game code'; status = 'todo'; detail = 'Generated from your executable on first launch. This takes a few minutes.' } }

    # Shaders
    $sh = Get-ShaderStatus
    if ($sh.present -ge $sh.total) {
        $d = 'All 3 fast-path shader sets are generated.'
        if ($sh.lastRun -eq 'fallback') { $d = 'Generated, but the last run reported a mismatch. Check the run log.' }
        $checks += @{ id = 'shaders'; label = 'Fast-path shaders'; status = $(if ($sh.lastRun -eq 'fallback') { 'warn' } else { 'ok' }); detail = $d }
    } elseif ($sh.fixture) {
        $checks += @{ id = 'shaders'; label = 'Fast-path shaders'; status = 'todo'; detail = 'Learned from your play. They will be generated on the next launch.' }
    } else {
        $checks += @{ id = 'shaders'; label = 'Fast-path shaders'; status = 'warn'; detail = 'Using generic fallbacks, so races run slower. Play through a 3D menu, a preview and a race, close the game, then launch again.' }
    }

    # Build
    $mode = Get-SettingValue (Get-Settings) 'play_mode' (Get-EffectiveDefaults)
    $exe = Get-BuildExe $mode
    if (Test-Path -LiteralPath $exe -PathType Leaf) {
        $checks += @{ id = 'build'; label = 'Game build'; status = 'ok'; detail = ('Built ' + (Get-Item -LiteralPath $exe).LastWriteTime.ToString('d MMM yyyy, HH:mm') + '.') }
    } else {
        $checks += @{ id = 'build'; label = 'Game build'; status = 'todo'; detail = 'Compiled on first launch. Expect several minutes.' }
    }

    # Diagnostics are exported by the title's full runner.
    if ($mode -ne 'Play') {
        $scriptPath = Join-P $script:Root 'rebuild-and-run.ps1'
        $exporter = Join-P $script:Root 'tools\qpr2_debug_metadata.py'
        $runner = if (Test-Path -LiteralPath $scriptPath -PathType Leaf) { Get-Content -LiteralPath $scriptPath -Raw }
        if (-not (Test-Path -LiteralPath $exporter -PathType Leaf) -or
            $runner -notmatch 'guest-symbols\.json' -or $runner -notmatch 'memory-map\.json') {
            $checks += @{ id = 'metadata'; label = 'Debugger exports'; status = 'warn'; detail = 'Update the QPR2 title checkout to export debugger JSON files in the run folder.' }
        }
    }

    # Toolchain (only matters when something still has to be built)
    $needsTools = (-not (Test-Path -LiteralPath $exe -PathType Leaf)) -or (-not $lifted)
    if ($script:IsWin) {
        $missing = @()
        if (-not $tools.cmake) { $missing += 'CMake' }
        if (-not $tools.vs) { $missing += 'Visual Studio 2022 C++ tools' }
        if (-not $tools.python) { $missing += 'Python 3.10+' }
        if ($missing.Count -eq 0) { $checks += @{ id = 'tools'; label = 'Build tools'; status = 'ok'; detail = 'CMake, Visual Studio and Python were found.' } }
        elseif ($needsTools) { $checks += @{ id = 'tools'; label = 'Build tools'; status = 'bad'; detail = ('Missing: ' + ($missing -join ', ') + '.') } }
        else { $checks += @{ id = 'tools'; label = 'Build tools'; status = 'warn'; detail = ('Not found: ' + ($missing -join ', ') + '. Needed for rebuilds.') } }
    }
    return $checks
}

# ---------------------------------------------------------------- UI window control (Windows only)
$script:WinApiReady = $false
function Initialize-WinApi {
    if (-not $script:IsWin -or $script:WinApiReady) { return }
    try {
        Add-Type -Namespace Qpr2 -Name Native -MemberDefinition @'
[DllImport("user32.dll")] public static extern bool ShowWindow(System.IntPtr h, int cmd);
[DllImport("user32.dll")] public static extern bool SetForegroundWindow(System.IntPtr h);
[DllImport("user32.dll")] public static extern short GetAsyncKeyState(int key);
'@ -ErrorAction Stop
        $script:WinApiReady = $true
    } catch {
        if ($_.Exception.Message -notmatch 'already exists') { Write-Log "WinApi init failed: $($_.Exception.Message)" } else { $script:WinApiReady = $true }
    }
}

function Get-UiProcess {
    $p = $script:UiProcess
    if (-not $p) { return $null }
    $p.Refresh()
    if (-not $p.HasExited) { return $p }
}

function Set-UiWindow([string]$Action) {
    if (-not $script:IsWin) { return }
    Initialize-WinApi
    try {
        $p = Get-UiProcess
        if (-not $p -or $p.MainWindowHandle -eq [IntPtr]::Zero) { return }
        switch ($Action) {
            'minimize' { [void][Qpr2.Native]::ShowWindow($p.MainWindowHandle, 6) }
            'restore'  { [void][Qpr2.Native]::ShowWindow($p.MainWindowHandle, 9); [void][Qpr2.Native]::SetForegroundWindow($p.MainWindowHandle) }
            'close'    { [void]$p.CloseMainWindow() }
        }
    } catch { Write-Log "Window action $Action failed: $($_.Exception.Message)" }
}

function Test-ShiftHeld {
    if (-not $script:IsWin) { return $false }
    Initialize-WinApi
    try { return (([Qpr2.Native]::GetAsyncKeyState(0x10) -band 0x8000) -ne 0) } catch { return $false }
}

# ---------------------------------------------------------------- launch jobs
$script:Job = $null
$script:QuitAfterJob = $false

function Get-PowerShellExe {
    try { $p = (Get-Process -Id $PID).Path; if ($p) { return $p } } catch { }
    if ($script:IsWin) { return (Join-P $env:SystemRoot 'System32\WindowsPowerShell\v1.0\powershell.exe') }
    return 'pwsh'
}

function Q([string]$s) { if ($script:IsWin) { return '"' + $s + '"' } else { return $s } }

function Test-JobRunning { return ($script:Job -and $script:Job.state -eq 'running') }

function Add-JobLine([string]$Text) {
    $clean = ($Text -replace '\x1B\[[0-9;]*[A-Za-z]', '').TrimEnd("`r", "`n")
    if ($clean.Trim().Length -eq 0) { return }
    [void]$script:Job.lines.Add($clean)
    $script:Job.lineCount++
    if ($script:Job.lines.Count -gt 600) { $script:Job.lines.RemoveRange(0, 200); $script:Job.lineBase += 200 }
    # Phase detection from the output of rebuild-and-run.ps1.
    $j = $script:Job
    switch -Regex ($clean) {
        'Starting QPR2'                          { $j.phase = 'run'; $j.phaseText = 'Starting the game'; if (-not $j.gameStarted) { $j.gameStarted = [DateTime]::UtcNow }; break }
        'Run ended with exit code'               { $j.phase = 'finish'; $j.phaseText = 'Saving run results'; break }
        '^\[PYTHON\]'                            { if ($j.phase -eq 'prepare') { $j.phaseText = 'Preparing tools' }; break }
        '\[ATV-PREP\]'                           { $j.phase = 'prepare'; $j.phaseText = 'Preparing the game archive'; break }
        '\[SHADER-(BUILD|LEARN)\]'               { $j.phase = 'prepare'; $j.phaseText = 'Preparing shaders'; break }
        '\[SHADER-(STATUS|EMBED)\]'              { if ($j.phase -ne 'run') { $j.phaseText = 'Checking shaders' }; break }
        '^-- (Configuring|Generating|Build files)' { $j.phase = 'build'; $j.phaseText = 'Configuring the build'; break }
        'MSBuild|\.vcxproj|Compiling|Linking|Building C'  { $j.phase = 'build'; $j.phaseText = 'Building the game'; break }
        'qpr2_pipeline|Lifting|lift complete'    { if ($j.phase -eq 'prepare') { $j.phaseText = 'Translating game code' }; break }
        '\[BOOT\]'                               { $j.phase = 'run'; if (-not $j.gameStarted) { $j.gameStarted = [DateTime]::UtcNow }; break }
    }
}

function Read-NewText([string]$Path, [ref]$Offset) {
    if (-not (Test-Path -LiteralPath $Path -PathType Leaf)) { return '' }
    $fs = $null
    try {
        $fs = [System.IO.File]::Open($Path, 'Open', 'Read', 'ReadWrite')
        if ($fs.Length -le $Offset.Value) { return '' }
        [void]$fs.Seek($Offset.Value, 'Begin')
        $len = [int]($fs.Length - $Offset.Value)
        $buf = New-Object byte[] $len
        $read = $fs.Read($buf, 0, $len)
        $Offset.Value += $read
        return [System.Text.Encoding]::UTF8.GetString($buf, 0, $read)
    } catch { return '' }
    finally { if ($fs) { $fs.Dispose() } }
}

function Update-Job {
    $j = $script:Job
    if (-not $j -or $j.state -ne 'running') { return }

    foreach ($which in @('out', 'err')) {
        $path = $j[$which + 'File']
        $off = [ref]([int64]$j[$which + 'Off'])
        $txt = Read-NewText $path $off
        $j[$which + 'Off'] = $off.Value
        if ($txt.Length -gt 0) {
            $txt = $j[$which + 'Part'] + $txt
            $parts = $txt -split "`n"
            $j[$which + 'Part'] = $parts[$parts.Length - 1]
            for ($i = 0; $i -lt $parts.Length - 1; $i++) {
                Add-JobLine $parts[$i]
                if ($which -eq 'err') { [void]$j.errTail.Add($parts[$i].Trim()); if ($j.errTail.Count -gt 12) { $j.errTail.RemoveAt(0) } }
            }
        }
    }

    # The game process is the most reliable "it is running" signal.
    if (-not $j.gameStarted -and $script:IsWin) {
        $g = Get-Process -Name 'qpr2_recomp' -ErrorAction SilentlyContinue | Select-Object -First 1
        if ($g) { $j.gameStarted = [DateTime]::UtcNow; $j.phase = 'run'; $j.phaseText = 'Starting the game' }
    }
    if ($j.gameStarted -and -not $j.uiHandled) {
        $j.uiHandled = $true
        $s = Get-Settings; $d = Get-EffectiveDefaults
        $after = Get-SettingValue $s 'ui_after_launch' $d
        if ($after -eq 'minimize') { Start-Sleep -Milliseconds 600; Set-UiWindow 'minimize' }
        elseif ($after -eq 'close') { Set-UiWindow 'close'; $script:QuitAfterJob = $true }
    }

    $proc = $j.proc
    if ($proc -and $proc.HasExited) {
        # Windows PowerShell 5.1 only exposes ExitCode reliably after WaitForExit().
        [void]$proc.WaitForExit(3000)
        $j.exitCode = $proc.ExitCode
        Complete-Job
    }
}

function Complete-Job {
    $j = $script:Job
    # Drain the last output.
    foreach ($which in @('out', 'err')) {
        $off = [ref]([int64]$j[$which + 'Off'])
        $txt = Read-NewText $j[$which + 'File'] $off
        if ($txt.Length -gt 0) {
            foreach ($l in ($txt -split "`n")) {
                Add-JobLine $l
                if ($which -eq 'err' -and $l.Trim().Length -gt 0) { [void]$j.errTail.Add($l.Trim()); if ($j.errTail.Count -gt 12) { $j.errTail.RemoveAt(0) } }
            }
        }
    }
    $j.ended = [DateTime]::UtcNow
    $code = $j.exitCode
    if ($null -eq $code) { $code = $(if ($j.stopRequested) { 1 } else { 125 }) }
    $j.exitCode = $code
    $started = [bool]$j.gameStarted
    $outcome = 'ok'
    if ($j.stopRequested) { $outcome = 'stopped' }
    elseif ($j.buildOnly) { $outcome = $(if ($code -eq 0) { 'built' } else { 'build_failed' }) }
    elseif (-not $started -and $code -ne 0) { $outcome = 'build_failed' }
    elseif ($code -eq 124) { $outcome = 'timeout' }
    elseif ($code -ne 0) { $outcome = 'crashed' }

    $message = ''
    if ($outcome -eq 'build_failed') {
        $tail = @($j.errTail | Where-Object { $_ -and $_ -notmatch '^\s*(\+|At line|CategoryInfo|FullyQualifiedErrorId)' })
        $message = ($tail | Select-Object -Last 3) -join ' '
        $tools = Get-ToolInfo
        if ($script:IsWin -and (-not $tools.cmake -or -not $tools.vs)) { $message += ' Build tools look incomplete: install Visual Studio 2022 (Desktop development with C++) and CMake.' }
    }
    $sessionSec = 0
    if ($j.gameStarted) { $sessionSec = [int](($j.ended - $j.gameStarted).TotalSeconds) }
    $cut = $j.started.AddSeconds(-3)
    $latest = Get-RunDirs $true | Where-Object { $_.LastWriteTimeUtc -ge $cut } | Select-Object -First 1
    $j.result = @{
        outcome = $outcome; exitCode = $code; message = $message.Trim(); sessionSeconds = $sessionSec
        runId = $(if ($latest) { $latest.Name } else { '' })
    }
    if ($j.kind -eq 'quick') {
        try {
            $mf = Join-P (Split-Path -Parent $j.outFile) 'run.json'
            $m = Read-JsonFile $mf @{}
            $m['status'] = $(if ($outcome -eq 'ok') { 'exited' } else { $outcome })
            $m['exit_code'] = $code
            $m['finished_utc'] = Get-IsoOf $j.ended
            Write-JsonFile $mf $m
        } catch { }
    }
    $j.state = 'finished'
    $j.phase = 'finish'
    $j.phaseText = 'Finished'
    Write-Log ("Job {0} finished: {1} (exit {2}, {3}s)" -f $j.id, $outcome, $code, $sessionSec)

    if ($j.gameStarted -and -not $j.buildOnly) {
        $st = Get-LState
        $st['sessions'] = [int]$st['sessions'] + 1
        $st['seconds'] = [int]$st['seconds'] + $sessionSec
        $st['lastPlayedUtc'] = Get-IsoOf $j.ended
        $st['lastSession'] = @{ mode = $j.mode; seconds = $sessionSec; outcome = $outcome; exitCode = $code }
        Save-LState $st
    }
    Clear-Cache
    if ($script:QuitAfterJob) { $script:StopServer = $true } else { Set-UiWindow 'restore' }
}

function Invoke-AutoBackup($s, $d) {
    if ((Get-SettingValue $s 'ui_auto_backup' $d) -ne '1') { return }
    try {
        $made = New-SaveBackup 'auto' $true
        if ($made) { Write-Log "Auto-backup: $made" }
        $keep = [int](Get-SettingValue $s 'ui_backup_keep' $d)
        Remove-OldBackups $keep
    } catch { Write-Log "Auto-backup failed: $($_.Exception.Message)" }
}

function Start-Job2([string]$Mode, [bool]$BuildOnly) {
    if (Test-JobRunning) { throw 'Something is already running.' }
    if ($Mode -notin @('Play', 'Benchmark', 'Debug')) { throw 'Unknown mode.' }
    $xbe = Get-XbeInfo
    if (-not $xbe.exists -or -not $xbe.match) { throw 'The game files are missing or do not match this build. Check Setup on the Play page.' }

    $s = Get-Settings; $d = Get-EffectiveDefaults
    $timeout = Get-SettingValue $s 'timeout' $d
    $method = Get-SettingValue $s 'launch_method' $d
    $exe = Get-BuildExe $Mode
    $quick = ($Mode -eq 'Play') -and (-not $BuildOnly) -and ($method -eq 'quick') -and (Test-Path -LiteralPath $exe -PathType Leaf)

    if (-not $BuildOnly) { Invoke-AutoBackup $s $d }

    $id = (Get-Date -Format 'yyyyMMdd-HHmmss-fff')
    $logDir = Join-P (Join-P $script:Root 'logs') ('launcher-' + $id)
    New-Item -ItemType Directory -Force -Path $logDir | Out-Null
    $outFile = Join-P $logDir 'launcher.stdout.log'
    $errFile = Join-P $logDir 'launcher.stderr.log'
    Set-Content -LiteralPath $outFile -Value '' -Encoding UTF8
    Set-Content -LiteralPath $errFile -Value '' -Encoding UTF8

    # Player overrides first, so rebuild-and-run.ps1 (which only fills unset variables) keeps them.
    $overrides = Get-GameEnv $s
    $applied = @{}
    foreach ($k in $overrides.Keys) { $applied[$k] = [Environment]::GetEnvironmentVariable($k); [Environment]::SetEnvironmentVariable($k, [string]$overrides[$k]) }

    try {
        $ps = Get-PowerShellExe
        $startParams = @{ PassThru = $true; WorkingDirectory = $script:Root; RedirectStandardOutput = $outFile; RedirectStandardError = $errFile }

        if ($quick) {
            # A quick Play launch uses the normal runtime profile.
            $psd = Get-Psd
            $bag = @{} + $psd.Runtime
            if ($timeout) { $bag['RECOMP_WATCHDOG_SECS'] = [string]$timeout }
            foreach ($k in $bag.Keys) {
                if ($null -eq [Environment]::GetEnvironmentVariable($k)) { $applied[$k] = $null; [Environment]::SetEnvironmentVariable($k, [string]$bag[$k]) }
            }
            $proc = Start-Process -FilePath $exe @startParams
            $null = $proc.Handle
            $kind = 'quick'
        } else {
            $args2 = @('-NoProfile', '-ExecutionPolicy', 'Bypass', '-File', (Q (Join-P $script:Root 'rebuild-and-run.ps1')), '-Mode', $Mode)
            if ($timeout) { $args2 += @('-TimeoutSeconds', $timeout) }
            if ($BuildOnly) { $args2 += '-BuildOnly' }
            # Only the PowerShell wrapper is hidden; never hide the game window itself.
            if ($script:IsWin) { $startParams['WindowStyle'] = 'Hidden' }
            $proc = Start-Process -FilePath $ps -ArgumentList $args2 @startParams
            $null = $proc.Handle
            $kind = $(if ($BuildOnly) { 'build' } else { 'full' })
        }
    } finally {
        foreach ($k in $applied.Keys) { [Environment]::SetEnvironmentVariable($k, $applied[$k]) }
    }

    $script:Job = @{
        id = $id; kind = $kind; mode = $Mode; buildOnly = $BuildOnly; state = 'running'
        proc = $proc; outFile = $outFile; errFile = $errFile; outOff = [int64]0; errOff = [int64]0; outPart = ''; errPart = ''
        lines = (New-Object System.Collections.ArrayList); lineBase = 0; lineCount = 0
        errTail = (New-Object System.Collections.ArrayList)
        phase = $(if ($quick) { 'run' } else { 'prepare' }); phaseText = $(if ($quick) { 'Starting the game' } else { 'Checking your setup' })
        started = [DateTime]::UtcNow; gameStarted = $null; ended = $null; exitCode = $null; result = $null
        stopRequested = $false; uiHandled = $false
    }
    if ($quick) {
        # No script to announce itself: write a small manifest like the real one.
        Write-JsonFile (Join-P $logDir 'run.json') @{ schema_version = 1; run_id = $id; mode = $Mode; status = 'running'; started_utc = (Get-IsoNow); source = 'launcher-quick' }
    }
    Write-Log "Job $id started: kind=$kind mode=$Mode overrides=$($overrides.Count)"
}

function Stop-Job2 {
    if (-not (Test-JobRunning)) { return }
    $j = $script:Job
    $j.stopRequested = $true
    try {
        if ($script:IsWin) { & taskkill.exe /F /T /PID $j.proc.Id 2>&1 | Out-Null }
        else { Stop-Process -Id $j.proc.Id -Force -ErrorAction SilentlyContinue }
    } catch { Write-Log "Stop failed: $($_.Exception.Message)" }
}

function Get-JobPayload([int]$Since) {
    if (-not $script:Job) { return @{ state = 'idle' } }
    Update-Job
    $j = $script:Job
    $from = [Math]::Max($Since, $j.lineBase) - $j.lineBase
    $new = @()
    if ($from -lt $j.lines.Count) { $new = @($j.lines.GetRange($from, $j.lines.Count - $from).ToArray()) }
    $now = [DateTime]::UtcNow
    $end = $(if ($j.ended) { $j.ended } else { $now })
    $payload = [ordered]@{
        state = $j.state; id = $j.id; kind = $j.kind; mode = $j.mode; buildOnly = $j.buildOnly
        phase = $j.phase; phaseText = $j.phaseText
        elapsed = [int](($end - $j.started).TotalSeconds)
        sessionSeconds = $(if ($j.gameStarted) { [int](($end - $j.gameStarted).TotalSeconds) } else { 0 })
        gameRunning = [bool]($j.gameStarted -and $j.state -eq 'running')
        lines = $new; next = ($j.lineBase + $j.lines.Count)
        result = $j.result
    }
    return $payload
}

# ---------------------------------------------------------------- saves / backups / cleanup
function Get-BackupDir { return (Join-P (Join-P $script:Here 'backups') 'saves') }

# Earlier versions kept backups in <project>\backups\saves. Move them next to the launcher once.
function Move-LegacyBackups {
    try {
        $old = Join-P (Join-P $script:Root 'backups') 'saves'
        $new = Get-BackupDir
        if ((Test-Path -LiteralPath $old -PathType Container) -and -not (Test-Path -LiteralPath $new)) {
            New-Item -ItemType Directory -Force -Path (Split-Path -Parent $new) | Out-Null
            Move-Item -LiteralPath $old -Destination $new
            $left = Join-P $script:Root 'backups'
            if (-not (Get-ChildItem -LiteralPath $left -Force -ErrorAction SilentlyContinue)) { Remove-Item -LiteralPath $left -Force -ErrorAction SilentlyContinue }
            Write-Log 'Moved legacy save backups into launcher\backups.'
        }
    } catch { Write-Log "Legacy backup move failed: $($_.Exception.Message)" }
}
function Get-SavesDir { return (Join-P $script:Root 'hdd') }

function Test-BackupName([string]$Name) { return ($Name -match '^[A-Za-z0-9][A-Za-z0-9._-]{0,80}\.zip$') }

function Get-Backups {
    $dir = Get-BackupDir
    if (-not (Test-Path -LiteralPath $dir -PathType Container)) { return @() }
    $out = @()
    foreach ($f in (Get-ChildItem -LiteralPath $dir -File -Filter '*.zip' -ErrorAction SilentlyContinue | Sort-Object LastWriteTimeUtc -Descending)) {
        $kind = 'manual'
        if ($f.Name -like 'auto-*') { $kind = 'auto' } elseif ($f.Name -like 'prerestore-*') { $kind = 'safety' }
        $out += @{ name = $f.Name; bytes = [int64]$f.Length; modifiedUtc = (Get-IsoOf $f.LastWriteTimeUtc); kind = $kind }
    }
    return $out
}

function New-SaveBackup([string]$Prefix, [bool]$OnlyIfChanged) {
    $hdd = Get-SavesDir
    if (-not (Test-Path -LiteralPath $hdd -PathType Container)) { return $null }
    $files = @(Get-ChildItem -LiteralPath $hdd -Recurse -File -Force -ErrorAction SilentlyContinue)
    if ($files.Count -eq 0) { return $null }
    if ($OnlyIfChanged) {
        $newest = ($files | Sort-Object LastWriteTimeUtc -Descending | Select-Object -First 1).LastWriteTimeUtc
        $last = Get-Backups | Select-Object -First 1
        if ($last -and ([DateTime]::Parse($last.modifiedUtc).ToUniversalTime() -ge $newest)) { return $null }
    }
    $dir = Get-BackupDir
    New-Item -ItemType Directory -Force -Path $dir | Out-Null
    $name = '{0}-{1}.zip' -f $Prefix, (Get-Date -Format 'yyyyMMdd-HHmmss')
    $dest = Join-P $dir $name
    Compress-Archive -Path (Join-P $hdd '*') -DestinationPath $dest -CompressionLevel Optimal -Force
    Clear-Cache
    return $name
}

function Remove-OldBackups([int]$Keep) {
    $auto = @(Get-Backups | Where-Object { $_.kind -eq 'auto' })
    if ($auto.Count -le $Keep) { return }
    foreach ($b in ($auto | Select-Object -Skip $Keep)) {
        Remove-Item -LiteralPath (Join-P (Get-BackupDir) $b.name) -Force -ErrorAction SilentlyContinue
    }
}

function Restore-SaveBackup([string]$Name) {
    if (Test-JobRunning) { throw 'Close the game before restoring saves.' }
    if (-not (Test-BackupName $Name)) { throw 'Invalid backup name.' }
    $zip = Join-P (Get-BackupDir) $Name
    if (-not (Test-Path -LiteralPath $zip -PathType Leaf)) { throw 'That backup no longer exists.' }
    $safety = New-SaveBackup 'prerestore' $false
    $hdd = Get-SavesDir
    New-Item -ItemType Directory -Force -Path $hdd | Out-Null
    Get-ChildItem -LiteralPath $hdd -Force | Remove-Item -Recurse -Force
    Expand-Archive -LiteralPath $zip -DestinationPath $hdd -Force
    Clear-Cache
    return $safety
}

function Remove-SaveBackup([string]$Name) {
    if (-not (Test-BackupName $Name)) { throw 'Invalid backup name.' }
    $zip = Join-P (Get-BackupDir) $Name
    if (Test-Path -LiteralPath $zip -PathType Leaf) { Remove-Item -LiteralPath $zip -Force }
    Clear-Cache
}

function Clear-ShaderCache {
    if (Test-JobRunning) { throw 'Close the game before clearing the shader cache.' }
    $dir = Join-P $script:Root 'shader-cache'
    if (Test-Path -LiteralPath $dir -PathType Container) { Get-ChildItem -LiteralPath $dir -Force | Remove-Item -Recurse -Force }
    Clear-Cache
}

function Clear-OldLogs([int]$Keep) {
    if (Test-JobRunning) { throw 'Close the game before cleaning logs.' }
    # Ordinary run logs are what teach the launcher the fast-path shaders. Keep them until it has.
    if (-not (Test-Path -LiteralPath (Join-P $script:Root 'tools\fixtures\qpr2-literal-programs.json') -PathType Leaf)) {
        throw 'These logs are still being used to learn your shaders. Clean them after the fast-path shaders are ready.'
    }
    $dirs = @(Get-RunDirs)
    $removed = 0
    foreach ($d in ($dirs | Select-Object -Skip $Keep)) {
        $rid = $d.Name -replace '^(clean-v1|launcher)-', ''
        Remove-Item -LiteralPath $d.FullName -Recurse -Force -ErrorAction SilentlyContinue
        Remove-Item -LiteralPath (Join-P (Join-P $script:Root 'logs') ('qpr2-clean-v1-results-' + $rid + '.zip')) -Force -ErrorAction SilentlyContinue
        $removed++
    }
    Clear-Cache
    return $removed
}

function Open-Target([string]$Target, [string]$RunId) {
    $path = $null
    switch ($Target) {
        'root'    { $path = $script:Root }
        'game'    { $path = Join-P $script:Root 'game' }
        'saves'   { $path = Get-SavesDir }
        'backups' { $path = Get-BackupDir }
        'logs'    { $path = Join-P $script:Root 'logs' }
        'cache'   { $path = Join-P $script:Root 'shader-cache' }
        'art'     { $path = $script:AssetDir }
        'launcher' { $path = $script:Here }
        'run' {
            if ($RunId -notmatch '^(clean-v1|launcher)-[0-9A-Za-z-]+$') { throw 'Invalid run id.' }
            $path = Join-P (Join-P $script:Root 'logs') $RunId
        }
        default   { throw 'Unknown location.' }
    }
    if (-not (Test-Path -LiteralPath $path)) {
        if ($Target -in @('saves', 'backups', 'logs', 'cache', 'art', 'game')) { New-Item -ItemType Directory -Force -Path $path | Out-Null }
        else { throw 'That folder does not exist yet.' }
    }
    Write-Log "Open: $path"
    if ($script:IsWin) { Start-Process -FilePath 'explorer.exe' -ArgumentList (Q $path) }
}

function New-DesktopShortcut {
    if (-not $script:IsWin) { throw 'Shortcuts are only supported on Windows.' }
    $bat = Join-P $script:Root 'QPR2 Launcher.bat'
    if (-not (Test-Path -LiteralPath $bat)) { throw 'QPR2 Launcher.bat was not found in the project folder.' }
    $desktop = [Environment]::GetFolderPath('Desktop')
    $lnkPath = Join-P $desktop 'ATV Quad Power Racing 2.lnk'
    $ws = New-Object -ComObject WScript.Shell
    $lnk = $ws.CreateShortcut($lnkPath)
    $lnk.TargetPath = $bat
    $lnk.WorkingDirectory = $script:Root
    $lnk.WindowStyle = 7
    $lnk.Description = 'ATV: Quad Power Racing 2 recomp launcher'
    $lnk.Save()
    return $lnkPath
}

# ---------------------------------------------------------------- artwork
function Get-ArtFile([string]$Kind) {
    foreach ($ext in @('png', 'webp', 'jpg', 'jpeg')) {
        $p = Join-P $script:AssetDir ($Kind + '.' + $ext)
        if (Test-Path -LiteralPath $p -PathType Leaf) { return $p }
    }
    return $null
}

function Get-ImageExt([byte[]]$b) {
    if ($b.Length -gt 12) {
        if ($b[0] -eq 0x89 -and $b[1] -eq 0x50 -and $b[2] -eq 0x4E -and $b[3] -eq 0x47) { return 'png' }
        if ($b[0] -eq 0xFF -and $b[1] -eq 0xD8 -and $b[2] -eq 0xFF) { return 'jpg' }
        if ($b[0] -eq 0x52 -and $b[1] -eq 0x49 -and $b[2] -eq 0x46 -and $b[3] -eq 0x46 -and $b[8] -eq 0x57 -and $b[9] -eq 0x45 -and $b[10] -eq 0x42 -and $b[11] -eq 0x50) { return 'webp' }
    }
    return $null
}

function Update-Art([bool]$Force) {
    New-Item -ItemType Directory -Force -Path $script:AssetDir | Out-Null
    try { [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12 } catch { }
    $results = [ordered]@{}
    foreach ($kind in $script:ArtUrls.Keys) {
        if (-not $Force -and (Get-ArtFile $kind)) { $results[$kind] = 'kept'; continue }
        $tmp = Join-P $script:AssetDir ($kind + '.download')
        try {
            $hdr = @{ 'Accept' = 'image/*' }
            $uri = [Uri]$script:ArtUrls[$kind]
            $hdr['Referer'] = $uri.Scheme + '://' + $uri.Host + '/'
            Invoke-WebRequest -Uri $uri -OutFile $tmp -UseBasicParsing -TimeoutSec 20 -Headers $hdr -UserAgent 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 Chrome/124 Safari/537.36' | Out-Null
            $fi = Get-Item -LiteralPath $tmp
            if ($fi.Length -gt 15MB -or $fi.Length -lt 512) { throw 'Unexpected file size.' }
            $head = New-Object byte[] 16
            $fs = [System.IO.File]::OpenRead($tmp); try { [void]$fs.Read($head, 0, 16) } finally { $fs.Dispose() }
            $ext = Get-ImageExt $head
            if (-not $ext) { throw 'Not an image.' }
            foreach ($old in @('png', 'webp', 'jpg', 'jpeg')) { Remove-Item -LiteralPath (Join-P $script:AssetDir ($kind + '.' + $old)) -Force -ErrorAction SilentlyContinue }
            Move-Item -LiteralPath $tmp -Destination (Join-P $script:AssetDir ($kind + '.' + $ext)) -Force
            $results[$kind] = 'downloaded'
        } catch {
            Remove-Item -LiteralPath $tmp -Force -ErrorAction SilentlyContinue
            $results[$kind] = 'failed'
            Write-Log "Art '$kind' failed: $($_.Exception.Message)"
        }
    }
    $st = Get-LState
    $st['artTriedUtc'] = Get-IsoNow
    Save-LState $st
    return $results
}

# ---------------------------------------------------------------- state payload
function Get-FullState {
    $s = Get-Settings
    $d = Get-EffectiveDefaults
    $st = Get-LState
    $checks = @(Get-Checks)
    $blocking = @($checks | Where-Object { $_.status -eq 'bad' -and $_.id -in @('xbe', 'archive') })
    $firstRun = @($checks | Where-Object { $_.status -eq 'todo' -and $_.id -in @('build', 'lift') }).Count -gt 0
    $runs = @()
    foreach ($r in (Get-RunDirs $true | Select-Object -First 6)) { $runs += , (Read-RunSummary $r) }
    $sizes = Get-Cached 'sizes' 20 {
        @{
            saves   = (Get-DirSize (Get-SavesDir))
            cache   = (Get-DirSize (Join-P $script:Root 'shader-cache'))
            logs    = (Get-DirSize (Join-P $script:Root 'logs'))
            backups = (Get-DirSize (Get-BackupDir))
        }
    }
    $sh = Get-ShaderStatus
    $logoFile = Get-ArtFile 'logo'; $bgFile = Get-ArtFile 'background'
    $psd = Get-Psd
    $liveSchema = @{ schema_version = $script:Schema.schema_version; settings = @() }
    $detected = @(Get-Monitors)
    foreach ($def in $script:Schema.settings) {
        if ($def.ContainsKey('choices_from') -and $def.choices_from -eq 'monitors') {
            $copy = @{}; foreach ($k in $def.Keys) { $copy[$k] = $def[$k] }
            $choices = @(@{ value = ''; label = 'Auto' }) + $detected
            $cur = Get-SettingValue $s $def.id $d
            if ($cur -and -not ($detected | Where-Object { $_.value -eq $cur })) { $choices += @{ value = $cur; label = ('Display {0} (not connected)' -f $cur.Substring(7)) } }
            $copy['choices'] = $choices
            $liveSchema.settings += $copy
        } else { $liveSchema.settings += $def }
    }
    return [ordered]@{
        ok = $true
        app = @{
            version = $script:LauncherVersion; platform = $(if ($script:IsWin) { 'windows' } else { 'other' })
            titleId = '0x4143000A'; xbeSha256 = [string]$psd.XbeSha256
            root = $script:Root
        }
        schema = $liveSchema
        defaults = $d
        settings = $s
        checks = $checks
        canPlay = ($blocking.Count -eq 0)
        firstRun = $firstRun
        shaders = $sh
        runs = $runs
        stats = $st
        sizes = $sizes
        backups = @(Get-Backups)
        art = @{
            logo = [bool]$logoFile; background = [bool]$bgFile
            logoVersion = $(if ($logoFile) { (Get-Item -LiteralPath $logoFile).LastWriteTimeUtc.Ticks } else { 0 })
            backgroundVersion = $(if ($bgFile) { (Get-Item -LiteralPath $bgFile).LastWriteTimeUtc.Ticks } else { 0 })
            tried = [bool]$st.ContainsKey('artTriedUtc')
        }
        job = (Get-JobPayload 0)
    }
}

# ---------------------------------------------------------------- HTTP plumbing
$script:Token = $null
$script:StopServer = $false
$script:LastPing = [DateTime]::UtcNow
$script:SeenPing = $false

$script:Mime = @{
    '.html' = 'text/html; charset=utf-8'; '.css' = 'text/css; charset=utf-8'; '.js' = 'application/javascript; charset=utf-8'
    '.json' = 'application/json; charset=utf-8'; '.png' = 'image/png'; '.jpg' = 'image/jpeg'; '.jpeg' = 'image/jpeg'
    '.webp' = 'image/webp'; '.svg' = 'image/svg+xml'; '.ico' = 'image/x-icon'
}

function Send-Bytes($ctx, [int]$Status, [string]$ContentType, [byte[]]$Bytes, [hashtable]$Headers) {
    $res = $ctx.Response
    try {
        $res.StatusCode = $Status
        $res.ContentType = $ContentType
        $res.Headers.Add('Cache-Control', 'no-store')
        $res.Headers.Add('X-Content-Type-Options', 'nosniff')
        $res.Headers.Add('Referrer-Policy', 'no-referrer')
        if ($Headers) { foreach ($k in $Headers.Keys) { $res.Headers.Add($k, [string]$Headers[$k]) } }
        $res.ContentLength64 = $Bytes.Length
        $res.OutputStream.Write($Bytes, 0, $Bytes.Length)
    } catch { }
    finally { try { $res.OutputStream.Close() } catch { } }
}

function Send-Json($ctx, $Object, [int]$Status = 200) {
    $json = ConvertTo-Json -InputObject $Object -Depth 12 -Compress
    Send-Bytes $ctx $Status 'application/json; charset=utf-8' ([System.Text.Encoding]::UTF8.GetBytes($json)) $null
}

function Send-Text($ctx, [int]$Status, [string]$Text) {
    Send-Bytes $ctx $Status 'text/plain; charset=utf-8' ([System.Text.Encoding]::UTF8.GetBytes($Text)) $null
}

function Read-Body($ctx) {
    $sr = New-Object System.IO.StreamReader($ctx.Request.InputStream, [System.Text.Encoding]::UTF8)
    try { $raw = $sr.ReadToEnd() } finally { $sr.Dispose() }
    if ($raw.Trim().Length -eq 0) { return @{} }
    return (ConvertTo-Hashtable (ConvertFrom-Json $raw))
}

function Send-Static($ctx, [string]$Name) {
    $full = [System.IO.Path]::GetFullPath((Join-P $script:UiDir $Name))
    if (-not $full.StartsWith([System.IO.Path]::GetFullPath($script:UiDir), [StringComparison]::OrdinalIgnoreCase) -or -not (Test-Path -LiteralPath $full -PathType Leaf)) {
        Send-Text $ctx 404 'Not found'; return
    }
    $ext = [System.IO.Path]::GetExtension($full).ToLowerInvariant()
    $mime = $script:Mime[$ext]; if (-not $mime) { $mime = 'application/octet-stream' }
    Send-Bytes $ctx 200 $mime ([System.IO.File]::ReadAllBytes($full)) $null
}

function Test-Authorized($ctx, [int]$ServerPort) {
    $req = $ctx.Request
    $hostHdr = [string]$req.Headers['Host']
    if ($hostHdr -ne "127.0.0.1:$ServerPort" -and $hostHdr -ne "localhost:$ServerPort") { return $false }
    $cookie = $req.Cookies['qpr2']
    if (-not $cookie -or $cookie.Value -ne $script:Token) { return $false }
    if ($req.HttpMethod -eq 'POST') {
        if ([string]$req.Headers['X-QPR2'] -ne '1') { return $false }
        $origin = [string]$req.Headers['Origin']
        if ($origin -and $origin -ne "http://127.0.0.1:$ServerPort" -and $origin -ne "http://localhost:$ServerPort") { return $false }
    }
    return $true
}

function Invoke-Api($ctx, [string]$Path, [string]$Method) {
    $q = $ctx.Request.QueryString
    switch ("$Method $Path") {
        'GET /api/state'    { Send-Json $ctx (Get-FullState); return }
        'GET /api/job'      { $since = 0; [void][int]::TryParse([string]$q['since'], [ref]$since); Send-Json $ctx (Get-JobPayload $since); return }
        'POST /api/ping'    { $script:LastPing = [DateTime]::UtcNow; $script:SeenPing = $true; Send-Json $ctx @{ ok = $true; running = (Test-JobRunning) }; return }
        'POST /api/quit'    {
            Send-Json $ctx @{ ok = $true }
            Set-UiWindow 'close'
            $script:StopServer = $true
            return
        }
    }
    $body = $null
    if ($Method -eq 'POST') { $body = Read-Body $ctx }
    switch ("$Method $Path") {
        'POST /api/setting' {
            Set-SettingValue ([string]$body.id) ([string]$body.value)
            Clear-Cache
            Send-Json $ctx @{ ok = $true; settings = (Get-Settings) }
        }
        'POST /api/reset' {
            $scope = [string]$body.scope
            $s = Get-Settings
            if ($scope -eq 'all') { $s['values'] = @{}; $s['ui'] = @{} }
            else {
                foreach ($bag in @('values', 'ui')) {
                    foreach ($k in @($s[$bag].Keys)) { if ($script:Defs[$k].page -eq $scope) { $s[$bag].Remove($k) } }
                }
            }
            Save-Settings $s
            Clear-Cache
            Send-Json $ctx @{ ok = $true; settings = (Get-Settings) }
        }
        'POST /api/launch' {
            $mode = [string]$body.mode
            if (-not $mode) { $mode = Get-SettingValue (Get-Settings) 'play_mode' (Get-EffectiveDefaults) }
            Start-Job2 $mode $false
            Send-Json $ctx @{ ok = $true }
        }
        'POST /api/build' {
            $mode = Get-SettingValue (Get-Settings) 'play_mode' (Get-EffectiveDefaults)
            Start-Job2 $mode $true
            Send-Json $ctx @{ ok = $true }
        }
        'POST /api/stop' { Stop-Job2; Send-Json $ctx @{ ok = $true } }
        'POST /api/dismiss' { if ($script:Job -and $script:Job.state -eq 'finished') { $script:Job = $null }; Send-Json $ctx @{ ok = $true } }
        'POST /api/open' { Open-Target ([string]$body.target) ([string]$body.run); Send-Json $ctx @{ ok = $true } }
        'POST /api/saves/backup' {
            if (Test-JobRunning) { throw 'Close the game before backing up saves.' }
            $name = New-SaveBackup 'manual' $false
            if (-not $name) { throw 'There are no saves to back up yet.' }
            Send-Json $ctx @{ ok = $true; name = $name }
        }
        'POST /api/saves/restore' { $safety = Restore-SaveBackup ([string]$body.name); Send-Json $ctx @{ ok = $true; safety = $safety } }
        'POST /api/saves/delete'  { Remove-SaveBackup ([string]$body.name); Send-Json $ctx @{ ok = $true } }
        'POST /api/cache/clear' {
            if ([string]$body.what -eq 'shaders') { Clear-ShaderCache; Send-Json $ctx @{ ok = $true } }
            elseif ([string]$body.what -eq 'logs') { $n = Clear-OldLogs 5; Send-Json $ctx @{ ok = $true; removed = $n } }
            else { throw 'Unknown cache.' }
        }
        'POST /api/art/fetch' { $r = Update-Art ([bool]$body.force); Send-Json $ctx @{ ok = $true; results = $r } }
        'POST /api/shortcut'  { $p = New-DesktopShortcut; Send-Json $ctx @{ ok = $true; path = $p } }
        default { Send-Json $ctx @{ ok = $false; error = 'Unknown endpoint.' } 404 }
    }
}

function Invoke-Request($ctx, [int]$ServerPort) {
    $req = $ctx.Request
    $path = $req.Url.AbsolutePath
    $method = $req.HttpMethod

    # Entry point: the token arrives once in the URL and then lives in a cookie.
    if ($method -eq 'GET' -and ($path -eq '/' -or $path -eq '/index.html')) {
        $hostHdr = [string]$req.Headers['Host']
        if ($hostHdr -ne "127.0.0.1:$ServerPort" -and $hostHdr -ne "localhost:$ServerPort") { Send-Text $ctx 403 'Forbidden'; return }
        $given = [string]$req.QueryString['t']
        $cookie = $req.Cookies['qpr2']
        if ($given -ne $script:Token -and (-not $cookie -or $cookie.Value -ne $script:Token)) { Send-Text $ctx 403 'Open the launcher with QPR2 Launcher.bat.'; return }
        $full = Join-P $script:UiDir 'index.html'
        Send-Bytes $ctx 200 $script:Mime['.html'] ([System.IO.File]::ReadAllBytes($full)) @{ 'Set-Cookie' = "qpr2=$($script:Token); Path=/; HttpOnly; SameSite=Strict" }
        return
    }

    if (-not (Test-Authorized $ctx $ServerPort)) { Send-Text $ctx 403 'Forbidden'; return }

    if ($path.StartsWith('/api/')) {
        try { Invoke-Api $ctx $path $method }
        catch {
            Write-Log "API $method $path failed: $($_.Exception.Message)"
            Send-Json $ctx @{ ok = $false; error = $_.Exception.Message } 400
        }
        return
    }
    if ($path -eq '/assets/logo' -or $path -eq '/assets/background') {
        $f = Get-ArtFile ($path.Substring(8))
        if (-not $f) { Send-Text $ctx 404 'No artwork'; return }
        $mime = $script:Mime[[System.IO.Path]::GetExtension($f).ToLowerInvariant()]
        Send-Bytes $ctx 200 $mime ([System.IO.File]::ReadAllBytes($f)) $null
        return
    }
    if ($method -eq 'GET' -and $path -match '^/[A-Za-z0-9._-]+$') { Send-Static $ctx $path.Substring(1); return }
    Send-Text $ctx 404 'Not found'
}

function Get-FreePort {
    $l = New-Object System.Net.Sockets.TcpListener([System.Net.IPAddress]::Loopback, 0)
    $l.Start(); $p = ([System.Net.IPEndPoint]$l.LocalEndpoint).Port; $l.Stop()
    return $p
}

function New-Token {
    $b = New-Object byte[] 24
    $rng = [System.Security.Cryptography.RandomNumberGenerator]::Create()
    try { $rng.GetBytes($b) } finally { $rng.Dispose() }
    return (($b | ForEach-Object { $_.ToString('x2') }) -join '')
}

function Start-Ui([string]$Url) {
    $s = Get-Settings; $d = Get-EffectiveDefaults
    $full = (Get-SettingValue $s 'ui_start_fullscreen' $d) -eq '1'
    $browser = $null
    if ($script:IsWin) {
        $pf = $env:ProgramFiles; $pf86 = ${env:ProgramFiles(x86)}; $lad = $env:LOCALAPPDATA
        $cands = @(
            (Join-P $pf86 'Microsoft\Edge\Application\msedge.exe'), (Join-P $pf 'Microsoft\Edge\Application\msedge.exe'),
            (Join-P $pf 'Google\Chrome\Application\chrome.exe'), (Join-P $pf86 'Google\Chrome\Application\chrome.exe'),
            (Join-P $lad 'Google\Chrome\Application\chrome.exe')
        )
        foreach ($c in $cands) { if ($c -and (Test-Path -LiteralPath $c -PathType Leaf)) { $browser = $c; break } }
    }
    if ($browser) {
        # A separate browser instance must pick up the launcher compositor flags.
        # Keep its DirectComposition surfaces out of the fullscreen focus transition.
        $script:UiProfile = Join-P $script:ProfileDir ([guid]::NewGuid().ToString('N'))
        New-Item -ItemType Directory -Force -Path $script:UiProfile | Out-Null
        $a = @("--app=$Url", ('--user-data-dir=' + (Q $script:UiProfile)), '--no-first-run', '--no-default-browser-check',
            '--autoplay-policy=no-user-gesture-required', '--disable-features=Translate,msEdgeSidebarV2', '--window-size=1600,900',
            '--disable-direct-composition', '--disable-background-mode')
        if ($full) { $a += '--start-fullscreen' }
        $script:UiProcess = Start-Process -FilePath $browser -ArgumentList $a -PassThru
        Write-Log "UI opened with $browser"
    } else {
        Write-Log 'No Edge/Chrome found; using the default browser.'
        if ($script:IsWin) { Start-Process $Url } else { Write-Host "Open: $Url" }
    }
}

# ---------------------------------------------------------------- startup
try {
    Write-Log "Launcher $script:LauncherVersion starting (root: $script:Root)"
    New-Item -ItemType Directory -Force -Path $script:AssetDir | Out-Null
    Move-LegacyBackups

    # "Skip launcher" QoL: start the game at once unless Shift is held.
    $s0 = Get-Settings; $d0 = Get-EffectiveDefaults
    if (-not $NoSkip -and -not $NoBrowser -and (Get-SettingValue $s0 'ui_skip' $d0) -eq '1' -and -not (Test-ShiftHeld)) {
        $xi = Get-XbeInfo
        if ($xi.exists -and $xi.match) {
            Write-Log 'Skip-launcher: starting the game directly.'
            Invoke-AutoBackup $s0 $d0
            foreach ($kv in (Get-GameEnv $s0).GetEnumerator()) { [Environment]::SetEnvironmentVariable($kv.Key, [string]$kv.Value) }
            $mode = Get-SettingValue $s0 'play_mode' $d0
            $a = @('-NoProfile', '-ExecutionPolicy', 'Bypass', '-NoExit', '-File', (Q (Join-P $script:Root 'rebuild-and-run.ps1')), '-Mode', $mode)
            $t = Get-SettingValue $s0 'timeout' $d0; if ($t) { $a += @('-TimeoutSeconds', $t) }
            Start-Process -FilePath (Get-PowerShellExe) -ArgumentList $a -WorkingDirectory $script:Root | Out-Null
            return
        }
    }

    $script:Token = New-Token
    if ($Port -le 0) { $Port = Get-FreePort }
    $listener = New-Object System.Net.HttpListener
    $listener.Prefixes.Add("http://127.0.0.1:$Port/")
    $listener.Start()
    $url = "http://127.0.0.1:$Port/?t=$($script:Token)"
    Write-Log "Serving on port $Port"
    if ($NoBrowser) { Write-Host "QPR2_LAUNCHER_URL=$url" } else { Start-Ui $url }

    $script:LastPing = [DateTime]::UtcNow
    while ($listener.IsListening -and -not $script:StopServer) {
        $task = $listener.GetContextAsync()
        while (-not $task.AsyncWaitHandle.WaitOne(400)) {
            if (Test-JobRunning) { Update-Job }
            if ($script:StopServer) { break }
            if (-not (Test-JobRunning)) {
                $uiClosed = $script:UiProcess -and $script:UiProcess.HasExited
                $pingExpired = -not $script:UiProcess -and $script:SeenPing -and
                    (([DateTime]::UtcNow - $script:LastPing).TotalSeconds -gt 45)
                if ($uiClosed -or $pingExpired) {
                    Write-Log 'Launcher window closed; shutting down.'
                    $script:StopServer = $true
                    break
                }
            }
        }
        if ($script:StopServer -and -not $task.IsCompleted) { break }
        $ctx = $task.GetAwaiter().GetResult()
        try { Invoke-Request $ctx $Port }
        catch { Write-Log "Request error: $($_.Exception.Message)"; try { $ctx.Response.Abort() } catch { } }
    }
} catch {
    Write-Log "FATAL: $($_.Exception.Message)"
    if ($script:IsWin -and -not $Console) {
        try {
            Add-Type -AssemblyName System.Windows.Forms
            [void][System.Windows.Forms.MessageBox]::Show("The launcher could not start:`n`n$($_.Exception.Message)`n`nDetails: launcher\launcher.log", 'QPR2 Launcher', 'OK', 'Error')
        } catch { }
    } else { Write-Host "FATAL: $($_.Exception.Message)" }
} finally {
    try { if ($listener) { $listener.Stop(); $listener.Close() } } catch { }
    if ($script:UiProfile) {
        try {
            Set-UiWindow 'close'
            if (-not $script:UiProcess -or $script:UiProcess.WaitForExit(2000)) {
                Remove-Item -LiteralPath $script:UiProfile -Recurse -Force -ErrorAction SilentlyContinue
            }
        } catch { Write-Log "UI cleanup failed: $($_.Exception.Message)" }
    }
    if (Test-JobRunning) { Write-Log 'Launcher exiting while a job is running; leaving it alone.' }
    Write-Log 'Launcher stopped.'
}
