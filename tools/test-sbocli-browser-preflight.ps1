param(
    [string]$OutDir = "out/browser-preflight",
    [string[]]$Sources = @(
        "SboCli/src/BrowserMain.cpp",
        "SboCli/src/Lib/Img32.cpp",
        "SboCli/src/Layer/LayerLogo.cpp",
        "SboCli/src/Layer/LayerCloud.cpp",
        "SboCli/src/Layer/LayerTitle.cpp",
        "SboCli/src/MgrSound.cpp",
        "SboCli/src/MgrData.cpp",
        "SboCli/src/MainFrame/MainFrame.cpp"
    ),
    # 差分ビルドを無効化して全ファイルを再コンパイルする
    [switch]$Force,
    [switch]$Rebuild  # -Force の別名（後方互換用）
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot = Split-Path -Parent $scriptDir
$outPath = Join-Path $repoRoot $OutDir

# -Rebuild は -Force の別名
if ($Rebuild) { $Force = $true }

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

    return $null
}

function Get-VsWherePath {
    $candidates = @(
        "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe",
        "$env:ProgramFiles\Microsoft Visual Studio\Installer\vswhere.exe"
    )
    foreach ($path in $candidates) {
        if (Test-Path $path) {
            return $path
        }
    }
    return $null
}

function Resolve-VsDevCmd {
    $vswhere = Get-VsWherePath
    if ($vswhere) {
        $found = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -find Common7\Tools\VsDevCmd.bat
        if ($found) {
            return $found | Select-Object -First 1
        }
    }
    return $null
}

function Get-IncludeArgs {
    param([string]$Root)

    $includeDirs = @(
        ".",
        "SboCli",
        "SboCli/src",
        "SboCli/src/Dialog",
        "SboCli/src/Info",
        "SboCli/src/Info/InfoChar",
        "SboCli/src/Layer",
        "SboCli/src/Lib",
        "SboCli/src/Lib/Png",
        "SboCli/src/LibInfo",
        "SboCli/src/MainFrame",
        "SboCli/src/StateProc",
        "SboCli/src/Window",
        "SboCli/src/Wnd",
        "Common",
        "Common/Lib",
        "Common/Lib/mfc",
        "Common/Info",
        "Common/Info/InfoMapEvent",
        "Common/Info/InfoItemType",
        "Common/Info/InfoSkill",
        "Common/Info/InfoTalkEvent",
        "Common/myLib",
        "Common/LibInfo",
        "Common/Packet",
        "Common/Packet/ACCOUNT",
        "Common/Packet/CHAR",
        "Common/Packet/CONNECT",
        "Common/Packet/ADMIN",
        "Common/Packet/CONECT",
        "Common/Packet/ITEM",
        "Common/Packet/MAP",
        "Common/Packet/MSGCMD",
        "Common/Packet/SKILL",
        "Common/myLib/myZLib",
        "SboSoundData",
        "SDL2/include",
        "imgui",
        "imgui/backends"
    )

    $args = New-Object System.Collections.Generic.List[string]
    foreach ($dir in $includeDirs) {
        $args.Add("-I")
        $args.Add((Join-Path $Root $dir))
    }
    return $args
}

# .d ファイルを解析して依存ファイルリストを返す（Makefile 形式）
function Get-DepsFromDFile {
    param([string]$DFile)

    if (-not (Test-Path $DFile)) {
        return @()
    }

    $content = Get-Content $DFile -Raw -Encoding utf8
    # "target: dep1 dep2 \" 形式を解析。行末バックスラッシュを除去して結合
    $content = $content -replace '\\\r?\n', ' '
    # コロン以降を取得
    if ($content -match ':\s+(.+)') {
        $depPart = $Matches[1]
        # スペース区切りで分割（バックスラッシュエスケープスペースは除く）
        $deps = $depPart -split '\s+' | Where-Object { $_ -ne '' -and $_ -ne '\' }
        return $deps
    }
    return @()
}

# ソースファイルが up-to-date かどうか判定
# .d ファイルが存在する場合はヘッダ依存も考慮する
function Test-UpToDate {
    param(
        [string]$ObjectPath,
        [string]$DepFile,
        [string]$SourcePath
    )

    if (-not (Test-Path $ObjectPath)) {
        return $false
    }

    $objTime = (Get-Item $ObjectPath).LastWriteTime

    # .d ファイルが存在する場合はその依存関係を使う
    if (Test-Path $DepFile) {
        $deps = Get-DepsFromDFile -DFile $DepFile
        if ($deps.Count -gt 0) {
            foreach ($dep in $deps) {
                # パスの区切り文字を正規化
                $depNorm = $dep -replace '/', '\'
                if (Test-Path $depNorm) {
                    $depTime = (Get-Item $depNorm).LastWriteTime
                    if ($depTime -gt $objTime) {
                        return $false
                    }
                }
            }
            return $true
        }
    }

    # .d ファイルがない場合はソースファイルのみで判定
    $srcTime = (Get-Item $SourcePath).LastWriteTime
    return $srcTime -lt $objTime
}

$empp = Resolve-Empp
$vsDevCmd = $null
if (-not $empp) {
    $vsDevCmd = Resolve-VsDevCmd
    if (-not $vsDevCmd) {
        throw "em++ も VsDevCmd.bat も見つかりません。emsdk か Visual Studio C++ ツールを有効にしてください。"
    }
}

New-Item -ItemType Directory -Force -Path $outPath | Out-Null
$includeArgs = Get-IncludeArgs -Root $repoRoot

$commonArgs = New-Object System.Collections.Generic.List[string]
$commonArgItems = @(
    "-O2",
    "-fno-strict-aliasing",
    "-c",
    "-std=gnu++17",
    "-D__EMSCRIPTEN__",
    "-DUNICODE",
    "-D_UNICODE",
    "-DNO_DIRECTMUSIC",
    "-D_WFL_NONDMUSIC",
    "-sUSE_SDL=2",
    "-sUSE_SDL_TTF=2",
    "-sDISABLE_EXCEPTION_CATCHING=0",
    "-Winvalid-pch",
    # -MMD: 依存関係ファイル（.d）を生成（差分ビルド用）
    "-MMD",
    "-include",
    (Join-Path $repoRoot "Common/rpcsal_fallback.h"),
    "-include",
    (Join-Path $repoRoot "SboCli/StdAfx.h")
)
foreach ($arg in $commonArgItems) {
    $commonArgs.Add($arg)
}
foreach ($arg in $includeArgs) {
    $commonArgs.Add($arg)
}

$failed = @()
$skipped = 0
$compiled = 0
$global:LASTEXITCODE = 0

foreach ($source in $Sources) {
    $sourcePath = Join-Path $repoRoot $source
    if (-not (Test-Path $sourcePath)) {
        Write-Warning "source not found: $source"
        $failed += $source
        continue
    }

    $baseName = [IO.Path]::GetFileNameWithoutExtension($sourcePath)
    $objectPath = Join-Path $outPath ($baseName + ".o")
    $depFile = Join-Path $outPath ($baseName + ".d")

    # 差分ビルドチェック（-Force/-Rebuild 指定時はスキップ）
    if (-not $Force -and $empp) {
        if (Test-UpToDate -ObjectPath $objectPath -DepFile $depFile -SourcePath $sourcePath) {
            Write-Host "[browser-preflight] skip (up-to-date): $source"
            $skipped++
            continue
        }
    }

    Write-Host "[browser-preflight] compile: $source"
    $compiled++

    if ($empp) {
        $args = New-Object System.Collections.Generic.List[string]
        foreach ($arg in $commonArgs) {
            $args.Add($arg)
        }
        $args.Add($sourcePath)
        $args.Add("-o")
        $args.Add($objectPath)
        # -MF で .d ファイルの出力先を明示指定（デフォルトはソースと同階層になるため）
        $args.Add("-MF")
        $args.Add($depFile)

        & $empp @args
        if ($LASTEXITCODE -ne 0) {
            $failed += $source
        }
    } else {
        $includeFlat = ($includeArgs | ForEach-Object {
            if ($_ -eq "-I") { $null } else { "/I`"$_`"" }
        }) -join " "

        $cmdParts = @(
            "call `"$vsDevCmd`"",
            "&&",
            "cl.exe /nologo /c /TP /std:c++17 /D__EMSCRIPTEN__ /DUNICODE /D_UNICODE /DNO_DIRECTMUSIC /D_WFL_NONDMUSIC",
            "/FI`"$repoRoot\Common\rpcsal_fallback.h`"",
            "/FI`"$repoRoot\SboCli\StdAfx.h`"",
            $includeFlat,
            "`"$sourcePath`"",
            "/Fo`"$objectPath`""
        )
        $cmdLine = [string]::Join(" ", $cmdParts)
        cmd.exe /d /s /c $cmdLine
        if ($LASTEXITCODE -ne 0) {
            $failed += $source
        }
    }
}

if ($failed.Count -gt 0) {
    Write-Error ("browser preflight failed: " + ($failed -join ", "))
}

Write-Host "[browser-preflight] success (compiled: $compiled, skipped: $skipped)"
