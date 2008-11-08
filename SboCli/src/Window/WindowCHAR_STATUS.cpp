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
	m_sizeWindow.cx	= 16 * 2 + 12 * 10;
	m_sizeWindow.cy	= 16 * 2 + 12 * 3;
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
	int nLevel, y;
	HDC hDC;
	HFONT hFontOld;
	COLORREF clText;
	PCInfoCharCli pInfoChar;
	CmyString strTmp;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	pInfoChar = m_pMgrData->GetPlayerChar ();
	DrawFrame ();

	clText		= RGB (1, 1, 1);
	hDC			= m_pDib->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont12);
	SetBkMode (hDC, TRANSPARENT);

	clText = RGB (1, 1, 1);

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

/* Copyright(C)URARA-works 2008 */
