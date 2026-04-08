/// @file WindowSTATUS.cpp
/// @brief 強さウィンドウクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/06/26
/// @copyright Copyright(C)URARA-works 2005

#include "stdafx.h"
#include <stdio.h>
#include "InfoCharCli.h"
#include "LibInfoCharCli.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "WindowSTATUS.h"


CWindowSTATUS::CWindowSTATUS()
{
	m_nID	= WINDOWTYPE_STATUS;
	m_ptViewPos.x	= 16 + 16 * 18 - 8;
	m_ptViewPos.y	= 16;
	m_sizeWindow.cx	= 16 * 2 + 16 * 9;
	m_sizeWindow.cy	= 16 * 2 + 16 * 12;

	m_dwCharID = 0;
}


CWindowSTATUS::~CWindowSTATUS()
{
}


void CWindowSTATUS::Create(CMgrData *pMgrData)
{
	CWindowBase::Create(pMgrData);

	m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey(RGB(255, 0, 255));
}


void CWindowSTATUS::Draw(PCImg32 pDst)
{
//	int i;
	HDC hDC;
//	char szTmp[128];
	PCInfoCharCli pChar;
	PCLibInfoCharCli pLibInfoChar;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	pLibInfoChar	= m_pMgrData->GetLibInfoChar();
	pChar	= (PCInfoCharCli)pLibInfoChar->GetPtr(m_dwCharID);
	if (pChar == NULL) {
		return;
	}

	DrawFrame(2);

	hDC	= m_pDib->Lock();
	SetBkColor(hDC, RGB(255, 255, 255));
#if 0
	for (i = 0; i < 2; i ++) {
		m_pDib->BltFrom256(16, 32 + i * 16, 16, 16, m_pDibSystem, 224 + i * 16, 32, TRUE);
	}
	for (i = 0; i < 8; i ++) {
		m_pDib->BltFrom256(16, 32 + 32 + i * 16, 16, 16, m_pDibSystem, 128 + i * 16, 48, TRUE);
	}
#endif

//	TextOut2(hDC, m_hFont, 32, 0, _T("強さ"), RGB(0, 0, 0));
	TextOut2(hDC, m_hFont, 16 + 8 * 0, 16 * 1, _T("レベル"), RGB(0, 0, 0));
#if 0
	char szTmp[128];

	TextOut2(hDC, 32 + 8 * 9, 16 * 1, _T(":"), RGB(255, 255, 255));
	sprintf_s(szTmp, sizeof(szTmp), "%d", pChar->m_byLevel);
	TextOut2(hDC, 32 + 8 * 10, 16 * 1, szTmp, RGB(150, 255, 200));

	TextOut2(hDC, 32 + 8 * 0, 16 * 2, _T("力"), RGB(255, 255, 255));
	TextOut2(hDC, 32 + 8 * 9, 16 * 2, _T(":"), RGB(255, 255, 255));
	sprintf_s(szTmp, sizeof(szTmp), "%d", pChar->m_byPW);
	TextOut2(hDC, 32 + 8 * 10, 16 * 2, szTmp, RGB(150, 255, 200));

	TextOut2(hDC, 32 + 8 * 0, 16 * 3, _T("素早さ"), RGB(255, 255, 255));
	TextOut2(hDC, 32 + 8 * 9, 16 * 3, _T(":"), RGB(255, 255, 255));
	sprintf_s(szTmp, sizeof(szTmp), "%d", pChar->m_byDEX);
	TextOut2(hDC, 32 + 8 * 10, 16 * 3, szTmp, RGB(150, 255, 200));

	TextOut2(hDC, 32 + 8 * 0, 16 * 4, _T("体力"), RGB(255, 255, 255));
	TextOut2(hDC, 32 + 8 * 9, 16 * 4, _T(":"), RGB(255, 255, 255));
	sprintf_s(szTmp, sizeof(szTmp), "%d", pChar->m_byST);
	TextOut2(hDC, 32 + 8 * 10, 16 * 4, szTmp, RGB(150, 255, 200));

	TextOut2(hDC, 32 + 8 * 0, 16 * 5, _T("賢さ"), RGB(255, 255, 255));
	TextOut2(hDC, 32 + 8 * 9, 16 * 5, _T(":"), RGB(255, 255, 255));
	sprintf_s(szTmp, sizeof(szTmp), "%d", pChar->m_byIT);
	TextOut2(hDC, 32 + 8 * 10, 16 * 5, szTmp, RGB(150, 255, 200));

	TextOut2(hDC, 32 + 8 * 0, 16 * 6, _T("運のよさ"), RGB(255, 255, 255));
	TextOut2(hDC, 32 + 8 * 9, 16 * 6, _T(":"), RGB(255, 255, 255));
	sprintf_s(szTmp, sizeof(szTmp), "%d", pChar->m_byLK);
	TextOut2(hDC, 32 + 8 * 10, 16 * 6, szTmp, RGB(150, 255, 200));

	TextOut2(hDC, 32 + 8 * 0, 16 * 7, _T("最大HP"), RGB(255, 255, 255));
	TextOut2(hDC, 32 + 8 * 9, 16 * 7, _T(":"), RGB(255, 255, 255));
	sprintf_s(szTmp, sizeof(szTmp), "%d", pChar->m_dwMaxHP);
	TextOut2(hDC, 32 + 8 * 10, 16 * 7, szTmp, RGB(150, 255, 200));

	TextOut2(hDC, 32 + 8 * 0, 16 * 8, _T("最大Mana"), RGB(255, 255, 255));
	TextOut2(hDC, 32 + 8 * 9, 16 * 8, _T(":"), RGB(255, 255, 255));
	sprintf_s(szTmp, sizeof(szTmp), "%d", pChar->m_dwMaxMana);
	TextOut2(hDC, 32 + 8 * 10, 16 * 8, szTmp, RGB(150, 255, 200));

	TextOut2(hDC, 32 + 8 * 0, 16 * 9, _T("攻撃力"), RGB(255, 255, 255));
	TextOut2(hDC, 32 + 8 * 9, 16 * 9, _T(":"), RGB(255, 255, 255));
	sprintf_s(szTmp, sizeof(szTmp), "%d", pChar->m_byAT);
	TextOut2(hDC, 32 + 8 * 10, 16 * 9, szTmp, RGB(150, 255, 200));

	TextOut2(hDC, 32 + 8 * 0, 16 * 10, _T("防御力"), RGB(255, 255, 255));
	TextOut2(hDC, 32 + 8 * 9, 16 * 10, _T(":"), RGB(255, 255, 255));
	sprintf_s(szTmp, sizeof(szTmp), "%d", pChar->m_byDF);
	TextOut2(hDC, 32 + 8 * 10, 16 * 10, szTmp, RGB(150, 255, 200));

	TextOut2(hDC, 32 + 8 * 0, 16 * 11, _T("魔法防御"), RGB(255, 255, 255));
	TextOut2(hDC, 32 + 8 * 9, 16 * 11, _T(":"), RGB(255, 255, 255));
	sprintf_s(szTmp, sizeof(szTmp), "%d", pChar->m_byMagicDF);
	TextOut2(hDC, 32 + 8 * 10, 16 * 11, szTmp, RGB(150, 255, 200));

	TextOut2(hDC, 32 + 8 * 0, 16 * 12, _T("Exp:"), RGB(255, 255, 255));
	sprintf_s(szTmp, sizeof(szTmp), "%I64d", pChar->m_ulExp);
	TextOut2(hDC, 32 + 8 * 5, 16 * 12, szTmp, RGB(150, 255, 200));
#endif
	m_pDib->Unlock();

	m_dwTimeDrawStart = timeGetTime();

Exit:
	pDst->BltAlpha2(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, 15, TRUE);
}


void CWindowSTATUS::SetID(DWORD dwID)
{
	m_dwCharID	= dwID;
	m_dwTimeDrawStart	= 0;
}
