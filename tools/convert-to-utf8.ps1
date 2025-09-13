param(
    [string]$Root = (Get-Location).Path,
    [switch]$Utf8Bom,
    [switch]$WhatIf
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

# Register code pages (for Shift-JIS, EUC-JP, etc.)
[System.Text.Encoding]::RegisterProvider([System.Text.CodePagesEncodingProvider]::Instance) | Out-Null

function New-ThrowingEncoding {
    param(
        [Parameter(Mandatory=$true)][int]$CodePage
    )
    $decoderFallback = New-Object System.Text.DecoderExceptionFallback
    $encoderFallback = New-Object System.Text.EncoderExceptionFallback
    return [System.Text.Encoding]::GetEncoding($CodePage, $encoderFallback, $decoderFallback)
}

function Test-IsBinary {
    param([byte[]]$Bytes)
    if (-not $Bytes -or $Bytes.Length -eq 0) { return $false }
    # If contains NUL and not UTF-16/32 BOM, treat as binary
    $hasNull = ([Array]::IndexOf($Bytes, [byte]0) -ge 0)
    if ($hasNull) { return $true }
    # Heuristic: too many control chars
    $ctrl = 0
    foreach ($b in $Bytes) {
        if ($b -lt 0x09 -or ($b -ge 0x0E -and $b -le 0x1F)) { $ctrl++ }
    }
    return ($ctrl / [double]$Bytes.Length) -gt 0.3
}

function Get-EncodingInfo {
    param([string]$Path)
    $fs = [System.IO.File]::Open($Path, [System.IO.FileMode]::Open, [System.IO.FileAccess]::Read, [System.IO.FileShare]::Read)
    try {
        $len = [Math]::Min(4096, [int]$fs.Length)
        $bytes = New-Object byte[] $len
        [void]$fs.Read($bytes, 0, $len)

        if ($len -ge 3 -and $bytes[0] -eq 0xEF -and $bytes[1] -eq 0xBB -and $bytes[2] -eq 0xBF) {
            return @{ Name = 'UTF-8 BOM'; Encoding = New-Object System.Text.UTF8Encoding($true); Bom = $true; Bytes=$bytes }
        }
        if ($len -ge 2 -and $bytes[0] -eq 0xFF -and $bytes[1] -eq 0xFE) { return @{ Name='UTF-16 LE'; Encoding=[System.Text.Encoding]::Unicode; Bom=$true; Bytes=$bytes } }
        if ($len -ge 2 -and $bytes[0] -eq 0xFE -and $bytes[1] -eq 0xFF) { return @{ Name='UTF-16 BE'; Encoding=[System.Text.Encoding]::BigEndianUnicode; Bom=$true; Bytes=$bytes } }
        if ($len -ge 4 -and $bytes[0] -eq 0xFF -and $bytes[1] -eq 0xFE -and $bytes[2] -eq 0x00 -and $bytes[3] -eq 0x00) { return @{ Name='UTF-32 LE'; Encoding=[System.Text.Encoding]::UTF32; Bom=$true; Bytes=$bytes } }
        if ($len -ge 4 -and $bytes[0] -eq 0x00 -and $bytes[1] -eq 0x00 -and $bytes[2] -eq 0xFE -and $bytes[3] -eq 0xFF) { return @{ Name='UTF-32 BE'; Encoding=[System.Text.Encoding]::GetEncoding(12001); Bom=$true; Bytes=$bytes } }

        if (Test-IsBinary -Bytes $bytes) {
            return @{ Name='Binary?'; Encoding=$null; Bom=$false; Bytes=$bytes }
        }

        # Try strict UTF-8
        try {
            $utf8Strict = New-Object System.Text.UTF8Encoding($false, $true)
            [void]$utf8Strict.GetString($bytes)
            return @{ Name='UTF-8'; Encoding=$utf8Strict; Bom=$false; Bytes=$bytes }
        } catch {}

        # Try common Japanese encodings
        foreach ($cp in 932, 51932, 50220, 20932, 65001) { # Shift-JIS, EUC-JP, ISO-2022-JP, EUC-JP (alt), UTF-8
            try {
                $enc = New-ThrowingEncoding -CodePage $cp
                [void]$enc.GetString($bytes)
                $name = switch ($cp) { 932 {'Shift-JIS'} 51932 {'EUC-JP'} 50220 {'ISO-2022-JP'} 20932 {'EUC-JP(20932)'} 65001 {'UTF-8'} default {"CP$cp"} }
                return @{ Name=$name; Encoding=$enc; Bom=$false; Bytes=$bytes }
            } catch {}
        }

        # Fallback to Windows-1252 strict
        try {
            $win1252 = New-ThrowingEncoding -CodePage 1252
            [void]$win1252.GetString($bytes)
            return @{ Name='Windows-1252'; Encoding=$win1252; Bom=$false; Bytes=$bytes }
        } catch {}

        return @{ Name='Unknown'; Encoding=$null; Bom=$false; Bytes=$bytes }
    } finally {
        $fs.Dispose()
    }
}

function Should-ProcessFile {
    param([string]$Path)
    $ext = [System.IO.Path]::GetExtension($Path).ToLowerInvariant()
    $binaryExts = @(
        '.png','.jpg','.jpeg','.gif','.bmp','.ico','.svgz',
        '.pdf','.zip','.7z','.rar','.gz','.bz2','.xz','.tar','.iso',
        '.dll','.exe','.pdb','.lib','.so','.dylib','.a','.class','.jar',
        '.ttf','.otf','.woff','.woff2','.eot',
        '.mp3','.mp4','.avi','.mov','.mkv','.wav','.flac','.ogg',
        '.psd','.ai','.sketch','.blend','.fbx','.glb','.gltf',
        '.bin'
    )
    if ($binaryExts -contains $ext) { return $false }
    # Skip VCS directories and built artifacts
    if ($Path -match "\\\.git\\|\\node_modules\\|\\bin\\|\\obj\\|\\.vs\\|\\dist\\|\\build\\") { return $false }
    return $true
}

$results = [System.Collections.Generic.List[object]]::new()

$files = Get-ChildItem -Path $Root -Recurse -File -Force | Where-Object { Should-ProcessFile $_.FullName }
foreach ($f in $files) {
    try {
        $info = Get-EncodingInfo -Path $f.FullName
        $action = 'skip'
        $from = $info.Name
        $to = if ($Utf8Bom) { 'UTF-8 BOM' } else { 'UTF-8' }

        if ($info.Name -eq 'Binary?') {
            $action = 'skip-binary'
        } elseif ($info.Name -eq 'Unknown' -or $null -eq $info.Encoding) {
            $action = 'skip-unknown'
        } else {
            $needs = $true
            if (-not $Utf8Bom -and $info.Name -eq 'UTF-8' -and -not $info.Bom) { $needs = $false }
            if ($Utf8Bom -and $info.Name -eq 'UTF-8 BOM') { $needs = $false }
            if ($needs) {
                # Try reading full file with a non-throwing decoder for robustness
                $readOk = $false
                try {
                    $encToUse = if ($info.Name -eq 'UTF-8' -and -not $info.Bom) { New-Object System.Text.UTF8Encoding($false, $false) } else { [System.Text.Encoding]::GetEncoding($info.Encoding.CodePage) }
                    $text = [System.IO.File]::ReadAllText($f.FullName, $encToUse)
                    $readOk = $true
                } catch {}
                if (-not $readOk) {
                    foreach ($cp in 65001, 932, 51932, 50220, 1252) {
                        try {
                            $encAlt = [System.Text.Encoding]::GetEncoding($cp)
                            $text = [System.IO.File]::ReadAllText($f.FullName, $encAlt)
                            $readOk = $true; break
                        } catch {}
                    }
                }
                if (-not $readOk) { throw "Failed to read with candidate encodings" }
                # Write with desired UTF-8 variant
                $utf8 = New-Object System.Text.UTF8Encoding($Utf8Bom.IsPresent)
                if ($WhatIf) {
                    $action = 'would-convert'
                } else {
                    # Backup
                    $bak = "$($f.FullName).bak-$(($from -replace '[^A-Za-z0-9\-]+','_').ToLower())"
                    if (-not (Test-Path $bak)) { [System.IO.File]::Copy($f.FullName, $bak, $false) }
                    [System.IO.File]::WriteAllText($f.FullName, $text, $utf8)
                    $action = 'converted'
                }
            } else {
                $action = 'already-utf8'
            }
        }

        $results.Add([pscustomobject]@{
            Path   = $f.FullName
            From   = $from
            To     = $to
            Action = $action
        }) | Out-Null
    } catch {
        $results.Add([pscustomobject]@{
            Path   = $f.FullName
            From   = 'error'
            To     = 'utf-8'
            Action = "error: $($_.Exception.Message)"
        }) | Out-Null
    }
}

# Summary
$converted = $results | Where-Object { $_.Action -eq 'converted' }
$skipped   = $results | Where-Object { $_.Action -like 'skip*' -or $_.Action -eq 'already-utf8' }
$errors    = $results | Where-Object { $_.Action -like 'error*' }

"Converted: $($converted.Count)  Skipped: $($skipped.Count)  Errors: $($errors.Count)" | Write-Host

$logPath = Join-Path $Root 'tools/convert-to-utf8.csv'
$results | Sort-Object Action, Path | Export-Csv -NoTypeInformation -Encoding utf8 -Path $logPath
Write-Host "Detailed log (CSV): $logPath"

exit 0
