/// @file WindowNAMEINPUT.cpp
/// @brief 名前入力ウィンドウクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/10
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "LibInfoCharBase.h"
#include "LibInfoCharCli.h"
#include "InfoCharCli.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "MgrDraw.h"
#include "WindowCHARNAME.h"
#include "WindowNAMEINPUT.h"

CWindowNAMEINPUT::CWindowNAMEINPUT()
{
	m_nPos	= 0;
	m_nPosMax	= 2;
	m_bInput	= TRUE;
	m_nID	= WINDOWTYPE_NAMEINPUT;
	m_ptViewPos.x	= 80;
	m_ptViewPos.y	= 80;
	m_sizeWindow.cx	= 320;
	m_sizeWindow.cy	= 320;

	m_pMgrWindow	= NULL;
	m_pMgrDraw	= NULL;
	m_pWindowCHARNAME	= NULL;
	m_pInfoCharCli	= new CInfoCharCli;
}


CWindowNAMEINPUT::~CWindowNAMEINPUT()
{
	SAFE_DELETE(m_pInfoCharCli);
}


void CWindowNAMEINPUT::Create(CMgrData *pMgrData)
{
	CWindowBase::Create(pMgrData);

	m_pMgrWindow	= m_pMgrData->GetMgrWindow();
	m_pMgrDraw	= m_pMgrData->GetMgrDraw();

	m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey(0);

	m_pInfoCharCli->Create(m_pMgrData);
	m_pInfoCharCli->m_dwMotionTypeID = 2;
	m_pInfoCharCli->MakeCharGrp();
	m_pMgrData->GetLibInfoChar()->RenewMotionInfo(m_pInfoCharCli);

	PostWindowMessage(1);
}


void CWindowNAMEINPUT::OnWindowMsg(int nType, DWORD dwPara)
{
	switch (nType) {
	case WINDOWTYPE_CHARNAME:	// キャラ名入力
		m_pInfoCharCli->m_strCharName = m_pWindowCHARNAME->m_strName;
		m_pMgrWindow->Delete(WINDOWTYPE_CHARNAME);
		m_pWindowCHARNAME = NULL;
		Redraw();
		break;
	}
}


void CWindowNAMEINPUT::Draw(PCImg32 pDst)
{
	HDC hDC;
	HFONT hFontOld;
	COLORREF clText;
	POINT nCursorPos[] = {
		108, 240,
		132, 292
	};

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame(2);
	DrawFrame(112, 96, 100, 104, 2);
	DrawFrame2(108, 240, 16 * 7, 8, 2);

	clText	= RGB(124, 123, 232);
	hDC	= m_pDib->Lock();
	hFontOld	= (HFONT)SelectObject(hDC, m_hFont14);
	SetBkMode(hDC, TRANSPARENT);

	TextOut4(hDC, 88,	24,	_T("新規キャラクター作成"), clText);
	TextOut4(hDC, 136,	48,	_T("名前入力"), clText);
	TextOut4(hDC, 112,	216,	_T("キャラクター名"), clText);
	TextOut4(hDC, 148,	288,	_T("登録"), clText);
	TextOut4(hDC, 256,	288,	_T("３／３"), clText);
	TextOut2(hDC, 108, 240 - 2, m_pInfoCharCli->m_strCharName, clText);

	SelectObject(hDC, hFontOld);
	m_pDib->Unlock();

	DrawCursor(nCursorPos[m_nPos].x - 32, nCursorPos[m_nPos].y - 6);
	m_dwTimeDrawStart = timeGetTime();

	m_pMgrDraw->DrawChar(m_pDib, 130, 124, m_pInfoCharCli);

Exit:
	pDst->Blt(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}


BOOL CWindowNAMEINPUT::TimerProc(void)
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


BOOL CWindowNAMEINPUT::OnUp(void)
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


BOOL CWindowNAMEINPUT::OnDown(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos >= m_nPosMax - 1) {
		goto Exit;
	}
	m_nPos ++;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
	PostWindowMessage(1);

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowNAMEINPUT::OnLeft(void)
{
	m_nPos = 0;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
	PostWindowMessage(1);

	return TRUE;
}


BOOL CWindowNAMEINPUT::OnRight(void)
{
	m_nPos = m_nPosMax - 1;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
	PostWindowMessage(1);

	return TRUE;
}


BOOL CWindowNAMEINPUT::OnX(BOOL bDown)
{
	BOOL bRet, bResult;
	CLibInfoCharBase LibInfoChar;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	switch (m_nPos) {
	case 0:	// 名前
		m_pMgrWindow->MakeWindowCHARNAME();
		m_pWindowCHARNAME = (PCWindowCHARNAME)m_pMgrWindow->GetWindow(WINDOWTYPE_CHARNAME);
		break;
	case 1:	// 登録
		if (m_pInfoCharCli->m_strCharName.IsEmpty()) {
			m_pMgrWindow->MakeWindowMSG("名前を入力してください", 3000);
			break;
		}
		bResult = LibInfoChar.NameCheck(m_pInfoCharCli->m_strCharName);
		if (bResult == FALSE) {
			m_pMgrWindow->MakeWindowMSG("名前に空白は使えません", 3000);
			break;
		}
	PostWindowMessage(0);
		break;
	}
	m_pMgrSound->PlaySound(SOUNDID_OK_PI73);

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowNAMEINPUT::OnZ(BOOL bDown)
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
