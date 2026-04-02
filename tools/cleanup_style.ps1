# コードスタイル整理スクリプト (PowerShell版)
# 対象: SboCli/src/Info/, SboCli/src/LibInfo/, SboCli/src/AdminApi/

$BasePath = "C:\Users\masakazu\MyProject\SBOP2\SboCli\src"

$ErrorActionPreference = "Stop"

# 制御構文キーワード（空白を保持する）
$script:ControlKeywords = @(
    "if", "for", "while", "switch", "return", "case", "catch",
    "else", "do", "try", "sizeof", "new", "delete", "throw",
    "__if_exists", "__if_not_exists", "static_assert"
)

function ConvertFileHeader($Lines) {
    if ($Lines.Count -eq 0) { return , $Lines }

    $firstLine = $Lines[0].TrimEnd("`r`n")
    if ($firstLine -notmatch '^/\* (Copyright\(C\).+?) \*/') { return , $Lines }
    $copyrightText = $Matches[1]

    if ($Lines.Count -lt 2) { return , $Lines }
    if ($Lines[1].TrimEnd("`r`n") -notmatch '^/\* =+') { return , $Lines }

    $fileName = $null; $content = $null; $author = $null; $date = $null
    $i = 2

    while ($i -lt $Lines.Count) {
        $line = $Lines[$i].TrimEnd("`r`n")
        if ($line -match '^/\* =+\s*\*/') { $i++; break }
        if ($line -match '^/\*\s*ファイル名\s*:(.+?)\s*\*/') { $fileName = $Matches[1].Trim() }
        if ($line -match '^/\*\s*内容\s*:(.+?)\s*\*/') { $content = $Matches[1].Trim() }
        if ($line -match '^/\*\s*作成\s*:(.+?)\s*\*/') { $author = $Matches[1].Trim() }
        if ($line -match '^/\*\s*作成開始日\s*:(.+?)\s*\*/') { $date = $Matches[1].Trim() }
        $i++
    }

    $newHeader = [System.Collections.Generic.List[string]]::new()
    if ($fileName) { $newHeader.Add("/// @file $fileName`n") }
    if ($content)  { $newHeader.Add("/// @brief $content`n") }
    if ($author)   { $newHeader.Add("/// @author $author`n") }
    if ($date)     { $newHeader.Add("/// @date $date`n") }
    $newHeader.Add("/// @copyright $copyrightText`n")

    $result = [System.Collections.Generic.List[string]]::new()
    $result.AddRange([string[]]$newHeader)
    for ($j = $i; $j -lt $Lines.Count; $j++) { $result.Add($Lines[$j]) }
    return , $result.ToArray()
}

function RemoveFooterCopyright($Lines) {
    $result = [System.Collections.Generic.List[string]]::new()
    foreach ($line in $Lines) {
        if ($line.TrimEnd("`r`n") -match '^/\* Copyright\(C\).+\*/$') { continue }
        $result.Add($line)
    }
    while ($result.Count -gt 1 -and $result[$result.Count-1].Trim() -eq '' -and $result[$result.Count-2].Trim() -eq '') {
        $result.RemoveAt($result.Count-1)
    }
    if ($result.Count -eq 0 -or $result[$result.Count-1].Trim() -ne '') { $result.Add("`n") }
    return , $result.ToArray()
}

function RemoveFunctionHeaders($Lines) {
    $result = [System.Collections.Generic.List[string]]::new()
    $i = 0

    while ($i -lt $Lines.Count) {
        $line = $Lines[$i].TrimEnd("`r`n")

        if ($line -match '^/\* =+') {
            $blockLines = [System.Collections.Generic.List[string]]::new()
            $blockLines.Add($line)
            $j = $i + 1
            $foundClose = $false

            while ($j -lt $Lines.Count) {
                $bl = $Lines[$j].TrimEnd("`r`n")
                $blockLines.Add($bl)
                if ($bl -match '^/\* =+\s*\*/') { $foundClose = $true; $j++; break }
                $j++
            }

            if (-not $foundClose) { $result.Add($Lines[$i]); $i++; continue }

            $isFuncHeader = $false
            foreach ($bl in $blockLines) {
                if ($bl -match '^/\*\s*(関数名|内容|日付|戻り値)\s*:') { $isFuncHeader = $true; break }
            }

            if ($isFuncHeader) {
                while ($result.Count -gt 0 -and $result[$result.Count-1].Trim() -eq '') {
                    $result.RemoveAt($result.Count-1)
                }
                $result.Add("`n")
                $i = $j
            } else {
                $result.Add($Lines[$i]); $i++
            }
        } else {
            $result.Add($Lines[$i]); $i++
        }
    }

    return , $result.ToArray()
}

function FixFunctionCallSpaces($Lines) {
    $result = [System.Collections.Generic.List[string]]::new()
    $kwSet = [System.Collections.Generic.HashSet[string]]::new([string[]]$script:ControlKeywords, [System.StringComparer]::Ordinal)

    foreach ($line in $Lines) {
        $stripped = $line.TrimEnd("`r`n")
        $eol = if ($line.EndsWith("`n")) { "`n" } else { "" }

        # // コメント行はそのまま
        if ($stripped -match '^\s*//') { $result.Add($line); continue }

        # 識別子 + スペース + ( を置換
        $matches2 = [regex]::Matches($stripped, '([A-Za-z_][A-Za-z0-9_]*) \(')
        if ($matches2.Count -gt 0) {
            $newLine = $stripped
            # 後ろから置換（位置がずれないように）
            for ($mi = $matches2.Count - 1; $mi -ge 0; $mi--) {
                $m = $matches2[$mi]
                $name = $m.Groups[1].Value
                if (-not $kwSet.Contains($name)) {
                    $newLine = $newLine.Remove($m.Index, $m.Length).Insert($m.Index, $name + "(")
                }
            }
            $result.Add($newLine + $eol)
        } else {
            $result.Add($line)
        }
    }

    return , $result.ToArray()
}

function FixTabAlignment($Lines) {
    $result = [System.Collections.Generic.List[string]]::new()
    foreach ($line in $Lines) {
        $stripped = $line.TrimEnd("`r`n")
        $eol = if ($line.EndsWith("`n")) { "`n" } else { "" }
        $m = [regex]::Match($stripped, '^(\t*)')
        $indent = $m.Groups[1].Value
        $rest = $stripped.Substring($indent.Length)
        $restNew = [regex]::Replace($rest, '\t{2,}', "`t")
        $result.Add($indent + $restNew + $eol)
    }
    return , $result.ToArray()
}

function SimplifySectionComments($Lines) {
    $result = [System.Collections.Generic.List[string]]::new()
    $i = 0

    while ($i -lt $Lines.Count) {
        $line = $Lines[$i].TrimEnd("`r`n")
        if ($line -match '^/\* =+\s*\*/$') {
            if ($i + 2 -lt $Lines.Count) {
                $midLine = $Lines[$i+1].TrimEnd("`r`n")
                $endLine = $Lines[$i+2].TrimEnd("`r`n")
                $midMatch = [regex]::Match($midLine, '^/\*\s+(.+?)\s*\*/$')
                if ($midMatch.Success -and $endLine -match '^/\* =+\s*\*/$') {
                    $heading = $midMatch.Groups[1].Value.Trim()
                    $result.Add("// $heading`n")
                    $i += 3; continue
                }
            }
        }
        $result.Add($Lines[$i]); $i++
    }

    return , $result.ToArray()
}

function ConvertBlockComments($Lines) {
    $result = [System.Collections.Generic.List[string]]::new()

    foreach ($line in $Lines) {
        $stripped = $line.TrimEnd("`r`n")
        $eol = if ($line.EndsWith("`n")) { "`n" } else { "" }

        # 行全体が /* ... */ コメント
        if ($stripped -match '^(\s*)/\*\s*(.*?)\s*\*/\s*$') {
            $ind = $Matches[1]; $cnt = $Matches[2]
            $result.Add("$ind// $cnt$eol"); continue
        }

        # コード + スペース/タブ + /* ... */ 末尾コメント（スペースなしも含む）
        if ($stripped -match '^(.*\S)(\s*)/\*\s*(.*?)\s*\*/\s*$') {
            $codePart = $Matches[1]; $spacePart = $Matches[2]; $comment = $Matches[3]
            # スペースが空の場合は1スペース補完
            if ($spacePart -eq '') { $spacePart = ' ' }
            $result.Add("$codePart$spacePart// $comment$eol"); continue
        }

        $result.Add($line)
    }

    return , $result.ToArray()
}

function ProcessFile($FilePath) {
    $fileName = Split-Path -Leaf $FilePath
    Write-Host "処理中: $fileName"

    $rawBytes = [System.IO.File]::ReadAllBytes($FilePath)

    # BOM検出
    $hasBom = ($rawBytes.Length -ge 3 -and $rawBytes[0] -eq 0xEF -and $rawBytes[1] -eq 0xBB -and $rawBytes[2] -eq 0xBF)

    # エンコーディング判定
    # BOM付きUTF-8 → UTF8Encoding(BOM=true) でデコード（BOM文字を消費）
    # BOMなし → CP932 でデコード
    if ($hasBom) {
        $encoding = [System.Text.UTF8Encoding]::new($true)  # BOM付きUTF-8
    } else {
        $encoding = [System.Text.Encoding]::GetEncoding(932)
    }

    $text = $encoding.GetString($rawBytes)
    # BOM付きUTF-8でデコードするとBOM文字(U+FEFF)は消費されるはずだが念のため除去
    if ($text.Length -gt 0 -and [int]$text[0] -eq 0xFEFF) {
        $text = $text.Substring(1)
    }

    # 行に分割（LF改行コードを保持）
    $lineList = [System.Collections.Generic.List[string]]::new()
    $pos = 0
    while ($pos -lt $text.Length) {
        $nl = $text.IndexOf([char]"`n", $pos)
        if ($nl -lt 0) {
            if ($pos -lt $text.Length) { $lineList.Add($text.Substring($pos)) }
            break
        }
        # CRLF対応（CRを除いてLFのみに統一）
        if ($nl -gt 0 -and $text[$nl-1] -eq [char]"`r") {
            $lineList.Add($text.Substring($pos, $nl - $pos - 1) + "`n")
        } else {
            $lineList.Add($text.Substring($pos, $nl - $pos) + "`n")
        }
        $pos = $nl + 1
    }

    $arr = $lineList.ToArray()
    $originalCount = $arr.Count

    $arr = ConvertFileHeader $arr
    $arr = RemoveFooterCopyright $arr
    $arr = RemoveFunctionHeaders $arr
    $arr = SimplifySectionComments $arr
    $arr = ConvertBlockComments $arr
    $arr = FixTabAlignment $arr
    $arr = FixFunctionCallSpaces $arr

    $outText = [string]::Join("", $arr)
    # 元のエンコーディングで書き戻し（BOM付きUTF-8はBOMも付加）
    if ($hasBom) {
        $encNoBom = [System.Text.UTF8Encoding]::new($false)
        $contentBytes = $encNoBom.GetBytes($outText)
        $bom = [byte[]]@(0xEF, 0xBB, 0xBF)
        $outBytes = $bom + $contentBytes
    } else {
        $outBytes = $encoding.GetBytes($outText)
    }
    [System.IO.File]::WriteAllBytes($FilePath, $outBytes)

    Write-Host "  完了: $($arr.Count)行 (元: $originalCount行)"
}

# メイン処理
$targetDirs = @(
    "$BasePath\Info",
    "$BasePath\Info\InfoChar",
    "$BasePath\LibInfo",
    "$BasePath\AdminApi"
)

$files = [System.Collections.Generic.List[string]]::new()
foreach ($dir in $targetDirs) {
    if (Test-Path $dir) {
        Get-ChildItem -Path $dir -Filter "*.h"   | ForEach-Object { $files.Add($_.FullName) }
        Get-ChildItem -Path $dir -Filter "*.cpp" | ForEach-Object { $files.Add($_.FullName) }
    }
}

$files = $files | Sort-Object

foreach ($f in $files) {
    ProcessFile $f
}

Write-Host "`n全ファイル処理完了 ($($files.Count)ファイル)"
