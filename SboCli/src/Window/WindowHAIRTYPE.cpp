/// @file WindowHAIRTYPE.cpp
/// @brief 髪型選択ウィンドウクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/08
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowHAIRTYPE.h"

CWindowHAIRTYPE::CWindowHAIRTYPE()
{
	m_nPos	= 1;
	m_nPosMax	= 4;//22;
	m_nPosX	= 1;
	m_bInput	= TRUE;
	m_nID	= WINDOWTYPE_HAIRTYPE;
	m_ptViewPos.x	= 88;
	m_ptViewPos.y	= 160;
	m_sizeWindow.cx	= 16 * 2 + 16 * 8;
	m_sizeWindow.cy	= 16 * 2 + 8 * 5;
}


CWindowHAIRTYPE::~CWindowHAIRTYPE()
{
}


void CWindowHAIRTYPE::Create(CMgrData *pMgrData)
{
	CWindowBase::Create(pMgrData);

	m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey(0);

	PostWindowMessage(1);
}


void CWindowHAIRTYPE::Draw(PCImg32 pDst)
{
	HDC hDC;
	COLORREF clText;
	CmyString strTmp;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame();

	clText	= RGB(255, 127, 53);
	hDC	= m_pDib->Lock();

	TextOut4(hDC, m_hFont14, 64, 8, _T("髪型"), clText);
	strTmp.Format(_T("%02d"), m_nPos);
	TextOut4(hDC, m_hFont14, 16, 32, strTmp, clText);
//	TextOut2(hDC, m_hFont14, 40, 32, m_pMgrData->GetHairTypeName((WORD)m_nPos), clText);

	m_pDib->Unlock();

	m_pDib->BltFrom256(16 + m_nPosX * 8 - 2, 21, 10, 8, m_pDibSystem, 96, 24, TRUE);
	m_pDib->BltFrom256(16 + m_nPosX * 8 - 2, 48, 10, 8, m_pDibSystem, 96, 32, TRUE);

	m_dwTimeDrawStart = timeGetTime();

Exit:
	pDst->Blt(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}


BOOL CWindowHAIRTYPE::TimerProc(void)
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


void CWindowHAIRTYPE::SetType(WORD wHairType)
{
	m_nPos = wHairType;
	Redraw();
}


WORD CWindowHAIRTYPE::GetType(void)
{
	return m_nPos;
}


BOOL CWindowHAIRTYPE::OnUp(void)
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

	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
	PostWindowMessage(1);

	bRet = TRUE;
	return bRet;
}


BOOL CWindowHAIRTYPE::OnDown(void)
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

	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
	PostWindowMessage(1);

	bRet = TRUE;
	return bRet;
}


BOOL CWindowHAIRTYPE::OnLeft(void)
{
	if (m_nPosX == 1) {
		m_nPosX = 0;
	}
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);

	return TRUE;
}


BOOL CWindowHAIRTYPE::OnRight(void)
{
	if (m_nPosX == 0) {
		m_nPosX = 1;
	}
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);

	return TRUE;
}


BOOL CWindowHAIRTYPE::OnX(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_pMgrSound->PlaySound(SOUNDID_OK_PI73);
	PostWindowMessage(0);

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowHAIRTYPE::OnZ(BOOL bDown)
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
