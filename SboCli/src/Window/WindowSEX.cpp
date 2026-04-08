/// @file WindowSEX.cpp
/// @brief 性別ウィンドウクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/14
/// @copyright Copyright(C)URARA-works 2006

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowSEX.h"

CWindowSEX::CWindowSEX()
{
	m_nPos	= 0;
	m_nPosMax	= 1;
	m_bInput	= TRUE;
	m_nID	= WINDOWTYPE_SEX;
	m_ptViewPos.x	= 88;
	m_ptViewPos.y	= 160;
	m_sizeWindow.cx	= 16 * 2 + 16 * 6;
	m_sizeWindow.cy	= 16 * 2 + 8 * 7;
}


CWindowSEX::~CWindowSEX()
{
}


void CWindowSEX::Create(CMgrData *pMgrData)
{
	CWindowBase::Create(pMgrData);

	m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey(0);

	PostWindowMessage(1);
}


void CWindowSEX::Draw(PCImg32 pDst)
{
	HDC hDC;
	COLORREF clText;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame();

	clText	= RGB(255, 127, 53);
	hDC	= m_pDib->Lock();

	TextOut4(hDC, m_hFont14, 48, 8, _T("性別"),	clText);
	TextOut2(hDC, m_hFont14, 40, 16 + 16 * 1,	_T("♂ 男の子"),	clText);
	TextOut2(hDC, m_hFont14, 40, 16 + 16 * 2 + 8,	_T("♀ 女の子"),	clText);

	m_pDib->Unlock();

	DrawCursor(8, 32 + 24 * m_nPos);
	m_dwTimeDrawStart = timeGetTime();

Exit:
	pDst->Blt(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}


BOOL CWindowSEX::TimerProc(void)
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


void CWindowSEX::SetType(int nSex)
{
	if (nSex == SEX_FEMALE) {
		m_nPos = 1;
	} else {
		m_nPos = 0;
	}
	Redraw();
}


int CWindowSEX::GetType(void)
{
	int nRet;

	nRet = SEX_MALE;
	if (m_nPos == 1) {
		nRet = SEX_FEMALE;
	}

	return nRet;
}


BOOL CWindowSEX::OnUp(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos <= 0) {
		goto Exit;
	}
	m_nPos --;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
	PostWindowMessage(1);

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowSEX::OnDown(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos >= m_nPosMax) {
		goto Exit;
	}
	m_nPos ++;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
	PostWindowMessage(1);

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowSEX::OnLeft(void)
{
	m_nPos = 0;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
	PostWindowMessage(1);

	return TRUE;
}


BOOL CWindowSEX::OnRight(void)
{
	m_nPos = m_nPosMax;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
	PostWindowMessage(1);

	return TRUE;
}


BOOL CWindowSEX::OnX(BOOL bDown)
{
	BOOL bRet;
	PCMgrWindow pMgrWindow;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	pMgrWindow = m_pMgrData->GetMgrWindow();

	m_pMgrSound->PlaySound(SOUNDID_OK_PI73);
	PostWindowMessage(0);

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowSEX::OnZ(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_pMgrSound->PlaySound(SOUNDID_CANCEL);
	PostWindowMessage(-1);
	m_bDelete = TRUE;

	bRet = TRUE;
Exit:
	return bRet;
}
