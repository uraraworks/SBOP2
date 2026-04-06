/// @file StateProcMAP.cpp
/// @brief 状態処理クラス(マップ画面) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/10/01
/// @copyright Copyright(C)URARA-works 2006

#include "stdafx.h"
#include "LibInfoMapBase.h"
#include "LibInfoItem.h"
#include "LibInfoMapParts.h"
#include "ParamUtil.h"
#include "InfoMapBase.h"
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
#include "DlgMsgLog.h"
#include "DlgDbg.h"
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

namespace {

static BOOL IsLeftMousePressed(void)
{
	return (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LMASK) ? TRUE : FALSE;
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
	m_dwLastTimeMove		= 0;
	m_dwLastTimeKeepAlive	= 0;
	m_dwLastBalloonID		= 0;
	m_dwLastKeyInput		= 0;
	m_dwLastTimeGauge		= 0;
	m_dwLastTimeMoveSyncSend = 0;
	m_dwStartChargeTime		= 0;
	m_bMoveSyncActive			= FALSE;
	m_nMoveSyncDirection		= -1;
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
	m_pDlgMsgLog		= NULL;
	m_pDlgDbg			= NULL;
}



CStateProcMAP::~CStateProcMAP()
{
	DestroyAdminUi();
	m_pMgrSound->StopBGM();
}



void CStateProcMAP::Create(CMgrData *pMgrData, CUraraSockTCPSBO *pSock)
{
	CStateProcBase::Create(pMgrData, pSock);

	m_pLibInfoChar		= pMgrData->GetLibInfoChar();
	m_pLibInfoMap		= pMgrData->GetLibInfoMap();
	m_pLibInfoItem		= pMgrData->GetLibInfoItem();

#if defined(_WIN32)
	m_pDlgMsgLog = new CDlgMsgLog;
	m_pDlgMsgLog->Create(pMgrData->GetMainWindow(), m_pMgrData);
	m_pDlgDbg = new CDlgDbg;
	m_pDlgDbg->Create(pMgrData->GetMainWindow(), m_pMgrData);
#endif
}



void CStateProcMAP::Init(void)
{
	TCHAR szFileName[MAX_PATH];
	CRect rc;

	GetModuleIniPath(szFileName, _countof(szFileName));

#if defined(_WIN32)
	rc.left		= GetPrivateProfileInt(_T("Pos"), _T("LogLeft"),	-1, szFileName);
	rc.top		= GetPrivateProfileInt(_T("Pos"), _T("LogTop"),	-1, szFileName);
	rc.right	= GetPrivateProfileInt(_T("Pos"), _T("LogRight"),	-1, szFileName);
	rc.bottom	= GetPrivateProfileInt(_T("Pos"), _T("LogBottom"),	-1, szFileName);
	if (!((rc.left == -1) && (rc.top == -1))) {
		m_pDlgMsgLog->SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	}
#endif

	m_pPlayerChar	= m_pMgrData->GetPlayerChar();
	m_pMap			= m_pMgrData->GetMap();
	m_dwLastEventMapID = 0;
	m_bHasLastEventTile = FALSE;
	m_bAutoWalkToEvent = FALSE;
	m_bNeedIdleMapEventCheck = FALSE;

	m_pMgrLayer->MakeMAP();
	m_pMgrLayer->MakeSYSTEMMSG();

	CreateAdminUi();

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
#if defined(_WIN32)
	if (m_pDlgMsgLog->IsWindowVisible()) {
		m_pDlgMsgLog->GetWindowRect(&rcDst);
	}
#else
	UNREFERENCED_PARAMETER(rcDst);
#endif
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

	nMovePixelsPerSec = GetPlayerMovePixelsPerSec(m_pPlayerChar);
	ullAccumulated = (ULONGLONG)nAccumOut + (ULONGLONG)dwElapsed * nMovePixelsPerSec;
	nMoveStep = (int)(ullAccumulated / 1000);
	nAccumOut = (int)(ullAccumulated % 1000);
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
}


void CStateProcMAP::StartAutoWalkToEvent(int nTileX, int nTileY)
{
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
			CPacketCHAR_MOVE_STOP PacketMoveStop;
			CPacketCHAR_PARA1 PacketPara1;

			/* 判定要求の直前に現在位置を強制同期して、サーバー側の取りこぼしを防ぐ */
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

			/* Phase 8: 自由移動では停止タイミング依存にすると取りこぼすため、待機フラグ時に即チェック要求する */
			PacketPara1.Make(SBOCOMMANDID_SUB_CHAR_REQ_CHECKMAPEVENT, m_pPlayerChar->m_dwCharID, 0);
			m_pSock->Send(&PacketPara1);
			m_bSendCheckMapEvent = TRUE;
		}
	}

	/* おひるねタイマー処理 */
	TimerProcSleepTimer();
	/* 溜め攻撃処理 */
	TimerProcChargeAtack();
	/* ゲージ回復 */
	TimerProcGauge();

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
#ifdef _WIN32
				OutputDebugString(strDbg);
#else
				SDL_Log("%s", (LPCSTR)CStringA(strDbg));
#endif
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
			strTmp.Format(_T("%s：%s"), (LPCTSTR)pInfoChar->m_strCharName, (LPCTSTR)pInfoChar->m_strSpeak);
			if (m_pDlgMsgLog) { m_pDlgMsgLog->Add(strTmp, pInfoChar->m_clSpeak); }
		}
		break;

	case MAINFRAMEMSG_RENEWCHARCOUNT:	// キャラ数更新
		m_pMgrData->SetCharCount(dwParam);
		if (m_pDlgDbg) { m_pDlgDbg->Renew(); }
		break;

	case MAINFRAMEMSG_RENEWONLINECOUNT:	// オンライン数更新
		if (m_pDlgDbg) { m_pDlgDbg->Renew(); }
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
					if (m_pDlgMsgLog) { m_pDlgMsgLog->Add(pSystemMsg->strMsg, pSystemMsg->clMsg); }
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
		{
			int nTmp;

			if (m_pPlayerChar == NULL) {
				break;
			}
			m_dwLastTimeGauge = timeGetTime();

			nTmp = 3;
			if (m_pPlayerChar->m_wAtackGauge < 3) {
				nTmp = m_pPlayerChar->m_wAtackGauge;
			}
			m_pPlayerChar->m_wAtackGauge -= nTmp;
			nTmp = 3;
			if (m_pPlayerChar->m_wDefenseGauge < 3) {
				nTmp = m_pPlayerChar->m_wDefenseGauge;
			}
			m_pPlayerChar->m_wDefenseGauge -= nTmp;
		}
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



void CStateProcMAP::TimerProcChargeAtack(void)
{
	BOOL bResult, bCancel;
	DWORD dwTime;
	CMainFrame *pMainFrame;
	PCMgrKeyInput pMgrKeyInput;
	CPacketCHAR_PARA1 Packet;

	bCancel = FALSE;
	pMgrKeyInput = m_pMgrData->GetMgrKeyInput();
	pMainFrame	 = m_pMgrData->GetMainFrame();

	if (m_pPlayerChar == NULL) {
		bCancel = TRUE;
		goto Exit;
	}
	if (m_dwStartChargeTime == 0) {
		return;
	}
	if (m_pMgrWindow->IsKeyInput()) {
		bCancel = TRUE;
		goto Exit;
	}
	if (m_pPlayerChar->m_nMoveState == CHARMOVESTATE_SWOON) {
		bCancel = TRUE;
		goto Exit;
	}
	if (m_pPlayerChar->m_nMoveState == CHARMOVESTATE_BATTLE_DEFENSE) {
		bCancel = TRUE;
		goto Exit;
	}
	bResult = pMgrKeyInput->IsInput('X');
	if (bResult == FALSE) {
		bCancel = TRUE;
		goto Exit;
	}
	if (m_pPlayerChar->m_bChargeAtack) {
		return;
	}

	dwTime = timeGetTime() - m_dwStartChargeTime;
	if (dwTime < 2000) {
		return;
	}

	/* 溜め状態ONを通知 */
	Packet.Make(SBOCOMMANDID_SUB_CHAR_STATE_CHARGE, m_pPlayerChar->m_dwCharID, 1);
	m_pSock->Send(&Packet);
	m_pPlayerChar->m_bChargeAtack = TRUE;
	pMainFrame->ChgMoveState(FALSE);

Exit:
	if (bCancel) {
		if (m_pPlayerChar) {
			m_pPlayerChar->m_bChargeAtack = FALSE;
			/* 溜め状態OFFを通知 */
			Packet.Make(SBOCOMMANDID_SUB_CHAR_STATE_CHARGE, m_pPlayerChar->m_dwCharID, 0);
			m_pSock->Send(&Packet);
		}
		m_dwStartChargeTime = 0;
		pMainFrame->ChgMoveState(FALSE);
	}
}



void CStateProcMAP::TimerProcGauge(void)
{
	BOOL bResult;
	int nTmp;
	float fAverage;
	DWORD dwTime;

	if (m_pPlayerChar == NULL) {
		return;
	}
	if (m_dwLastTimeGauge == 0) {
		return;
	}

	bResult = m_pPlayerChar->IsStateBattle();
	if (bResult == FALSE) {
		return;
	}

	dwTime = timeGetTime() - m_dwLastTimeGauge;
	if (dwTime < 750) {
		return;
	}
	dwTime -= 750;
	fAverage = (float)dwTime * 100.0f / 5000.0f;

	/* アタックゲージの処理 */
	{
		nTmp = (int)(MAX_ATACKGAUGE * fAverage * 0.02f);
		if (nTmp > 0) {
			m_pPlayerChar->m_wAtackGauge += (WORD)nTmp;
			if (m_pPlayerChar->m_wAtackGauge >= MAX_ATACKGAUGE) {
				m_pPlayerChar->m_wAtackGauge = MAX_ATACKGAUGE;
			}
		}
	}

	/* ガードゲージの処理 */
	{
		nTmp = (int)(MAX_DEFENSEGAUGE * fAverage * 0.02f);
		if (nTmp > 0) {
			m_pPlayerChar->m_wDefenseGauge += (WORD)nTmp;
			if (m_pPlayerChar->m_wDefenseGauge >= MAX_DEFENSEGAUGE) {
				m_pPlayerChar->m_wDefenseGauge = MAX_DEFENSEGAUGE;
			}
		}
	}

	if ((m_pPlayerChar->m_wAtackGauge >= MAX_ATACKGAUGE) &&
		(m_pPlayerChar->m_wDefenseGauge >= MAX_DEFENSEGAUGE)) {
		m_dwLastTimeGauge = 0;

	} else {
		/* アタックゲージとガードゲージの最大が同じ間は同じ処理で。 */
		if (nTmp > 0) {
			m_dwLastTimeGauge = timeGetTime() - 750;
		}
	}
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
	int i, nCount, anDirection[] = {0, 1, 2, 3, 0, 1, 1, 0};
	BOOL bRet, bResult;
	DWORD dwFrontCharID;
	POINT ptPos, ptFrontPos;
	PCInfoItem pInfoItem;
	CMainFrame *pMainFrame;
	CPacketCHAR_STATE PacketCHAR_STATE;
	CPacketCHAR_REQ_PUTGET PacketCHAR_REQ_PUTGET;
	CPacketCHAR_MOVE_STOP PacketCHAR_MOVE_STOP;
	CPacketCHAR_REQ_TAIL PacketCHAR_REQ_TAIL;
	CPacketCHAR_PARA1 PacketCHAR_PARA1;
	std::vector<POINT> aptPos;

	bRet = FALSE;

	pMainFrame = m_pMgrData->GetMainFrame();
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
	if (bDown) {
		if ((m_dwStartChargeTime == 0) && (m_pPlayerChar->m_bChargeAtack == FALSE)) {
			if (m_pPlayerChar->m_nMoveState == CHARMOVESTATE_BATTLE) {
//Todo:暫定
				if (m_pPlayerChar->m_wAtackGauge > 10) {
					m_pPlayerChar->m_wAtackGauge -= 10;
					m_dwLastTimeGauge = timeGetTime();

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

					m_pPlayerChar->SetChgWait(TRUE);
					PacketCHAR_STATE.Make(m_pPlayerChar->m_dwCharID, CHARMOVESTATE_BATTLEATACK);
					m_pSock->Send(&PacketCHAR_STATE);
					/* 溜め開始 */
					m_dwStartChargeTime = timeGetTime();
				}
			}
		}
		goto Exit;
	}
	if (m_dwStartChargeTime) {
		if (m_pPlayerChar->m_bChargeAtack) {
			/* 溜め攻撃 */
			m_pPlayerChar->SetChgWait(TRUE);
			PacketCHAR_STATE.Make(m_pPlayerChar->m_dwCharID, CHARMOVESTATE_BATTLEATACK);
			m_pSock->Send(&PacketCHAR_STATE);
			pMainFrame->ChgMoveState(FALSE);
		}
		m_dwStartChargeTime = 0;
		goto Exit;
	}

	bResult = m_pPlayerChar->IsEnableMove();
	if (bResult == FALSE) {
		if ((m_pPlayerChar->m_dwFrontCharID) || (m_pPlayerChar->m_dwTailCharID)) {
			PacketCHAR_REQ_TAIL.Make(m_pPlayerChar->m_dwCharID, 0, FALSE);
			m_pSock->Send(&PacketCHAR_REQ_TAIL);
		}
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime();

	switch (m_pPlayerChar->m_nMoveState) {
	case CHARMOVESTATE_STAND:			// 立ち
		dwFrontCharID = GetTalkCharID(m_pPlayerChar->m_dwCharID, m_pPlayerChar->m_nDirection);
		bResult = OnXChar(dwFrontCharID);
		if (bResult) {
			break;
		}

		m_pPlayerChar->GetFrontPos(ptFrontPos);
		m_pPlayerChar->RenewBlockMapArea(ptFrontPos.x, ptFrontPos.y, m_pPlayerChar->m_nDirection);
		nCount = m_pPlayerChar->m_aposBockMapArea.size();
		for (i = 0; i < nCount; i ++) {
			/* 進入可能かチェック */
			bResult |= !m_pMap->IsMove(m_pPlayerChar->m_aposBockMapArea[i].x, m_pPlayerChar->m_aposBockMapArea[i].y, m_pPlayerChar->m_nDirection);
		}
		bResult = !bResult;
		if (bResult) {
			m_pPlayerChar->GetFrontPos(aptPos);
			nCount = aptPos.size();
			for (i = 0; i < nCount; i ++) {
				ptPos = aptPos[i];
				pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr(m_pPlayerChar->m_dwMapID, &ptPos, FALSE);
				if (pInfoItem) {
					break;
				}
			}

			if (i < nCount) {
				/* アイテム拾う要求 */
				PacketCHAR_REQ_PUTGET.Make(m_pPlayerChar->m_dwCharID, 0);
				m_pSock->Send(&PacketCHAR_REQ_PUTGET);
				break;
			}
		}
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
	BOOL bRet, bResult, bStateBattle;
	DWORD dwCharID;
	CMainFrame *pMainFrame;
	CPacketCHAR_REQ_TAIL PacketCHAR_REQ_TAIL;
	CPacketCHAR_STATE Packet;
	CPacketCHAR_PARA1 PacketCHAR_PARA1;

	bRet = FALSE;

	pMainFrame	  = m_pMgrData->GetMainFrame();
	m_pPlayerChar = m_pMgrData->GetPlayerChar();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable() == FALSE) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime();
	bStateBattle = m_pPlayerChar->IsStateBattle();
	if (bDown) {
		if (bStateBattle == FALSE) {
			goto Exit;
		}
		bResult = m_pPlayerChar->IsChgWait();
		if (bResult) {
			goto Exit;
		}
		if (m_pPlayerChar->m_nMoveState != CHARMOVESTATE_BATTLE) {
			goto Exit;
		}
		Packet.Make(m_pPlayerChar->m_dwCharID, CHARMOVESTATE_BATTLE_DEFENSE);
		m_pSock->Send(&Packet);
		m_pPlayerChar->SetChgWait(TRUE);
		bRet = TRUE;

		m_pPlayerChar->m_bChargeAtack = FALSE;
		/* 溜め状態OFFを通知 */
		PacketCHAR_PARA1.Make(SBOCOMMANDID_SUB_CHAR_STATE_CHARGE, m_pPlayerChar->m_dwCharID, 0);
		m_pSock->Send(&PacketCHAR_PARA1);
		m_dwStartChargeTime = 0;
		pMainFrame->ChgMoveState(FALSE);
		goto Exit;
	}

	if (bStateBattle == FALSE) {
		dwCharID = m_pLibInfoChar->GetFrontCharID(m_pPlayerChar->m_dwCharID, m_pPlayerChar->m_nDirection);
		PacketCHAR_REQ_TAIL.Make(m_pPlayerChar->m_dwCharID, dwCharID, TRUE);
		m_pSock->Send(&PacketCHAR_REQ_TAIL);
	} else {
		/* 防御解除 */
		DefenseOff();
	}

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
	BOOL bRet, bResult;
	CMainFrame *pMainFrame;
	CPacketCHAR_STATE Packet;
	CPacketCHAR_PARA1 PacketCHAR_PARA1;

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
	bResult = m_pPlayerChar->IsEnableMove();
	if (bResult == FALSE) {
		goto Exit;
	}
	/* 座り中？ */
	if (m_pPlayerChar->m_nMoveState == CHARMOVESTATE_SIT) {
		goto Exit;
	}

	/* 誰かに付いて行っている？ */
	if (m_pPlayerChar->m_dwFrontCharID) {
		AddSystemMsg(FALSE, "付いて行っている時はモード変更できません", RGB(255, 255, 255));
		goto Exit;
	}

	switch (m_pPlayerChar->m_nMoveState) {
	case CHARMOVESTATE_STAND:			// 立ち
		bResult = m_pMap->IsEnableBattle();
		if (bResult == FALSE) {
			AddSystemMsg(FALSE, "このマップでは戦闘できません", RGB(255, 255, 255));
			goto Exit;
		}
		m_pPlayerChar->SetMoveState(CHARMOVESTATE_BATTLE);
		m_dwLastTimeGauge = timeGetTime();
		break;
	case CHARMOVESTATE_BATTLE:			// 戦闘中
		m_pPlayerChar->SetMoveState(CHARMOVESTATE_STAND);

		if (m_dwStartChargeTime || m_pPlayerChar->m_bChargeAtack) {
			m_dwStartChargeTime = 0;
			m_pPlayerChar->m_bChargeAtack = FALSE;
			/* 溜め状態OFFを通知 */
			PacketCHAR_PARA1.Make(SBOCOMMANDID_SUB_CHAR_STATE_CHARGE, m_pPlayerChar->m_dwCharID, 0);
			m_pSock->Send(&PacketCHAR_PARA1);
		}
		break;
	}
	pMainFrame = m_pMgrData->GetMainFrame();
	pMainFrame->ChgMoveState(TRUE);

	Packet.Make(m_pPlayerChar->m_dwCharID, m_pPlayerChar->m_nMoveState);
	m_pSock->Send(&Packet);
	m_pPlayerChar->SetChgWait(TRUE);

	bRet = TRUE;
Exit:
	return bRet;
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
	BOOL bRet, bResult;
	DWORD dwCharID;
	RECT rcTmp;
	PCInfoMapBase pMap;
	PCLayerMap pLayerMap;
	PCMgrKeyInput pMgrKeyInput;
	CPacketCHAR_MOVE_START PacketMoveStart;
	CPacketCHAR_MOVE_DIR_CHANGE PacketMoveDirChange;
	CPacketCHAR_REQ_PUSH PacketREQ_PUSH;
	CPacketCHAR_STATE PacketSTATE;
	ARRAYINT anDirection;

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
	/* 1歩前に押せるキャラがいる？ */
	dwCharID = m_pLibInfoChar->GetFrontCharIDPush(m_pPlayerChar->m_dwCharID, nDirection);
	if (dwCharID) {
		PacketREQ_PUSH.Make(dwCharID, nDirection);
		m_pSock->Send(&PacketREQ_PUSH);
		m_pPlayerChar->m_dwMoveWaitOnce = BATTLEMOVEWAIT;
		pLayerMap->m_dwMoveWaitOnce = BATTLEMOVEWAIT;
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
	if (anDirection.size() == 1) {
		/* ぶつかる？ */
		bResult = m_pLibInfoChar->IsBlockChar(m_pPlayerChar, nDirection, TRUE, TRUE);
		if (bResult) {
			bRet = TRUE;
			goto Exit;
		}
	} else {
		bResult = m_pLibInfoChar->IsBlockChar(m_pPlayerChar, nDirection, TRUE, TRUE);
		if (bResult) {
			bResult = m_pLibInfoChar->IsBlockChar(m_pPlayerChar, anDirection[0], TRUE, TRUE);
			if (bResult == FALSE) {
				nDirection = anDirection[0];
			} else {
				bResult = m_pLibInfoChar->IsBlockChar(m_pPlayerChar, anDirection[1], TRUE, TRUE);
				if (bResult == FALSE) {
					nDirection = anDirection[1];
				} else {
					bRet = TRUE;
					goto Exit;
				}
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
	m_pPlayerChar->SetDrawDirectionOverride(nDirectionView);
	m_pPlayerChar->SetPos(x + xx, y + yy);
	m_pPlayerChar->ChgMoveState(nState);

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

	x += xx;
	y += yy;
ExitSend:
	m_pPlayerChar->m_bRedraw = TRUE;
	if (bSyncSend == FALSE) {
		bRet = TRUE;
		goto Exit;
	}
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
		   キャラ座標・範囲イベントはタイル内でも発火しうるため、
		   自キャラが実際に移動したフレームではサーバーへ判定を委譲する。
		   マップ切替直後だけは再発火防止のため、このフレームでは送らない。
		*/
		if (bMapChanged == FALSE && !m_bAutoWalkToEvent) {
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
		m_dwLastTimeGauge = timeGetTime();
	}

	bRet = TRUE;
Exit:
	if (bRet && pLayerMap) {
		rcTmp.left	 = pLayerMap->m_nViewX - (MAPPARTSSIZE * 2);
		rcTmp.right	 = pLayerMap->m_nViewX + (DRAW_PARTS_X * MAPPARTSSIZE) + (MAPPARTSSIZE * 2);
		rcTmp.top	 = pLayerMap->m_nViewY - (MAPPARTSSIZE * 2);
		rcTmp.bottom = pLayerMap->m_nViewY + (DRAW_PARTS_Y * MAPPARTSSIZE) + (MAPPARTSSIZE * 2);
		m_pLibInfoItem->SetArea(m_pPlayerChar->m_dwMapID, &rcTmp);
		if (m_pDlgDbg) { m_pDlgDbg->Renew(); }
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
		bResult = FALSE;
		m_pPlayerChar->GetFrontPos(ptFrontPos);
		m_pPlayerChar->RenewBlockMapArea(ptFrontPos.x, ptFrontPos.y, m_pPlayerChar->m_nDirection);
		nCount = m_pPlayerChar->m_aposBockMapArea.size();
		for (i = 0; i < nCount; i ++) {
			/* 脱出可能かチェック */
			bResult |= !m_pMap->IsMoveOut(m_pPlayerChar->m_aposBockMapArea[i].x, m_pPlayerChar->m_aposBockMapArea[i].y, m_pPlayerChar->m_nDirection);
		}
		bResult = !bResult;
		if (bResult) {
			bResult = FALSE;
			m_pPlayerChar->RenewBlockMapArea(ptFrontPos.x, ptFrontPos.y, m_pPlayerChar->m_nDirection);
			nCount = m_pPlayerChar->m_aposBockMapArea.size();
			for (i = 0; i < nCount; i ++) {
				/* 進入可能かチェック */
				bResult |= !m_pMap->IsMove(m_pPlayerChar->m_aposBockMapArea[i].x, m_pPlayerChar->m_aposBockMapArea[i].y, m_pPlayerChar->m_nDirection);
			}
			bResult = !bResult;
		}
		if (bResult == FALSE) {
			break;
		}
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
		m_dwLastTimeGauge = timeGetTime();
		break;
	case ITEMMENU_SELECT_COMMAND_EQUIP_UNSET:	// 装備を外す
		if (bEnableMove == FALSE) {
			AddSystemMsg(FALSE, "行動中なので装備変更できません", RGB(255, 255, 255));
			break;
		}
		PacketCHAR_REQ_EQUIP.Make(m_pPlayerChar->m_dwCharID, dwItemID, pWndITEMMENU_SELECT->GetType());
		pPacket = &PacketCHAR_REQ_EQUIP;
		m_dwLastTimeGauge = timeGetTime();
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
	CPacketCHAR_REQ_PUSH PacketREQ_PUSH;
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
	case CHARMOVETYPE_BALL:				// ボール
		PacketREQ_PUSH.Make(dwCharID, m_pPlayerChar->m_nDirection, 2);
		m_pSock->Send(&PacketREQ_PUSH);
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
		ptFrontMapPos.x = nPosX[nDirection];
		ptFrontMapPos.y = nPosY[nDirection];
		ptFrontPos.x += (nPosX[nDirection] * 2);
		ptFrontPos.y += (nPosY[nDirection] * 2);
	}
	dwRet = m_pLibInfoChar->GetHitCharID(dwCharID, ptFrontPos.x, ptFrontPos.y);

	return dwRet;
}

