/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名：	WindowLOGINMENU.cpp											 */
/* 内容：		ログインメニューウィンドウクラス 実装ファイル				 */
/* 作成：		年がら年中春うらら(URARA-works)								 */
/* 作成開始日：	2006/11/05													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoAccount.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowLOGINMENU.h"


/* ========================================================================= */
/* 関数名：	CWindowLOGINMENU::CWindowLOGINMENU								 */
/* 内容：	コンストラクタ													 */
/* 日付：	2006/11/05														 */
/* ========================================================================= */

CWindowLOGINMENU::CWindowLOGINMENU()
{
	m_nPosMax		= 3;
	m_bInput		= TRUE;
	m_nID			= WINDOWTYPE_LOGINMENU;
	m_ptViewPos.x	= 40;
	m_ptViewPos.y	= 48;
	m_sizeWindow.cx	= 16 * 2 + 16 * 6;
	m_sizeWindow.cy	= 16 * 2 + 16 * 4;

	m_abEnable[0] = FALSE;
	m_abEnable[1] = FALSE;
	m_abEnable[2] = FALSE;
	m_abEnable[3] = TRUE;
}


/* ========================================================================= */
/* 関数名：	CWindowLOGINMENU::~CWindowLOGINMENU								 */
/* 内容：	デストラクタ													 */
/* 日付：	2006/11/05														 */
/* ========================================================================= */

CWindowLOGINMENU::~CWindowLOGINMENU()
{
}


/* ========================================================================= */
/* 関数名	:CWindowLOGINMENU::Create										 */
/* 内容		:作成															 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

void CWindowLOGINMENU::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_bActive = TRUE;
	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);
}


/* ========================================================================= */
/* 関数名	:CWindowLOGINMENU::Update										 */
/* 内容		:更新															 */
/* 日付		:2006/11/08														 */
/* ========================================================================= */

void CWindowLOGINMENU::Update(void)
{
	int nCharCount;
	PCInfoAccount pInfoAccount;

	SetEnable (1, FALSE);

	pInfoAccount	= m_pMgrData->GetAccount ();
	nCharCount		= pInfoAccount->m_adwCharID.GetSize ();
	if (nCharCount == 0) {
		SetEnable (0, FALSE);
		SetEnable (2, FALSE);
	} else {
		SetEnable (0, TRUE);
		SetEnable (2, TRUE);
	}
	if ((nCharCount >= 0) && (nCharCount < 3)) {
		SetEnable (1, TRUE);
	}
//Todo:
SetEnable (2, FALSE);
	SetEnable (3, TRUE);

	CWindowBase::Update ();
}


/* ========================================================================= */
/* 関数名：	CWindowLOGINMENU::Draw											 */
/* 内容：	描画															 */
/* 日付：	2006/11/05														 */
/* ========================================================================= */

void CWindowLOGINMENU::Draw(PCImg32 pDst)
{
	int nLevel;
	HDC hDC;
	HFONT hFontOld;
	COLORREF clText, clBack;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame ();

	clText		= RGB (1, 1, 1);
	hDC			= m_pDib->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont16Normal);
	SetBkMode (hDC, TRANSPARENT);

	clBack = RGB (150, 80, 30);
	clText = (m_abEnable[0] == TRUE) ? RGB (255, 255, 255) : RGB (128, 128, 128);
	TextOut4 (hDC, 32, 16 + 16 * 0, "キャラ選択", clBack, clText);
	clText = (m_abEnable[1] == TRUE) ? RGB (255, 255, 255) : RGB (128, 128, 128);
	TextOut4 (hDC, 32, 16 + 16 * 1, "新規作成", clBack, clText);
	clText = (m_abEnable[2] == TRUE) ? RGB (255, 255, 255) : RGB (128, 128, 128);
	TextOut4 (hDC, 32, 16 + 16 * 2, "キャラ削除", clBack, clText);
	clText = (m_abEnable[3] == TRUE) ? RGB (255, 255, 255) : RGB (128, 128, 128);
	TextOut4 (hDC, 32, 16 + 16 * 3, "戻る", clBack, clText);

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
/* 関数名：	CWindowLOGINMENU::TimerProc										 */
/* 内容：	時間処理														 */
/* 日付：	2006/11/05														 */
/* ========================================================================= */

BOOL CWindowLOGINMENU::TimerProc(void)
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
/* 関数名：	CWindowLOGINMENU::SetEnable										 */
/* 内容：	メニュー項目の入力状態設定										 */
/* 日付：	2006/11/05														 */
/* ========================================================================= */

void CWindowLOGINMENU::SetEnable(int nNo, BOOL bEnable)
{
	m_abEnable[nNo] = bEnable;
}


/* ========================================================================= */
/* 関数名：	CWindowLOGINMENU::OnUp											 */
/* 内容：	キーハンドラ(↑)												 */
/* 日付：	2006/11/05														 */
/* ========================================================================= */

BOOL CWindowLOGINMENU::OnUp(void)
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
/* 関数名：	CWindowLOGINMENU::OnDown										 */
/* 内容：	キーハンドラ(↓)												 */
/* 日付：	2006/11/05														 */
/* ========================================================================= */

BOOL CWindowLOGINMENU::OnDown(void)
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
/* 関数名：	CWindowLOGINMENU::OnLeft										 */
/* 内容：	キーハンドラ(←)												 */
/* 日付：	2006/11/05														 */
/* ========================================================================= */

BOOL CWindowLOGINMENU::OnLeft(void)
{
	m_nPos = 0;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* 関数名：	CWindowLOGINMENU::OnRight										 */
/* 内容：	キーハンドラ(→)												 */
/* 日付：	2006/11/05														 */
/* ========================================================================= */

BOOL CWindowLOGINMENU::OnRight(void)
{
	m_nPos = m_nPosMax;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* 関数名：	CWindowLOGINMENU::OnX											 */
/* 内容：	キーハンドラ(X)													 */
/* 日付：	2006/11/05														 */
/* ========================================================================= */

BOOL CWindowLOGINMENU::OnX(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}
	if (m_abEnable[m_nPos] == FALSE) {
		goto Exit;
	}

	m_pMgrSound->PlaySound (SOUNDID_OK_PI73);
	PostMessage (m_hWndMain, WM_WINDOWMSG, WINDOWTYPE_LOGINMENU, m_nPos);

	bRet = TRUE;
Exit:
	return bRet;
}
/* Copyright(C)URARA-works 2006 */
