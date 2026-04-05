/// @file WindowLOGINMENU.cpp
/// @brief ログインメニューウィンドウクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/05
/// @copyright Copyright(C)URARA-works 2006

#include "stdafx.h"
#include "InfoAccount.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowLOGINMENU.h"
#include "TextRenderer.h"


CWindowLOGINMENU::CWindowLOGINMENU()
{
	m_nPosMax	= 3;
	m_bInput	= TRUE;
	m_nID	= WINDOWTYPE_LOGINMENU;
	m_ptViewPos.x	= 40;
	m_ptViewPos.y	= 48;
	m_sizeWindow.cx	= 16 * 2 + 16 * 6;
	m_sizeWindow.cy	= 16 * 2 + 16 * 4;

	m_abEnable[0] = FALSE;
	m_abEnable[1] = FALSE;
	m_abEnable[2] = FALSE;
	m_abEnable[3] = TRUE;
}


CWindowLOGINMENU::~CWindowLOGINMENU()
{
}


void CWindowLOGINMENU::Create(CMgrData *pMgrData)
{
	CWindowBase::Create(pMgrData);

	m_bActive = TRUE;
	m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey(0);
}


void CWindowLOGINMENU::Update(void)
{
	int nCharCount;
	PCInfoAccount pInfoAccount;

	SetEnable(1, FALSE);

	pInfoAccount	= m_pMgrData->GetAccount();
	nCharCount	= pInfoAccount->m_adwCharID.size();
	if (nCharCount == 0) {
		SetEnable(0, FALSE);
		SetEnable(2, FALSE);
	} else {
		SetEnable(0, TRUE);
		SetEnable(2, TRUE);
	}
	if ((nCharCount >= 0) && (nCharCount < 3)) {
		SetEnable(1, TRUE);
	}
//Todo:
SetEnable(2, FALSE);
	SetEnable(3, TRUE);

	CWindowBase::Update();
}


void CWindowLOGINMENU::Draw(PCImg32 pDst)
{
	int nLevel;
	COLORREF clText;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame();

	clText = (m_abEnable[0] == TRUE) ? RGB(1, 1, 1) : RGB(128, 128, 128);
	TextOut2(m_pDib, FONTID_PGOTHIC_16_BOLD, 32, 16 + 16 * 0, _T("キャラ選択"), clText);
	clText = (m_abEnable[1] == TRUE) ? RGB(1, 1, 1) : RGB(128, 128, 128);
	TextOut2(m_pDib, FONTID_PGOTHIC_16_BOLD, 32, 16 + 16 * 1, _T("新規作成"), clText);
	clText = (m_abEnable[2] == TRUE) ? RGB(1, 1, 1) : RGB(128, 128, 128);
	TextOut2(m_pDib, FONTID_PGOTHIC_16_BOLD, 32, 16 + 16 * 2, _T("キャラ削除"), clText);
	clText = (m_abEnable[3] == TRUE) ? RGB(1, 1, 1) : RGB(128, 128, 128);
	TextOut2(m_pDib, FONTID_PGOTHIC_16_BOLD, 32, 16 + 16 * 3, _T("戻る"), clText);

	DrawCursor(8, 16 + 16 * m_nPos);
	m_dwTimeDrawStart = timeGetTime();

Exit:
	nLevel = 100;
	if (m_bActive == FALSE) {
		nLevel = 60;
	}
	pDst->BltLevel(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, nLevel, TRUE);
}


BOOL CWindowLOGINMENU::TimerProc(void)
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


void CWindowLOGINMENU::SetEnable(int nNo, BOOL bEnable)
{
	m_abEnable[nNo] = bEnable;
}


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
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);

	bRet = TRUE;
Exit:
	return bRet;
}


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
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowLOGINMENU::OnLeft(void)
{
	m_nPos = 0;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);

	return TRUE;
}


BOOL CWindowLOGINMENU::OnRight(void)
{
	m_nPos = m_nPosMax;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);

	return TRUE;
}


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

	m_pMgrSound->PlaySound(SOUNDID_OK_PI73);
	m_pMgrData->PostWindowMessage(WINDOWTYPE_LOGINMENU, m_nPos);

	bRet = TRUE;
Exit:
	return bRet;
}
