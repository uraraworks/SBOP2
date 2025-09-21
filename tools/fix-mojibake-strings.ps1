param(
  [string]$Root = 'SboCli/src/Dialog'
)

[System.Text.Encoding]::RegisterProvider([System.Text.CodePagesEncodingProvider]::Instance)
$utf8Bom = [System.Text.UTF8Encoding]::new($true)
$enc1252 = [System.Text.Encoding]::GetEncoding(1252)
$enc932  = [System.Text.Encoding]::GetEncoding(932)

# Rough matcher for C/C++ string literals (no multiline support, simple escapes)
$rx = [regex]'"([^"\\]|\\.)*"'

function Should-Fix([string]$s) {
  # Heuristic: contains characters typical of CP1252 moji-bake (e.g., ƒ, –, —, …, “ ” ‘ ’, •, ™, €)
  return ($s -match '[\u0080-\u00FF\u0192\u2013\u2014\u2018\u2019\u201C\u201D\u2026\u2022\u2122\u20AC]')
}

function Fix-String([string]$s) {
  try {
    $bytes = $enc1252.GetBytes($s)
    $cand  = $enc932.GetString($bytes)
  } catch {
    return $s
  }
  # Accept only if it seems to produce Japanese Hiragana/Katakana/CJK
  if ($cand -match '[\u3040-\u30FF\u3400-\u9FFF]') { return $cand }
  return $s
}

$files = Get-ChildItem -Path $Root -Recurse -Include *.cpp,*.h | Where-Object { -not ($_.FullName -match '\\(Debug|Release|ipch|\.git|\.vs)\\') }

foreach ($f in $files) {
  $text = [System.IO.File]::ReadAllText($f.FullName, $utf8Bom)
  $changed = $false
  $text2 = $rx.Replace($text, {
      param($m)
      $lit = $m.Value
      $inner = $lit.Substring(1, $lit.Length - 2)
      if (Should-Fix $inner) {
        $fixed = Fix-String $inner
        if ($fixed -ne $inner) {
          $script:changed = $true
          return '"' + $fixed + '"'
        }
      }
      return $lit
    })
  if ($changed) {
    [System.IO.File]::WriteAllText($f.FullName, $text2, $utf8Bom)
    Write-Output ("Fixed: " + $f.FullName)
  }
}

Write-Output "Done."

