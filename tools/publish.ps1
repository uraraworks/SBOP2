#requires -Version 5.1
<#
.SYNOPSIS
    VPS デプロイ用に SboSvr + Web 関連一式を zip にまとめる publish スクリプト。

.DESCRIPTION
    以下を順に行います:
      1) ブラウザクライアント (out/browser-title) のビルド
         （tools/build-sbocli-browser-title.ps1 に委譲。-SkipBrowserBuild で抑止可）
      2) SboSvr Release|Win32 のビルド (MSBuild)
         （PostBuildEvent で webroot/ , webroot/game/ , SboGrpData.dll が SboSvr/Release/ に入る）
      3) staging ディレクトリへ成果物を集約
            SboSvr/        ... SboSvr.exe, SboGrpData.dll, SboSvr.ini, webroot/, SBODATA/seed
            WebSocketBridge/ ... server.js, package.json, README.txt
            README.txt     ... 起動手順
      4) publish/SBOP2_Server_yyyyMMdd_HHmmss.zip に圧縮

.PARAMETER Configuration
    ビルド構成 (既定: Release)

.PARAMETER Platform
    ビルドプラットフォーム (既定: Win32)

.PARAMETER OutputDir
    生成物の出力先ディレクトリ (既定: <repo>/publish)

.PARAMETER SkipBrowserBuild
    out/browser-title の再ビルドをスキップ (既存成果物をそのまま使う)

.PARAMETER SkipServerBuild
    SboSvr の再ビルドをスキップ

.PARAMETER NoZip
    staging までで止め、zip 化を行わない

.EXAMPLE
    pwsh -File tools\publish.ps1
    pwsh -File tools\publish.ps1 -SkipBrowserBuild
#>
param(
    [string]$Configuration = "Release",
    [string]$Platform      = "Win32",
    [string]$OutputDir     = "",
    [switch]$SkipBrowserBuild,
    [switch]$SkipServerBuild,
    [switch]$NoZip,
    [string]$EmsdkDir = "C:\emsdk",
    [int]$HttpPort = 18080,
    [int]$GamePort = 2007,
    [switch]$IncludeBridge
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot  = Split-Path -Parent $scriptDir
if ([string]::IsNullOrEmpty($OutputDir)) {
    $OutputDir = Join-Path $repoRoot "publish"
}

function Write-Step([string]$msg) {
    Write-Host "==> $msg" -ForegroundColor Cyan
}

function Resolve-MsBuild {
    # 1) PATH 上の msbuild
    $cmd = Get-Command msbuild.exe -ErrorAction SilentlyContinue
    if ($cmd) { return $cmd.Source }

    # 2) vswhere 経由で最新 VS の MSBuild を探す
    $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    if (Test-Path $vswhere) {
        $path = & $vswhere -latest -products * `
            -requires Microsoft.Component.MSBuild `
            -find "MSBuild\**\Bin\MSBuild.exe" | Select-Object -First 1
        if ($path -and (Test-Path $path)) { return $path }
    }

    throw "msbuild.exe が見つかりません。Visual Studio Build Tools を導入するか PATH に追加してください。"
}

function Initialize-Emsdk {
    param([string]$EmsdkRoot)

    # 既に em++ が PATH か EMSDK 経由で見えていれば何もしない
    if (Get-Command em++ -ErrorAction SilentlyContinue) { return $true }
    if ($env:EMSDK -and (Test-Path (Join-Path $env:EMSDK "upstream\emscripten\em++.bat"))) { return $true }

    if (-not (Test-Path $EmsdkRoot)) {
        Write-Warning "emsdk が見つかりません: $EmsdkRoot (em++ にフォールバックされず cl.exe が使われます)"
        return $false
    }

    $envScript = Join-Path $EmsdkRoot "emsdk_env.ps1"
    if (-not (Test-Path $envScript)) {
        Write-Warning "emsdk_env.ps1 が見つかりません: $envScript"
        return $false
    }

    # emsdk_env.ps1 は ARM64 で誤判定されることがあるため明示
    if (-not $env:EMSDK_ARCH) { $env:EMSDK_ARCH = "x86_64" }

    Write-Step "emsdk を有効化: $EmsdkRoot"
    & $envScript | Out-Null
    if (-not $env:EMSDK) {
        $env:EMSDK = $EmsdkRoot
    }
    return $true
}

# --- 1) ブラウザクライアントのビルド ---
if (-not $SkipBrowserBuild) {
    Initialize-Emsdk -EmsdkRoot $EmsdkDir | Out-Null

    Write-Step "ブラウザクライアント (out/browser-title) をビルド"
    $browserScript = Join-Path $scriptDir "build-sbocli-browser-title.ps1"
    if (-not (Test-Path $browserScript)) {
        throw "ブラウザビルドスクリプトが見つかりません: $browserScript"
    }
    # em++ が無い環境では preflight(cl.exe フォールバック) が必ず失敗するので、
    # em++ が解決できないときだけ -SkipPreflight を付ける
    $browserArgs = @()
    if (-not (Get-Command em++ -ErrorAction SilentlyContinue) -and
        -not ($env:EMSDK -and (Test-Path (Join-Path $env:EMSDK "upstream\emscripten\em++.bat")))) {
        Write-Warning "em++ が解決できないため preflight をスキップします (本ビルド側で em++ が必要)"
        $browserArgs += "-SkipPreflight"
    }

    # 子 pwsh に EMSDK 環境を引き継がせるため -NoProfile のみで invoke
    & pwsh.exe -NoProfile -ExecutionPolicy Bypass -File $browserScript @browserArgs
    if ($LASTEXITCODE -ne 0) {
        throw "ブラウザクライアントのビルドに失敗しました (exit=$LASTEXITCODE)"
    }
} else {
    Write-Step "ブラウザビルドをスキップ"
}

$browserOutDir = Join-Path $repoRoot "out\browser-title"
if (-not (Test-Path (Join-Path $browserOutDir "sbocli-title.html"))) {
    throw "ブラウザクライアント成果物が見つかりません: $browserOutDir\sbocli-title.html"
}

# --- 2) SboSvr Release ビルド ---
$svrProjDir  = Join-Path $repoRoot "SboSvr"
$svrOutDir   = Join-Path $svrProjDir $Configuration   # e.g. SboSvr\Release
$svrExePath  = Join-Path $svrOutDir "SboSvr.exe"
$svrDllPath  = Join-Path $svrOutDir "SboGrpData.dll"

if (-not $SkipServerBuild) {
    Write-Step "SboSvr ($Configuration|$Platform) をビルド"
    $msbuild = Resolve-MsBuild
    $slnPath = Join-Path $repoRoot "SBO.sln"
    $logPath = Join-Path $repoRoot "publish_build.log"
    & $msbuild $slnPath `
        "/t:SboSvr" `
        "/p:Configuration=$Configuration" `
        "/p:Platform=$Platform" `
        "/m" `
        "/nologo" `
        "/v:minimal" `
        "/flp:Verbosity=normal;LogFile=$logPath"
    if ($LASTEXITCODE -ne 0) {
        throw "SboSvr のビルドに失敗しました (exit=$LASTEXITCODE)。詳細: $logPath"
    }
} else {
    Write-Step "SboSvr ビルドをスキップ"
}

if (-not (Test-Path $svrExePath)) {
    throw "SboSvr.exe が見つかりません: $svrExePath"
}

# --- 3) staging ---
$stamp     = Get-Date -Format "yyyyMMdd_HHmmss"
$pkgName   = "SBOP2_Server_$stamp"
$stageRoot = Join-Path $OutputDir $pkgName

if (Test-Path $stageRoot) {
    Remove-Item -Recurse -Force $stageRoot
}
New-Item -ItemType Directory -Force -Path $stageRoot | Out-Null

Write-Step "staging へ集約: $stageRoot"

# 3-a) SboSvr 一式
$stageSvr = Join-Path $stageRoot "SboSvr"
New-Item -ItemType Directory -Force -Path $stageSvr | Out-Null

Copy-Item $svrExePath  $stageSvr -Force
if (Test-Path $svrDllPath) {
    Copy-Item $svrDllPath $stageSvr -Force
} else {
    Write-Warning "SboGrpData.dll が $svrDllPath に見つかりません (PostBuild が走らなかった可能性)"
}

# webroot (PostBuild で SboSvr/Release/webroot に出ている)
$stageWebroot = Join-Path $stageSvr "webroot"
$svrWebroot   = Join-Path $svrOutDir "webroot"
if (-not (Test-Path $svrWebroot)) {
    throw "webroot が見つかりません: $svrWebroot (PostBuild 失敗?)"
}
Copy-Item $svrWebroot $stageSvr -Recurse -Force

# webroot/game (念のためソース側 out/browser-title から上書き同期)
$stageGameDir = Join-Path $stageWebroot "game"
if (-not (Test-Path $stageGameDir)) {
    New-Item -ItemType Directory -Force -Path $stageGameDir | Out-Null
}
Copy-Item (Join-Path $browserOutDir "sbocli-title.*") $stageGameDir -Force

# wss 書換シムを sbocli-title.html の <title>…</title> 直後に注入
# (HTTPS 配信時に WASM が組み立てる ws://host:port を同一オリジンの
#  wss://<host>/__sbop2bridge__ に書き換える。IIS が 443 で受けて 18081 にプロキシ)
$gameHtml = Join-Path $stageGameDir "sbocli-title.html"
if (Test-Path $gameHtml) {
    $shimMarker = "/*__SBOP2_WSS_SHIM__*/"
    $shim = '<script>' + $shimMarker + `
            '(function(){if(location.protocol!==''https:'')return;' + `
            'var N=window.WebSocket;function P(u,p){try{if(typeof u===''string''&&u.indexOf(''ws://'')===0){' + `
            'u=''wss://''+location.host+''/__sbop2bridge__'';}}catch(e){}' + `
            'return (arguments.length>1)?new N(u,p):new N(u);}' + `
            'P.prototype=N.prototype;P.OPEN=N.OPEN;P.CONNECTING=N.CONNECTING;P.CLOSING=N.CLOSING;P.CLOSED=N.CLOSED;' + `
            'window.WebSocket=P;})();</script>'

    $html = Get-Content $gameHtml -Raw -Encoding utf8
    if ($html -match [regex]::Escape($shimMarker)) {
        Write-Host "  wss シムは既に注入済み (skip)"
    } elseif ($html -match '(?is)(</title>)') {
        $html = [regex]::Replace($html, '(?is)(</title>)', "`$1`r`n$shim", 1)
        Set-Content -Path $gameHtml -Value $html -Encoding utf8 -NoNewline
        Write-Host "  wss 書換シムを注入: sbocli-title.html"
    } else {
        Write-Warning "sbocli-title.html に </title> が見つからずシム注入できませんでした"
    }
} else {
    Write-Warning "sbocli-title.html が見つかりません: $gameHtml"
}

# SboSvr.ini (本番用に Port + HttpPort を固定して生成 / 既存 ini があっても上書き)
# - Port      : ゲーム本体 TCP
# - HttpPort  : 管理 Web / ブラウザ版クライアント配信 (本番 IIS が 80/443 でフロント)
#               内蔵 WebSocket ブリッジは HttpPort+1 で待ち受け
$iniDst = Join-Path $stageSvr "SboSvr.ini"
$iniSrc = Join-Path $repoRoot "Release\SboSvr.ini"
$posBlock = "[Pos]`nMainLeft=31`nMainTop=44`nMainRight=318`nMainBottom=194"
if (Test-Path $iniSrc) {
    # 既存 ini の [Pos] を尊重しつつ [Setting] を再生成
    $existing = Get-Content $iniSrc -Raw -Encoding ASCII
    if ($existing -match '(?ms)\[Pos\].*?(?=^\[|\z)') {
        $posBlock = $Matches[0].TrimEnd()
    }
}
$iniContent = @"
$posBlock
[Setting]
Port=$GamePort
HttpPort=$HttpPort
"@
Set-Content -Path $iniDst -Value $iniContent -Encoding ASCII
Write-Host "  SboSvr.ini  Port=$GamePort / HttpPort=$HttpPort"

# SBODATA seed (既存 Release/SBODATA の .dat と ServerInfo.csv を初期データとして同梱)
$sbodataSrc = Join-Path $repoRoot "Release\SBODATA"
$sbodataDst = Join-Path $stageSvr "SBODATA"
New-Item -ItemType Directory -Force -Path $sbodataDst | Out-Null
if (Test-Path $sbodataSrc) {
    Get-ChildItem $sbodataSrc -Filter "*.dat" -ErrorAction SilentlyContinue |
        ForEach-Object { Copy-Item $_.FullName $sbodataDst -Force }
    $csv = Join-Path $sbodataSrc "ServerInfo.csv"
    if (Test-Path $csv) { Copy-Item $csv $sbodataDst -Force }
} else {
    Write-Warning "Release\SBODATA が無いため空の SBODATA/ を同梱します (初回起動時に SboData.db が作成されます)"
}

# Log/ は実行時に作られるので空フォルダだけ用意
New-Item -ItemType Directory -Force -Path (Join-Path $stageSvr "Log") | Out-Null

# 3-b) WebSocketBridge (既定では同梱しない。SboSvr.exe がブリッジを内蔵しているため)
if ($IncludeBridge) {
    Write-Step "WebSocketBridge を同梱 (-IncludeBridge)"
    $bridgeSrc = Join-Path $repoRoot "WebSocketBridge"
    $bridgeDst = Join-Path $stageRoot "WebSocketBridge"
    New-Item -ItemType Directory -Force -Path $bridgeDst | Out-Null
    Copy-Item (Join-Path $bridgeSrc "server.js")    $bridgeDst -Force
    Copy-Item (Join-Path $bridgeSrc "package.json") $bridgeDst -Force

    @"
# WebSocketBridge (旧/開発用)

本番では SboSvr.exe が内蔵ブリッジ (HttpPort+1) で待ち受けるため未使用。
ローカル開発で別プロセスとして動かしたい場合だけ使用する。

## セットアップ
    npm install

## 起動
    node server.js

## 環境変数
- WS_PORT  : WebSocket 待ち受けポート (既定 9100)
- TCP_HOST : SboSvr のアドレス        (既定 127.0.0.1)
- TCP_PORT : SboSvr の TCP ポート     (既定 10000 — SboSvr.ini の Port と合わせること)
"@ | Set-Content -Path (Join-Path $bridgeDst "README.txt") -Encoding UTF8
}

# 3-c) ルートの README
$wsPort = $HttpPort + 1
@"
# SBOP2 サーバー配布パッケージ

ビルド日時: $stamp

## 構成
- SboSvr/             ゲーム本体サーバー + 管理 Web + ブラウザ版クライアント配信
    - SboSvr.exe
    - SboGrpData.dll
    - SboSvr.ini       Port=$GamePort (ゲーム TCP) / HttpPort=$HttpPort (管理Web+ブラウザ配信)
                       内蔵 WebSocket ブリッジは HttpPort+1=$wsPort で待ち受け
    - webroot/         管理 Web 画面
    - webroot/game/    ブラウザ版クライアント (HTTPS 用 wss 書換シム注入済み)
    - SBODATA/         マスタデータ seed (SboData.db は初回起動時に作成)
                       ※ 再デプロイ時に既存 SboData.db を上書きしないこと
    - Log/             サーバーログ出力先

## 本番再デプロイ手順 (恒久設定は IIS / scheduled task 側で済み)

    Stop-ScheduledTask -TaskName SBOP2_SboSvr
    Get-Process -Name SboSvr -ErrorAction SilentlyContinue | Stop-Process -Force
    # この zip を C:\SBOP2 へ上書き展開 (SBODATA\SboData.db は残す)
    Start-ScheduledTask -TaskName SBOP2_SboSvr

## ポート
- $HttpPort/tcp : 管理 Web + ブラウザ配信 (HTTP, IIS が 80/443 でフロント)
- $wsPort/tcp : 内蔵 WebSocket ブリッジ (IIS が /__sbop2bridge__ から逆プロキシ)
- $GamePort/tcp  : SboSvr 本体ゲーム TCP
"@ | Set-Content -Path (Join-Path $stageRoot "README.txt") -Encoding UTF8

# --- 4) zip ---
if ($NoZip) {
    Write-Step "NoZip 指定のため staging で終了: $stageRoot"
    return
}

$zipPath = Join-Path $OutputDir "$pkgName.zip"
if (Test-Path $zipPath) { Remove-Item -Force $zipPath }

Write-Step "zip 化: $zipPath"
Compress-Archive -Path (Join-Path $stageRoot "*") -DestinationPath $zipPath -CompressionLevel Optimal

$sizeMB = [math]::Round((Get-Item $zipPath).Length / 1MB, 2)
Write-Host ""
Write-Host "完了！ $zipPath ($sizeMB MB)" -ForegroundColor Green
