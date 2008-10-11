/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:StateProcLOGINMENU.cpp										 */
/* 内容			:状態処理クラス(ログイン画面) 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/05													 */
/* ========================================================================= */

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "PacketACCOUNT_REQ_MAKECHAR.h"
#include "PacketACCOUNT_RES_MAKECHAR.h"
#include "PacketCHAR_REQ_CHARINFO.h"
#include "PacketCONNECT_REQ_PLAY.h"
#include "InfoAccount.h"
#include "LibInfoCharCli.h"
#include "WindowSTATUS.h"
#include "WindowLOGINMENU.h"
#include "WindowCHARNAME.h"
#include "WindowSEX.h"
#include "WindowSTYLESELECT.h"
#include "WindowNAMEINPUT.h"
#include "WindowFAMILYTYPE.h"
#include "LayerCharSelect.h"
#include "InfoCharCli.h"
#include "MgrData.h"
#include "MgrWindow.h"
#include "MgrDraw.h"
#include "MgrLayer.h"
#include "StateProcLOGINMENU.h"

/* ========================================================================= */
/* 定数の定義																 */
/* ========================================================================= */

enum {
	STATE_MENU = 0,										/* メニュー選択中 */
	STATE_SELECTPLAYCHAR,								/* キャラ選択中 */
	STATE_SELECTDELETECHAR,								/* 削除キャラ選択中 */
	STATE_BACK,											/* 戻る */
};


/* ========================================================================= */
/* 関数名	:CStateProcLOGINMENU::CStateProcLOGINMENU						 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

CStateProcLOGINMENU::CStateProcLOGINMENU()
{
	m_dwLastTimeKey		= 0;
	m_nCountKeyRepeate	= 0;
	m_nSelect			= 0;
	m_nState			= STATE_MENU;

	m_pInfoAccount			= NULL;
	m_pWindowLOGINMENU		= NULL;
	m_pWindowSTATUS			= NULL;
	m_pWindowSTYLESELECT	= NULL;
	m_pWindowNAMEINPUT		= NULL;

	m_pInfoCharCli			= new CInfoCharCli;
}


/* ========================================================================= */
/* 関数名	:CStateProcLOGINMENU::~CStateProcLOGINMENU						 */
/* 内容		:デストラクタ													 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

CStateProcLOGINMENU::~CStateProcLOGINMENU()
{
	SAFE_DELETE (m_pInfoCharCli);
}


/* ========================================================================= */
/* 関数名	:CStateProcLOGINMENU::Init										 */
/* 内容		:初期化															 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

void CStateProcLOGINMENU::Init(void)
{
	m_pInfoCharCli->Create (m_pMgrData);

	m_pMgrWindow->MakeWindowLOGINMENU ();
//	m_pMgrWindow->MakeWindowACCOUNTINFO ();
//	m_pMgrWindow->MakeWindowSTATUS ();

	m_pWindowLOGINMENU	= (PCWindowLOGINMENU)m_pMgrWindow->GetWindow (WINDOWTYPE_LOGINMENU);
//	m_pWindowSTATUS		= (PCWindowSTATUS)m_pMgrWindow->GetWindow (WINDOWTYPE_STATUS);
	m_pMgrLayer->MakeLOGINMENU ();
	m_pMgrLayer->MakeCHARSELECT (m_pMgrData->GetAccountID ());

	PostMessage (m_hWndMain, WM_MAINFRAME, MAINFRAMEMSG_RENEWACCOUNTINFO, 0);
}


/* ========================================================================= */
/* 関数名	:CStateProcLOGINMENU::OnWindowMsg								 */
/* 内容		:メッセージハンドラ(WM_WINDOWMSG)								 */
/* 日付		:2006/11/08														 */
/* ========================================================================= */

void CStateProcLOGINMENU::OnWindowMsg(int nType, DWORD dwPara)
{
	switch (nType) {
	case WINDOWTYPE_LOGINMENU:		OnWindowMsgLOGINMENU (dwPara);		break;	/* メニュー */
	case WINDOWTYPE_FAMILYTYPE:		OnWindowMsgFAMILYTYPE (dwPara);		break;	/* 種族選択 */
	case WINDOWTYPE_STYLESELECT:	OnWindowMsgSTYLESELECT (dwPara);	break;	/* 容姿選択 */
	case WINDOWTYPE_NAMEINPUT:		OnWindowMsgNAMEINPUT (dwPara);		break;	/* 名前入力 */

	case WINDOWTYPE_CHARNAME:		/* キャラ名入力 */
		m_pWindowNAMEINPUT->OnWindowMsg (nType, dwPara);
		break;
	case WINDOWTYPE_SEX:			/* 性別 */
	case WINDOWTYPE_HAIRTYPE:		/* 髪型選択 */
	case WINDOWTYPE_HAIRCOLOR:		/* 髪色選択 */
	case WINDOWTYPE_EYECOLOR:		/* 目色選択 */
		m_pWindowSTYLESELECT->OnWindowMsg (nType, dwPara);
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CStateProcLOGINMENU::OnMainFrame								 */
/* 内容		:メッセージハンドラ(WM_MAINFRAME)								 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

void CStateProcLOGINMENU::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
	switch (dwCommand) {
	case MAINFRAMEMSG_RES_MAKECHAR:			/* キャラ作成応答 */
		switch (dwParam) {
		case MAKECHARRES_NG_USE:		/* 使用済み */
			m_pMgrWindow->MakeWindowMSG ("その名前は使用済みです", 3000, 4);
			break;
		case MAKECHARRES_NG_SPACE:		/* 空白が使用されている */
			m_pMgrWindow->MakeWindowMSG ("キャラ名に空白は使えません", 3000, 4);
			break;
		default:
			m_pMgrWindow->MakeWindowMSG ("キャラクターを作成しました", 3000, 4);
			m_pMgrWindow->Delete (WINDOWTYPE_NAMEINPUT);
			m_pWindowLOGINMENU->SetPos (0);
			break;
		}
		m_pMgrWindow->Update ();
		break;
	case MAINFRAMEMSG_RENEWACCOUNTINFO:		/* アカウント情報更新 */
		{
			int i, nCount;
			CPacketCHAR_REQ_CHARINFO Packet;

			m_pMgrWindow->Update ();
			m_pInfoAccount = m_pMgrData->GetAccount ();
			nCount = m_pInfoAccount->m_adwCharID.GetSize ();
			for (i = 0; i < nCount; i ++) {
				Packet.Make (m_pInfoAccount->m_adwCharID[i]);
				m_pSock->Send (&Packet);
			}
		}
		break;
	case MAINFRAMEMSG_RENEWCHARINFO:		/* キャラ情報更新 */
		m_pMgrWindow->Update ();
		break;
	default:
		CStateProcBase::OnMainFrame (dwCommand, dwParam);
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CStateProcLOGINMENU::OnLeft									 */
/* 内容		:キーハンドラ(←)												 */
/* 日付		:2005/07/09														 */
/* ========================================================================= */

BOOL CStateProcLOGINMENU::OnLeft(BOOL bDown)
{
	BOOL bRet;
	DWORD dwTime, dwTimeTmp;
	PCLayerCharSelect pLayer;

	bRet	= FALSE;
	dwTime	= 250;

	if (bDown == FALSE) {
		m_dwLastTimeKey = 0;
		goto Exit;
	}

	if (m_nCountKeyRepeate > 1) {
		dwTime = 125;
	}
	dwTimeTmp = timeGetTime () - m_dwLastTimeKey;
	if (dwTimeTmp < dwTime) {
		goto Exit;
	}

	if (m_nSelect <= 0) {
		goto Exit;
	}

	m_nSelect --;
	m_dwLastTimeKey = timeGetTime ();
	pLayer = (PCLayerCharSelect)m_pMgrLayer->Get (LAYERTYPE_CHARSELECT);
	pLayer->SetSelect (m_nSelect);

Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CStateProcLOGINMENU::OnRight									 */
/* 内容		:キーハンドラ(→)												 */
/* 日付		:2005/07/09														 */
/* ========================================================================= */

BOOL CStateProcLOGINMENU::OnRight(BOOL bDown)
{
	BOOL bRet;
	DWORD dwTime, dwTimeTmp;
	PCLayerCharSelect pLayer;

	bRet	= FALSE;
	dwTime	= 250;

	if (bDown == FALSE) {
		m_dwLastTimeKey = 0;
		goto Exit;
	}

	if (m_nCountKeyRepeate > 1) {
		dwTime = 125;
	}
	dwTimeTmp = timeGetTime () - m_dwLastTimeKey;
	if (dwTimeTmp < dwTime) {
		goto Exit;
	}

	if (m_nSelect >= m_pInfoAccount->GetCharCount () - 1) {
		goto Exit;
	}

	m_nSelect ++;
	m_dwLastTimeKey = timeGetTime ();
	pLayer = (PCLayerCharSelect)m_pMgrLayer->Get (LAYERTYPE_CHARSELECT);
	pLayer->SetSelect (m_nSelect);

Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CStateProcLOGINMENU::OnX										 */
/* 内容		:キーハンドラ(X)												 */
/* 日付		:2005/06/26														 */
/* ========================================================================= */

BOOL CStateProcLOGINMENU::OnX(BOOL bDown)
{
	PCLayerCharSelect pLayer;

	if (bDown) {
		return FALSE;
	}

	pLayer = (PCLayerCharSelect)m_pMgrLayer->Get (LAYERTYPE_CHARSELECT);
	m_pMgrData->SetCharID (pLayer->GetSelectCharID ());

	m_pMgrDraw->SetFadeState (FADESTATE_FADEOUT);

	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CStateProcLOGINMENU::OnZ										 */
/* 内容		:キーハンドラ(Z)												 */
/* 日付		:2005/06/26														 */
/* ========================================================================= */

BOOL CStateProcLOGINMENU::OnZ(BOOL bDown)
{
	PCLayerCharSelect pLayer;

	m_pWindowLOGINMENU->SetActive (TRUE);
	m_pWindowLOGINMENU->SetInput (TRUE);

	m_nSelect	= 0;
	m_nState	= STATE_MENU;

	pLayer = (PCLayerCharSelect)m_pMgrLayer->Get (LAYERTYPE_CHARSELECT);
	pLayer->SetSelect (-1);
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CStateProcLOGINMENU::OnMgrDrawEND_FADEOUT						 */
/* 内容		:フェードアウト完了												 */
/* 日付		:2007/02/27														 */
/* ========================================================================= */

void CStateProcLOGINMENU::OnMgrDrawEND_FADEOUT(DWORD dwPara)
{
	PCLibInfoCharCli pLibInfoCharCli;
	CPacketCHAR_REQ_CHARINFO PacketCHAR_REQ_CHARINFO;
	CPacketCONNECT_REQ_PLAY PacketCONNECT_REQ_PLAY;

	switch (m_nState) {
	case STATE_SELECTPLAYCHAR:	/* キャラ選択 */
		pLibInfoCharCli = m_pMgrData->GetLibInfoChar ();
		pLibInfoCharCli->DeleteAll ();

		PacketCHAR_REQ_CHARINFO.Make (m_pMgrData->GetCharID ());
		m_pSock->Send (&PacketCHAR_REQ_CHARINFO);
		PacketCONNECT_REQ_PLAY.Make (m_pMgrData->GetAccountID (), m_pMgrData->GetCharID ());
		m_pSock->Send (&PacketCONNECT_REQ_PLAY);
		break;

	case STATE_BACK:			/* 戻る */
		m_pSock->Destroy ();
		PostMessage (m_hWndMain, WM_MAINFRAME, MAINFRAMEMSG_CHGSTATE, GAMESTATE_LOGIN);
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CStateProcLOGINMENU::OnWindowMsgLOGINMENU						 */
/* 内容		:メニューウィンドウからの通知									 */
/* 日付		:2007/04/24														 */
/* ========================================================================= */

void CStateProcLOGINMENU::OnWindowMsgLOGINMENU(DWORD dwPara)
{
	PCLayerCharSelect pLayer;

	switch (dwPara) {
	case 0:		/* キャラ選択 */
		m_nState = STATE_SELECTPLAYCHAR;
		m_pWindowLOGINMENU->SetActive (FALSE);
		m_pWindowLOGINMENU->SetInput (FALSE);
		pLayer = (PCLayerCharSelect)m_pMgrLayer->Get (LAYERTYPE_CHARSELECT);
		pLayer->SetSelect (0);
		break;

	case 1:		/* 新規作成 */
		m_pMgrWindow->MakeWindowFAMILYTYPE ();
		break;

	case 2:		/* キャラ削除 */
		break;

	case 3:		/* 戻る */
		m_nState = STATE_BACK;
		m_pMgrDraw->SetFadeState (FADESTATE_FADEOUT);
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CStateProcLOGINMENU::OnWindowMsgFAMILYTYPE						 */
/* 内容		:種族選択ウィンドウからの通知									 */
/* 日付		:2007/04/24														 */
/* ========================================================================= */

void CStateProcLOGINMENU::OnWindowMsgFAMILYTYPE(DWORD dwPara)
{
	PCWindowFAMILYTYPE pWindow;

	switch (dwPara) {
	case 0:
		m_pMgrWindow->MakeWindowSTYLESELECT ();
		m_pWindowSTYLESELECT = (PCWindowSTYLESELECT)m_pMgrWindow->GetWindow (WINDOWTYPE_STYLESELECT);

		pWindow = (PCWindowFAMILYTYPE)m_pMgrWindow->GetWindow (WINDOWTYPE_FAMILYTYPE);
		if (pWindow) {
			/* 種族選択画面から来た */
			m_pInfoCharCli->m_wFamilyID = pWindow->GetType ();
			m_pMgrWindow->Delete (WINDOWTYPE_FAMILYTYPE);
			m_pWindowSTYLESELECT->SetFamilyType (m_pInfoCharCli->m_wFamilyID);

		} else {
			/* 容姿選択画面から戻ってきた */
			m_pWindowSTYLESELECT->SetInfoChar (m_pInfoCharCli);
		}
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CStateProcLOGINMENU::OnWindowMsgSTYLESELECT					 */
/* 内容		:容姿選択ウィンドウからの通知									 */
/* 日付		:2007/04/24														 */
/* ========================================================================= */

void CStateProcLOGINMENU::OnWindowMsgSTYLESELECT(DWORD dwPara)
{
	switch (dwPara) {
	case 0:
		m_pInfoCharCli->Copy (m_pWindowSTYLESELECT->m_pInfoCharCli);
		m_pMgrWindow->Delete (WINDOWTYPE_STYLESELECT);
		m_pMgrWindow->MakeWindowNAMEINPUT ();
		m_pWindowNAMEINPUT = (PCWindowNAMEINPUT)m_pMgrWindow->GetWindow (WINDOWTYPE_NAMEINPUT);
		m_pWindowNAMEINPUT->m_pInfoCharCli->Copy (m_pInfoCharCli);
		m_pWindowNAMEINPUT->m_pInfoCharCli->MakeCharGrp ();
		break;

	case -1:
		/* キャンセルされたので前の画面に戻る */
		m_pMgrWindow->Delete (WINDOWTYPE_STYLESELECT);
		PostMessage (m_hWndMain, WM_WINDOWMSG, WINDOWTYPE_LOGINMENU, 1);
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CStateProcLOGINMENU::OnWindowMsgNAMEINPUT						 */
/* 内容		:名前入力ウィンドウからの通知									 */
/* 日付		:2007/04/24														 */
/* ========================================================================= */

void CStateProcLOGINMENU::OnWindowMsgNAMEINPUT(DWORD dwPara)
{
	CPacketACCOUNT_REQ_MAKECHAR Packet;

	switch (dwPara) {
	case 0:
		m_pInfoCharCli->Copy (m_pWindowNAMEINPUT->m_pInfoCharCli);
		Packet.Make (m_pMgrData->GetAccountID (), m_pInfoCharCli);
		m_pSock->Send (&Packet);
		break;

	case -1:
		/* キャンセルされたので前の画面に戻る */
		m_pMgrWindow->Delete (WINDOWTYPE_NAMEINPUT);
		PostMessage (m_hWndMain, WM_WINDOWMSG, WINDOWTYPE_FAMILYTYPE, 0);
		break;
	}
}

/* Copyright(C)URARA-works 2006 */
