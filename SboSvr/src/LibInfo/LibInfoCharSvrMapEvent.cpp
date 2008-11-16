/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:LibInfoCharSvrMapEvent.cpp									 */
/* 内容			:キャラ情報ライブラリクライアントクラス 実装ファイル		 */
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
/* ========================================================================= */

BOOL CLibInfoCharSvr::CheckMapEvent(CInfoCharSvr *pInfoChar)
{
	BOOL bRet;
	int i, nCount;
	PCInfoMapBase pInfoMap;
	PCInfoMapEventBase pInfoMapEventBase;
	CPacketMAP_PARA1 PacketMAP_PARA1;
	CPacketCHAR_STATE PacketCHAR_STATE;
	CmyArray<PCInfoCharSvr, PCInfoCharSvr> apInfoChar;

	bRet = TRUE;

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (pInfoChar->m_dwMapID);
	if (pInfoMap == NULL) {
		goto Exit;
	}
	pInfoChar->RenewBlockMapArea (pInfoChar->m_nMapX, pInfoChar->m_nMapY, pInfoChar->m_nDirection);

	pInfoMapEventBase = NULL;
	nCount = pInfoChar->m_aposBockMapArea.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoMapEventBase = pInfoMap->GetEvent (pInfoChar->m_aposBockMapArea[i].x, pInfoChar->m_aposBockMapArea[i].y);
		if (pInfoMapEventBase == NULL) {
			continue;
		}
		if (pInfoMapEventBase->m_nHitType == MAPEVENTHITTYPE_MAPPOS) {
			/* マップ座標で判定の時に半端な位置にいる？ */
			if ((pInfoChar->m_nMapX % 2) || (pInfoChar->m_nMapY % 2)) {
				pInfoMapEventBase = NULL;
				continue;
			}
		}
		break;
	}
	if (pInfoMapEventBase == NULL) {
		goto Exit;
	}

	switch (pInfoMapEventBase->m_nType) {
	case MAPEVENTTYPE_MOVE:			bRet = MapEventProcMOVE			(pInfoChar, pInfoMapEventBase);	break;	/* マップ内移動 */
	case MAPEVENTTYPE_MAPMOVE:		bRet = MapEventProcMAPMOVE		(pInfoChar, pInfoMapEventBase);	break;	/* マップ間移動 */
	case MAPEVENTTYPE_INITSTATUS:	bRet = MapEventProcINITSTATUS	(pInfoChar, pInfoMapEventBase);	break;	/* ステータス初期化 */
	case MAPEVENTTYPE_GRPIDTMP:		bRet = MapEventProcGRPIDTMP		(pInfoChar, pInfoMapEventBase);	break;	/* 一時画像設定 */
	}

Exit:
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
	int i, nCount;
	PCInfoMapEventMOVE pInfoMapEvent;
	PCInfoCharSvr pInfoCharTmp;
	CPacketMAP_PARA1 PacketMAP_PARA1;
	CPacketCHAR_STATE PacketCHAR_STATE;
	CmyArray<PCInfoCharSvr, PCInfoCharSvr> apInfoChar;

	bRet = FALSE;
	pInfoMapEvent = (PCInfoMapEventMOVE)pInfoMapEventBase;

	GetTailCharID (pInfoChar, apInfoChar);
	apInfoChar.InsertAt (0, pInfoChar);
	nCount = apInfoChar.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = apInfoChar[i];

		/* 周りのキャラに通知 */
		PacketCHAR_STATE.Make (pInfoCharTmp->m_dwCharID, CHARMOVESTATE_DELETE);
		m_pMainFrame->SendToScreenChar (pInfoCharTmp, &PacketCHAR_STATE);

		pInfoCharTmp->SetPos (pInfoMapEvent->m_ptDst.x * 2, pInfoMapEvent->m_ptDst.y * 2, TRUE);
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
	int i, nCount;
	PCInfoCharSvr pInfoCharTmp;
	PCInfoMapEventMAPMOVE pInfoMapEvent;
	CPacketMAP_PARA1 PacketMAP_PARA1;
	CPacketCHAR_STATE PacketCHAR_STATE;
	CmyArray<PCInfoCharSvr, PCInfoCharSvr> apInfoChar;

	bRet = FALSE;
	pInfoMapEvent = (PCInfoMapEventMAPMOVE)pInfoMapEventBase;

	GetTailCharID (pInfoChar, apInfoChar);
	apInfoChar.InsertAt (0, pInfoChar);

	nCount = apInfoChar.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = apInfoChar[i];

		/* 周りのキャラに通知 */
		PacketCHAR_STATE.Make (pInfoCharTmp->m_dwCharID, CHARMOVESTATE_DELETE);
		m_pMainFrame->SendToScreenChar (pInfoCharTmp, &PacketCHAR_STATE);

		pInfoCharTmp->m_dwMapID = pInfoMapEvent->m_dwMapID;
		pInfoCharTmp->SetPos (pInfoMapEvent->m_ptDst.x * 2, pInfoMapEvent->m_ptDst.y * 2, TRUE);
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

	bRet = FALSE;
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

	bRet = FALSE;
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

/* Copyright(C)URARA-works 2008 */
