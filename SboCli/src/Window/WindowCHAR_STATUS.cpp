/// @file WindowCHAR_STATUS.cpp
/// @brief キャラ-ステータスウィンドウクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/10/27
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "InfoCharCli.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowCHAR_STATUS.h"
#include "../Platform/SdlFont.h"


CWindowCHAR_STATUS::CWindowCHAR_STATUS()
{
	m_nPosMax	= 1;
	m_bInput	= TRUE;
	m_nID	= WINDOWTYPE_CHAR_STATUS;
	m_ptViewPos.x	= 8 * 2;
	m_ptViewPos.y	= 8 * 3;
	m_sizeWindow.cx	= 16 * 2 + 8 * 24;
	m_sizeWindow.cy	= 16 * 2 + 8 * 50;
}


CWindowCHAR_STATUS::~CWindowCHAR_STATUS()
{
}


void CWindowCHAR_STATUS::Create(CMgrData *pMgrData)
{
	CWindowBase::Create(pMgrData);

	m_bActive = TRUE;
	m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey(0);
}


// 1 行の高さと行送り（小さい文字 16px + 上下 2px）
#define CHARSTATUS_CELL_H	(20)
#define CHARSTATUS_ROW_H	(22)
// 2 列に並べるときの欄の幅と右列の位置
#define CHARSTATUS_COL_W	(96)
#define CHARSTATUS_COL2_X	(12 + CHARSTATUS_COL_W + 8)

void CWindowCHAR_STATUS::Draw(PCImg32 pDst)
{
	int nLevel, x, y;
	HDC hDC;
	PCInfoCharCli pInfoChar;
	CmyString strTmp;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	pInfoChar = m_pMgrData->GetPlayerChar();

	hDC	= m_pDib->Lock();

	DrawFrame(5);

	// キャラ情報
	DrawTab(hDC, 12, 6, _T("キャラ情報(J)"));
	y = 24;
	DrawFrame(4, y, m_sizeWindow.cx - 8, 80, 6);
	y += 8;
	DrawCell(hDC, 12, y, 200, 44, _T("名前"), (LPCTSTR)pInfoChar->m_strCharName);
	y += CHARSTATUS_ROW_H;
	DrawCell(hDC, 12, y, 128, 44, _T("職業"), _T(""));
	strTmp.Format(_T("%d"), pInfoChar->m_wLevel);
	DrawCell(hDC, 12 + 128 + 8, y, 64, 24, _T("LV"), (LPCTSTR)strTmp);
	y += CHARSTATUS_ROW_H;
	DrawCell(hDC, 12, y, 200, 44, _T("所属"), _T(""));

	// 基本値
	y = 110;
	DrawTab(hDC, 12, y, _T("基本値"));
	y += 18;
	DrawFrame(4, y, m_sizeWindow.cx - 8, 80, 6);
	y += 8;
	strTmp.Format(_T("%d"), pInfoChar->m_dwMaxHP);
	DrawCell(hDC, 12, y, CHARSTATUS_COL_W, 44, _T("HP"), (LPCTSTR)strTmp);
	strTmp.Format(_T("%d"), pInfoChar->m_dwMaxSP);
	DrawCell(hDC, CHARSTATUS_COL2_X, y, CHARSTATUS_COL_W, 44, _T("MP"), (LPCTSTR)strTmp);
	y += CHARSTATUS_ROW_H;
	strTmp.Format(_T("%d"), pInfoChar->m_wPower);
	DrawCell(hDC, 12, y, CHARSTATUS_COL_W, 44, _T("力"), (LPCTSTR)strTmp);
	strTmp.Format(_T("%d"), pInfoChar->m_wMagic);
	DrawCell(hDC, CHARSTATUS_COL2_X, y, CHARSTATUS_COL_W, 44, _T("魔力"), (LPCTSTR)strTmp);
	y += CHARSTATUS_ROW_H;
	strTmp.Format(_T("%d"), pInfoChar->m_wStrength);
	DrawCell(hDC, 12, y, CHARSTATUS_COL_W, 44, _T("体力"), (LPCTSTR)strTmp);
	strTmp.Format(_T("%d"), pInfoChar->m_wSkillful);
	DrawCell(hDC, CHARSTATUS_COL2_X, y, CHARSTATUS_COL_W, 44, _T("器用"), (LPCTSTR)strTmp);

	// ステータス
	y = 214;
	DrawTab(hDC, 12, y, _T("ステータス"));
	y += 18;
	DrawFrame(4, y, m_sizeWindow.cx - 8, 192, 6);
	y += 8;
	strTmp.Format(_T("%d"), pInfoChar->m_wPAtack);
	DrawCell(hDC, 12, y, CHARSTATUS_COL_W, 44, _T("攻撃"), (LPCTSTR)strTmp);
	strTmp.Format(_T("%d"), pInfoChar->m_wPDefense);
	DrawCell(hDC, CHARSTATUS_COL2_X, y, CHARSTATUS_COL_W, 44, _T("防御"), (LPCTSTR)strTmp);
	y += CHARSTATUS_ROW_H;
	strTmp.Format(_T("%d"), pInfoChar->m_wAbillityAT);
	DrawCell(hDC, 12, y, CHARSTATUS_COL_W, 44, _T("攻術"), (LPCTSTR)strTmp);
	strTmp.Format(_T("%d"), pInfoChar->m_wAbillityDF);
	DrawCell(hDC, CHARSTATUS_COL2_X, y, CHARSTATUS_COL_W, 44, _T("防術"), (LPCTSTR)strTmp);
	y += CHARSTATUS_ROW_H;
	strTmp.Format(_T("%d"), pInfoChar->m_wPMagic);
	DrawCell(hDC, 12, y, CHARSTATUS_COL_W, 44, _T("魔攻"), (LPCTSTR)strTmp);
	strTmp.Format(_T("%d"), pInfoChar->m_wPMagicDefense);
	DrawCell(hDC, CHARSTATUS_COL2_X, y, CHARSTATUS_COL_W, 44, _T("魔防"), (LPCTSTR)strTmp);
	y += CHARSTATUS_ROW_H;
	strTmp.Format(_T("%d%%"), pInfoChar->m_wPHitAverage);
	DrawCell(hDC, 12, y, CHARSTATUS_COL_W, 44, _T("命中"), (LPCTSTR)strTmp);
	strTmp.Format(_T("%d%%"), pInfoChar->m_wPAvoidAverage);
	DrawCell(hDC, CHARSTATUS_COL2_X, y, CHARSTATUS_COL_W, 44, _T("回避"), (LPCTSTR)strTmp);
	y += CHARSTATUS_ROW_H;
	strTmp.Format(_T("%d%%"), pInfoChar->m_wPCriticalAverage);
	DrawCell(hDC, 12, y, CHARSTATUS_COL_W, 44, _T("必殺"), (LPCTSTR)strTmp);
	y += CHARSTATUS_ROW_H;
	strTmp.Format(_T("%d%%"), pInfoChar->m_wAttrFire);
	DrawCell(hDC, 12, y, CHARSTATUS_COL_W, 44, _T("火"), (LPCTSTR)strTmp);
	strTmp.Format(_T("%d%%"), pInfoChar->m_wAttrWater);
	DrawCell(hDC, CHARSTATUS_COL2_X, y, CHARSTATUS_COL_W, 44, _T("水"), (LPCTSTR)strTmp);
	y += CHARSTATUS_ROW_H;
	strTmp.Format(_T("%d%%"), pInfoChar->m_wAttrWind);
	DrawCell(hDC, 12, y, CHARSTATUS_COL_W, 44, _T("風"), (LPCTSTR)strTmp);
	strTmp.Format(_T("%d%%"), pInfoChar->m_wAttrEarth);
	DrawCell(hDC, CHARSTATUS_COL2_X, y, CHARSTATUS_COL_W, 44, _T("土"), (LPCTSTR)strTmp);
	y += CHARSTATUS_ROW_H;
	strTmp.Format(_T("%d%%"), pInfoChar->m_wAttrLight);
	DrawCell(hDC, 12, y, CHARSTATUS_COL_W, 44, _T("光"), (LPCTSTR)strTmp);
	strTmp.Format(_T("%d%%"), pInfoChar->m_wAttrDark);
	DrawCell(hDC, CHARSTATUS_COL2_X, y, CHARSTATUS_COL_W, 44, _T("闇"), (LPCTSTR)strTmp);

	m_pDib->Unlock();

	m_dwTimeDrawStart = timeGetTime();

Exit:
	nLevel = 100;
	if (m_bActive == FALSE) {
		nLevel = 60;
	}
	pDst->BltLevel(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, nLevel, TRUE);
}


void CWindowCHAR_STATUS::DrawCell(HDC hDC, int x, int y, int cx, int cxLabel, LPCTSTR pszLabel, LPCTSTR pszValue)
{
	int nTextW, nTextH;

	DrawFrame(x, y, cx, CHARSTATUS_CELL_H, 6);
	// 右端を消す描き方だと 8px 手前で切れて細い線が残るので、右も角丸で描く
	DrawFrame(x, y, cxLabel, CHARSTATUS_CELL_H, 7);

	// 見出しは欄の中央にそろえる（2 ドット単位）
	nTextW = nTextH = 0;
	SdlFontGetTextExtent((void *)m_hFont12, pszLabel, (int)_tcslen(pszLabel), &nTextW, &nTextH);
	TextOut2(hDC, m_hFont12, x + ((cxLabel - nTextW) / 2 & ~1), y + 2, pszLabel, RGB(255, 255, 255));
	if (pszValue[0] != 0) {
		TextOut2(hDC, m_hFont12, x + cxLabel + 4, y + 2, pszValue, RGB(1, 1, 1));
	}
}


void CWindowCHAR_STATUS::DrawTab(HDC hDC, int x, int y, LPCTSTR pszTitle)
{
	int nTextW, nTextH;

	nTextW = nTextH = 0;
	SdlFontGetTextExtent((void *)m_hFont12, pszTitle, (int)_tcslen(pszTitle), &nTextW, &nTextH);
	DrawFrame(x, y, WND_ALIGN8(nTextW + 16), 24, 7);
	// 見出しの文字は下の枠に隠れないよう上に寄せる
	TextOut2(hDC, m_hFont12, x + 8, y + 2, pszTitle, RGB(255, 255, 255));
}


BOOL CWindowCHAR_STATUS::OnX(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_bDelete = TRUE;
	m_pMgrSound->PlaySound(SOUNDID_OK_PI73);

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowCHAR_STATUS::OnZ(BOOL bDown)
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


BOOL CWindowCHAR_STATUS::OnJ(BOOL bDown)
{
	return OnZ(bDown);
}
