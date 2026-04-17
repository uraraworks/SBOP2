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
$fontDir = Join-Path $repoRoot "SboCli\font"
$bgmDir  = Join-Path $repoRoot "Release\BGM"
$wavDir  = Join-Path $repoRoot "SboSoundData\res\WAVE"

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
    "SboCli/src/Platform/SoundDataTableBrowser.cpp",
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
    # RecvProc (VERSION/CONNECT/ACCOUNT。MAP/ITEM/ADMIN/EFFECT/SYSTEM/SKILLはPhase12で追加)
    "SboCli/src/MainFrame/MainFrameRecvProcVERSION.cpp",
    "SboCli/src/MainFrame/MainFrameRecvProcCONNECT.cpp",
    "SboCli/src/MainFrame/MainFrameRecvProcACCOUNT.cpp",
    # StateProc (DISCONNECT/INFO/LOGINMENU/MAP - Phase12でMAP有効化)
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
    "SboCli/src/Info/InfoChar/InfoCharSCORECli.cpp",
    # --- Phase 12: マップ画面ブラウザ対応 ---
    # StateProc (MAP)
    "SboCli/src/StateProc/StateProcMAP.cpp",
    # RecvProc (MAP/ITEM/ADMIN/EFFECT/SYSTEM/SKILL)
    "SboCli/src/MainFrame/MainFrameRecvProcMAP.cpp",
    "SboCli/src/MainFrame/MainFrameRecvProcITEM.cpp",
    "SboCli/src/MainFrame/MainFrameRecvProcADMIN.cpp",
    "SboCli/src/MainFrame/MainFrameRecvProcEFFECT.cpp",
    "SboCli/src/MainFrame/MainFrameRecvProcSYSTEM.cpp",
    "SboCli/src/MainFrame/MainFrameRecvProcSKILL.cpp",
    # Layer (MAP 関連)
    "SboCli/src/Layer/LayerSystemMsg.cpp",
    "SboCli/src/Layer/LayerMisty.cpp",
    "SboCli/src/Layer/LayerSnow.cpp",
    # Platform (ImGui MAP 関連)
    "SboCli/src/Platform/ImGuiMsgLog.cpp",
    "SboCli/src/Platform/ImGuiDbg.cpp",
    # LibInfo (マップ系)
    "Common/LibInfo/LibInfoMapBase.cpp",
    "Common/LibInfo/LibInfoMapObject.cpp",
    "Common/LibInfo/LibInfoMapObjectData.cpp",
    "Common/LibInfo/LibInfoMapParts.cpp",
    "Common/LibInfo/LibInfoMapShadow.cpp",
    "Common/LibInfo/LibInfoMapEvent.cpp",
    "Common/LibInfo/LibInfoSkill.cpp",
    "Common/LibInfo/LibInfoSystem.cpp",
    "Common/LibInfo/LibInfoDisable.cpp",
    # Info (マップ系)
    "Common/Info/InfoMapObject.cpp",
    "Common/Info/InfoMapObjectData.cpp",
    "Common/Info/InfoMapParts.cpp",
    "Common/Info/InfoMapShadow.cpp",
    "Common/Info/InfoMapEvent/InfoMapEventBase.cpp",
    "Common/Info/InfoMapEvent/InfoMapEventGRPIDTMP.cpp",
    "Common/Info/InfoMapEvent/InfoMapEventINITSTATUS.cpp",
    "Common/Info/InfoMapEvent/InfoMapEventLIGHT.cpp",
    "Common/Info/InfoMapEvent/InfoMapEventMAPMOVE.cpp",
    "Common/Info/InfoMapEvent/InfoMapEventMOVE.cpp",
    "Common/Info/InfoSkill/InfoSkillBase.cpp",
    "Common/Info/InfoSkill/InfoSkillHEAL.cpp",
    "Common/Info/InfoSkill/InfoSkillMOVEATACK.cpp",
    "Common/Info/InfoSystem.cpp",
    # Packet (MAP)
    "Common/Packet/MAP/PacketMAP_DELETEMAPSHADOW.cpp",
    "Common/Packet/MAP/PacketMAP_DELETEPARTS.cpp",
    "Common/Packet/MAP/PacketMAP_FORMATMSG.cpp",
    "Common/Packet/MAP/PacketMAP_MAPEVENT.cpp",
    "Common/Packet/MAP/PacketMAP_MAPNAME.cpp",
    "Common/Packet/MAP/PacketMAP_MAPOBJECT.cpp",
    "Common/Packet/MAP/PacketMAP_MAPOBJECTDATA.cpp",
    "Common/Packet/MAP/PacketMAP_MAPPARTS.cpp",
    "Common/Packet/MAP/PacketMAP_MAPSHADOW.cpp",
    "Common/Packet/MAP/PacketMAP_ONLINE.cpp",
    "Common/Packet/MAP/PacketMAP_PARA1.cpp",
    "Common/Packet/MAP/PacketMAP_RENEWMAPSIZE.cpp",
    "Common/Packet/MAP/PacketMAP_REQ_MAPINFO.cpp",
    "Common/Packet/MAP/PacketMAP_RES_MAPINFO.cpp",
    "Common/Packet/MAP/PacketMAP_SETMAPSHADOW.cpp",
    "Common/Packet/MAP/PacketMAP_SETPARTS.cpp",
    "Common/Packet/MAP/PacketMAP_SYSTEMMSG.cpp",
    # Packet (ITEM)
    "Common/Packet/ITEM/PacketITEM_DELETEITEMINFO.cpp",
    "Common/Packet/ITEM/PacketITEM_DELETEITEMTYPEINFO.cpp",
    "Common/Packet/ITEM/PacketITEM_ITEMINFO.cpp",
    "Common/Packet/ITEM/PacketITEM_ITEMTYPEINFO.cpp",
    "Common/Packet/ITEM/PacketITEM_ITEMWEAPONINFO.cpp",
    "Common/Packet/ITEM/PacketITEM_RENEWITEMINFO.cpp",
    "Common/Packet/ITEM/PacketITEM_RENEWITEMTYPEINFO.cpp",
    "Common/Packet/ITEM/PacketITEM_REQ_ITEMINFO.cpp",
    "Common/Packet/ITEM/PacketITEM_REQ_ITEMTYPEINFO.cpp",
    "Common/Packet/ITEM/PacketITEM_REQ_ITEMWEAPONINFO.cpp",
    "Common/Packet/ITEM/PacketITEM_RES_ITEMINFO.cpp",
    "Common/Packet/ITEM/PacketITEM_RES_ITEMTYPEINFO.cpp",
    "Common/Packet/ITEM/PacketITEM_RES_ITEMWEAPONINFO.cpp",
    # Packet (SKILL)
    "Common/Packet/SKILL/PacketSKILL_SKILLINFO.cpp",
    "Common/Packet/SKILL/PacketSKILL_PARA2.cpp",
    # Packet (EFFECT)
    "Common/Packet/EFFECT/PacketEFFECT_BALLOONINFO.cpp",
    "Common/Packet/EFFECT/PacketEFFECT_DELETEBALLOONINFO.cpp",
    "Common/Packet/EFFECT/PacketEFFECT_DELETEEFFECTINFO.cpp",
    "Common/Packet/EFFECT/PacketEFFECT_EFFECTINFO.cpp",
    "Common/Packet/EFFECT/PacketEFFECT_REQ_BALLOONINFO.cpp",
    "Common/Packet/EFFECT/PacketEFFECT_REQ_EFFECTINFO.cpp",
    "Common/Packet/EFFECT/PacketEFFECT_RES_EFFECTINFO.cpp",
    # Packet (SYSTEM)
    "Common/Packet/SYSTEM/PacketSYSTEM_INFO.cpp",
    # Packet (ADMIN)
    "Common/Packet/ADMIN/PacketADMIN_ACCOUNT_REQ_ADD.cpp",
    "Common/Packet/ADMIN/PacketADMIN_CHARINFO.cpp",
    "Common/Packet/ADMIN/PacketADMIN_CHAR_ADDMOTIONTYPE.cpp",
    "Common/Packet/ADMIN/PacketADMIN_CHAR_ADDNPC.cpp",
    "Common/Packet/ADMIN/PacketADMIN_CHAR_MODIFYITEM.cpp",
    "Common/Packet/ADMIN/PacketADMIN_CHAR_MODIFYSKILL.cpp",
    "Common/Packet/ADMIN/PacketADMIN_CHAR_RENEWMOTION.cpp",
    "Common/Packet/ADMIN/PacketADMIN_CHAR_RENEWMOTIONTYPE.cpp",
    "Common/Packet/ADMIN/PacketADMIN_CHAR_RENEWSTATUS.cpp",
    "Common/Packet/ADMIN/PacketADMIN_CHAR_RENEW_ACCOUNT.cpp",
    "Common/Packet/ADMIN/PacketADMIN_CHAR_RENEW_TALKEVENT.cpp",
    "Common/Packet/ADMIN/PacketADMIN_CHAR_REQ_ACCOUNT.cpp",
    "Common/Packet/ADMIN/PacketADMIN_CHAR_RES_ACCOUNT.cpp",
    "Common/Packet/ADMIN/PacketADMIN_CHAR_RES_ONLINE.cpp",
    "Common/Packet/ADMIN/PacketADMIN_DISABLE_RENEWINFO.cpp",
    "Common/Packet/ADMIN/PacketADMIN_DISABLE_RES_INFO.cpp",
    "Common/Packet/ADMIN/PacketADMIN_EFC_RENEWBALLOON.cpp",
    "Common/Packet/ADMIN/PacketADMIN_EFC_RENEWEFFECT.cpp",
    "Common/Packet/ADMIN/PacketADMIN_ITEMTYPE_ADD.cpp",
    "Common/Packet/ADMIN/PacketADMIN_ITEMTYPE_COPY.cpp",
    "Common/Packet/ADMIN/PacketADMIN_ITEMTYPE_DELETE.cpp",
    "Common/Packet/ADMIN/PacketADMIN_ITEMWEAPON_ADD.cpp",
    "Common/Packet/ADMIN/PacketADMIN_ITEMWEAPON_RENEW.cpp",
    "Common/Packet/ADMIN/PacketADMIN_ITEM_ADD.cpp",
    "Common/Packet/ADMIN/PacketADMIN_ITEM_COPY.cpp",
    "Common/Packet/ADMIN/PacketADMIN_ITEM_DELETE.cpp",
    "Common/Packet/ADMIN/PacketADMIN_MAP_COPYPARTS.cpp",
    "Common/Packet/ADMIN/PacketADMIN_MAP_DELETEMAPSHADOW.cpp",
    "Common/Packet/ADMIN/PacketADMIN_MAP_DELETEPARTS.cpp",
    "Common/Packet/ADMIN/PacketADMIN_MAP_EVENT.cpp",
    "Common/Packet/ADMIN/PacketADMIN_MAP_RENEWMAPOBJECT.cpp",
    "Common/Packet/ADMIN/PacketADMIN_MAP_RENEWMAPSIZE.cpp",
    "Common/Packet/ADMIN/PacketADMIN_MAP_RENEWOBJECTDATA.cpp",
    "Common/Packet/ADMIN/PacketADMIN_MAP_SETMAPNAME.cpp",
    "Common/Packet/ADMIN/PacketADMIN_MAP_SETMAPSHADOW.cpp",
    "Common/Packet/ADMIN/PacketADMIN_MAP_SETPARTS.cpp",
    "Common/Packet/ADMIN/PacketADMIN_PARA2.cpp",
    "Common/Packet/ADMIN/PacketADMIN_PLAYSOUND.cpp",
    "Common/Packet/ADMIN/PacketADMIN_RENEWADMINLEVEL.cpp",
    "Common/Packet/ADMIN/PacketADMIN_RENEWMAPPARTS.cpp",
    "Common/Packet/ADMIN/PacketADMIN_RENEWMAPSHADOW.cpp",
    "Common/Packet/ADMIN/PacketADMIN_RENEW_CLIENTVERSION.cpp",
    "Common/Packet/ADMIN/PacketADMIN_REQ_ADMINLEVEL.cpp",
    "Common/Packet/ADMIN/PacketADMIN_REQ_PLAYSOUND.cpp",
    "Common/Packet/ADMIN/PacketADMIN_RES_ADMINLEVEL.cpp",
    "Common/Packet/ADMIN/PacketADMIN_SERVER_SAVEINFO.cpp",
    "Common/Packet/ADMIN/PacketADMIN_SKILL_RENEWSKILL.cpp",
    "Common/Packet/ADMIN/PacketADMIN_SYSTEM_RENEWINFO.cpp",
    # Packet (MSGCMD - StateProcMAP の依存)
    "Common/Packet/MSGCMD/PacketMSGCMD_CHGACCE.cpp",
    "Common/Packet/MSGCMD/PacketMSGCMD_CHGARMS.cpp",
    "Common/Packet/MSGCMD/PacketMSGCMD_CHGCLOTH.cpp",
    "Common/Packet/MSGCMD/PacketMSGCMD_CHGCOLOR.cpp",
    "Common/Packet/MSGCMD/PacketMSGCMD_CHGFACE.cpp",
    "Common/Packet/MSGCMD/PacketMSGCMD_MAKEITEM.cpp",
    "Common/Packet/MSGCMD/PacketMSGCMD_PARA1.cpp",
    # Window (MAP 関連 - StateProcMAP の依存)
    "SboCli/src/Window/WindowCHAT.cpp",
    "SboCli/src/Window/WindowSYSTEMMENU.cpp",
    "SboCli/src/Window/WindowSETCOLOR.cpp",
    "SboCli/src/Window/WindowSETBGMVOLUME.cpp",
    "SboCli/src/Window/WindowSETSOUNDVOLUME.cpp",
    "SboCli/src/Window/WindowSETDRAWMODE.cpp",
    "SboCli/src/Window/WindowITEMMENU.cpp",
    "SboCli/src/Window/WindowITEMMENU_SELECT.cpp",
    "SboCli/src/Window/WindowOPTION.cpp",
    "SboCli/src/Window/WindowOPTION_VIEWSET.cpp",
    "SboCli/src/Window/WindowOPTION_INPUTSET.cpp",
    "SboCli/src/Window/WindowOPTION_TASKBAR.cpp",
    "SboCli/src/Window/WindowOPTION_INPUTSET_SETDEVICE.cpp",
    "SboCli/src/Window/WindowOPTION_ACTIONSET.cpp",
    "SboCli/src/Window/WindowOPTION_ACTIONSET_SLEEPTIMER.cpp",
    "SboCli/src/Window/WindowCOMMANDMENU.cpp",
    "SboCli/src/Window/WindowSKILLMENU.cpp",
    "SboCli/src/Window/WindowPLACEINFORMATION.cpp",
    "SboCli/src/Window/WindowCHAR_STATUS.cpp",
    "SboCli/src/Window/WindowCHAR_STATUS2.cpp",
    "SboCli/src/Window/WindowCHAR_STATUS3.cpp",
    "SboCli/src/Window/WindowCHAR_STATUS4.cpp",
    "SboCli/src/Window/WindowACCOUNTINFO.cpp",
    "SboCli/src/Window/WindowSWOON.cpp",
    "SboCli/src/Window/WindowOPTION_VOLUMESET.cpp",
    # Phase 12: リンクエラー解消用追加ファイル
    "Common/mt19937int.cpp",
    "Common/Info/InfoDisable.cpp",
    "Common/Lib/ParamUtil.cpp",
    "SboCli/src/AdminApi/AdminUiLoader.cpp"
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
    "-O2",
    "-sUSE_SDL=2",
    "-sUSE_SDL_TTF=2",
    "-sALLOW_MEMORY_GROWTH=1",
    "-sASSERTIONS=0",
    "-sSTACK_SIZE=2097152",
    "-sEXPORTED_RUNTIME_METHODS=['ccall','cwrap','HEAPU8']",
    "-sEXPORTED_FUNCTIONS=['_main','_malloc','_free']",
    "-sDISABLE_EXCEPTION_CATCHING=0",
    "-lwebsocket",
    "-sENVIRONMENT=web",
    "-Wl,--error-limit=0",
    "--preload-file", "$resDir@/SboGrpData/res",
    "--preload-file", "$fontDir@/font",
    "--preload-file", "$bgmDir@/BGM",
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

Write-Host "[browser-link] success"
