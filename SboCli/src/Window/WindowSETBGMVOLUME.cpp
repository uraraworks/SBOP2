/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:WindowSETBGMVOLUME.cpp										 */
/* 内容			:BGM音量の設定ウィンドウクラス 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/06/21													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowSETBGMVOLUME.h"


/* ========================================================================= */
/* 関数名	:CWindowSETBGMVOLUME::CWindowSETBGMVOLUME						 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/06/21														 */
/* ========================================================================= */

CWindowSETBGMVOLUME::CWindowSETBGMVOLUME()
{
	m_nPosMax		= 4;
	m_bInput		= TRUE;
	m_nID			= WINDOWTYPE_SETBGMVOLUME;
	m_ptViewPos.x	= 80;
	m_ptViewPos.y	= 96;
	m_sizeWindow.cx	= 16 * 2 + 16 * 4;
	m_sizeWindow.cy	= 16 * 2 + 16 * 5;
}


/* ========================================================================= */
/* 関数名	:CWindowSETBGMVOLUME::~CWindowSETBGMVOLUME						 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/06/21														 */
/* ========================================================================= */

CWindowSETBGMVOLUME::~CWindowSETBGMVOLUME()
{
}


/* ========================================================================= */
/* 関数名	:CWindowSETBGMVOLUME::Create									 */
/* 内容		:作成															 */
/* 日付		:2007/06/21														 */
/* ========================================================================= */

void CWindowSETBGMVOLUME::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_bActive = TRUE;
	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);

	m_nPos = m_pMgrData->GetBGMVolume ();
}


/* ========================================================================= */
/* 関数名	:CWindowSETBGMVOLUME::Draw										 */
/* 内容		:描画															 */
/* 日付		:2007/06/21														 */
/* ========================================================================= */

void CWindowSETBGMVOLUME::Draw(PCImg32 pDst)
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
	TextOut2 (hDC, 32, 16 + 16 * 0, _T("無し"),	clText);
	TextOut2 (hDC, 32, 16 + 16 * 1, _T("１"),	clText);
	TextOut2 (hDC, 32, 16 + 16 * 2, _T("２"),	clText);
	TextOut2 (hDC, 32, 16 + 16 * 3, _T("３"),	clText);
	TextOut2 (hDC, 32, 16 + 16 * 4, _T("４"),	clText);

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
/* 関数名	:CWindowSETBGMVOLUME::OnUp										 */
/* 内容		:キーハンドラ(↑)												 */
/* 日付		:2007/06/21														 */
/* ========================================================================= */

BOOL CWindowSETBGMVOLUME::OnUp(void)
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
/* 関数名	:CWindowSETBGMVOLUME::OnDown									 */
/* 内容		:キーハンドラ(↓)												 */
/* 日付		:2007/06/21														 */
/* ========================================================================= */

BOOL CWindowSETBGMVOLUME::OnDown(void)
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
/* 関数名	:CWindowSETBGMVOLUME::OnLeft									 */
/* 内容		:キーハンドラ(←)												 */
/* 日付		:2007/06/21														 */
/* ========================================================================= */

BOOL CWindowSETBGMVOLUME::OnLeft(void)
{
	m_nPos = 0;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CWindowSETBGMVOLUME::OnRight									 */
/* 内容		:キーハンドラ(→)												 */
/* 日付		:2007/06/21														 */
/* ========================================================================= */

BOOL CWindowSETBGMVOLUME::OnRight(void)
{
	m_nPos = m_nPosMax;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CWindowSETBGMVOLUME::OnX										 */
/* 内容		:キーハンドラ(X)												 */
/* 日付		:2007/06/21														 */
/* ========================================================================= */

BOOL CWindowSETBGMVOLUME::OnX(BOOL bDown)
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
/* 関数名	:CWindowSETBGMVOLUME::OnZ										 */
/* 内容		:キーハンドラ(Z)												 */
/* 日付		:2007/06/21														 */
/* ========================================================================= */

BOOL CWindowSETBGMVOLUME::OnZ(BOOL bDown)
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
