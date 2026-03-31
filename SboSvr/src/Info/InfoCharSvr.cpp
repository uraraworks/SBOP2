/// @file InfoCharSvr.cpp
/// @brief キャラ情報サーバークラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/01/14
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "InfoMotion.h"
#include "InfoCharSvr.h"

namespace {

static const int SERVER_CHAR_MOVE_PIXELS_PER_SEC = 128;

static DWORD GetMoveWaitBase(CInfoCharSvr *pInfoChar)
{
	DWORD dwMoveWait;

	if (pInfoChar == NULL) {
		return 11;
	}
	dwMoveWait = pInfoChar->GetMoveWait();
	if (dwMoveWait == 0) {
		return 11;
	}
	return dwMoveWait;
}

static int GetMovePixelsPerSec(CInfoCharSvr *pInfoChar)
{
	DWORD dwMoveWait;
	ULONGLONG ullSpeed;

	dwMoveWait = GetMoveWaitBase(pInfoChar);
	ullSpeed = (ULONGLONG)SERVER_CHAR_MOVE_PIXELS_PER_SEC * 11;
	ullSpeed = (ullSpeed + dwMoveWait - 1) / dwMoveWait;
	if (ullSpeed == 0) {
		return 1;
	}
	if (ullSpeed > INT_MAX) {
		return INT_MAX;
	}
	return (int)ullSpeed;
}

static DWORD GetHalfTileMoveInterval(CInfoCharSvr *pInfoChar)
{
	int nMovePixelsPerSec;

	nMovePixelsPerSec = GetMovePixelsPerSec(pInfoChar);
	return max((DWORD)(((ULONGLONG)HALF_TILE * 1000 + nMovePixelsPerSec - 1) / nMovePixelsPerSec), (DWORD)1);
}

}	// namespace

CInfoCharSvr::CInfoCharSvr()
{
	m_nReserveChgEfect	= 0;
	m_nReserveChgMoveState	= 0;
	m_nMoveCount	= 0;
	m_bChgPos	= FALSE;
	m_bChgMap	= FALSE;
	m_bChgUpdatePos	= FALSE;
	m_bChgSpeak	= FALSE;
	m_bChgInfo	= FALSE;
	m_bChgMoveState	= FALSE;
	m_bChgProcState	= FALSE;
	m_bChgScreenPos	= FALSE;
	m_bChgGrp	= FALSE;
	m_bChgEfcBalloon	= FALSE;
	m_bChgMotion	= FALSE;
	m_bChgStatus	= FALSE;
	m_bChgFishingHit	= FALSE;
	m_bChgMoveCount	= FALSE;
	m_bChgPutNpc	= FALSE;
	m_bChgTargetChar	= FALSE;
	m_bWaitCheckMapEvent	= FALSE;
	m_bDropItem	= FALSE;
	m_bAtack	= FALSE;
	m_bRenewTargetPos	= FALSE;
	m_bProcMoveMapIn	= FALSE;
	m_bProcMoveMapOut	= FALSE;
	m_bProcMoveMarkPos	= FALSE;
	m_bProcSwoon	= FALSE;
	m_bProcInvincible	= FALSE;
	m_bStateFadeInOut	= FALSE;
	m_bStatusInvincible	= FALSE;
	m_pInfoMap	= NULL;
	m_dwChgWait	= 0;
	m_dwLastTimeChg	= 0;
	m_dwEfcBalloonID	= 0;
	m_dwMotionID	= 0;
	m_dwMoveCount	= 0;
	m_dwLastMoveSyncSendTime = 0;
	m_dwSuppressMapEventMapID = 0;
	m_dwLastRecvMoveTime	= 0;
	m_dwLastRecvMovePacketTime = 0;
	m_nSuppressMapEventTileX = 0;
	m_nSuppressMapEventTileY = 0;
	m_bSuppressMapEventUntilLeave = FALSE;
	m_nLastMoveSyncDirection = -1;
	m_bMoveSyncActive = FALSE;
	m_bPendingMapEvent = FALSE;
	m_nPendingEventTileX = 0;
	m_nPendingEventTileY = 0;

	m_pLibInfoCharSvr	= NULL;
}

CInfoCharSvr::~CInfoCharSvr()
{
	DeleteAllProcInfo();
}

void CInfoCharSvr::SetSpeak(LPCSTR pszSpeak)
{
	CInfoCharBase::SetSpeak(pszSpeak);

	if (m_strSpeak.IsEmpty() == FALSE) {
		m_bChgSpeak = TRUE;
	}
}

void CInfoCharSvr::SetMoveState(int nMoveState)
{
	if (m_nMoveState == CHARMOVESTATE_DELETE) {
		// 削除は取り消せないようにする
		return;
	}
	if (nMoveState == m_nMoveState) {
		return;
	}

	m_bChgMoveState = TRUE;
	switch (nMoveState) {
	case CHARMOVESTATE_BATTLEATACK_WAIT:	// 戦闘攻撃後の待ち時間
		m_dwChgWait = 1000;
		m_dwLastTimeChg = timeGetTime();
		m_bChgMoveState = FALSE;
		break;
	case CHARMOVESTATE_BATTLE:
		// 溜め攻撃解除
		m_bChargeAtack = FALSE;
		break;
	case CHARMOVESTATE_SWOON:	// 気絶
		// 溜め攻撃解除
		m_bChargeAtack = FALSE;
		// 行動情報を全て削除
		DeleteAllProcInfo();
		SetMotion(-1);
		SetProcState(CHARPROCSTATEID_NORMAL);
		break;
	}

	CInfoCharBase::SetMoveState(nMoveState);
}

void CInfoCharSvr::SetProcState(int nProcState)
{
	m_bChgProcState = TRUE;

	CInfoCharBase::SetProcState(nProcState);
}

void CInfoCharSvr::SetMap(CInfoMapBase *pInfoMap)
{
	m_pInfoMap = pInfoMap;
}

void CInfoCharSvr::SetLibInfoChar(CLibInfoCharSvr *pLibInfoChar)
{
	m_pLibInfoCharSvr = pLibInfoChar;
}

void CInfoCharSvr::SetEfcBalloon(DWORD dwEfcBalloonID)
{
	m_dwEfcBalloonID = dwEfcBalloonID;
	m_bChgEfcBalloon = TRUE;
}

void CInfoCharSvr::SetMotion(DWORD dwMotionID)
{
	m_dwMotionID = dwMotionID;
	m_bChgMotion = TRUE;
}

BOOL CInfoCharSvr::IsEnableBattle(void)
{
	BOOL bRet;

	bRet = FALSE;

	// ついていっている？
	if (m_dwFrontCharID) {
		goto Exit;
	}
	// 気絶中？
	if (m_nMoveState == CHARMOVESTATE_SWOON) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}

BOOL CInfoCharSvr::IsEnableMove(void)
{
	BOOL bRet;

	bRet = CInfoCharBase::IsEnableMove();
	if (bRet == FALSE) {
		goto Exit;
	}
	if (m_bStateFadeInOut) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}

void CInfoCharSvr::CopyAll(CInfoCharSvr *pSrc)
{
	CInfoCharBase::Copy(pSrc);

	m_abyMark = pSrc->m_abyMark;
	m_bNPC	= pSrc->m_bNPC;
	m_dwSessionID	= pSrc->m_dwSessionID;
	m_dwAccountID	= pSrc->m_dwAccountID;
	m_dwLastTimeSpeak	= pSrc->m_dwLastTimeSpeak;
	m_dwTailCharID	= pSrc->m_dwTailCharID;
	m_dwFrontCharID	= pSrc->m_dwFrontCharID;
	m_nReserveChgEfect	= pSrc->m_nReserveChgEfect;
	m_bChgPos	= pSrc->m_bChgPos;
	m_bChgUpdatePos	= pSrc->m_bChgUpdatePos;
	m_bChgSpeak	= pSrc->m_bChgSpeak;
	m_bChgInfo	= pSrc->m_bChgInfo;
	m_bChgMoveState	= pSrc->m_bChgMoveState;
	m_bChgScreenPos	= pSrc->m_bChgScreenPos;
	m_bChgPosRenew	= pSrc->m_bChgPosRenew;
	m_dwLastMoveSyncSendTime = pSrc->m_dwLastMoveSyncSendTime;
	m_dwSuppressMapEventMapID = pSrc->m_dwSuppressMapEventMapID;
	m_nSuppressMapEventTileX = pSrc->m_nSuppressMapEventTileX;
	m_nSuppressMapEventTileY = pSrc->m_nSuppressMapEventTileY;
	m_bSuppressMapEventUntilLeave = pSrc->m_bSuppressMapEventUntilLeave;
	m_nLastMoveSyncDirection = pSrc->m_nLastMoveSyncDirection;
	m_bMoveSyncActive = pSrc->m_bMoveSyncActive;
	m_dwLastRecvMoveTime	= pSrc->m_dwLastRecvMoveTime;
	m_dwLastRecvMovePacketTime = pSrc->m_dwLastRecvMovePacketTime;
}

void CInfoCharSvr::Copy(CInfoCharBase *pSrc)
{
	PCInfoCharSvr pSrcTmp;

	pSrcTmp = (PCInfoCharSvr)pSrc;
	CInfoCharBase::Copy(pSrc);

	m_dwMoveCount	= pSrcTmp->m_dwMoveCount;
	m_dwSuppressMapEventMapID = pSrcTmp->m_dwSuppressMapEventMapID;
	m_nSuppressMapEventTileX = pSrcTmp->m_nSuppressMapEventTileX;
	m_nSuppressMapEventTileY = pSrcTmp->m_nSuppressMapEventTileY;
	m_bSuppressMapEventUntilLeave = pSrcTmp->m_bSuppressMapEventUntilLeave;
}

void CInfoCharSvr::ProcAtack(void)
{
	// 溜め攻撃解除
	m_bChargeAtack = FALSE;
}

BOOL CInfoCharSvr::ProcHit(CInfoCharSvr *pInfoChar)
{
	return TRUE;
}

BOOL CInfoCharSvr::TimerProc(DWORD dwTime)
{
	BOOL bRet;

	bRet = CInfoCharBase::TimerProc(dwTime);

	if (m_nMoveState == CHARMOVESTATE_BATTLEATACK_WAIT) {
		if (m_dwChgWait) {
			if (dwTime - m_dwLastTimeChg > m_dwChgWait) {
				m_dwChgWait = 0;
				m_dwLastTimeChg = 0;
				SetMoveState(CHARMOVESTATE_BATTLEATACK);
			}
		}
	}
	bRet |= TimerProcMOVE(dwTime);

	Proc(dwTime);

	if (m_dwLightTime != 0) {
		if (dwTime > m_dwLightTime) {
			m_nLightLevel = 0;
			m_dwLightTime = 0;
			m_bChgStatus  = TRUE;
		}
	}

	return bRet;
}

BOOL CInfoCharSvr::TimerProcMOVE(DWORD dwTime)
{
	BOOL bRet;
	DWORD dwTimeTmp;
	DWORD dwMoveInterval;

	bRet = FALSE;
	if (m_nMoveCount == 0) {
		goto Exit;
	}

	dwMoveInterval = GetHalfTileMoveInterval(this);
	dwTimeTmp = dwTime - m_dwLastTimeMove;
	if (dwTimeTmp < dwMoveInterval) {
		goto Exit;
	}
	m_bChgMoveCount = TRUE;
	m_nMoveCount --;
	m_dwLastTimeMove = dwTime;

	bRet = TRUE;
Exit:
	return bRet;
}

BOOL CInfoCharSvr::IsAtackTarget(void)
{
	BOOL bRet;

	bRet = CInfoCharBase::IsAtackTarget();
	if (bRet == FALSE) {
		goto Exit;
	}
	if (m_bStatusInvincible) {
		goto Exit;
	}
	switch (m_nMoveType) {
	case CHARMOVETYPE_STAND:	// 移動しない
	case CHARMOVETYPE_BALL:	// ボール
	case CHARMOVETYPE_SCORE:	// 得点
	case CHARMOVETYPE_PUTNPC:	// NPC発生
		bRet = FALSE;
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}

DWORD CInfoCharSvr::GetHitEffectID(void)
{
	return 0;
}

DWORD CInfoCharSvr::GetDamage(void)
{
	return 0;
}

void CInfoCharSvr::Proc(DWORD dwTime)
{
	int i, nCount;
	BOOL bResult;
	DWORD dwTimeTmp;
	PCHARPROCINFO pInfo;

	nCount = m_apProcInfo.size();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_apProcInfo[i];
		dwTimeTmp = dwTime - pInfo->dwProcSetTime;
		if (dwTimeTmp < pInfo->dwProcStartTime) {
			continue;
		}

		switch (pInfo->dwProcID) {
		case CHARPROCID_FISHING:	// 釣り
			bResult = ProcFISHING(pInfo->dwPara);
			break;
		case CHARPROCID_FISHING_HIT:	// 釣り(ヒット)
			bResult = ProcFISHING_HIT(pInfo->dwPara);
			break;
		case CHARPROCID_MAPMOVEIN:	// マップ内移動
			bResult = ProcMAPMOVEIN(pInfo->dwPara);
			break;
		case CHARPROCID_MAPMOVEOUT:	// マップ外移動
			bResult = ProcMAPMOVEOUT(pInfo->dwPara);
			break;
		case CHARPROCID_SWOON:	// 気絶
			bResult = ProcSWOON(pInfo->dwPara);
			break;
		case CHARPROCID_INVINCIBLE:	// 無敵
			bResult = ProcINVINCIBLE(pInfo->dwPara);
			break;
		}
		if (bResult) {
			DeleteProcInfo(i);
			break;
		}
	}
}

BOOL CInfoCharSvr::ProcFISHING(DWORD dwPara)
{
	int nState;

	nState = CHARMOVESTATE_STAND;
	if (IsStateBattle()) {
		nState = CHARMOVESTATE_BATTLE;
	}

	// 釣れなかった
	if ((genrand() % 100) < 60) {
//Todo:
		SetEfcBalloon(8);
		SetProcState(CHARPROCSTATEID_NORMAL);
		SetMoveState(nState);
	} else {
		SetMotion(CHARMOTIONLISTID_FISHING_HIT_UP);
		AddProcInfo(CHARPROCID_FISHING_HIT, 5000, 0);
	}

	return TRUE;
}

BOOL CInfoCharSvr::ProcFISHING_HIT(DWORD dwPara)
{
	int nState;

	nState = CHARMOVESTATE_STAND;
	if (IsStateBattle()) {
		nState = CHARMOVESTATE_BATTLE;
	}

	// 釣れなかった
	if ((genrand() % 100) < 60) {
//Todo:
		SetEfcBalloon(8);

	} else {
		SetEfcBalloon(2);
		m_bChgFishingHit = TRUE;
	}
	SetMotion(-1);
	SetMoveState(nState);
	SetProcState(CHARPROCSTATEID_NORMAL);
	DeleteProcInfo(CHARPROCID_FISHING);

	return TRUE;
}

BOOL CInfoCharSvr::ProcMAPMOVEIN(DWORD dwPara)
{
	m_bProcMoveMapIn = TRUE;
	return TRUE;
}

BOOL CInfoCharSvr::ProcMAPMOVEOUT(DWORD dwPara)
{
	m_bProcMoveMapOut = TRUE;
	return TRUE;
}

BOOL CInfoCharSvr::ProcSWOON(DWORD dwPara)
{
	m_bProcSwoon = TRUE;
	m_nReserveChgEfect = -1;

	// 記録位置へ戻る？
	if (dwPara != 0) {
		m_bProcMoveMarkPos = TRUE;
	}

	return TRUE;
}

BOOL CInfoCharSvr::ProcINVINCIBLE(DWORD dwPara)
{
	m_bProcInvincible = TRUE;
	// 無敵解除
	m_bStatusInvincible = FALSE;
	return TRUE;
}

void CInfoCharSvr::DeleteProcInfo(int nNo)
{
	PCHARPROCINFO pInfo;

	if ((nNo < 0) || (nNo >= static_cast<int>(m_apProcInfo.size()))) {
		return;
	}

	pInfo = m_apProcInfo[nNo];
	SAFE_DELETE(pInfo);
	m_apProcInfo.erase(m_apProcInfo.begin() + nNo);
}

void CInfoCharSvr::DeleteProcInfo(DWORD dwProcID)
{
	int i, nCount;
	PCHARPROCINFO pInfo;

	nCount = m_apProcInfo.size();
	for (i = nCount - 1; i >= 0; i --) {
		pInfo = m_apProcInfo[i];
		if (pInfo->dwProcID != dwProcID) {
			continue;
		}
		DeleteProcInfo(i);
	}
}

void CInfoCharSvr::DeleteAllProcInfo(void)
{
	int i, nCount;

	nCount = m_apProcInfo.size();
	for (i = nCount - 1; i >= 0; i --) {
		DeleteProcInfo(i);
	}
}

void CInfoCharSvr::AddProcInfo(
	DWORD dwProcID,	// [in] 処理ID
	DWORD dwStartTime,	// [in] 開始時間
	DWORD dwPara)	// [in] パラメータ
{
	PCHARPROCINFO pInfo;

	pInfo = new CHARPROCINFO;
	pInfo->dwProcID	= dwProcID;	// 行動ID
	pInfo->dwProcSetTime	= timeGetTime();	// 処理設定時間
	pInfo->dwProcStartTime	= dwStartTime;	// 処理開始時間
	pInfo->dwPara	= dwPara;	// パラメータ

	m_apProcInfo.push_back(pInfo);
}

void CInfoCharSvr::IncPutCount(void)
{
}

void CInfoCharSvr::DecPutCount(void)
{
}
