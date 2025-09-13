/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:WindowACCOUNTINFO.cpp										 */
/* 内容			:アカウント情報ウィンドウクラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/12/29													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoAccount.h"
#include "InfoCharCli.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "WindowACCOUNTINFO.h"


/* ========================================================================= */
/* 関数名：	CWindowACCOUNTINFO::CWindowACCOUNTINFO							 */
/* 内容：	コンストラクタ													 */
/* 日付：	2006/12/29														 */
/* ========================================================================= */

CWindowACCOUNTINFO::CWindowACCOUNTINFO()
{
	m_nPos			= 0;
	m_nPosMax		= 2;
	m_nID			= WINDOWTYPE_ACCOUNTINFO;
	m_ptViewPos.x	= 280;
	m_ptViewPos.y	= 48;
	m_sizeWindow.cx	= 16 * 2 + 16 * 10;
	m_sizeWindow.cy	= 16 * 2 + 16 * 4;
}


/* ========================================================================= */
/* 関数名：	CWindowACCOUNTINFO::~CWindowACCOUNTINFO							 */
/* 内容：	デストラクタ													 */
/* 日付：	2006/12/29														 */
/* ========================================================================= */

CWindowACCOUNTINFO::~CWindowACCOUNTINFO()
{
}


/* ========================================================================= */
/* 関数名	:CWindowACCOUNTINFO::Create										 */
/* 内容		:作成															 */
/* 日付		:2006/12/29														 */
/* ========================================================================= */

void CWindowACCOUNTINFO::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (RGB (40, 40, 40));
}


/* ========================================================================= */
/* 関数名：	CWindowACCOUNTINFO::Draw										 */
/* 内容：	描画															 */
/* 日付：	2006/12/29														 */
/* ========================================================================= */

void CWindowACCOUNTINFO::Draw(PCImg32 pDst)
{
	HDC hDC;
	HFONT hFontOld;
	COLORREF clText;
	CmyString strTmp;
	PCInfoAccount pInfoAccount;
	PCInfoCharCli pInfoChar;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame ();

	pInfoAccount	= m_pMgrData->GetAccount ();
	pInfoChar		= m_pMgrData->GetPlayerChar ();
	if (pInfoChar == NULL) {
		goto Exit;
	}

	clText		= RGB (0, 0, 0);
	hDC			= m_pDib->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont);
	SetBkMode (hDC, TRANSPARENT);

	strTmp.Format ("キャラ名:%s", (LPCSTR)pInfoChar->m_strCharName);
	TextOut2 (hDC, 16, 16 + 16 * 0, strTmp,	clText);
	strTmp.Format ("アカウント作成日時:");
	TextOut2 (hDC, 16, 16 + 16 * 1, strTmp,	clText);
	TextOut2 (hDC, 16, 16 + 16 * 2, "前回ログイン日時:",	clText);
	TextOut2 (hDC, 16, 16 + 16 * 3, "ログイン回数:",		clText);

	SelectObject (hDC, hFontOld);
	m_pDib->Unlock ();

	m_dwTimeDrawStart = timeGetTime ();

Exit:
	pDst->Blt (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}

/* Copyright(C)URARA-works 2006 */
