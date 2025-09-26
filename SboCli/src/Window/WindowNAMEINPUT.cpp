/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:WindowNAMEINPUT.cpp										 */
/* 内容			:名前入力ウィンドウクラス 実装ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/04/10													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoCharBase.h"
#include "LibInfoCharCli.h"
#include "InfoCharCli.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "MgrDraw.h"
#include "WindowCHARNAME.h"
#include "WindowNAMEINPUT.h"

/* ========================================================================= */
/* 関数名	:CWindowNAMEINPUT::CWindowNAMEINPUT								 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/04/10														 */
/* ========================================================================= */

CWindowNAMEINPUT::CWindowNAMEINPUT()
{
	m_nPos			= 0;
	m_nPosMax		= 2;
	m_bInput		= TRUE;
	m_nID			= WINDOWTYPE_NAMEINPUT;
	m_ptViewPos.x	= 80;
	m_ptViewPos.y	= 80;
	m_sizeWindow.cx	= 320;
	m_sizeWindow.cy	= 320;

	m_pMgrWindow		= NULL;
	m_pMgrDraw			= NULL;
	m_pWindowCHARNAME	= NULL;
	m_pInfoCharCli		= new CInfoCharCli;
}


/* ========================================================================= */
/* 関数名	:CWindowNAMEINPUT::~CWindowNAMEINPUT							 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/04/10														 */
/* ========================================================================= */

CWindowNAMEINPUT::~CWindowNAMEINPUT()
{
	SAFE_DELETE (m_pInfoCharCli);
}


/* ========================================================================= */
/* 関数名	:CWindowNAMEINPUT::Create										 */
/* 内容		:作成															 */
/* 日付		:2007/04/10														 */
/* ========================================================================= */

void CWindowNAMEINPUT::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_pMgrWindow	= m_pMgrData->GetMgrWindow ();
	m_pMgrDraw		= m_pMgrData->GetMgrDraw ();

	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);

	m_pInfoCharCli->Create (m_pMgrData);
	m_pInfoCharCli->m_dwMotionTypeID = 2;
	m_pInfoCharCli->MakeCharGrp ();
	m_pMgrData->GetLibInfoChar ()->RenewMotionInfo (m_pInfoCharCli);

	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, 1);
}


/* ========================================================================= */
/* 関数名	:CWindowNAMEINPUT::OnWindowMsg									 */
/* 内容		:メッセージハンドラ(WM_WINDOWMSG)								 */
/* 日付		:2007/04/11														 */
/* ========================================================================= */

void CWindowNAMEINPUT::OnWindowMsg(int nType, DWORD dwPara)
{
	switch (nType) {
	case WINDOWTYPE_CHARNAME:		/* キャラ名入力 */
		m_pInfoCharCli->m_strCharName = m_pWindowCHARNAME->m_strName;
		m_pMgrWindow->Delete (WINDOWTYPE_CHARNAME);
		m_pWindowCHARNAME = NULL;
		Redraw ();
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CWindowNAMEINPUT::Draw											 */
/* 内容		:描画															 */
/* 日付		:2007/04/10														 */
/* ========================================================================= */

void CWindowNAMEINPUT::Draw(PCImg32 pDst)
{
	HDC hDC;
	HFONT hFontOld;
	COLORREF clText;
	POINT nCursorPos[] = {
		108, 240,
		132, 292
	};

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame (2);
	DrawFrame (112, 96, 100, 104, 2);
	DrawFrame2 (108, 240, 16 * 7, 8, 2);

	clText		= RGB (124, 123, 232);
	hDC			= m_pDib->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont14);
	SetBkMode (hDC, TRANSPARENT);

	TextOut4 (hDC, 88,	24,		_T("新規キャラクター作成"), clText);
	TextOut4 (hDC, 136,	48,		_T("名前入力"), clText);
	TextOut4 (hDC, 112,	216,	_T("キャラクター名"), clText);
	TextOut4 (hDC, 148,	288,	_T("登録"), clText);
	TextOut4 (hDC, 256,	288,	_T("３／３"), clText);
	TextOut2 (hDC, 108, 240 - 2, m_pInfoCharCli->m_strCharName, clText);

	SelectObject (hDC, hFontOld);
	m_pDib->Unlock ();

	DrawCursor (nCursorPos[m_nPos].x - 32, nCursorPos[m_nPos].y - 6);
	m_dwTimeDrawStart = timeGetTime ();

	m_pMgrDraw->DrawChar (m_pDib, 130, 124, m_pInfoCharCli);

Exit:
	pDst->Blt (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}


/* ========================================================================= */
/* 関数名	:CWindowNAMEINPUT::TimerProc									 */
/* 内容		:時間処理														 */
/* 日付		:2007/04/10														 */
/* ========================================================================= */

BOOL CWindowNAMEINPUT::TimerProc(void)
{
	BOOL bRet;
	int nCursorAnimeBack;

	bRet = FALSE;

	nCursorAnimeBack = m_nCursorAnime;
	CWindowBase::TimerProc ();
	if (nCursorAnimeBack == m_nCursorAnime) {
		goto Exit;
	}

	m_dwTimeDrawStart = 0;
	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWindowNAMEINPUT::OnUp											 */
/* 内容		:キーハンドラ(↑)												 */
/* 日付		:2007/04/10														 */
/* ========================================================================= */

BOOL CWindowNAMEINPUT::OnUp(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos <= 0) {
		goto Exit;
	}
	m_nPos --;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, 1);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWindowNAMEINPUT::OnDown										 */
/* 内容		:キーハンドラ(↓)												 */
/* 日付		:2007/04/10														 */
/* ========================================================================= */

BOOL CWindowNAMEINPUT::OnDown(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos >= m_nPosMax - 1) {
		goto Exit;
	}
	m_nPos ++;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, 1);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWindowNAMEINPUT::OnLeft										 */
/* 内容		:キーハンドラ(←)												 */
/* 日付		:2007/04/10														 */
/* ========================================================================= */

BOOL CWindowNAMEINPUT::OnLeft(void)
{
	m_nPos = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, 1);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CWindowNAMEINPUT::OnRight										 */
/* 内容		:キーハンドラ(→)												 */
/* 日付		:2007/04/10														 */
/* ========================================================================= */

BOOL CWindowNAMEINPUT::OnRight(void)
{
	m_nPos = m_nPosMax - 1;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, 1);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CWindowNAMEINPUT::OnX											 */
/* 内容		:キーハンドラ(X)												 */
/* 日付		:2007/04/10														 */
/* ========================================================================= */

BOOL CWindowNAMEINPUT::OnX(BOOL bDown)
{
	BOOL bRet, bResult;
	CLibInfoCharBase LibInfoChar;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	switch (m_nPos) {
	case 0:	/* 名前 */
		m_pMgrWindow->MakeWindowCHARNAME ();
		m_pWindowCHARNAME = (PCWindowCHARNAME)m_pMgrWindow->GetWindow (WINDOWTYPE_CHARNAME);
		break;
	case 1:	/* 登録 */
		if (m_pInfoCharCli->m_strCharName.IsEmpty ()) {
			m_pMgrWindow->MakeWindowMSG ("名前を入力してください", 3000);
			break;
		}
		bResult = LibInfoChar.NameCheck (m_pInfoCharCli->m_strCharName);
		if (bResult == FALSE) {
			m_pMgrWindow->MakeWindowMSG ("名前に空白は使えません", 3000);
			break;
		}
		PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, 0);
		break;
	}
	m_pMgrSound->PlaySound (SOUNDID_OK_PI73);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWindowNAMEINPUT::OnZ											 */
/* 内容		:キーハンドラ(Z)												 */
/* 日付		:2007/04/10														 */
/* ========================================================================= */

BOOL CWindowNAMEINPUT::OnZ(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_pMgrSound->PlaySound (SOUNDID_CANCEL);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, -1);
	m_bDelete = TRUE;

	bRet = TRUE;
Exit:
	return bRet;
}

/* Copyright(C)URARA-works 2007 */
