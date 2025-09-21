param(
  [switch]$DryRun
)

# Extensions to process
$extensions = @(
  'h','hpp','hh','hxx','inl',
  'c','cpp','cc','cxx',
  'rc','rc2','def','idl','mc','rcv',
  'txt'
)

$excludePattern = '(\\\.git\\|\\\.vs\\|\\ipch\\|\\Release\\|\\Debug\\)'

$utf8Strict = [System.Text.UTF8Encoding]::new($false, $true)
$utf8Bom = [System.Text.UTF8Encoding]::new($true)
$sjisStrict = [System.Text.Encoding]::GetEncoding(932, [System.Text.EncoderFallback]::ExceptionFallback, [System.Text.DecoderFallback]::ExceptionFallback)

function Convert-File([string]$path) {
  try {
    [byte[]]$bytes = [System.IO.File]::ReadAllBytes($path)
  } catch {
    Write-Warning "Read failed: $path - $_"
    return
  }

  if ($bytes.Length -eq 0) { return }

  # Skip likely binary files (contains NUL)
  if ($bytes.Contains([byte]0)) { return }

  # Already UTF-8 BOM?
  if ($bytes.Length -ge 3 -and $bytes[0] -eq 0xEF -and $bytes[1] -eq 0xBB -and $bytes[2] -eq 0xBF) {
    # Normalize line endings to CRLF only if LF-only detected
    try {
      $text = [System.Text.Encoding]::UTF8.GetString($bytes, 3, $bytes.Length - 3)
      if ($text -match "(?<!\r)\n") {
        $normalized = ($text -replace "\r?\n", "`r`n")
        if (-not $DryRun) { [System.IO.File]::WriteAllText($path, $normalized, $utf8Bom) }
        Write-Output "Normalized (UTF8+BOM): $path"
      }
    } catch {}
    return
  }

  $decoded = $null
  $from = $null
  try {
    $decoded = $utf8Strict.GetString($bytes)
    $from = 'UTF8'
  } catch {
    try {
      $decoded = $sjisStrict.GetString($bytes)
      $from = 'CP932'
    } catch {
      Write-Output "Skipped (unknown encoding): $path"
      return
    }
  }

  # Normalize to CRLF
  $normalized = $decoded -replace "\r?\n", "`r`n"

  if (-not $DryRun) {
    try {
      [System.IO.File]::WriteAllText($path, $normalized, $utf8Bom)
      Write-Output "Converted $from -> UTF8+BOM: $path"
    } catch {
      Write-Warning "Write failed: $path - $_"
    }
  } else {
    Write-Output "[DryRun] Would convert $from -> UTF8+BOM: $path"
  }
}

$files = Get-ChildItem -Path . -Recurse -File |
  Where-Object { $_.FullName -notmatch $excludePattern } |
  Where-Object { $extensions -contains $_.Extension.TrimStart('.') }

foreach ($f in $files) {
  Convert-File -path $f.FullName
}

Write-Output "Done. Processed $($files.Count) files."

