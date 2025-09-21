/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名	:WindowCHAR_STATUS4.cpp										 */
/* 内容			:場所情報ウィンドウクラス 実装ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/02/15													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "InfoMapBase.h"
#include "InfoCharCli.h"
#include "MgrWindow.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "WindowCHAR_STATUS4.h"

/* ========================================================================= */
/* 関数名	:CWindowCHAR_STATUS4::CWindowCHAR_STATUS4						 */
/* 内容		:コンストラクタ													 */
/* 日付		:2009/02/15														 */
/* ========================================================================= */

CWindowCHAR_STATUS4::CWindowCHAR_STATUS4()
{
	m_nID			= WINDOWTYPE_CHAR_STATUS4;
	m_ptViewPos.x	= 0;
	m_ptViewPos.y	= 23;
	m_sizeWindow.cx	= 256;
	m_sizeWindow.cy	= 71;
}


/* ========================================================================= */
/* 関数名	:CWindowCHAR_STATUS4::~CWindowCHAR_STATUS4						 */
/* 内容		:デストラクタ													 */
/* 日付		:2009/02/15														 */
/* ========================================================================= */

CWindowCHAR_STATUS4::~CWindowCHAR_STATUS4()
{
}


/* ========================================================================= */
/* 関数名	:CWindowCHAR_STATUS4::Create									 */
/* 内容		:作成															 */
/* 日付		:2009/02/15														 */
/* ========================================================================= */

void CWindowCHAR_STATUS4::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);
}


/* ========================================================================= */
/* 関数名	:CWindowCHAR_STATUS4::Draw										 */
/* 内容		:描画															 */
/* 日付		:2009/02/15														 */
/* ========================================================================= */

void CWindowCHAR_STATUS4::Draw(PCImg32 pDst)
{
	int nTmp;
	float fTmp;
	HDC hDC;
	HFONT hFontOld;
	PCInfoMapBase pInfoMap;
	PCInfoCharCli pInfoChar;
	CmyString strTmp;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}
	pInfoMap = m_pMgrData->GetMap ();
	if (pInfoMap == NULL) {
		goto Exit;
	}
	pInfoChar = m_pMgrData->GetPlayerChar ();
	if (pInfoChar == NULL) {
		goto Exit;
	}

	m_pDib->BltFrom256 (0, 0, m_sizeWindow.cx, m_sizeWindow.cy, m_pDibSystem, 0, 647, TRUE);

	hDC			= m_pDib->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont12);
	SetBkMode (hDC, TRANSPARENT);

	TextOut2 (hDC, 7, 6, (LPCSTR)pInfoChar->m_strCharName, RGB (255, 255, 255));

	fTmp = (float)pInfoChar->m_dwHP * 100.0f / (float)pInfoChar->m_dwMaxHP;
	nTmp = (int)((float)200 * fTmp / 100.0f);
	m_pDib->BltFrom256 (46, 28, nTmp, 7, m_pDibSystem, 0, 730, TRUE);
	fTmp = (float)pInfoChar->m_dwSP * 100.0f / (float)pInfoChar->m_dwMaxSP;
	nTmp = (int)((float)200 * fTmp / 100.0f);
	m_pDib->BltFrom256 (46, 43, nTmp, 7, m_pDibSystem, 0, 737, TRUE);

	SelectObject (hDC, hFontOld);
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont12Bold);
	strTmp.Format ("%d/%d", pInfoChar->m_dwHP, pInfoChar->m_dwMaxHP);
	TextOut2 (hDC, 240 - (strTmp.GetLength () * 7), 23, (LPCSTR)strTmp, RGB (255, 255, 255), TRUE, RGB (1, 1, 1));
	strTmp.Format ("%d/%d", pInfoChar->m_dwSP, pInfoChar->m_dwMaxSP);
	TextOut2 (hDC, 240 - (strTmp.GetLength () * 7), 38, (LPCSTR)strTmp, RGB (255, 255, 255), TRUE, RGB (1, 1, 1));

	SelectObject (hDC, hFontOld);
	m_pDib->Unlock ();

	m_dwTimeDrawStart = timeGetTime ();
Exit:
	pDst->Blt (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}

/* Copyright(C)URARA-works 2009 */
