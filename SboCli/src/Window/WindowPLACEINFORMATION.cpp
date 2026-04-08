/// @file WindowPLACEINFORMATION.cpp
/// @brief 場所情報ウィンドウクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2009/02/11
/// @copyright Copyright(C)URARA-works 2009

#include "stdafx.h"
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
	m_sizeWindow.cy	= 23;
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

	m_pDib->BltFrom256(0, 0, m_sizeWindow.cx, m_sizeWindow.cy, m_pDibSystem, 0, 624, TRUE);

	hDC	= m_pDib->Lock();

	TextOut2(hDC, m_hFont12, 34, 6, (LPCTSTR)pInfoMap->m_strMapName, RGB(1, 1, 1));

	fTmp = (float)pInfoChar->m_dwHP * 100.0f / (float)pInfoChar->m_dwMaxHP;
	nTmp = (int)((float)65 * fTmp / 100.0f);
	m_pDib->BltFrom256(216, 13, nTmp, 7, m_pDibSystem, 0, 758, TRUE);

	m_pDib->Unlock();

	m_dwTimeDrawStart = timeGetTime();
Exit:
	pDst->Blt(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}
