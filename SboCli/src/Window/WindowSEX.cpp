/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:WindowSEX.cpp												 */
/* 内容			:性別ウィンドウクラス 実装ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/14													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowSEX.h"

/* ========================================================================= */
/* 関数名	:CWindowSEX::CWindowSEX											 */
/* 内容		:コンストラクタ													 */
/* 日付		:2005/06/22														 */
/* ========================================================================= */

CWindowSEX::CWindowSEX()
{
	m_nPos			= 0;
	m_nPosMax		= 1;
	m_bInput		= TRUE;
	m_nID			= WINDOWTYPE_SEX;
	m_ptViewPos.x	= 88;
	m_ptViewPos.y	= 160;
	m_sizeWindow.cx	= 16 * 2 + 16 * 6;
	m_sizeWindow.cy	= 16 * 2 + 8 * 7;
}


/* ========================================================================= */
/* 関数名	:CWindowSEX::~CWindowSEX										 */
/* 内容		:デストラクタ													 */
/* 日付		:2005/06/22														 */
/* ========================================================================= */

CWindowSEX::~CWindowSEX()
{
}


/* ========================================================================= */
/* 関数名	:CWindowSEX::Create												 */
/* 内容		:作成															 */
/* 日付		:2006/11/14														 */
/* ========================================================================= */

void CWindowSEX::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);

	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, 1);
}


/* ========================================================================= */
/* 関数名	:CWindowSEX::Draw												 */
/* 内容		:描画															 */
/* 日付		:2005/06/22														 */
/* ========================================================================= */

void CWindowSEX::Draw(PCImg32 pDst)
{
	HDC hDC;
	HFONT hFontOld;
	COLORREF clText;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame ();

	clText		= RGB (255, 127, 53);
	hDC			= m_pDib->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont14);
	SetBkMode (hDC, TRANSPARENT);

	TextOut4 (hDC, 48, 8, "性別",	clText);
	TextOut2 (hDC, 40, 16 + 16 * 1,		"♂ 男の子",	clText);
	TextOut2 (hDC, 40, 16 + 16 * 2 + 8,	"♀ 女の子",	clText);

	SelectObject (hDC, hFontOld);
	m_pDib->Unlock ();

	DrawCursor (8, 32 + 24 * m_nPos);
	m_dwTimeDrawStart = timeGetTime ();

Exit:
	pDst->Blt (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}


/* ========================================================================= */
/* 関数名	:CWindowSEX::TimerProc											 */
/* 内容		:時間処理														 */
/* 日付		:2005/06/22														 */
/* ========================================================================= */

BOOL CWindowSEX::TimerProc(void)
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
/* 関数名	:CWindowSEX::SetType											 */
/* 内容		:性別を設定														 */
/* 日付		:2007/04/18														 */
/* ========================================================================= */

void CWindowSEX::SetType(int nSex)
{
	if (nSex == SEX_FEMALE) {
		m_nPos = 1;
	} else {
		m_nPos = 0;
	}
	Redraw ();
}


/* ========================================================================= */
/* 関数名	:CWindowSEX::GetType											 */
/* 内容		:性別を取得														 */
/* 日付		:2005/06/23														 */
/* ========================================================================= */

int CWindowSEX::GetType(void)
{
	int nRet;

	nRet = SEX_MALE;
	if (m_nPos == 1) {
		nRet = SEX_FEMALE;
	}

	return nRet;
}


/* ========================================================================= */
/* 関数名	:CWindowSEX::OnUp												 */
/* 内容		:キーハンドラ(↑)												 */
/* 日付		:2005/06/17														 */
/* ========================================================================= */

BOOL CWindowSEX::OnUp(void)
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
/* 関数名	:CWindowSEX::OnDown												 */
/* 内容		:キーハンドラ(↓)												 */
/* 日付		:2005/06/17														 */
/* ========================================================================= */

BOOL CWindowSEX::OnDown(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos >= m_nPosMax) {
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
/* 関数名	:CWindowSEX::OnLeft												 */
/* 内容		:キーハンドラ(←)												 */
/* 日付		:2005/06/17														 */
/* ========================================================================= */

BOOL CWindowSEX::OnLeft(void)
{
	m_nPos = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, 1);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CWindowSEX::OnRight											 */
/* 内容		:キーハンドラ(→)												 */
/* 日付		:2005/06/17														 */
/* ========================================================================= */

BOOL CWindowSEX::OnRight(void)
{
	m_nPos = m_nPosMax;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, 1);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CWindowSEX::OnX												 */
/* 内容		:キーハンドラ(X)												 */
/* 日付		:2005/06/21														 */
/* ========================================================================= */

BOOL CWindowSEX::OnX(BOOL bDown)
{
	BOOL bRet;
	PCMgrWindow pMgrWindow;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	pMgrWindow = m_pMgrData->GetMgrWindow ();

	m_pMgrSound->PlaySound (SOUNDID_OK_PI73);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, 0);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWindowSEX::OnZ												 */
/* 内容		:キーハンドラ(Z)												 */
/* 日付		:2005/06/22														 */
/* ========================================================================= */

BOOL CWindowSEX::OnZ(BOOL bDown)
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

/* Copyright(C)URARA-works 2006 */
