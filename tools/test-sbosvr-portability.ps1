# SboSvr の移植状況チェック
#
# 「非Windows でも通る」ことを主張するファイルを em++(clang) でコンパイルし、
# Windows 依存が残っていないことを確認する。リンクはしない。
#
#   pwsh tools/test-sbosvr-portability.ps1
#
# 脱Windows 対応が進むたびに $PortableFiles へ足していく。
# ここに載っているファイルは windows.h や Win32 API に依存してはいけない
# (実装内の #ifdef _WIN32 分岐は可。非Windows 側が通ればよい)。

param([switch]$Verbose)

$ErrorActionPreference = "Stop"
$Root = Resolve-Path (Join-Path $PSScriptRoot "..")

# 移植済みとして扱うファイル(.cpp)
$PortableFiles = @(
    "SboSvr/src/Platform/SvrPlatform.cpp"
)

# 移植済みとして扱うヘッダ
# 単体で include できること(依存を自前で取り込んでいること)も確認する
$PortableHeaders = @(
    "Common/Platform/PlatformDefs.h",
    "Common/Platform/CStringCompat.h"
)

# em++ を探す
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
if (-not $env:EMSDK_ARCH) { $env:EMSDK_ARCH = "x86_64" }

$OutDir = Join-Path $Root "out\portability"
if (-not (Test-Path $OutDir)) { New-Item -ItemType Directory -Force $OutDir | Out-Null }

$ok = 0
$ng = 0

Write-Output "=== SboSvr 移植状況チェック (em++ でコンパイル) ==="
foreach ($rel in $PortableFiles) {
    $src = Join-Path $Root $rel
    if (-not (Test-Path $src)) {
        Write-Output "  NG   $rel (ファイルが無い)"
        $ng++
        continue
    }

    $obj = Join-Path $OutDir ((Split-Path $rel -Leaf) + ".o")
    $log = & $EmPP -std=c++14 -c $src -o $obj -I (Join-Path $Root "SboSvr/src") 2>&1
    if ($LASTEXITCODE -eq 0) {
        Write-Output "  OK   $rel"
        $ok++
    } else {
        Write-Output "  NG   $rel"
        $log | Select-Object -First 8 | ForEach-Object { "         $_" }
        $ng++
    }
    if ($Verbose -and $log) { $log | ForEach-Object { "         $_" } }
}

foreach ($rel in $PortableHeaders) {
    $hdr = Join-Path $Root $rel
    if (-not (Test-Path $hdr)) {
        Write-Output "  NG   $rel (ファイルが無い)"
        $ng++
        continue
    }

    # ヘッダ単体を include するだけの一時ソースを作ってコンパイルする
    $tmp = Join-Path $OutDir ((Split-Path $rel -Leaf) + ".probe.cpp")
    Set-Content $tmp "#include `"$rel`"`nint main(){ return 0; }" -Encoding utf8
    $obj = Join-Path $OutDir ((Split-Path $rel -Leaf) + ".o")
    $log = & $EmPP -std=c++14 -c $tmp -o $obj -I $Root 2>&1
    if ($LASTEXITCODE -eq 0) {
        Write-Output "  OK   $rel (単体include)"
        $ok++
    } else {
        Write-Output "  NG   $rel (単体include)"
        $log | Select-Object -First 6 | ForEach-Object { "         $_" }
        $ng++
    }
}

Write-Output "=== 成功 $ok / 失敗 $ng ==="
if ($ng -gt 0) { exit 1 }
