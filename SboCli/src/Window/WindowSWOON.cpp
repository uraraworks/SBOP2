/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:WindowSWOON.cpp											 */
/* 内容			:気絶メニューウィンドウクラス 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/02													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowSWOON.h"


/* ========================================================================= */
/* 関数名	:CWindowSWOON::CWindowSWOON										 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/02														 */
/* ========================================================================= */

CWindowSWOON::CWindowSWOON()
{
	m_nPosMax		= 1;
	m_bInput		= TRUE;
	m_nID			= WINDOWTYPE_SWOON;
	m_sizeWindow.cx	= 16 * 2 + 16 * 12;
	m_sizeWindow.cy	= 16 * 2 + 16 * 2;

	m_ptViewPos.x	= SCRSIZEX / 2 - m_sizeWindow.cx / 2;
	m_ptViewPos.y	= SCRSIZEY / 2 - m_sizeWindow.cy / 2 - m_sizeWindow.cy - 32;
}


/* ========================================================================= */
/* 関数名	:CWindowSWOON::~CWindowSWOON									 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/02														 */
/* ========================================================================= */

CWindowSWOON::~CWindowSWOON()
{
}


/* ========================================================================= */
/* 関数名	:CWindowSWOON::Create											 */
/* 内容		:作成															 */
/* 日付		:2008/12/02														 */
/* ========================================================================= */

void CWindowSWOON::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_bActive = TRUE;
	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);
}


/* ========================================================================= */
/* 関数名	:CWindowSWOON::Draw												 */
/* 内容		:描画															 */
/* 日付		:2008/12/02														 */
/* ========================================================================= */

void CWindowSWOON::Draw(PCImg32 pDst)
{
	int nLevel;
	HDC hDC;
	HFONT hFontOld;
	COLORREF clText;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame ();

	clText		= RGB (1, 1, 1);
	hDC			= m_pDib->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont);
	SetBkMode (hDC, TRANSPARENT);

	clText = RGB (1, 1, 1);
	TextOut2 (hDC, 16 + 16, 16 + 16 * 0, "この場で助けを待つ",		clText);
	TextOut2 (hDC, 16 + 15, 16 + 16 * 1, "記録した場所で復活する",	clText);

	SelectObject (hDC, hFontOld);
	m_pDib->Unlock ();

	DrawCursor (8, 16 + 16 * m_nPos);
	m_dwTimeDrawStart = timeGetTime ();

Exit:
	nLevel = 100;
	if (m_bActive == FALSE) {
		nLevel = 60;
	}
	pDst->BltLevel (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, nLevel, TRUE);
}


/* ========================================================================= */
/* 関数名	:CWindowSWOON::OnUp												 */
/* 内容		:キーハンドラ(↑)												 */
/* 日付		:2008/12/02														 */
/* ========================================================================= */

BOOL CWindowSWOON::OnUp(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos <= 0) {
		goto Exit;
	}
	m_nPos --;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWindowSWOON::OnDown											 */
/* 内容		:キーハンドラ(↓)												 */
/* 日付		:2008/12/02														 */
/* ========================================================================= */

BOOL CWindowSWOON::OnDown(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos >= m_nPosMax) {
		goto Exit;
	}
	m_nPos ++;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWindowSWOON::OnLeft											 */
/* 内容		:キーハンドラ(←)												 */
/* 日付		:2008/12/02														 */
/* ========================================================================= */

BOOL CWindowSWOON::OnLeft(void)
{
	m_nPos = 0;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CWindowSWOON::OnRight											 */
/* 内容		:キーハンドラ(→)												 */
/* 日付		:2008/12/02														 */
/* ========================================================================= */

BOOL CWindowSWOON::OnRight(void)
{
	m_nPos = m_nPosMax;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CWindowSWOON::OnX												 */
/* 内容		:キーハンドラ(X)												 */
/* 日付		:2008/12/02														 */
/* ========================================================================= */

BOOL CWindowSWOON::OnX(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_pMgrSound->PlaySound (SOUNDID_OK_PI73);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, m_nPos);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWindowSWOON::OnZ												 */
/* 内容		:キーハンドラ(Z)												 */
/* 日付		:2008/12/02														 */
/* ========================================================================= */

BOOL CWindowSWOON::OnZ(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_bDelete = TRUE;
	m_pMgrSound->PlaySound (SOUNDID_CANCEL);

	bRet = TRUE;
Exit:
	return bRet;
}

/* Copyright(C)URARA-works 2008 */
