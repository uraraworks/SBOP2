/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:WindowOPTION_INPUTSET.cpp									 */
/* 内容			:オプション-入力設定ウィンドウクラス 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/07/12													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowOPTION_INPUTSET.h"


/* ========================================================================= */
/* 関数名	:CWindowOPTION_INPUTSET::CWindowOPTION_INPUTSET					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

CWindowOPTION_INPUTSET::CWindowOPTION_INPUTSET()
{
	m_nPosMax		= 0;
	m_bInput		= TRUE;
	m_nID			= WINDOWTYPE_OPTION_INPUTSET;
	m_ptViewPos.x	= 8 * 13;
	m_ptViewPos.y	= 16 * 10;
	m_sizeWindow.cx	= 16 * 2 + 16 * 16 + 8;
	m_sizeWindow.cy	= 16 * 2 + 16 * 1;
}


/* ========================================================================= */
/* 関数名	:CWindowOPTION_INPUTSET::~CWindowOPTION_INPUTSET				 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

CWindowOPTION_INPUTSET::~CWindowOPTION_INPUTSET()
{
}


/* ========================================================================= */
/* 関数名	:CWindowOPTION_INPUTSET::Create									 */
/* 内容		:作成															 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

void CWindowOPTION_INPUTSET::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_bActive = TRUE;
	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);
	m_nPos = 0;
}


/* ========================================================================= */
/* 関数名	:CWindowOPTION_INPUTSET::Draw									 */
/* 内容		:描画															 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

void CWindowOPTION_INPUTSET::Draw(PCImg32 pDst)
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

	TextOut2 (hDC, 32, 16 + 16 * 0, "使用するジョイパッドの設定", clText);

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
/* 関数名	:CWindowOPTION_INPUTSET::OnUp									 */
/* 内容		:キーハンドラ(↑)												 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

BOOL CWindowOPTION_INPUTSET::OnUp(void)
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
/* 関数名	:CWindowOPTION_INPUTSET::OnDown									 */
/* 内容		:キーハンドラ(↓)												 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

BOOL CWindowOPTION_INPUTSET::OnDown(void)
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
/* 関数名	:CWindowOPTION_INPUTSET::OnLeft									 */
/* 内容		:キーハンドラ(←)												 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

BOOL CWindowOPTION_INPUTSET::OnLeft(void)
{
	m_nPos = 0;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CWindowOPTION_INPUTSET::OnRight								 */
/* 内容		:キーハンドラ(→)												 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

BOOL CWindowOPTION_INPUTSET::OnRight(void)
{
	m_nPos = m_nPosMax;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CWindowOPTION_INPUTSET::OnX									 */
/* 内容		:キーハンドラ(X)												 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

BOOL CWindowOPTION_INPUTSET::OnX(BOOL bDown)
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
/* 関数名	:CWindowOPTION_INPUTSET::OnZ									 */
/* 内容		:キーハンドラ(Z)												 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

BOOL CWindowOPTION_INPUTSET::OnZ(BOOL bDown)
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
