# Launcher soundtrack discovery and playback from local game assets.

function Get-MusicMeta {
    $meta = @{}
    $path = Join-P $script:Here 'music/tracks.xml'
    if (-not (Test-Path -LiteralPath $path -PathType Leaf)) { return $meta }

    $reader = $null
    try {
        $settings = New-Object System.Xml.XmlReaderSettings
        $settings.DtdProcessing = [System.Xml.DtdProcessing]::Prohibit
        $settings.XmlResolver = $null
        $settings.MaxCharactersInDocument = 65536
        $reader = [System.Xml.XmlReader]::Create($path, $settings)
        $doc = New-Object System.Xml.XmlDocument
        $doc.XmlResolver = $null
        $doc.Load($reader)
        foreach ($song in $doc.SelectNodes('/soundtrack/song')) {
            $number = $song.GetAttribute('number')
            if ($number -notmatch '^[0-9]{1,3}$' -or [int]$number -eq 0) { continue }
            $n = [int]$number
            if ($meta.ContainsKey($n)) { continue }
            $title = $song.SelectSingleNode('title')
            $artist = $song.SelectSingleNode('artist')
            $meta[$n] = @{
                title = if ($title) { $title.InnerText.Trim() } else { '' }
                artist = if ($artist) { $artist.InnerText.Trim() } else { '' }
            }
        }
    } catch { Write-Log "tracks.xml unreadable: $($_.Exception.Message)" }
    finally { if ($reader) { $reader.Dispose() } }
    return $meta
}

function Open-MusicArchive([string]$Path) {
    Add-Type -AssemblyName System.IO.Compression.FileSystem
    return [System.IO.Compression.ZipFile]::OpenRead($Path)
}

function Get-MusicSources {
    return (Get-Cached 'music-sources' 10 {
        $sources = @{}
        $game = Join-P $script:Root 'game'
        foreach ($dir in @((Join-P $game 'Licensed'), (Join-P $game 'ATV/Licensed'))) {
            if (-not (Test-Path -LiteralPath $dir -PathType Container)) { continue }
            foreach ($file in (Get-ChildItem -LiteralPath $dir -File -ErrorAction SilentlyContinue | Sort-Object Name)) {
                if (($file.Attributes -band [System.IO.FileAttributes]::ReparsePoint) -or
                    $file.Name -notmatch '^Track([0-9]{1,3})\.wav$') { continue }
                $n = [int]$Matches[1]
                if ($n -gt 0 -and -not $sources.ContainsKey($n)) {
                    $sources[$n] = @{ path = $file.FullName; bytes = $file.Length }
                }
            }
        }

        $path = Join-P $game 'ATV.zip'
        if (Test-Path -LiteralPath $path -PathType Leaf) {
            $archive = $null
            try {
                $archive = Open-MusicArchive $path
                foreach ($entry in $archive.Entries) {
                    if ($entry.FullName -notmatch '^Licensed[/\\]Track([0-9]{1,3})\.wav$') { continue }
                    $n = [int]$Matches[1]
                    if ($n -gt 0 -and -not $sources.ContainsKey($n)) {
                        $sources[$n] = @{ path = $path; entry = $entry.FullName; bytes = $entry.Length }
                    }
                }
            } catch { Write-Log "Could not read soundtrack from ATV.zip: $($_.Exception.Message)" }
            finally { if ($archive) { $archive.Dispose() } }
        }
        $sources
    })
}

function Get-MusicState {
    $sources = Get-MusicSources
    $meta = Get-MusicMeta
    $tracks = @(
        foreach ($n in ($sources.Keys | Sort-Object)) {
            $song = $meta[$n]
            @{
                n = $n
                title = if ($song.title) { $song.title } else { "Track $n" }
                artist = [string]$song.artist
                bytes = $sources[$n].bytes
            }
        }
    )
    return @{ tracks = $tracks }
}

function Send-MusicTrack($ctx, [int]$Number) {
    $source = (Get-MusicSources)[$Number]
    if (-not $source) { Send-Text $ctx 404 'No such track'; return }

    $res = $ctx.Response
    $archive = $stream = $null
    try {
        if ($source.entry) {
            $archive = Open-MusicArchive $source.path
            $entry = $archive.GetEntry($source.entry)
            if (-not $entry) { Send-Text $ctx 404 'No such track'; return }
            $length = $entry.Length
            $stream = $entry.Open()
        } else {
            $stream = [System.IO.File]::OpenRead($source.path)
            $length = $stream.Length
        }
    } catch {
        Write-Log "Could not open soundtrack track ${Number}: $($_.Exception.Message)"
        if ($stream) { $stream.Dispose() }
        if ($archive) { $archive.Dispose() }
        Send-Text $ctx 404 'Track unavailable'
        return
    }

    try {
        $res.StatusCode = 200
        $res.ContentType = 'audio/wav'
        $res.Headers.Add('Cache-Control', 'no-store')
        $res.Headers.Add('X-Content-Type-Options', 'nosniff')
        $res.ContentLength64 = $length
        $stream.CopyTo($res.OutputStream)
    } catch { Write-Log "Music stream ended early: $($_.Exception.Message)" }
    finally {
        $stream.Dispose()
        if ($archive) { $archive.Dispose() }
        try { $res.OutputStream.Close() } catch { }
    }
}
