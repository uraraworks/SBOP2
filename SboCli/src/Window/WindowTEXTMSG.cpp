/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:WindowTEXTMSG.cpp											 */
/* 内容			:テキストメッセージ表示ウィンドウクラス 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/11/22													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "MgrWindow.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrSound.h"
#include "WindowTEXTMSG.h"

/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::CWindowTEXTMSG									 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/11/22														 */
/* ========================================================================= */

CWindowTEXTMSG::CWindowTEXTMSG()
{
	m_nSpaceHeight	= 16 * 3;

	m_nID			= WINDOWTYPE_TEXTMSG;
	m_ptViewPos.x	= 16;
	m_sizeWindow.cx	= 16 * 2 + 16 * 26;
	m_sizeWindow.cy	= 16 * 2 + 16 * 5 + m_nSpaceHeight;
	m_ptViewPos.y	= SCRSIZEY - 16 - m_sizeWindow.cy;

	m_bInputWait	= FALSE;
	m_nType			= 0;
	m_nProcPos		= 0;
	m_dwLastProc	= 0;
	m_pDibTitle		= NULL;
	m_pDibText		= NULL;
	m_ptDraw.x = m_ptDraw.y = 0;
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::~CWindowTEXTMSG								 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/11/22														 */
/* ========================================================================= */

CWindowTEXTMSG::~CWindowTEXTMSG()
{
	SAFE_DELETE (m_pDibTitle);
	SAFE_DELETE (m_pDibText);
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::Create											 */
/* 内容		:作成															 */
/* 日付		:2008/11/22														 */
/* ========================================================================= */

void CWindowTEXTMSG::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_bActive = TRUE;
	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);

	m_pDibTitle = new CImg32;
	m_pDibTitle->Create (16 * 6, 16 * 2);
	m_pDibText = new CImg32;
	m_pDibText->Create (m_sizeWindow.cx - 16 * 2 + 2, m_sizeWindow.cy - 16 * 2 - m_nSpaceHeight + 2);
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::Draw											 */
/* 内容		:描画															 */
/* 日付		:2005/06/12														 */
/* ========================================================================= */

void CWindowTEXTMSG::Draw(PCImg32 pDst)
{
	int nTmp, x, y, cx, cy;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame (0, m_nSpaceHeight, m_sizeWindow.cx, m_sizeWindow.cy - m_nSpaceHeight, m_nType);

	nTmp = 0;
	if (m_strTitle.IsEmpty () == FALSE) {
		nTmp ++;
	}
	if (m_strName.IsEmpty () == FALSE) {
		nTmp ++;
	}
	if (nTmp > 0) {
		cx = m_pDibTitle->Width ()  + 16 * 2;
		cy = 16 * (2 + nTmp);
		x = m_sizeWindow.cx / 2 - cx / 2;
		y = (2 - nTmp) * 16;
		DrawFrame (x, y, cx, cy, m_nType);
		m_pDib->Blt (x + 16, y + 16, m_pDibTitle->Width (), m_pDibTitle->Height (), m_pDibTitle, 0, 0, TRUE);
	}

	m_pDib->Blt (16, 16 + m_nSpaceHeight, m_pDibText->Width (), m_pDibText->Height (), m_pDibText, 0, 0, TRUE);

	m_dwTimeDrawStart = timeGetTime ();
Exit:
	pDst->Blt (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::TimerProc										 */
/* 内容		:時間処理														 */
/* 日付		:2008/11/22														 */
/* ========================================================================= */

BOOL CWindowTEXTMSG::TimerProc(void)
{
	BOOL bRet, bResult;
	DWORD dwTimeTmp;
	LPCSTR pszTmp;
	char szTmp[3];

	bRet = FALSE;

	if (m_pDibText == NULL) {
		goto Exit;
	}
	dwTimeTmp = timeGetTime () - m_dwLastProc;
	if (dwTimeTmp < 100) {
		goto Exit;
	}
	m_dwLastProc = timeGetTime ();

	ZeroMemory (szTmp, sizeof (szTmp));
	pszTmp = (LPCSTR)m_strMsg;

	szTmp[0] = pszTmp[m_nProcPos];
	if (szTmp[0] == 0) {
		m_bInputWait = TRUE;
		goto Exit;
	}
	if (pszTmp[m_nProcPos] == 0x0D) {
		if (pszTmp[m_nProcPos + 1] == 0x0A) {
			szTmp[1] = pszTmp[m_nProcPos + 1];
		}
	}

	bResult = IsDBCSLeadByte ((BYTE)pszTmp[m_nProcPos]);
	if (bResult) {
		szTmp[1] = pszTmp[m_nProcPos + 1];
		m_nProcPos ++;
	}

	DrawChar (szTmp);

	m_nProcPos ++;
	Redraw ();

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::SetTitle										 */
/* 内容		:肩書きを設定													 */
/* 日付		:2008/11/24														 */
/* ========================================================================= */

void CWindowTEXTMSG::SetTitle(LPCSTR pszTitle)
{
	m_strTitle = pszTitle;
	RenewTitle ();
	Redraw ();
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::SetName										 */
/* 内容		:名前を設定														 */
/* 日付		:2008/11/24														 */
/* ========================================================================= */

void CWindowTEXTMSG::SetName(LPCSTR pszName)
{
	m_strName = pszName;
	RenewTitle ();
	Redraw ();
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::SetMsg											 */
/* 内容		:メッセージ設定													 */
/* 日付		:2008/11/22														 */
/* ========================================================================= */

void CWindowTEXTMSG::SetMsg(LPCSTR pszMsg)
{
	m_ptDraw.x = m_ptDraw.y = 0;
	m_nProcPos		= 0;
	m_dwLastProc	= timeGetTime ();
	m_strMsg		= pszMsg;

	m_pDibText->FillRect (0, 0, m_pDibText->Width (), m_pDibText->Height (), RGB (0, 0, 0));
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::OnX											 */
/* 内容		:キーハンドラ(X)												 */
/* 日付		:2008/11/22														 */
/* ========================================================================= */

BOOL CWindowTEXTMSG::OnX(BOOL bDown)
{
	if (bDown == FALSE) {
		if (m_bInputWait) {
			m_pMgrSound->PlaySound (SOUNDID_OK_PI73);
			m_bDelete = TRUE;
		}
	}

	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::OnZ											 */
/* 内容		:キーハンドラ(Z)												 */
/* 日付		:2008/11/22														 */
/* ========================================================================= */

BOOL CWindowTEXTMSG::OnZ(BOOL bDown)
{
	if (bDown == FALSE) {
		if (m_bInputWait) {
			m_pMgrSound->PlaySound (SOUNDID_OK_PI73);
			m_bDelete = TRUE;
		}
	}

	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::DrawChar										 */
/* 内容		:１文字表示														 */
/* 日付		:2008/11/24														 */
/* ========================================================================= */

void CWindowTEXTMSG::DrawChar(LPCSTR pszText)
{
	int cx, cy;
	HDC hDC;
	HFONT hFontOld;
	COLORREF clText;

	if (pszText == NULL) {
		return;
	}

	cx = m_pDibText->Width ();
	cy = m_pDibText->Height () - 16;

	clText		= RGB (1, 1, 1);
	hDC			= m_pDibText->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont16Normal);
	SetBkMode (hDC, TRANSPARENT);

	clText = RGB (1, 1, 1);
	TextOut2 (hDC, m_ptDraw.x, m_ptDraw.y, pszText, clText);

	if (pszText[0] == 0x0D) {
		if (pszText[1] == 0x0A) {
			m_ptDraw.x = cx;
			m_nProcPos ++;
		}
	}

	m_ptDraw.x += (strlen (pszText) * 8);
	if (m_ptDraw.x + 8 >= cx) {
		m_ptDraw.x = 0;
		m_ptDraw.y += 16;
		if (m_ptDraw.y >= cy) {
			m_ptDraw.y -= 16;
			m_pDibText->Blt (0, 0, cx, cy, m_pDibText, 0, 16);
			m_pDibText->FillRect (0, cy, cx, 16, RGB (0, 0, 0));
		}
	}

	SelectObject (hDC, hFontOld);
	m_pDibText->Unlock ();
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::RenewTitle										 */
/* 内容		:肩書と名前画像を更新											 */
/* 日付		:2008/11/24														 */
/* ========================================================================= */

void CWindowTEXTMSG::RenewTitle(void)
{
	int y;
	HDC hDC;
	COLORREF clText, clFrame;
	HFONT hFontOld;

	m_pDibTitle->FillRect (0, 0, m_pDibTitle->Width (), m_pDibTitle->Height (), RGB (0, 0, 0));

	hDC			= m_pDibTitle->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont16);
	SetBkMode (hDC, TRANSPARENT);

	y = 1;
	clText  = RGB (255, 255, 255);
	clFrame = RGB (1, 1, 1);
	if (m_strTitle.GetLength () > 0) {
		TextOut2 (hDC, 1, 1, (LPCSTR)m_strTitle, clText, TRUE, clFrame);
		y += 16;
	}
	if (m_strName.GetLength () > 0) {
		TextOut2 (hDC, 1, y, (LPCSTR)m_strName, clText, TRUE, clFrame);
	}

	SelectObject (hDC, hFontOld);
	m_pDibTitle->Unlock ();
}

/* Copyright(C)URARA-works 2008 */
