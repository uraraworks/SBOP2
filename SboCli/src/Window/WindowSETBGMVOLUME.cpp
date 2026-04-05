/// @file WindowSETBGMVOLUME.cpp
/// @brief BGM音量の設定ウィンドウクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/06/21
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "TextRenderer.h"
#include "WindowSETBGMVOLUME.h"


CWindowSETBGMVOLUME::CWindowSETBGMVOLUME()
{
	m_nPosMax	= 4;
	m_bInput	= TRUE;
	m_nID	= WINDOWTYPE_SETBGMVOLUME;
	m_ptViewPos.x	= 80;
	m_ptViewPos.y	= 96;
	m_sizeWindow.cx	= 16 * 2 + 16 * 4;
	m_sizeWindow.cy	= 16 * 2 + 16 * 5;
}


CWindowSETBGMVOLUME::~CWindowSETBGMVOLUME()
{
}


void CWindowSETBGMVOLUME::Create(CMgrData *pMgrData)
{
	CWindowBase::Create(pMgrData);

	m_bActive = TRUE;
	m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey(0);

	m_nPos = m_pMgrData->GetBGMVolume();
}


void CWindowSETBGMVOLUME::Draw(PCImg32 pDst)
{
	int nLevel;
	COLORREF clText;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame();

	clText = RGB(1, 1, 1);
	TextOut2(m_pDib, FONTID_PGOTHIC_16_BOLD, 32, 16 + 16 * 0, _T("無し"),	clText);
	TextOut2(m_pDib, FONTID_PGOTHIC_16_BOLD, 32, 16 + 16 * 1, _T("１"),	clText);
	TextOut2(m_pDib, FONTID_PGOTHIC_16_BOLD, 32, 16 + 16 * 2, _T("２"),	clText);
	TextOut2(m_pDib, FONTID_PGOTHIC_16_BOLD, 32, 16 + 16 * 3, _T("３"),	clText);
	TextOut2(m_pDib, FONTID_PGOTHIC_16_BOLD, 32, 16 + 16 * 4, _T("４"),	clText);

	DrawCursor(8, 16 + 16 * m_nPos);
	m_dwTimeDrawStart = timeGetTime();

Exit:
	nLevel = 100;
	if (m_bActive == FALSE) {
		nLevel = 60;
	}
	pDst->BltLevel(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, nLevel, TRUE);
}


BOOL CWindowSETBGMVOLUME::OnUp(void)
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


BOOL CWindowSETBGMVOLUME::OnDown(void)
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


BOOL CWindowSETBGMVOLUME::OnLeft(void)
{
	m_nPos = 0;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);

	return TRUE;
}


BOOL CWindowSETBGMVOLUME::OnRight(void)
{
	m_nPos = m_nPosMax;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);

	return TRUE;
}


BOOL CWindowSETBGMVOLUME::OnX(BOOL bDown)
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


BOOL CWindowSETBGMVOLUME::OnZ(BOOL bDown)
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
