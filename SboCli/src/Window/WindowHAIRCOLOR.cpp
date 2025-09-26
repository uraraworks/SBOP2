/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:WindowHAIRCOLOR.cpp										 */
/* 内容			:髪色選択ウィンドウクラス 実装ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/04/08													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowHAIRCOLOR.h"

/* ========================================================================= */
/* 関数名	:CWindowHAIRCOLOR::CWindowHAIRCOLOR								 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/04/08														 */
/* ========================================================================= */

CWindowHAIRCOLOR::CWindowHAIRCOLOR()
{
	m_nPos			= 1;
	m_nPosMax		= 13;
	m_nPosX			= 1;
	m_bInput		= TRUE;
	m_nID			= WINDOWTYPE_HAIRCOLOR;
	m_ptViewPos.x	= 88;
	m_ptViewPos.y	= 160;
	m_sizeWindow.cx	= 16 * 2 + 16 * 8;
	m_sizeWindow.cy	= 16 * 2 + 16 * 2;
}


/* ========================================================================= */
/* 関数名	:CWindowHAIRCOLOR::~CWindowHAIRCOLOR							 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/04/08														 */
/* ========================================================================= */

CWindowHAIRCOLOR::~CWindowHAIRCOLOR()
{
}


/* ========================================================================= */
/* 関数名	:CWindowHAIRCOLOR::Create										 */
/* 内容		:作成															 */
/* 日付		:2007/04/08														 */
/* ========================================================================= */

void CWindowHAIRCOLOR::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);

	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, 1);
}


/* ========================================================================= */
/* 関数名	:CWindowHAIRCOLOR::Draw											 */
/* 内容		:描画															 */
/* 日付		:2007/04/08														 */
/* ========================================================================= */

void CWindowHAIRCOLOR::Draw(PCImg32 pDst)
{
	HDC hDC;
	HFONT hFontOld;
	COLORREF clText;
	CmyString strTmp;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame ();

	clText		= RGB (255, 127, 53);
	hDC			= m_pDib->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont14);
	SetBkMode (hDC, TRANSPARENT);

	TextOut4 (hDC, 56, 8, _T("髪の色"), clText);
	strTmp.Format(_T("%02d"), m_nPos);
	TextOut4 (hDC, 16, 32, strTmp, clText);
	TextOut2 (hDC, 40, 32, m_pMgrData->GetHairColorName ((WORD)m_nPos), clText);

	SelectObject (hDC, hFontOld);
	m_pDib->Unlock ();

	m_pDib->BltFrom256 (16 + m_nPosX * 8 - 2, 21, 10, 8, m_pDibSystem, 96, 24, TRUE);
	m_pDib->BltFrom256 (16 + m_nPosX * 8 - 2, 48, 10, 8, m_pDibSystem, 96, 32, TRUE);

	m_dwTimeDrawStart = timeGetTime ();

Exit:
	pDst->Blt (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}


/* ========================================================================= */
/* 関数名	:CWindowHAIRCOLOR::TimerProc									 */
/* 内容		:時間処理														 */
/* 日付		:2007/04/08														 */
/* ========================================================================= */

BOOL CWindowHAIRCOLOR::TimerProc(void)
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
/* 関数名	:CWindowHAIRCOLOR::SetType										 */
/* 内容		:髪色を設定														 */
/* 日付		:2007/04/18														 */
/* ========================================================================= */

void CWindowHAIRCOLOR::SetType(WORD wHairColor)
{
	m_nPos = wHairColor;
	Redraw ();
}


/* ========================================================================= */
/* 関数名	:CWindowHAIRCOLOR::GetType										 */
/* 内容		:髪色を取得														 */
/* 日付		:2007/04/08														 */
/* ========================================================================= */

WORD CWindowHAIRCOLOR::GetType(void)
{
	return m_nPos;
}


/* ========================================================================= */
/* 関数名	:CWindowHAIRCOLOR::OnUp											 */
/* 内容		:キーハンドラ(↑)												 */
/* 日付		:2007/04/08														 */
/* ========================================================================= */

BOOL CWindowHAIRCOLOR::OnUp(void)
{
	int nTmp;
	BOOL bRet;

	bRet = FALSE;

	if (m_nPosX == 0) {
		nTmp = m_nPos / 10;
		nTmp = (nTmp + 1) * 10;
		m_nPos = (nTmp / 10 * 10) + (m_nPos % 10);
	} else {
		m_nPos ++;
	}
	m_nPos = (m_nPos > m_nPosMax) ? 1 : m_nPos;
	m_nPos = (m_nPos <= 0) ? m_nPosMax : m_nPos;

	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, 1);

	bRet = TRUE;
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWindowHAIRCOLOR::OnDown										 */
/* 内容		:キーハンドラ(↓)												 */
/* 日付		:2007/04/08														 */
/* ========================================================================= */

BOOL CWindowHAIRCOLOR::OnDown(void)
{
	int nTmp;
	BOOL bRet;

	bRet = FALSE;

	if (m_nPosX == 0) {
		nTmp = m_nPos / 10;
		nTmp -= 10;
		m_nPos = (nTmp / 10 * 10) + (m_nPos % 10);
	} else {
		nTmp = m_nPos % 10;
		nTmp --;
		m_nPos = (m_nPos / 10 * 10) + (nTmp % 10);
	}
	m_nPos = (m_nPos > m_nPosMax) ? 1 : m_nPos;
	m_nPos = (m_nPos <= 0) ? m_nPosMax : m_nPos;

	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, 1);

	bRet = TRUE;
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWindowHAIRCOLOR::OnLeft										 */
/* 内容		:キーハンドラ(←)												 */
/* 日付		:2007/04/08														 */
/* ========================================================================= */

BOOL CWindowHAIRCOLOR::OnLeft(void)
{
	if (m_nPosX == 1) {
		m_nPosX = 0;
	}
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CWindowHAIRCOLOR::OnRight										 */
/* 内容		:キーハンドラ(→)												 */
/* 日付		:2007/04/08														 */
/* ========================================================================= */

BOOL CWindowHAIRCOLOR::OnRight(void)
{
	if (m_nPosX == 0) {
		m_nPosX = 1;
	}
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CWindowHAIRCOLOR::OnX											 */
/* 内容		:キーハンドラ(X)												 */
/* 日付		:2007/04/08														 */
/* ========================================================================= */

BOOL CWindowHAIRCOLOR::OnX(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_pMgrSound->PlaySound (SOUNDID_OK_PI73);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, 0);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWindowHAIRCOLOR::OnZ											 */
/* 内容		:キーハンドラ(Z)												 */
/* 日付		:2007/04/08														 */
/* ========================================================================= */

BOOL CWindowHAIRCOLOR::OnZ(BOOL bDown)
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
