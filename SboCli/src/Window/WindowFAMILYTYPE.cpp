/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:WindowFAMILYTYPE.cpp										 */
/* 内容			:種族選択ウィンドウクラス 実装ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/04/07													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowFAMILYTYPE.h"

/* ========================================================================= */
/* 関数名	:CWindowFAMILYTYPE::CWindowFAMILYTYPE							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/04/07														 */
/* ========================================================================= */

CWindowFAMILYTYPE::CWindowFAMILYTYPE()
{
	m_nPos			= 0;
	m_nPosMax		= 4-3;
	m_bInput		= TRUE;
	m_nID			= WINDOWTYPE_FAMILYTYPE;
	m_ptViewPos.x	= 80;
	m_ptViewPos.y	= 80;
	m_sizeWindow.cx	= 320;
	m_sizeWindow.cy	= 320;
}


/* ========================================================================= */
/* 関数名	:CWindowFAMILYTYPE::~CWindowFAMILYTYPE							 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/04/07														 */
/* ========================================================================= */

CWindowFAMILYTYPE::~CWindowFAMILYTYPE()
{
}


/* ========================================================================= */
/* 関数名	:CWindowFAMILYTYPE::Create										 */
/* 内容		:作成															 */
/* 日付		:2007/04/07														 */
/* ========================================================================= */

void CWindowFAMILYTYPE::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);

//	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, 1);
}


/* ========================================================================= */
/* 関数名	:CWindowFAMILYTYPE::Draw										 */
/* 内容		:描画															 */
/* 日付		:2007/04/07														 */
/* ========================================================================= */

void CWindowFAMILYTYPE::Draw(PCImg32 pDst)
{
	int i, nCount;
	HDC hDC;
	HFONT hFontOld;
	COLORREF clText;
	POINT nCursorPos[] = {
		40, 104
//		40, 152,
//		40, 200,
//		40, 248
	};

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	nCount = 4-3;
	DrawFrame (2);
	for (i = 0; i < nCount; i ++) {
		DrawFrame2 (nCursorPos[i].x, nCursorPos[i].y, 96, 8, 2);
	}
	DrawFrame (160, 96, 132, 168, 2);

//	m_pDib->BltFrom256 (166, 128, 120, 104, m_pDibSystem, m_nPos * 120, 520, TRUE);

	clText		= RGB (124, 123, 232);
	hDC			= m_pDib->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont14);
	SetBkMode (hDC, TRANSPARENT);

	TextOut4 (hDC, 88,	24,		_T("新規キャラクター作成"), clText);
	TextOut4 (hDC, 136,	48,		_T("種族選択"), clText);
	TextOut4 (hDC, 256,	288,	_T("１／３"), clText);

	for (i = 0; i < nCount; i ++) {
		TextOut2 (hDC, nCursorPos[i].x, nCursorPos[i].y - 4, m_pMgrData->GetFamilyTypeName ((WORD)(FAMILYTYPE_HUMAN + i)), clText);
	}

	SelectObject (hDC, hFontOld);
	m_pDib->Unlock ();

	DrawCursor (nCursorPos[m_nPos].x - 32, nCursorPos[m_nPos].y - 6);
	m_dwTimeDrawStart = timeGetTime ();

Exit:
	pDst->Blt (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}


/* ========================================================================= */
/* 関数名	:CWindowFAMILYTYPE::TimerProc									 */
/* 内容		:時間処理														 */
/* 日付		:2007/04/07														 */
/* ========================================================================= */

BOOL CWindowFAMILYTYPE::TimerProc(void)
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
/* 関数名	:CWindowFAMILYTYPE::GetType										 */
/* 内容		:種族を取得														 */
/* 日付		:2007/04/07														 */
/* ========================================================================= */

WORD CWindowFAMILYTYPE::GetType(void)
{
	return (FAMILYTYPE_NONE + 1 + m_nPos);
}


/* ========================================================================= */
/* 関数名	:CWindowFAMILYTYPE::OnUp										 */
/* 内容		:キーハンドラ(↑)												 */
/* 日付		:2007/04/07														 */
/* ========================================================================= */

BOOL CWindowFAMILYTYPE::OnUp(void)
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
/* 関数名	:CWindowFAMILYTYPE::OnDown										 */
/* 内容		:キーハンドラ(↓)												 */
/* 日付		:2007/04/07														 */
/* ========================================================================= */

BOOL CWindowFAMILYTYPE::OnDown(void)
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
/* 関数名	:CWindowFAMILYTYPE::OnLeft										 */
/* 内容		:キーハンドラ(←)												 */
/* 日付		:2007/04/07														 */
/* ========================================================================= */

BOOL CWindowFAMILYTYPE::OnLeft(void)
{
	m_nPos = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, 1);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CWindowFAMILYTYPE::OnRight										 */
/* 内容		:キーハンドラ(→)												 */
/* 日付		:2007/04/07														 */
/* ========================================================================= */

BOOL CWindowFAMILYTYPE::OnRight(void)
{
	m_nPos = m_nPosMax - 1;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, 1);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CWindowFAMILYTYPE::OnX											 */
/* 内容		:キーハンドラ(X)												 */
/* 日付		:2007/04/07														 */
/* ========================================================================= */

BOOL CWindowFAMILYTYPE::OnX(BOOL bDown)
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
/* 関数名	:CWindowFAMILYTYPE::OnZ											 */
/* 内容		:キーハンドラ(Z)												 */
/* 日付		:2007/04/07														 */
/* ========================================================================= */

BOOL CWindowFAMILYTYPE::OnZ(BOOL bDown)
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
