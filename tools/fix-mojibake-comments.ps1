param(
  [string]$Root = 'SboCli/src'
)

[System.Text.Encoding]::RegisterProvider([System.Text.CodePagesEncodingProvider]::Instance)
$utf8Bom = [System.Text.UTF8Encoding]::new($true)
$enc1252 = [System.Text.Encoding]::GetEncoding(1252, [System.Text.EncoderFallback]::ExceptionFallback, [System.Text.DecoderFallback]::ExceptionFallback)
$enc932  = [System.Text.Encoding]::GetEncoding(932,  [System.Text.EncoderFallback]::ExceptionFallback, [System.Text.DecoderFallback]::ExceptionFallback)

function Should-Fix([string]$s) {
  # contains typical CP1252/UCN mojibake characters likely from mis-decoding
  return ($s -match "[\u0080-\u00FF\u0192\u2013\u2014\u2018\u2019\u201C\u201D\u2026\u2022\u2122\u20AC]")
}

function Fix-Text([string]$s) {
  try {
    $bytes = $enc1252.GetBytes($s)
    $cand  = $enc932.GetString($bytes)
  } catch {
    return $s
  }
  if ($cand -match "[\u3040-\u30FF\u3400-\u9FFF]") { return $cand }
  return $s
}

function Fix-Comments-In-Text([string]$text) {
  $sb = [System.Text.StringBuilder]::new()
  $len = $text.Length
  $i = 0
  $inStr = $false
  $inChar = $false
  $inLine = $false
  $inBlock = $false
  $escape = $false

  while ($i -lt $len) {
    $ch = $text[$i]

    if ($inStr) {
      $null = $sb.Append($ch)
      if ($escape) { $escape = $false }
      elseif ($ch -eq '\\') { $escape = $true }
      elseif ($ch -eq '"') { $inStr = $false }
      $i++
      continue
    }
    if ($inChar) {
      $null = $sb.Append($ch)
      if ($escape) { $escape = $false }
      elseif ($ch -eq '\\') { $escape = $true }
      elseif ($ch -eq [char]39) { $inChar = $false }
      $i++
      continue
    }
    if ($inLine) {
      # collect until newline
      $start = $i
      while ($i -lt $len -and $text[$i] -ne "`n" -and $text[$i] -ne "`r") { $i++ }
      $segment = $text.Substring($start, $i - $start)
      if (Should-Fix $segment) { $segment = Fix-Text $segment }
      $null = $sb.Append($segment)
      $inLine = $false
      continue
    }
    if ($inBlock) {
      # find closing */
      $start = $i
      $end = $text.IndexOf('*/', $start)
      if ($end -lt 0) { $end = $len }
      $segment = $text.Substring($start, $end - $start)
      if (Should-Fix $segment) { $segment = Fix-Text $segment }
      $null = $sb.Append($segment)
      if ($end -lt $len) {
        $null = $sb.Append('*/')
        $i = $end + 2
        $inBlock = $false
      } else {
        $i = $len
      }
      continue
    }

    # normal code
    if ($ch -eq '/' -and ($i + 1) -lt $len) {
      $nxt = $text[$i+1]
      if ($nxt -eq '/') {
        $null = $sb.Append('//')
        $i += 2
        $inLine = $true
        continue
      } elseif ($nxt -eq '*') {
        $null = $sb.Append('/*')
        $i += 2
        $inBlock = $true
        continue
      }
    }
    elseif ($ch -eq '"') { $inStr = $true; $null = $sb.Append($ch); $i++; continue }
    elseif ($ch -eq [char]39) { $inChar = $true; $null = $sb.Append($ch); $i++; continue }

    $null = $sb.Append($ch)
    $i++
  }
  return $sb.ToString()
}

$files = Get-ChildItem -Path $Root -Recurse -Include *.cpp,*.c,*.h,*.hpp,*.rc,*.rc2 | Where-Object { -not ($_.FullName -match '\\(Debug|Release|ipch|\.git|\.vs)\\') }

foreach ($f in $files) {
  $orig = [System.IO.File]::ReadAllText($f.FullName, $utf8Bom)
  $fixed = Fix-Comments-In-Text $orig
  if ($fixed -ne $orig) {
    [System.IO.File]::WriteAllText($f.FullName, $fixed, $utf8Bom)
    Write-Output ("Fixed comments: " + $f.FullName)
  }
}

Write-Output 'Done.'
