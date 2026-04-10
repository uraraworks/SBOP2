param(
    [string]$OutDir = "out/browser-title",
    [string]$PreflightOutDir = "out/browser-title/obj",
    [switch]$SkipPreflight,
    [switch]$Force
)

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
$fontDir = Join-Path $repoRoot "SboCli\font"

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
            exit 0
        }
    }
}

$sources = @(
    "SboCli/src/BrowserMain.cpp",
    "SboCli/src/MainFrame/MainFrame.cpp",
    "SboCli/src/MgrData.cpp",
    "SboCli/src/MgrSound.cpp",
    "SboCli/src/MgrDraw.cpp",
    "SboCli/src/MgrLayer.cpp",
    "SboCli/src/MgrGrpData.cpp",
    "SboCli/src/MgrKeyInput.cpp",
    "SboCli/src/MgrWindow.cpp",
    "SboCli/src/Platform/SDLApp.cpp",
    "SboCli/src/Platform/SDLInput.cpp",
    "SboCli/src/Platform/SDLWindow.cpp",
    "SboCli/src/Platform/SdlFont.cpp",
    "SboCli/src/Lib/DInputUtil.cpp",
    "SboCli/src/Lib/DXAudio.cpp",
    "SboCli/src/Lib/Img32.cpp",
    "SboCli/src/LibSboSoundLoader.cpp",
    "SboCli/src/Lib/third_party/lodepng.cpp",
    "SboCli/src/Layer/LayerBase.cpp",
    "SboCli/src/Layer/LayerLogo.cpp",
    "SboCli/src/Layer/LayerCloud.cpp",
    "SboCli/src/Layer/LayerTitle.cpp",
    "SboCli/src/StateProc/StateProcBase.cpp",
    "SboCli/src/StateProc/StateProcLOGO.cpp",
    "SboCli/src/StateProc/StateProcLOGIN.cpp",
    "SboCli/src/Window/WindowBase.cpp",
    "SboCli/src/Window/WindowLOGIN.cpp",
    "SboCli/src/Window/WindowLOGINNull.cpp",
    "SboCli/src/Window/WindowMSG.cpp",
    "Common/Lib/CryptUtil.cpp",
    "Common/Lib/DataSlot.cpp",
    "Common/SBOGlobal.cpp",
    "Common/myLib/mySection.cpp",
    "Common/myLib/myString.cpp",
    "Common/UraraSockTCPSBO.cpp",
    "Common/UraraSockTCPWebSocket.cpp",
    "Common/crc.cpp",
    "Common/myLib/myZlib/myZlib.cpp",
    "imgui/imgui.cpp",
    "imgui/imgui_draw.cpp",
    "imgui/imgui_tables.cpp",
    "imgui/imgui_widgets.cpp",
    "imgui/backends/imgui_impl_sdl2.cpp",
    "imgui/backends/imgui_impl_sdlrenderer2.cpp"
)

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
    & $preflightScript -OutDir $PreflightOutDir -Sources $sources
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
    "-sUSE_SDL=2",
    "-sUSE_SDL_TTF=2",
    "-sALLOW_MEMORY_GROWTH=1",
    "-sASSERTIONS=1",
    "-sSTACK_SIZE=2097152",
    "-sEXPORTED_RUNTIME_METHODS=['ccall','cwrap']",
    "-sENVIRONMENT=web",
    "--preload-file", "$resDir@/SboGrpData/res",
    "--preload-file", "$fontDir@/font",
    "--shell-file", $shellFile,
    "--post-js", $eglSwapPost,
    "-o", (Join-Path $outPath "sbocli-title.html")
) + $objects

Write-Host "[browser-link] linking sbocli-title.html"
& $empp @linkArgs
if ($LASTEXITCODE -ne 0) {
    throw "browser link に失敗しました。"
}

Write-Host "[browser-link] success"
