/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:MainFrameRecvProcCHAR.cpp									 */
/* 内容			:クライアントメインフレーム(キャラ系受信処理) 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/12/31													 */
/* ========================================================================= */

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "InfoAccount.h"
#include "InfoMapBase.h"
#include "LibInfoMotion.h"
#include "LibInfoMotionType.h"
#include "LibInfoCharCli.h"
#include "LibInfoItem.h"
#include "InfoCharCli.h"
#include "LayerMap.h"
#include "StateProcBase.h"
#include "MgrLayer.h"
#include "MgrData.h"
#include "MgrSound.h"
#include "MgrWindow.h"
#include "MainFrame.h"


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCHAR										 */
/* 内容		:受信処理(キャラ系)												 */
/* 日付		:2006/12/31														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR(BYTE byCmdSub, PBYTE pData)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_CHAR_RES_CHARINFO:		RecvProcCHAR_RES_CHARINFO		(pData);	break;	/* キャラ情報応答 */
	case SBOCOMMANDID_SUB_CHAR_CHARINFO:			RecvProcCHAR_CHARINFO			(pData);	break;	/* キャラ情報通知 */
	case SBOCOMMANDID_SUB_CHAR_MOVEPOS:				RecvProcCHAR_MOVEPOS			(pData);	break;	/* 移動通知 */
	case SBOCOMMANDID_SUB_CHAR_STATE:				RecvProcCHAR_STATE				(pData);	break;	/* 状態通知 */
	case SBOCOMMANDID_SUB_CHAR_PROCSTATE:			RecvProcCHAR_PROCSTATE			(pData);	break;	/* 行動状態通知 */
	case SBOCOMMANDID_SUB_CHAR_CHAT:				RecvProcCHAR_CHAT				(pData);	break;	/* チャット通知 */
	case SBOCOMMANDID_SUB_CHAR_CHARID:				RecvProcCHAR_CHARID				(pData);	break;	/* キャラID通知 */
	case SBOCOMMANDID_SUB_CHAR_RES_PUTGET:			RecvProcCHAR_RES_PUTGET			(pData);	break;	/* アイテムを拾う置く応答 */
	case SBOCOMMANDID_SUB_CHAR_SET_EFFECT:			RecvProcCHAR_SET_EFFECT			(pData);	break;	/* エフェクトを指定 */
	case SBOCOMMANDID_SUB_CHAR_ITEMINFO:			RecvProcCHAR_ITEMINFO			(pData);	break;	/* アイテム情報通知 */
	case SBOCOMMANDID_SUB_CHAR_RES_TAIL:			RecvProcCHAR_RES_TAIL			(pData);	break;	/* 付いて行く応答 */
	case SBOCOMMANDID_SUB_CHAR_MODIFY_PARAM:		RecvProcCHAR_MODIFY_PARAM		(pData);	break;	/* パラメータ変更通知 */
	case SBOCOMMANDID_SUB_CHAR_GRP:					RecvProcCHAR_GRP				(pData);	break;	/* 画像通知 */
	case SBOCOMMANDID_SUB_CHAR_MOTION:				RecvProcCHAR_MOTION				(pData);	break;	/* モーション情報通知 */
	case SBOCOMMANDID_SUB_CHAR_MOTIONTYPE:			RecvProcCHAR_MOTIONTYPE			(pData);	break;	/* モーション種別情報通知 */
	case SBOCOMMANDID_SUB_CHAR_SET_EFCBALLOON:		RecvProcCHAR_SET_EFCBALLOON		(pData);	break;	/* 噴出しを指定 */
	case SBOCOMMANDID_SUB_CHAR_SET_MOTION:			RecvProcCHAR_SET_MOTION			(pData);	break;	/* モーションを指定 */
	case SBOCOMMANDID_SUB_CHAR_RES_CHECKMAPEVENT:	RecvProcCHAR_RES_CHECKMAPEVENT	(pData);	break;	/* マップイベントチェック応答 */
	case SBOCOMMANDID_SUB_CHAR_STATUS:				RecvProcCHAR_STATUS				(pData);	break;	/* ステータス情報通知 */
	case SBOCOMMANDID_SUB_CHAR_TEXTEFFECT:			RecvProcCHAR_TEXTEFFECT			(pData);	break;	/* 文字エフェクト通知 */
	case SBOCOMMANDID_SUB_CHAR_STATE_CHARGE:		RecvProcCHAR_STATE_CHARGE		(pData);	break;	/* 溜め状態通知 */
	case SBOCOMMANDID_SUB_CHAR_RES_TALKEVENT:		RecvProcCHAR_RES_TALKEVENT		(pData);	break;	/* 会話イベント情報応答 */
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCHAR_RES_CHARINFO							 */
/* 内容		:受信処理(キャラ情報応答)										 */
/* 日付		:2006/12/31														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_RES_CHARINFO(PBYTE pData)
{
	PCInfoCharCli pInfoChar;
	PCInfoAccount pInfoAccount;
	PCLayerMap pLayerMap;
	CPacketCHAR_RES_CHARINFO Packet;

	pInfoChar = NULL;
	Packet.Set (pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_pInfo->m_dwCharID);
	if (pInfoChar == NULL) {
		pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetNew (Packet.m_pInfo->m_nMoveType);
		pInfoChar->Copy (Packet.m_pInfo);
		pInfoChar->SetViewState (INFOCHARCLI_VIEWSTATE_FADEIN);
		m_pLibInfoChar->Add (pInfoChar);
	} else {
		/* 移動種別が変わった？ */
		if (Packet.m_pInfo->m_nMoveType != pInfoChar->m_nMoveType) {
			pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetNew (Packet.m_pInfo->m_nMoveType);
			m_pLibInfoChar->SetPtr (Packet.m_pInfo->m_dwCharID, pInfoChar);
		}
	}
	if (pInfoChar->m_nMoveState == CHARMOVESTATE_DELETEREADY) {
		return;
	}

	pInfoChar->Copy (Packet.m_pInfo);
	pInfoChar->MakeCharGrp ();
	pInfoChar->m_bRedraw = TRUE;
	m_pLibInfoChar->RenewMotionInfo (pInfoChar);

	pInfoAccount = m_pMgrData->GetAccount ();
	if (pInfoAccount->m_dwCharID == Packet.m_pInfo->m_dwCharID) {
		m_pMgrData->SetPlayerChar (pInfoChar);
	}

	/* 自キャラ？ */
	if (m_pMgrData->GetCharID () == pInfoChar->m_dwCharID) {
		if (Packet.m_bChgScreenPos) {
			pLayerMap = (PCLayerMap)m_pMgrLayer->Get (LAYERTYPE_MAP);
			if (pLayerMap) {
				pLayerMap->SetCenterPos (pInfoChar->m_nMapX, pInfoChar->m_nMapY);
			}
			RenewItemArea ();
		}
	}

	if (pInfoChar) {
		PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWCHARINFO, pInfoChar->m_dwCharID);
	}
	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWCHARCOUNT, m_pLibInfoChar->GetCount ());
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCHAR_CHARINFO								 */
/* 内容		:受信処理(キャラ情報通知)										 */
/* 日付		:2007/01/27														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_CHARINFO(PBYTE pData)
{
	int i, nCount;
	PCInfoCharCli pInfoChar, pInfoCharTmp;
	CPacketCHAR_CHARINFO Packet;

	pInfoChar = NULL;
	Packet.Set (pData);

	nCount = Packet.m_pLibInfo->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = (PCInfoCharCli)Packet.m_pLibInfo->GetPtr (i);

		/* 自キャラ？ */
		if (m_pMgrData->GetCharID () == pInfoCharTmp->m_dwCharID) {
			continue;
		}
		pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (pInfoCharTmp->m_dwCharID);
		if (pInfoChar == NULL) {
			pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetNew (pInfoCharTmp->m_nMoveType);
			pInfoChar->Copy (pInfoCharTmp);
			m_pLibInfoChar->Add (pInfoChar);
		} else {
			if (pInfoChar->m_nMoveState == CHARMOVESTATE_DELETEREADY) {
				continue;
			}
			pInfoChar->Copy (pInfoCharTmp);
		}
		pInfoChar->MakeCharGrp ();
		m_pLibInfoChar->RenewMotionInfo (pInfoChar);
	}
	/* 重なり調整 */
	m_pLibInfoChar->SortY ();

	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWCHARCOUNT, m_pLibInfoChar->GetCount ());
	if (nCount == 1) {
		if (pInfoChar) {
			PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWCHARINFO, pInfoChar->m_dwCharID);
		}
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCHAR_MOVEPOS								 */
/* 内容		:受信処理(移動通知)												 */
/* 日付		:2007/01/14														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_MOVEPOS(PBYTE pData)
{
	int nState, nStateStand, nStateMove, nResult;
	BOOL bResult;
	PCInfoCharCli pInfoChar, pInfoCharPlayer;
	PCLayerMap pLayerMap;
	POINT ptPos;
	CPacketCHAR_MOVEPOS Packet;

	Packet.Set (pData);
	pLayerMap = (PCLayerMap)m_pMgrLayer->Get (LAYERTYPE_MAP);

	/* 自キャラ？ */
	if (m_pMgrData->GetCharID () == Packet.m_dwCharID) {
		m_pStateProc->KeyProc (0, FALSE);
		if (Packet.m_bUpdate == FALSE) {
			m_pLibInfoChar->SortY ();
			return;
		}
		if (pLayerMap) {
			nResult = pLayerMap->IsScrollPos (Packet.m_pos.x, Packet.m_pos.y, Packet.m_nDirection);
			if (nResult >= 0) {
				pLayerMap->Scroll (nResult, TRUE);
			}
			/* 画面外に出る？ */
			bResult = pLayerMap->IsInScreen (Packet.m_pos.x, Packet.m_pos.y);
			if (bResult == FALSE) {
				pLayerMap->SetScrollMode (TRUE, 0);
				pLayerMap->SetCenterPos (Packet.m_pos.x, Packet.m_pos.y);
			}

			RenewItemArea ();
		}
	}
	pInfoCharPlayer = m_pMgrData->GetPlayerChar ();

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	/* 知らないキャラ？ */
	if (pInfoChar == NULL) {
		CPacketCHAR_REQ_CHARINFO PacketCHAR_REQ_CHARINFO;

		/* 画面内？ */
		if (pLayerMap) {
			bResult = pLayerMap->IsInScreen (Packet.m_pos.x, Packet.m_pos.y);
			if (bResult) {
				PacketCHAR_REQ_CHARINFO.Make (Packet.m_dwCharID);
				m_pSock->Send (&PacketCHAR_REQ_CHARINFO);
			}
		}
		return;
	}
	nState = -1;

	nStateStand	= CHARMOVESTATE_STAND;
	nStateMove	= CHARMOVESTATE_MOVE;
	if (pInfoChar->IsStateBattle ()) {
		nStateStand	= CHARMOVESTATE_BATTLE;
		if (pInfoChar->m_nMoveState == CHARMOVESTATE_BATTLE_DEFENSE) {
			nStateStand	= CHARMOVESTATE_BATTLE_DEFENSE;
		}
		nStateMove	= CHARMOVESTATE_BATTLEMOVE;
	}

	if (pInfoChar->m_nDirection != Packet.m_nDirection) {
		if (pInfoChar->m_nMoveState != CHARMOVESTATE_SIT) {
			nState = nStateStand;
		}
	}

	if (!((pInfoChar->m_nMapX == Packet.m_pos.x) && (pInfoChar->m_nMapY == Packet.m_pos.y))) {
		nState = nStateMove;
	}

	ptPos.x = Packet.m_pos.x;
	ptPos.y = Packet.m_pos.y;
	bResult = TRUE;
	if (pInfoCharPlayer) {
		bResult = pInfoCharPlayer->IsViewArea (pInfoCharPlayer->m_dwMapID, &ptPos);
	}
	if (bResult == FALSE) {
		nState = CHARMOVESTATE_DELETEREADY;

	} else {
		bResult = pInfoChar->IsStateMove ();
		if (bResult) {
			pInfoChar->AddMovePosQue (nState, Packet.m_nDirection, Packet.m_pos.x, Packet.m_pos.y);
			nState = -1;
		} else {
			pInfoChar->SetPos (Packet.m_pos.x, Packet.m_pos.y);
			pInfoChar->SetDirection (Packet.m_nDirection);
			pInfoChar->m_bRedraw = TRUE;
		}
	}

	if (nState != -1) {
		pInfoChar->ChgMoveState (nState);
	}
	m_bRenewCharInfo = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCHAR_STATE									 */
/* 内容		:受信処理(状態通知)												 */
/* 日付		:2007/01/24														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_STATE(PBYTE pData)
{
	BOOL bChgBGM;
	int nState;
	PCInfoMapBase pInfoMap;
	PCInfoCharCli pInfoChar, pInfoCharPlayer;
	CPacketCHAR_STATE Packet;

	Packet.Set (pData);

	pInfoCharPlayer = m_pMgrData->GetPlayerChar ();

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	/* 知らないキャラ？ */
	if (pInfoChar == NULL) {
		return;
	}
	nState = Packet.m_nState;
	if (nState == CHARMOVESTATE_DELETE) {
		nState = CHARMOVESTATE_DELETEREADY;
	}

	if (nState == CHARMOVESTATE_BATTLEATACK) {
		m_pLibInfoChar->RenewMotionInfo (pInfoChar);
	}
	pInfoChar->ChgMoveState (nState);
	pInfoChar->SetChgWait (FALSE);

	if (pInfoChar == pInfoCharPlayer) {
		bChgBGM = FALSE;
		pInfoMap = m_pMgrData->GetMap ();
		if (pInfoMap && (pInfoMap->m_bRecovery == FALSE)) {
			/* 気絶した？ */
			if (pInfoChar->m_nMoveState == CHARMOVESTATE_SWOON) {
				bChgBGM = TRUE;
				m_pMgrWindow->MakeWindowSWOON ();
			} else {
				m_pMgrWindow->Delete (WINDOWTYPE_SWOON);
			}
		}
		ChgMoveState (bChgBGM);
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCHAR_PROCSTATE								 */
/* 内容		:受信処理(行動状態通知)											 */
/* 日付		:2008/06/11														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_PROCSTATE(PBYTE pData)
{
	PCInfoCharCli pInfoChar;
	CPacketCHAR_PARA1 Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	/* 知らないキャラ？ */
	if (pInfoChar == NULL) {
		return;
	}
	pInfoChar->SetProcState (Packet.m_dwPara);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCHAR_CHAT									 */
/* 内容		:受信処理(チャット通知)											 */
/* 日付		:2007/02/04														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_CHAT(PBYTE pData)
{
	PCInfoCharCli pInfoChar;
	CPacketCHAR_CHAT Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	/* 知らないキャラ？ */
	if (pInfoChar == NULL) {
		return;
	}
	pInfoChar->SetSpeak (Packet.m_strChat);
	m_pMgrSound->PlaySound (SOUNDID_CHAT);

	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWCHARCHAT, Packet.m_dwCharID);
	FlashMainWindow ();
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCHAR_CHARID								 */
/* 内容		:受信処理(キャラID通知)											 */
/* 日付		:2007/02/12														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_CHARID(PBYTE pData)
{
	int i, nCount;
	PCInfoCharCli pInfoChar;
	CPacketCHAR_CHARID Packet;
	CPacketCHAR_REQ_CHARINFO2 PacketCHAR_CHARINFO2;

	Packet.Set (pData);

	/* 知っているキャラを削除する */
	nCount = Packet.m_adwCharID.GetSize ();
	for (i = nCount - 1; i >= 0; i --) {
		pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_adwCharID[i]);
		/* 知らないキャラ？ */
		if (pInfoChar == NULL) {
			continue;
		}
		Packet.m_adwCharID.RemoveAt (i);
	}
	if (Packet.m_adwCharID.GetSize () <= 0) {
		return;
	}

	/* 残ったキャラの情報を要求 */
	PacketCHAR_CHARINFO2.Make (&Packet.m_adwCharID);
	m_pSock->Send (&PacketCHAR_CHARINFO2);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCHAR_RES_PUTGET							 */
/* 内容		:受信処理(アイテムを拾う置く応答)								 */
/* 日付		:2007/09/08														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_RES_PUTGET(PBYTE pData)
{
	PCInfoItem pInfoItem;
	CPacketCHAR_RES_PUTGET Packet;
	CmyString strTmp;

	Packet.Set (pData);

	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (Packet.m_dwItemID);
	if (pInfoItem == NULL) {
		return;
	}

	switch (Packet.m_nResult) {
	case RESULTID_CHAR_RES_PUTGET_PUT:		/* 置いた */
		strTmp.Format ("%sを置きました", (LPCSTR)pInfoItem->m_strName);
		break;
	case RESULTID_CHAR_RES_PUTGET_GET:		/* 取得した */
		strTmp.Format ("%sを拾いました", (LPCSTR)pInfoItem->m_strName);
		break;
	case RESULTID_CHAR_RES_PUTGET_DELETE:	/* 削除した */
		strTmp.Format ("%sは処分されました", (LPCSTR)pInfoItem->m_strName);
		break;
	}

	if (strTmp.IsEmpty ()) {
		return;
	}
	m_pMgrData->AddSystemMsg (FALSE, strTmp, RGB (255, 255, 255));
	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWSYSTEMMSG, 0);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCHAR_SET_EFFECT							 */
/* 内容		:受信処理(エフェクトを指定)										 */
/* 日付		:2007/07/27														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_SET_EFFECT(PBYTE pData)
{
	PCInfoCharCli pInfoChar;
	CPacketCHAR_SET_EFFECT Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	/* 知らないキャラ？ */
	if (pInfoChar == NULL) {
		return;
	}

	pInfoChar->SetEffectID (Packet.m_dwEffectID);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCHAR_ITEMINFO								 */
/* 内容		:受信処理(アイテム情報通知)										 */
/* 日付		:2007/08/05														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_ITEMINFO(PBYTE pData)
{
	PCInfoCharCli pInfoChar, pInfoCharPlayer;
	CPacketCHAR_ITEMINFO Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	/* 知らないキャラ？ */
	if (pInfoChar == NULL) {
		return;
	}

	pInfoChar->SetItem (&Packet.m_adwItemID);

	pInfoCharPlayer = m_pMgrData->GetPlayerChar ();
	if (pInfoChar == pInfoCharPlayer) {
		PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWCHARINFO, pInfoChar->m_dwCharID);
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCHAR_RES_TAIL								 */
/* 内容		:受信処理(付いて行く応答)										 */
/* 日付		:2007/09/05														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_RES_TAIL(PBYTE pData)
{
	PCInfoCharCli pInfoChar, pInfoCharPlayer, pInfoCharTarget, pInfoCharPara;
	CPacketCHAR_RES_TAIL Packet;
	CmyString strTmp, strNamePara;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	/* 知らないキャラ？ */
	if (pInfoChar == NULL) {
		return;
	}
	pInfoCharTarget = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwTargetCharID);
	pInfoCharPara	= (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwPara);
	pInfoCharPlayer = m_pMgrData->GetPlayerChar ();
	if (pInfoChar == pInfoCharPlayer) {
		switch (Packet.m_nResult) {
		case RES_TAIL_RESULT_OK:			/* ついていく */
			if (pInfoCharTarget) {
				strTmp.Format ("%sについていきます", (LPCSTR)pInfoCharTarget->m_strCharName);
			}
			pInfoCharPlayer->m_dwFrontCharID = Packet.m_dwTargetCharID;
			break;
		case RES_TAIL_RESULT_PARGE:			/* ついていく解除 */
			strTmp.Format ("ついていくのをやめました");
			pInfoCharPlayer->m_dwFrontCharID = Packet.m_dwTargetCharID;
			break;
		case RES_TAIL_RESULT_TAILPARGE:		/* つれていく解除 */
			pInfoCharPlayer->m_dwTailCharID = 0;
			strTmp.Format ("つれていくのをやめました");
			break;
		case RES_TAIL_RESULT_NG_TAIL:		/* 既についていっている */
			if (pInfoCharPara) {
				strNamePara = (LPCSTR)pInfoCharPara->m_strCharName;
			}
			if (pInfoCharTarget) {
				strTmp.Format ("%sには%sがついてきています", (LPCSTR)pInfoCharTarget->m_strCharName, (LPCSTR)strNamePara);
			}
			break;
		case RES_TAIL_RESULT_NG_STATE:		/* ついていけない状態 */
			strTmp.Format ("%sは取り込み中のようです", (LPCSTR)pInfoCharTarget->m_strCharName);
			break;
		case RES_TAIL_RESULT_NG_SWOON:		/* ついていけない */
			strTmp.Format ("気絶中なのでついていくことはできません");
			break;
		case RES_TAIL_RESULT_NG_PLACE:		/* ついていけない場所 */
			strTmp.Format ("%sがいる場所ではついていくことができません", (LPCSTR)pInfoCharTarget->m_strCharName);
			break;
		}
		if (strTmp.IsEmpty () == FALSE) {
			m_pMgrData->AddSystemMsg (FALSE, strTmp, RGB (255, 255, 255));
			PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWSYSTEMMSG, 0);
		}

	} else if (pInfoCharTarget == pInfoCharPlayer) {
		switch (Packet.m_nResult) {
		case RES_TAIL_RESULT_OK:
			strTmp.Format ("%sがついてきます", (LPCSTR)pInfoChar->m_strCharName);
			m_pMgrData->AddSystemMsg (FALSE, strTmp, RGB (255, 255, 255));
			PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWSYSTEMMSG, 0);
			pInfoCharPlayer->m_dwTailCharID = Packet.m_dwCharID;
			break;
		case RES_TAIL_RESULT_PARGE:
			strTmp.Format ("%sはついていくのをやめました", (LPCSTR)pInfoChar->m_strCharName);
			m_pMgrData->AddSystemMsg (FALSE, strTmp, RGB (255, 255, 255));
			PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWSYSTEMMSG, 0);
			pInfoCharPlayer->m_dwTailCharID = Packet.m_dwCharID;
			break;
		}
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCHAR_MODIFY_PARAM							 */
/* 内容		:受信処理(パラメータ変更通知)									 */
/* 日付		:2007/09/09														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_MODIFY_PARAM(PBYTE pData)
{
	PCInfoCharCli pInfoChar;
	CPacketCHAR_MODIFY_PARAM Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	/* 知らないキャラ？ */
	if (pInfoChar == NULL) {
		return;
	}

	switch (Packet.m_nType) {
	case PARAMID_CHAR_MODIFY_ANIME:		/* アニメーション番号の変更 */
		pInfoChar->m_nAnime = Packet.m_nParam;
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCHAR_GRP									 */
/* 内容		:受信処理(画像通知)												 */
/* 日付		:2007/10/08														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_GRP(PBYTE pData)
{
	PCInfoCharCli pInfoChar;
	CPacketCHAR_GRP Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	/* 知らないキャラ？ */
	if (pInfoChar == NULL) {
		return;
	}
	Packet.SetParam (pInfoChar);
	pInfoChar->MakeCharGrp ();
	pInfoChar->m_bRedraw = TRUE;
	m_pLibInfoChar->RenewMotionInfo (pInfoChar);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCHAR_MOTION								 */
/* 内容		:受信処理(モーション情報通知)									 */
/* 日付		:2007/11/23														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_MOTION(PBYTE pData)
{
	CPacketCHAR_MOTION Packet;

	Packet.Set (pData);
	m_pLibInfoMotion->SetList (Packet.m_dwMotionTypeID, Packet.m_dwMotionListID, Packet.m_pInfo);
	m_pLibInfoChar->RenewMotionInfo ((DWORD)0);

	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWMOTION, Packet.m_dwMotionListID);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCHAR_MOTIONTYPE							 */
/* 内容		:受信処理(モーション種別情報通知)								 */
/* 日付		:2008/06/13														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_MOTIONTYPE(PBYTE pData)
{
	PBYTE pDataTmp;
	CPacketCHAR_MOTIONTYPE Packet;

	Packet.Set (pData);
	pDataTmp = Packet.m_pInfo->GetSendData (Packet.m_dwMotionTypeID);
	m_pLibInfoMotionType->SetSendData (pDataTmp);

	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWMOTIONTYPE, Packet.m_dwMotionTypeID);
	SAFE_DELETE (pDataTmp);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCHAR_SET_EFCBALLOON						 */
/* 内容		:受信処理(噴出しを指定)											 */
/* 日付		:2007/12/31														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_SET_EFCBALLOON(PBYTE pData)
{
	PCInfoCharCli pInfoChar;
	CPacketCHAR_SET_EFCBALLOON Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	/* 知らないキャラ？ */
	if (pInfoChar == NULL) {
		return;
	}

	pInfoChar->SetBalloonID (Packet.m_dwBalloonID);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCHAR_SET_MOTION							 */
/* 内容		:受信処理(モーションを指定)										 */
/* 日付		:2008/01/03														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_SET_MOTION(PBYTE pData)
{
	PCInfoCharCli pInfoChar;
	CPacketCHAR_PARA1 Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	/* 知らないキャラ？ */
	if (pInfoChar == NULL) {
		return;
	}

	if (Packet.m_dwPara == -1) {
		m_pLibInfoChar->RenewMotionInfo (pInfoChar);
	} else {
		m_pLibInfoChar->SetMotionInfo (pInfoChar, CHARMOTIONID_INTERRUUPT, Packet.m_dwPara);
		pInfoChar->InitMotionInfo (CHARMOTIONID_INTERRUUPT);
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCHAR_RES_CHECKMAPEVENT						 */
/* 内容		:受信処理(マップイベントチェック応答)							 */
/* 日付		:2008/06/28														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_RES_CHECKMAPEVENT(PBYTE pData)
{
	PCInfoCharCli pInfoChar;
	CPacketCHAR_PARA1 Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	/* 知らないキャラ？ */
	if (pInfoChar == NULL) {
		return;
	}

	pInfoChar->m_bWaitCheckMapEvent = FALSE;
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCHAR_STATUS								 */
/* 内容		:受信処理(ステータス情報通知)									 */
/* 日付		:2008/06/29														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_STATUS(PBYTE pData)
{
	DWORD dwHPBack;
	PCInfoCharCli pInfoChar, pInfoCharPlayer;
	CPacketCHAR_STATUS Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	/* 知らないキャラ？ */
	if (pInfoChar == NULL) {
		return;
	}
	pInfoCharPlayer = m_pMgrData->GetPlayerChar ();

	dwHPBack = pInfoChar->m_dwHP;
	Packet.SetParam (pInfoChar);
	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWSTATUS, Packet.m_dwCharID);
	if (pInfoChar == pInfoCharPlayer) {
		/* HPが減った？ */
		if (dwHPBack > pInfoChar->m_dwHP) {
			PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_DAMAGE, Packet.m_dwCharID);
		}
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCHAR_TEXTEFFECT							 */
/* 内容		:受信処理(文字エフェクト通知)									 */
/* 日付		:2008/07/13														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_TEXTEFFECT(PBYTE pData)
{
	PCInfoCharCli pInfoChar;
	CPacketCHAR_TEXTEFFECT Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	/* 知らないキャラ？ */
	if (pInfoChar == NULL) {
		return;
	}

	if (Packet.m_dwData == -1) {
		pInfoChar->AddTextEffect ((LPCSTR)Packet.m_strData, Packet.m_nColor, Packet.m_nSize);
	} else {
		pInfoChar->AddTextEffect (Packet.m_dwData, Packet.m_nColor, Packet.m_nSize);
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCHAR_STATE_CHARGE							 */
/* 内容		:受信処理(溜め状態通知)											 */
/* 日付		:2008/08/13														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_STATE_CHARGE(PBYTE pData)
{
	PCInfoCharCli pInfoChar, pInfoCharPlayer;
	CPacketCHAR_PARA1 Packet;

	Packet.Set (pData);

	pInfoCharPlayer = m_pMgrData->GetPlayerChar ();

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}

	pInfoChar->m_bChargeAtack = (Packet.m_dwPara == 0) ? FALSE : TRUE;
	if (pInfoChar == pInfoCharPlayer) {
		ChgMoveState (FALSE);
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCHAR_RES_TALKEVENT							 */
/* 内容		:受信処理(会話イベント情報応答)									 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_RES_TALKEVENT(PBYTE pData)
{
	PCInfoCharCli pInfoCharPlayer;
	CPacketCHAR_RES_TALKEVENT Packet;

	Packet.Set (pData);

	if (Packet.m_dwParam == 0) {
		pInfoCharPlayer = m_pMgrData->GetPlayerChar ();
		if (pInfoCharPlayer) {
			pInfoCharPlayer->m_bWaitCheckMapEvent = FALSE;
		}
	}

	m_pMgrData->SetInfoTalkEvent (Packet.m_pInfo);

	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWTALKEVENT, Packet.m_dwParam);
	PostMessage (m_hWnd, WM_ADMINMSG, ADMINMSG_RENEWTALKEVENT, Packet.m_dwParam);
}

/* Copyright(C)URARA-works 2006 */
