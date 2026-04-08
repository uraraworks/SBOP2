/// @file WindowCHAR_STATUS2.cpp
/// @brief キャラ-ステータス2ウィンドウクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/10/27
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "InfoCharCli.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowCHAR_STATUS2.h"


CWindowCHAR_STATUS2::CWindowCHAR_STATUS2()
{
	m_nPosMax	= 1;
	m_nID	= WINDOWTYPE_CHAR_STATUS2;
	m_ptViewPos.x	= 8 * 2;
	m_ptViewPos.y	= 16 * 3 + 16 * 2 + 12 * 3;
	m_sizeWindow.cx	= 16 * 2 + 12 * 10;
	m_sizeWindow.cy	= 16 * 2 + 12 * 16;
}


CWindowCHAR_STATUS2::~CWindowCHAR_STATUS2()
{
}


void CWindowCHAR_STATUS2::Create(CMgrData *pMgrData)
{
	CWindowBase::Create(pMgrData);

	m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey(0);
}


void CWindowCHAR_STATUS2::Draw(PCImg32 pDst)
{
	int nLevel, y;
	HDC hDC;
	COLORREF clText;
	PCInfoCharCli pInfoChar;
	CmyString strTmp;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	pInfoChar = m_pMgrData->GetPlayerChar();
	DrawFrame();

	clText	= RGB(1, 1, 1);
	hDC	= m_pDib->Lock();
	SetBkMode(hDC, TRANSPARENT);

	clText = RGB(1, 1, 1);

	y = 0;
	strTmp = "スタミナ";
	TextOut2(hDC, m_hFont12,16, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	strTmp.Format(_T("%d"), pInfoChar->m_wStamina);
	TextOut2(hDC, m_hFont12,16 + 12 * 7, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	y ++;
	strTmp = "腕力";
	TextOut2(hDC, m_hFont12,16, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	strTmp.Format(_T("%d"), pInfoChar->m_wPower);
	TextOut2(hDC, m_hFont12,16 + 12 * 7, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	y ++;
	strTmp = "体力";
	TextOut2(hDC, m_hFont12,16, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	strTmp.Format(_T("%d"), pInfoChar->m_wStrength);
	TextOut2(hDC, m_hFont12,16 + 12 * 7, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	y ++;
	strTmp = "魔力";
	TextOut2(hDC, m_hFont12,16, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	strTmp.Format(_T("%d"), pInfoChar->m_wMagic);
	TextOut2(hDC, m_hFont12,16 + 12 * 7, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	y ++;
	strTmp = "器用";
	TextOut2(hDC, m_hFont12,16, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	strTmp.Format(_T("%d"), pInfoChar->m_wSkillful);
	TextOut2(hDC, m_hFont12,16 + 12 * 7, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	y ++;
	strTmp = "攻撃技能";
	TextOut2(hDC, m_hFont12,16, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	strTmp.Format(_T("%d"), pInfoChar->m_wAbillityAT);
	TextOut2(hDC, m_hFont12,16 + 12 * 7, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	y ++;
	strTmp = "防御技能";
	TextOut2(hDC, m_hFont12,16, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	strTmp.Format(_T("%d"), pInfoChar->m_wAbillityDF);
	TextOut2(hDC, m_hFont12,16 + 12 * 7, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	y ++;
	strTmp = "攻撃力";
	TextOut2(hDC, m_hFont12,16, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	strTmp.Format(_T("%d"), pInfoChar->m_wPAtack);
	TextOut2(hDC, m_hFont12,16 + 12 * 7, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	y ++;
	strTmp = "防御力";
	TextOut2(hDC, m_hFont12,16, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	strTmp.Format(_T("%d"), pInfoChar->m_wPDefense);
	TextOut2(hDC, m_hFont12,16 + 12 * 7, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	y ++;
	strTmp = "魔法力";
	TextOut2(hDC, m_hFont12,16, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	strTmp.Format(_T("%d"), pInfoChar->m_wPMagic);
	TextOut2(hDC, m_hFont12,16 + 12 * 7, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	y ++;
	strTmp = "魔法防御力";
	TextOut2(hDC, m_hFont12,16, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	strTmp.Format(_T("%d"), pInfoChar->m_wPMagicDefense);
	TextOut2(hDC, m_hFont12,16 + 12 * 7, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	y ++;
	strTmp = "命中率";
	TextOut2(hDC, m_hFont12,16, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	strTmp.Format(_T("%d"), pInfoChar->m_wPHitAverage);
	TextOut2(hDC, m_hFont12,16 + 12 * 7, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	y ++;
	strTmp = "回避率";
	TextOut2(hDC, m_hFont12,16, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	strTmp.Format(_T("%d"), pInfoChar->m_wPAvoidAverage);
	TextOut2(hDC, m_hFont12,16 + 12 * 7, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	y ++;
	strTmp = "クリティカル率";
	TextOut2(hDC, m_hFont12,16, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	strTmp.Format(_T("%d"), pInfoChar->m_wPCriticalAverage);
	TextOut2(hDC, m_hFont12,16 + 12 * 7, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	y ++;
	strTmp = "最大HP";
	TextOut2(hDC, m_hFont12,16, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	strTmp.Format(_T("%d"), pInfoChar->m_dwMaxHP);
	TextOut2(hDC, m_hFont12,16 + 12 * 7, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	y ++;
	strTmp = "最大SP";
	TextOut2(hDC, m_hFont12,16, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	strTmp.Format(_T("%d"), pInfoChar->m_dwMaxSP);
	TextOut2(hDC, m_hFont12,16 + 12 * 7, 12 + 12 * y, (LPCTSTR)strTmp, clText);
	y ++;
	strTmp.Format(_T("Exp  %ld"), pInfoChar->m_dwExp);
	TextOut2(hDC, m_hFont12,16, 12 + 12 * y, (LPCTSTR)strTmp, clText);

	m_pDib->Unlock();

	m_dwTimeDrawStart = timeGetTime();

Exit:
	nLevel = 100;
	pDst->BltLevel(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, nLevel, TRUE);
}
