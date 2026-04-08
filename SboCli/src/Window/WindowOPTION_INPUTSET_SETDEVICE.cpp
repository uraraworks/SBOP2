/// @file WindowOPTION_INPUTSET_SETDEVICE.cpp
/// @brief オプション-入力設定-入力デバイスの設定ウィンドウクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/07/12
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "MgrKeyInput.h"
#include "WindowOPTION_INPUTSET_SETDEVICE.h"


CWindowOPTION_INPUTSET_SETDEVICE::CWindowOPTION_INPUTSET_SETDEVICE()
{
	m_nPosMax	= 0;
	m_bInput	= TRUE;
	m_nID	= WINDOWTYPE_OPTION_INPUTSET_SETDEVICE;
	m_ptViewPos.x	= 8 * 17;
	m_ptViewPos.y	= 16 * 12;
	m_sizeWindow.cx	= 16 * 2 + 16 * 16 + 12;
	m_sizeWindow.cy	= 16 * 2 + 16 * 0;

	m_MgrKeyInput = NULL;
}


CWindowOPTION_INPUTSET_SETDEVICE::~CWindowOPTION_INPUTSET_SETDEVICE()
{
}


void CWindowOPTION_INPUTSET_SETDEVICE::Create(CMgrData *pMgrData)
{
	int i, nCount;
	GUID stGuidTmp, stGuidInput;

	CWindowBase::Create(pMgrData);

	m_MgrKeyInput = m_pMgrData->GetMgrKeyInput();

	m_nPosMax = m_MgrKeyInput->GetDeviceCount();
	m_sizeWindow.cy	+= (16 * (m_nPosMax + 1));

	m_bActive = TRUE;
	m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey(0);
	m_nPos = 0;

	stGuidInput = m_pMgrData->GetInputGuid();
	nCount = m_MgrKeyInput->GetDeviceCount();
	for (i = 0; i < nCount; i ++) {
		m_MgrKeyInput->GetGUID(i, stGuidTmp);
		if (stGuidTmp == stGuidInput) {
			m_nPos = i + 1;
			break;
		}
	}
}


void CWindowOPTION_INPUTSET_SETDEVICE::Draw(PCImg32 pDst)
{
	int nLevel, i;
	HDC hDC;
	COLORREF clText;
	CmyString strTmp;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame();

	clText	= RGB(1, 1, 1);
	hDC	= m_pDib->Lock();
	SetBkMode(hDC, TRANSPARENT);

	TextOut2(hDC, m_hFont, 32, 16 + 16 * 0, _T("使用しない"), clText);

	for (i = 0; i < m_nPosMax; i ++) {
		m_MgrKeyInput->GetDeviceName(i, strTmp);
		TextOut2(hDC, m_hFont, 32, 16 + 16 * (i + 1), strTmp, clText);
	}

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


BOOL CWindowOPTION_INPUTSET_SETDEVICE::OnUp(void)
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


BOOL CWindowOPTION_INPUTSET_SETDEVICE::OnDown(void)
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


BOOL CWindowOPTION_INPUTSET_SETDEVICE::OnLeft(void)
{
	m_nPos = 0;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);

	return TRUE;
}


BOOL CWindowOPTION_INPUTSET_SETDEVICE::OnRight(void)
{
	m_nPos = m_nPosMax;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);

	return TRUE;
}


BOOL CWindowOPTION_INPUTSET_SETDEVICE::OnX(BOOL bDown)
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


BOOL CWindowOPTION_INPUTSET_SETDEVICE::OnZ(BOOL bDown)
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
