/// @file WindowSYSTEMMENU.cpp
/// @brief システムメニューウィンドウクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/06/19
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowSYSTEMMENU.h"


CWindowSYSTEMMENU::CWindowSYSTEMMENU()
{
	m_nPosMax	= 1;
	m_bInput	= TRUE;
	m_nID	= WINDOWTYPE_SYSTEMMENU;
	m_ptViewPos.x	= 8 * 5;
	m_ptViewPos.y	= 16 * 3;
	m_sizeWindow.cx	= 16 * 2 + 16 * 11;
	m_sizeWindow.cy	= 16 * 2 + 16 * 2;
}


CWindowSYSTEMMENU::~CWindowSYSTEMMENU()
{
}


void CWindowSYSTEMMENU::Create(CMgrData *pMgrData)
{
	CWindowBase::Create(pMgrData);

	m_bActive = TRUE;
	m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey(0);
}


void CWindowSYSTEMMENU::Draw(PCImg32 pDst)
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

	clText = RGB(1, 1, 1);
	TextOut2(hDC, m_hFont, 32, 16 + 16 * 0, _T("名前と発言色の設定"),	clText);
	TextOut2(hDC, m_hFont, 32, 16 + 16 * 1, _T("オプション"),	clText);

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


BOOL CWindowSYSTEMMENU::OnUp(void)
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


BOOL CWindowSYSTEMMENU::OnDown(void)
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


BOOL CWindowSYSTEMMENU::OnLeft(void)
{
	m_nPos = 0;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);

	return TRUE;
}


BOOL CWindowSYSTEMMENU::OnRight(void)
{
	m_nPos = m_nPosMax;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);

	return TRUE;
}


BOOL CWindowSYSTEMMENU::OnX(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_pMgrSound->PlaySound(SOUNDID_OK_PI73);
	m_pMgrData->PostWindowMessage(WINDOWTYPE_SYSTEMMENU, m_nPos);

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowSYSTEMMENU::OnZ(BOOL bDown)
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


BOOL CWindowSYSTEMMENU::OnEscape(BOOL bDown)
{
	return OnZ(bDown);
}
