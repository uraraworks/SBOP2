/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* ファイル名	:LayerCharSelect.cpp										 */
/* 内容			:レイヤー描画クラス(キャラ選択) 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2005/07/04													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoAccount.h"
#include "LibInfoCharCli.h"
#include "InfoCharCli.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrDraw.h"
#include "Img32.h"
#include "LayerCharSelect.h"


/* ========================================================================= */
/* 関数名	:CLayerCharSelect::CLayerCharSelect								 */
/* 内容		:コンストラクタ													 */
/* 日付		:2005/07/04														 */
/* ========================================================================= */

CLayerCharSelect::CLayerCharSelect()
{
	m_nID = LAYERTYPE_CHARSELECT;

	m_nSelect			= -1;
	m_byAnime			= 0;
	m_dwTimeLastAnime	= 0;
	m_pInfoAccount		= NULL;
}


/* ========================================================================= */
/* 関数名	:CLayerCharSelect::~CLayerCharSelect							 */
/* 内容		:デストラクタ													 */
/* 日付		:2005/07/04														 */
/* ========================================================================= */

CLayerCharSelect::~CLayerCharSelect()
{
}


/* ========================================================================= */
/* 関数名	:CLayerCharSelect::Create										 */
/* 内容		:作成															 */
/* 日付		:2005/07/04														 */
/* ========================================================================= */

void CLayerCharSelect::Create(
	CMgrData	*pMgrData,		/* [in] データ管理 */
	DWORD		dwAccountID)	/* [in] アカウントID */
{
	CLayerBase::Create (pMgrData);

	m_pInfoAccount = m_pMgrData->GetAccount ();
}


/* ========================================================================= */
/* 関数名	:CLayerCharSelect::Draw											 */
/* 内容		:描画															 */
/* 日付		:2005/07/04														 */
/* ========================================================================= */

void CLayerCharSelect::Draw(PCImg32 pDst)
{
	int i, nCount, x, y, nTmp;
	BYTE byAnimeNo, byLevel;
	HDC hDC;
	HFONT hFontOld;
	PCInfoCharCli pChar;
	POINT ptViewCharPos;

	nCount	= m_pInfoAccount->m_adwCharID.GetSize ();
	nTmp	= SCRSIZEX / (3 + 1);

	for (i = 0; i < 3; i ++) {
		x			= 32 + (nTmp * (i + 1));
		y			= SCRSIZEY / 2 + 32;
		byAnimeNo	= m_byAnime;
		byLevel		= 100;

		DrawFrame (pDst, x - 16, y - 24, 64, 72, 3);
		if (i >= nCount) {
			pDst->BltFrom256 (x, y, 32, 32, m_pDibSystem, 112, 96, TRUE);
			continue;
		}

		pChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (m_pInfoAccount->m_adwCharID[i]);
		if (pChar == NULL) {
			continue;
		}
		pChar->GetViewCharPos (ptViewCharPos);
		if (i != m_nSelect) {
			byAnimeNo	= 1;
			byLevel		= 50;
		}
		if (ptViewCharPos.y != 0) {
			y += 16;
		}
		/* 必ず下向き */
		pChar->m_nDirection = 1;

		/* キャラの描画 */
		m_pMgrDraw->DrawChar (
				pDst,
				x - ptViewCharPos.x, y - ptViewCharPos.y + 16,
				pChar);
		if (ptViewCharPos.y != 0) {
			y -= 16;
		}

		/* 名前の描画 */
		hDC			= pDst->Lock ();
		hFontOld	= (HFONT)SelectObject (hDC, m_hFont);
		SetBkMode (hDC, TRANSPARENT);
		TextOut2 (
				hDC,
				x + 16 - (pChar->m_strCharName.GetLength () * 6 / 2),
				y + 56,
				pChar->m_strCharName,
				pChar->m_clName);
		SelectObject (hDC, hFontOld);
		pDst->Unlock ();
	}
	if (m_nSelect >= 0) {
		x = 32 + (nTmp * (m_nSelect + 1));
		y = SCRSIZEY / 2 + 32;
		pDst->BltFrom256 (x - 24, y - 21, 24, 21, m_pDibSystem, 72, 0, TRUE);
	}
}


/* ========================================================================= */
/* 関数名	:CLayerCharSelect::TimerProc									 */
/* 内容		:時間処理														 */
/* 日付		:2005/07/09														 */
/* ========================================================================= */

BOOL CLayerCharSelect::TimerProc(void)
{
	BOOL bRet;
	DWORD dwTime;

	bRet = FALSE;

	dwTime = timeGetTime ();
	if (dwTime - m_dwTimeLastAnime > 300) {
		m_byAnime ++;
		if (m_byAnime >= 4) {
			m_byAnime = 0;
		}
		m_dwTimeLastAnime = dwTime;

	} else {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLayerCharSelect::SetSelect									 */
/* 内容		:選択位置を設定													 */
/* 日付		:2005/07/09														 */
/* ========================================================================= */

void CLayerCharSelect::SetSelect(int nNo)
{
	int i, nCount, nState;
	PCInfoCharCli pChar;

	m_nSelect			= nNo;
	m_byAnime			= 0;
	m_dwTimeLastAnime	= timeGetTime ();

	nCount = m_pInfoAccount->m_adwCharID.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (m_pInfoAccount->m_adwCharID[i]);
		if (pChar == NULL) {
			continue;
		}
		nState = CHARMOVESTATE_STAND;
		if (i == m_nSelect) {
			nState = CHARMOVESTATE_WALKANIME;
		}
		pChar->ChgMoveState (nState);
		pChar->m_nDirection = 1;
	}
}


/* ========================================================================= */
/* 関数名	:CLayerCharSelect::GetSelectCharID								 */
/* 内容		:選択中のキャラIDを取得											 */
/* 日付		:2007/03/21														 */
/* ========================================================================= */

DWORD CLayerCharSelect::GetSelectCharID(void)
{
	int nCount;
	DWORD dwRet;
	PCInfoCharCli pChar;

	dwRet	= 0;
	nCount	= m_pInfoAccount->m_adwCharID.GetSize ();

	if ((nCount <= 0) || (m_nSelect >= nCount)) {
		goto Exit;
	}

	pChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (m_pInfoAccount->m_adwCharID[m_nSelect]);
	if (pChar == NULL) {
		goto Exit;
	}

	dwRet = pChar->m_dwCharID;
Exit:
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLayerCharSelect::DrawFrame									 */
/* 内容		:フレームを描画													 */
/* 日付		:2007/04/01														 */
/* ========================================================================= */

void CLayerCharSelect::DrawFrame(CImg32 *pDst, int x, int y, int cx, int cy, int nType)
{
	int i, nTmp, xx, yy, aTypeX[] = {0, 0, 48, 96, 0}, aTypeY[] = {0, 48, 48, 48, 96};
	COLORREF clTmp, clType[] = {RGB (255, 235, 200), RGB (255, 235, 200), RGB (255, 255, 255), RGB (255, 235, 200), RGB (255, 255, 255)};

	xx		= aTypeX[nType];
	yy		= aTypeY[nType];
	clTmp	= clType[nType];
	nTmp	= (nType == 4) ? 3 : 0;

	/* 背景塗りつぶし */
	pDst->FillRect (x + 16, y + 16, cx - 32, cy - 32, clTmp);

	/* 四隅 */
	pDst->BltFrom256 (x,			y,				16, 16 + nTmp,	m_pDibSystem, xx, yy, TRUE);
	pDst->BltFrom256 (x + cx - 16,	y,				16, 16 + nTmp,	m_pDibSystem, xx + 32, yy, TRUE);
	pDst->BltFrom256 (x,			y + cy - 16,	16, 16,			m_pDibSystem, xx, yy + 32, TRUE);
	pDst->BltFrom256 (x + cx - 16,	y + cy - 16,	16, 16,			m_pDibSystem, xx + 32, yy + 32, TRUE);

	/* 横線 */
	for (i = 1; i < cx / 8 - 2; i ++) {
		pDst->BltFrom256 (x + (i + 1) * 8, y,			8, 16, m_pDibSystem, xx + 16, yy, TRUE);
		pDst->BltFrom256 (x + (i + 1) * 8, y + cy - 16,	8, 16, m_pDibSystem, xx + 16, yy + 32, TRUE);
	}

	/* 縦線 */
	for (i = 1; i < cy / 8 - 2; i ++) {
		if ((i == 1) && (nTmp != 0)) {
			pDst->BltFrom256 (x,			y + (i + 1) * 8 + nTmp, 16, 8 - nTmp, m_pDibSystem, xx,		yy + 16 + nTmp, TRUE);
			pDst->BltFrom256 (x + cx - 16,	y + (i + 1) * 8 + nTmp, 16, 8 - nTmp, m_pDibSystem, xx + 32,	yy + 16 + nTmp, TRUE);
		} else {
			pDst->BltFrom256 (x,			y + (i + 1) * 8, 16, 8, m_pDibSystem, xx,		yy + 16 + nTmp, TRUE);
			pDst->BltFrom256 (x + cx - 16,	y + (i + 1) * 8, 16, 8, m_pDibSystem, xx + 32,	yy + 16 + nTmp, TRUE);
		}
	}
}

/* Copyright(C)URARA-works 2005 */
