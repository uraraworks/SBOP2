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


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::CheckMapEvent									 */
/* 内容		:マップイベントチェック											 */
/* 日付		:2008/06/28														 */
/* 戻り値	:TRUE:応答を返す	チェックのみの場合は TRUE:マップイベントあり */
/* ========================================================================= */

BOOL CLibInfoCharSvr::CheckMapEvent(
	CInfoCharSvr *pInfoChar,	/* [in] 対象キャラ */
	BOOL bCheck/*FALSE*/)		/* [in] TRUE:チェックのみ */
{
	BOOL bRet, bResult, bBreak;
	int x, y;
	RECT rcChar, rcMap, rcMapEvent;
	POINT *pptPos1, *pptPos2;
	PCInfoMapBase pInfoMap;
	PCInfoMapEventBase pInfoMapEventBase;
	CPacketMAP_PARA1 PacketMAP_PARA1;
	CPacketCHAR_STATE PacketCHAR_STATE;
	CmyArray<PCInfoCharSvr, PCInfoCharSvr> apInfoChar;

	bRet = TRUE;
	pInfoMapEventBase = NULL;

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (pInfoChar->m_dwMapID);
	if (pInfoMap == NULL) {
		goto Exit;
	}
	pInfoMapEventBase = NULL;
	/* 座標を矩形で取得 */
	pInfoChar->GetPosRect (rcChar);
	pInfoChar->GetMapPosRect (rcMap);

	bBreak = FALSE;
	for (y = rcMap.top; y <= rcMap.bottom; y ++) {
		for (x = rcMap.left; x <= rcMap.right; x ++) {
			pInfoMapEventBase = pInfoMap->GetEvent (x, y);
			if (pInfoMapEventBase == NULL) {
				continue;
			}
			bResult = TRUE;
			pptPos1 = &pInfoMapEventBase->m_ptPos;
			pptPos2 = &pInfoMapEventBase->m_ptPos2;
			switch (pInfoMapEventBase->m_nHitType) {
			case MAPEVENTHITTYPE_MAPPOS:		/* マップ座標縦横いずれか */
				SetRect (&rcMapEvent, pptPos1->x * 2, pptPos1->y * 2, pptPos1->x * 2 + 1, pptPos1->y * 2 + 1);
				bResult = IsInRect (&rcChar, &rcMapEvent);
				break;
			case MAPEVENTHITTYPE_CHARPOS:		/* キャラ座標 */
				SetRect (&rcMapEvent, pptPos1->x * 2, pptPos1->y * 2, pptPos1->x * 2 + 1, pptPos1->y * 2 + 1);
				bResult = IsHitRect (&rcChar, &rcMapEvent);
				break;
			case MAPEVENTHITTYPE_AREA:			/* 範囲 */
				SetRect (&rcMapEvent, pptPos1->x * 2, pptPos1->y * 2, pptPos2->x * 2 + 1, pptPos2->y * 2 + 1);
				bResult = IsInRect (&rcChar, &rcMapEvent);
				break;
			case MAPEVENTHITTYPE_MAPPOS2:		/* マップ座標完全一致 */
				bResult = FALSE;
				if ((rcChar.left == pptPos1->x * 2) & (rcChar.top == pptPos1->y * 2 + 1)) {
					bResult = TRUE;
				}
				break;
			}
			if (bResult == FALSE) {
				pInfoMapEventBase = NULL;
				continue;
			}
			bBreak = TRUE;
			break;
		}
		if (bBreak) {
			break;
		}
	}
	if (pInfoMapEventBase == NULL) {
		goto Exit;
	}
	/* 向き指定あり？ */
	if (pInfoMapEventBase->m_nHitDirection >= 0) {
		if (pInfoChar->m_nDirection != pInfoMapEventBase->m_nHitDirection) {
			pInfoMapEventBase = NULL;
			goto Exit;
		}
	}
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
	CmyArray<PCInfoCharSvr, PCInfoCharSvr> apInfoChar;

	bRet = FALSE;
	pInfoMapEvent = (PCInfoMapEventMOVE)pInfoMapEventBase;

	nDirection = pInfoMapEvent->m_nDirection;

	GetTailCharInfo (pInfoChar, apInfoChar);
	apInfoChar.InsertAt (0, pInfoChar);
	nCount = apInfoChar.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = apInfoChar[i];

		/* 周りのキャラに通知 */
		PacketCHAR_STATE.Make (pInfoCharTmp->m_dwCharID, CHARMOVESTATE_DELETE);
		m_pMainFrame->SendToScreenChar (pInfoCharTmp, &PacketCHAR_STATE);

		pInfoCharTmp->SetPos (pInfoMapEvent->m_ptDst.x * 2, pInfoMapEvent->m_ptDst.y * 2 + 1, TRUE);
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
	CmyArray<PCInfoCharSvr, PCInfoCharSvr> apInfoChar;

	bRet = FALSE;
	pInfoMapEvent = (PCInfoMapEventMAPMOVE)pInfoMapEventBase;

	nDirection = pInfoMapEvent->m_nDirection;

	GetTailCharInfo (pInfoChar, apInfoChar);
	apInfoChar.InsertAt (0, pInfoChar);

	nCount = apInfoChar.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = apInfoChar[i];

		/* 周りのキャラに通知 */
		PacketCHAR_STATE.Make (pInfoCharTmp->m_dwCharID, CHARMOVESTATE_DELETE);
		m_pMainFrame->SendToScreenChar (pInfoCharTmp, &PacketCHAR_STATE);

		pInfoCharTmp->m_dwMapID = pInfoMapEvent->m_dwMapID;
		pInfoCharTmp->SetPos (pInfoMapEvent->m_ptDst.x * 2, pInfoMapEvent->m_ptDst.y * 2 + 1, TRUE);
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
	CmyArray<PCInfoCharSvr, PCInfoCharSvr> apInfoChar;

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
