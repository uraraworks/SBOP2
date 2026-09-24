param(
    [string]$OutDir = "out/browser-title",
    [string]$PreflightOutDir = "out/browser-title/obj",
    [switch]$SkipPreflight,
    [switch]$Force,
    [switch]$Rebuild  # -Force の別名（後方互換用）
)

# -Rebuild は -Force の別名
if ($Rebuild) { $Force = $true }

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot = Split-Path -Parent $scriptDir
$outPath = Join-Path $repoRoot $OutDir
$objOutDir = Join-Path $repoRoot $PreflightOutDir
$preflightScript = Join-Path $scriptDir "test-sbocli-browser-preflight.ps1"
$eglSwapPost = Join-Path $scriptDir "emscripten\egl_swapinterval_post.js"
$shellFile = Join-Path $scriptDir "emscripten\sbocli-title.shell.html"
$resDir = Join-Path $repoRoot "SboGrpData\res"
# ブラウザ版はサブセット版フォントを使う(全部入りは 1 本 16〜17MB あり
# 初回ロードを大きく圧迫するため)。再生成は tools/make-font-subset.py。
$fontDir = Join-Path $repoRoot "SboCli\font\subset"
if (-not (Test-Path (Join-Path $fontDir "NotoSansCJKjp-Regular.otf"))) {
    throw "サブセットフォントが見つかりません: $fontDir (tools/make-font-subset.py で生成してください)"
}
$bgmDir  = Join-Path $repoRoot "Release\BGM"
$wavDir  = Join-Path $repoRoot "SboSoundData\res\WAVE"

function Get-BrowserSourceList {
    param([string]$Path)
    if (-not (Test-Path $Path)) {
        throw "ブラウザ版ソース一覧ファイルが見つかりません: $Path"
    }
    $lines = Get-Content -Path $Path -Encoding utf8
    $result = New-Object System.Collections.Generic.List[string]
    foreach ($line in $lines) {
        $trimmed = $line.Trim()
        if ($trimmed -eq '') { continue }
        if ($trimmed.StartsWith('#')) { continue }
        $result.Add($trimmed)
    }
    return $result.ToArray()
}

function Resolve-NodeExe {
    # emsdk 同梱の node でよい。PATH に無ければ EMSDK 配下を探す。
    $cmd = Get-Command node -ErrorAction SilentlyContinue
    if ($cmd) {
        return $cmd.Source
    }
    $emsdkRoot = if ($env:EMSDK) { $env:EMSDK } else { "C:\emsdk" }
    $nodeDir = Join-Path $emsdkRoot "node"
    if (Test-Path $nodeDir) {
        $candidate = Get-ChildItem -Path $nodeDir -Filter "node.exe" -Recurse -ErrorAction SilentlyContinue | Select-Object -First 1
        if ($candidate) {
            return $candidate.FullName
        }
    }
    return $null
}

function Copy-BgmAssets {
    param(
        [Parameter(Mandatory=$true)]
        [string]$SourceDir,
        [Parameter(Mandatory=$true)]
        [string]$OutputDir
    )

    # BGM は .data に含めず個別ファイルとして配信する。
    if (-not (Test-Path $SourceDir)) {
        Write-Warning "[browser-bgm] BGM ディレクトリが見つかりません: $SourceDir"
        return
    }
    $targetDir = Join-Path $OutputDir "BGM"
    if (-not (Test-Path $targetDir)) {
        New-Item -ItemType Directory -Path $targetDir | Out-Null
    }
    $copied = 0
    foreach ($file in (Get-ChildItem -Path $SourceDir -Filter "*.ogg" -File)) {
        $dest = Join-Path $targetDir $file.Name
        # 変わっていないものは触らない(タイムスタンプを動かさない)
        if ((Test-Path $dest) -and ((Get-Item $dest).LastWriteTime -ge $file.LastWriteTime)) {
            continue
        }
        Copy-Item -Path $file.FullName -Destination $dest -Force
        $copied++
    }
    Write-Host "[browser-bgm] copied $copied ogg file(s) to $targetDir"
}

function New-PrecompressedAssets {
    param(
        [Parameter(Mandatory=$true)]
        [string]$SourceDir
    )

    $script = Join-Path $scriptDir "emscripten\precompress.mjs"
    if (-not (Test-Path $script)) {
        Write-Warning "[browser-precompress] precompress.mjs が見つかりません。スキップします。"
        return
    }
    $nodeExe = Resolve-NodeExe
    if (-not $nodeExe) {
        Write-Warning "[browser-precompress] node が見つからないため事前圧縮をスキップします。"
        return
    }

    # 既に圧縮済みの ogg/png が大半を占める .data も、フォント(otf)が効くので対象に含める
    $targets = @("sbocli-title.wasm", "sbocli-title.js", "sbocli-title.html", "sbocli-title.data")
    try {
        & $nodeExe $script $SourceDir @targets
        if ($LASTEXITCODE -ne 0) {
            Write-Warning "[browser-precompress] 事前圧縮に失敗しました (exit=$LASTEXITCODE)"
        }
    } catch {
        Write-Warning "[browser-precompress] 事前圧縮に失敗しました: $_"
    }
}

function Sync-BrowserTitleToAdminWebroot {
    param(
        [Parameter(Mandatory=$true)]
        [string]$SourceDir
    )

    if (-not (Test-Path (Join-Path $SourceDir "sbocli-title.html"))) {
        return
    }

    $adminWebroots = @(
        (Join-Path $repoRoot "SboSvr\Debug\webroot"),
        (Join-Path $repoRoot "SboSvr\Release\webroot")
    )

    foreach ($webroot in $adminWebroots) {
        if (-not (Test-Path $webroot)) {
            continue
        }

        $targetDir = Join-Path $webroot "game"
        if (-not (Test-Path $targetDir)) {
            New-Item -ItemType Directory -Path $targetDir | Out-Null
        }

        Copy-Item -Path (Join-Path $SourceDir "sbocli-title.*") -Destination $targetDir -Force

        # BGM は個別ファイル配信なのでディレクトリごと運ぶ
        $bgmSource = Join-Path $SourceDir "BGM"
        if (Test-Path $bgmSource) {
            Copy-Item -Path $bgmSource -Destination $targetDir -Recurse -Force
        }

        Write-Host "[browser-deploy] copied browser client to $targetDir"
    }
}

# Up-to-date check: 出力 html がソースよりも新しければ何もしない
if (-not $Force) {
    $outFile = Join-Path $outPath "sbocli-title.html"
    if (Test-Path $outFile) {
        $outTime = (Get-Item $outFile).LastWriteTime
        $watchedDirs = @(
            (Join-Path $repoRoot "SboCli/src"),
            (Join-Path $repoRoot "SboCli/StdAfx.h"),
            (Join-Path $repoRoot "Common"),
            (Join-Path $repoRoot "imgui")
        )
        $newest = $null
        foreach ($d in $watchedDirs) {
            if (Test-Path $d) {
                $items = Get-ChildItem -Path $d -Recurse -ErrorAction SilentlyContinue `
                    -Include "*.cpp","*.h","*.hpp","*.c","*.cc"
                foreach ($item in $items) {
                    if (($newest -eq $null) -or ($item.LastWriteTime -gt $newest)) {
                        $newest = $item.LastWriteTime
                    }
                }
            }
        }
        # ビルドスクリプト群やシェルファイル等の補助ファイルもチェック
        $auxFiles = Get-ChildItem -Path $scriptDir -Recurse -ErrorAction SilentlyContinue `
            -Include "*.ps1","*.js","*.html","*.json"
        foreach ($item in $auxFiles) {
            if (($newest -eq $null) -or ($item.LastWriteTime -gt $newest)) {
                $newest = $item.LastWriteTime
            }
        }
        if ($newest -ne $null -and $newest -lt $outTime) {
            Write-Host "[browser-build] up-to-date, skipping (latest source: $newest, output: $outTime)"
            Copy-BgmAssets -SourceDir $bgmDir -OutputDir $outPath
            Sync-BrowserTitleToAdminWebroot -SourceDir $outPath
            exit 0
        }
    }
}

$sourcesListPath = Join-Path $scriptDir "browser-sources.txt"
$sources = Get-BrowserSourceList -Path $sourcesListPath

function Resolve-Empp {
    $cmd = Get-Command em++ -ErrorAction SilentlyContinue
    if ($cmd) {
        return $cmd.Source
    }

    if ($env:EMSDK) {
        $candidate = Join-Path $env:EMSDK "upstream\emscripten\em++.bat"
        if (Test-Path $candidate) {
            return $candidate
        }
    }

    throw "em++ が見つかりません。emsdk_env.ps1 を先に読み込んでください。"
}

if (-not $SkipPreflight) {
    # -Force/-Rebuild が指定された場合は preflight にも伝播して全ファイル再コンパイル
    if ($Force) {
        & $preflightScript -OutDir $PreflightOutDir -Sources $sources -Force
    } else {
        & $preflightScript -OutDir $PreflightOutDir -Sources $sources
    }
    if ($LASTEXITCODE -ne 0) {
        throw "browser preflight に失敗しました。"
    }
}

$empp = Resolve-Empp

New-Item -ItemType Directory -Force -Path $outPath | Out-Null

$objects = foreach ($source in $sources) {
    $objectName = ([IO.Path]::GetFileNameWithoutExtension($source)) + ".o"
    Join-Path $objOutDir $objectName
}

$linkArgs = @(
    "-O3",
    "-msimd128",
    "-sUSE_SDL=2",
    "-sUSE_SDL_TTF=2",
    "-sALLOW_MEMORY_GROWTH=1",
    "-sASSERTIONS=0",
    "-sSTACK_SIZE=2097152",
    "-sEXPORTED_RUNTIME_METHODS=['ccall','cwrap','HEAPU8','FS']",
    "-sEXPORTED_FUNCTIONS=['_main','_malloc','_free','_SBOP2_SetWebAdminMode','_SBOP2_SetWebAdminSelectedPartsID','_SBOP2_AdminPlaySound','_SBOP2_BrowserChatSetComposition','_SBOP2_BrowserChatCommitText','_SBOP2_BrowserSetPadKey','_SBOP2_BrowserSetPadFacing']",
    "-sDISABLE_EXCEPTION_CATCHING=0",
    "-lwebsocket",
    "-sENVIRONMENT=web",
    "-Wl,--error-limit=0",
    "--preload-file", "$resDir@/SboGrpData/res",
    "--preload-file", "$fontDir@/font",
    # BGM は .data に同梱せず out/browser-title/BGM/ に個別配置し、
    # 起動後に emscripten_async_wget で取得する(初回ロードを軽くするため)。
    "--preload-file", "$wavDir@/WAVE",
    "--shell-file", $shellFile,
    "--post-js", $eglSwapPost,
    "-o", (Join-Path $outPath "sbocli-title.html")
) + $objects

Write-Host "[browser-link] linking sbocli-title.html"

# コマンドライン長制限を回避するためレスポンスファイルを使用
$rspFile = Join-Path $outPath "link_args.rsp"
# バックスラッシュをフォワードスラッシュに変換（em++ がエスケープ文字として解釈するのを防止）
$linkArgs | ForEach-Object { $_.Replace('\', '/') } | Out-File -FilePath $rspFile -Encoding utf8NoBOM
& $empp "@$rspFile"
if ($LASTEXITCODE -ne 0) {
    throw "browser link に失敗しました。"
}

# ファイル別ハッシュのキャッシュバスターと表示バージョンを埋め込む。
# 通常のリロードだけで最新の .js/.wasm/.data を確実に取得させ、
# ハードリロード/キャッシュクリアを不要にするための後処理。
# 失敗してもビルドは止めない（try/catch で安全側に倒す）。
try {
    $htmlFile = Join-Path $outPath "sbocli-title.html"
    $jsFile   = Join-Path $outPath "sbocli-title.js"
    $wasmFile = Join-Path $outPath "sbocli-title.wasm"
    $dataFile = Join-Path $outPath "sbocli-title.data"

    # 出力3ファイルの短縮ハッシュ（SHA1先頭10桁・小文字）を算出
    $jsHash   = (Get-FileHash -Algorithm SHA1 $jsFile).Hash.Substring(0, 10).ToLower()
    $wasmHash = (Get-FileHash -Algorithm SHA1 $wasmFile).Hash.Substring(0, 10).ToLower()
    $dataHash = (Get-FileHash -Algorithm SHA1 $dataFile).Hash.Substring(0, 10).ToLower()

    # 表示バージョンは Common/SBOVersion.h の VERTEXT を単一ソースとして読み取る
    $versionHeaderFile = Join-Path $repoRoot "Common\SBOVersion.h"
    $versionHeaderText = Get-Content -Path $versionHeaderFile -Raw -Encoding UTF8
    $versionMatch = [regex]::Match($versionHeaderText, 'VERTEXT\s+"([^"]+)"')
    if (-not $versionMatch.Success) {
        throw "SBOVersion.h から VERTEXT を取得できませんでした。"
    }
    $displayVersion = $versionMatch.Groups[1].Value

    # ビルド識別子はビルド日時（コミット有無に依存せず常に正直な値）を用いる
    $buildStamp = Get-Date -Format "yyyy-MM-dd HH:mm"

    $htmlText = Get-Content -Path $htmlFile -Raw -Encoding UTF8

    # (1) script の js 参照にクエリを付与する。
    #     Emscripten 生成物は `<script async src=sbocli-title.js>` のように
    #     引用符なしで出力されるため、引用符あり/なし両対応の正規表現にする。
    $htmlText = [regex]::Replace(
        $htmlText,
        '(src=["'']?sbocli-title\.js)(["''\s>])',
        { param($m) $m.Groups[1].Value + "?v=$jsHash" + $m.Groups[2].Value }
    )

    # (2) プレースホルダをアセットハッシュ表＋表示バージョン情報の埋め込みJSに置換する
    # .data の展開後実サイズ（バイト）。ローディング表示の分母に使う
    # （file_packager の setStatus total は .data.br の圧縮後 Content-Length なので使えない）。
    $dataSizeBytes = (Get-Item $dataFile).Length
    $injectJs = 'window.__SBOP2_ASSET_HASHES__={"sbocli-title.wasm":"' + $wasmHash + '","sbocli-title.data":"' + $dataHash + '"};' +
        'window.SBOP2_BUILD={display:"' + $displayVersion + '",build:"' + $buildStamp + '"};' +
        'window.__SBOP2_DATA_SIZE__=' + $dataSizeBytes + ';'
    # JSコメントはHTML圧縮時に消えるため、保持されるmeta要素を置換する。
    $placeholder = '<meta\s+name\s*=\s*["'']?sbop2-build-inject["'']?\s*/?>'
    if ([regex]::IsMatch($htmlText, $placeholder)) {
        $htmlText = [regex]::Replace($htmlText, $placeholder, { param($m) "<script>$injectJs</script>" })
    } else {
        throw "生成HTMLにキャッシュバスター埋め込み用プレースホルダが見つかりませんでした。"
    }

    # UTF-8 BOMなしで書き戻す
    $utf8NoBom = New-Object System.Text.UTF8Encoding($false)
    [IO.File]::WriteAllText($htmlFile, $htmlText, $utf8NoBom)

    Write-Host "[browser-cachebust] embedded asset hashes (js=$jsHash wasm=$wasmHash data=$dataHash) version=$displayVersion build=$buildStamp"
} catch {
    Write-Warning "[browser-cachebust] キャッシュバスター埋め込みに失敗しました: $_"
}

Copy-BgmAssets -SourceDir $bgmDir -OutputDir $outPath
New-PrecompressedAssets -SourceDir $outPath
Sync-BrowserTitleToAdminWebroot -SourceDir $outPath
Write-Host "[browser-link] success"
