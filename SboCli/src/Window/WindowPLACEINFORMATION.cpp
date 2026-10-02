/// @file WindowPLACEINFORMATION.cpp
/// @brief 場所情報ウィンドウクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2009/02/11
/// @copyright Copyright(C)URARA-works 2009

#include "StdAfx.h"
#include "Img32.h"
#include "InfoMapBase.h"
#include "InfoCharCli.h"
#include "MgrWindow.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "WindowPLACEINFORMATION.h"

CWindowPLACEINFORMATION::CWindowPLACEINFORMATION()
{
	m_nID	= WINDOWTYPE_PLACEINFORMATION;
	m_ptViewPos.x	= 0;
	m_ptViewPos.y	= 0;
	m_sizeWindow.cx	= 480;
	m_sizeWindow.cy	= 24;
}


CWindowPLACEINFORMATION::~CWindowPLACEINFORMATION()
{
}


void CWindowPLACEINFORMATION::Create(CMgrData *pMgrData)
{
	CWindowBase::Create(pMgrData);

	m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey(0);
}


void CWindowPLACEINFORMATION::Draw(PCImg32 pDst)
{
	int nTmp;
	float fTmp;
	HDC hDC;
	PCInfoMapBase pInfoMap;
	PCInfoCharCli pInfoChar;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}
	pInfoMap = m_pMgrData->GetMap();
	if (pInfoMap == NULL) {
		goto Exit;
	}
	pInfoChar = m_pMgrData->GetPlayerChar();
	if (pInfoChar == NULL) {
		goto Exit;
	}

	// 帯と欄は 2 ドット単位の枠で描き、アイコンだけ元の絵から貼る
	m_pDib->FillRect(0, 0, m_sizeWindow.cx, m_sizeWindow.cy, RGB(0, 0, 0));
	DrawFrame(0, 0, m_sizeWindow.cx, m_sizeWindow.cy, 5);
	DrawFrame(2, 2, 192, 20, 6);
	DrawFrame(2, 2, 40, 20, 7);
	m_pDib->BltFrom256(198, 2, 16, 19, m_pDibSystem, 198, 626, TRUE);
	m_pDib->BltFrom256(284, 2, 19, 19, m_pDibSystem, 285, 626, TRUE);
	m_pDib->BltFrom256(374, 2, 17, 19, m_pDibSystem, 375, 626, TRUE);

	nTmp = 0;
	if (pInfoChar->m_dwMaxHP > 0) {
		fTmp = (float)pInfoChar->m_dwHP * 100.0f / (float)pInfoChar->m_dwMaxHP;
		nTmp = (int)fTmp;
	}
	DrawGauge(216, 10, 66, nTmp, RGB(255, 98, 20));
	DrawGauge(304, 10, 68, 0, RGB(255, 98, 20));
	DrawGauge(394, 10, 80, 0, RGB(255, 98, 20));

	hDC	= m_pDib->Lock();

	TextOutCenter(hDC, m_hFont12, 2, 40, 4, _T("場所"), RGB(255, 255, 255));
	TextOut2(hDC, m_hFont12, 46, 4, (LPCTSTR)pInfoMap->m_strMapName, RGB(1, 1, 1));

	m_pDib->Unlock();

	m_dwTimeDrawStart = timeGetTime();
Exit:
	pDst->Blt(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}
