param(
    [string]$OutDir = "out/browser-title",
    [string]$PreflightOutDir = "out/browser-title/obj",
    [switch]$SkipPreflight
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
