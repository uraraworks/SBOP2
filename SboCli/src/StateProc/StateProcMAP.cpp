/// @file StateProcMAP.cpp
/// @brief 状態処理クラス(マップ画面) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/10/01
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
#include "LibInfoMapBase.h"
#include "LibInfoItem.h"
#include "InfoItemTypeBase.h"
#include "LibInfoMapParts.h"
#include "ParamUtil.h"
#include "InfoMapBase.h"
#include "InfoMapEventBase.h"
#include "InfoTalkEvent.h"
#include "PacketMAP_REQ_MAPINFO.h"
#include "PacketCHAR_MOVE_START.h"
#include "PacketCHAR_MOVE_DIR_CHANGE.h"
#include "PacketCHAR_MOVE_STOP.h"
#include "PacketCHAR_STATE.h"
#include "PacketCHAR_REQ_CHAT.h"
#include "PacketCHAR_REQ_PUTGET.h"
#include "PacketCHAR_REQ_PUSH.h"
#include "PacketCHAR_REQ_TAIL.h"
#include "PacketCHAR_REQ_MODIFY_PARAM.h"
#include "PacketCHAR_REQ_EQUIP.h"
#include "PacketCHAR_REQ_USEITEM.h"
#include "PacketCHAR_REQ_DRAGITEM.h"
#include "PacketCHAR_PARA1.h"
#include "PacketCONNECT_KEEPALIVE.h"
#include "PacketADMIN_MAP_SETPARTS.h"
#include "PacketADMIN_MAP_SETMAPSHADOW.h"
#include "PacketMSGCMD_CHGFACE.h"
#include "PacketMSGCMD_CHGCLOTH.h"
#include "PacketMSGCMD_CHGACCE.h"
#include "PacketMSGCMD_CHGCOLOR.h"
#include "PacketMSGCMD_CHGARMS.h"
#include "PacketMSGCMD_MAKEITEM.h"
#include "PacketMSGCMD_PARA1.h"
#include "Command.h"
#include "UraraSockTCPSBO.h"
#include "LibInfoCharCli.h"
#include "myString.h"
#include "InfoCharCli.h"
#include "LayerMap.h"
#include "LayerSystemMsg.h"
#include "WindowCHAT.h"
#include "WindowSYSTEMMENU.h"
#include "WindowSETCOLOR.h"
#include "WindowSETBGMVOLUME.h"
#include "WindowSETSOUNDVOLUME.h"
#include "WindowSETDRAWMODE.h"
#include "WindowITEMMENU.h"
#include "WindowITEMMENU_SELECT.h"
#include "WindowOPTION.h"
#include "WindowOPTION_VIEWSET.h"
#include "WindowOPTION_INPUTSET.h"
#include "WindowOPTION_TASKBAR.h"
#include "WindowOPTION_INPUTSET_SETDEVICE.h"
#include "WindowOPTION_ACTIONSET.h"
#include "WindowOPTION_ACTIONSET_SLEEPTIMER.h"
#include "WindowCOMMANDMENU.h"
#include "WindowSKILLMENU.h"
#include "WindowPLACEINFORMATION.h"
#include "WindowCHAR_STATUS.h"
#include "WindowCHAR_STATUS4.h"
#include "Platform/ImGuiMsgLog.h"
#include "Platform/ImGuiDbg.h"
#include "Platform/SDLApp.h"
#include "MainFrame.h"
#include "MgrData.h"

#include "MgrSound.h"
#include "MgrLayer.h"
#include "MgrKeyInput.h"
#include "MgrWindow.h"
#include "MgrDraw.h"
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "StateProcMAP.h"
#include "PacketADMIN_MAP_SELECTPICK.h"

#if defined(__EMSCRIPTEN__)
#include <emscripten/em_js.h>
#include <emscripten/emscripten.h>
EM_JS(void, SBOP2_PostAdminPick, (unsigned int mapId, int cellX, int cellY, unsigned int charId, unsigned int itemId), {
	if (typeof window !== 'undefined' && window.parent && window.parent !== window) {
		window.parent.postMessage({
			kind: 'sbop2_admin_pick',
			mapId: mapId,
			cellX: cellX,
			cellY: cellY,
			charId: charId,
			itemId: itemId
		}, '*');
	}
});

EM_JS(void, SBOP2_PostAdminPickupParts, (unsigned int mapId, int cellX, int cellY, unsigned int partsId), {
	if (typeof window !== 'undefined' && window.parent && window.parent !== window) {
		window.parent.postMessage({
			kind: 'sbop2_admin_pickup_parts',
			mapId: mapId, cellX: cellX, cellY: cellY, partsId: partsId
		}, '*');
	}
});

// 現在のマップが戦闘可能マップ(IsEnableBattle())かどうかの変化を JS へ通知する。
// JS 側 (sbocli-title.shell.html) の window.sbop2OnBattleModeChange(bool) がバーチャルパッドの
// ボタン表記（戦闘可能マップ中: 盾/剣 ⇔ それ以外: ✕/○）を切り替える。
// docs/battle-redesign.md S3: 戦闘モード切替(Tab)廃止に伴い、自キャラの状態でなく
// マップの戦闘可否だけで判定する(状態に関わらずA=防御/B=攻撃を表示するため)。
EM_JS(void, SBOP2_NotifyBattleModeChange, (int bBattle), {
	if (typeof window.sbop2OnBattleModeChange === 'function') {
		window.sbop2OnBattleModeChange(!!bBattle);
	}
});

static CMgrData *s_pMgrDataForAdminMode = NULL;

/// アクティブな CStateProcMAP インスタンスへのグローバルポインタ（ブラウザ版のみ）
/// Create() でセット、デストラクタでクリアする
static CStateProcMAP *s_pBrowserStateProcMAP = NULL;

extern "C" {
	EMSCRIPTEN_KEEPALIVE void SBOP2_SetWebAdminMode(int mode)
	{
		if (s_pMgrDataForAdminMode != NULL) {
			s_pMgrDataForAdminMode->SetWebAdminMode(mode);
		}
	}
	EMSCRIPTEN_KEEPALIVE void SBOP2_SetWebAdminSelectedPartsID(int partsId)
	{
		if (s_pMgrDataForAdminMode != NULL) {
			s_pMgrDataForAdminMode->SetWebAdminSelectedPartsID(static_cast<WORD>(partsId));
		}
	}

	/// @brief Web管理画面からの効果音試聴エントリポイント
	/// @param soundId SOUNDID。CMgrSound未生成時は何もしない
	EMSCRIPTEN_KEEPALIVE void SBOP2_AdminPlaySound(int soundId)
	{
		if ((s_pMgrDataForAdminMode != NULL) && (s_pMgrDataForAdminMode->GetMgrSound() != NULL)) {
			s_pMgrDataForAdminMode->GetMgrSound()->PlaySound(static_cast<DWORD>(soundId));
		}
	}

	/// @brief DOM(JS)側からのチャット送信エントリポイント
	/// @param pszText 送信テキスト（UTF-8。空文字列なら何もしない）
	/// @param nType   チャット種別。負値は 0 に丸める
	EMSCRIPTEN_KEEPALIVE void SBOP2_BrowserChatSubmit(const char *pszText, int nType)
	{
		if (s_pBrowserStateProcMAP != NULL) {
			s_pBrowserStateProcMAP->BrowserChatSubmit(pszText, nType);
		}
	}
}
#else
static void SBOP2_PostAdminPick(unsigned int, int, int, unsigned int, unsigned int)
{
}
static void SBOP2_PostAdminPickupParts(unsigned int, int, int, unsigned int)
{
}
static void SBOP2_NotifyBattleModeChange(int)
{
}
#endif

namespace {

static BOOL IsLeftMousePressed(void)
{
	return (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LMASK) ? TRUE : FALSE;
}

static DWORD FindAdminClickedCharID(CLibInfoCharCli *pLibInfoChar, PCLayerMap pLayerMap, int nScreenClickX, int nScreenClickY)
{
	int i, nCount, nBestScore;
	DWORD dwCharID;

	if ((pLibInfoChar == NULL) || (pLayerMap == NULL)) {
		return 0;
	}

	dwCharID = 0;
	nBestScore = INT_MAX;
	nCount = pLibInfoChar->GetCount();
	for (i = 0; i < nCount; i ++) {
		PCInfoCharCli pInfoChar;
		PCInfoMotion pInfoMotion;
		POINT ptDrawMapPos;
		POINT ptViewCharPos;
		RECT rcChar;
		int nDrawX, nDrawY;
		int nHitW, nHitH;
		int nCenterX, nCenterY;
		int nScore;

		pInfoChar = (PCInfoCharCli)pLibInfoChar->GetPtr(i);
		if (pInfoChar == NULL) {
			continue;
		}
		pInfoMotion = pInfoChar->GetMotionInfo();
		pInfoChar->GetDrawMapPos(ptDrawMapPos);
		pInfoChar->GetViewCharPos(ptViewCharPos);

		nDrawX = 32 - ptViewCharPos.x + ptDrawMapPos.x - pLayerMap->m_nViewX;
		nDrawY = 32 - ptViewCharPos.y + ptDrawMapPos.y - pLayerMap->m_nViewY - HALF_TILE;
		if (pInfoMotion != NULL) {
			nDrawX += pInfoMotion->m_ptDrawPosPile0.x;
			nDrawY += pInfoMotion->m_ptDrawPosPile0.y;
		}

		nHitW = pInfoChar->m_nGrpSize * 2;
		nHitH = pInfoChar->m_nGrpSize * 2;
		if (nHitW < MAPPARTSSIZE) {
			nHitW = MAPPARTSSIZE;
		}
		if (nHitH < MAPPARTSSIZE * 2) {
			nHitH = MAPPARTSSIZE * 2;
		}
		SetRect(
			&rcChar,
			nDrawX - 8,
			nDrawY - 8,
			nDrawX + nHitW + 7,
			nDrawY + nHitH + 7);
		if ((nScreenClickX < rcChar.left) || (nScreenClickX > rcChar.right) ||
			(nScreenClickY < rcChar.top) || (nScreenClickY > rcChar.bottom)) {
			continue;
		}

		nCenterX = nDrawX + (nHitW / 2);
		nCenterY = nDrawY + (nHitH / 2);
		nScore = abs(nCenterX - nScreenClickX) + abs(nCenterY - nScreenClickY);
		if (nScore >= nBestScore) {
			continue;
		}
		nBestScore = nScore;
		dwCharID = pInfoChar->m_dwCharID;
	}
	return dwCharID;
}

static double NormalizeMoveAngle(double dAngle)
{
	while (dAngle <= -3.14159265358979323846) {
		dAngle += 6.28318530717958647692;
	}
	while (dAngle > 3.14159265358979323846) {
		dAngle -= 6.28318530717958647692;
	}
	return dAngle;
}

static double GetDirectionAngle(int nDirection)
{
	int anPosChangeX[] = {0, 0, -1, 1, 1, 1, -1, -1};
	int anPosChangeY[] = {-1, 1, 0, 0, -1, 1, 1, -1};

	if ((nDirection < 0) || (nDirection > 7)) {
		return 0.0;
	}
	return atan2((double)anPosChangeY[nDirection], (double)anPosChangeX[nDirection]);
}

static int GetAngleDirection(double dAngle)
{
	double dBestDiff, dDiff, dTargetAngle;
	int i, nBestDirection;

	nBestDirection = 0;
	dBestDiff = 100.0;
	for (i = 0; i < 8; i ++) {
		dTargetAngle = GetDirectionAngle(i);
		dDiff = fabs(NormalizeMoveAngle(dTargetAngle - dAngle));
		if (dDiff < dBestDiff) {
			dBestDiff = dDiff;
			nBestDirection = i;
		}
	}
	return nBestDirection;
}

}	// namespace


CStateProcMAP::CStateProcMAP()
{
	m_bChgScrollMode		= FALSE;
	m_bSendCheckMapEvent	= FALSE;
	m_nScrollMode			= 0;
	m_nLastNotifiedBattleMode = -1;
	m_dwLastTimeMove		= 0;
	m_dwLastTimeKeepAlive	= 0;
	m_dwLastBalloonID		= 0;
	m_dwLastKeyInput		= 0;
	m_dwLastTimeMoveSyncSend = 0;
	m_dwLastAtackTime		= 0;
	m_bAtackKeyAutoRepeat	= FALSE;
	m_bZKeyDefenseActive	= FALSE;
	m_bMoveSyncActive			= FALSE;
	m_nMoveSyncDirection		= -1;
	m_bPushSyncActive			= FALSE;
	m_dwPushSyncObjCharID		= 0;
	m_nPushSyncDirection		= -1;
	m_dwLastTimePushSyncSend	= 0;
	m_dwLastTimePushContact	= 0;
	m_nLastPushSyncSentX		= 0;
	m_nLastPushSyncSentY		= 0;
	m_bLastPushSyncSentValid	= FALSE;
	m_bPushReleaseSent			= FALSE;
	// S5: 入れ替わり(SWAP)予測状態。docs/push-object-redesign.md 2章7項
	m_bPushSwapActive			= FALSE;
	m_dwPushSwapObjCharID		= 0;
	m_nPushSwapDirection		= -1;
	m_ptPushSwapP0.x = m_ptPushSwapP0.y = 0;
	m_ptPushSwapB0.x = m_ptPushSwapB0.y = 0;
	m_dwLastTimePushSwapSend	= 0;
	m_nMoveSpeedAccum			= 0;
	m_dwLastPlayerMoveStepTime	= 0;
	m_dwLastPlayerMoveTurnTime	= 0;
	m_dwLastEventMapID		= 0;
	m_bHasLastEventTile		= FALSE;
	m_bHasPlayerMoveHeading	= FALSE;
	m_bNeedIdleMapEventCheck = FALSE;
	m_nLastEventTileX		= 0;
	m_nLastEventTileY		= 0;
	m_dPlayerMoveHeading	= 0.0;
	m_bAutoWalkToEvent	= FALSE;
	m_nAutoWalkTargetX	= 0;
	m_nAutoWalkTargetY	= 0;

	m_pPlayerChar		= NULL;
	m_pMap				= NULL;
	m_pLibInfoChar		= NULL;
	m_pLibInfoMap		= NULL;
	m_pLibInfoItem		= NULL;
	m_pImGuiMsgLog		= NULL;
	m_pImGuiDbg			= NULL;
}



CStateProcMAP::~CStateProcMAP()
{
	DestroyAdminUi();
#if defined(__EMSCRIPTEN__)
	// チャット送信ブリッジ用グローバルポインタをクリア（破棄後のアクセスを防ぐ）
	if (s_pBrowserStateProcMAP == this) {
		s_pBrowserStateProcMAP = NULL;
	}
#else
	// マップ画面終了時にデバッグ/ログのサブウィンドウを破棄する
	if (CSDLApp::GetInstance() != NULL) {
		// ポインタクリアを先に行い、破棄後のアクセスを防ぐ
		CSDLApp::GetInstance()->SetMsgLogPtr(NULL);
		CSDLApp::GetInstance()->HideImGuiSubWindows();
	}
#endif
	m_pMgrSound->StopBGM();
}



void CStateProcMAP::Create(CMgrData *pMgrData, CUraraSockTCPSBO *pSock)
{
	CStateProcBase::Create(pMgrData, pSock);

	m_pLibInfoChar		= pMgrData->GetLibInfoChar();
	m_pLibInfoMap		= pMgrData->GetLibInfoMap();
	m_pLibInfoItem		= pMgrData->GetLibInfoItem();

#if defined(__EMSCRIPTEN__)
	s_pMgrDataForAdminMode = pMgrData;
	s_pBrowserStateProcMAP = this;  // チャット送信ブリッジ用にインスタンスを登録
#endif

	m_pImGuiMsgLog = new CImGuiMsgLog;
	m_pImGuiMsgLog->Init(m_pMgrData);
	m_pImGuiDbg = new CImGuiDbg;
	m_pImGuiDbg->Init(m_pMgrData);
}



void CStateProcMAP::Init(void)
{
	m_pPlayerChar	= m_pMgrData->GetPlayerChar();
	m_pMap			= m_pMgrData->GetMap();
	// マップ入り直後は必ず現在値をJSへ通知し直す（未通知状態に戻す）
	m_nLastNotifiedBattleMode = -1;
	m_dwLastEventMapID = 0;
	m_bHasLastEventTile = FALSE;
	m_bAutoWalkToEvent = FALSE;
	m_bNeedIdleMapEventCheck = FALSE;

	m_pMgrLayer->MakeMAP();
	m_pMgrLayer->MakeSYSTEMMSG();

	CreateAdminUi();

#if !defined(__EMSCRIPTEN__)
	// ゲーム画面遷移時にデバッグ/ログのサブウィンドウを生成・表示する
	if (CSDLApp::GetInstance() != NULL) {
		CSDLApp::GetInstance()->ShowImGuiSubWindows();
		// ログ窓の自前入力実装にメッセージログポインタを渡す
		CSDLApp::GetInstance()->SetMsgLogPtr(m_pImGuiMsgLog);
	}
#endif

	m_pMgrDraw->SetFadeState(FADESTATE_FADEIN);
	m_nMoveSpeedAccum = 0;
	m_dwLastPlayerMoveStepTime = 0;
	m_dwLastPlayerMoveTurnTime = 0;
	m_bHasPlayerMoveHeading = FALSE;

	if (m_pMap) {
		m_pMgrSound->PlayBGM(m_pMap->m_dwBGMID);
	} else {
		m_pMgrSound->StopBGM();
	}
}



void CStateProcMAP::GetMsgLogRect(RECT &rcDst)
{
	UNREFERENCED_PARAMETER(rcDst);
}


namespace {

static DWORD GetPlayerMoveWaitBase(PCInfoCharCli pPlayerChar)
{
	DWORD dwMoveWait;

	if (pPlayerChar == NULL) {
		return 11;
	}
	dwMoveWait = pPlayerChar->GetMoveWait();
	if (dwMoveWait == 0) {
		return 11;
	}
	return dwMoveWait;
}

static int GetPlayerMovePixelsPerSec(PCInfoCharCli pPlayerChar)
{
	DWORD dwMoveWait;
	ULONGLONG ullSpeed;

	dwMoveWait = GetPlayerMoveWaitBase(pPlayerChar);
	ullSpeed = (ULONGLONG)CHAR_MOVE_PIXELS_PER_SEC * 11;
	ullSpeed = (ullSpeed + dwMoveWait - 1) / dwMoveWait;
	if (ullSpeed == 0) {
		return 1;
	}
	if (ullSpeed > INT_MAX) {
		return INT_MAX;
	}
	return (int)ullSpeed;
}

}	// namespace



int CStateProcMAP::GetPlayerMoveStep(DWORD dwNowTime, int &nAccumOut, DWORD &dwLastStepTimeOut)
{
	ULONGLONG ullAccumulated;
	int nMoveStep;
	int nMovePixelsPerSec;
	DWORD dwElapsed;

	if (dwLastStepTimeOut == 0) {
		dwLastStepTimeOut = dwNowTime;
		return 0;
	}
	dwElapsed = dwNowTime - dwLastStepTimeOut;
	dwLastStepTimeOut = dwNowTime;
	if (dwElapsed == 0) {
		return 0;
	}

	/* ヒッチ（タブ非表示・GC等）明けの追いつき移動でサーバーの速度超過チェック
	   （120px/s＋32px余裕）を超えないよう、1回の加算分を頭打ちにする */
	if (dwElapsed > 250) {
		dwElapsed = 250;
	}

	nMovePixelsPerSec = GetPlayerMovePixelsPerSec(m_pPlayerChar);
	ullAccumulated = (ULONGLONG)nAccumOut + (ULONGLONG)dwElapsed * nMovePixelsPerSec;
	nMoveStep = (int)(ullAccumulated / 1000);
	nAccumOut = (int)(ullAccumulated % 1000);

#if SBO_ENABLE_POS_SYNC_DEBUG_LOG
	// [DBG-MOVESTEP] 大移動（5px超）のときのみログ出力してログ欄肥大を防ぐ
	if (nMoveStep > 5) {
		DWORD dwMoveWaitLog = GetPlayerMoveWaitBase(m_pPlayerChar);
		SboDbgLog("[GetPlayerMoveStep] dwElapsed=%u dwMoveWait=%u nMovePixelsPerSec=%d nMoveStep=%d",
			dwElapsed, dwMoveWaitLog, nMovePixelsPerSec, nMoveStep);
	}
#endif

	return nMoveStep;
}



int CStateProcMAP::GetSmoothedMoveDirection(int nTargetDirection, DWORD dwNowTime)
{
	double dTargetAngle, dDiff, dStep, dDt;

	if ((nTargetDirection < 0) || (nTargetDirection > 7)) {
		m_bHasPlayerMoveHeading = FALSE;
		m_dwLastPlayerMoveTurnTime = 0;
		return nTargetDirection;
	}

	dTargetAngle = GetDirectionAngle(nTargetDirection);
	if ((m_bHasPlayerMoveHeading == FALSE) || (m_dwLastPlayerMoveTurnTime == 0)) {
		m_dPlayerMoveHeading = dTargetAngle;
		m_bHasPlayerMoveHeading = TRUE;
		m_dwLastPlayerMoveTurnTime = dwNowTime;
		return nTargetDirection;
	}

	dDt = (double)(dwNowTime - m_dwLastPlayerMoveTurnTime) / 1000.0;
	m_dwLastPlayerMoveTurnTime = dwNowTime;
	if (dDt < 0.0) {
		dDt = 0.0;
	}
	if (dDt > 0.1) {
		dDt = 0.1;
	}

	dStep = 7.0 * dDt;
	dDiff = NormalizeMoveAngle(dTargetAngle - m_dPlayerMoveHeading);
	if (fabs(dDiff) <= dStep) {
		m_dPlayerMoveHeading = dTargetAngle;
	} else if (dDiff > 0.0) {
		m_dPlayerMoveHeading = NormalizeMoveAngle(m_dPlayerMoveHeading + dStep);
	} else {
		m_dPlayerMoveHeading = NormalizeMoveAngle(m_dPlayerMoveHeading - dStep);
	}
	return GetAngleDirection(m_dPlayerMoveHeading);
}



void CStateProcMAP::SyncLastEventTile(DWORD dwMapID, int x, int y)
{
	m_dwLastEventMapID = dwMapID;
	m_nLastEventTileX = x / MAPPARTSSIZE;
	m_nLastEventTileY = y / MAPPARTSSIZE;
	m_bHasLastEventTile = TRUE;
}



void CStateProcMAP::ResetMapEventCheckSendState(void)
{
	m_bSendCheckMapEvent = FALSE;
}

void CStateProcMAP::OnPushSwapRejected(DWORD dwObjCharID)
{
	// S5: 入れ替わり中のボールがRES_PUSHで却下された時の後始末(親レビュー指摘)。
	// 呼び出し元(MainFrameRecvProcCHAR.cpp)は却下時に確定座標へSetPos済み、
	// m_bPushPredictingもFALSE済みなので、ここでは入れ替わり状態(m_bPushSwapActive
	// 等)だけを片付ける(bClearOwnership=FALSE。所有者クリアは呼び出し元に委ねる
	// 必要はなく、既にm_bPushPredicting=FALSEなので二重処理にならない)。
	if (m_bPushSwapActive == FALSE) {
		return;
	}
	if (m_dwPushSwapObjCharID != dwObjCharID) {
		return;
	}
	DiscardPushSwapState(FALSE, "reject");
}

void CStateProcMAP::ResetPlayerMoveSyncState(void)
{
	if (m_pPlayerChar) {
		m_pPlayerChar->m_ptMove.x = 0;
		m_pPlayerChar->m_ptMove.y = 0;
		m_pPlayerChar->m_bWaitCheckMapEvent = FALSE;
		m_pPlayerChar->ClearDrawDirectionOverride();
	}
	m_bMoveSyncActive = FALSE;
	m_nMoveSyncDirection = -1;
	m_dwLastTimeMoveSyncSend = 0;
	m_nMoveSpeedAccum = 0;
	m_dwLastPlayerMoveStepTime = 0;
	m_dwLastPlayerMoveTurnTime = 0;
	m_bHasPlayerMoveHeading = FALSE;
	m_bAutoWalkToEvent = FALSE;
	m_bNeedIdleMapEventCheck = FALSE;
	m_bSendCheckMapEvent = FALSE;
	EndPushPredict(FALSE);	// S3b: マップ切替等では最終送信せずローカル予測状態だけ破棄する
	DiscardPushSwapState(TRUE, "map");	// S5: マップ切替は入れ替わりの一時停止では済まないので、ここで明示的に手放す(親レビュー指摘)
}


void CStateProcMAP::StartAutoWalkToEvent(int nTileX, int nTileY)
{
	int i, nCount;
	PCInfoMapEventBase pEvent;

	if ((m_pMap != NULL) && (m_pPlayerChar != NULL)) {
		RECT rcChar, rcEvent;
		int nCharCenterX, nCharCenterY, nEventType;
		int nLeft, nTop, nRight, nBottom;

		m_pPlayerChar->GetCollisionRect(rcChar);
		nCharCenterX = (rcChar.left + rcChar.right) / 2;
		nCharCenterY = (rcChar.top + rcChar.bottom) / 2;
		nLeft = nRight = nTileX;
		nTop = nBottom = nTileY;
		nCount = m_pMap->GetEventCount();
		for (i = 0; i < nCount; i ++) {
			pEvent = m_pMap->GetEvent(i);
			if (pEvent == NULL) {
				continue;
			}

			nLeft = min(pEvent->m_ptPos.x, pEvent->m_ptPos2.x);
			nTop = min(pEvent->m_ptPos.y, pEvent->m_ptPos2.y);
			nRight = max(pEvent->m_ptPos.x, pEvent->m_ptPos2.x);
			nBottom = max(pEvent->m_ptPos.y, pEvent->m_ptPos2.y);
			if (pEvent->m_nHitType != MAPEVENTHITTYPE_AREA) {
				nLeft = nRight = pEvent->m_ptPos.x;
				nTop = nBottom = pEvent->m_ptPos.y;
			}
			if ((nTileX < nLeft) || (nTileX > nRight) ||
				(nTileY < nTop) || (nTileY > nBottom)) {
				continue;
			}
			break;
		}
		if (i >= nCount) {
			nLeft = nRight = nTileX;
			nTop = nBottom = nTileY;
		}

		nEventType = m_pMap->GetMapEventType(nTileX, nTileY);
		while ((nLeft > 0) && (m_pMap->GetMapEventType(nLeft - 1, nTileY) == nEventType)) {
			nLeft --;
		}
		while ((nRight + 1 < m_pMap->m_sizeMap.cx) && (m_pMap->GetMapEventType(nRight + 1, nTileY) == nEventType)) {
			nRight ++;
		}
		while ((nTop > 0) && (m_pMap->GetMapEventType(nTileX, nTop - 1) == nEventType)) {
			nTop --;
		}
		while ((nBottom + 1 < m_pMap->m_sizeMap.cy) && (m_pMap->GetMapEventType(nTileX, nBottom + 1) == nEventType)) {
			nBottom ++;
		}

		SetRect(
			&rcEvent,
			nLeft * MAPPARTSSIZE,
			nTop * MAPPARTSSIZE,
			(nRight + 1) * MAPPARTSSIZE - 1,
			(nBottom + 1) * MAPPARTSSIZE - 1);
		if (nLeft < nRight) {
			int nOverlapLeft, nOverlapRight, nTargetX;

			nOverlapLeft = max(rcChar.left, rcEvent.left);
			nOverlapRight = min(rcChar.right, rcEvent.right);
			nTargetX = ((nOverlapLeft <= nOverlapRight) ? ((nOverlapLeft + nOverlapRight) / 2) : nCharCenterX) / MAPPARTSSIZE;
			nTileX = min(max(nTargetX, nLeft), nRight);
		}
		if (nTop < nBottom) {
			int nOverlapTop, nOverlapBottom, nTargetY;

			nOverlapTop = max(rcChar.top, rcEvent.top);
			nOverlapBottom = min(rcChar.bottom, rcEvent.bottom);
			nTargetY = ((nOverlapTop <= nOverlapBottom) ? ((nOverlapTop + nOverlapBottom) / 2) : nCharCenterY) / MAPPARTSSIZE;
			nTileY = min(max(nTargetY, nTop), nBottom);
		}
	}

	// タイル中央ピクセル座標を設定
	// ヒットボックス形状: left=X+8, right=X+23, top=Y-15, bottom=Y
	// X: タイル中央(nTileX*32+16) にヒットボックス中央(X+15)を合わせる
	// Y: ヒットボックス下端(Y)をタイル下端((nTileY+1)*32-1) に合わせる
	m_nAutoWalkTargetX = nTileX * MAPPARTSSIZE;
	m_nAutoWalkTargetY = nTileY * MAPPARTSSIZE + (MAPPARTSSIZE / 2) + ((HALF_TILE - 1) / 2);
	m_bAutoWalkToEvent = TRUE;
	m_bNeedIdleMapEventCheck = FALSE;
	m_bSendCheckMapEvent = FALSE;
}



void CStateProcMAP::ProcAutoWalkToEvent(void)
{
	int i, nMoveStep, nMoveSpeedAccum, nDx, nDy, nDirection, nDistX, nDistY;
	DWORD dwMoveStepTime;
	BOOL bResult;

	if (m_pPlayerChar == NULL) {
		return;
	}

	nDistX = m_nAutoWalkTargetX - m_pPlayerChar->m_nMapX;
	nDistY = m_nAutoWalkTargetY - m_pPlayerChar->m_nMapY;

	// 到達チェック
	if (nDistX == 0 && nDistY == 0) {
		m_bAutoWalkToEvent = FALSE;
		m_pPlayerChar->m_bWaitCheckMapEvent = TRUE;
		m_bSendCheckMapEvent = FALSE;
		return;
	}

	// 移動方向を決定
	nDx = (nDistX > 0) ? 1 : (nDistX < 0 ? -1 : 0);
	nDy = (nDistY > 0) ? 1 : (nDistY < 0 ? -1 : 0);
	if      (nDy < 0 && nDx == 0) nDirection = 0;	// 上
	else if (nDy > 0 && nDx == 0) nDirection = 1;	// 下
	else if (nDy == 0 && nDx < 0) nDirection = 2;	// 左
	else if (nDy == 0 && nDx > 0) nDirection = 3;	// 右
	else if (nDy < 0 && nDx > 0)  nDirection = 4;	// 右上
	else if (nDy > 0 && nDx > 0)  nDirection = 5;	// 右下
	else if (nDy > 0 && nDx < 0)  nDirection = 6;	// 左下
	else                           nDirection = 7;	// 左上

	// 通常移動と同じ速度でステップ数を取得
	nMoveSpeedAccum = m_nMoveSpeedAccum;
	dwMoveStepTime  = m_dwLastPlayerMoveStepTime;
	nMoveStep = GetPlayerMoveStep(timeGetTime(), nMoveSpeedAccum, dwMoveStepTime);
	m_nMoveSpeedAccum = nMoveSpeedAccum;
	m_dwLastPlayerMoveStepTime = dwMoveStepTime;

	for (i = 0; i < nMoveStep; i++) {
		int dx = nDx, dy = nDy;
		BOOL bSyncSend;
		nDistX = m_nAutoWalkTargetX - m_pPlayerChar->m_nMapX;
		nDistY = m_nAutoWalkTargetY - m_pPlayerChar->m_nMapY;
			// 目標に到達した軸は動かさない
		if (dx != 0 && nDistX == 0) dx = 0;
		if (dy != 0 && nDistY == 0) dy = 0;
		if (dx == 0 && dy == 0) {
			break;
		}
		bSyncSend = ((m_pPlayerChar->m_nMapX + dx) == m_nAutoWalkTargetX) &&
					((m_pPlayerChar->m_nMapY + dy) == m_nAutoWalkTargetY);

		bResult = MoveProc(m_pPlayerChar->m_nMapX, m_pPlayerChar->m_nMapY,
		                    dx, dy, nDirection, bSyncSend);
		if (!bResult) {
				// 移動失敗: 現在位置でイベントチェックへ進む
			m_bAutoWalkToEvent = FALSE;
			m_pPlayerChar->m_bWaitCheckMapEvent = TRUE;
			m_bSendCheckMapEvent = FALSE;
			return;
		}
			// 各ステップで到達チェック
		if (m_pPlayerChar->m_nMapX == m_nAutoWalkTargetX &&
		    m_pPlayerChar->m_nMapY == m_nAutoWalkTargetY) {
			m_bAutoWalkToEvent = FALSE;
			m_pPlayerChar->m_bWaitCheckMapEvent = TRUE;
			m_bSendCheckMapEvent = FALSE;
			return;
		}
	}
}



BOOL CStateProcMAP::TimerProc(void)
{
	BOOL bRet, bResult;
	DWORD dwTime;
	PCLayerMap pLayerMap;
	PCMgrKeyInput pMgrKeyInput;

	bRet = FALSE;
	m_pPlayerChar = m_pMgrData->GetPlayerChar();

	{
		// docs/battle-redesign.md S3: 戦闘モード切替(Tab)は既に廃止済みで「戦闘状態か」という
		// 概念はもう無いため、パッドのA/B表記は自キャラの状態でなく「戦闘可能マップかどうか」
		// (m_pMap->IsEnableBattle())だけで決める(マップ切替等でも追従するよう毎フレーム判定)
		BOOL bIsBattleMode = (m_pMap && m_pMap->IsEnableBattle()) ? TRUE : FALSE;
		if ((int)bIsBattleMode != m_nLastNotifiedBattleMode) {
			m_nLastNotifiedBattleMode = (int)bIsBattleMode;
			SBOP2_NotifyBattleModeChange(bIsBattleMode);
		}
	}

	if (m_dwLastTimeMove != 0) {
		dwTime = timeGetTime() - m_dwLastTimeMove;
		if (dwTime > 2 * 1000) {
			pLayerMap = (PCLayerMap)m_pMgrLayer->Get(LAYERTYPE_MAP);
			if (pLayerMap->m_nSystemIconMode != 0) {
				pLayerMap->SetSystemIconMode(0);
				bRet = TRUE;
			}
		}
	}

	// S3b: 押し予測タイムアウト。壁際で押しが塞がれた等でMoveProcが押しを続けられない
	// フレームが続いても、300ms以内なら接触継続とみなして予測を保つ
	// (docs/push-object-redesign.md 4章。1フレームでも即終了させると軸が合わない
	// 一瞬の入力揺れで予測がちらつく)。300msを超えたら停止とみなし、予測を終える。
	if (m_bPushSyncActive && (timeGetTime() - m_dwLastTimePushContact > 300)) {
		EndPushPredict(FALSE);
	}
	// S5: 入れ替わり中の一時停止(接触喪失/キー解放)は上のEndPushPredictでは
	// 手放さない(m_bPushSwapActiveを保つ)ので、そのままだと押し合いを離れた人が
	// いつまでも専有し続けてしまう。サーバーは最後のSWAP受信から2000msで
	// 入れ替わりを打ち切るので、それより先にクライアント側も手放す
	// (docs/push-object-redesign.md 2章7項。親レビュー指摘)。
	if (m_bPushSwapActive && (timeGetTime() - m_dwLastTimePushSwapSend > 1800)) {
		DiscardPushSwapState(TRUE, "timeout");
	}

	dwTime = timeGetTime() - m_dwLastTimeKeepAlive;
	if (dwTime > 20 * 1000) {
		CPacketCONNECT_KEEPALIVE Packet;

		/* 生存確認通知を送信 */
		m_dwLastTimeKeepAlive = timeGetTime();
		Packet.Make(m_dwLastTimeKeepAlive);
		m_pSock->Send(&Packet);
	}

	if (m_pPlayerChar) {
		pMgrKeyInput = m_pMgrData->GetMgrKeyInput();
		bResult = pMgrKeyInput->IsInput('Z');
		if (m_pMgrWindow->IsKeyInput()) {
			bResult = FALSE;
		}
		if (m_pPlayerChar->m_nMoveState == CHARMOVESTATE_BATTLE_DEFENSE) {
			if (bResult == FALSE) {
				/* 防御解除 */
				DefenseOff();
			}
		} else {
			if (bResult) {
				OnZ(TRUE);
			}
		}
		/* 繧､繝吶Φ繝医ち繧､繝ｫ縺ｸ縺ｮ閾ｪ蜍墓ｭｩ陦悟・逅・*/
		if (m_bAutoWalkToEvent) {
			ProcAutoWalkToEvent();
		}
		if (!m_bAutoWalkToEvent &&
			m_bNeedIdleMapEventCheck &&
			!m_pPlayerChar->m_bWaitCheckMapEvent &&
			(m_pPlayerChar->IsStateMove() == FALSE)) {
			BOOL bMoveKeyOn;

			bMoveKeyOn = pMgrKeyInput->IsInput(VK_UP) ||
						 pMgrKeyInput->IsInput(VK_DOWN) ||
						 pMgrKeyInput->IsInput(VK_LEFT) ||
						 pMgrKeyInput->IsInput(VK_RIGHT);
			if (!bMoveKeyOn) {
				m_pPlayerChar->m_bWaitCheckMapEvent = TRUE;
				m_bSendCheckMapEvent = FALSE;
				m_bNeedIdleMapEventCheck = FALSE;
			}
		}
		if (m_pPlayerChar->m_bWaitCheckMapEvent && (m_bSendCheckMapEvent == FALSE)) {
			CPacketCHAR_MOVE_DIR_CHANGE PacketMoveDirChange;
			CPacketCHAR_MOVE_STOP PacketMoveStop;
			CPacketCHAR_PARA1 PacketPara1;

			/* 判定要求の直前に現在位置を強制同期して、サーバー側の取りこぼしを防ぐ。
			   サーバーからの状態反映が一瞬 STAND に戻っても、MoveSync 継続中に
			   MOVE_STOP を送ると他クライアント側の予測移動まで止まるため、
			   Dead Reckoning 送信中は常に MOVE_DIR_CHANGE(update=TRUE) で同期する。 */
			if (m_bMoveSyncActive) {
				PacketMoveDirChange.Make(
					m_pPlayerChar->m_dwMapID,
					m_pPlayerChar->m_dwCharID,
					m_pPlayerChar->m_nDirection,
					m_pPlayerChar->m_nMapX,
					m_pPlayerChar->m_nMapY,
					TRUE,
					1,
					timeGetTime());
				m_pSock->Send(&PacketMoveDirChange);
				m_dwLastTimeMoveSyncSend = timeGetTime();
			} else {
				PacketMoveStop.Make(
					m_pPlayerChar->m_dwMapID,
					m_pPlayerChar->m_dwCharID,
					m_pPlayerChar->m_nDirection,
					m_pPlayerChar->m_nMapX,
					m_pPlayerChar->m_nMapY,
					FALSE,
					0,
					timeGetTime());
				m_pSock->Send(&PacketMoveStop);
			}

			/* Phase 8: 自由移動では停止タイミング依存にすると取りこぼすため、待機フラグ時に即チェック要求する */
			PacketPara1.Make(SBOCOMMANDID_SUB_CHAR_REQ_CHECKMAPEVENT, m_pPlayerChar->m_dwCharID, 0);
			m_pSock->Send(&PacketPara1);
			m_bSendCheckMapEvent = TRUE;
		}
	}

	/* おひるねタイマー処理 */
	TimerProcSleepTimer();
	/* Xキー押しっぱなしの連続攻撃 */
	TimerProcAtackRepeat();
	/* 無操作で戦闘状態を自動解除 */
	TimerProcAtackAutoOff();

	return bRet;
}



void CStateProcMAP::KeyProc(
	BYTE byCode,		// [in] イベント
	BOOL bDown)			// [in] 押下状態
{
	m_pPlayerChar = m_pMgrData->GetPlayerChar();

	if ((bDown == FALSE) &&
		((byCode == VK_UP) || (byCode == VK_DOWN) || (byCode == VK_LEFT) || (byCode == VK_RIGHT))) {
		PCMgrKeyInput pMgrKeyInput;
		BOOL bUp, bDownKey, bLeft, bRight;

		pMgrKeyInput = m_pMgrData->GetMgrKeyInput();
		if (pMgrKeyInput && m_pPlayerChar) {
			bUp = pMgrKeyInput->IsInput(VK_UP);
			bDownKey = pMgrKeyInput->IsInput(VK_DOWN);
			bLeft = pMgrKeyInput->IsInput(VK_LEFT);
			bRight = pMgrKeyInput->IsInput(VK_RIGHT);
			if (!(bUp || bDownKey || bLeft || bRight)) {
				int nStopState;

				nStopState = CHARMOVESTATE_STAND;
				if (m_pPlayerChar->IsStateBattle()) {
					nStopState = CHARMOVESTATE_BATTLE;
					if (m_pPlayerChar->m_nMoveState == CHARMOVESTATE_BATTLE_DEFENSE) {
						nStopState = CHARMOVESTATE_BATTLE_DEFENSE;
					}
				}
				if (m_pPlayerChar->IsStateMove()) {
					m_pPlayerChar->ChgMoveState(nStopState);
				}
				m_pPlayerChar->m_ptMove.x = 0;
				m_pPlayerChar->m_ptMove.y = 0;
				m_nMoveSpeedAccum = 0;
				m_dwLastPlayerMoveStepTime = 0;
				m_dwLastPlayerMoveTurnTime = 0;
				m_bHasPlayerMoveHeading = FALSE;
				m_pPlayerChar->ClearDrawDirectionOverride();

				if (m_bMoveSyncActive) {
					CPacketCHAR_MOVE_STOP PacketMoveStop;
					PacketMoveStop.Make(
						m_pPlayerChar->m_dwMapID,
						m_pPlayerChar->m_dwCharID,
						m_pPlayerChar->m_nDirection,
							m_pPlayerChar->m_nMapX,
							m_pPlayerChar->m_nMapY,
						FALSE,
						0,
						timeGetTime());
					m_pSock->Send(&PacketMoveStop);
					m_bMoveSyncActive = FALSE;
					m_nMoveSyncDirection = -1;
					m_dwLastTimeMoveSyncSend = 0;
				}
				// S3b: 移動キーを全て離した＝押すのもやめたとみなし、最終座標を1回送ってから
				// 予測を終える(docs/push-object-redesign.md 4章「送信」参照)。
				EndPushPredict(TRUE);
				if (!m_bAutoWalkToEvent) {
					/* 接触位置で止まっただけでも、停止直後にイベント判定を要求する */
					m_pPlayerChar->m_bWaitCheckMapEvent = TRUE;
					m_bSendCheckMapEvent = FALSE;
					m_bNeedIdleMapEventCheck = FALSE;
				}
			}
		}
	}

	if ((byCode == 0) && (bDown == FALSE)) {
		/* おひるねタイマー解除の為に時間更新 */
		m_dwLastKeyInput = timeGetTime();
		return;
	}

	CStateProcBase::KeyProc(byCode, bDown);
}



void CStateProcMAP::OnWindowMsg(int nType, DWORD dwPara)
{
	BOOL bClose;

	bClose = FALSE;
	switch (nType) {
	case WINDOWTYPE_CHAT:							bClose = OnWindowMsgCHAT(dwPara);	break;	// チャット入力
	case WINDOWTYPE_SYSTEMMENU:						bClose = OnWindowMsgSYSTEMMENU(dwPara);	break;	// システムメニュー
	case WINDOWTYPE_SETCOLOR:						bClose = OnWindowMsgSETCOLOR(dwPara);	break;	// 名前と発言色の設定
	case WINDOWTYPE_SETDRAWMODE:					bClose = OnWindowMsgSETDRAWMODE(dwPara);	break;	// 表示内容の設定
	case WINDOWTYPE_ITEMMENU:						bClose = OnWindowMsgITEMMENU(dwPara);	break;	// アイテムメニュー
	case WINDOWTYPE_ITEMMENU_SELECT:				bClose = OnWindowMsgITEMMENU_SELECT(dwPara);	break;	// アイテムどうするかメニュー
	case WINDOWTYPE_OPTION:							bClose = OnWindowMsgOPTION(dwPara);	break;	// オプション
	case WINDOWTYPE_OPTION_VIEWSET:					bClose = OnWindowMsgOPTION_VIEWSET(dwPara);	break;	// オプション-表示設定
	case WINDOWTYPE_OPTION_INPUTSET:				bClose = OnWindowMsgOPTION_INPUTSET(dwPara);	break;	// オプション-入力設定
	case WINDOWTYPE_OPTION_TASKBAR:					bClose = OnWindowMsgOPTION_TASKBAR(dwPara);	break;	// オプション(発言時にタスクバーチカチカ)
	case WINDOWTYPE_OPTION_INPUTSET_SETDEVICE:		bClose = OnWindowMsgOPTION_INPUTSET_SETDEVICE(dwPara);	break;	// オプション-入力設定-入力デバイスの設定
	case WINDOWTYPE_OPTION_ACTIONSET:				bClose = OnWindowMsgOPTION_ACTIONSET(dwPara);	break;	// オプション-動作設定
	case WINDOWTYPE_OPTION_ACTIONSET_SLEEPTIMER:	bClose = OnWindowMsgOPTION_ACTIONSET_SLEEPTIMER(dwPara);	break;	// オプション-動作設定-おひるねタイマーの設定
	case WINDOWTYPE_COMMANDMENU:					bClose = OnWindowMsgCOMMANDMENU(dwPara);	break;	// コマンドメニュー
	case WINDOWTYPE_SWOON:							bClose = OnWindowMsgSWOON(dwPara);	break;	// 気絶メニュー
	case WINDOWTYPE_SKILLMENU:						bClose = OnWindowMsgSKILLMENU(dwPara);	break;	// スキルメニュー
	}
	if (bClose) {
		m_pMgrWindow->Delete(nType);
	}
}



void CStateProcMAP::OnLButtonDown(int x, int y)
{
	int nMapX, nMapY, nType, xx, yy;
	DWORD dwNotifyData;
	PCLayerMap pLayerMap;
	SIZE size;

	pLayerMap = (PCLayerMap)m_pMgrLayer->Get(LAYERTYPE_MAP);
	if (pLayerMap == NULL) {
		return;
	}
	pLayerMap->GetViewMapPos(nMapX, nMapY);
	/* Phase 3: m_nViewX/Y はpx単位。サブタイル端数を加算してタイル境界を正確に計算 */
	xx = x + (pLayerMap->m_nViewX % MAPPARTSSIZE);
	yy = y + (pLayerMap->m_nViewY % MAPPARTSSIZE);

	// Web管理モード分岐（partsモードは独自処理、charモードは既存処理に流す）
	{
		int nWebMode = m_pMgrData->GetWebAdminMode();
		if (nWebMode == 2) {	// parts モード: 選択中パーツID > 0 なら配置
			WORD wPartsID = m_pMgrData->GetWebAdminSelectedPartsID();
			if (wPartsID > 0 && m_pMap != NULL) {
				/* 入力 x,y は OnSDLMouseLeftButtonDown で CImg32 パディング +32 が
				   加算されている。可視マップ座標に戻してからセル換算する */
				int nVisibleX = x - MAPPARTSSIZE + (pLayerMap->m_nViewX % MAPPARTSSIZE);
				int nVisibleY = y - MAPPARTSSIZE + (pLayerMap->m_nViewY % MAPPARTSSIZE);
				int nCellX = (nVisibleX / MAPPARTSSIZE) + nMapX;
				int nCellY = (nVisibleY / MAPPARTSSIZE) + nMapY;
				m_pMap->SetParts(nCellX, nCellY, wPartsID);
				CPacketADMIN_MAP_SETPARTS Packet;
				Packet.Make(m_pMap->m_dwMapID, nCellX, nCellY, wPartsID, FALSE);
				m_pSock->Send(&Packet);
			}
			return;	// 既存処理（pick 含む）はスキップ
		}
		// char モード (nWebMode == 1) は既存処理に流す
	}

	// Web管理画面への pick 通知（DLL 有無に関わらずブラウザ版でも実行する）
	// char > item > cell の優先度で情報をまとめて1回送信
	// ※ここでは switch 文による x/y の書き換え前なのでスクリーン座標のまま渡す（FindAdminClickedCharID の本来の意図）
	if ((m_pMgrData->GetAdminLevel() > ADMINLEVEL_NONE) && (m_pMap != NULL) && (m_pMgrData->GetWebAdminMode() >= 0)) {
		/* x,y には CImg32 パディング +MAPPARTSSIZE が加算されている。
		   parts モードと同じ補正式でセル座標を求める */
		int nPickX = ((x - MAPPARTSSIZE + (pLayerMap->m_nViewX % MAPPARTSSIZE)) / MAPPARTSSIZE) + nMapX;
		int nPickY = ((y - MAPPARTSSIZE + (pLayerMap->m_nViewY % MAPPARTSSIZE)) / MAPPARTSSIZE) + nMapY;
		CPacketADMIN_MAP_SELECTPICK PickPacket;
		PickPacket.Make(
			SELECTPICK_TYPE_MAPCELL,
			m_pMap->m_dwMapID,
			static_cast<WORD>(nPickX),
			static_cast<WORD>(nPickY),
			0);
		m_pSock->Send(&PickPacket);

		DWORD dwPickedCharID = FindAdminClickedCharID(m_pLibInfoChar, pLayerMap, x, y);
		DWORD dwPickedItemID = 0;
		{
			/* パディング補正: x,y から +MAPPARTSSIZE を引いてアイテムセル座標を求める */
			POINT ptItem = { (x - MAPPARTSSIZE + pLayerMap->m_nViewX) / SCROLLSIZE, (y - MAPPARTSSIZE + pLayerMap->m_nViewY) / SCROLLSIZE };
			PCInfoItem pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr(m_pMap->m_dwMapID, &ptItem);
			dwPickedItemID = pInfoItem ? pInfoItem->m_dwItemID : 0;
		}
		m_pMgrData->SetAdminPick(
			m_pMap->m_dwMapID,
			static_cast<WORD>(nPickX),
			static_cast<WORD>(nPickY),
			dwPickedCharID);
		SBOP2_PostAdminPick(
			static_cast<unsigned int>(m_pMap->m_dwMapID),
			nPickX,
			nPickY,
			static_cast<unsigned int>(dwPickedCharID),
			static_cast<unsigned int>(dwPickedItemID));
	}

	// 管理者 DLL がロードされていない場合は管理者枠描画・クリック通知をスキップ
	if (!m_AdminUi.IsLoadedFromDll()) {
		return;
	}

	nType = m_pMgrData->GetAdminNotifyTypeL();
	switch (nType) {
	case ADMINNOTIFYTYPE_CHARID:			// キャラID
	case ADMINNOTIFYTYPE_ACCOUNTID:			// アカウントID
		{
			int i, nCount, nBestScore;
			PCInfoCharCli pInfoChar;
			SIZE sizeProbe;
			int nProbeX, nProbeY;
			RECT rcProbe;

			/* Phase 8: ドット移動後は見た目と足元判定がずれやすいので、
			   管理者クリックは描画矩形ベースで少し広めに拾う。 */
			x = x + pLayerMap->m_nViewX;
			y = y + pLayerMap->m_nViewY;
			dwNotifyData = 0;
			sizeProbe.cx = MAPPARTSSIZE / 2;
			sizeProbe.cy = HALF_TILE;
			nProbeX = x - (sizeProbe.cx / 2);
			nProbeY = y - (sizeProbe.cy / 2);
			SetRect(&rcProbe, nProbeX, nProbeY, nProbeX + sizeProbe.cx - 1, nProbeY + sizeProbe.cy - 1);
			nBestScore = INT_MAX;

			nCount = m_pLibInfoChar->GetCount();
			for (i = 0; i < nCount; i ++) {
				RECT rcChar;
				RECT rcCharScreen;
				int nScreenX, nScreenY;
				pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr(i);
				if (pInfoChar == NULL) {
					continue;
				}
				SetRect(&rcChar,
					pInfoChar->m_nMapX,
					pInfoChar->m_nMapY - HALF_TILE,
					pInfoChar->m_nMapX + MAPPARTSSIZE - 1,
					pInfoChar->m_nMapY + HALF_TILE - 1);
				if ((rcChar.left <= rcProbe.right) && (rcProbe.left <= rcChar.right) &&
					(rcChar.top <= rcProbe.bottom) && (rcProbe.top <= rcChar.bottom)) {
					int nScore;

					nScore = abs(pInfoChar->m_nMapX - x) + abs(pInfoChar->m_nMapY - y);
					if (nScore >= nBestScore) {
						continue;
					}
					nBestScore = nScore;
					dwNotifyData = pInfoChar->m_dwCharID;
					if (nType == ADMINNOTIFYTYPE_ACCOUNTID) {
						dwNotifyData = pInfoChar->m_dwAccountID;
					}
				}
				nScreenX = pInfoChar->m_nMapX - pLayerMap->m_nViewX;
				nScreenY = pInfoChar->m_nMapY - pLayerMap->m_nViewY - HALF_TILE;
				SetRect(&rcCharScreen,
					nScreenX - 8,
					nScreenY - 8,
					nScreenX + MAPPARTSSIZE - 1 + 8,
					nScreenY + MAPPARTSSIZE - 1 + 8);
				if ((x >= rcCharScreen.left) && (x <= rcCharScreen.right) &&
					(y >= rcCharScreen.top) && (y <= rcCharScreen.bottom)) {
					int nScore;

					nScore = abs((nScreenX + HALF_TILE) - x);
					nScore += abs((nScreenY + HALF_TILE) - y);
					if (nScore >= nBestScore) {
						continue;
					}
					nBestScore = nScore;
					dwNotifyData = pInfoChar->m_dwCharID;
					if (nType == ADMINNOTIFYTYPE_ACCOUNTID) {
						dwNotifyData = pInfoChar->m_dwAccountID;
					}
				}
			}
			{
				CString strDbg;
				strDbg.Format(_T("[AdminCharPick] left_click screen=(%d,%d) world=(%d,%d) type=%d notify=%u count=%d\r\n"),
					x - pLayerMap->m_nViewX,
					y - pLayerMap->m_nViewY,
					x,
					y,
					nType,
					dwNotifyData,
					nCount);
				SDL_Log("%ls", (LPCWSTR)strDbg);
			}
			PostAdminUiMessage(WM_ADMINMSG, ADMINMSG_NOTIFYTYPE_LBUTTONDOWN, dwNotifyData);
		}
		break;

	case ADMINNOTIFYTYPE_MAPEDIT:			// マップ編集(通知は無し)
		{
			BOOL bPile;
			CPacketADMIN_MAP_SETPARTS Packet;

			x = (xx / 32) + nMapX;
			y = (yy / 32) + nMapY;

			bPile = m_pMgrData->GetEditMapPile();
			if (bPile) {
				m_pMap->SetPartsPile(x, y, m_pMgrData->GetSelectMapPartsID());
			} else {
				m_pMap->SetParts(x, y, m_pMgrData->GetSelectMapPartsID());
			}
			Packet.Make(m_pMap->m_dwMapID, x, y, m_pMgrData->GetSelectMapPartsID(), bPile);
			m_pSock->Send(&Packet);
		}
		break;

	case ADMINNOTIFYTYPE_MAPSHADOWEDIT:		// マップ影編集(通知は無し)
		{
			CPacketADMIN_MAP_SETMAPSHADOW Packet;

			x = (xx / 32) + nMapX;
			y = (yy / 32) + nMapY;
			m_pMap->SetShadow(x, y, m_pMgrData->GetSelectMapShadowID());

			Packet.Make(m_pMap->m_dwMapID, x, y, m_pMgrData->GetSelectMapShadowID());
			m_pSock->Send(&Packet);
		}
		break;

	case ADMINNOTIFYTYPE_ITEMID:			// アイテムID
		{
			POINT ptTmp;
			PCInfoItem pInfoItem;

			/* Phase 3: 画面クリックpx + カメラpx = ワールドpx → 旧スケール */
			x = (x + pLayerMap->m_nViewX) / SCROLLSIZE;
			y = (y + pLayerMap->m_nViewY) / SCROLLSIZE;
			dwNotifyData = 0;
			ptTmp.x = x;
			ptTmp.y = y;

			pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr(m_pMap->m_dwMapID, &ptTmp);
			if (pInfoItem == NULL) {
				break;
			}
			dwNotifyData = pInfoItem->m_dwItemID;
			PostAdminUiMessage(WM_ADMINMSG, ADMINMSG_NOTIFYTYPE_LBUTTONDOWN, dwNotifyData);
		}
		break;

	case ADMINNOTIFYTYPE_POS:				// マップ座標
		x = (xx / 32) + nMapX;
		y = (yy / 32) + nMapY;
		dwNotifyData = MAKELPARAM(y, x);
		PostAdminUiMessage(WM_ADMINMSG, ADMINMSG_NOTIFYTYPE_LBUTTONDOWN, dwNotifyData);
		break;

	case ADMINNOTIFYTYPE_CHARPOS:			// キャラ座標
		/* Phase 8: キャラ座標通知はワールドpx座標で返す */
		x = x + pLayerMap->m_nViewX;
		y = y + pLayerMap->m_nViewY;
		dwNotifyData = MAKELPARAM(y, x);
		PostAdminUiMessage(WM_ADMINMSG, ADMINMSG_NOTIFYTYPE_LBUTTONDOWN, dwNotifyData);
		break;
	}

}



void CStateProcMAP::OnRButtonDown(int x, int y)
{
	BOOL bPile;
	int nMapX, nMapY, xx, yy;
	WORD wTmp;
	DWORD dwNotifyData;
	PCLayerMap pLayerMap;

	pLayerMap = (PCLayerMap)m_pMgrLayer->Get(LAYERTYPE_MAP);
	if (pLayerMap == NULL) {
		return;
	}
	pLayerMap->GetViewMapPos(nMapX, nMapY);
	/* Phase 3: m_nViewX/Y はpx単位。サブタイル端数を加算 */
	xx = x + (pLayerMap->m_nViewX % MAPPARTSSIZE);
	yy = y + (pLayerMap->m_nViewY % MAPPARTSSIZE);

	// Web管理モード分岐
	{
		int nWebMode = m_pMgrData->GetWebAdminMode();
		if (nWebMode == 2) {	// parts モード: 右クリックでパーツID取得 → 親へ通知
			if (m_pMap != NULL) {
				/* OnLButtonDown と同様に CImg32 パディング +32 を相殺 */
				int nVisibleX = x - MAPPARTSSIZE + (pLayerMap->m_nViewX % MAPPARTSSIZE);
				int nVisibleY = y - MAPPARTSSIZE + (pLayerMap->m_nViewY % MAPPARTSSIZE);
				int nCellX = (nVisibleX / MAPPARTSSIZE) + nMapX;
				int nCellY = (nVisibleY / MAPPARTSSIZE) + nMapY;
				WORD wPickedPartsID = m_pMap->GetParts(nCellX, nCellY);
				m_pMgrData->SetWebAdminSelectedPartsID(wPickedPartsID);
				SBOP2_PostAdminPickupParts(
					static_cast<unsigned int>(m_pMap->m_dwMapID),
					nCellX, nCellY,
					static_cast<unsigned int>(wPickedPartsID));
			}
			return;
		}
		if (nWebMode == 1) {	// char モード: 右クリックは何もしない
			return;
		}
	}

	// 管理者 DLL がロードされていない場合は管理者枠描画・クリック通知をスキップ
	if (!m_AdminUi.IsLoadedFromDll()) {
		return;
	}

	switch (m_pMgrData->GetAdminNotifyTypeR()) {
	case ADMINNOTIFYTYPE_MAPEDIT:			// マップ編集(通知は無し)
		x = (xx / 32) + nMapX;
		y = (yy / 32) + nMapY;
		bPile = m_pMgrData->GetEditMapPile();
		if (bPile) {
			wTmp = m_pMap->GetPartsPile(x, y);
		} else {
			wTmp = m_pMap->GetParts(x, y);
		}
		m_pMgrData->SetSelectMapPartsID(wTmp);
		break;

	case ADMINNOTIFYTYPE_MAPSHADOWEDIT:		// マップ影編集(通知は無し)
		x = (xx / 32) + nMapX;
		y = (yy / 32) + nMapY;
		wTmp = m_pMap->GetShadow(x, y);
		m_pMgrData->SetSelectMapShadowID(wTmp);
		break;

	case ADMINNOTIFYTYPE_POS:				// マップ座標
		x = (xx / 32) + nMapX;
		y = (yy / 32) + nMapY;
		dwNotifyData = MAKELPARAM(y, x);
		PostAdminUiMessage(WM_ADMINMSG, ADMINMSG_NOTIFYTYPE_RBUTTONDOWN, dwNotifyData);
		break;

	case ADMINNOTIFYTYPE_CHARPOS:			// キャラ座標
		/* Phase 8: キャラ座標通知はワールドpx座標で返す */
		x = x + pLayerMap->m_nViewX;
		y = y + pLayerMap->m_nViewY;
		dwNotifyData = MAKELPARAM(y, x);
		PostAdminUiMessage(WM_ADMINMSG, ADMINMSG_NOTIFYTYPE_RBUTTONDOWN, dwNotifyData);
		break;
	}
}



void CStateProcMAP::OnRButtonDblClk(int x, int y)
{
	int nMapX, nMapY, xx, yy;
	DWORD dwNotifyData;
	PCLayerMap pLayerMap;

	pLayerMap = (PCLayerMap)m_pMgrLayer->Get(LAYERTYPE_MAP);
	if (pLayerMap == NULL) {
		return;
	}
	pLayerMap->GetViewMapPos(nMapX, nMapY);
	/* Phase 3: m_nViewX/Y はpx単位。サブタイル端数を加算 */
	xx = x + (pLayerMap->m_nViewX % MAPPARTSSIZE);
	yy = y + (pLayerMap->m_nViewY % MAPPARTSSIZE);

	// 管理者 DLL がロードされていない場合は管理者枠描画・クリック通知をスキップ
	if (!m_AdminUi.IsLoadedFromDll()) {
		return;
	}

	switch (m_pMgrData->GetAdminNotifyTypeRR()) {
	case ADMINNOTIFYTYPE_POS:				// マップ座標
		x = (xx / 32) + nMapX;
		y = (yy / 32) + nMapY;
		dwNotifyData = MAKELPARAM(y, x);
		PostAdminUiMessage(WM_ADMINMSG, ADMINMSG_NOTIFYTYPE_RBUTTONDBLCLK, dwNotifyData);
		break;

	case ADMINNOTIFYTYPE_CHARPOS:			// キャラ座標
		/* Phase 8: 右ダブルクリックも右クリックと同じくワールドpx座標で返す */
		x = x + pLayerMap->m_nViewX;
		y = y + pLayerMap->m_nViewY;
		dwNotifyData = MAKELPARAM(y, x);
		PostAdminUiMessage(WM_ADMINMSG, ADMINMSG_NOTIFYTYPE_RBUTTONDBLCLK, dwNotifyData);
		break;
	}
}



void CStateProcMAP::OnMouseMove(int x, int y)
{
	int nMapX, nMapY, xx, yy;
	PCLayerMap pLayerMap;

	pLayerMap = (PCLayerMap)m_pMgrLayer->Get(LAYERTYPE_MAP);
	if (pLayerMap == NULL) {
		return;
	}
	pLayerMap->GetViewMapPos(nMapX, nMapY);
	/* Phase 3: m_nViewX/Y はpx単位。サブタイル端数を加算 */
	xx = x + (pLayerMap->m_nViewX % MAPPARTSSIZE);
	yy = y + (pLayerMap->m_nViewY % MAPPARTSSIZE);

	// 管理者 DLL がロードされていない場合は管理者枠描画・クリック通知をスキップ
	if (!m_AdminUi.IsLoadedFromDll()) {
		return;
	}

	switch (m_pMgrData->GetAdminNotifyTypeL()) {
	case ADMINNOTIFYTYPE_MAPEDIT:	// マップ編集(通知は無し)
		{
			BOOL bPile;
			WORD wMapParts;
			DWORD dwMapPartsID;
			CPacketADMIN_MAP_SETPARTS Packet;

			if (IsLeftMousePressed() == FALSE) {
				break;
			}
			x = (xx / 32) + nMapX;
			y = (yy / 32) + nMapY;
			wMapParts		= m_pMap->GetParts(x, y);
			dwMapPartsID	= m_pMgrData->GetSelectMapPartsID();
			if (wMapParts == dwMapPartsID) {
				break;
			}
			bPile = m_pMgrData->GetEditMapPile();
			if (bPile) {
				m_pMap->SetPartsPile(x, y, dwMapPartsID);
			} else {
				m_pMap->SetParts(x, y, dwMapPartsID);
			}

			Packet.Make(m_pMap->m_dwMapID, x, y, m_pMgrData->GetSelectMapPartsID(), bPile);
			m_pSock->Send(&Packet);
		}
		break;

	case ADMINNOTIFYTYPE_MAPSHADOWEDIT:		// マップ影編集(通知は無し)
		{
			WORD wMapShadow;
			DWORD dwMapShadowID;
			CPacketADMIN_MAP_SETMAPSHADOW Packet;

			if (IsLeftMousePressed() == FALSE) {
				break;
			}
			x = (xx / 32) + nMapX;
			y = (yy / 32) + nMapY;
			wMapShadow		= m_pMap->GetShadow(x, y);
			dwMapShadowID	= m_pMgrData->GetSelectMapShadowID();
			if (wMapShadow == dwMapShadowID) {
				break;
			}
			m_pMap->SetShadow(x, y, dwMapShadowID);

			Packet.Make(m_pMap->m_dwMapID, x, y, m_pMgrData->GetSelectMapShadowID());
			m_pSock->Send(&Packet);
		}
		break;
	}
}



void CStateProcMAP::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
#if defined(_WIN32)
	PostAdminUiMessage(WM_MAINFRAME, dwCommand, dwParam);
#endif
	m_pPlayerChar = m_pMgrData->GetPlayerChar();

	switch (dwCommand) {
	case MAINFRAMEMSG_RENEWMAPINFO:		// マップ情報更新
		{
			PCLayerMap pLayerMap;
			CRect rcTmp;

			m_pMap = m_pMgrData->GetMap();
			pLayerMap = (PCLayerMap)m_pMgrLayer->Get(LAYERTYPE_MAP);
			pLayerMap->SetCenterPos(m_pPlayerChar->m_nMapX, m_pPlayerChar->m_nMapY);

			rcTmp.left	 = pLayerMap->m_nViewX - (MAPPARTSSIZE * 2);
			rcTmp.right	 = pLayerMap->m_nViewX + (DRAW_PARTS_X * MAPPARTSSIZE) + (MAPPARTSSIZE * 2);
			rcTmp.top	 = pLayerMap->m_nViewY - (MAPPARTSSIZE * 2);
			rcTmp.bottom = pLayerMap->m_nViewY + (DRAW_PARTS_Y * MAPPARTSSIZE) + (MAPPARTSSIZE * 2);
			m_pLibInfoItem->SetArea(m_pMgrData->GetPlayerChar()->m_dwMapID, &rcTmp);
		}
		break;

	case MAINFRAMEMSG_RENEWSTATUS:		// ステータス情報更新
	case MAINFRAMEMSG_RENEWCHARINFO:	// キャラ情報更新
		m_pMgrWindow->Update();
		break;

	case MAINFRAMEMSG_RENEWCHARCHAT:	// 発言内容更新
		{
			PCInfoCharCli pInfoChar;
			CmyString strTmp;

			pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr(dwParam);
			if (pInfoChar == NULL) {
				break;
			}
			if (pInfoChar == m_pPlayerChar) {
				KeyProc(0, FALSE);
			}
			// 注意: Format(_T("%s..."), ...) はワイド書式の %s が
			// MSVC ではワイド・emscripten(POSIX) ではナローを期待する方言差があり、
			// ブラウザ版で結果が空になる。方言に依存しない連結で組み立てる。
			strTmp = pInfoChar->m_strCharName;
			strTmp += _T("：");
			strTmp += (LPCTSTR)pInfoChar->m_strSpeak;
			if (m_pImGuiMsgLog) { m_pImGuiMsgLog->Add(strTmp, pInfoChar->m_clSpeak); }
		}
		break;

	case MAINFRAMEMSG_RENEWCHARCOUNT:	// キャラ数更新
		m_pMgrData->SetCharCount(dwParam);
		break;

	case MAINFRAMEMSG_RENEWONLINECOUNT:	// オンライン数更新
		break;

	case MAINFRAMEMSG_RENEWSYSTEMMSG:	// システムメッセージ更新
		{
			int i, nCount;
			PCLayerSystemMsg pLayerSystemMsg;
			PSTSYSTEMMSG pSystemMsg;

			nCount = m_pMgrData->GetSystemMsgCount();
			if (nCount <= 0) {
				break;
			}
			pLayerSystemMsg = (PCLayerSystemMsg)m_pMgrLayer->Get(LAYERTYPE_SYSTEMMSG);
			for (i = 0; i < nCount; i ++) {
				pSystemMsg = m_pMgrData->GetSystemMsg(i);

				if (pSystemMsg->bAddLog) {
					if (m_pImGuiMsgLog) { m_pImGuiMsgLog->Add(pSystemMsg->strMsg, pSystemMsg->clMsg); }
				}
				pLayerSystemMsg->AddMsg(pSystemMsg->strMsg, pSystemMsg->clMsg);
			}
			m_pMgrData->DeleteAllSystemMsg();
		}
		break;

	case MAINFRAMEMSG_RENEWADMINLEVEL:	// 管理者権限変更
		CreateAdminUi();
		break;

	case MAINFRAMEMSG_RENEWITEMINFO:	// アイテム情報更新
		OnMainFrameRENEWITEMINFO(dwParam);
		break;

	case MAINFRAMEMSG_RENEWTALKEVENT:	// 会話イベント情報更新
		OnMainFrameRENEWTALKEVENT(dwParam);
		break;

	case MAINFRAMEMSG_DAMAGE:			// ダメージを受けた
		// docs/battle-redesign.md S2: 攻撃/防御ゲージ廃止に伴い削除(被弾減速はS5で別途実装)
		break;
	}
}



void CStateProcMAP::OnAdminMsg(int nCode, DWORD dwPara)
{
#if !defined(_WIN32)
	(void)nCode;
	(void)dwPara;
	return;
#else
	PostAdminUiMessage(WM_ADMINMSG, nCode, dwPara);
#endif
}

void CStateProcMAP::CreateAdminUi(void)
{
	DestroyAdminUi();

#if !defined(_WIN32)
	if (m_pMgrData) {
		m_pMgrData->SetAdminWindow(NULL);
		m_pMgrData->SetAdminNotifyTypeL(ADMINNOTIFYTYPE_NONE);
		m_pMgrData->SetAdminNotifyTypeR(ADMINNOTIFYTYPE_NONE);
		m_pMgrData->SetAdminNotifyTypeRR(ADMINNOTIFYTYPE_NONE);
	}
	return;
#endif

	if ((m_pMgrData == NULL) || (m_pMgrData->GetAdminLevel() <= ADMINLEVEL_NONE)) {
		return;
	}
	if (m_AdminUi.Create(m_pMgrData->GetMainWindow(), m_pMgrData)) {
		m_AdminUi.Show();
	}
}

void CStateProcMAP::DestroyAdminUi(void)
{
#if defined(_WIN32)
	m_AdminUi.Destroy();
#endif
	if (m_pMgrData) {
		m_pMgrData->SetAdminWindow(NULL);
		m_pMgrData->SetAdminNotifyTypeL(ADMINNOTIFYTYPE_NONE);
		m_pMgrData->SetAdminNotifyTypeR(ADMINNOTIFYTYPE_NONE);
		m_pMgrData->SetAdminNotifyTypeRR(ADMINNOTIFYTYPE_NONE);
	}
}

void CStateProcMAP::PostAdminUiMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
#if !defined(_WIN32)
	(void)message;
	(void)wParam;
	(void)lParam;
	return;
#else
	m_AdminUi.Notify(message, wParam, lParam);
#endif
}



void CStateProcMAP::TimerProcSleepTimer(void)
{
	BOOL bSleepTime;
	int nState, nSleepTimer, anSleepTimer[] = {0, 3, 5, 10, 30, 60};
	CPacketCHAR_STATE Packet;

	if (m_pPlayerChar == NULL) {
		return;
	}

	nState		= -1;
	nSleepTimer	= m_pMgrData->GetSleepTimer();
	nSleepTimer	= anSleepTimer[nSleepTimer];

	if (!((nSleepTimer > 0) && (m_dwLastKeyInput != 0))) {
		return;
	}

	bSleepTime = FALSE;
	if (timeGetTime() - m_dwLastKeyInput > (DWORD)(nSleepTimer * 1000 * 60)) {
		bSleepTime = TRUE;
	}

	switch (m_pPlayerChar->m_nMoveState) {
	case CHARMOVESTATE_STAND:				// 立ち
	case CHARMOVESTATE_SIT:					// 座り中
	case CHARMOVESTATE_SLEEPTIMER:			// おひるねタイマー中
		if (m_pPlayerChar->IsEnableMove() == FALSE) {
			m_pPlayerChar->SetSleepTimer(FALSE);
			break;
		}
		if (bSleepTime) {
			/* 初回？ */
			if (m_pPlayerChar->m_bModeSleepTimer == FALSE) {
				nState = CHARMOVESTATE_SLEEPTIMER;
			}
			m_pPlayerChar->SetSleepTimer(TRUE);
		}
		break;
	}

	/* おひるねタイマー中？ */
	if (m_pPlayerChar->m_nMoveState == CHARMOVESTATE_SLEEPTIMER) {
		if (bSleepTime == FALSE) {
			if (m_pPlayerChar->m_bModeSleepTimer) {
				nState = CHARMOVESTATE_SIT;
			}
			m_pPlayerChar->SetSleepTimer(FALSE);
		}
	}

	if (nState != -1) {
		m_pPlayerChar->ChgMoveState(nState);
		Packet.Make(m_pPlayerChar->m_dwCharID, m_pPlayerChar->m_nMoveState);
		m_pSock->Send(&Packet);
		m_pPlayerChar->SetChgWait(TRUE);
	}
}



void CStateProcMAP::TimerProcAtackRepeat(void)
{
	BOOL bResult;
	PCMgrKeyInput pMgrKeyInput;

	if (m_pPlayerChar == NULL) {
		return;
	}
	if (m_bAtackKeyAutoRepeat == FALSE) {
		return;
	}
	if (m_pPlayerChar->m_nMoveState != CHARMOVESTATE_BATTLE) {
		/* 攻撃モーション(BATTLEATACK)中や防御中はここでは何もせず、
		   モーション終了で自動的にBATTLEへ戻ってから再攻撃する */
		return;
	}
	if (m_pMgrWindow->IsKeyInput()) {
		m_bAtackKeyAutoRepeat = FALSE;
		return;
	}
	pMgrKeyInput = m_pMgrData->GetMgrKeyInput();
	bResult = pMgrKeyInput->IsInput('X');
	if (bResult == FALSE) {
		m_bAtackKeyAutoRepeat = FALSE;
		return;
	}
	if (m_pPlayerChar->IsChgWait()) {
		return;
	}
	if ((m_pMap == NULL) || (m_pMap->IsEnableBattle() == FALSE)) {
		m_bAtackKeyAutoRepeat = FALSE;
		return;
	}
	if (IsWeaponFishingRod()) {
		/* 押しっぱなし継続中に釣り竿へ持ち替えた場合、釣り竿は武器ではないため
		   連続攻撃を止める(docs/battle-redesign.md 7章: 釣り竿装備中は攻撃しない) */
		m_bAtackKeyAutoRepeat = FALSE;
		return;
	}

	StartLocalAtack();
}



void CStateProcMAP::TimerProcAtackAutoOff(void)
{
	DWORD dwNow;
	CPacketCHAR_STATE Packet;

	if (m_pPlayerChar == NULL) {
		return;
	}
	if (m_pPlayerChar->m_nMoveState != CHARMOVESTATE_BATTLE) {
		/* 戦闘移動中(BATTLEMOVE)は止まってから判定すればよい */
		return;
	}
	if (m_dwLastAtackTime == 0) {
		return;
	}
	dwNow = timeGetTime();
	if (dwNow - m_dwLastAtackTime < 5000) {
		return;
	}

	m_bAtackKeyAutoRepeat = FALSE;
	m_dwLastAtackTime = 0;
	m_pPlayerChar->ChgMoveState(CHARMOVESTATE_STAND);
	Packet.Make(m_pPlayerChar->m_dwCharID, CHARMOVESTATE_STAND);
	m_pSock->Send(&Packet);
}



BOOL CStateProcMAP::StartLocalAtack(void)
{
	/* 攻撃モーションをサーバーの返事を待たずローカルで即時開始する(docs/battle-redesign.md S2)。
	   CHAR_STATE(BATTLEATACK)は他プレイヤーの見た目用に1回だけ送る。
	   呼び出し前提: m_pPlayerChar・m_pMap が有効で、戦闘可能マップ */
	int anDirection[] = {0, 1, 2, 3, 0, 1, 1, 0};
	CPacketCHAR_STATE PacketCHAR_STATE;
	CPacketCHAR_MOVE_STOP PacketCHAR_MOVE_STOP;

	PacketCHAR_MOVE_STOP.Make(
			m_pPlayerChar->m_dwMapID,
			m_pPlayerChar->m_dwCharID,
			anDirection[m_pPlayerChar->m_nDirection],
			m_pPlayerChar->m_nMapX,
			m_pPlayerChar->m_nMapY,
			FALSE,
			1,
			timeGetTime());
	m_pSock->Send(&PacketCHAR_MOVE_STOP);
	m_bMoveSyncActive = FALSE;
	m_nMoveSyncDirection = -1;
	m_dwLastTimeMoveSyncSend = 0;

	/* 他プレイヤーの見た目用の通知(自キャラへのエコーはRecvProcCHAR_STATEで無視する) */
	PacketCHAR_STATE.Make(m_pPlayerChar->m_dwCharID, CHARMOVESTATE_BATTLEATACK);
	m_pSock->Send(&PacketCHAR_STATE);

	/* サーバーの返事を待たずローカルで攻撃モーションを開始 */
	m_pLibInfoChar->RenewMotionInfo(m_pPlayerChar);
	m_pPlayerChar->ChgMoveState(CHARMOVESTATE_BATTLEATACK);

	m_dwLastAtackTime = timeGetTime();
	m_bAtackKeyAutoRepeat = TRUE;

	return TRUE;
}



BOOL CStateProcMAP::IsWeaponFishingRod(void)
{
	/* 右手装備が釣り竿かを判定する(docs/battle-redesign.md 7章: 釣り竿装備中は攻撃しない)。
	   「釣り竿は武器ではない」方針のため、装備中は攻撃(敵への攻撃・空振り攻撃の
	   両方)へ進ませないための共通判定として使う。判定自体はSetMotionInfo
	   (LibInfoCharCli.cpp)やIsFishingAvailableと同じGetMotionIDAtackのフラグを見る */
	DWORD dwMotionType;

	if (m_pPlayerChar == NULL) {
		return FALSE;
	}
	dwMotionType = m_pLibInfoItem->GetMotionIDAtack(m_pPlayerChar->m_dwEquipItemIDArmsRight);
	return (dwMotionType & INFOITEMARMS_MOTION_FISHING) ? TRUE : FALSE;
}



BOOL CStateProcMAP::IsFishingAvailable(void)
{
	/* 釣り竿装備＋正面が水タイルかを判定する(docs/battle-redesign.md S4)。
	   水タイル判定はCInfoCharBase::IsFacingFishingSpot()でサーバー
	   (RecvProcCHAR_PROC_FISHING/UseSkillFISHING)と共通の判定を使う */
	if ((m_pPlayerChar == NULL) || (m_pMap == NULL)) {
		return FALSE;
	}
	if (IsWeaponFishingRod() == FALSE) {
		return FALSE;
	}
	return m_pPlayerChar->IsFacingFishingSpot(m_pMap);
}



BOOL CStateProcMAP::StartLocalFishing(void)
{
	/* 釣りをローカルで即時開始する(docs/battle-redesign.md S4)。攻撃と違い戦闘状態
	   (BATTLE系のm_nMoveState)へは遷移させず、割り込みモーション(CHARMOTIONID_INTERRUUPT)
	   で釣りモーションだけ再生する。モーション中の所定コマでCInfoCharCli::MotionProcが
	   CHARMOTIONPROCID_FISHINGを検知しSBOCOMMANDID_SUB_CHAR_PROC_FISHINGを送る(既存の仕組み)。
	   移動中の見た目を止めるためMOVE_STOPのみここで送る。押しっぱなし連続は起こさない
	   (m_bAtackKeyAutoRepeatは立てない。呼び出し元のOnXが押した瞬間のみ呼ぶ) */
	int anDirection[] = {0, 1, 2, 3, 0, 1, 1, 0};
	CPacketCHAR_MOVE_STOP PacketCHAR_MOVE_STOP;

	PacketCHAR_MOVE_STOP.Make(
			m_pPlayerChar->m_dwMapID,
			m_pPlayerChar->m_dwCharID,
			anDirection[m_pPlayerChar->m_nDirection],
			m_pPlayerChar->m_nMapX,
			m_pPlayerChar->m_nMapY,
			FALSE,
			1,
			timeGetTime());
	m_pSock->Send(&PacketCHAR_MOVE_STOP);
	m_bMoveSyncActive = FALSE;
	m_nMoveSyncDirection = -1;
	m_dwLastTimeMoveSyncSend = 0;

	m_pLibInfoChar->SetMotionInfo(m_pPlayerChar, CHARMOTIONID_INTERRUUPT, CHARMOTIONLISTID_FISHING_UP);
	m_pPlayerChar->InitMotionInfo(CHARMOTIONID_INTERRUUPT);

	return TRUE;
}



BOOL CStateProcMAP::IsKeyInputEnable(void)
{
	BOOL bRet;
	PCLayerMap pLayerMap;
	PCInfoCharCli pInfoChar;

	bRet = FALSE;
	pLayerMap = (PCLayerMap)m_pMgrLayer->Get(LAYERTYPE_MAP);
	if (pLayerMap == NULL) {
		goto Exit;
	}
	if (m_pMgrDraw->IsFade()) {
		goto Exit;
	}
	pInfoChar = m_pMgrData->GetPlayerChar();
	if (pInfoChar == NULL) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}



BOOL CStateProcMAP::OnUp(BOOL bDown)
{
	BOOL bRet, bResult;
	int i, x, y, xBefore, yBefore, nDirection, nMoveStep, nMoveSpeedAccum;
	DWORD dwMoveStepTime;
	PCMgrKeyInput pMgrKeyInput;

	pMgrKeyInput = m_pMgrData->GetMgrKeyInput();
	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable() == FALSE) {
		goto Exit;
	}
	if (bDown == FALSE) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime();
	nMoveSpeedAccum = m_nMoveSpeedAccum;
	dwMoveStepTime = m_dwLastPlayerMoveStepTime;
	nMoveStep = GetPlayerMoveStep(m_dwLastKeyInput, nMoveSpeedAccum, dwMoveStepTime);
	m_nMoveSpeedAccum = nMoveSpeedAccum;
	m_dwLastPlayerMoveStepTime = dwMoveStepTime;
	if (nMoveStep <= 0) {
		bRet = TRUE;
		goto Exit;
	}

	for (i = 0; i < nMoveStep; i ++) {
		x = 0;
		y = -1;
		nDirection = 0;
		bResult = pMgrKeyInput->IsInput(VK_RIGHT);
		if (bResult) {
			nDirection = 4;
			x = 1;
		}
		bResult = pMgrKeyInput->IsInput(VK_LEFT);
		if (bResult) {
			nDirection = 7;
			x = -1;
		}
		xBefore = m_pPlayerChar->m_nMapX;
		yBefore = m_pPlayerChar->m_nMapY;
		bResult = MoveProc(m_pPlayerChar->m_nMapX, m_pPlayerChar->m_nMapY, x, y, nDirection, (i == nMoveStep - 1) ? TRUE : FALSE);
		if (bResult == FALSE) {
			goto Exit;
		}
		if ((xBefore == m_pPlayerChar->m_nMapX) && (yBefore == m_pPlayerChar->m_nMapY)) {
			break;
		}
	}

	bRet = TRUE;
Exit:
	return bRet;
}



BOOL CStateProcMAP::OnDown(BOOL bDown)
{
	BOOL bRet, bResult;
	int i, x, y, xBefore, yBefore, nDirection, nMoveStep, nMoveSpeedAccum;
	DWORD dwMoveStepTime;
	PCMgrKeyInput pMgrKeyInput;

	pMgrKeyInput = m_pMgrData->GetMgrKeyInput();
	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable() == FALSE) {
		goto Exit;
	}
	if (bDown == FALSE) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime();
	nMoveSpeedAccum = m_nMoveSpeedAccum;
	dwMoveStepTime = m_dwLastPlayerMoveStepTime;
	nMoveStep = GetPlayerMoveStep(m_dwLastKeyInput, nMoveSpeedAccum, dwMoveStepTime);
	m_nMoveSpeedAccum = nMoveSpeedAccum;
	m_dwLastPlayerMoveStepTime = dwMoveStepTime;
	if (nMoveStep <= 0) {
		bRet = TRUE;
		goto Exit;
	}

	for (i = 0; i < nMoveStep; i ++) {
		x = 0;
		y = 1;
		nDirection = 1;
		bResult = pMgrKeyInput->IsInput(VK_RIGHT);
		if (bResult) {
			nDirection = 5;
			x = 1;
		}
		bResult = pMgrKeyInput->IsInput(VK_LEFT);
		if (bResult) {
			nDirection = 6;
			x = -1;
		}
		xBefore = m_pPlayerChar->m_nMapX;
		yBefore = m_pPlayerChar->m_nMapY;
		bResult = MoveProc(m_pPlayerChar->m_nMapX, m_pPlayerChar->m_nMapY, x, y, nDirection, (i == nMoveStep - 1) ? TRUE : FALSE);
		if (bResult == FALSE) {
			goto Exit;
		}
		if ((xBefore == m_pPlayerChar->m_nMapX) && (yBefore == m_pPlayerChar->m_nMapY)) {
			break;
		}
	}

	bRet = TRUE;
Exit:
	return bRet;
}



BOOL CStateProcMAP::OnLeft(BOOL bDown)
{
	BOOL bRet, bResult;
	int i, x, y, xBefore, yBefore, nDirection, nMoveStep, nMoveSpeedAccum;
	DWORD dwMoveStepTime;
	PCMgrKeyInput pMgrKeyInput;

	pMgrKeyInput = m_pMgrData->GetMgrKeyInput();
	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable() == FALSE) {
		goto Exit;
	}
	if (bDown == FALSE) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime();
	nMoveSpeedAccum = m_nMoveSpeedAccum;
	dwMoveStepTime = m_dwLastPlayerMoveStepTime;
	nMoveStep = GetPlayerMoveStep(m_dwLastKeyInput, nMoveSpeedAccum, dwMoveStepTime);
	m_nMoveSpeedAccum = nMoveSpeedAccum;
	m_dwLastPlayerMoveStepTime = dwMoveStepTime;
	if (nMoveStep <= 0) {
		bRet = TRUE;
		goto Exit;
	}

	for (i = 0; i < nMoveStep; i ++) {
		x = -1;
		y = 0;
		nDirection = 2;
		bResult = pMgrKeyInput->IsInput(VK_UP);
		if (bResult) {
			nDirection = 7;
			y = -1;
		}
		bResult = pMgrKeyInput->IsInput(VK_DOWN);
		if (bResult) {
			nDirection = 6;
			y = 1;
		}
		xBefore = m_pPlayerChar->m_nMapX;
		yBefore = m_pPlayerChar->m_nMapY;
		bResult = MoveProc(m_pPlayerChar->m_nMapX, m_pPlayerChar->m_nMapY, x, y, nDirection, (i == nMoveStep - 1) ? TRUE : FALSE);
		if (bResult == FALSE) {
			goto Exit;
		}
		if ((xBefore == m_pPlayerChar->m_nMapX) && (yBefore == m_pPlayerChar->m_nMapY)) {
			break;
		}
	}

	bRet = TRUE;
Exit:
	return bRet;
}



BOOL CStateProcMAP::OnRight(BOOL bDown)
{
	BOOL bRet, bResult;
	int i, x, y, xBefore, yBefore, nDirection, nMoveStep, nMoveSpeedAccum;
	DWORD dwMoveStepTime;
	PCMgrKeyInput pMgrKeyInput;

	pMgrKeyInput = m_pMgrData->GetMgrKeyInput();
	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable() == FALSE) {
		goto Exit;
	}
	if (bDown == FALSE) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime();
	nMoveSpeedAccum = m_nMoveSpeedAccum;
	dwMoveStepTime = m_dwLastPlayerMoveStepTime;
	nMoveStep = GetPlayerMoveStep(m_dwLastKeyInput, nMoveSpeedAccum, dwMoveStepTime);
	m_nMoveSpeedAccum = nMoveSpeedAccum;
	m_dwLastPlayerMoveStepTime = dwMoveStepTime;
	if (nMoveStep <= 0) {
		bRet = TRUE;
		goto Exit;
	}

	for (i = 0; i < nMoveStep; i ++) {
		x = 1;
		y = 0;
		nDirection = 3;
		bResult = pMgrKeyInput->IsInput(VK_UP);
		if (bResult) {
			nDirection = 4;
			y = -1;
		}
		bResult = pMgrKeyInput->IsInput(VK_DOWN);
		if (bResult) {
			nDirection = 5;
			y = 1;
		}
		xBefore = m_pPlayerChar->m_nMapX;
		yBefore = m_pPlayerChar->m_nMapY;
		bResult = MoveProc(m_pPlayerChar->m_nMapX, m_pPlayerChar->m_nMapY, x, y, nDirection, (i == nMoveStep - 1) ? TRUE : FALSE);
		if (bResult == FALSE) {
			goto Exit;
		}
		if ((xBefore == m_pPlayerChar->m_nMapX) && (yBefore == m_pPlayerChar->m_nMapY)) {
			break;
		}
	}

	bRet = TRUE;
Exit:
	return bRet;
}



BOOL CStateProcMAP::OnX(BOOL bDown)
{
	BOOL bRet, bResult;
	DWORD dwFrontCharID, dwEnemyCharID;
	PCInfoItem pInfoItem;
	CPacketCHAR_REQ_PUTGET PacketCHAR_REQ_PUTGET;
	CPacketCHAR_REQ_TAIL PacketCHAR_REQ_TAIL;

	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable() == FALSE) {
		goto Exit;
	}
	bResult = m_pPlayerChar->IsChgWait();
	/* 状態変更待ち？ */
	if (bResult) {
		goto Exit;
	}
	if (bDown == FALSE) {
		/* docs/battle-redesign.md S2: 攻撃・会話・拾いは押した瞬間(bDown==TRUE)に
		   決定済み。離した時は何もしない */
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime();
	m_bAtackKeyAutoRepeat = FALSE;

	bResult = m_pPlayerChar->IsEnableMove();
	if (bResult == FALSE) {
		/* 付いて行き中(m_dwFrontCharID)・攻撃モーション中・気絶中・防御中等はここに来る。
		   付いて行き中はOnTabの旧メッセージ相当を出す */
		if (m_pPlayerChar->m_dwFrontCharID) {
			AddSystemMsg(FALSE, "付いて行っている時は攻撃できません", RGB(255, 255, 255));
		}
		if ((m_pPlayerChar->m_dwFrontCharID) || (m_pPlayerChar->m_dwTailCharID)) {
			PacketCHAR_REQ_TAIL.Make(m_pPlayerChar->m_dwCharID, 0, FALSE);
			m_pSock->Send(&PacketCHAR_REQ_TAIL);
		}
		goto Exit;
	}

	switch (m_pPlayerChar->m_nMoveState) {
	case CHARMOVESTATE_STAND:			// 立ち
	case CHARMOVESTATE_BATTLE:			// 戦闘中(静止)
		/* docs/battle-redesign.md S3/S4: 押した瞬間の正面判定で
		   a.攻撃(敵優先) → b.会話 → c.釣り → d.拾い → e.空振り攻撃 → f.従来どおり(付いて行き解除)
		   の順に決定する。STAND/BATTLE(静止)は同じ優先順で扱う */

		/* a. 戦闘可能マップで正面に攻撃できる敵がいれば最優先で攻撃する
		   (敵がm_strTalkを持っていても敵優先。連続攻撃中に会話へ化けないよう
		   押しっぱなし中の再判定はTimerProcAtackRepeat側でBATTLE静止時のみ行う)
		   ただし釣り竿は武器ではないため、装備中は敵が正面にいても攻撃しない
		   (docs/battle-redesign.md 7章: 釣り竿装備中は攻撃しない) */
		if ((m_pMap && m_pMap->IsEnableBattle()) && (IsWeaponFishingRod() == FALSE)) {
			dwEnemyCharID = GetFrontEnemyCharID(m_pPlayerChar->m_dwCharID, m_pPlayerChar->m_nDirection);
			if (dwEnemyCharID != 0) {
				StartLocalAtack();
				break;
			}
		}

		/* b. 正面に話せる相手(敵以外のNPC)がいれば会話する */
		dwFrontCharID = GetTalkCharID(m_pPlayerChar->m_dwCharID, m_pPlayerChar->m_nDirection);
		bResult = OnXChar(dwFrontCharID);
		if (bResult) {
			break;
		}

		/* c. 釣り竿装備で正面が水なら釣る(docs/battle-redesign.md S4。戦闘状態には
		   遷移させない。押しっぱなし連続は起こらない(m_bAtackKeyAutoRepeatを立てないため)) */
		if (IsFishingAvailable()) {
			StartLocalFishing();
			break;
		}

		/* d. 足元にアイテムがあれば拾う(戦闘状態(BATTLE静止)でも拾える) */
		{
			RECT rcFeet;
			/* 足元の当たり判定矩形を HALF_TILE 広げてアイテムを探す（向き非依存） */
			m_pPlayerChar->GetCollisionRect(rcFeet);
			rcFeet.left   -= HALF_TILE;
			rcFeet.top    -= HALF_TILE;
			rcFeet.right  += HALF_TILE;
			rcFeet.bottom += HALF_TILE;
			pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtrInRect(m_pPlayerChar->m_dwMapID, &rcFeet);
			if (pInfoItem) {
				/* アイテム拾う要求 */
				PacketCHAR_REQ_PUTGET.Make(m_pPlayerChar->m_dwCharID, 0);
				m_pSock->Send(&PacketCHAR_REQ_PUTGET);
				break;
			}
		}

		/* e. 会話・釣り・拾いが無ければ、戦闘可能マップなら空振り攻撃で戦闘状態へ自動遷移する(S2)。
		   釣り竿装備中は武器扱いしないため空振り攻撃も出さない(docs/battle-redesign.md 7章: 釣り竿装備中は攻撃しない) */
		if ((m_pMap && m_pMap->IsEnableBattle()) && (IsWeaponFishingRod() == FALSE)) {
			StartLocalAtack();
			break;
		}

		/* f. それ以外は従来どおり付いて行き解除 */
		if ((m_pPlayerChar->m_dwFrontCharID) || (m_pPlayerChar->m_dwTailCharID)) {
			PacketCHAR_REQ_TAIL.Make(m_pPlayerChar->m_dwCharID, 0, FALSE);
			m_pSock->Send(&PacketCHAR_REQ_TAIL);
		}
		break;

	default:
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}



BOOL CStateProcMAP::OnZ(BOOL bDown)
{
	BOOL bRet, bResult;
	DWORD dwCharID;
	PCInfoCharCli pInfoCharFront;
	CPacketCHAR_REQ_TAIL PacketCHAR_REQ_TAIL;
	CPacketCHAR_STATE Packet;

	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable() == FALSE) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime();

	if (bDown == FALSE) {
		/* docs/battle-redesign.md S3: 離した時は押した瞬間の判定(m_bZKeyDefenseActive)に
		   従って防御解除するか付いて行い要求を送るか決める(現在の状態でなく押下時の判定を使う。
		   戦闘状態の自動タイムアウト等で押下中に状態が変わっても意図が変わらないようにするため) */
		if (m_bZKeyDefenseActive) {
			m_bZKeyDefenseActive = FALSE;
			DefenseOff();
		} else {
			dwCharID = m_pLibInfoChar->GetFrontCharID(m_pPlayerChar->m_dwCharID, m_pPlayerChar->m_nDirection);
			PacketCHAR_REQ_TAIL.Make(m_pPlayerChar->m_dwCharID, dwCharID, TRUE);
			m_pSock->Send(&PacketCHAR_REQ_TAIL);
		}
		bRet = TRUE;
		goto Exit;
	}

	/* 押した瞬間: 付いて行き中・座り中・気絶中は不可 */
	if (m_pPlayerChar->m_dwFrontCharID) {
		goto Exit;
	}
	switch (m_pPlayerChar->m_nMoveState) {
	case CHARMOVESTATE_STAND:		// 立ち
	case CHARMOVESTATE_BATTLE:		// 戦闘中(静止)
		break;
	default:
		/* 座り中・気絶中・攻撃モーション中・既に防御中等はここで弾く */
		goto Exit;
	}
	bResult = m_pPlayerChar->IsChgWait();
	if (bResult) {
		goto Exit;
	}

	/* 正面にPCがいれば防御でなく付いて行き優先(PvPマップ設定は未実装のため常にPC優先。
	   離した時にGetFrontCharIDで改めて対象を取り直してREQ_TAILを送る) */
	dwCharID = m_pLibInfoChar->GetFrontCharID(m_pPlayerChar->m_dwCharID, m_pPlayerChar->m_nDirection);
	pInfoCharFront = (PCInfoCharCli)m_pLibInfoChar->GetPtr(dwCharID);
	if ((pInfoCharFront != NULL) && (pInfoCharFront->IsNPC() == FALSE)) {
		goto Exit;
	}

	/* それ以外は戦闘可能マップなら防御開始(STAND/BATTLEから自動で戦闘状態へ入る) */
	if ((m_pMap == NULL) || (m_pMap->IsEnableBattle() == FALSE)) {
		goto Exit;
	}

	Packet.Make(m_pPlayerChar->m_dwCharID, CHARMOVESTATE_BATTLE_DEFENSE);
	m_pSock->Send(&Packet);
	m_pPlayerChar->SetChgWait(TRUE);
	m_bZKeyDefenseActive = TRUE;
	/* 防御開始も「最後の戦闘行動」として自動解除タイマーを更新する(docs/battle-redesign.md S3) */
	m_dwLastAtackTime = timeGetTime();
	/* 防御中は押しっぱなし連続攻撃を止める(docs/battle-redesign.md S2) */
	m_bAtackKeyAutoRepeat = FALSE;
	bRet = TRUE;

Exit:
	return bRet;
}



BOOL CStateProcMAP::OnS(BOOL bDown)
{
	return FALSE;
}



BOOL CStateProcMAP::OnR(BOOL bDown)
{
	BOOL bRet, bResult;
	CPacketCHAR_STATE Packet;

	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable() == FALSE) {
		goto Exit;
	}
	if (bDown) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime();
	bResult = m_pPlayerChar->IsChgWait();
	/* 状態変更待ち？ */
	if (bResult) {
		goto Exit;
	}

	switch (m_pPlayerChar->m_nMoveState) {
	case CHARMOVESTATE_STAND:		// 立ち
		if (m_pPlayerChar->m_dwFrontCharID != 0) {
			/* ついていく中は座れない */
			goto Exit;
		}
		m_pPlayerChar->ChgMoveState(CHARMOVESTATE_SIT);
		break;
	case CHARMOVESTATE_SIT:			// 座り中
		m_pPlayerChar->ChgMoveState(CHARMOVESTATE_STAND);
		break;
	default:
		goto Exit;
	}

	Packet.Make(m_pPlayerChar->m_dwCharID, m_pPlayerChar->m_nMoveState);
	m_pSock->Send(&Packet);
	m_pPlayerChar->SetChgWait(TRUE);

	bRet = TRUE;
Exit:
	return bRet;
}



BOOL CStateProcMAP::OnB(BOOL bDown)
{
	BOOL bRet;
	PCWindowITEMMENU pWnd;

	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (m_pPlayerChar->m_nMoveState == CHARMOVESTATE_SWOON) {
		goto Exit;
	}
	if (IsKeyInputEnable() == FALSE) {
		goto Exit;
	}
	if (bDown) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime();

	pWnd = (PCWindowITEMMENU)m_pMgrWindow->GetWindow(WINDOWTYPE_ITEMMENU);
	if (pWnd) {
		goto Exit;
	}

	m_pMgrWindow->MakeWindowITEMMENU();
Exit:
	return bRet;
}



BOOL CStateProcMAP::OnN(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable() == FALSE) {
		goto Exit;
	}
	if (bDown) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime();

	if (m_pMgrData->GetDrawMode() != 0) {
		m_pMgrData->SetDrawMode(0);
		AddSystemMsg(FALSE, "名前非表示に変更されました　解除は[ N ]キー", RGB(255, 255, 255));
	} else {
		m_pMgrData->SetDrawMode(1);
		AddSystemMsg(FALSE, "名前を表示します", RGB(255, 255, 255));
	}
	m_pMgrData->SaveIniData();

Exit:
	return bRet;
}



BOOL CStateProcMAP::OnV(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable() == FALSE) {
		goto Exit;
	}
	if (bDown) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime();

	if (m_pMgrData->GetOptionViewItemName()) {
		m_pMgrData->SetOptionViewItemName(FALSE);
		AddSystemMsg(FALSE, "アイテム名を非表示にしました　解除は[ V ]キー", RGB(255, 255, 255));
	} else {
		m_pMgrData->SetOptionViewItemName(TRUE);
		AddSystemMsg(FALSE, "アイテム名を表示します", RGB(255, 255, 255));
	}
	m_pMgrData->SaveIniData();

Exit:
	return bRet;
}



BOOL CStateProcMAP::OnF(BOOL bDown)
{
	BOOL bRet;
	PCWindowSKILLMENU pWnd;

	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable() == FALSE) {
		goto Exit;
	}
	if (bDown) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime();

	pWnd = (PCWindowSKILLMENU)m_pMgrWindow->GetWindow(WINDOWTYPE_SKILLMENU);
	if (pWnd) {
		goto Exit;
	}

	m_pMgrWindow->MakeWindowSKILLMENU(0);
	bRet = TRUE;
Exit:
	return bRet;
}



BOOL CStateProcMAP::OnJ(BOOL bDown)
{
	BOOL bRet;
	PCWindowCHAR_STATUS pWnd;

	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable() == FALSE) {
		goto Exit;
	}
	if (bDown) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime();

	pWnd = (PCWindowCHAR_STATUS)m_pMgrWindow->GetWindow(WINDOWTYPE_CHAR_STATUS);
	if (pWnd) {
		goto Exit;
	}

	m_pMgrWindow->MakeWindowCHAR_STATUS();
	bRet = TRUE;
Exit:
	return bRet;
}



BOOL CStateProcMAP::OnL(BOOL bDown)
{
	BOOL bRet;
	PCWindowSKILLMENU pWnd;

	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable() == FALSE) {
		goto Exit;
	}
	if (bDown) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime();

	pWnd = (PCWindowSKILLMENU)m_pMgrWindow->GetWindow(WINDOWTYPE_SKILLMENU);
	if (pWnd) {
		goto Exit;
	}

	m_pMgrWindow->MakeWindowSKILLMENU(1);
	bRet = TRUE;
Exit:
	return bRet;
}



BOOL CStateProcMAP::OnAt(BOOL bDown)
{
	BOOL bRet;
	CPacketMSGCMD_PARA1 Packet;

	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable() == FALSE) {
		goto Exit;
	}
	if (bDown) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime();

	Packet.Make(SBOCOMMANDID_SUB_MSGCMD_BALLOON, m_pPlayerChar->m_dwCharID, m_dwLastBalloonID);
	m_pSock->Send(&Packet);

	bRet = TRUE;
Exit:
	return bRet;
}



BOOL CStateProcMAP::OnEnter(BOOL bDown)
{
	BOOL bRet;
	PCWindowCHAT pWnd;

	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable() == FALSE) {
		goto Exit;
	}
	if (bDown) {
		goto Exit;
	}
	pWnd = (PCWindowCHAT)m_pMgrWindow->GetWindow(WINDOWTYPE_CHAT);
	if (pWnd) {
		goto Exit;
	}

	m_pMgrWindow->MakeWindowCHAT();
Exit:
	return bRet;
}



BOOL CStateProcMAP::OnEscape(BOOL bDown)
{
	BOOL bRet;
	PCWindowSYSTEMMENU pWnd;

	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable() == FALSE) {
		goto Exit;
	}
	if (bDown) {
		goto Exit;
	}
	pWnd = (PCWindowSYSTEMMENU)m_pMgrWindow->GetWindow(WINDOWTYPE_SYSTEMMENU);
	if (pWnd) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime();

	m_pMgrWindow->MakeWindowSYSTEMMENU();
Exit:
	return bRet;
}



BOOL CStateProcMAP::OnTab(BOOL bDown)
{
	// docs/battle-redesign.md S2: 戦闘モードのTab切替は廃止。戦闘状態は攻撃(OnX)で自動遷移する
	return FALSE;
}



BOOL CStateProcMAP::OnShift(BOOL bDown)
{
	BOOL bRet, bResult;
	PCLayerMap pLayerMap;
	PCMgrKeyInput pMgrKeyInput;

	bRet = FALSE;
	pMgrKeyInput = m_pMgrData->GetMgrKeyInput();

	m_pPlayerChar = m_pMgrData->GetPlayerChar();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable() == FALSE) {
		goto Exit;
	}
	if (bDown) {
		pLayerMap = (PCLayerMap)m_pMgrLayer->Get(LAYERTYPE_MAP);
		pLayerMap->SetSystemIconMode(1);
		m_dwLastTimeMove = timeGetTime();

		bResult = pMgrKeyInput->IsInput(VK_CONTROL);
		if (bResult == FALSE) {
			goto Exit;
		}
		pLayerMap->SetCenterPos(m_pPlayerChar->m_nMapX, m_pPlayerChar->m_nMapY);
		AddSystemMsg(FALSE, "初期位置に視点を戻しました", RGB(255, 255, 255));
	}

	bRet = TRUE;
Exit:
	return bRet;
}



BOOL CStateProcMAP::OnCtrl(BOOL bDown)
{
	BOOL bRet;
	RECT rcTmp;
	CmyString strTmp;
	PCLayerMap pLayerMap;

	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable() == FALSE) {
		goto Exit;
	}
	if (bDown) {
		if (m_bChgScrollMode == FALSE) {
			m_bChgScrollMode = TRUE;
		}
		goto Exit;
	}

	if (m_bChgScrollMode == FALSE) {
		goto Exit;
	}

	m_bChgScrollMode = FALSE;
	m_nScrollMode ++;
	if (m_nScrollMode > 1) {
		m_nScrollMode = 0;
	}

	pLayerMap = (PCLayerMap)m_pMgrLayer->Get(LAYERTYPE_MAP);

	switch (m_nScrollMode) {
	case 0:
		strTmp = "スクロールします";
		pLayerMap->SetCenterPos(m_pPlayerChar->m_nMapX, m_pPlayerChar->m_nMapY);
		pLayerMap->SetScrollMode(TRUE, 0);
		break;
	case 1:
		strTmp = "画面固定します";
		pLayerMap->SetScrollMode(FALSE, 2);
		break;
	}
	if (strTmp.IsEmpty() == FALSE) {
		AddSystemMsg(FALSE, strTmp, RGB(255, 255, 255));
	}

	if (pLayerMap) {
		rcTmp.left	 = pLayerMap->m_nViewX - (MAPPARTSSIZE * 2);
		rcTmp.right	 = pLayerMap->m_nViewX + (DRAW_PARTS_X * MAPPARTSSIZE) + (MAPPARTSSIZE * 2);
		rcTmp.top	 = pLayerMap->m_nViewY - (MAPPARTSSIZE * 2);
		rcTmp.bottom = pLayerMap->m_nViewY + (DRAW_PARTS_Y * MAPPARTSSIZE) + (MAPPARTSSIZE * 2);
		m_pLibInfoItem->SetArea(m_pPlayerChar->m_dwMapID, &rcTmp);
	}

	bRet = TRUE;
Exit:
	return bRet;
}



BOOL CStateProcMAP::OnSpace(BOOL bDown)
{
	BOOL bRet;
	PCWindowCOMMANDMENU pWnd;

	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable() == FALSE) {
		goto Exit;
	}
	if (bDown) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime();

	switch (m_pPlayerChar->m_nMoveState) {
	case CHARMOVESTATE_SWOON:	// 気絶中
		pWnd = (PCWindowCOMMANDMENU)m_pMgrWindow->GetWindow(WINDOWTYPE_SWOON);
		if (pWnd) {
			goto Exit;
		}
		m_pMgrWindow->MakeWindowSWOON();
		break;
	default:
		pWnd = (PCWindowCOMMANDMENU)m_pMgrWindow->GetWindow(WINDOWTYPE_COMMANDMENU);
		if (pWnd) {
			goto Exit;
		}
		m_pMgrWindow->MakeWindowCOMMANDMENU();
		m_pMgrWindow->MakeWindowPLACEINFORMATION();
		m_pMgrWindow->MakeWindowCHAR_STATUS4();
		m_pMgrSound->PlaySound(SOUNDID_OPEN_WINDOW);
		break;
	}

	bRet = TRUE;
Exit:
	return bRet;
}



void CStateProcMAP::OnMgrDrawSTART_FADEIN(DWORD dwPara)
{
	PCLayerMap pLayerMap;
	CMainFrame *pMainFrame;
	RECT rcTmp;

	m_pPlayerChar = m_pMgrData->GetPlayerChar();
	if (m_pPlayerChar == NULL) {
		return;
	}
	if (m_pMgrLayer == NULL) {
		return;
	}
	pMainFrame = m_pMgrData->GetMainFrame();
	pMainFrame->ChgMoveState(FALSE);
	ResetPlayerMoveSyncState();

	pLayerMap = (PCLayerMap)m_pMgrLayer->Get(LAYERTYPE_MAP);
	if (pLayerMap) {
		pLayerMap->RenewMapName(NULL);
		pLayerMap->SetCenterPos(m_pPlayerChar->m_nMapX, m_pPlayerChar->m_nMapY);
		rcTmp.left	 = pLayerMap->m_nViewX - (MAPPARTSSIZE * 2);
		rcTmp.right	 = pLayerMap->m_nViewX + (DRAW_PARTS_X * MAPPARTSSIZE) + (MAPPARTSSIZE * 2);
		rcTmp.top	 = pLayerMap->m_nViewY - (MAPPARTSSIZE * 2);
		rcTmp.bottom = pLayerMap->m_nViewY + (DRAW_PARTS_Y * MAPPARTSSIZE) + (MAPPARTSSIZE * 2);
		m_pLibInfoItem->SetArea(m_pPlayerChar->m_dwMapID, &rcTmp);
	}
}



void CStateProcMAP::OnMgrDrawEND_FADEIN(DWORD dwPara)
{
	PCLayerMap pLayerMap;

	pLayerMap = (PCLayerMap)m_pMgrLayer->Get(LAYERTYPE_MAP);
	if (pLayerMap == NULL) {
		return;
	}
	if (m_pMap == NULL) {
		return;
	}
	CString strMapName = Utf8ToTString((LPCSTR)m_pMap->m_strMapName);
	pLayerMap->RenewMapName(strMapName);
}



// ─────────────────────────────────────────────
// S3b: 押せる物(Push=1 NPC)の押し予測。docs/push-object-redesign.md 4章。
// サーバー(SboSvr/src/MainFrame/MainFrameRecvProcCHAR.cpp の
// RecvProcCHAR_REQ_PUSH/IsPushMapFree/IsPushCharAreaFree)と同じ判定
// (CLibInfoCharBase::CanMoveDirection/IsPushAreaFree、Commonで共用)を使い、
// ずれによる引き戻しを減らす。
// ─────────────────────────────────────────────

// 診断用。通常は0。原因調査時に1にする。斜め押しで止めた直後に押せる物が
// 少し逆走する不具合の調査用ログ。予測の1pxごと(TryPushObject)は多すぎるので
// 出さず、それ以外の座標変更イベント(受理/却下/位置パケット/予測終了/接触喪失時の
// 即時送信)だけ SboDbgLog へ出す。ブラウザのコンソールで "[PushDbg]" で検索できる。
#define PUSH_CLIENT_DEBUG_LOG 0

void CStateProcMAP::SendReqPush(DWORD dwObjCharID, int nPushDir, CInfoCharCli *pInfoObj, BOOL bRelease, int nPushType)
{
	CPacketCHAR_REQ_PUSH PacketReqPush;
	POINT ptObjTarget, ptSelf;

	if ((m_pPlayerChar == NULL) || (pInfoObj == NULL) || (m_pSock == NULL)) {
		return;
	}
	ptObjTarget.x = pInfoObj->m_nMapX;
	ptObjTarget.y = pInfoObj->m_nMapY;
	ptSelf.x = m_pPlayerChar->m_nMapX;
	ptSelf.y = m_pPlayerChar->m_nMapY;

	// S5: 入れ替わり中はPUSHTYPE_SWAPで送る(docs/push-object-redesign.md 2章7項)
	PacketReqPush.Make(dwObjCharID, nPushDir, nPushType, ptObjTarget, ptSelf, timeGetTime(), bRelease);
	m_pSock->Send(&PacketReqPush);

#if PUSH_CLIENT_DEBUG_LOG
	SboDbgLog("[PushDbg]SEND t:%d d:%d tgt:%d,%d self:%d,%d rel:%d swapAct:%d",
		nPushType, nPushDir, ptObjTarget.x, ptObjTarget.y, ptSelf.x, ptSelf.y, bRelease ? 1 : 0, m_bPushSwapActive ? 1 : 0);
#endif

	m_dwLastTimePushSyncSend = timeGetTime();
	// S3b: 接触喪失時の即時送信(MoveProc)が同じ座標で重複送信しないための記録
	m_nLastPushSyncSentX = ptObjTarget.x;
	m_nLastPushSyncSentY = ptObjTarget.y;
	m_bLastPushSyncSentValid = TRUE;
	if (nPushType == PUSHTYPE_SWAP) {
		// S5: 一時停止中も入れ替わり状態を保つため、最後にSWAPを送った時刻を別に
		// 記録する(1800ms音信不通なら手放す。親レビュー指摘)
		m_dwLastTimePushSwapSend = timeGetTime();
	}
}

void CStateProcMAP::EndPushPredict(BOOL bSendFinal)
{
	PCInfoCharCli pInfoObj;
	BOOL bSwapObj;

	if (m_bPushSyncActive == FALSE) {
		return;
	}
	pInfoObj = (PCInfoCharCli)m_pLibInfoChar->GetPtr(m_dwPushSyncObjCharID);
	// S5: 対象が入れ替わり中のボール本体かどうか。親レビュー指摘: キーを離した時や
	// 接触喪失300msのタイムアウトはあくまで「一時停止」であって「手放し」ではない。
	// ここでは押し送信の状態(m_bPushSyncActive等)だけ片付け、入れ替わり状態
	// (m_bPushSwapActive等)とボールのm_bPushPredicting/所有者は保ったままにする
	// (受信更新に乱されず、同じ向きで再開したら続きから入れ替われるように)。
	// 入れ替わり状態を実際に消すのはDiscardPushSwapState(完了/向き変更/マップ切替/
	// 却下/1800msタイムアウト)だけの役目にする。
	bSwapObj = m_bPushSwapActive && (m_dwPushSwapObjCharID == m_dwPushSyncObjCharID);
	if (bSendFinal && (pInfoObj != NULL)) {
		int nPushType;

		// S5: 入れ替わり中に止まった(=本人が動くのをやめた)場合もSWAPで送る
		nPushType = bSwapObj ? PUSHTYPE_SWAP : PUSHTYPE_PUSH;
		// 離す直前の最新座標をすぐ反映させる(最大100ms分の送信間引きを打ち切る)。
		// S4: 離した印(bRelease=TRUE)を付け、サーバーが150msタイムアウトを待たず
		// 即停止できるようにする(docs/push-object-redesign.md S4)。
		SendReqPush(m_dwPushSyncObjCharID, m_nPushSyncDirection, pInfoObj, TRUE, nPushType);
	}
	if ((pInfoObj != NULL) && m_pPlayerChar && (pInfoObj->m_dwPushPredictOwnerCharID == m_pPlayerChar->m_dwCharID)) {
#if PUSH_CLIENT_DEBUG_LOG
		SboDbgLog("[PushDbg][予測終了][obj:%u][pos:%d,%d][bSendFinal:%d][swap一時停止:%d]",
			pInfoObj->m_dwCharID, pInfoObj->m_nMapX, pInfoObj->m_nMapY, bSendFinal ? 1 : 0, bSwapObj ? 1 : 0);
#endif
		if (!bSwapObj) {
			pInfoObj->m_bPushPredicting = FALSE;
			// S4: 予測を終えた時刻を記録する。直後(1000ms以内)に届く自分の押し更新は
			// DRで動かすと行き過ぎた予測分がもう一度戻る動きに見えるため、
			// RecvProcCHAR_MOVE_CORE 側でこの時刻を見て一時的に直接座標合わせへ戻す
			// (docs/push-object-redesign.md S4)。
			pInfoObj->m_dwPushPredictEndTime = SDL_GetTicks();
		}
		// 追従用の経由点が万一残っていれば破棄する。押せる物は
		// RecvProcCHAR_MOVE_CORE 側で既にキューへ乗せない扱いにしているため
		// 通常は空だが、予測を終える境目で古い点が再生されないよう防御的に消す
		// (docs/push-object-redesign.md 4章)。
		pInfoObj->m_bWaypointMove = FALSE;
		if (pInfoObj->m_apMovePosQue.size() > 0) {
			pInfoObj->DeleteAllMovePosQue();
		}
		pInfoObj->ForceStopMoveState(CHARMOVESTATE_STAND);
	}
	m_bPushSyncActive = FALSE;
	m_dwPushSyncObjCharID = 0;
	m_nPushSyncDirection = -1;
	m_dwLastTimePushSyncSend = 0;
	m_dwLastTimePushContact = 0;
	m_bLastPushSyncSentValid = FALSE;
	m_bPushReleaseSent = FALSE;
}

void CStateProcMAP::DiscardPushSwapState(BOOL bClearOwnership, LPCSTR pszReason)
{
	PCInfoCharCli pInfoObj;

	// S5: 入れ替わり状態を実際に消す唯一の場所(完了/向き変更/マップ切替/
	// RES_PUSH却下/1800ms無応答タイムアウト)。親レビュー指摘。
	if (m_bPushSwapActive == FALSE) {
		return;
	}
#if PUSH_CLIENT_DEBUG_LOG
	SboDbgLog("[PushDbg]SWAPEND why:%s", (pszReason != NULL) ? pszReason : "");
#endif
	if (bClearOwnership) {
		pInfoObj = (PCInfoCharCli)m_pLibInfoChar->GetPtr(m_dwPushSwapObjCharID);
		if ((pInfoObj != NULL) && m_pPlayerChar && (pInfoObj->m_dwPushPredictOwnerCharID == m_pPlayerChar->m_dwCharID)) {
			pInfoObj->m_bPushPredicting = FALSE;
			pInfoObj->m_dwPushPredictEndTime = SDL_GetTicks();
			pInfoObj->m_dwPushPredictOwnerCharID = 0;
		}
	}
	m_bPushSwapActive = FALSE;
	m_dwPushSwapObjCharID = 0;
	m_nPushSwapDirection = -1;
	m_dwLastTimePushSwapSend = 0;
}

BOOL CStateProcMAP::TryPushObject(PCInfoMapBase pMap, DWORD dwObjCharID, int nPushDir)
{
	static const int anPosX[] = {0, 0, -1, 1};
	static const int anPosY[] = {-1, 1, 0, 0};
	PCInfoCharCli pInfoObj;
	RECT rcObjMoveTo;
	int nSaveX, nSaveY, nNewX, nNewY;
	BOOL bMapFree, bAreaFree;

	if ((pMap == NULL) || (m_pPlayerChar == NULL)) {
		return FALSE;
	}
	pInfoObj = (PCInfoCharCli)m_pLibInfoChar->GetPtr(dwObjCharID);
	if (pInfoObj == NULL) {
		return FALSE;
	}
	// サーバー(RecvProcCHAR_REQ_PUSH)は CHARMOVETYPE_PUTNPC を押し対象から除外して
	// 即座に却下する。送っても無駄なので予測もしない。
	if (pInfoObj->m_nMoveType == CHARMOVETYPE_PUTNPC) {
		return FALSE;
	}
	// 他の人が押し予測中(専有)なら、こちらでは押さない(横取りしない)。
	// 自分が既に予測中の物はそのまま続行してよい。
	if (pInfoObj->m_bPushPredicting && (pInfoObj->m_dwPushPredictOwnerCharID != m_pPlayerChar->m_dwCharID)) {
		return FALSE;
	}

	nNewX = pInfoObj->m_nMapX + anPosX[nPushDir];
	nNewY = pInfoObj->m_nMapY + anPosY[nPushDir];

	// マップ判定: サーバーと共用のCanMoveDirection(Common)を使う
	bMapFree = m_pLibInfoChar->CanMoveDirection(pMap, pInfoObj, nPushDir);
	if (!bMapFree) {
		return FALSE;
	}

	// キャラ判定: 押している本人・押せる物自身を除く全キャラ(サーバーと共用のIsPushAreaFree)
	nSaveX = pInfoObj->m_nMapX;
	nSaveY = pInfoObj->m_nMapY;
	pInfoObj->m_nMapX = nNewX;
	pInfoObj->m_nMapY = nNewY;
	pInfoObj->GetCollisionRect(rcObjMoveTo);
	pInfoObj->m_nMapX = nSaveX;
	pInfoObj->m_nMapY = nSaveY;

	bAreaFree = m_pLibInfoChar->IsPushAreaFree(m_pPlayerChar, pInfoObj, pInfoObj->m_dwMapID, rcObjMoveTo);
	if (!bAreaFree) {
		return FALSE;
	}

	// 二重駆動防止: 直前まで見ていた側として DR(Dead Reckoning) 中だった場合、
	// これから始めるローカル押し予測と競合しないよう先に止める(座標はそのまま)。
	if (pInfoObj->m_bPredictedMove) {
		pInfoObj->m_bPredictedMove = FALSE;
		pInfoObj->m_nPredictDirection = -1;
	}

	// 予測: 押せる物をローカルで1px動かす
	pInfoObj->SetPos(nNewX, nNewY);
	pInfoObj->SetDirection(nPushDir);
	if (pInfoObj->m_nMoveState != CHARMOVESTATE_MOVE) {
		pInfoObj->ChgMoveState(CHARMOVESTATE_MOVE);
	}
	pInfoObj->m_bPushPredicting = TRUE;
	pInfoObj->m_dwPushPredictOwnerCharID = m_pPlayerChar->m_dwCharID;

	return TRUE;
}

// S5: ボールとの入れ替わり開始を試す(docs/push-object-redesign.md 2章7項)。
// pInfoObj(BALL)の1px先がTryPushObjectで塞がっていた時に呼ばれる。開始前に
// ボールの逆向き(-nDirection)の経路を、本人の当たり幅ぶん1pxずつマップ・
// 本人/ボール以外の全キャラで空いているか確認し、空いていれば入れ替わり状態を
// 初期化してTRUEを返す。塞がっていればFALSEを返し、何も変更しない。
BOOL CStateProcMAP::TryStartPushSwap(PCInfoMapBase pMap, CInfoCharCli *pInfoObj, int nDirection)
{
	static const int anPosX[] = {0, 0, -1, 1};
	static const int anPosY[] = {-1, 1, 0, 0};
	static const int anOppositeDir[] = {1, 0, 3, 2};
	RECT rcSelf, rcMoveTo;
	int nWidth, i, nOppositeDir;
	int nSaveX, nSaveY;
	BOOL bMapFree, bAreaFree;

	if ((pMap == NULL) || (m_pPlayerChar == NULL) || (pInfoObj == NULL)) {
		return FALSE;
	}
	if ((nDirection < 0) || (nDirection > 3)) {
		return FALSE;
	}
	// 他の人が押し予測中(専有)なら、こちらでは入れ替わりを始めない
	if (pInfoObj->m_bPushPredicting && (pInfoObj->m_dwPushPredictOwnerCharID != m_pPlayerChar->m_dwCharID)) {
		return FALSE;
	}
	nOppositeDir = anOppositeDir[nDirection];

	// 本人の当たり幅(d軸方向)ぶん、ボールの逆向きの経路を1pxずつ確認する
	m_pPlayerChar->GetCollisionRect(rcSelf);
	// 当たり矩形は端を含む(横x..x+31、縦y-15..y)ので +1 して実際の幅(32/16px)にする。
	// +1しないと入れ替わり完了後もキャラとボールが重なったまま残る。
	nWidth = (nDirection <= 1) ? (rcSelf.bottom - rcSelf.top + 1) : (rcSelf.right - rcSelf.left + 1);
	if (nWidth <= 0) {
		return FALSE;
	}

	nSaveX = pInfoObj->m_nMapX;
	nSaveY = pInfoObj->m_nMapY;
	for (i = 0; i < nWidth; i ++) {
		bMapFree = m_pLibInfoChar->CanMoveDirection(pMap, pInfoObj, nOppositeDir);
		if (!bMapFree) {
			pInfoObj->m_nMapX = nSaveX;
			pInfoObj->m_nMapY = nSaveY;
			return FALSE;
		}
		pInfoObj->m_nMapX += anPosX[nOppositeDir];
		pInfoObj->m_nMapY += anPosY[nOppositeDir];
		pInfoObj->GetCollisionRect(rcMoveTo);
		// 本人・ボール自身以外の全キャラと当たらないか(サーバーと共用のIsPushAreaFree)
		bAreaFree = m_pLibInfoChar->IsPushAreaFree(m_pPlayerChar, pInfoObj, pInfoObj->m_dwMapID, rcMoveTo);
		if (!bAreaFree) {
			pInfoObj->m_nMapX = nSaveX;
			pInfoObj->m_nMapY = nSaveY;
			return FALSE;
		}
	}
	pInfoObj->m_nMapX = nSaveX;
	pInfoObj->m_nMapY = nSaveY;

	// 親レビュー指摘: 壁際まで押した最後の数pxがまだ100ms間引き中で未送信だと、
	// サーバー側のボールが壁の手前に残ったままで「入れ替わり先が空いている」と
	// 誤判定されCanStartSwapが偽になる。入れ替わりを始める前に、ボールの現在地
	// (=B0)を通常PUSHで1回送っておく(TCPなのでPUSH→SWAPの順で処理される)。
	// 直前と同じ座標を送信済みなら省略する。
	{
		BOOL bFlush;

		bFlush = !m_bLastPushSyncSentValid ||
			(m_nLastPushSyncSentX != pInfoObj->m_nMapX) || (m_nLastPushSyncSentY != pInfoObj->m_nMapY);
		if (bFlush) {
			SendReqPush(pInfoObj->m_dwCharID, nDirection, pInfoObj, FALSE, PUSHTYPE_PUSH);
		}
#if PUSH_CLIENT_DEBUG_LOG
		SboDbgLog("[PushDbg]SWAPSTART P0:%d,%d B0:%d,%d d:%d w:%d flush:%d",
			m_pPlayerChar->m_nMapX, m_pPlayerChar->m_nMapY, pInfoObj->m_nMapX, pInfoObj->m_nMapY,
			nDirection, nWidth, bFlush ? 1 : 0);
#endif
	}

	// 二重駆動防止: DR中だった場合は先に止める(座標はそのまま)
	if (pInfoObj->m_bPredictedMove) {
		pInfoObj->m_bPredictedMove = FALSE;
		pInfoObj->m_nPredictDirection = -1;
	}

	m_bPushSwapActive = TRUE;
	m_dwPushSwapObjCharID = pInfoObj->m_dwCharID;
	m_nPushSwapDirection = nDirection;
	// 1800msの放置判定(最後にSWAPを送った時刻)の基準をここで取り直す。
	// 前回の入れ替わりを手放した時の0のままだと、開始した次のフレームで
	// 即座に「放置」と判定されて打ち切られ、入れ替わりが始まらない。
	m_dwLastTimePushSwapSend = timeGetTime();
	m_ptPushSwapP0.x = m_pPlayerChar->m_nMapX;
	m_ptPushSwapP0.y = m_pPlayerChar->m_nMapY;
	m_ptPushSwapB0.x = pInfoObj->m_nMapX;
	m_ptPushSwapB0.y = pInfoObj->m_nMapY;

	// ボールの向きは逆向き(-d)にしておく。RecvProcCHAR_MOVE_CORE側の押し予測の
	// 遅延判定(m_nDirectionを使う)と整合させるため(親からの指示事項)
	pInfoObj->SetDirection(nOppositeDir);
	if (pInfoObj->m_nMoveState != CHARMOVESTATE_MOVE) {
		pInfoObj->ChgMoveState(CHARMOVESTATE_MOVE);
	}
	pInfoObj->m_bPushPredicting = TRUE;
	pInfoObj->m_dwPushPredictOwnerCharID = m_pPlayerChar->m_dwCharID;

	return TRUE;
}

void CStateProcMAP::EndPushSwapOnDirectionChange(int nNewPushDir)
{
	PCInfoCharCli pInfoObj;

	if (m_bPushSwapActive == FALSE) {
		return;
	}
	pInfoObj = (PCInfoCharCli)m_pLibInfoChar->GetPtr(m_dwPushSwapObjCharID);
	if ((pInfoObj != NULL) && (m_pPlayerChar != NULL) && (m_pSock != NULL)) {
		CPacketCHAR_REQ_PUSH PacketReqPush;
		POINT ptObjTarget, ptSelf;

		ptObjTarget.x = pInfoObj->m_nMapX;
		ptObjTarget.y = pInfoObj->m_nMapY;
		ptSelf.x = m_pPlayerChar->m_nMapX;
		ptSelf.y = m_pPlayerChar->m_nMapY;
		// 新しい向きへSWAP要求を1回送る(サーバーが2倍速で自走させる。docs 2章7項)
		PacketReqPush.Make(m_dwPushSwapObjCharID, nNewPushDir, PUSHTYPE_SWAP, ptObjTarget, ptSelf, timeGetTime(), FALSE);
		m_pSock->Send(&PacketReqPush);

#if PUSH_CLIENT_DEBUG_LOG
		SboDbgLog("[PushDbg]SEND t:%d d:%d tgt:%d,%d self:%d,%d rel:%d swapAct:%d",
			PUSHTYPE_SWAP, nNewPushDir, ptObjTarget.x, ptObjTarget.y, ptSelf.x, ptSelf.y, 0, m_bPushSwapActive ? 1 : 0);
#endif

		if (pInfoObj->m_dwPushPredictOwnerCharID == m_pPlayerChar->m_dwCharID) {
			pInfoObj->m_bPushPredicting = FALSE;
			// 以後の自走(eject)は、押している本人の画面ではDRを使わず、届いた座標へ
			// 直接合わせる(RecvProcCHAR_MOVE_CORE の「予測終了直後1000msは直接合わせる」
			// 例外を使うため、所有者は残したまま終了時刻を入れる)。DRだと止まる時に
			// 先読みで行き過ぎた分だけ本人の方へ引き戻され、歩き続けている本人と
			// 重なって押せなくなる(重なった相手は押す判定から外れるため)。
			pInfoObj->m_dwPushPredictEndTime = SDL_GetTicks();
		}
	}
	// 向き変更は入れ替わりの正式な終わり(eject)。所有権は上で既に手放しているので
	// bClearOwnership=FALSEで入れ替わり状態(m_bPushSwapActive等)だけ片付ける。
	DiscardPushSwapState(FALSE, "dirchg");
	// 押し送信管理(m_bPushSyncActive等)もここで片付ける(親レビュー指摘: 片付け
	// ないと古い向きのまま接触喪失送信が出る)。
	EndPushPredict(FALSE);
}

BOOL CStateProcMAP::TryMoveOrPushDirection(PCInfoMapBase pMap, int nDirection, int nPushDir, int nMovePixel, DWORD &dwPushObjCharIDOut)
{
	static const int anOppositeDir[] = {1, 0, 3, 2};
	static const int anPosX[] = {0, 0, -1, 1};
	static const int anPosY[] = {-1, 1, 0, 0};
	BOOL bResult;
	DWORD dwObjCharID;

	// 通常キャラのブロック判定(押せる物以外)
	bResult = m_pLibInfoChar->IsBlockChar(m_pPlayerChar, nDirection, TRUE, TRUE);
	if (bResult) {
		return TRUE;
	}

	// S5: 入れ替わり継続中は、本人とそのボールの当たり判定をしない仕様
	// (docs/push-object-redesign.md 2章7項)。GetPushBlockCharID(Common/LibInfo/
	// LibInfoCharBase.cpp)は既に重なっている相手を除外するため、重なった時点で
	// 検出漏れし継続できなくなる(親レビュー指摘)。GetPushBlockCharIDに頼らず、
	// ここでボールが逆向きへ今回の移動量(nMovePixel)ぶん進めるか直接確認する。
	if (m_bPushSwapActive && (nDirection == m_nPushSwapDirection)) {
		PCInfoCharCli pInfoSwapObj;
		int nOppositeDir, i, nSaveX, nSaveY, nSteps;
		BOOL bFree;

		pInfoSwapObj = (PCInfoCharCli)m_pLibInfoChar->GetPtr(m_dwPushSwapObjCharID);
		if (pInfoSwapObj == NULL) {
			return TRUE;
		}
		nOppositeDir = anOppositeDir[m_nPushSwapDirection];
		nSteps = (nMovePixel > 0) ? nMovePixel : 1;

		// MoveProcが1回に複数pxまとめて進めても判定がずれないよう、実際に進む
		// 距離ぶん1pxずつ経路を確認する(途中の1pxだけ塞がっているケースを見逃さない)
		nSaveX = pInfoSwapObj->m_nMapX;
		nSaveY = pInfoSwapObj->m_nMapY;
		bFree = TRUE;
		for (i = 0; i < nSteps; i ++) {
			RECT rcMoveTo;
			BOOL bMapFree, bAreaFree;

			bMapFree = m_pLibInfoChar->CanMoveDirection(pMap, pInfoSwapObj, nOppositeDir);
			if (!bMapFree) {
				bFree = FALSE;
				break;
			}
			pInfoSwapObj->m_nMapX += anPosX[nOppositeDir];
			pInfoSwapObj->m_nMapY += anPosY[nOppositeDir];
			pInfoSwapObj->GetCollisionRect(rcMoveTo);
			bAreaFree = m_pLibInfoChar->IsPushAreaFree(m_pPlayerChar, pInfoSwapObj, pInfoSwapObj->m_dwMapID, rcMoveTo);
			if (!bAreaFree) {
				bFree = FALSE;
				break;
			}
		}
		pInfoSwapObj->m_nMapX = nSaveX;
		pInfoSwapObj->m_nMapY = nSaveY;

		if (!bFree) {
			// 割り込み等で塞がった: 二人とも止める
			return TRUE;
		}
		dwPushObjCharIDOut = pInfoSwapObj->m_dwCharID;
		return FALSE;
	}

	dwObjCharID = m_pLibInfoChar->GetPushBlockCharID(m_pPlayerChar, nDirection);
	if (dwObjCharID == 0) {
		return FALSE;
	}
	// 押している向き(nPushDir)の軸成分を含む移動でなければ、押さずに今まで通り固い物として扱う
	// (斜め移動で押す向きと違う軸に押せる物がある場合。入れ替わりはS5で別途対応)
	if (nDirection != nPushDir) {
		return TRUE;
	}
	// 入れ替わり中(上のnDirection==m_nPushSwapDirectionブロックでは無い軸、または
	// 既に別のボールで入れ替わり中)は、新たな押しを始めず固い物として扱う
	if (m_bPushSwapActive) {
		return TRUE;
	}
	if (TryPushObject(pMap, dwObjCharID, nPushDir) == FALSE) {
		PCInfoCharCli pInfoObj;

		// マップ/他キャラで塞がっていて押せない: BALLなら入れ替わり開始を試す(S5)。
		// それ以外(岩・箱、または開始失敗)は今まで通りプレイヤーもその軸では進めない。
		pInfoObj = (PCInfoCharCli)m_pLibInfoChar->GetPtr(dwObjCharID);
		if ((pInfoObj != NULL) && (pInfoObj->m_nMoveType == CHARMOVETYPE_BALL) &&
			TryStartPushSwap(pMap, pInfoObj, nDirection)) {
			dwPushObjCharIDOut = dwObjCharID;
			return FALSE;
		}
		return TRUE;
	}
	dwPushObjCharIDOut = dwObjCharID;
	return FALSE;
}



BOOL CStateProcMAP::MoveProc(
	int x,				// [in] 現在位置(ヨコ)
	int y,				// [in] 現在位置(タテ)
	int xx,				// [in] 増減(ヨコ)
	int yy,				// [in] 増減(タテ)
	int nDirection,		// [in] 向き
	BOOL bSyncSend)		// [in] サーバ同期送信を行う
{
	int nDirectionBack, nDirectionView, nState, nTmp, nInputDirection, nKeepDirection, nOtherDirection, xBack, yBack, nMovePixel,
		anPosChangeX[] = {0, 0, -1, 1, 1, 1, -1, -1}, anPosChangeY[] = {-1, 1, 0, 0, -1, 1, 1, -1};
	int nPushDir;			// S3b: 押している人の向き(4方向)。押し判定・送信に使う
	DWORD dwPushObjCharID;	// S3b: このMoveProc呼び出しで実際に押した押せる物のCharID(0:押していない)
	BOOL bRet, bResult;
	RECT rcTmp;
	PCInfoMapBase pMap;
	PCLayerMap pLayerMap;
	PCMgrKeyInput pMgrKeyInput;
	CPacketCHAR_MOVE_START PacketMoveStart;
	CPacketCHAR_MOVE_DIR_CHANGE PacketMoveDirChange;
	CPacketCHAR_STATE PacketSTATE;
	ARRAYINT anDirection;

	nPushDir = -1;
	dwPushObjCharID = 0;

	bRet = FALSE;
	m_pPlayerChar = m_pMgrData->GetPlayerChar();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}

	m_bChgScrollMode = FALSE;

	xBack = m_pPlayerChar->m_nMapX;
	yBack = m_pPlayerChar->m_nMapY;
	nMovePixel = max(abs(xx), abs(yy));
	nInputDirection = nDirection;
	nDirectionView = nDirection;
	nDirectionBack = m_pPlayerChar->m_nDirection;

	pLayerMap		= (PCLayerMap)m_pMgrLayer->Get(LAYERTYPE_MAP);
	pMgrKeyInput	= m_pMgrData->GetMgrKeyInput();

	pMap = m_pMgrData->GetMap();
	if (pMap == NULL) {
		goto Exit;
	}
	/* 移動せずにスクロールだけ？ */
	bResult = pMgrKeyInput->IsInput(VK_CONTROL);
	if (bResult) {
		/* Phase 3: m_nViewX/Y がpx単位になったので変換不要（IsScrollArea 内部も更新済み） */
		bResult = pLayerMap->IsScrollArea(x, y, nDirection);
		if (bResult) {
			pLayerMap->SetScrollMode(TRUE, 1);
			pLayerMap->Scroll(nDirection);
			if (m_nScrollMode == 1) {
				pLayerMap->SetScrollMode(FALSE, 2);
			}
		}
		bRet = TRUE;
		goto Exit;
	}
	/* Phase 4: カメラ追随・フレームごと移動のため IsMove() ブロック不要 */
	bResult = m_pPlayerChar->IsChgWait();
	/* 状態変更待ち？ */
	if (bResult) {
		goto Exit;
	}
	/* 誰かに付いて行っている？ */
	if (m_pPlayerChar->m_dwFrontCharID) {
		goto Exit;
	}
	bResult = m_pPlayerChar->IsEnableMove();
	if ((bResult == FALSE) && (bSyncSend == FALSE)) {
		if ((m_pPlayerChar->m_nMoveState == CHARMOVESTATE_MOVE) ||
			(m_pPlayerChar->m_nMoveState == CHARMOVESTATE_BATTLEMOVE)) {
			bResult = TRUE;
		}
	}
	if (bResult == FALSE) {
		if (m_pPlayerChar->m_nMoveState == CHARMOVESTATE_BATTLE_DEFENSE) {
			/* 防御中は向きの変更だけ */
			m_pPlayerChar->ChgDirection(nDirection);
			goto ExitSend;
		}
		goto Exit;
	}

	if ((nDirectionBack < 4) && (nDirection >= 4)) {
		bResult = TRUE;
		switch (nDirectionBack) {
		case 0:
			if (yy > 0) {
				bResult = FALSE;
			}
			break;
		case 1:
			if (yy < 0) {
				bResult = FALSE;
			}
			break;
		case 2:
			if (xx > 0) {
				bResult = FALSE;
			}
			break;
		case 3:
			if (xx < 0) {
				bResult = FALSE;
			}
			break;
		}
		if (bResult) {
			/* 斜めに移動する時は最後の4方向を維持する */
			nDirectionView = nDirectionBack;
		}
	}
	if (m_pPlayerChar->IsStateBattle()) {
		bResult = pMgrKeyInput->IsInput('X');
		if (bResult) {
			nDirectionView = nDirectionBack;
		}
	}
	m_pPlayerChar->ChgDirection(nDirection);
	m_pPlayerChar->ClearDrawDirectionOverride();

	/* 移動せずに向き変更だけ？ */
	bResult = pMgrKeyInput->IsInput(VK_SHIFT);
	if (bResult) {
		goto ExitSend;
	}
	/* 座っている？ */
	if ((m_pPlayerChar->m_nMoveState == CHARMOVESTATE_SIT) ||
		(m_pPlayerChar->m_nMoveState == CHARMOVESTATE_SLEEPTIMER)) {
		x = m_pPlayerChar->m_nMapX;
		y = m_pPlayerChar->m_nMapY;
		goto ExitSend;
	}

	nTmp = nDirection;
	nKeepDirection = -1;
	nOtherDirection = -1;
	if ((nInputDirection >= 4) && (nInputDirection <= 7) && (nDirectionBack >= 0) && (nDirectionBack < 4)) {
		switch (nInputDirection) {
		case 4:
			if (nDirectionBack == 0) {
				nKeepDirection = 0;
				nOtherDirection = 3;
			} else if (nDirectionBack == 3) {
				nKeepDirection = 3;
				nOtherDirection = 0;
			}
			break;
		case 5:
			if (nDirectionBack == 1) {
				nKeepDirection = 1;
				nOtherDirection = 3;
			} else if (nDirectionBack == 3) {
				nKeepDirection = 3;
				nOtherDirection = 1;
			}
			break;
		case 6:
			if (nDirectionBack == 1) {
				nKeepDirection = 1;
				nOtherDirection = 2;
			} else if (nDirectionBack == 2) {
				nKeepDirection = 2;
				nOtherDirection = 1;
			}
			break;
		case 7:
			if (nDirectionBack == 0) {
				nKeepDirection = 0;
				nOtherDirection = 2;
			} else if (nDirectionBack == 2) {
				nKeepDirection = 2;
				nOtherDirection = 0;
			}
			break;
		}
	}
	if (nKeepDirection != -1) {
		int nDirectionTmp;

		nDirectionTmp = nOtherDirection;
		bResult = m_pLibInfoChar->IsMove(m_pPlayerChar, nDirectionTmp);
		if (bResult == FALSE) {
			nDirection = nKeepDirection;
			xx = anPosChangeX[nDirection] * nMovePixel;
			yy = anPosChangeY[nDirection] * nMovePixel;
		}
	}
	bResult = m_pMgrData->GetMoveNoBlock();
	if (bResult == FALSE) {
		/* 当たり判定無効状態でないならチェック */
		bResult = m_pLibInfoChar->IsMove(m_pPlayerChar, nDirection);
	}
	if (bResult) {
		/* IsMove 内のスライド補正・斜め補正はキャラ座標を直接ずらして許可を返すことがある。
		   補正後の座標を基準にしないと、未検証の元座標のまま移動が適用されて
		   障害物にめり込むため、ここで基準座標を最新化する */
		x = m_pPlayerChar->m_nMapX;
		y = m_pPlayerChar->m_nMapY;
		if (nTmp != nDirection) {
			xx = anPosChangeX[nDirection] * nMovePixel;
			yy = anPosChangeY[nDirection] * nMovePixel;
			m_pPlayerChar->ChgDirection(nDirection);
		}

	} else {
		m_pPlayerChar->RenewBlockMapArea(0, 0, -1);
		/* 壁に当たって移動できない場合でも、現在位置でイベントチェックを行う
		   （移動なしだと到達しない 2870 行のフラグ設定をここで補完する） */
		if (!m_bAutoWalkToEvent) {
			m_pPlayerChar->m_bWaitCheckMapEvent = TRUE;
			m_bSendCheckMapEvent = FALSE;
		}
		if (nDirection != nDirectionBack) {
			goto ExitSend;
		} else {
			goto Exit;
		}
	}
	nTmp = nDirection;
	switch (nDirection) {
	case 4:
		anDirection.push_back(0);
		anDirection.push_back(3);
		break;
	case 5:
		anDirection.push_back(1);
		anDirection.push_back(3);
		break;
	case 6:
		anDirection.push_back(1);
		anDirection.push_back(2);
		break;
	case 7:
		anDirection.push_back(0);
		anDirection.push_back(2);
		break;
	default:
		anDirection.push_back(nDirection);
		break;
	}
	// S3b: 押している人の向き(4方向)。パッド入力があればその向きを、無ければ
	// 移動方向(斜め含む)をGetDrawDirectionで4方向へ丸めたものを使う
	// (docs/push-object-redesign.md 4章1項。後段のnBodyDirと同じ考え方だが、
	// 押し判定で先に必要なためここで求めて使い回す)。
	{
		int nPadFacing = pMgrKeyInput ? pMgrKeyInput->GetBrowserPadFacing() : -1;
		nPushDir = (nPadFacing >= 0) ? nPadFacing : m_pPlayerChar->GetDrawDirection(nDirection);
	}
	// S5: 入れ替わり中に本人が向きを変えたら、入れ替わりを終える
	// (docs/push-object-redesign.md 2章7項。以後はサーバーが2倍速で自走させる)
	if (m_bPushSwapActive && (m_nPushSwapDirection != nPushDir)) {
		EndPushSwapOnDirectionChange(nPushDir);
	}

	if (anDirection.size() == 1) {
		/* ぶつかる？(通常キャラのブロック判定 + 押せる物なら押し予測を試みる) */
		bResult = TryMoveOrPushDirection(pMap, nDirection, nPushDir, nMovePixel, dwPushObjCharID);
		if (bResult) {
			bRet = TRUE;
			goto Exit;
		}
	} else {
		// 斜め移動(4方向へ分解)。まず斜めそのものをブロック判定し、通れるなら
		// (押せる物が斜めの角に無い限り)従来通り斜めのまま進む。
		// 斜めがブロックされた場合、以前は anDirection[0]→[1] の順に「片方の軸だけ」へ
		// 逃がしていたため、押す向き(nPushDir)側の軸で押せる物に接していても、
		// その軸を先に試して押せてしまうと直交軸の成分が失われ、逆に直交軸を先に
		// 試すと押す向きの軸そのものを試さず素通りしてしまい、斜め移動中は
		// 押せる物を押せなかった。ここでは2軸を独立に判定し、押す向きの軸では
		// 押し判定(押せればその軸へ1px押す)、直交する軸では従来通り押せる物を
		// 固い物として扱う通常のブロック判定を行い、両方の結果を合成する。
		// (docs/push-object-redesign.md 4章2項)
		BOOL bResult0, bResult1;
		DWORD dwPushObjCharID0, dwPushObjCharID1;

		bResult = TryMoveOrPushDirection(pMap, nDirection, nPushDir, nMovePixel, dwPushObjCharID);
		if (bResult) {
			dwPushObjCharID = 0;
			dwPushObjCharID0 = 0;
			dwPushObjCharID1 = 0;
			bResult0 = TryMoveOrPushDirection(pMap, anDirection[0], nPushDir, nMovePixel, dwPushObjCharID0);
			bResult1 = TryMoveOrPushDirection(pMap, anDirection[1], nPushDir, nMovePixel, dwPushObjCharID1);

			if (bResult0 && bResult1) {
				// 両軸ともブロック: 斜めはおろかどちらの軸へも進めない
				bRet = TRUE;
				goto Exit;
			} else if (bResult0) {
				// anDirection[0]側がブロック: anDirection[1]側だけへ進む(押していればそれも維持)
				nDirection = anDirection[1];
				dwPushObjCharID = dwPushObjCharID1;
			} else if (bResult1) {
				// anDirection[1]側がブロック: anDirection[0]側だけへ進む(押していればそれも維持)
				nDirection = anDirection[0];
				dwPushObjCharID = dwPushObjCharID0;
			} else {
				// 両軸とも通れる: nDirectionは斜めのまま(xx/yyは両軸分そのまま使う)。
				// 押す向きの軸で押せた物があれば、直交軸へ移動しつつその物も押す。
				dwPushObjCharID = (dwPushObjCharID0 != 0) ? dwPushObjCharID0 : dwPushObjCharID1;
			}
		}
	}
	if (nTmp != nDirection) {
		xx = anPosChangeX[nDirection] * nMovePixel;
		yy = anPosChangeY[nDirection] * nMovePixel;
		m_pPlayerChar->ChgDirection(nDirection);
	}
	nDirectionView = GetSmoothedMoveDirection(nDirection, timeGetTime());

	nState = CHARMOVESTATE_MOVE;
	if (m_pPlayerChar->IsStateBattle()) {
		nState = CHARMOVESTATE_BATTLEMOVE;
	}

	m_pPlayerChar->ChgDirection(nDirection);
	// ChgDirection は MOVE(移動中) 状態では m_nDirection を更新しない。ドット移動で
	// 連続 MOVE 状態が続く自キャラは、これだと向き(体)が最初の方向に固定されてしまう
	// （顔/目は描画向き override で更新されるため「顔だけ向く」状態になる）。
	// 連続移動中も体の向きを追従させるため、ここで自キャラの向きを直接更新する。
	// バーチャルパッドのスティック角度から決めた4方向(padFacing)があればそれを優先し、
	// 無ければ移動方向そのものを向く。移動量(xx/yy)は8方向のままなので斜め移動は維持される。
	{
		int nBodyDir = nDirection;
		if (pMgrKeyInput != NULL) {
			int nPadFacing = pMgrKeyInput->GetBrowserPadFacing();
			if (nPadFacing >= 0) {
				nBodyDir = nPadFacing;
			}
		}
		m_pPlayerChar->m_nDirection = nBodyDir;
		nDirectionView = nBodyDir;
	}
	m_pPlayerChar->SetDrawDirectionOverride(nDirectionView);
	m_pPlayerChar->SetPos(x + xx, y + yy);
	m_pPlayerChar->ChgMoveState(nState);

	// S5: 入れ替わり中は、本人が動いたd軸方向の量(打ち切りnShift)ぶんだけ
	// ボールをB0から-d方向へ動かす(docs/push-object-redesign.md 2章7項)。
	// d軸以外(横方向)はB0のまま変えない。斜め移動の横方向成分をそのまま
	// 反映すると横ずれとしてサーバーに却下される。また移動量が本人の幅を
	// 超えてもnShiftをnWidthで打ち切ることで、幅超過による却下も防ぐ
	// (親レビュー指摘。サーバーも同じ打ち切りで判定する)。
	if (m_bPushSwapActive && (dwPushObjCharID == m_dwPushSwapObjCharID)) {
		PCInfoCharCli pInfoSwapObj;

		pInfoSwapObj = (PCInfoCharCli)m_pLibInfoChar->GetPtr(m_dwPushSwapObjCharID);
		if (pInfoSwapObj != NULL) {
			static const int anOppositeDir[] = {1, 0, 3, 2};
			static const int anPosX[] = {0, 0, -1, 1};
			static const int anPosY[] = {-1, 1, 0, 0};
			int nNewBallX, nNewBallY, nWidth, nMoved, nShift, nOppositeDir;
			RECT rcSelf;

			m_pPlayerChar->GetCollisionRect(rcSelf);
			// 当たり矩形は端を含むので +1 して実際の幅にする(TryStartPushSwapと同じ)
			nWidth = (m_nPushSwapDirection <= 1) ? (rcSelf.bottom - rcSelf.top + 1) : (rcSelf.right - rcSelf.left + 1);
			nMoved = (m_nPushSwapDirection <= 1) ?
				abs(m_pPlayerChar->m_nMapY - m_ptPushSwapP0.y) : abs(m_pPlayerChar->m_nMapX - m_ptPushSwapP0.x);
			nShift = min(nMoved, nWidth);
			nOppositeDir = anOppositeDir[m_nPushSwapDirection];
			nNewBallX = m_ptPushSwapB0.x + anPosX[nOppositeDir] * nShift;
			nNewBallY = m_ptPushSwapB0.y + anPosY[nOppositeDir] * nShift;
			pInfoSwapObj->SetPos(nNewBallX, nNewBallY);

			if (nMoved >= nWidth) {
				// 本人の幅ぶん進んで入れ替わり完了。状態を先に消すと、後段の通常
				// 送信管理がPUSH種別で送ってしまい、サーバーに却下されてボールが
				// 引き戻される(親レビュー指摘)。最後のボール位置をSWAPの離した印
				// 付きで1回送ってから、EndPushPredict(FALSE)でまとめて片付ける
				// (ForceStop・m_dwPushPredictEndTime設定・押し送信/入れ替わり状態の
				// 破棄を行う)。開始と完了が同フレームになる(本人の幅が今回の移動量
				// 以下)場合、後段の通常送信管理がまだ今回のボールでm_bPushSyncActive
				// を立てていないことがあるため、EndPushPredictが必ず後始末できるよう
				// ここで先に立てておく。
				m_bPushSyncActive = TRUE;
				m_dwPushSyncObjCharID = m_dwPushSwapObjCharID;
				m_nPushSyncDirection = nPushDir;
				SendReqPush(m_dwPushSwapObjCharID, nPushDir, pInfoSwapObj, TRUE, PUSHTYPE_SWAP);
				EndPushPredict(FALSE);
				// EndPushPredictは一時停止と区別が付かないため入れ替わり状態を消さない
				// (親レビュー指摘)。ここは本当の完了なので所有権ごと明示的に手放す。
				DiscardPushSwapState(TRUE, "done");
				// 後段(3662行付近)の通常送信管理が、入れ替わり完了済みのこのボールを
				// 「まだ押している」として直後にPUSH種別で送り直さないよう、
				// 今回分は押し対象なしにしておく(このボールは既に本人の後ろにいる)
				dwPushObjCharID = 0;
			}
		}
	}

	if (nDirection <= 1) {
		/* 重なり調整 */
		m_pLibInfoChar->SortY();
	}

	/* カメラは移動後位置へ滑らかに追従させる */
	pLayerMap->SetCameraTargetCenterPos(x + xx, y + yy);

	bResult = m_pLibInfoChar->DeleteOutScreen(m_pPlayerChar);
	if (bResult) {
		m_pMgrData->PostMainFrameMessage(MAINFRAMEMSG_RENEWCHARCOUNT, m_pLibInfoChar->GetCount());
	}
	pLayerMap->SetSystemIconMode(1);
	m_dwLastTimeMove = timeGetTime();

	if (dwPushObjCharID != 0) {
		// S3b: 押し要求の送信。100ms毎・向き変更時に送る(docs/push-object-redesign.md 4章3項)。
		// 押し始めの1px目は m_bPushSyncActive がFALSEなのですぐ送る。
		PCInfoCharCli pInfoObj;

		pInfoObj = (PCInfoCharCli)m_pLibInfoChar->GetPtr(dwPushObjCharID);
		if (pInfoObj != NULL) {
			BOOL bPushChanged;
			int nPushType;

			bPushChanged = (m_bPushSyncActive == FALSE) ||
				(m_dwPushSyncObjCharID != dwPushObjCharID) ||
				(m_nPushSyncDirection != nPushDir);
			// S5: 入れ替わり中はSWAPで送る
			nPushType = (m_bPushSwapActive && (m_dwPushSwapObjCharID == dwPushObjCharID)) ? PUSHTYPE_SWAP : PUSHTYPE_PUSH;
			if (bPushChanged || (timeGetTime() - m_dwLastTimePushSyncSend >= 100)) {
				SendReqPush(dwPushObjCharID, nPushDir, pInfoObj, FALSE, nPushType);
			}
			m_bPushSyncActive = TRUE;
			m_dwPushSyncObjCharID = dwPushObjCharID;
			m_nPushSyncDirection = nPushDir;
			m_dwLastTimePushContact = timeGetTime();
			// まだ押し続けているので「離した」印はまだ送っていない扱いに戻す(S4)
			m_bPushReleaseSent = FALSE;
		}
	} else if (m_bPushSyncActive) {
		// S3b: 接触を失った瞬間の即時送信(docs/push-object-redesign.md 4章3項)。
		// 斜め移動で直前の1歩までは押せていたのに、この1歩ですり抜けて押せなかった
		// (dwPushObjCharID==0)場合、100ms間引き・300msタイムアウトを待つと、その間に
		// 自分だけ押せる物から離れていき、最後に送るREQ_PUSHの本人座標が接触していた
		// 頃からずれてサーバーにNOT_CONTACT等で却下され、押せる物が古い座標へ
		// 引き戻される(逆走)。まだ接していた最後の瞬間の座標で今すぐ送っておく。
		PCInfoCharCli pInfoObj;

		pInfoObj = (PCInfoCharCli)m_pLibInfoChar->GetPtr(m_dwPushSyncObjCharID);
		if (pInfoObj != NULL) {
			BOOL bAlreadySent;

			bAlreadySent = m_bLastPushSyncSentValid &&
				(m_nLastPushSyncSentX == pInfoObj->m_nMapX) &&
				(m_nLastPushSyncSentY == pInfoObj->m_nMapY);
			// S4: 座標が同じで送信済みでも、離した印(m_bRelease)はまだ送っていなければ
			// 1回だけ送っておく。ここが接触を失った瞬間＝押すのをやめた瞬間のため
			// (docs/push-object-redesign.md S4)。毎フレーム重複送信しないよう
			// m_bPushReleaseSent で1回に絞る。
			if (!bAlreadySent || !m_bPushReleaseSent) {
				int nPushType;

				// S5: 入れ替わり中に接触を失った(=止まった)場合もSWAPで送る
				nPushType = (m_bPushSwapActive && (m_dwPushSwapObjCharID == m_dwPushSyncObjCharID)) ? PUSHTYPE_SWAP : PUSHTYPE_PUSH;
#if PUSH_CLIENT_DEBUG_LOG
				SboDbgLog("[PushDbg][接触喪失即送信][obj:%u][pos:%d,%d][dir:%d]",
					pInfoObj->m_dwCharID, pInfoObj->m_nMapX, pInfoObj->m_nMapY, m_nPushSyncDirection);
#endif
				SendReqPush(m_dwPushSyncObjCharID, m_nPushSyncDirection, pInfoObj, TRUE, nPushType);
				m_bPushReleaseSent = TRUE;
			}
		}
	}

	x += xx;
	y += yy;
ExitSend:
	m_pPlayerChar->m_bRedraw = TRUE;
	if (!((xBack == x) && (yBack == y) && (nDirectionBack == nDirection))) {
		/* サーバへ移動通知（Dead Reckoning専用） */
		if (m_bMoveSyncActive == FALSE) {
			PacketMoveStart.Make(
				m_pPlayerChar->m_dwMapID,
				m_pPlayerChar->m_dwCharID,
				nDirection,
				x,
				y,
				FALSE,
				1,
				timeGetTime());
			m_pSock->Send(&PacketMoveStart);
			m_bMoveSyncActive = TRUE;
			m_nMoveSyncDirection = nDirection;
			m_dwLastTimeMoveSyncSend = timeGetTime();
		} else if (m_nMoveSyncDirection != nDirection) {
			PacketMoveDirChange.Make(
				m_pPlayerChar->m_dwMapID,
				m_pPlayerChar->m_dwCharID,
				nDirection,
				x,
				y,
				FALSE,
				1,
				timeGetTime());
			m_pSock->Send(&PacketMoveDirChange);
			m_nMoveSyncDirection = nDirection;
			m_dwLastTimeMoveSyncSend = timeGetTime();
		} else if (bSyncSend || (timeGetTime() - m_dwLastTimeMoveSyncSend >= 100)) {
			PacketMoveDirChange.Make(
				m_pPlayerChar->m_dwMapID,
				m_pPlayerChar->m_dwCharID,
				nDirection,
				x,
				y,
				TRUE,
				1,
				timeGetTime());
			m_pSock->Send(&PacketMoveDirChange);
			m_dwLastTimeMoveSyncSend = timeGetTime();
		}
	}

	if ((xBack != x) || (yBack != y)) {
		int nCurTileX, nCurTileY;
		BOOL bMapChanged;

		nCurTileX = x / MAPPARTSSIZE;
		nCurTileY = y / MAPPARTSSIZE;
		bMapChanged = FALSE;
		if (m_bHasLastEventTile && (m_dwLastEventMapID != m_pPlayerChar->m_dwMapID)) {
			bMapChanged = TRUE;
		}

		/*
		   移動イベント判定は毎ピクセル要求すると Web 版では同期量が過剰になり、
		   移動の滑らかさや停止位置に悪影響が出る。
		   そのため、マップ切替直後を除き、タイルを跨いだ時だけサーバーへ判定を委譲する。
		*/
		if (bMapChanged == FALSE &&
			!m_bAutoWalkToEvent &&
			((m_bHasLastEventTile == FALSE) ||
			 (m_nLastEventTileX != nCurTileX) ||
			 (m_nLastEventTileY != nCurTileY))) {
			m_pPlayerChar->m_bWaitCheckMapEvent = TRUE;
			m_bSendCheckMapEvent = FALSE;
			/* サーバーが最新位置でイベント判定できるよう現在位置を強制同期
			   （MOVE_DIR_CHANGE は100ms毎のため、その間の移動分をここで補完する） */
			if (m_bMoveSyncActive) {
				PacketMoveDirChange.Make(
					m_pPlayerChar->m_dwMapID,
					m_pPlayerChar->m_dwCharID,
					nDirection,
					x,
					y,
					TRUE,
					1,
					timeGetTime());
				m_pSock->Send(&PacketMoveDirChange);
				m_dwLastTimeMoveSyncSend = timeGetTime();
			}
		}

		m_dwLastEventMapID = m_pPlayerChar->m_dwMapID;
		m_nLastEventTileX = nCurTileX;
		m_nLastEventTileY = nCurTileY;
		m_bHasLastEventTile = TRUE;
		m_bNeedIdleMapEventCheck = TRUE;
	}

	bRet = TRUE;
Exit:
	if (bRet && pLayerMap) {
		rcTmp.left	 = pLayerMap->m_nViewX - (MAPPARTSSIZE * 2);
		rcTmp.right	 = pLayerMap->m_nViewX + (DRAW_PARTS_X * MAPPARTSSIZE) + (MAPPARTSSIZE * 2);
		rcTmp.top	 = pLayerMap->m_nViewY - (MAPPARTSSIZE * 2);
		rcTmp.bottom = pLayerMap->m_nViewY + (DRAW_PARTS_Y * MAPPARTSSIZE) + (MAPPARTSSIZE * 2);
		m_pLibInfoItem->SetArea(m_pPlayerChar->m_dwMapID, &rcTmp);
	}

	return bRet;
}



BOOL CStateProcMAP::OnWindowMsgCHAT(DWORD dwPara)
{
	int nType;
	LPCSTR pszMsg;
	PCWindowCHAT pWndChat;
	CMainFrame *pMainFrame;

	m_pPlayerChar = m_pMgrData->GetPlayerChar();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable() == FALSE) {
		goto Exit;
	}
	pWndChat = (PCWindowCHAT)m_pMgrWindow->GetWindow(WINDOWTYPE_CHAT);
	if (pWndChat == NULL) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime();

	pszMsg = pWndChat->m_strChat;
	nType  = pWndChat->GetType();

	pMainFrame = m_pMgrData->GetMainFrame();
	pMainFrame->SendChat(nType, pszMsg, &m_dwLastBalloonID);

Exit:
	return TRUE;
}



#if defined(__EMSCRIPTEN__)
/// @brief DOM(JS)→C++ チャット送信ブリッジの実体
/// @details OnWindowMsgCHAT と同等の処理をブラウザ向けに提供する。
///          プレイヤーキャラ未設定・キー入力無効時は何もしない。
///          既存のネイティブ経路の挙動は変更しない。
void CStateProcMAP::BrowserChatSubmit(const char *pszText, int nType)
{
	CMainFrame *pMainFrame;

	// 空文字列は何もしない
	if (pszText == NULL || pszText[0] == '\0') {
		return;
	}

	// nType は負値なら 0 に丸める
	if (nType < 0) {
		nType = 0;
	}

	m_pPlayerChar = m_pMgrData->GetPlayerChar();
	if (m_pPlayerChar == NULL) {
		return;
	}
	if (IsKeyInputEnable() == FALSE) {
		return;
	}

	m_dwLastKeyInput = timeGetTime();

	pMainFrame = m_pMgrData->GetMainFrame();
	if (pMainFrame == NULL) {
		return;
	}

	// DOM 入力は UTF-8 で届く。サーバ側も UTF-8 前提なのでそのまま送る
	pMainFrame->SendChat(nType, pszText, &m_dwLastBalloonID);
}
#endif // __EMSCRIPTEN__



BOOL CStateProcMAP::OnWindowMsgSYSTEMMENU(DWORD dwPara)
{
	BOOL bRet;
	PCWindowSYSTEMMENU pWnd;

	bRet = TRUE;
	pWnd = (PCWindowSYSTEMMENU)m_pMgrWindow->GetWindow(WINDOWTYPE_SYSTEMMENU);
	if (pWnd == NULL) {
		goto Exit;
	}

	switch (dwPara) {
	case 0:		// 名前と発言色の設定
		m_pMgrWindow->MakeWindowSETCOLOR();
		break;
	case 1:		// オプション
		m_pMgrWindow->MakeWindowOPTION();
		break;
	default:
		goto Exit;
	}

	bRet = FALSE;
Exit:
	return bRet;
}



BOOL CStateProcMAP::OnWindowMsgSETCOLOR(DWORD dwPara)
{
	BOOL bRet;
	PCWindowSETCOLOR pWnd;
	CPacketMSGCMD_CHGCOLOR Packet;

	bRet = TRUE;
	pWnd = (PCWindowSETCOLOR)m_pMgrWindow->GetWindow(WINDOWTYPE_SETCOLOR);
	if (pWnd == NULL) {
		goto Exit;
	}

	Packet.Make(m_pPlayerChar->m_dwCharID, dwPara);
	m_pSock->Send(&Packet);

Exit:
	return bRet;
}



BOOL CStateProcMAP::OnWindowMsgSETDRAWMODE(DWORD dwPara)
{
	BOOL bRet;
	PCWindowSETDRAWMODE pWnd;

	bRet = TRUE;
	pWnd = (PCWindowSETDRAWMODE)m_pMgrWindow->GetWindow(WINDOWTYPE_SETDRAWMODE);
	if (pWnd == NULL) {
		goto Exit;
	}

	m_pMgrData->SetDrawMode(dwPara);
	m_pMgrData->SaveIniData();

Exit:
	return bRet;
}



BOOL CStateProcMAP::OnWindowMsgITEMMENU(DWORD dwPara)
{
	BOOL bRet;
	int nPos;
	DWORD dwDragItemID;
	PCWindowITEMMENU pWnd;
	CPacketCHAR_REQ_DRAGITEM Packet;

	bRet = TRUE;
	pWnd = (PCWindowITEMMENU)m_pMgrWindow->GetWindow(WINDOWTYPE_ITEMMENU);
	if (pWnd == NULL) {
		goto Exit;
	}

	dwDragItemID = pWnd->GetDragItemID();
	if (dwDragItemID != 0) {
		Packet.Make(m_pPlayerChar->m_dwCharID, dwDragItemID, pWnd->GetDragPos());
		m_pSock->Send(&Packet);
		pWnd->DragOff();

	} else {
		if (dwPara == 0) {
			goto Exit;
		}
		nPos = pWnd->GetPos();
		m_pMgrWindow->MakeWindowITEMMENU_SELECT(nPos, dwPara);
	}

	bRet = FALSE;
Exit:
	return bRet;
}



BOOL CStateProcMAP::OnWindowMsgITEMMENU_SELECT(DWORD dwPara)
{
	BOOL bRet, bResult, bEnableMove;
	DWORD dwItemID;
	int i, nCount, nPosX[] = {0, 0, -1, 1}, nPosY[] = {-1, 1, 0, 0};
	POINT ptFrontPos;
	PCWindowITEMMENU pWndITEMMENU;
	PCWindowITEMMENU_SELECT pWndITEMMENU_SELECT;
	PCPacketBase pPacket;
	CPacketCHAR_REQ_PUTGET PacketCHAR_REQ_PUTGET;
	CPacketCHAR_REQ_EQUIP PacketCHAR_REQ_EQUIP;
	CPacketCHAR_REQ_USEITEM PacketCHAR_REQ_USEITEM;

	bRet	= FALSE;
	pPacket	= NULL;

	pWndITEMMENU = (PCWindowITEMMENU)m_pMgrWindow->GetWindow(WINDOWTYPE_ITEMMENU);
	if (pWndITEMMENU == NULL) {
		goto Exit;
	}
	pWndITEMMENU_SELECT = (PCWindowITEMMENU_SELECT)m_pMgrWindow->GetWindow(WINDOWTYPE_ITEMMENU_SELECT);
	if (pWndITEMMENU_SELECT == NULL) {
		goto Exit;
	}
	dwItemID = pWndITEMMENU->GetSelectItemID();
	if (dwItemID == 0) {
		goto Exit;
	}
	bEnableMove = m_pPlayerChar->IsEnableMove();

	switch (dwPara) {
	case ITEMMENU_SELECT_COMMAND_PUT:			// 地面に置く
		// アイテムは足元（自分の立っている有効な地面）に置くため、前方タイルの
		// 進入/脱出可否チェックは廃止する。これが残っていると、ゴミ箱や壁が前方の
		// 判定エリアに入る距離で「置く」を選んでもパケットが送られず持ったままになる。
		// 投棄（ゴミ箱）と足元設置の振り分けはサーバー側が行う。
		PacketCHAR_REQ_PUTGET.Make(m_pPlayerChar->m_dwCharID, dwItemID);
		pPacket = &PacketCHAR_REQ_PUTGET;
		break;
	case ITEMMENU_SELECT_COMMAND_EQUIP:			// 装備する
		if (bEnableMove == FALSE) {
			AddSystemMsg(FALSE, "行動中なので装備変更できません", RGB(255, 255, 255));
			break;
		}
		PacketCHAR_REQ_EQUIP.Make(m_pPlayerChar->m_dwCharID, dwItemID, -1);
		pPacket = &PacketCHAR_REQ_EQUIP;
		break;
	case ITEMMENU_SELECT_COMMAND_EQUIP_UNSET:	// 装備を外す
		if (bEnableMove == FALSE) {
			AddSystemMsg(FALSE, "行動中なので装備変更できません", RGB(255, 255, 255));
			break;
		}
		PacketCHAR_REQ_EQUIP.Make(m_pPlayerChar->m_dwCharID, dwItemID, pWndITEMMENU_SELECT->GetType());
		pPacket = &PacketCHAR_REQ_EQUIP;
		break;
	case ITEMMENU_SELECT_COMMAND_USE:			// 使う
		PacketCHAR_REQ_USEITEM.Make(m_pPlayerChar->m_dwCharID, dwItemID);
		pPacket = &PacketCHAR_REQ_USEITEM;
		break;
	default:
		goto Exit;
	}

	if (pPacket) {
		m_pSock->Send(pPacket);
	}

	bRet = TRUE;
Exit:
	return bRet;
}



BOOL CStateProcMAP::OnWindowMsgOPTION(DWORD dwPara)
{
	BOOL bRet;
	PCWindowOPTION pWnd;

	bRet = TRUE;
	pWnd = (PCWindowOPTION)m_pMgrWindow->GetWindow(WINDOWTYPE_OPTION);
	if (pWnd == NULL) {
		goto Exit;
	}

	switch (dwPara) {
	case 0:		// 表示設定
		m_pMgrWindow->MakeWindowOPTION_VIEWSET();
		break;
	case 1:		// 音量設定
		m_pMgrWindow->MakeWindowOPTION_VOLUMESET();
		break;
	case 2:		// 入力設定
		m_pMgrWindow->MakeWindowOPTION_INPUTSET();
		break;
	case 3:		// 動作設定
		m_pMgrWindow->MakeWindowOPTION_ACTIONSET();
		break;
	default:
		goto Exit;
	}

	bRet = FALSE;
Exit:
	return bRet;
}



BOOL CStateProcMAP::OnWindowMsgOPTION_VIEWSET(DWORD dwPara)
{
	BOOL bRet;
	PCWindowOPTION_VIEWSET pWnd;

	bRet = TRUE;
	pWnd = (PCWindowOPTION_VIEWSET)m_pMgrWindow->GetWindow(WINDOWTYPE_OPTION_VIEWSET);
	if (pWnd == NULL) {
		goto Exit;
	}

	switch (dwPara) {
	case 0:		// 発言時にタスクバーチカチカ
		if (m_pMgrData->GetOptionTaskbar()) {
			m_pMgrData->SetOptionTaskbar(FALSE);
			AddSystemMsg(FALSE, "発言時でも変化しません", RGB(255, 255, 255));
		} else {
			m_pMgrData->SetOptionTaskbar(TRUE);
			AddSystemMsg(FALSE, "発言時にタスクバーを点滅させます", RGB(255, 255, 255));
		}
		m_pMgrData->SaveIniData();
		break;
	case 1:		// 名前を表示する
		if (m_pMgrData->GetDrawMode() != 0) {
			m_pMgrData->SetDrawMode(0);
			AddSystemMsg(FALSE, "名前を表示しません", RGB(255, 255, 255));
		} else {
			m_pMgrData->SetDrawMode(1);
			AddSystemMsg(FALSE, "名前を表示します", RGB(255, 255, 255));
		}
		m_pMgrData->SaveIniData();
		break;
	case 2:		// 発言を表示する
		if (m_pMgrData->GetOptionViewChat() != 0) {
			m_pMgrData->SetOptionViewChat(0);
			AddSystemMsg(FALSE, "発言を表示しません", RGB(255, 255, 255));
		} else {
			m_pMgrData->SetOptionViewChat(1);
			AddSystemMsg(FALSE, "発言を表示します", RGB(255, 255, 255));
		}
		m_pMgrData->SaveIniData();
		break;
	case 3:		// アイテムを表示する
		if (m_pMgrData->GetOptionViewItem()) {
			m_pMgrData->SetOptionViewItem(FALSE);
			AddSystemMsg(FALSE, "アイテムを表示しません", RGB(255, 255, 255));
		} else {
			m_pMgrData->SetOptionViewItem(TRUE);
			AddSystemMsg(FALSE, "アイテムを表示します", RGB(255, 255, 255));
		}
		m_pMgrData->SaveIniData();
		break;
	case 4:		// アイテム名を表示する
		if (m_pMgrData->GetOptionViewItemName()) {
			m_pMgrData->SetOptionViewItemName(FALSE);
			AddSystemMsg(FALSE, "アイテム名を表示しません", RGB(255, 255, 255));
		} else {
			m_pMgrData->SetOptionViewItemName(TRUE);
			AddSystemMsg(FALSE, "アイテム名を表示します", RGB(255, 255, 255));
		}
		m_pMgrData->SaveIniData();
		break;
	case 5:		// ヘルプアイコンを表示する
		if (m_pMgrData->GetOptionViewHelpIcon() != 0) {
			m_pMgrData->SetOptionViewHelpIcon(0);
			AddSystemMsg(FALSE, "ヘルプアイコンを表示しません", RGB(255, 255, 255));
		} else {
			m_pMgrData->SetOptionViewHelpIcon(1);
			AddSystemMsg(FALSE, "ヘルプアイコンを表示します", RGB(255, 255, 255));
		}
		m_pMgrData->SaveIniData();
		break;
	case 6:		// 戦闘メッセージをログに残す
		if (m_pMgrData->GetOptionBattleMsgLog() == TRUE) {
			m_pMgrData->SetOptionBattleMsgLog(FALSE);
			AddSystemMsg(FALSE, "戦闘メッセージをログに残しません", RGB(255, 255, 255));
		} else {
			m_pMgrData->SetOptionBattleMsgLog(TRUE);
			AddSystemMsg(FALSE, "戦闘メッセージをログに残します", RGB(255, 255, 255));
		}
		m_pMgrData->SaveIniData();
		break;
	case 7:		// 60フレームで表示する
		if (m_pMgrData->GetOption60Frame() == TRUE) {
			m_pMgrData->SetOption60Frame(FALSE);
			AddSystemMsg(FALSE, "秒間30フレームで表示します", RGB(255, 255, 255));
		} else {
			m_pMgrData->SetOption60Frame(TRUE);
			AddSystemMsg(FALSE, "秒間60フレームで表示します", RGB(255, 255, 255));
		}
		m_pMgrData->SaveIniData();
		m_pMgrData->PostMainFrameMessage(MAINFRAMEMSG_RENEWVIEWSET, 0);
		break;
	default:
		goto Exit;
	}

	bRet = FALSE;
Exit:
	return bRet;
}



BOOL CStateProcMAP::OnWindowMsgOPTION_INPUTSET(DWORD dwPara)
{
	BOOL bRet;
	PCWindowOPTION_INPUTSET pWnd;
	PCMgrKeyInput pMgrKeyInput;

	bRet = TRUE;
	pWnd = (PCWindowOPTION_INPUTSET)m_pMgrWindow->GetWindow(WINDOWTYPE_OPTION_INPUTSET);
	if (pWnd == NULL) {
		goto Exit;
	}

	switch (dwPara) {
	case 0:		// 使用するジョイパッドの設定
		pMgrKeyInput = m_pMgrData->GetMgrKeyInput();
		pMgrKeyInput->Enum();
		m_pMgrWindow->MakeWindowOPTION_INPUTSET_SETDEVICE();
		break;
	case 1:		// バーチャルパッドの表示設定（自動/表示/非表示を切り替え。ウィンドウは閉じない）
		CWindowOPTION_INPUTSET::CycleVirtualPadMode();
		break;
	default:
		goto Exit;
	}

	bRet = FALSE;
Exit:
	return bRet;
}



BOOL CStateProcMAP::OnWindowMsgOPTION_TASKBAR(DWORD dwPara)
{
	BOOL bRet;
	PCWindowOPTION_TASKBAR pWnd;

	bRet = TRUE;
	pWnd = (PCWindowOPTION_TASKBAR)m_pMgrWindow->GetWindow(WINDOWTYPE_OPTION_TASKBAR);
	if (pWnd == NULL) {
		goto Exit;
	}

	switch (dwPara) {
	case 0:		// ON
		m_pMgrData->SetOptionTaskbar(TRUE);
		AddSystemMsg(FALSE, "発言時にタスクバーを点滅させます", RGB(255, 255, 255));
		m_pMgrData->SaveIniData();
		break;
	case 1:		// OFF
		m_pMgrData->SetOptionTaskbar(FALSE);
		AddSystemMsg(FALSE, "発言時でも変化しません", RGB(255, 255, 255));
		m_pMgrData->SaveIniData();
		break;
	default:
		goto Exit;
	}

Exit:
	return bRet;
}



BOOL CStateProcMAP::OnWindowMsgOPTION_INPUTSET_SETDEVICE(DWORD dwPara)
{
	BOOL bRet;
	int nNo;
	GUID stGuid;
	PCWindowOPTION_INPUTSET_SETDEVICE pWnd;
	PCMgrKeyInput pMgrKeyInput;
	CmyString strName, strTmp;

	bRet = TRUE;
	pWnd = (PCWindowOPTION_INPUTSET_SETDEVICE)m_pMgrWindow->GetWindow(WINDOWTYPE_OPTION_INPUTSET_SETDEVICE);
	if (pWnd == NULL) {
		goto Exit;
	}
	pMgrKeyInput = m_pMgrData->GetMgrKeyInput();
	nNo = (int)dwPara;
	nNo --;
	pMgrKeyInput->SetDevice(nNo);

	ZeroMemory(&stGuid, sizeof (stGuid));
	if (nNo >= 0) {
		pMgrKeyInput->GetGUID(nNo, stGuid);
		pMgrKeyInput->GetDeviceName(nNo, strName);

		strTmp.Format(_T("[%s]を使用します"), (LPCTSTR)strName);
	} else {
		strTmp.Format(_T("ジョイパッドを使用しません"));
	}
	m_pMgrData->SetInputGuid(stGuid);
	m_pMgrData->SaveIniData();
	AddSystemMsg(FALSE, (LPCSTR)strTmp, RGB(255, 255, 255));

Exit:
	return bRet;
}



BOOL CStateProcMAP::OnWindowMsgOPTION_ACTIONSET(DWORD dwPara)
{
	BOOL bRet;
	PCWindowOPTION_ACTIONSET pWnd;

	bRet = TRUE;
	pWnd = (PCWindowOPTION_ACTIONSET)m_pMgrWindow->GetWindow(WINDOWTYPE_OPTION_ACTIONSET);
	if (pWnd == NULL) {
		goto Exit;
	}

	switch (dwPara) {
	case 0:		// おひるねタイマーの設定
		m_pMgrWindow->MakeWindowOPTION_ACTIONSET_SLEEPTIMER();
		break;
	default:
		goto Exit;
	}

	bRet = FALSE;
Exit:
	return bRet;
}



BOOL CStateProcMAP::OnWindowMsgOPTION_ACTIONSET_SLEEPTIMER(DWORD dwPara)
{
	BOOL bRet;
	PCWindowOPTION_ACTIONSET_SLEEPTIMER pWnd;

	bRet = TRUE;
	pWnd = (PCWindowOPTION_ACTIONSET_SLEEPTIMER)m_pMgrWindow->GetWindow(WINDOWTYPE_OPTION_ACTIONSET_SLEEPTIMER);
	if (pWnd == NULL) {
		goto Exit;
	}

	m_pMgrData->SetSleepTimer(dwPara);
	m_pMgrData->SaveIniData();

Exit:
	return bRet;
}



BOOL CStateProcMAP::OnWindowMsgCOMMANDMENU(DWORD dwPara)
{
	int nTmp;
	BOOL bRet;
	PCWindowCOMMANDMENU pWnd;

	bRet = TRUE;
	pWnd = (PCWindowCOMMANDMENU)m_pMgrWindow->GetWindow(WINDOWTYPE_COMMANDMENU);
	if (pWnd == NULL) {
		goto Exit;
	}

	switch (dwPara) {
	case 0:	// キャラクター(C)
		m_pMgrWindow->MakeWindowCHAR_STATUS();
		break;
	case 1:	// スキル(S)
		nTmp = pWnd->GetPosSub();
		m_pMgrWindow->MakeWindowSKILLMENU(nTmp);
		break;
	case 2:	// バッグ(B)
		m_pMgrWindow->MakeWindowITEMMENU();
		break;
//	case 3:	// 招待(I)
	case 4:	// システム(ESC)
		m_pMgrWindow->MakeWindowSYSTEMMENU();
		break;
	}
	m_pMgrWindow->Delete(WINDOWTYPE_PLACEINFORMATION);
	m_pMgrWindow->Delete(WINDOWTYPE_CHAR_STATUS4);

Exit:
	return bRet;
}



BOOL CStateProcMAP::OnWindowMsgSWOON(DWORD dwPara)
{
	CPacketCHAR_PARA1 PacketCHAR_PARA1;

	switch (dwPara) {
	case 0:		// この場で助けを待つ
		break;
	case 1:		// 記録した場所で復活する
		PacketCHAR_PARA1.Make(SBOCOMMANDID_SUB_CHAR_REQ_RECOVERY, m_pPlayerChar->m_dwCharID, 0);
		m_pSock->Send(&PacketCHAR_PARA1);
		break;
	}

	return TRUE;
}



BOOL CStateProcMAP::OnWindowMsgSKILLMENU(DWORD dwPara)
{
	BOOL bRet;
	PCWindowSKILLMENU pWnd;
	CPacketCHAR_PARA1 PacketCHAR_PARA1;

	bRet = TRUE;
	pWnd = (PCWindowSKILLMENU)m_pMgrWindow->GetWindow(WINDOWTYPE_SKILLMENU);
	if (pWnd == NULL) {
		goto Exit;
	}

	if (dwPara == 0) {
		goto Exit;
	}

	PacketCHAR_PARA1.Make(SBOCOMMANDID_SUB_CHAR_REQ_USESKILL, m_pPlayerChar->m_dwCharID, dwPara);
	m_pSock->Send(&PacketCHAR_PARA1);

Exit:
	return bRet;
}



void CStateProcMAP::OnMainFrameRENEWITEMINFO(DWORD dwItemID)
{
	BOOL bResult, bDelete;
	HWND hWndAdmin;
	PCInfoItem pInfoItem;
	PCInfoCharBase pInfoChar;

	if (dwItemID == 0) {
		return;
	}
	if (m_pPlayerChar == NULL) {
		return;
	}
	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr(dwItemID);
	if (pInfoItem == NULL) {
		return;
	}
	hWndAdmin = m_pMgrData->GetAdminWindow();
	if (hWndAdmin == NULL) {
		bDelete = FALSE;
		pInfoChar = (PCInfoCharBase)m_pLibInfoChar->GetPtr(pInfoItem->m_dwCharID);
		if (pInfoChar == NULL) {
			if (pInfoItem->m_dwMapID == 0) {
				bDelete = TRUE;
			}
		}
		if (bDelete) {
			m_pLibInfoItem->Delete(dwItemID);
			return;
		}
	}
	if (pInfoItem->m_dwDropSoundID == 0) {
		return;
	}
	bResult = m_pPlayerChar->IsViewArea(pInfoItem->m_dwMapID, &pInfoItem->m_ptPos);
	if (bResult == FALSE) {
		return;
	}

	/* 近距離にいるので効果音再生 */
	m_pMgrSound->PlaySound(pInfoItem->m_dwDropSoundID);
}



void CStateProcMAP::OnMainFrameRENEWTALKEVENT(DWORD dwParam)
{
	LPCSTR pszName;
	PCInfoTalkEvent pInfo;
	PCInfoCharBase pInfoChar;

	if (dwParam != 0) {
		return;
	}
	pInfo = m_pMgrData->GetInfoTalkEvent();
	if (pInfo->GetPageCount() <= 0) {
		return;
	}
	pszName	  = NULL;
	pInfoChar = (PCInfoCharBase)m_pLibInfoChar->GetPtr(pInfo->m_dwTalkEventID);
	if (pInfoChar) {
		pszName = (LPCSTR)pInfoChar->m_strCharName;
	}

	m_pMgrWindow->MakeWindowTEXTMSG(NULL, pszName, pInfo);
}



BOOL CStateProcMAP::OnXChar(DWORD dwCharID)
{
	int nLen;
	BOOL bRet, bResult;
	PCInfoCharBase pInfoChar;
	CPacketCHAR_REQ_MODIFY_PARAM Packet;
	CmyString strTmp;

	bRet = FALSE;

	pInfoChar = (PCInfoCharBase)m_pLibInfoChar->GetPtr(dwCharID);
	if (pInfoChar == NULL) {
		goto Exit;
	}
	bResult = pInfoChar->IsNPC();
	if (bResult == FALSE) {
		goto Exit;
	}

	switch (pInfoChar->m_nMoveType) {
	case CHARMOVETYPE_SCORE:			// 得点
		Packet.Make(dwCharID, PARAMID_CHAR_REQ_MODIFY_ANIME, 0);
		m_pSock->Send(&Packet);
		break;
	case CHARMOVETYPE_STYLECOPY_PUT:	// 容姿コピー(取り込み)
		Packet.Make(dwCharID, PARAMID_CHAR_REQ_MODIFY_STYLECOPY_PUT, m_pPlayerChar->m_dwCharID);
		m_pSock->Send(&Packet);
		break;
	case CHARMOVETYPE_STYLECOPY_GET:	// 容姿コピー(反映)
		Packet.Make(m_pPlayerChar->m_dwCharID, PARAMID_CHAR_REQ_MODIFY_STYLECOPY_GET, dwCharID);
		m_pSock->Send(&Packet);
		break;
	default:
		TrimViewString(strTmp, (LPCSTR)pInfoChar->m_strTalk);
		nLen = strTmp.GetLength();
		if (nLen == 1) {
			if (strcmp((LPCSTR)strTmp, "@") == 0) {
				CPacketCHAR_PARA1 Packet;

				Packet.Make(SBOCOMMANDID_SUB_CHAR_REQ_TALKEVENT, dwCharID, 0);
				m_pSock->Send(&Packet);
				m_pPlayerChar->m_bWaitCheckMapEvent = TRUE;
				break;
			}
		}
		if (nLen > 0) {
			m_pMgrWindow->MakeWindowTEXTMSG(NULL, (LPCSTR)pInfoChar->m_strCharName, (LPCSTR)strTmp);
			break;
		}
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}



void CStateProcMAP::AddSystemMsg(
	BOOL bAddLog,		// [in] TRUE:ログに追加
	LPCSTR pszMsg,		// [in] メッセージ
	COLORREF cl)		// [in] 表示色
{
	m_pMgrData->AddSystemMsg(bAddLog, pszMsg, cl);
	m_pMgrData->PostMainFrameMessage(MAINFRAMEMSG_RENEWSYSTEMMSG, 0);
}



void CStateProcMAP::DefenseOff(void)
{
	BOOL bResult;
	CPacketCHAR_STATE Packet;

	if (m_pPlayerChar == NULL) {
		return;
	}
	if (m_pPlayerChar->m_nMoveState != CHARMOVESTATE_BATTLE_DEFENSE) {
		return;
	}
	bResult = m_pPlayerChar->IsChgWait();
	if (bResult) {
		return;
	}
	Packet.Make(m_pPlayerChar->m_dwCharID, CHARMOVESTATE_BATTLE);
	m_pSock->Send(&Packet);
	m_pPlayerChar->SetChgWait(TRUE);
}



BOOL CStateProcMAP::IsEnemyChar(PCInfoCharCli pInfoChar)
{
	/* docs/battle-redesign.md S3: 敵 = NPCで、攻撃対象の移動種別ホワイトリスト
	   (CInfoCharBase::IsAtackTarget、PC/BATTLE1/BATTLE2/ATACKANIME)に合致し、HP≧1。
	   PCはPvPマップ設定ができるまで敵扱いしない(常にIsNPC()==FALSEで除外される) */
	if (pInfoChar == NULL) {
		return FALSE;
	}
	if (pInfoChar->IsNPC() == FALSE) {
		return FALSE;
	}
	if (pInfoChar->IsAtackTarget() == FALSE) {
		return FALSE;
	}
	return TRUE;
}



DWORD CStateProcMAP::GetFrontEnemyCharID(DWORD dwCharID, int nDirection)
{
	/* docs/battle-redesign.md S3: 攻撃の届く範囲(サーバーのGetFrontCharIDTargetと同じ、
	   斜めは上下左右に分解)で正面の敵を探す */
	DWORD dwTargetCharID;
	PCInfoCharCli pInfoChar;

	dwTargetCharID = m_pLibInfoChar->GetFrontCharIDTarget(dwCharID, nDirection);
	if (dwTargetCharID == 0) {
		return 0;
	}
	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr(dwTargetCharID);
	if (IsEnemyChar(pInfoChar) == FALSE) {
		return 0;
	}
	return dwTargetCharID;
}



DWORD CStateProcMAP::GetTalkCharID(DWORD dwCharID, int nDirection)
{
	BOOL bContinue;
	DWORD dwRet;
	WORD wPartsID;
	int nPosX[] = {0, 0, -1, 1}, nPosY[] = {-1, 1, 0, 0};
	PCInfoCharCli pInfoChar;
	PCLibInfoMapParts pLibInfoMapParts;
	PCInfoMapParts pInfoMapParts;
	POINT ptFrontPos, ptFrontMapPos;

	dwRet = m_pLibInfoChar->GetFrontCharID(dwCharID);
	if (dwRet != 0) {
		/* docs/battle-redesign.md S3: 敵は会話の対象にしない(敵優先はOnX側の判定順で
		   保証済みだが、GetFrontCharIDTarget(攻撃の届く範囲)とGetFrontCharID(1マス)は
		   判定基準が違うため、ここでも念のため敵を除外する) */
		if (IsEnemyChar((PCInfoCharCli)m_pLibInfoChar->GetPtr(dwRet))) {
			return 0;
		}
		return dwRet;
	}
	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr(dwCharID);
	if (pInfoChar == NULL) {
		return dwRet;
	}
	pInfoChar->GetFrontMapPos(ptFrontMapPos);
	pInfoChar->GetFrontPos(ptFrontPos);
	pInfoChar->ChgDirection(nDirection);

	pLibInfoMapParts = m_pMgrData->GetLibInfoMapParts();

	while (1) {
		bContinue = FALSE;
		wPartsID = m_pMap->GetPartsPile(ptFrontMapPos.x, ptFrontMapPos.y);
		if (wPartsID != 0) {
			pInfoMapParts = (PCInfoMapParts)pLibInfoMapParts->GetPtr((DWORD)wPartsID);
			if (pInfoMapParts->m_dwPartsType & BIT_PARTSHIT_COUNTER) {
				bContinue = TRUE;
			}
		}
		wPartsID = m_pMap->GetParts(ptFrontMapPos.x, ptFrontMapPos.y);
		if (wPartsID != 0) {
			pInfoMapParts = (PCInfoMapParts)pLibInfoMapParts->GetPtr((DWORD)wPartsID);
			if (pInfoMapParts->m_dwPartsType & BIT_PARTSHIT_COUNTER) {
				bContinue = TRUE;
			}
		}
		if (bContinue == FALSE) {
			break;
		}
		/* カウンター越しに奥へ1マス進める。従来は代入(=)になっていて現在位置を失っていた
		   不具合を修正し、他の書き方(ptFrontPos)と同様に加算(+=)にする
		   (docs/battle-redesign.md S3。カウンター越し会話の座標不具合) */
		ptFrontMapPos.x += nPosX[nDirection];
		ptFrontMapPos.y += nPosY[nDirection];
		ptFrontPos.x += (nPosX[nDirection] * 2);
		ptFrontPos.y += (nPosY[nDirection] * 2);
	}
	dwRet = m_pLibInfoChar->GetHitCharID(dwCharID, ptFrontPos.x, ptFrontPos.y);
	if (IsEnemyChar((PCInfoCharCli)m_pLibInfoChar->GetPtr(dwRet))) {
		return 0;
	}

	return dwRet;
}


void CStateProcMAP::DrawImGui(void)
{
#if defined(__EMSCRIPTEN__)
	// ブラウザ版はメイン窓内サブウィンドウで両方描画
	if (m_pImGuiMsgLog) { m_pImGuiMsgLog->Draw(); }
	if (m_pImGuiDbg) { m_pImGuiDbg->Draw(); }
#endif
}

void CStateProcMAP::DrawImGuiSub(int kind)
{
	if (kind == 0 /* IMGUI_SUBWINDOW_DEBUG */) {
		if (m_pImGuiDbg) { m_pImGuiDbg->Draw(); }
	} else if (kind == 1 /* IMGUI_SUBWINDOW_LOG */) {
		if (m_pImGuiMsgLog) { m_pImGuiMsgLog->Draw(); }
	}
}
