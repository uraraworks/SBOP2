/// @file WindowACCOUNTINFO.cpp
/// @brief アカウント情報ウィンドウクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/12/29
/// @copyright Copyright(C)URARA-works 2006

#include "stdafx.h"
#include "InfoAccount.h"
#include "InfoCharCli.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "WindowACCOUNTINFO.h"


CWindowACCOUNTINFO::CWindowACCOUNTINFO()
{
	m_nPos	= 0;
	m_nPosMax	= 2;
	m_nID	= WINDOWTYPE_ACCOUNTINFO;
	m_ptViewPos.x	= 280;
	m_ptViewPos.y	= 48;
	m_sizeWindow.cx	= 16 * 2 + 16 * 10;
	m_sizeWindow.cy	= 16 * 2 + 16 * 4;
}


CWindowACCOUNTINFO::~CWindowACCOUNTINFO()
{
}


void CWindowACCOUNTINFO::Create(CMgrData *pMgrData)
{
	CWindowBase::Create(pMgrData);

	m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey(RGB(40, 40, 40));
}


void CWindowACCOUNTINFO::Draw(PCImg32 pDst)
{
	HDC hDC;
	COLORREF clText;
	CmyString strTmp;
	PCInfoAccount pInfoAccount;
	PCInfoCharCli pInfoChar;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame();

	pInfoAccount	= m_pMgrData->GetAccount();
	pInfoChar	= m_pMgrData->GetPlayerChar();
	if (pInfoChar == NULL) {
		goto Exit;
	}

	clText	= RGB(0, 0, 0);
	hDC	= m_pDib->Lock();

	strTmp.Format(_T("キャラ名:%s"), (LPCTSTR)pInfoChar->m_strCharName);
	TextOut2(hDC, m_hFont, 16, 16 + 16 * 0, strTmp,	clText);
	strTmp.Format(_T("アカウント作成日時:"));
	TextOut2(hDC, m_hFont, 16, 16 + 16 * 1, strTmp,	clText);
	TextOut2(hDC, m_hFont, 16, 16 + 16 * 2, _T("前回ログイン日時:"),	clText);
	TextOut2(hDC, m_hFont, 16, 16 + 16 * 3, _T("ログイン回数:"),	clText);

	m_pDib->Unlock();

	m_dwTimeDrawStart = timeGetTime();

Exit:
	pDst->Blt(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}
