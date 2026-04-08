/// @file LayerBase.cpp
/// @brief レイヤー描画基底クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/09/24
/// @copyright Copyright(C)URARA-works 2006

#include "stdafx.h"
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
	ctx->textColor = (unsigned long)colorFrame;
	SdlFontTextOut(hDC, x - 1, y, pStr, nLen);
	SdlFontTextOut(hDC, x + 1, y, pStr, nLen);
	SdlFontTextOut(hDC, x, y - 1, pStr, nLen);
	SdlFontTextOut(hDC, x, y + 1, pStr, nLen);
	// 本体
	ctx->textColor = (unsigned long)color;
	SdlFontTextOut(hDC, x, y, pStr, nLen);
}


void CLayerBase::TextOut3(HDC hDC, HFONT hFont, int x, int y, LPCTSTR pStr, COLORREF color, COLORREF colorFrame)
{
	if ((hDC == NULL) || (pStr == NULL) || (hFont == NULL)) return;
	int nLen = lstrlen(pStr);
	if (nLen <= 0) return;

	SdlDCContext* ctx = SdlDCGet(hDC);
	if (ctx == NULL) return;
	ctx->currentFont = (void*)hFont;

	// 縁取り 20 方向
	ctx->textColor = (unsigned long)colorFrame;
	SdlFontTextOut(hDC, x - 2, y, pStr, nLen);
	SdlFontTextOut(hDC, x - 1, y, pStr, nLen);
	SdlFontTextOut(hDC, x - 1, y - 2, pStr, nLen);
	SdlFontTextOut(hDC, x - 2, y - 1, pStr, nLen);
	SdlFontTextOut(hDC, x - 1, y - 1, pStr, nLen);
	SdlFontTextOut(hDC, x - 2, y + 1, pStr, nLen);
	SdlFontTextOut(hDC, x - 1, y + 1, pStr, nLen);
	SdlFontTextOut(hDC, x - 1, y + 2, pStr, nLen);

	SdlFontTextOut(hDC, x + 2, y, pStr, nLen);
	SdlFontTextOut(hDC, x + 1, y, pStr, nLen);
	SdlFontTextOut(hDC, x + 1, y - 2, pStr, nLen);
	SdlFontTextOut(hDC, x + 2, y - 1, pStr, nLen);
	SdlFontTextOut(hDC, x + 1, y - 1, pStr, nLen);
	SdlFontTextOut(hDC, x + 2, y + 1, pStr, nLen);
	SdlFontTextOut(hDC, x + 1, y + 1, pStr, nLen);
	SdlFontTextOut(hDC, x + 1, y + 2, pStr, nLen);

	SdlFontTextOut(hDC, x, y - 2, pStr, nLen);
	SdlFontTextOut(hDC, x, y - 1, pStr, nLen);
	SdlFontTextOut(hDC, x, y + 2, pStr, nLen);
	SdlFontTextOut(hDC, x, y + 1, pStr, nLen);

	// 本体
	ctx->textColor = (unsigned long)color;
	SdlFontTextOut(hDC, x, y, pStr, nLen);
}
