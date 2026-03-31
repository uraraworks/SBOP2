/// @file WindowOPTION_VIEWSET.cpp
/// @brief オプション-表示設定ウィンドウクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/06/27
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowOPTION_VIEWSET.h"


CWindowOPTION_VIEWSET::CWindowOPTION_VIEWSET()
{
	m_nPosMax	= 7;
	m_bInput	= TRUE;
	m_nID	= WINDOWTYPE_OPTION_VIEWSET;
	m_ptViewPos.x	= 8 * 13;
	m_ptViewPos.y	= 16 * 8;
	m_sizeWindow.cx	= 16 * 2 + 16 * 16 + 8;
	m_sizeWindow.cy	= 16 * 2 + 16 * (m_nPosMax + 1);
}


CWindowOPTION_VIEWSET::~CWindowOPTION_VIEWSET()
{
}


void CWindowOPTION_VIEWSET::Create(CMgrData *pMgrData)
{
	BOOL bResult;
	int i, nCount;

	CWindowBase::Create(pMgrData);

	m_bActive = TRUE;
	m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey(0);
	m_nPos = 0;

	nCount = m_nPosMax + 1;
	for (i = 0; i < nCount; i ++) {
		bResult = GetCheck(i);
		m_anCheck.push_back((bResult) ? 464 : 400);
		m_adwCheckTime.push_back(0);
	}
}


void CWindowOPTION_VIEWSET::Draw(PCImg32 pDst)
{
	int nLevel, i, nCount;
	HDC hDC;
	HFONT hFontOld;
	COLORREF clText;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame();

	clText	= RGB(1, 1, 1);
	hDC	= m_pDib->Lock();
	hFontOld	= (HFONT)SelectObject(hDC, m_hFont);
	SetBkMode(hDC, TRANSPARENT);

	TextOut2(hDC, 32 + 24, 16 + 16 * 0, _T("発言時にタスクバーチカチカ"),	clText);
	TextOut2(hDC, 32 + 24, 16 + 16 * 1, _T("名前を表示する"),	clText);
	TextOut2(hDC, 32 + 24, 16 + 16 * 2, _T("発言を表示する"),	clText);
	TextOut2(hDC, 32 + 24, 16 + 16 * 3, _T("アイテムを表示する"),	clText);
	TextOut2(hDC, 32 + 24, 16 + 16 * 4, _T("アイテム名を表示する"),	clText);
	TextOut2(hDC, 32 + 24, 16 + 16 * 5, _T("ヘルプアイコンを表示する"),	clText);
	TextOut2(hDC, 32 + 24, 16 + 16 * 6, _T("戦闘メッセージをログに残す"),	clText);
	TextOut2(hDC, 32 + 24, 16 + 16 * 7, _T("60フレームで表示する"),	clText);

	SelectObject(hDC, hFontOld);
	m_pDib->Unlock();

	nCount = m_adwCheckTime.size();
	for (i = 0; i < nCount; i ++) {
		m_pDib->BltFrom256(32, 16 + 16 * i, 16, 16, m_pDibSystem, m_anCheck[i], 0, TRUE);
	}

	DrawCursor(8, 16 + 16 * m_nPos);
	m_dwTimeDrawStart = timeGetTime();

Exit:
	nLevel = 100;
	if (m_bActive == FALSE) {
		nLevel = 60;
	}
	pDst->BltLevel(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, nLevel, TRUE);
}


BOOL CWindowOPTION_VIEWSET::TimerProc(void)
{
	BOOL bRet;
	int i, nCount;
	DWORD dwTime;

	bRet = CWindowBase::TimerProc();

	dwTime = timeGetTime();
	nCount = m_adwCheckTime.size();
	for (i = 0; i < nCount; i ++) {
		if (m_adwCheckTime[i] == 0) {
			continue;
		}
		if (dwTime - m_adwCheckTime[i] > 40) {
			m_adwCheckTime[i] = dwTime;
			m_anCheck[i] += 16;
			if (m_anCheck[i] >= 464) {
				m_adwCheckTime[i] = 0;
			}
			Redraw();
		}
	}

	return bRet;
}


BOOL CWindowOPTION_VIEWSET::OnUp(void)
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


BOOL CWindowOPTION_VIEWSET::OnDown(void)
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


BOOL CWindowOPTION_VIEWSET::OnLeft(void)
{
	m_nPos = 0;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);

	return TRUE;
}


BOOL CWindowOPTION_VIEWSET::OnRight(void)
{
	m_nPos = m_nPosMax;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);

	return TRUE;
}


BOOL CWindowOPTION_VIEWSET::OnX(BOOL bDown)
{
	BOOL bRet, bOn;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_pMgrSound->PlaySound(SOUNDID_OK_PI73);
	PostMessage(m_hWndMain, WM_WINDOWMSG, m_nID, m_nPos);

	bOn = GetCheck(m_nPos);

	m_anCheck[m_nPos] = 400;
	if (bOn == FALSE) {
		m_adwCheckTime[m_nPos] = timeGetTime();
	}

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowOPTION_VIEWSET::OnZ(BOOL bDown)
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


BOOL CWindowOPTION_VIEWSET::GetCheck(int nNo)
{
	BOOL bRet;

	bRet = FALSE;
	switch (nNo) {
	case 0:
		bRet = m_pMgrData->GetOptionTaskbar();
		break;
	case 1:
		bRet = m_pMgrData->GetDrawMode();
		break;
	case 2:
		bRet = m_pMgrData->GetOptionViewChat();
		break;
	case 3:
		bRet = m_pMgrData->GetOptionViewItem();
		break;
	case 4:
		bRet = m_pMgrData->GetOptionViewItemName();
		break;
	case 5:
		bRet = m_pMgrData->GetOptionViewHelpIcon();
		break;
	case 6:
		bRet = m_pMgrData->GetOptionBattleMsgLog();
		break;
	case 7:
		bRet = m_pMgrData->GetOption60Frame();
		break;
	}

	return bRet;
}
