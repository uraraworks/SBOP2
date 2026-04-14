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
    "SboCli/src/Platform/SjisConvert.cpp",
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
    "SboCli/src/Window/WindowLOGINBrowser.cpp",
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
    "imgui/backends/imgui_impl_sdlrenderer2.cpp",
    # RecvProc (VERSION/CONNECT/ACCOUNT のみ。CHAR以降はブラウザ版で未対応)
    "SboCli/src/MainFrame/MainFrameRecvProcVERSION.cpp",
    "SboCli/src/MainFrame/MainFrameRecvProcCONNECT.cpp",
    "SboCli/src/MainFrame/MainFrameRecvProcACCOUNT.cpp",
    # StateProc (DISCONNECT/INFO/LOGINMENU。MAP はブラウザ版で未対応)
    "SboCli/src/StateProc/StateProcDISCONNECT.cpp",
    "SboCli/src/StateProc/StateProcINFO.cpp",
    "SboCli/src/StateProc/StateProcLOGINMENU.cpp",
    # Packet (基底クラス)
    "Common/Packet/PacketBase.cpp",
    # Packet (VERSION)
    "Common/Packet/VERSION/PacketVERSION_REQ_VERSIONCHECK.cpp",
    "Common/Packet/VERSION/PacketVERSION_RES_VERSIONCHECK.cpp",
    "Common/Packet/VERSION/PacketVERSION_RES_FILELISTCHECK.cpp",
    # Packet (CONNECT)
    "Common/Packet/CONNECT/PacketCONNECT_REQ_LOGIN.cpp",
    "Common/Packet/CONNECT/PacketCONNECT_RES_LOGIN.cpp",
    "Common/Packet/CONNECT/PacketCONNECT_REQ_PLAY.cpp",
    "Common/Packet/CONNECT/PacketCONNECT_RES_PLAY.cpp",
    "Common/Packet/CONNECT/PacketCONNECT_KEEPALIVE.cpp",
    # Packet (ACCOUNT)
    "Common/Packet/ACCOUNT/PacketACCOUNT_REQ_ACCOUNTINFO.cpp",
    "Common/Packet/ACCOUNT/PacketACCOUNT_RES_ACCOUNTINFO.cpp",
    "Common/Packet/ACCOUNT/PacketACCOUNT_REQ_MAKECHAR.cpp",
    "Common/Packet/ACCOUNT/PacketACCOUNT_RES_MAKECHAR.cpp",
    # Packet (CHAR - ログインメニューで使用する分のみ)
    "Common/Packet/CHAR/PacketCHAR_REQ_CHARINFO.cpp",
    # Info
    "Common/Info/InfoAccount.cpp",
    "Common/Info/InfoCharBase.cpp",
    "SboCli/src/Info/InfoCharCli.cpp",
    # LibInfo
    "Common/LibInfo/LibInfoAccount.cpp",
    "Common/LibInfo/LibInfoCharBase.cpp",
    "SboCli/src/LibInfo/LibInfoCharCli.cpp",
    # Window (ログインメニュー関連)
    "SboCli/src/Window/WindowLOGINMENU.cpp",
    "SboCli/src/Window/WindowSTATUS.cpp",
    "SboCli/src/Window/WindowCHARNAME.cpp",
    "SboCli/src/Window/WindowSEX.cpp",
    "SboCli/src/Window/WindowSTYLESELECT.cpp",
    "SboCli/src/Window/WindowNAMEINPUT.cpp",
    "SboCli/src/Window/WindowFAMILYTYPE.cpp",
    "SboCli/src/Window/WindowHAIRTYPE.cpp",
    "SboCli/src/Window/WindowHAIRCOLOR.cpp",
    "SboCli/src/Window/WindowEYECOLOR.cpp",
    # Layer (ログインメニュー関連)
    "SboCli/src/Layer/LayerCharSelect.cpp",
    "SboCli/src/Layer/LayerLoginMenu.cpp",
    "SboCli/src/Layer/LayerInfo.cpp",
    # Lib
    "SboCli/src/Lib/MacAddr.cpp",
    # Info (基底クラス・エフェクト)
    "Common/Info/InfoBase.cpp",
    "Common/Info/InfoEffect.cpp",
    "Common/Info/InfoTextEffect.cpp",
    # LibInfo (エフェクト)
    "Common/LibInfo/LibInfoEfcBalloon.cpp",
    "Common/LibInfo/LibInfoEffect.cpp",
    # Layer (マップ - InfoCharCli からの参照で必要)
    "SboCli/src/Layer/LayerMap.cpp",
    # Packet (InfoCharCli からの参照で必要)
    "Common/Packet/BATTLE/PacketBATTLE_REQ_ATACK.cpp",
    "Common/Packet/CHAR/PacketCHAR_PARA1.cpp",
    # Info (アニメーション - InfoEffect からの参照で必要)
    "Common/Info/InfoAnime.cpp",
    # RecvProc (CHAR - キャラ情報受信処理)
    "SboCli/src/MainFrame/MainFrameRecvProcCHAR.cpp",
    # Packet (CHAR - 全 CHAR パケット。PacketCHAR_REQ_CHARINFO と PacketCHAR_PARA1 は追加済み)
    "Common/Packet/CHAR/PacketCHAR_CHARID.cpp",
    "Common/Packet/CHAR/PacketCHAR_CHARINFO.cpp",
    "Common/Packet/CHAR/PacketCHAR_CHAT.cpp",
    "Common/Packet/CHAR/PacketCHAR_GRP.cpp",
    "Common/Packet/CHAR/PacketCHAR_ITEMINFO.cpp",
    "Common/Packet/CHAR/PacketCHAR_MODIFY_PARAM.cpp",
    "Common/Packet/CHAR/PacketCHAR_MOTION.cpp",
    "Common/Packet/CHAR/PacketCHAR_MOTIONTYPE.cpp",
    "Common/Packet/CHAR/PacketCHAR_MOVEPOS.cpp",
    "Common/Packet/CHAR/PacketCHAR_REQ_CHARINFO2.cpp",
    "Common/Packet/CHAR/PacketCHAR_REQ_CHAT.cpp",
    "Common/Packet/CHAR/PacketCHAR_REQ_DRAGITEM.cpp",
    "Common/Packet/CHAR/PacketCHAR_REQ_EQUIP.cpp",
    "Common/Packet/CHAR/PacketCHAR_REQ_MODIFY_PARAM.cpp",
    "Common/Packet/CHAR/PacketCHAR_REQ_PUSH.cpp",
    "Common/Packet/CHAR/PacketCHAR_REQ_PUTGET.cpp",
    "Common/Packet/CHAR/PacketCHAR_REQ_TAIL.cpp",
    "Common/Packet/CHAR/PacketCHAR_REQ_USEITEM.cpp",
    "Common/Packet/CHAR/PacketCHAR_RES_CHARINFO.cpp",
    "Common/Packet/CHAR/PacketCHAR_RES_PUTGET.cpp",
    "Common/Packet/CHAR/PacketCHAR_RES_TALKEVENT.cpp",
    "Common/Packet/CHAR/PacketCHAR_RES_TAIL.cpp",
    "Common/Packet/CHAR/PacketCHAR_SET_EFCBALLOON.cpp",
    "Common/Packet/CHAR/PacketCHAR_SET_EFFECT.cpp",
    "Common/Packet/CHAR/PacketCHAR_SKILLINFO.cpp",
    "Common/Packet/CHAR/PacketCHAR_STATE.cpp",
    "Common/Packet/CHAR/PacketCHAR_STATUS.cpp",
    "Common/Packet/CHAR/PacketCHAR_TEXTEFFECT.cpp",
    # LibInfo (モーション・アイテム - RecvProcCHAR の依存)
    "Common/LibInfo/LibInfoMotion.cpp",
    "Common/LibInfo/LibInfoMotionType.cpp",
    "Common/LibInfo/LibInfoItem.cpp",
    "Common/LibInfo/LibInfoItemType.cpp",
    "Common/LibInfo/LibInfoItemWeapon.cpp",
    # LibInfo (基底クラス)
    "Common/LibInfo/LibInfoBase.cpp",
    # Info (マップ基底・モーション・トークイベント)
    "Common/Info/InfoMapBase.cpp",
    "Common/Info/InfoMotion.cpp",
    "Common/Info/InfoMotionType.cpp",
    "Common/Info/InfoTalkEvent/InfoTalkEvent.cpp",
    "Common/Info/InfoTalkEvent/InfoTalkEventBase.cpp",
    "Common/Info/InfoTalkEvent/InfoTalkEventMENU.cpp",
    "Common/Info/InfoTalkEvent/InfoTalkEventPAGE.cpp",
    # Info (アイテム・吹き出し - LibInfo* の依存)
    "Common/Info/InfoItem.cpp",
    "Common/Info/InfoItemType/InfoItemTypeBase.cpp",
    "Common/Info/InfoItemWeapon.cpp",
    "Common/Info/InfoEfcBalloon.cpp",
    # Info (キャラ系 - InfoCharCli の依存)
    "SboCli/src/Info/InfoChar/InfoCharATACKANIMECli.cpp",
    "SboCli/src/Info/InfoChar/InfoCharBALLCli.cpp",
    "SboCli/src/Info/InfoChar/InfoCharMOVEATACKCli.cpp",
    "SboCli/src/Info/InfoChar/InfoCharSCORECli.cpp"
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
    "-sASSERTIONS=2",
    "-sSAFE_HEAP=1",
    "-sSTACK_OVERFLOW_CHECK=2",
    "-sSTACK_SIZE=2097152",
    "-sEXPORTED_RUNTIME_METHODS=['ccall','cwrap']",
    "-sDISABLE_EXCEPTION_CATCHING=0",
    "-lwebsocket",
    "-sENVIRONMENT=web",
    "-Wl,--error-limit=0",
    "--preload-file", "$resDir@/SboGrpData/res",
    "--preload-file", "$fontDir@/font",
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

Write-Host "[browser-link] success"
