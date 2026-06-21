/// @file WindowDELCONFIRM.cpp
/// @brief 削除確認ダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/14
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowDELCONFIRM.h"

CWindowDELCONFIRM::CWindowDELCONFIRM()
{
	m_nPos	= 1;	// 誤爆防止のため「いいえ」にフォーカス
	m_nPosMax	= 1;
	m_bInput	= TRUE;
	m_nID	= WINDOWTYPE_DELCONFIRM;
	m_ptViewPos.x	= 88;
	m_ptViewPos.y	= 160;
	m_sizeWindow.cx	= 16 * 2 + 16 * 8;
	m_sizeWindow.cy	= 16 * 2 + 8 * 9;

	m_strMsg	= _T("削除しますか？");
}


CWindowDELCONFIRM::~CWindowDELCONFIRM()
{
}


void CWindowDELCONFIRM::Create(CMgrData *pMgrData)
{
	CWindowBase::Create(pMgrData);

	m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey(0);
}


void CWindowDELCONFIRM::Draw(PCImg32 pDst)
{
	HDC hDC;
	COLORREF clText;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame();

	clText	= RGB(255, 127, 53);
	hDC	= m_pDib->Lock();

	TextOut4(hDC, m_hFont14, 48, 8, (LPCTSTR)m_strMsg, clText);
	TextOut2(hDC, m_hFont14, 40, 16 + 16 * 1,	_T("はい"),	clText);
	TextOut2(hDC, m_hFont14, 40, 16 + 16 * 2 + 8,	_T("いいえ"),	clText);

	m_pDib->Unlock();

	DrawCursor(8, 32 + 24 * m_nPos);
	m_dwTimeDrawStart = timeGetTime();

Exit:
	pDst->Blt(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}


BOOL CWindowDELCONFIRM::TimerProc(void)
{
	BOOL bRet;
	int nCursorAnimeBack;

	bRet = FALSE;

	nCursorAnimeBack = m_nCursorAnime;
	CWindowBase::TimerProc();
	if (nCursorAnimeBack == m_nCursorAnime) {
		goto Exit;
	}

	m_dwTimeDrawStart = 0;
	bRet = TRUE;
Exit:
	return bRet;
}


void CWindowDELCONFIRM::SetCharName(LPCTSTR pszName)
{
	m_strMsg = pszName;
	m_strMsg += _T(" を削除しますか？");
	Redraw();
}


BOOL CWindowDELCONFIRM::OnUp(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos <= 0) {
		goto Exit;
	}
	m_nPos--;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
	Redraw();

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowDELCONFIRM::OnDown(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos >= m_nPosMax) {
		goto Exit;
	}
	m_nPos++;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
	Redraw();

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowDELCONFIRM::OnX(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_pMgrSound->PlaySound(SOUNDID_OK_PI73);
	// m_nPos==0:はい → PostWindowMessage(0)、m_nPos==1:いいえ → PostWindowMessage(1)
	PostWindowMessage(m_nPos);
	m_bDelete = TRUE;

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowDELCONFIRM::OnZ(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_pMgrSound->PlaySound(SOUNDID_CANCEL);
	PostWindowMessage(1);	// いいえ扱い
	m_bDelete = TRUE;

	bRet = TRUE;
Exit:
	return bRet;
}
