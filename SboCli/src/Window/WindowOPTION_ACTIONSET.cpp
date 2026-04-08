/// @file WindowOPTION_ACTIONSET.cpp
/// @brief オプション-動作設定ウィンドウクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/07/21
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowOPTION_ACTIONSET.h"


CWindowOPTION_ACTIONSET::CWindowOPTION_ACTIONSET()
{
	m_nPosMax	= 0;
	m_bInput	= TRUE;
	m_nID	= WINDOWTYPE_OPTION_ACTIONSET;
	m_ptViewPos.x	= 8 * 13;
	m_ptViewPos.y	= 16 * 11;
	m_sizeWindow.cx	= 16 * 2 + 16 * 12 + 8;
	m_sizeWindow.cy	= 16 * 2 + 16 * 1;
}


CWindowOPTION_ACTIONSET::~CWindowOPTION_ACTIONSET()
{
}


void CWindowOPTION_ACTIONSET::Create(CMgrData *pMgrData)
{
	CWindowBase::Create(pMgrData);

	m_bActive = TRUE;
	m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey(0);
	m_nPos = 0;
}


void CWindowOPTION_ACTIONSET::Draw(PCImg32 pDst)
{
	int nLevel;
	HDC hDC;
	COLORREF clText;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame();

	clText	= RGB(1, 1, 1);
	hDC	= m_pDib->Lock();
	SetBkMode(hDC, TRANSPARENT);

	TextOut2(hDC, m_hFont, 32, 16 + 16 * 0, _T("おひるねタイマーの設定"), clText);

	m_pDib->Unlock();

	DrawCursor(8, 16 + 16 * m_nPos);
	m_dwTimeDrawStart = timeGetTime();

Exit:
	nLevel = 100;
	if (m_bActive == FALSE) {
		nLevel = 60;
	}
	pDst->BltLevel(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, nLevel, TRUE);
}


BOOL CWindowOPTION_ACTIONSET::OnUp(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos <= 0) {
		goto Exit;
	}
	m_nPos --;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowOPTION_ACTIONSET::OnDown(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos >= m_nPosMax) {
		goto Exit;
	}
	m_nPos ++;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowOPTION_ACTIONSET::OnLeft(void)
{
	m_nPos = 0;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);

	return TRUE;
}


BOOL CWindowOPTION_ACTIONSET::OnRight(void)
{
	m_nPos = m_nPosMax;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);

	return TRUE;
}


BOOL CWindowOPTION_ACTIONSET::OnX(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_pMgrSound->PlaySound(SOUNDID_OK_PI73);
	PostWindowMessage(m_nPos);

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowOPTION_ACTIONSET::OnZ(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_bDelete = TRUE;
	m_pMgrSound->PlaySound(SOUNDID_CANCEL);

	bRet = TRUE;
Exit:
	return bRet;
}
