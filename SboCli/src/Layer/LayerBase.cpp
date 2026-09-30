/// @file LayerBase.cpp
/// @brief レイヤー描画基底クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/09/24
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
#include "Platform/SdlFont.h"
#include "LibInfoMapBase.h"
#include "LibInfoCharCli.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "LayerBase.h"


CLayerBase::CLayerBase()
{
	m_nID = 0;
	m_pMgrData = NULL;
	m_pMgrGrpData = NULL;
	m_pMgrDraw = NULL;
	m_pLibInfoChar = NULL;
	m_pLibInfoMap = NULL;
	m_pDibSystem = NULL;

	m_pDib = new CImg32;
	m_pDibBase = new CImg32;

	m_hFont = (HFONT)SdlFontCreate(12, false);
}


CLayerBase::~CLayerBase()
{
	if (m_hFont) {
		SdlFontDestroy((void*)m_hFont);
		m_hFont = NULL;
	}
	SAFE_DELETE(m_pDib);
	SAFE_DELETE(m_pDibBase);
}


void CLayerBase::Create(
	CMgrData *pMgrData) // [in] データ管理
{
	m_pMgrData = pMgrData;
	m_pMgrGrpData = m_pMgrData->GetMgrGrpData();
	m_pMgrDraw = m_pMgrData->GetMgrDraw();
	m_pDibSystem = m_pMgrGrpData->GetDibSystem();

	m_pLibInfoChar = m_pMgrData->GetLibInfoChar();
	m_pLibInfoMap = m_pMgrData->GetLibInfoMap();

	m_pDibBase->Create(16 * DRAW_PARTS_X, 16 * DRAW_PARTS_Y);
	m_pDibBase->SetColorKey(RGB(255, 0, 255));
}


void CLayerBase::Destroy(void)
{
}


void CLayerBase::Draw(PCImg32 pDst)
{
}


BOOL CLayerBase::TimerProc(void)
{
	return FALSE;
}


void CLayerBase::TextOut1(HDC hDC, HFONT hFont, int x, int y, LPCTSTR pStr, COLORREF color)
{
	if ((hDC == NULL) || (pStr == NULL) || (hFont == NULL)) return;
	int nLen = lstrlen(pStr);
	if (nLen <= 0) return;

	SdlDCContext* ctx = SdlDCGet(hDC);
	if (ctx == NULL) return;
	ctx->currentFont = (void*)hFont;
	ctx->textColor = (unsigned long)color;
	SdlFontTextOut(hDC, x, y, pStr, nLen);
}


void CLayerBase::TextOut2(HDC hDC, HFONT hFont, int x, int y, LPCTSTR pStr, COLORREF color, COLORREF colorFrame)
{
	if ((hDC == NULL) || (pStr == NULL) || (hFont == NULL)) return;
	int nLen = lstrlen(pStr);
	if (nLen <= 0) return;

	SdlDCContext* ctx = SdlDCGet(hDC);
	if (ctx == NULL) return;
	ctx->currentFont = (void*)hFont;

	// 縁取り 4 方向
	SdlFontTextOutFramed(hDC, x, y, pStr, nLen, (unsigned long)color, (unsigned long)colorFrame, false);
}


void CLayerBase::TextOut3(HDC hDC, HFONT hFont, int x, int y, LPCTSTR pStr, COLORREF color, COLORREF colorFrame)
{
	if ((hDC == NULL) || (pStr == NULL) || (hFont == NULL)) return;
	int nLen = lstrlen(pStr);
	if (nLen <= 0) return;

	SdlDCContext* ctx = SdlDCGet(hDC);
	if (ctx == NULL) return;
	ctx->currentFont = (void*)hFont;

	// 縁取り 8 方向（太め）
	SdlFontTextOutFramed(hDC, x, y, pStr, nLen, (unsigned long)color, (unsigned long)colorFrame, true);
}


// キーの絵の高さと、操作案内 1 行の高さ
#define KEYCAP_H	(20)

int CLayerBase::DrawKeyCap(CImg32 *pDst, int x, int y, LPCTSTR pszKey)
{
	int nTextW, nTextH, cx;
	HDC hDC;

	nTextW = nTextH = 0;
	SdlFontGetTextExtent((void *)m_hFont, pszKey, lstrlen(pszKey), &nTextW, &nTextH);
	cx = max(nTextW + 8, KEYCAP_H);

	// 1 ドットの黒い縁（角は落とす）、上に明るい線、下に影
	pDst->FillRect(x + 2, y, cx - 4, KEYCAP_H, RGB(20, 20, 20));
	pDst->FillRect(x, y + 2, cx, KEYCAP_H - 4, RGB(20, 20, 20));
	pDst->FillRect(x + 2, y + 2, cx - 4, KEYCAP_H - 4, RGB(70, 70, 70));
	pDst->FillRect(x + 4, y + 2, cx - 8, 2, RGB(101, 101, 101));
	pDst->FillRect(x + 2, y + KEYCAP_H - 4, cx - 4, 2, RGB(45, 45, 45));

	hDC = pDst->Lock();
	TextOut1(hDC, m_hFont, x + (((cx - nTextW) / 2) & ~1), y + 2, pszKey, RGB(255, 255, 255));
	pDst->Unlock();

	return cx;
}


// pszKeys はキーの名前を半角スペースで区切って並べる（例: "← ↑ ↓ →"）
void CLayerBase::DrawKeyHelp(CImg32 *pDst, int x, int y, LPCTSTR pszKeys, LPCTSTR pszText)
{
	CString strKeys, strKey;
	int nStart, nPos;
	HDC hDC;

	x &= ~1;
	y &= ~1;
	strKeys = pszKeys;
	nStart = 0;
	while (nStart < strKeys.GetLength()) {
		nPos = strKeys.Find(_T(' '), nStart);
		if (nPos < 0) {
			nPos = strKeys.GetLength();
		}
		strKey = strKeys.Mid(nStart, nPos - nStart);
		if (strKey.IsEmpty() == FALSE) {
			x += DrawKeyCap(pDst, x, y, strKey) + 2;
		}
		nStart = nPos + 1;
	}

	hDC = pDst->Lock();
	TextOut2(hDC, m_hFont, x + 2, y + 2, pszText, RGB(1, 1, 1), RGB(255, 255, 255));
	pDst->Unlock();
}
