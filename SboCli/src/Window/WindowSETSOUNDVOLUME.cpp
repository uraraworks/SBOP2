/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:WindowSETSOUNDVOLUME.cpp									 */
/* 内容			:効果音音量の設定ウィンドウクラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/06/21													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowSETSOUNDVOLUME.h"


/* ========================================================================= */
/* 関数名	:CWindowSETSOUNDVOLUME::CWindowSETSOUNDVOLUME					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/06/21														 */
/* ========================================================================= */

CWindowSETSOUNDVOLUME::CWindowSETSOUNDVOLUME()
{
	m_nPosMax		= 4;
	m_bInput		= TRUE;
	m_nID			= WINDOWTYPE_SETSOUNDVOLUME;
	m_ptViewPos.x	= 80;
	m_ptViewPos.y	= 112;
	m_sizeWindow.cx	= 16 * 2 + 16 * 4;
	m_sizeWindow.cy	= 16 * 2 + 16 * 5;
}


/* ========================================================================= */
/* 関数名	:CWindowSETSOUNDVOLUME::~CWindowSETSOUNDVOLUME					 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/06/21														 */
/* ========================================================================= */

CWindowSETSOUNDVOLUME::~CWindowSETSOUNDVOLUME()
{
}


/* ========================================================================= */
/* 関数名	:CWindowSETSOUNDVOLUME::Create									 */
/* 内容		:作成															 */
/* 日付		:2007/06/21														 */
/* ========================================================================= */

void CWindowSETSOUNDVOLUME::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_bActive = TRUE;
	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);

	m_nPos = m_pMgrData->GetSEVolume ();
}


/* ========================================================================= */
/* 関数名	:CWindowSETSOUNDVOLUME::Draw									 */
/* 内容		:描画															 */
/* 日付		:2007/06/21														 */
/* ========================================================================= */

void CWindowSETSOUNDVOLUME::Draw(PCImg32 pDst)
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
	TextOut2 (hDC, 32, 16 + 16 * 0, "無し",	clText);
	TextOut2 (hDC, 32, 16 + 16 * 1, "１",	clText);
	TextOut2 (hDC, 32, 16 + 16 * 2, "２",	clText);
	TextOut2 (hDC, 32, 16 + 16 * 3, "３",	clText);
	TextOut2 (hDC, 32, 16 + 16 * 4, "４",	clText);

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
/* 関数名	:CWindowSETSOUNDVOLUME::OnUp									 */
/* 内容		:キーハンドラ(↑)												 */
/* 日付		:2007/06/21														 */
/* ========================================================================= */

BOOL CWindowSETSOUNDVOLUME::OnUp(void)
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
/* 関数名	:CWindowSETSOUNDVOLUME::OnDown									 */
/* 内容		:キーハンドラ(↓)												 */
/* 日付		:2007/06/21														 */
/* ========================================================================= */

BOOL CWindowSETSOUNDVOLUME::OnDown(void)
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
/* 関数名	:CWindowSETSOUNDVOLUME::OnLeft									 */
/* 内容		:キーハンドラ(←)												 */
/* 日付		:2007/06/21														 */
/* ========================================================================= */

BOOL CWindowSETSOUNDVOLUME::OnLeft(void)
{
	m_nPos = 0;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CWindowSETSOUNDVOLUME::OnRight									 */
/* 内容		:キーハンドラ(→)												 */
/* 日付		:2007/06/21														 */
/* ========================================================================= */

BOOL CWindowSETSOUNDVOLUME::OnRight(void)
{
	m_nPos = m_nPosMax;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CWindowSETSOUNDVOLUME::OnX										 */
/* 内容		:キーハンドラ(X)												 */
/* 日付		:2007/06/21														 */
/* ========================================================================= */

BOOL CWindowSETSOUNDVOLUME::OnX(BOOL bDown)
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
/* 関数名	:CWindowSETSOUNDVOLUME::OnZ										 */
/* 内容		:キーハンドラ(Z)												 */
/* 日付		:2007/06/21														 */
/* ========================================================================= */

BOOL CWindowSETSOUNDVOLUME::OnZ(BOOL bDown)
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
