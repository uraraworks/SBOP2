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
    )
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot = Split-Path -Parent $scriptDir
$outPath = Join-Path $repoRoot $OutDir

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
        "SDL2/include"
    )

    $args = New-Object System.Collections.Generic.List[string]
    foreach ($dir in $includeDirs) {
        $args.Add("-I")
        $args.Add((Join-Path $Root $dir))
    }
    return $args
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
    "-c",
    "-std=gnu++17",
    "-D__EMSCRIPTEN__",
    "-DUNICODE",
    "-D_UNICODE",
    "-DNO_DIRECTMUSIC",
    "-D_WFL_NONDMUSIC",
    "-Winvalid-pch",
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

foreach ($source in $Sources) {
    $sourcePath = Join-Path $repoRoot $source
    if (-not (Test-Path $sourcePath)) {
        Write-Warning "source not found: $source"
        $failed += $source
        continue
    }

    $objectName = ([IO.Path]::GetFileNameWithoutExtension($sourcePath)) + ".o"
    $objectPath = Join-Path $outPath $objectName

    Write-Host "[browser-preflight] $source"
    if ($empp) {
        $args = New-Object System.Collections.Generic.List[string]
        foreach ($arg in $commonArgs) {
            $args.Add($arg)
        }
        $args.Add($sourcePath)
        $args.Add("-o")
        $args.Add($objectPath)

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

Write-Host "[browser-preflight] success"
