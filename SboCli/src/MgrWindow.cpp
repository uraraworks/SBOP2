/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:MgrWindow.cpp												 */
/* 内容			:ウィンドウマネージャクラス 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/03													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrKeyInput.h"
#include "WindowLOGIN.h"
#include "WindowMSG.h"
#include "WindowLOGINMENU.h"
#include "WindowCHARNAME.h"
#include "WindowSEX.h"
#include "WindowACCOUNTINFO.h"
#include "WindowSTATUS.h"
#include "WindowCHAT.h"
#include "WindowFAMILYTYPE.h"
#include "WindowHAIRTYPE.h"
#include "WindowHAIRCOLOR.h"
#include "WindowEYECOLOR.h"
#include "WindowSTYLESELECT.h"
#include "WindowNAMEINPUT.h"
#include "WindowSYSTEMMENU.h"
#include "WindowSETCOLOR.h"
#include "WindowSETBGMVOLUME.h"
#include "WindowSETSOUNDVOLUME.h"
#include "WindowSETDRAWMODE.h"
#include "WindowITEMMENU.h"
#include "WindowITEMMENU_SELECT.h"
#include "WindowOPTION.h"
#include "WindowOPTION_VIEWSET.h"
#include "WindowOPTION_VOLUMESET.h"
#include "WindowOPTION_INPUTSET.h"
#include "WindowOPTION_TASKBAR.h"
#include "WindowOPTION_INPUTSET_SETDEVICE.h"
#include "WindowOPTION_ACTIONSET.h"
#include "WindowOPTION_ACTIONSET_SLEEPTIMER.h"
#include "WindowCOMMANDMENU.h"
#include "WindowCHAR_STATUS.h"
#include "WindowCHAR_STATUS2.h"
#include "WindowCHAR_STATUS3.h"
#include "MgrWindow.h"


/* ========================================================================= */
/* 関数名	:CMgrWindow::CMgrWindow											 */
/* 内容		:コンストラクタ													 */
/* 日付		:2005/05/06														 */
/* ========================================================================= */

CMgrWindow::CMgrWindow()
{
	m_bDraw			= FALSE;
	m_bKeyInput		= FALSE;
	m_pMgrGrpData	= NULL;
	m_pMgrData		= NULL;
	m_paWindow		= new ARRAYWINDOWBASE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::~CMgrWindow										 */
/* 内容		:デストラクタ													 */
/* 日付		:2005/05/06														 */
/* ========================================================================= */

CMgrWindow::~CMgrWindow()
{
	Destroy ();
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::Create												 */
/* 内容		:作成															 */
/* 日付		:2005/05/21														 */
/* ========================================================================= */

void CMgrWindow::Create(
	CMgrData	*pMgrData)		/* [in] データ管理 */
{
	m_pMgrData		= pMgrData;
	m_pMgrGrpData	= m_pMgrData->GetMgrGrpData ();
	m_pMgrKeyInput	= m_pMgrData->GetMgrKeyInput ();
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::Destroy											 */
/* 内容		:破棄															 */
/* 日付		:2005/05/21														 */
/* ========================================================================= */

void CMgrWindow::Destroy(void)
{
	DeleteAll ();
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::Update												 */
/* 内容		:全てのウィンドウを更新											 */
/* 日付		:2006/11/08														 */
/* ========================================================================= */

void CMgrWindow::Update(void)
{
	int i, nCount;
	PCWindowBase pWindow;

	nCount = m_paWindow->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pWindow = m_paWindow->GetAt (i);
		pWindow->Update ();
	}
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::Draw												 */
/* 内容		:描画															 */
/* 日付		:2005/05/06														 */
/* ========================================================================= */

void CMgrWindow::Draw(PCImg32 pDst)
{
	int i, nCount;
	PCWindowBase pWindow;

	nCount = m_paWindow->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pWindow = m_paWindow->GetAt (i);
		if (pWindow->IsShow () == FALSE) {
			continue;
		}
		pWindow->Draw (pDst);
	}
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::TimerProc											 */
/* 内容		:時間処理														 */
/* 日付		:2005/06/12														 */
/* ========================================================================= */

BOOL CMgrWindow::TimerProc(void)
{
	int i, nCount;
	BOOL bRet;
	PCWindowBase pWindow;

	bRet = FALSE;
	nCount = m_paWindow->GetSize ();
	for (i = nCount - 1; i >= 0; i --) {
		pWindow = m_paWindow->GetAt (i);
		if (pWindow->IsDelete ()) {
			Delete (pWindow->GetID ());
			bRet = TRUE;

		} else {
			bRet |= pWindow->TimerProc ();
		}
	}

	if (m_bDraw) {
		m_bDraw	= FALSE;
		bRet	= TRUE;
	}
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::KeyProc											 */
/* 内容		:キー処理														 */
/* 日付		:2005/05/06														 */
/* ========================================================================= */

void CMgrWindow::KeyProc(BYTE byEvent, BOOL bDown)
{
	int i, nCount;
	PCWindowBase pWindow;

	nCount = m_paWindow->GetSize ();
	for (i = nCount - 1; i >= 0; i --) {
		pWindow = m_paWindow->GetAt (i);
		if (pWindow->IsActive ()) {
			pWindow->KeyProc (byEvent, bDown);
			break;
		}
	}
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::DeleteAll											 */
/* 内容		:全てのウィンドウを破棄											 */
/* 日付		:2005/05/22														 */
/* ========================================================================= */

void CMgrWindow::DeleteAll(void)
{
	HWND hWndMain;
	PCWindowBase pTmp;

	pTmp = NULL;
	FreeInfo (m_paWindow, pTmp);
	m_bDraw		= TRUE;
	m_bKeyInput	= FALSE;

	if (m_pMgrData) {
		hWndMain = m_pMgrData->GetMainWindow ();
		SetFocus (hWndMain);
	}
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::Delete												 */
/* 内容		:指定IDのウィンドウを削除										 */
/* 日付		:2005/06/16														 */
/* ========================================================================= */

void CMgrWindow::Delete(int nID)
{
	int i, j, nCount, nCount2;
	PCWindowBase pWindow, pChild;

	nCount = m_paWindow->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pWindow = m_paWindow->GetAt (i);
		if (pWindow->GetID () == nID) {
			nCount2 = pWindow->m_apChild.GetSize ();
			for (j = 0; j < nCount2; j ++) {
				pChild = pWindow->m_apChild[j];
				pChild->m_bDelete = TRUE;
			}

			SAFE_DELETE (pWindow);
			m_paWindow->RemoveAt (i);
			SetActive ();
			m_bDraw = TRUE;
			m_pMgrKeyInput->Reset ();
			break;
		}
	}
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::GetWindow											 */
/* 内容		:指定IDのウィンドウを取得										 */
/* 日付		:2005/06/11														 */
/* ========================================================================= */

CWindowBase *CMgrWindow::GetWindow(int nID)
{
	int i, nCount;
	PCWindowBase pRet, pTmp;

	pRet	= NULL;
	nCount	= m_paWindow->GetSize ();

	for (i = 0; i < nCount; i ++) {
		pTmp = m_paWindow->GetAt (i);
		if (pTmp->GetID () == nID) {
			pRet = pTmp;
			break;
		}
	}

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::IsKeyInput											 */
/* 内容		:キー処理の必要があるか判定										 */
/* 日付		:2005/06/26														 */
/* ========================================================================= */

BOOL CMgrWindow::IsKeyInput(void)
{
	int i, nCount;
	BOOL bRet;
	PCWindowBase pTmp;

	bRet	= FALSE;
	nCount	= m_paWindow->GetSize ();

	if (nCount <= 0) {
		goto Exit;
	}

	for (i = 0; i < nCount; i ++) {
		pTmp = m_paWindow->GetAt (i);
		if (pTmp->IsActive ()) {
			bRet = TRUE;
			break;
		}
	}

Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowLOGIN									 */
/* 内容		:ログイン情報													 */
/* 日付		:2005/05/06														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowLOGIN(void)
{
	PCWindowLOGIN pWindowNew;

	pWindowNew = new CWindowLOGIN;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowMSG										 */
/* 内容		:メッセージ表示													 */
/* 日付		:2005/06/11														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowMSG(LPCSTR pszMsg, DWORD dwTime, int nType)
{
	PCWindowMSG pWindowNew;
	PCWindowBase pWindowTmp;

	pWindowTmp = GetWindow (WINDOWTYPE_MSG);
	if (pWindowTmp) {
		pWindowNew = (PCWindowMSG)pWindowTmp;

	} else {
		pWindowNew = new CWindowMSG;
		pWindowNew->Create (m_pMgrData);
		m_paWindow->Add (pWindowNew);
	}

	pWindowNew->SetMsg (pszMsg, dwTime, nType);
	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowLOGINMENU								 */
/* 内容		:メニュー														 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowLOGINMENU(void)
{
	PCWindowLOGINMENU pWindowNew;

	pWindowNew = new CWindowLOGINMENU;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowCHARNAME									 */
/* 内容		:キャラ名入力													 */
/* 日付		:2006/11/08														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowCHARNAME(void)
{
	PCWindowCHARNAME pWindowNew;

	pWindowNew = new CWindowCHARNAME;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowSEX										 */
/* 内容		:性別															 */
/* 日付		:2006/11/14														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowSEX(void)
{
	PCWindowSEX pWindowNew;

	pWindowNew = new CWindowSEX;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowACCOUNTINFO								 */
/* 内容		:アカウント情報													 */
/* 日付		:2006/12/31														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowACCOUNTINFO(void)
{
	PCWindowACCOUNTINFO pWindowNew;

	pWindowNew = new CWindowACCOUNTINFO;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowSTATUS									 */
/* 内容		:強さ															 */
/* 日付		:2005/06/26														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowSTATUS(BOOL bShow)
{
	PCWindowSTATUS pWindowNew;

	pWindowNew = new CWindowSTATUS;
	pWindowNew->Create (m_pMgrData);
	pWindowNew->SetShow (bShow);
	m_paWindow->Add (pWindowNew);
	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowCHAT										 */
/* 内容		:チャット入力													 */
/* 日付		:2007/02/03														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowCHAT(void)
{
	PCWindowCHAT pWindowNew;

	pWindowNew = new CWindowCHAT;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowFAMILYTYPE								 */
/* 内容		:種族選択														 */
/* 日付		:2007/04/07														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowFAMILYTYPE(void)
{
	PCWindowFAMILYTYPE pWindowNew;

	pWindowNew = new CWindowFAMILYTYPE;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowHAIRTYPE									 */
/* 内容		:髪型選択														 */
/* 日付		:2007/04/08														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowHAIRTYPE(void)
{
	PCWindowHAIRTYPE pWindowNew;

	pWindowNew = new CWindowHAIRTYPE;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowHAIRCOLOR								 */
/* 内容		:髪色選択														 */
/* 日付		:2007/04/08														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowHAIRCOLOR(void)
{
	PCWindowHAIRCOLOR pWindowNew;

	pWindowNew = new CWindowHAIRCOLOR;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowEYECOLOR									 */
/* 内容		:目色選択														 */
/* 日付		:2007/04/08														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowEYECOLOR(void)
{
	PCWindowEYECOLOR pWindowNew;

	pWindowNew = new CWindowEYECOLOR;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowSTYLESELECT								 */
/* 内容		:容姿選択														 */
/* 日付		:2007/04/10														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowSTYLESELECT(void)
{
	PCWindowSTYLESELECT pWindowNew;

	pWindowNew = new CWindowSTYLESELECT;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowNAMEINPUT								 */
/* 内容		:名前入力														 */
/* 日付		:2007/04/10														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowNAMEINPUT(void)
{
	PCWindowNAMEINPUT pWindowNew;

	pWindowNew = new CWindowNAMEINPUT;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowSYSTEMMENU								 */
/* 内容		:システムメニュー												 */
/* 日付		:2007/06/19														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowSYSTEMMENU(void)
{
	PCWindowSYSTEMMENU pWindowNew;

	pWindowNew = new CWindowSYSTEMMENU;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowSETCOLOR									 */
/* 内容		:名前と発言色の設定												 */
/* 日付		:2007/06/20														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowSETCOLOR(void)
{
	PCWindowSETCOLOR pWindowNew;

	pWindowNew = new CWindowSETCOLOR;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowSETBGMVOLUME								 */
/* 内容		:BGM音量の設定													 */
/* 日付		:2007/06/21														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowSETBGMVOLUME(void)
{
	PCWindowSETBGMVOLUME pWindowNew;

	pWindowNew = new CWindowSETBGMVOLUME;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowSETSOUNDVOLUME							 */
/* 内容		:効果音音量の設定												 */
/* 日付		:2007/06/21														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowSETSOUNDVOLUME(void)
{
	PCWindowSETSOUNDVOLUME pWindowNew;

	pWindowNew = new CWindowSETSOUNDVOLUME;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowSETDRAWMODE								 */
/* 内容		:表示内容の設定													 */
/* 日付		:2007/07/24														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowSETDRAWMODE(void)
{
	PCWindowSETDRAWMODE pWindowNew;

	pWindowNew = new CWindowSETDRAWMODE;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowITEMMENU									 */
/* 内容		:アイテムメニュー												 */
/* 日付		:2007/07/30														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowITEMMENU(void)
{
	PCWindowITEMMENU pWindowNew;

	pWindowNew = new CWindowITEMMENU;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowITEMMENU_SELECT							 */
/* 内容		:アイテムどうするかメニュー										 */
/* 日付		:2007/08/13														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowITEMMENU_SELECT(int nPos, DWORD dwItemID)
{
	PCWindowITEMMENU_SELECT pWindowNew;

	pWindowNew = new CWindowITEMMENU_SELECT;
	pWindowNew->Create (m_pMgrData);
	pWindowNew->SetItemID (nPos, dwItemID);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowOPTION									 */
/* 内容		:オプション														 */
/* 日付		:2008/06/21														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowOPTION(void)
{
	PCWindowOPTION pWindowNew;

	pWindowNew = new CWindowOPTION;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowOPTION_VIEWSET							 */
/* 内容		:オプション-表示設定											 */
/* 日付		:2008/06/27														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowOPTION_VIEWSET(void)
{
	PCWindowOPTION_VIEWSET pWindowNew;

	pWindowNew = new CWindowOPTION_VIEWSET;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowOPTION_VOLUMESET							 */
/* 内容		:オプション-音量設定											 */
/* 日付		:2008/06/28														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowOPTION_VOLUMESET(void)
{
	PCWindowOPTION_VOLUMESET pWindowNew;

	pWindowNew = new CWindowOPTION_VOLUMESET;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowOPTION_INPUTSET							 */
/* 内容		:オプション-入力設定											 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowOPTION_INPUTSET(void)
{
	PCWindowOPTION_INPUTSET pWindowNew;

	pWindowNew = new CWindowOPTION_INPUTSET;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowOPTION_TASKBAR							 */
/* 内容		:オプション(発言時にタスクバーチカチカ)							 */
/* 日付		:2008/06/21														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowOPTION_TASKBAR(void)
{
	PCWindowOPTION_TASKBAR pWindowNew;

	pWindowNew = new CWindowOPTION_TASKBAR;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowOPTION_INPUTSET_SETDEVICE				 */
/* 内容		:オプション-入力設定-入力デバイスの設定							 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowOPTION_INPUTSET_SETDEVICE(void)
{
	PCWindowOPTION_INPUTSET_SETDEVICE pWindowNew;

	pWindowNew = new CWindowOPTION_INPUTSET_SETDEVICE;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowOPTION_ACTIONSET							 */
/* 内容		:オプション-動作設定											 */
/* 日付		:2008/07/21														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowOPTION_ACTIONSET(void)
{
	PCWindowOPTION_ACTIONSET pWindowNew;

	pWindowNew = new CWindowOPTION_ACTIONSET;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowOPTION_ACTIONSET_SLEEPTIMER				 */
/* 内容		:オプション-動作設定-おひるねタイマーの設定						 */
/* 日付		:2008/07/21														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowOPTION_ACTIONSET_SLEEPTIMER(void)
{
	PCWindowOPTION_ACTIONSET_SLEEPTIMER pWindowNew;

	pWindowNew = new CWindowOPTION_ACTIONSET_SLEEPTIMER;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowCOMMANDMENU								 */
/* 内容		:コマンドメニュー												 */
/* 日付		:2008/08/02														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowCOMMANDMENU(void)
{
	PCWindowCOMMANDMENU pWindowNew;

	pWindowNew = new CWindowCOMMANDMENU;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::MakeWindowCHAR_STATUS								 */
/* 内容		:キャラ-ステータス												 */
/* 日付		:2008/10/27														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowCHAR_STATUS(void)
{
	PCWindowCHAR_STATUS pWindowNew;
	PCWindowCHAR_STATUS2 pWindowNew2;
	PCWindowCHAR_STATUS3 pWindowNew3;

	pWindowNew2 = new CWindowCHAR_STATUS2;
	pWindowNew2->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew2);
	pWindowNew3 = new CWindowCHAR_STATUS3;
	pWindowNew3->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew3);
	pWindowNew = new CWindowCHAR_STATUS;
	pWindowNew->Create (m_pMgrData);
	pWindowNew->AddChild (pWindowNew2);
	pWindowNew->AddChild (pWindowNew3);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMgrWindow::SetActive											 */
/* 内容		:アクティブなウィンドウの設定									 */
/* 日付		:2005/06/16														 */
/* ========================================================================= */

void CMgrWindow::SetActive(void)
{
	int i, nCount;
	BOOL bSet;
	PCWindowBase pWindow;

	bSet		= FALSE;
	m_bKeyInput	= FALSE;
	nCount		= m_paWindow->GetSize ();
	if (nCount <= 0) {
		return;
	}

	for (i = nCount - 1; i >= 0; i --) {
		pWindow = m_paWindow->GetAt (i);
		if (pWindow->IsInput () == FALSE) {
			continue;
		}
		pWindow->SetActive (1 - bSet);
		bSet = TRUE;
	}

	m_bKeyInput = bSet;
}

/* Copyright(C)URARA-works 2006 */
