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
    "SboSvr/src/Platform/SvrPlatform.cpp",
    "Common/Platform/SjisConvert.cpp",
    "Common/Info/InfoCharBase.cpp",
    "SboSvr/src/LibInfo/LibInfoCharSvrMapEvent.cpp",
    "Common/Info/InfoTextEffect.cpp",
    "Common/LibInfo/LibInfoDisable.cpp",
    "Common/LibInfo/LibInfoEffect.cpp",
    "Common/LibInfo/LibInfoMapParts.cpp",
    "Common/LibInfo/LibInfoMapShadow.cpp",
    "Common/Lib/TextInput.cpp",
    "Common/LibInfo/LibInfoCharBase.cpp",
    "Common/Lib/TextOutput.cpp",
    "SboSvr/src/LibInfo/LibInfoCharSvr.cpp",
    "SboSvr/src/SaveLoad/SaveLoadInfoBase.cpp",
    "SboSvr/src/MainFrame/MainFrameRecvProcVERSION.cpp",
    "SboSvr/src/Web/WebSocketProtocol.cpp",
    "SboSvr/src/Web/Handlers/StaticFileHandler.cpp",
    # WebSocketProtocol.cpp: SHA-1 は自前実装化済みで依存ゼロ。
    # RecvAll/SendAll が使う WSAEINTR は SvrCompat.h に追加済み(WSAEWOULDBLOCK
    # の隣)なので、ここへ載せられるようになった。
    "SboSvr/src/Web/GrpImageStore.cpp",
    "SboSvr/src/Web/AdminWsHub.cpp",
    "SboSvr/src/Web/WebSocketBridge.cpp",
    # GrpImageStore.cpp: 実行ファイルディレクトリ取得を GetModuleFileNameA から
    # SboPlatform::GetExeDirectory() へ置き換え、windows.h 依存を除去。
    # AdminWsHub.cpp / WebSocketBridge.cpp: デバッグログの wsprintfA を
    # snprintf へ置き換え、残っていた windows.h 専用 API 依存を除去。
    "SboSvr/src/MgrData.cpp",
    "SboSvr/src/Web/Handlers/ServerInfoHandler.cpp",
    "SboSvr/src/MainFrame/MainFrameRecvProcADMIN.cpp",
    "SboSvr/src/MainFrame/MainFrameRecvProcCONNECT.cpp",
    "SboSvr/src/Web/SessionStore.cpp",
    "SboSvr/src/PasswordHash.cpp",
    "SboSvr/src/SboSvr.cpp"
    # SessionStore.cpp: セッショントークン用の乱数生成(CryptAcquireContext/
    # CryptGenRandom)を SboPlatform::GenerateRandomBytes() へ切り出し、
    # wincrypt.h 依存を除去。
    # MgrData.cpp: ログファイルパス組み立てを GetModuleFileName から
    # SboPlatform::GetExeDirectory() へ置き換え。
    # ServerInfoHandler.cpp: GetSystemTime(SYSTEMTIME) を
    # SboPlatform::GetSystemTime(LOCALTIME) へ置き換え(出力書式は不変)。
    # MainFrameRecvProcADMIN.cpp / MainFrameRecvProcCONNECT.cpp: IN_ADDR.S_un
    # (Windows固有のメンバ名)をやめ、DWORD を ntohl() してビットシフトで
    # オクテットを取り出す形に置き換え。
    # PasswordHash.cpp: PBKDF2-HMAC-SHA256 を Windows CNG (bcrypt.h) から
    # RFC 6234/2104/8018 準拠の自前実装に置き換え、bcrypt.lib 依存を除去。
    # SboSvr.cpp: 最後に残った Windows 依存だった __argc/__argv を、
    # 非Windows 側の main(argc, argv) から SboSvrMain() を直接呼ぶ形に
    # 置き換え、エントリポイント選択の #ifdef _WIN32 一箇所のみを残した。
    "SboSvr/src/Web/Handlers/GrpDraftHandler.cpp",
    # GrpDraftHandler.cpp: MSVC 方言の _atoi64 を標準の strtoll へ置き換え、
    # 唯一残っていた Windows 依存を除去。
    "SboSvr/src/Web/Handlers/MapPartsHandler.cpp"
    # MapPartsHandler.cpp: SboGrpData.dll のリソースを読む
    # LoadLibraryW/FindResourceW/LockResource/FreeLibrary を
    # SboPlatform::LoadEmbeddedPng() へ集約し、windows.h 直接依存を除去。
    #
    # SpriteSheetHandler.cpp は同じ DLL 段の隔離を済ませたが、
    # 2段目のファイル読み込み(TryLoadFromFileLocked, res/ 探索)が
    # GetModuleFileNameW/CreateFileW 等の Win32 API を直接使っており、
    # そちらは今回のスコープ外(res/ 読み込みロジックは変更しない)なので
    # まだ em++ を通らない。ここには未登録のまま残す。
)

# 移植済みとして扱うヘッダ
# 単体で include できること(依存を自前で取り込んでいること)も確認する
$PortableHeaders = @(
    "Common/Platform/PlatformDefs.h",
    "Common/Platform/CStringCompat.h",
    "Common/Platform/TCharCompat.h",
    "Common/Platform/SjisConvert.h",
    "SboSvr/StdAfx.h",
    "SboSvr/src/Web/SessionStore.h"
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

# コンパイル時のインクルードパス
$IncArgs = @(
    "-I", "$Root",
    "-I", (Join-Path $Root "SboSvr"),
    "-I", (Join-Path $Root "SboSvr/src"),
    "-I", (Join-Path $Root "SboSvr/src/Info"),
    "-I", (Join-Path $Root "SboSvr/src/Info/InfoChar"),
    "-I", (Join-Path $Root "SboSvr/src/Lib"),
    "-I", (Join-Path $Root "SboSvr/src/LibInfo"),
    "-I", (Join-Path $Root "SboSvr/src/MainFrame"),
    "-I", (Join-Path $Root "SboSvr/src/SaveLoad"),
    "-I", (Join-Path $Root "SboSvr/third_party/sqlite"),
    "-I", (Join-Path $Root "SboSvr/third_party/lodepng"),
    "-I", (Join-Path $Root "Common"),
    "-I", (Join-Path $Root "Common/Lib"),
    "-I", (Join-Path $Root "Common/Lib/md5"),
    "-I", (Join-Path $Root "Common/Info"),
    "-I", (Join-Path $Root "Common/Info/InfoMapEvent"),
    "-I", (Join-Path $Root "Common/Info/InfoItemType"),
    "-I", (Join-Path $Root "Common/Info/InfoSkill"),
    "-I", (Join-Path $Root "Common/Info/InfoTalkEvent"),
    "-I", (Join-Path $Root "Common/myLib"),
    "-I", (Join-Path $Root "Common/LibInfo"),
    "-I", (Join-Path $Root "Common/Packet"),
    "-I", (Join-Path $Root "Common/Packet/ACCOUNT"),
    "-I", (Join-Path $Root "Common/Packet/CHAR"),
    "-I", (Join-Path $Root "Common/Packet/CONNECT"),
    "-I", (Join-Path $Root "Common/Packet/ADMIN"),
    "-I", (Join-Path $Root "Common/Packet/ITEM"),
    "-I", (Join-Path $Root "Common/Packet/EFFECT"),
    "-I", (Join-Path $Root "Common/Packet/MSGCMD"),
    "-I", (Join-Path $Root "Common/Packet/SKILL"),
    "-I", (Join-Path $Root "Common/myLib/myZLib")
)

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
    $log = & $EmPP -std=c++14 -c $src -o $obj @IncArgs 2>&1
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
    $log = & $EmPP -std=c++14 -c $tmp -o $obj @IncArgs 2>&1
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
