/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:WindowSETCOLOR.cpp											 */
/* 内容			:名前と発言色の設定ウィンドウクラス 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/06/20													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowSETCOLOR.h"


/* ========================================================================= */
/* 関数名	:CWindowSETCOLOR::CWindowSETCOLOR								 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/06/20														 */
/* ========================================================================= */

CWindowSETCOLOR::CWindowSETCOLOR()
{
	m_nPosMax		= 6;
	m_bInput		= TRUE;
	m_nID			= WINDOWTYPE_SETCOLOR;
	m_ptViewPos.x	= 80;
	m_ptViewPos.y	= 80;
	m_sizeWindow.cx	= 16 * 2 + 16 * 4;
	m_sizeWindow.cy	= 16 * 2 + 16 * 7;
}


/* ========================================================================= */
/* 関数名	:CWindowSETCOLOR::~CWindowSETCOLOR								 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/06/20														 */
/* ========================================================================= */

CWindowSETCOLOR::~CWindowSETCOLOR()
{
}


/* ========================================================================= */
/* 関数名	:CWindowSETCOLOR::Create										 */
/* 内容		:作成															 */
/* 日付		:2007/06/20														 */
/* ========================================================================= */

void CWindowSETCOLOR::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_bActive = TRUE;
	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);
}


/* ========================================================================= */
/* 関数名	:CWindowSETCOLOR::Draw											 */
/* 内容		:描画															 */
/* 日付		:2007/06/20														 */
/* ========================================================================= */

void CWindowSETCOLOR::Draw(PCImg32 pDst)
{
	int nLevel;
	HDC hDC;
	HFONT hFontOld;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame ();

	hDC			= m_pDib->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont);
	SetBkMode (hDC, TRANSPARENT);

	TextOut2 (hDC, 32, 16 + 16 * 0, _T("白"),	RGB (255, 255, 255), TRUE);
	TextOut2 (hDC, 32, 16 + 16 * 1, _T("赤"),	RGB (255, 200, 200), TRUE);
	TextOut2 (hDC, 32, 16 + 16 * 2, _T("黄"),	RGB (255, 255, 200), TRUE);
	TextOut2 (hDC, 32, 16 + 16 * 3, _T("緑"),	RGB (200, 255, 200), TRUE);
	TextOut2 (hDC, 32, 16 + 16 * 4, _T("青緑"),	RGB (200, 255, 255), TRUE);
	TextOut2 (hDC, 32, 16 + 16 * 5, _T("青"),	RGB (200, 200, 255), TRUE);
	TextOut2 (hDC, 32, 16 + 16 * 6, _T("紫"),	RGB (255, 200, 255), TRUE);

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
/* 関数名	:CWindowSETCOLOR::OnUp											 */
/* 内容		:キーハンドラ(↑)												 */
/* 日付		:2007/06/20														 */
/* ========================================================================= */

BOOL CWindowSETCOLOR::OnUp(void)
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
/* 関数名	:CWindowSETCOLOR::OnDown										 */
/* 内容		:キーハンドラ(↓)												 */
/* 日付		:2007/06/20														 */
/* ========================================================================= */

BOOL CWindowSETCOLOR::OnDown(void)
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
/* 関数名	:CWindowSETCOLOR::OnLeft										 */
/* 内容		:キーハンドラ(←)												 */
/* 日付		:2007/06/20														 */
/* ========================================================================= */

BOOL CWindowSETCOLOR::OnLeft(void)
{
	m_nPos = 0;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CWindowSETCOLOR::OnRight										 */
/* 内容		:キーハンドラ(→)												 */
/* 日付		:2007/06/20														 */
/* ========================================================================= */

BOOL CWindowSETCOLOR::OnRight(void)
{
	m_nPos = m_nPosMax;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CWindowSETCOLOR::OnX											 */
/* 内容		:キーハンドラ(X)												 */
/* 日付		:2007/06/20														 */
/* ========================================================================= */

BOOL CWindowSETCOLOR::OnX(BOOL bDown)
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
/* 関数名	:CWindowSETCOLOR::OnZ											 */
/* 内容		:キーハンドラ(Z)												 */
/* 日付		:2007/06/20														 */
/* ========================================================================= */

BOOL CWindowSETCOLOR::OnZ(BOOL bDown)
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

/* Copyright(C)URARA-works 2007 */
