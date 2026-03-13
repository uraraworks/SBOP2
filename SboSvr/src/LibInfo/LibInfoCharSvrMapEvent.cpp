/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:LibInfoCharSvrMapEvent.cpp									 */
/* 内容			:キャラ情報ライブラリクラス 実装ファイル					 */
/* 				:マップイベント関連処理										 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/11/16													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Packet.h"
#include "Command.h"
#include "LibInfoMapBase.h"
#include "InfoMapEvent.h"
#include "InfoCharSvr.h"
#include "MainFrame.h"
#include "MgrData.h"
#include "LibInfoCharSvr.h"

static int MapTileToPixelX(int x)
{
	return x * MAPPARTSSIZE;
}

static int MapTileToPixelY(int y)
{
	return y * MAPPARTSSIZE;
}

static void GetMapEventTileRect(RECT &rcDst, int x, int y)
{
	SetRect (
		&rcDst,
		MapTileToPixelX (x),
		MapTileToPixelY (y),
		MapTileToPixelX (x) + MAPPARTSSIZE - 1,
		MapTileToPixelY (y) + MAPPARTSSIZE - 1);
}

static void GetMapEventCharRect(RECT &rcDst, PCInfoCharSvr pInfoChar)
{
	/* 移動衝突判定と同じ矩形を使用（ドット単位） */
	pInfoChar->GetCollisionRect (rcDst);
}

static void GetMapEventSearchTileRect(RECT &rcDst, const RECT &rcChar, BOOL bCheck)
{
	RECT rcSearch;

	rcSearch = rcChar;
	if (bCheck) {
		/* 辺接触による自動移動開始を拾えるよう、探索範囲だけ 1px 広げる */
		rcSearch.left -= 1;
		rcSearch.top -= 1;
		rcSearch.right += 1;
		rcSearch.bottom += 1;
	}

	rcDst.left = rcSearch.left / MAPPARTSSIZE;
	rcDst.right = rcSearch.right / MAPPARTSSIZE;
	rcDst.top = rcSearch.top / MAPPARTSSIZE;
	rcDst.bottom = rcSearch.bottom / MAPPARTSSIZE;
}

static BOOL IsMapEventRectOverlapEnough(const RECT &rcChar, const RECT &rcEvent, int nMinOverlap = 8)
{
	int nOverlapX, nOverlapY;

	nOverlapX = min (rcChar.right, rcEvent.right) - max (rcChar.left, rcEvent.left) + 1;
	nOverlapY = min (rcChar.bottom, rcEvent.bottom) - max (rcChar.top, rcEvent.top) + 1;
	if ((nOverlapX <= 0) || (nOverlapY <= 0)) {
		return FALSE;
	}
	if (nOverlapX < nMinOverlap) {
		return FALSE;
	}
	if (nOverlapY < nMinOverlap) {
		return FALSE;
	}
	return TRUE;
}

static BOOL IsMapEventRectTouchOrOverlap(const RECT &rcChar, const RECT &rcEvent)
{
	int nOverlapX, nOverlapY;

	nOverlapX = min (rcChar.right, rcEvent.right) - max (rcChar.left, rcEvent.left) + 1;
	nOverlapY = min (rcChar.bottom, rcEvent.bottom) - max (rcChar.top, rcEvent.top) + 1;
	if (nOverlapX < 0) {
		return FALSE;
	}
	if (nOverlapY < 0) {
		return FALSE;
	}
	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::CheckMapEvent									 */
/* 内容		:マップイベントチェック											 */
/* 日付		:2008/06/28														 */
/* 戻り値	:TRUE:応答を返す	チェックのみの場合は TRUE:マップイベントあり */
/* ========================================================================= */

BOOL CLibInfoCharSvr::CheckMapEvent(
	CInfoCharSvr *pInfoChar,	/* [in] 対象キャラ */
	BOOL bCheck/*FALSE*/,		/* [in] TRUE:チェックのみ */
	int *pnTileX/*NULL*/,		/* [out] イベントタイルX（省略可） */
	int *pnTileY/*NULL*/,		/* [out] イベントタイルY（省略可） */
	BOOL bIgnoreDirection/*FALSE*/)	/* [in] TRUE:向き条件を無視 */
{
	BOOL bRet, bResult, bSuppressCurrentTile;
	int i, nEventCount, nEventLeft, nEventTop, nEventRight, nEventBottom,
		nCharCenterX, nCharCenterY, nEventCenterX, nEventCenterY, nDistSq, nBestDistSq;
	RECT rcChar, rcMap, rcMapEvent, rcSuppressEvent;
	POINT *pptPos1, *pptPos2;
	PCInfoMapBase pInfoMap;
	PCInfoMapEventBase pInfoMapEventBase, pInfoMapEventBest;
       CPacketMAP_PARA1 PacketMAP_PARA1;
       CPacketCHAR_STATE PacketCHAR_STATE;
        ARRAYINFOCHARSVR apInfoChar;

	bRet = TRUE;
	pInfoMapEventBase = NULL;
	pInfoMapEventBest = NULL;
	bSuppressCurrentTile = FALSE;

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (pInfoChar->m_dwMapID);
	if (pInfoMap == NULL) {
		goto Exit;
	}
	pInfoMapEventBase = NULL;
	nBestDistSq = INT_MAX;
	/* イベント判定用の足元矩形を取得 */
	GetMapEventCharRect (rcChar, pInfoChar);
	GetMapEventSearchTileRect (rcMap, rcChar, bCheck);
	nCharCenterX = (rcChar.left + rcChar.right) / 2;
	nCharCenterY = (rcChar.top + rcChar.bottom) / 2;

	if (pInfoChar->m_bSuppressMapEventUntilLeave) {
		if (pInfoChar->m_dwSuppressMapEventMapID == pInfoChar->m_dwMapID) {
			GetMapEventTileRect (
				rcSuppressEvent,
				pInfoChar->m_nSuppressMapEventTileX,
				pInfoChar->m_nSuppressMapEventTileY);
			if (IsMapEventRectOverlapEnough (rcChar, rcSuppressEvent, 1)) {
				bSuppressCurrentTile = TRUE;
			}
		}

		if (!bSuppressCurrentTile) {
			pInfoChar->m_bSuppressMapEventUntilLeave = FALSE;
			pInfoChar->m_dwSuppressMapEventMapID = 0;
		}
	}

	nEventCount = pInfoMap->GetEventCount ();
	for (i = 0; i < nEventCount; i ++) {
		pInfoMapEventBase = pInfoMap->GetEvent (i);
		if (pInfoMapEventBase == NULL) {
			continue;
		}

		pptPos1 = &pInfoMapEventBase->m_ptPos;
		pptPos2 = &pInfoMapEventBase->m_ptPos2;
		nEventLeft = pptPos1->x;
		nEventTop = pptPos1->y;
		nEventRight = pptPos1->x;
		nEventBottom = pptPos1->y;
		if (pInfoMapEventBase->m_nHitType == MAPEVENTHITTYPE_AREA) {
			nEventRight = pptPos2->x;
			nEventBottom = pptPos2->y;
		}
		if ((nEventRight < rcMap.left) || (rcMap.right < nEventLeft) ||
			(nEventBottom < rcMap.top) || (rcMap.bottom < nEventTop)) {
			continue;
		}

		bResult = TRUE;
		switch (pInfoMapEventBase->m_nHitType) {
		case MAPEVENTHITTYPE_MAPPOS:		/* マップ座標縦横いずれか */
			GetMapEventTileRect (rcMapEvent, pptPos1->x, pptPos1->y);
			bResult = bCheck
				? IsMapEventRectTouchOrOverlap (rcChar, rcMapEvent)
				: IsMapEventRectOverlapEnough (rcChar, rcMapEvent, 8);
			break;
		case MAPEVENTHITTYPE_CHARPOS:		/* キャラ座標 */
			GetMapEventTileRect (rcMapEvent, pptPos1->x, pptPos1->y);
			bResult = bCheck
				? IsMapEventRectTouchOrOverlap (rcChar, rcMapEvent)
				: IsMapEventRectOverlapEnough (rcChar, rcMapEvent, 8);
			break;
		case MAPEVENTHITTYPE_AREA:			/* 範囲 */
			SetRect (&rcMapEvent,
				MapTileToPixelX (pptPos1->x),
				MapTileToPixelY (pptPos1->y),
				MapTileToPixelX (pptPos2->x) + MAPPARTSSIZE - 1,
				MapTileToPixelY (pptPos2->y) + MAPPARTSSIZE - 1);
			bResult = bCheck
				? IsMapEventRectTouchOrOverlap (rcChar, rcMapEvent)
				: IsMapEventRectOverlapEnough (rcChar, rcMapEvent, 8);
			break;
		case MAPEVENTHITTYPE_MAPPOS2:		/* マップ座標完全一致 */
			GetMapEventTileRect (rcMapEvent, pptPos1->x, pptPos1->y);
			bResult = bCheck
				? IsMapEventRectTouchOrOverlap (rcChar, rcMapEvent)
				: IsMapEventRectOverlapEnough (rcChar, rcMapEvent, 8);
			break;
		}
		if (bResult && bSuppressCurrentTile) {
			if ((nEventLeft <= pInfoChar->m_nSuppressMapEventTileX) && (pInfoChar->m_nSuppressMapEventTileX <= nEventRight) &&
				(nEventTop <= pInfoChar->m_nSuppressMapEventTileY) && (pInfoChar->m_nSuppressMapEventTileY <= nEventBottom)) {
				bResult = FALSE;
			}
		}
		if (bResult) {
			nEventCenterX = (rcMapEvent.left + rcMapEvent.right) / 2;
			nEventCenterY = (rcMapEvent.top + rcMapEvent.bottom) / 2;
			nDistSq = (nEventCenterX - nCharCenterX) * (nEventCenterX - nCharCenterX) +
					  (nEventCenterY - nCharCenterY) * (nEventCenterY - nCharCenterY);
			if ((pInfoMapEventBest == NULL) || (nDistSq < nBestDistSq)) {
				pInfoMapEventBest = pInfoMapEventBase;
				nBestDistSq = nDistSq;
			}
		}
		pInfoMapEventBase = NULL;
	}
	pInfoMapEventBase = pInfoMapEventBest;
	if (pInfoMapEventBase == NULL) {
		goto Exit;
	}
	/* 向き指定あり？ */
	if (!bIgnoreDirection && (pInfoMapEventBase->m_nHitDirection >= 0)) {
		if (pInfoChar->m_nDirection != pInfoMapEventBase->m_nHitDirection) {
			pInfoMapEventBase = NULL;
			goto Exit;
		}
	}
	/* イベントタイル座標を出力（省略可能） */
	if (pnTileX != NULL) { *pnTileX = pInfoMapEventBase->m_ptPos.x; }
	if (pnTileY != NULL) { *pnTileY = pInfoMapEventBase->m_ptPos.y; }

	/* チェックのみ？ */
	if (bCheck) {
		goto Exit;
	}

	if (pInfoMapEventBase->m_dwSoundID != 0) {
		CPacketADMIN_PLAYSOUND PacketPLAYSOUND;

		PacketPLAYSOUND.Make (pInfoMapEventBase->m_dwSoundID);
		m_pMainFrame->SendToClient (pInfoChar->m_dwSessionID, &PacketPLAYSOUND);
	}

	switch (pInfoMapEventBase->m_nType) {
	case MAPEVENTTYPE_MOVE:			bRet = MapEventProcMOVE			(pInfoChar, pInfoMapEventBase);	break;	/* マップ内移動 */
	case MAPEVENTTYPE_MAPMOVE:		bRet = MapEventProcMAPMOVE		(pInfoChar, pInfoMapEventBase);	break;	/* マップ間移動 */
	case MAPEVENTTYPE_INITSTATUS:	bRet = MapEventProcINITSTATUS	(pInfoChar, pInfoMapEventBase);	break;	/* ステータス初期化 */
	case MAPEVENTTYPE_GRPIDTMP:		bRet = MapEventProcGRPIDTMP		(pInfoChar, pInfoMapEventBase);	break;	/* 一時画像設定 */
	case MAPEVENTTYPE_LIGHT:		bRet = MapEventProcLIGHT		(pInfoChar, pInfoMapEventBase);	break;	/* 灯り */
	}

Exit:
	/* チェックのみ？ */
	if (bCheck) {
		if (pInfoMapEventBase == NULL) {
			bRet = FALSE;
		}
	}
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::MapEventProcMOVE								 */
/* 内容		:マップイベント処理(マップ内移動)								 */
/* 日付		:2008/11/16														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::MapEventProcMOVE(CInfoCharSvr *pInfoChar, CInfoMapEventBase *pInfoMapEventBase)
{
	BOOL bRet;
	int i, nCount, nDirection;
	PCInfoMapEventMOVE pInfoMapEvent;
	PCInfoCharSvr pInfoCharTmp;
	CPacketMAP_PARA1 PacketMAP_PARA1;
	CPacketCHAR_STATE PacketCHAR_STATE;
        ARRAYINFOCHARSVR apInfoChar;

	bRet = TRUE;
	pInfoMapEvent = (PCInfoMapEventMOVE)pInfoMapEventBase;

	nDirection = pInfoMapEvent->m_nDirection;

	GetTailCharInfo (pInfoChar, apInfoChar);
	apInfoChar.insert (apInfoChar.begin (), pInfoChar);
	nCount = apInfoChar.size();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = apInfoChar[i];

		/* 周りのキャラに通知 */
		PacketCHAR_STATE.Make (pInfoCharTmp->m_dwCharID, CHARMOVESTATE_DELETE);
		m_pMainFrame->SendToScreenChar (pInfoCharTmp, &PacketCHAR_STATE);

		pInfoCharTmp->SetPos (
			MapTileToPixelX (pInfoMapEvent->m_ptDst.x),
			MapTileToPixelY (pInfoMapEvent->m_ptDst.y) + MAPPARTSSIZE,
			TRUE);
		pInfoCharTmp->m_bSuppressMapEventUntilLeave = TRUE;
		pInfoCharTmp->m_dwSuppressMapEventMapID = pInfoCharTmp->m_dwMapID;
		pInfoCharTmp->m_nSuppressMapEventTileX = pInfoMapEvent->m_ptDst.x;
		pInfoCharTmp->m_nSuppressMapEventTileY = pInfoMapEvent->m_ptDst.y;
		if (nDirection >= 0) {
			pInfoCharTmp->SetDirection (nDirection);
		}
		pInfoCharTmp->AddProcInfo (CHARPROCID_MAPMOVEIN, 2000, 0);
		pInfoCharTmp->AddProcInfo (CHARPROCID_INVINCIBLE, 5000, 0);
		pInfoCharTmp->m_bStatusInvincible = TRUE;
		pInfoCharTmp->m_bStateFadeInOut = TRUE;

		PacketMAP_PARA1.Make (SBOCOMMANDID_SUB_MAP_FADEINOUT, pInfoCharTmp->m_dwMapID, 1);
		m_pMainFrame->SendToClient (pInfoCharTmp->m_dwSessionID, &PacketMAP_PARA1);
	}

	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::MapEventProcMAPMOVE							 */
/* 内容		:マップイベント処理(マップ間移動)								 */
/* 日付		:2008/11/16														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::MapEventProcMAPMOVE(CInfoCharSvr *pInfoChar, CInfoMapEventBase *pInfoMapEventBase)
{
	BOOL bRet;
	int i, nCount, nDirection;
	PCInfoCharSvr pInfoCharTmp;
	PCInfoMapEventMAPMOVE pInfoMapEvent;
	CPacketMAP_PARA1 PacketMAP_PARA1;
	CPacketCHAR_STATE PacketCHAR_STATE;
        ARRAYINFOCHARSVR apInfoChar;

	bRet = TRUE;
	pInfoMapEvent = (PCInfoMapEventMAPMOVE)pInfoMapEventBase;

	nDirection = pInfoMapEvent->m_nDirection;

	GetTailCharInfo (pInfoChar, apInfoChar);
	apInfoChar.insert (apInfoChar.begin (), pInfoChar);

	nCount = apInfoChar.size();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = apInfoChar[i];

		/* 周りのキャラに通知 */
		PacketCHAR_STATE.Make (pInfoCharTmp->m_dwCharID, CHARMOVESTATE_DELETE);
		m_pMainFrame->SendToScreenChar (pInfoCharTmp, &PacketCHAR_STATE);

		pInfoCharTmp->m_dwMapID = pInfoMapEvent->m_dwMapID;
		pInfoCharTmp->SetPos (
			MapTileToPixelX (pInfoMapEvent->m_ptDst.x),
			MapTileToPixelY (pInfoMapEvent->m_ptDst.y) + MAPPARTSSIZE,
			TRUE);
		pInfoCharTmp->m_bSuppressMapEventUntilLeave = TRUE;
		pInfoCharTmp->m_dwSuppressMapEventMapID = pInfoCharTmp->m_dwMapID;
		pInfoCharTmp->m_nSuppressMapEventTileX = pInfoMapEvent->m_ptDst.x;
		pInfoCharTmp->m_nSuppressMapEventTileY = pInfoMapEvent->m_ptDst.y;
		if (nDirection >= 0) {
			pInfoCharTmp->SetDirection (nDirection);
		}
		pInfoCharTmp->AddProcInfo (CHARPROCID_MAPMOVEOUT, 2000, 0);
		pInfoCharTmp->AddProcInfo (CHARPROCID_INVINCIBLE, 5000, 0);
		pInfoCharTmp->m_bStatusInvincible = TRUE;
		pInfoCharTmp->m_bStateFadeInOut = TRUE;

		PacketMAP_PARA1.Make (SBOCOMMANDID_SUB_MAP_FADEINOUT, pInfoCharTmp->m_dwMapID, 1);
		m_pMainFrame->SendToClient (pInfoCharTmp->m_dwSessionID, &PacketMAP_PARA1);
	}

	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::MapEventProcINITSTATUS						 */
/* 内容		:マップイベント処理(ステータス初期化)							 */
/* 日付		:2008/11/16														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::MapEventProcINITSTATUS(CInfoCharSvr *pInfoChar, CInfoMapEventBase *pInfoMapEventBase)
{
	BOOL bRet;
        PCInfoMapEventINITSTATUS pInfoMapEvent;
        ARRAYINFOCHARSVR apInfoChar;

	bRet = TRUE;
	pInfoMapEvent = (PCInfoMapEventINITSTATUS)pInfoMapEventBase;

	if (pInfoMapEvent->m_dwEffectID != 0) {
		CPacketCHAR_SET_EFFECT PacketCHAR_SET_EFFECT;

		PacketCHAR_SET_EFFECT.Make (pInfoChar->m_dwCharID, pInfoMapEvent->m_dwEffectID);
		m_pMainFrame->SendToScreenChar (pInfoChar, &PacketCHAR_SET_EFFECT);
	}

	SetInitStatus (pInfoChar);
	pInfoChar->m_bChgStatus = TRUE;

	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::MapEventProcGRPIDTMP							 */
/* 内容		:マップイベント処理(一時画像設定)								 */
/* 日付		:2008/11/16														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::MapEventProcGRPIDTMP(CInfoCharSvr *pInfoChar, CInfoMapEventBase *pInfoMapEventBase)
{
	BOOL bRet;
	PCInfoMapEventGRPIDTMP pInfoMapEvent;

	bRet = TRUE;
	pInfoMapEvent = (PCInfoMapEventGRPIDTMP)pInfoMapEventBase;

	/* 設定？ */
	if (pInfoMapEvent->m_nSetType == GRPIDTMPTYPE_ON) {
		pInfoChar->m_wGrpIDTmpMain	= (WORD)pInfoMapEvent->m_dwIDMain;	/* 画像ID(一時服:メイン) */
		pInfoChar->m_wGrpIDTmpSub	= (WORD)pInfoMapEvent->m_dwIDSub;	/* 画像ID(一時服:サブ) */
	/* 解除 */
	} else {
		pInfoChar->m_wGrpIDTmpMain	= 0;		/* 画像ID(一時服:メイン) */
		pInfoChar->m_wGrpIDTmpSub	= 0;		/* 画像ID(一時服:サブ) */
	}
	pInfoChar->m_bChgGrp = TRUE;

	/* 付いて行くのと付いて来ているのを解除させる為2回呼ぶ */
	Tail (pInfoChar, NULL, FALSE);
	Tail (pInfoChar, NULL, FALSE);

	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::MapEventProcLIGHT								 */
/* 内容		:マップイベント処理(灯り)										 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::MapEventProcLIGHT(CInfoCharSvr *pInfoChar, CInfoMapEventBase *pInfoMapEventBase)
{
	BOOL bRet;
	PCInfoMapEventLIGHT pInfoMapEvent;

	bRet = TRUE;
	pInfoMapEvent = (PCInfoMapEventLIGHT)pInfoMapEventBase;

	pInfoChar->m_nLightLevel = 0;
	pInfoChar->m_dwLightTime = 0;
	/* 設定？ */
	if (pInfoMapEvent->m_bLightOn) {
		pInfoChar->m_nLightLevel = 1;
		if (pInfoMapEvent->m_dwTime != 0) {
			pInfoChar->m_dwLightTime = timeGetTime () + pInfoMapEvent->m_dwTime;
		}
	}
	pInfoChar->m_bChgStatus = TRUE;

	return bRet;
}

/* Copyright(C)URARA-works 2008 */
