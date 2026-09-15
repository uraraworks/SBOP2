/// @file InfoCharSvr.cpp
/// @brief キャラ情報サーバークラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/01/14
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "InfoMotion.h"
#include "InfoCharSvr.h"
#include "InfoMapBase.h"
#include "LibInfoMapBase.h"
#include "LibInfoCharSvr.h"
#include "../MainFrame/PushDecision.h"
#include "../MainFrame/MainFrame.h"
#include "TextOutput.h"
#include "../Platform/SvrPlatform.h"

// 診断用。MainFrameRecvProcCHAR.cppのPUSH_DEBUG_LOGと同じ値にすること
// (原因調査時のみ1にする)。モジュールが違うため定義を共有できず重複させている。
#define PUSH_DEBUG_LOG 0

namespace {

// PushDecision::DIRECTION の単位ベクトル。MainFrameRecvProcCHAR.cppのPushDirVectorと
// 同じ対応(CHARDIR_*とインデックスが一致する前提)。モジュールが違うため重複させている。
void EjectDirVector(int nDir, int &dx, int &dy)
{
	dx = 0;
	dy = 0;
	switch (nDir) {
	case PushDecision::DIR_UP:    dy = -1; break;
	case PushDecision::DIR_DOWN:  dy =  1; break;
	case PushDecision::DIR_LEFT:  dx = -1; break;
	case PushDecision::DIR_RIGHT: dx =  1; break;
	}
}

PushDecision::RECT_PX ToPushRectEject(const RECT &rc)
{
	PushDecision::RECT_PX r;
	r.nLeft   = rc.left;
	r.nTop    = rc.top;
	r.nRight  = rc.right;
	r.nBottom = rc.bottom;
	return r;
}

// 押せる物がマップ的に1px先(nDir)へ進めるか。MainFrameRecvProcCHAR.cppの
// IsPushMapFreeと同じ実装(モジュールが違うため重複させている)。
BOOL EjectIsMapFree(CLibInfoCharBase *pLibInfoChar, CInfoMapBase *pInfoMap, CInfoCharSvr *pInfoObj, int nCurX, int nCurY, int nDir)
{
	int nSaveX, nSaveY;
	BOOL bResult;

	if (pInfoMap == NULL) {
		return FALSE;
	}

	nSaveX = pInfoObj->m_nMapX;
	nSaveY = pInfoObj->m_nMapY;
	pInfoObj->m_nMapX = nCurX;
	pInfoObj->m_nMapY = nCurY;
	bResult = pLibInfoChar->CanMoveDirection(pInfoMap, pInfoObj, nDir);
	pInfoObj->m_nMapX = nSaveX;
	pInfoObj->m_nMapY = nSaveY;
	return bResult;
}

}

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
	m_dwFiredMapEventMapID = 0;
	m_dwLastRecvMoveTime	= 0;
	m_dwLastRecvMovePacketTime = 0;
	m_dwLastMoveRejectSyncTime = 0;
	m_dwLastTalkEventNPCID = 0;
	m_dwLastTalkEventTime = 0;
	m_dwLastPushedTime = 0;
	m_dwPushingCharID = 0;
	m_dwLastPushClientTime = 0;
	m_dwSwapOwnerSessionID = 0;
	m_dwLastSwapReqTime = 0;
	m_dwEjectOwnerCharID = 0;
	m_dwLastEjectProcTime = 0;
	m_dwLastPushRejectSyncTime = 0;
	m_dwLastPushRejectLogTime = 0;
	m_dwLastPushAcceptLogTime = 0;
	m_dwLastPushDiagLogTime = 0;
	m_dwLastPushDecideLogTime = 0;
	m_nFiredMapEventCount = 0;
	m_nLastMoveSyncDirection = -1;
	m_nPushRejectSuppressedCount = 0;
	m_bMoveSyncActive = FALSE;
	m_bPendingMapEvent = FALSE;
	m_bSwapActive = FALSE;
	m_bEjectActive = FALSE;
	m_nPendingEventTileX = 0;
	m_nPendingEventTileY = 0;
	m_nSwapDir = 0;
	m_nEjectDir = 0;
	m_ptSwapP0.x = m_ptSwapP0.y = 0;
	m_ptSwapB0.x = m_ptSwapB0.y = 0;
	m_dEjectPxRemainder = 0.0;

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
		m_dwLastTimeChg = SboPlatform::GetTickMs();
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
	m_dwFiredMapEventMapID = pSrc->m_dwFiredMapEventMapID;
	m_nFiredMapEventCount = pSrc->m_nFiredMapEventCount;
	{
		int k;
		for (k = 0; k < MAPEVENT_FIRED_MAX; k ++) {
			m_nFiredMapEventTileX[k] = pSrc->m_nFiredMapEventTileX[k];
			m_nFiredMapEventTileY[k] = pSrc->m_nFiredMapEventTileY[k];
		}
	}
	m_nLastMoveSyncDirection = pSrc->m_nLastMoveSyncDirection;
	m_bMoveSyncActive = pSrc->m_bMoveSyncActive;
	m_dwLastRecvMoveTime	= pSrc->m_dwLastRecvMoveTime;
	m_dwLastRecvMovePacketTime = pSrc->m_dwLastRecvMovePacketTime;
	m_dwLastMoveRejectSyncTime = pSrc->m_dwLastMoveRejectSyncTime;
}

void CInfoCharSvr::Copy(CInfoCharBase *pSrc)
{
	PCInfoCharSvr pSrcTmp;

	pSrcTmp = (PCInfoCharSvr)pSrc;
	CInfoCharBase::Copy(pSrc);

	m_dwMoveCount	= pSrcTmp->m_dwMoveCount;
	m_dwFiredMapEventMapID = pSrcTmp->m_dwFiredMapEventMapID;
	m_nFiredMapEventCount = pSrcTmp->m_nFiredMapEventCount;
	{
		int k;
		for (k = 0; k < MAPEVENT_FIRED_MAX; k ++) {
			m_nFiredMapEventTileX[k] = pSrcTmp->m_nFiredMapEventTileX[k];
			m_nFiredMapEventTileY[k] = pSrcTmp->m_nFiredMapEventTileY[k];
		}
	}
}

void CInfoCharSvr::ClearFiredMapEvent()
{
	m_dwFiredMapEventMapID = 0;
	m_nFiredMapEventCount = 0;
}

void CInfoCharSvr::AddFiredMapEvent(DWORD dwMapID, int nTileX, int nTileY)
{
	int k;

	// 別マップの集合になっていたら作り直す
	if (m_dwFiredMapEventMapID != dwMapID) {
		m_dwFiredMapEventMapID = dwMapID;
		m_nFiredMapEventCount = 0;
	}
	// 既に登録済みなら何もしない
	for (k = 0; k < m_nFiredMapEventCount; k ++) {
		if ((m_nFiredMapEventTileX[k] == nTileX) && (m_nFiredMapEventTileY[k] == nTileY)) {
			return;
		}
	}
	// 満杯なら黙って無視（同時に重なるイベントは通常数個で上限に達しない）
	if (m_nFiredMapEventCount < MAPEVENT_FIRED_MAX) {
		m_nFiredMapEventTileX[m_nFiredMapEventCount] = nTileX;
		m_nFiredMapEventTileY[m_nFiredMapEventCount] = nTileY;
		m_nFiredMapEventCount ++;
	}
}

BOOL CInfoCharSvr::IsFiredMapEventInRect(DWORD dwMapID, int nLeft, int nTop, int nRight, int nBottom)
{
	int k;

	if (m_dwFiredMapEventMapID != dwMapID) {
		return FALSE;
	}
	for (k = 0; k < m_nFiredMapEventCount; k ++) {
		if ((nLeft <= m_nFiredMapEventTileX[k]) && (m_nFiredMapEventTileX[k] <= nRight) &&
			(nTop <= m_nFiredMapEventTileY[k]) && (m_nFiredMapEventTileY[k] <= nBottom)) {
			return TRUE;
		}
	}
	return FALSE;
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

	bRet = FALSE;

	// S3: 押せる物(m_bPush)の停止判定。押しはRecvProcCHAR_REQ_PUSHが1pxずつ
	// SetPos()するだけで、歩数(m_nMoveCount)方式は使わない(docs/push-object-redesign.md)。
	// 最後に押されてから PUSH_STOP_TIMEOUT_MS 経っても次の押しが来なければ、
	// MOVE状態のまま止まっているクライアントへ MOVE_STOP を送らせるため停止させる。
	if (m_bPush && (m_nMoveType != CHARMOVETYPE_PUTNPC) && IsStateMove()) {
		const DWORD PUSH_STOP_TIMEOUT_MS = 150;
		if (dwTime - m_dwLastPushedTime >= PUSH_STOP_TIMEOUT_MS) {
			int nState = CHARMOVESTATE_STAND;
			if (IsStateBattle()) {
				nState = CHARMOVESTATE_BATTLE;
			}
			SetMoveState(nState);
			m_dwPushingCharID = 0;
		}
	}

	// S5: 入れ替わり(SWAP)専有者からの要求がPUSH_SWAP_OWNER_TIMEOUT_MS以上
	// 途絶えたら、入れ替わりを打ち切る(eject化はしない。docs/push-object-redesign.md 2章7項)。
	if (m_bSwapActive) {
		const DWORD PUSH_SWAP_OWNER_TIMEOUT_MS = 2000;
		if (dwTime - m_dwLastSwapReqTime >= PUSH_SWAP_OWNER_TIMEOUT_MS) {
			m_bSwapActive = FALSE;
		}
	}

	// S5: 自走(eject)を1周期ぶん進める。
	if (m_bEjectActive) {
		ProcEjectMove(dwTime);
	}

	return bRet;
}

// S5: 自走(eject)を1周期ぶん進める。入れ替わり(SWAP)が向き変更で終了した後、
// 専有者と重なったままのボールを専有者の2倍速で転がし、重ならなくなった所で
// 止める(docs/push-object-redesign.md 2章7項)。配信が多くなりすぎないよう
// 50ms未満はまとめて進める。
void CInfoCharSvr::ProcEjectMove(DWORD dwTime)
{
	const DWORD EJECT_BATCH_MS = 50;

	if (m_pLibInfoCharSvr == NULL) {
		m_bEjectActive = FALSE;
		return;
	}

	PCInfoCharSvr pOwner = (PCInfoCharSvr)m_pLibInfoCharSvr->GetPtrLogIn(m_dwEjectOwnerCharID);
	if (pOwner == NULL) {
		// 専有者ログアウトで自走を終了する。
		m_bEjectActive = FALSE;
#if PUSH_DEBUG_LOG
		// 診断用。原因特定後に無効化。
		if ((m_pLibInfoCharSvr->m_pMainFrame != NULL) && (m_pLibInfoCharSvr->m_pMainFrame->GetLog() != NULL)) {
			m_pLibInfoCharSvr->m_pMainFrame->GetLog()->Write("EJECT終了 why:owner mv:0 obj:%d,%d", m_nMapX, m_nMapY);
		}
#endif
		return;
	}

	if (m_dwLastEjectProcTime == 0) {
		// 初回呼び出し。ここでは進めず、次回からの経過時間計算の基準だけ作る。
		m_dwLastEjectProcTime = dwTime;
		return;
	}

	DWORD dwElapsed = dwTime - m_dwLastEjectProcTime;
	if (dwElapsed < EJECT_BATCH_MS) {
		return;
	}

	int nOwnerPixelsPerSec = m_pLibInfoCharSvr->GetCharMovePixelsPerSec(pOwner);
	double dDistance = (static_cast<double>(dwElapsed) / 1000.0) * (nOwnerPixelsPerSec * 2) + m_dEjectPxRemainder;
	int nMaxDistance = static_cast<int>(dDistance);
	m_dEjectPxRemainder = dDistance - nMaxDistance;
	m_dwLastEjectProcTime = dwTime;

	if (nMaxDistance <= 0) {
		return;
	}

	RECT rcObjWin, rcOwnerWin;
	GetCollisionRect(rcObjWin);
	pOwner->GetCollisionRect(rcOwnerWin);
	PushDecision::RECT_PX rcObjStart = ToPushRectEject(rcObjWin);
	PushDecision::RECT_PX rcOwner = ToPushRectEject(rcOwnerWin);
	// 専有者の矩形を四方に EJECT_CLEAR_MARGIN_PX 広げてから「重なりが解けたか」を見る。
	// サーバーが知る専有者の位置は、クライアントより通信の遅れ分(100ms毎の送信＋通信時間で
	// 10〜20px)後ろにある。重なりが解けた瞬間(すき間数px)で止めると、歩き続けている本人が
	// すぐボールに重なり、重なった相手は押す判定から外れるため「めり込んだまま押せない」。
	const int EJECT_CLEAR_MARGIN_PX = 24;
	rcOwner.nLeft   -= EJECT_CLEAR_MARGIN_PX;
	rcOwner.nTop    -= EJECT_CLEAR_MARGIN_PX;
	rcOwner.nRight  += EJECT_CLEAR_MARGIN_PX;
	rcOwner.nBottom += EJECT_CLEAR_MARGIN_PX;

	CInfoMapBase *pMap = (m_pLibInfoCharSvr->m_pLibInfoMap != NULL)
		? (CInfoMapBase *)m_pLibInfoCharSvr->m_pLibInfoMap->GetPtr(m_dwMapID)
		: NULL;

	int nCurX = m_nMapX;
	int nCurY = m_nMapY;
	CLibInfoCharSvr *pLibInfoChar = m_pLibInfoCharSvr;
	CInfoCharSvr *pThis = this;
	int nDir = m_nEjectDir;
	PushDecision::IsPositionFreeFunc isFree = [&nCurX, &nCurY, pLibInfoChar, pMap, pThis, pOwner, nDir](const PushDecision::RECT_PX &rcMoveTo) -> bool {
		if (!EjectIsMapFree(pLibInfoChar, pMap, pThis, nCurX, nCurY, nDir)) {
			return false;
		}
		RECT rcTmp;
		SetRect(&rcTmp, rcMoveTo.nLeft, rcMoveTo.nTop, rcMoveTo.nRight, rcMoveTo.nBottom);
		// 専有者と自分自身を除く全キャラとの当たりを見る(専有者と重なっている
		// 間は「塞がっている」扱いにしない。それが自走終了の条件のため)。
		if (!pLibInfoChar->IsPushAreaFree(pOwner, pThis, pThis->m_dwMapID, rcTmp)) {
			return false;
		}
		int dx, dy;
		EjectDirVector(nDir, dx, dy);
		nCurX += dx;
		nCurY += dy;
		return true;
	};

	bool bSeparated = false;
	int nMoved = PushDecision::ResolveEjectDistance(rcObjStart, nDir, nMaxDistance, rcOwner, isFree, bSeparated);

	if (nMoved > 0) {
		int dx, dy;
		EjectDirVector(nDir, dx, dy);
		int nNewX = m_nMapX + dx * nMoved;
		int nNewY = m_nMapY + dy * nMoved;

		SetPos(nNewX, nNewY);
		SetDirection(nDir);
		{
			int nMoveStateOnMove = IsStateBattle() ? CHARMOVESTATE_BATTLEMOVE : CHARMOVESTATE_MOVE;
			if (m_nMoveState != nMoveStateOnMove) {
				SetMoveState(nMoveStateOnMove);
			}
		}
		m_bChgPos = TRUE;
		m_dwLastPushedTime = dwTime;
	}

	if (bSeparated) {
		m_bEjectActive = FALSE;
#if PUSH_DEBUG_LOG
		// 診断用。原因特定後に無効化。
		if ((m_pLibInfoCharSvr->m_pMainFrame != NULL) && (m_pLibInfoCharSvr->m_pMainFrame->GetLog() != NULL)) {
			m_pLibInfoCharSvr->m_pMainFrame->GetLog()->Write("EJECT終了 why:sep mv:%d obj:%d,%d", nMoved, m_nMapX, m_nMapY);
		}
#endif
	} else if (nMoved < nMaxDistance) {
		// 重なりが解ける前に塞がれた: ボールはそこで止める(docs/push-object-redesign.md
		// 2章7項)。自走を続けたままだと、自走中は押し要求を全て却下するため
		// 専有者がログアウトするまで誰も押せなくなる。
		m_bEjectActive = FALSE;
#if PUSH_DEBUG_LOG
		// 診断用。原因特定後に無効化。
		if ((m_pLibInfoCharSvr->m_pMainFrame != NULL) && (m_pLibInfoCharSvr->m_pMainFrame->GetLog() != NULL)) {
			m_pLibInfoCharSvr->m_pMainFrame->GetLog()->Write("EJECT終了 why:blk mv:%d obj:%d,%d", nMoved, m_nMapX, m_nMapY);
		}
#endif
	}
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
	pInfo->dwProcSetTime	= SboPlatform::GetTickMs();	// 処理設定時間
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
