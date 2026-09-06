# SboSvr 移植の棚卸し(調査用)
#
# tools/test-sbosvr-portability.ps1 との役割の違い:
#   - test-sbosvr-portability.ps1 … 回帰防止。「移植済み」と宣言したファイルが
#     今も通ることを確認する。CI 向け。1ファイルずつ -c でコンパイルするので遅いが、
#     対象は少数(移植済みリスト分)なので問題にならない。
#   - scan-sbosvr-portability.ps1(このファイル) … 棚卸し。SboSvr/src と Common 配下の
#     全 .cpp を em++ の -fsyntax-only にまとめて掛け、まだ通らないファイルと
#     最初のエラーを一覧化する。移植の残作業量を把握するための調査用であり、
#     CI には組み込まない(全ファイル対象で低速なうえ、失敗は「まだ移植していない」
#     という既知の状態なので exit code で落とす意味が無い)。
#
# 高速化の理由:
#   em++ は1ファイルずつ起動すると起動オーバーヘッドだけで約10秒かかり、
#   317ファイルを1本ずつ処理すると現実的な時間で終わらない。
#   -fsyntax-only はリンクせず構文・型チェックだけを行うため、複数ファイルを
#   まとめて1回の起動で渡せる。-ferror-limit=1 でファイルごとの最初の1エラーだけ
#   拾い、40ファイル単位のバッチに分けてバッチ間の詰まり(1バッチが巨大になり
#   途中のエラーで打ち切られる)を避けている。
#   実測: 317ファイルを40件ずつのバッチ(約8バッチ)で処理して約4分(2026-09-06)。
#
# 実行例:
#   pwsh tools/scan-sbosvr-portability.ps1
#   pwsh tools/scan-sbosvr-portability.ps1 -Roots "SboSvr\src" -Batch 20
#
# 出力:
#   out/portability-scan/errors.txt   … 抽出したエラー行の全文
#   out/portability-scan/badfiles.txt … 失敗ファイル別に1行ずつ(ファイル名 + 最初のエラー)
#   (out/ はリポジトリの .gitignore で除外済み)

param([string[]]$Roots = @("SboSvr\src", "Common"), [int]$Batch = 40)

$Root = Resolve-Path (Join-Path $PSScriptRoot "..")
Set-Location $Root
$env:EMSDK_ARCH = "x86_64"

$EmPP = $null
if ($env:EMSDK -and (Test-Path (Join-Path $env:EMSDK "upstream\emscripten\em++.bat"))) {
    $EmPP = Join-Path $env:EMSDK "upstream\emscripten\em++.bat"
} elseif (Test-Path "C:\emsdk\upstream\emscripten\em++.bat") {
    $EmPP = "C:\emsdk\upstream\emscripten\em++.bat"
}
if (-not $EmPP) {
    Write-Output "em++ が見つかりません。C:\emsdk か EMSDK 環境変数を確認してください。"
    exit 2
}

$incDirs = @(".", "SboSvr", "SboSvr\src", "SboSvr\src\Info", "SboSvr\src\Info\InfoChar", "SboSvr\src\Lib",
    "SboSvr\src\LibInfo", "SboSvr\src\MainFrame", "SboSvr\src\SaveLoad", "SboSvr\third_party\sqlite",
    "SboSvr\third_party\lodepng", "Common", "Common\Lib", "Common\Lib\md5", "Common\Info",
    "Common\Info\InfoMapEvent", "Common\Info\InfoItemType", "Common\Info\InfoSkill", "Common\Info\InfoTalkEvent",
    "Common\myLib", "Common\LibInfo", "Common\Packet", "Common\Packet\ACCOUNT", "Common\Packet\CHAR",
    "Common\Packet\CONNECT", "Common\Packet\ADMIN", "Common\Packet\ITEM", "Common\Packet\EFFECT",
    "Common\Packet\MSGCMD", "Common\Packet\SKILL", "Common\myLib\myZLib")
$inc = @()
foreach ($d in $incDirs) { $inc += "-I"; $inc += (Join-Path $Root $d) }

$OutDir = Join-Path $Root "out\portability-scan"
if (-not (Test-Path $OutDir)) { New-Item -ItemType Directory -Force $OutDir | Out-Null }

$files = Get-ChildItem -Recurse -Path $Roots -Filter *.cpp |
    ForEach-Object { $_.FullName.Substring("$Root".Length + 1) }

$errLines = @()
for ($i = 0; $i -lt $files.Count; $i += $Batch) {
    $chunk = $files[$i..([Math]::Min($i + $Batch - 1, $files.Count - 1))]
    $o = & $EmPP -std=c++14 -fsyntax-only -ferror-limit=1 @chunk @inc 2>&1
    $errLines += ($o | Where-Object { $_ -match "(error|fatal error):" })
    Write-Output "batch $i / $($files.Count)"
}
$errLines | Set-Content (Join-Path $OutDir "errors.txt") -Encoding utf8

# ファイル別に集計(同じファイルで複数エラーが出ても最初の1件だけ残す)
$bad = @{}
foreach ($l in $errLines) {
    if ("$l" -match '^([^:]+\.(cpp|h)):') { $bad[$Matches[1]] = "$l" }
}
Write-Output "--- 失敗ファイル数: $($bad.Count) / 全 $($files.Count) ---"
$bad.GetEnumerator() | Sort-Object Name |
    ForEach-Object { "$($_.Key)`t$($_.Value)" } |
    Set-Content (Join-Path $OutDir "badfiles.txt") -Encoding utf8
Write-Output "詳細: $OutDir\errors.txt / badfiles.txt"
