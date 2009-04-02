/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:WindowCHAR_STATUS.cpp										 */
/* 内容			:キャラ-ステータスウィンドウクラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/10/27													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoCharCli.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowCHAR_STATUS.h"


/* ========================================================================= */
/* 関数名	:CWindowCHAR_STATUS::CWindowCHAR_STATUS							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/10/27														 */
/* ========================================================================= */

CWindowCHAR_STATUS::CWindowCHAR_STATUS()
{
	m_nPosMax		= 1;
	m_bInput		= TRUE;
	m_nID			= WINDOWTYPE_CHAR_STATUS;
	m_ptViewPos.x	= 8 * 2;
	m_ptViewPos.y	= 16 * 3;
	m_sizeWindow.cx	= 16 * 2 + 8 * 22;
	m_sizeWindow.cy	= 16 * 2 + 8 * 41;
}


/* ========================================================================= */
/* 関数名	:CWindowCHAR_STATUS::~CWindowCHAR_STATUS						 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/10/27														 */
/* ========================================================================= */

CWindowCHAR_STATUS::~CWindowCHAR_STATUS()
{
}


/* ========================================================================= */
/* 関数名	:CWindowCHAR_STATUS::Create										 */
/* 内容		:作成															 */
/* 日付		:2008/10/27														 */
/* ========================================================================= */

void CWindowCHAR_STATUS::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_bActive = TRUE;
	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);
}


/* ========================================================================= */
/* 関数名	:CWindowCHAR_STATUS::Draw										 */
/* 内容		:描画															 */
/* 日付		:2008/10/27														 */
/* ========================================================================= */

void CWindowCHAR_STATUS::Draw(PCImg32 pDst)
{
	int nLevel, x, y;
	HDC hDC;
	HFONT hFontOld;
	COLORREF clText;
	PCInfoCharCli pInfoChar;
	CmyString strTmp;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	pInfoChar = m_pMgrData->GetPlayerChar ();

	clText		= RGB (1, 1, 1);
	hDC			= m_pDib->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont12);
	SetBkMode (hDC, TRANSPARENT);

	DrawFrame (5);
	DrawFrame (12, 7, 80, 24, 7);
	TextOut2 (hDC, 12 + 5, 7 + 4, "キャラ情報(J)", RGB (255, 255, 255));
	y = 8 * 3;
	DrawFrame (4, y, 200, 64, 6);
	y += 6;
	x = 12;
	DrawFrame (x, y, 184, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "名前", RGB (255, 255, 255));
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)pInfoChar->m_strCharName, RGB (1, 1, 1));
	y += 18;
	DrawFrame (x, y, 120, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "職業", RGB (255, 255, 255));
	x = 12 + 8 * 16;
	DrawFrame (x, y, 56, 16, 6);
	DrawFrame (x, y, 32, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "LV", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wLevel);
	TextOut2 (hDC, x + 27, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	y += 18;
	x = 12;
	DrawFrame (x, y, 184, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "所属", RGB (255, 255, 255));

	y = 8 * 11 + 7;
	x = 12;
	DrawFrame (x, y, 48, 24, 7);
	TextOut2 (hDC, x + 5, y + 4, "基本値", RGB (255, 255, 255));
	y += 16;
	DrawFrame (4, y, 200, 64, 6);
	y += 6;
	x = 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5 + 3, y + 2, "HP", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_dwMaxHP);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	x = 12 + 8 * 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5 + 3, y + 2, "MP", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_dwMaxSP);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	y += 18;
	x = 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5 + 6, y + 2, "力", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wPower);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	x = 12 + 8 * 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "魔力", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wMagic);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	y += 18;
	x = 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "体力", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wStrength);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	x = 12 + 8 * 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "器用", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wSkillful);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));

	y += 8 * 4;
	x = 12;
	DrawFrame (x, y, 64, 24, 7);
	TextOut2 (hDC, x + 5, y + 4, "ステータス", RGB (255, 255, 255));
	y += 16;
	DrawFrame (4, y, 200, 152, 6);
	y += 6;
	x = 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "攻撃", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wPAtack);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	x = 12 + 8 * 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "防御", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wPDefense);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	y += 18;
	x = 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "攻術", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wAbillityAT);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	x = 12 + 8 * 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "防術", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wAbillityDF);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	y += 18;
	x = 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "魔攻", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wPMagic);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	x = 12 + 8 * 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "魔防", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wPMagicDefense);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	y += 18;
	x = 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "命中", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wPHitAverage);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	x = 12 + 8 * 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "回避", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wPAvoidAverage);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	y += 18;
	x = 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "必殺", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wPCriticalAverage);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	y += 18;
	x = 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5 + 6, y + 2, "火", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wAttrFire);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	x = 12 + 8 * 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5 + 6, y + 2, "水", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wAttrWater);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	y += 18;
	x = 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5 + 6, y + 2, "風", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wAttrWind);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	x = 12 + 8 * 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5 + 6, y + 2, "土", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wAttrEarth);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	y += 18;
	x = 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5 + 6, y + 2, "光", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wAttrLight);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	x = 12 + 8 * 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5 + 6, y + 2, "闇", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wAttrDark);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));



#if 0
	y = 0;
	TextOut2 (hDC, 16, 12 + 12 * y, (LPCSTR)pInfoChar->m_strCharName, clText);
	y ++;
	strTmp = "レベル";
	TextOut2 (hDC, 16, 16 + 12 * y, (LPCSTR)strTmp, clText);
	strTmp.Format ("%d", pInfoChar->m_wLevel);
	TextOut2 (hDC, 16 + 12 * 7, 16 + 12 * y, (LPCSTR)strTmp, clText);
	y ++;
	strTmp = "HP";
	TextOut2 (hDC, 16, 16 + 12 * y, (LPCSTR)strTmp, clText);
	strTmp.Format ("%d", pInfoChar->m_dwHP);
	TextOut2 (hDC, 16 + 12 * 7, 16 + 12 * y, (LPCSTR)strTmp, clText);
	y ++;
	strTmp = "SP";
	TextOut2 (hDC, 16, 16 + 12 * y, (LPCSTR)strTmp, clText);
	strTmp.Format ("%d", pInfoChar->m_dwSP);
	TextOut2 (hDC, 16 + 12 * 7, 16 + 12 * y, (LPCSTR)strTmp, clText);
#endif

	SelectObject (hDC, hFontOld);
	m_pDib->Unlock ();

	m_dwTimeDrawStart = timeGetTime ();

Exit:
	nLevel = 100;
	if (m_bActive == FALSE) {
		nLevel = 60;
	}
	pDst->BltLevel (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, nLevel, TRUE);
}


/* ========================================================================= */
/* 関数名	:CWindowCHAR_STATUS::OnX										 */
/* 内容		:キーハンドラ(X)												 */
/* 日付		:2008/10/27														 */
/* ========================================================================= */

BOOL CWindowCHAR_STATUS::OnX(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_bDelete = TRUE;
	m_pMgrSound->PlaySound (SOUNDID_OK_PI73);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWindowCHAR_STATUS::OnZ										 */
/* 内容		:キーハンドラ(Z)												 */
/* 日付		:2008/10/27														 */
/* ========================================================================= */

BOOL CWindowCHAR_STATUS::OnZ(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_bDelete = TRUE;
	m_pMgrSound->PlaySound (SOUNDID_CANCEL);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWindowCHAR_STATUS::OnJ										 */
/* 内容		:キーハンドラ(J)												 */
/* 日付		:2009/04/02														 */
/* ========================================================================= */

BOOL CWindowCHAR_STATUS::OnJ(BOOL bDown)
{
	return OnZ (bDown);
}

/* Copyright(C)URARA-works 2008 */
