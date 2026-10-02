/// @file WindowCHAR_STATUS4.cpp
/// @brief 場所情報ウィンドウクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2009/02/15
/// @copyright Copyright(C)URARA-works 2009

#include "StdAfx.h"
#include "Img32.h"
#include "InfoMapBase.h"
#include "InfoCharCli.h"
#include "MgrWindow.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "WindowCHAR_STATUS4.h"
#include "../Platform/SdlFont.h"

// 行の位置（小さい文字 16px ＋縁取りが重ならないよう 18px 送り）とゲージの位置
#define STATUS4_ROW_Y(n)	(26 + 18 * (n))
#define STATUS4_GAUGE_X	(40)
#define STATUS4_GAUGE_W	(128)
// 数値の右端（ゲージの右に黒い文字で並べる。ゲージに重ねると読みにくいため）
#define STATUS4_NUM_R	(248)

CWindowCHAR_STATUS4::CWindowCHAR_STATUS4()
{
	m_nID	= WINDOWTYPE_CHAR_STATUS4;
	m_ptViewPos.x	= 0;
	m_ptViewPos.y	= 24;
	m_sizeWindow.cx	= 256;
	m_sizeWindow.cy	= 84;
}


CWindowCHAR_STATUS4::~CWindowCHAR_STATUS4()
{
}


void CWindowCHAR_STATUS4::Create(CMgrData *pMgrData)
{
	CWindowBase::Create(pMgrData);

	m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey(0);
}


void CWindowCHAR_STATUS4::Draw(PCImg32 pDst)
{
	int nTmp;
	float fTmp;
	HDC hDC;
	PCInfoMapBase pInfoMap;
	PCInfoCharCli pInfoChar;
	CmyString strTmp;

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

	// 絵を貼るのをやめ、枠・ゲージ・文字を 2 ドット単位で描く（見出しも文字なので英語化しやすい）
	m_pDib->FillRect(0, 0, m_sizeWindow.cx, m_sizeWindow.cy, RGB(0, 0, 0));
	DrawFrame(0, 0, m_sizeWindow.cx, m_sizeWindow.cy, 5);
	m_pDib->FillRect(4, 2, m_sizeWindow.cx - 8, 20, GetFrameBackColor(7));
	m_pDib->FillRect(2, 4, m_sizeWindow.cx - 4, 18, GetFrameBackColor(7));

	nTmp = 0;
	if (pInfoChar->m_dwMaxHP > 0) {
		fTmp = (float)pInfoChar->m_dwHP * 100.0f / (float)pInfoChar->m_dwMaxHP;
		nTmp = (int)fTmp;
	}
	DrawGauge(STATUS4_GAUGE_X, STATUS4_ROW_Y(0) + 4, STATUS4_GAUGE_W, nTmp, RGB(246, 49, 55));
	nTmp = 0;
	if (pInfoChar->m_dwMaxSP > 0) {
		fTmp = (float)pInfoChar->m_dwSP * 100.0f / (float)pInfoChar->m_dwMaxSP;
		nTmp = (int)fTmp;
	}
	DrawGauge(STATUS4_GAUGE_X, STATUS4_ROW_Y(1) + 4, STATUS4_GAUGE_W, nTmp, RGB(50, 110, 240));
	DrawGauge(STATUS4_GAUGE_X, STATUS4_ROW_Y(2) + 4, STATUS4_GAUGE_W, 0, RGB(112, 186, 34));

	hDC	= m_pDib->Lock();

	TextOut2(hDC, m_hFont12, 8, 4, (LPCTSTR)pInfoChar->m_strCharName, RGB(255, 255, 255), TRUE, RGB(69, 46, 13));

	TextOut2(hDC, m_hFont12, 10, STATUS4_ROW_Y(0), _T("HP"), RGB(150, 96, 40));
	TextOut2(hDC, m_hFont12, 10, STATUS4_ROW_Y(1), _T("MP"), RGB(150, 96, 40));
	TextOut2(hDC, m_hFont12, 6, STATUS4_ROW_Y(2), _T("ACT"), RGB(150, 96, 40));

	// 数値はゲージの右に右寄せで描く
	strTmp.Format(_T("%d/%d"), pInfoChar->m_dwHP, pInfoChar->m_dwMaxHP);
	DrawNumberRight(hDC, STATUS4_NUM_R, STATUS4_ROW_Y(0), (LPCTSTR)strTmp);
	strTmp.Format(_T("%d/%d"), pInfoChar->m_dwSP, pInfoChar->m_dwMaxSP);
	DrawNumberRight(hDC, STATUS4_NUM_R, STATUS4_ROW_Y(1), (LPCTSTR)strTmp);

	m_pDib->Unlock();

	m_dwTimeDrawStart = timeGetTime();
Exit:
	pDst->Blt(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}


void CWindowCHAR_STATUS4::DrawNumberRight(HDC hDC, int xRight, int y, LPCTSTR pszText)
{
	int nTextW, nTextH;

	nTextW = nTextH = 0;
	SdlFontGetTextExtent((void *)m_hFont12, pszText, lstrlen(pszText), &nTextW, &nTextH);
	TextOut2(hDC, m_hFont12, (xRight - nTextW) & ~1, y, pszText, RGB(1, 1, 1));
}
