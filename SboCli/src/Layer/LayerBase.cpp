/// @file LayerBase.cpp
/// @brief レイヤー描画基底クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/09/24
/// @copyright Copyright(C)URARA-works 2006

#include "stdafx.h"
#include "LibInfoMapBase.h"
#include "LibInfoCharCli.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "LayerBase.h"
#include "TextRenderer.h"


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
}


CLayerBase::~CLayerBase()
{
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


void CLayerBase::TextOut1(CImg32 *pDst, int nFontId, int x, int y, LPCTSTR pStr, COLORREF color)
{
	if (pStr == NULL || pDst == NULL) {
		return;
	}
	CTextRenderer::Instance().DrawText(pDst, (FontId)nFontId, x, y, pStr, color);
}


void CLayerBase::TextOut2(CImg32 *pDst, int nFontId, int x, int y, LPCTSTR pStr, COLORREF color, COLORREF colorFrame)
{
	if (pStr == NULL || pDst == NULL) {
		return;
	}
	CTextRenderer::Instance().DrawTextOutlined(pDst, (FontId)nFontId, x, y, pStr, color, colorFrame);
}


void CLayerBase::TextOut3(CImg32 *pDst, int nFontId, int x, int y, LPCTSTR pStr, COLORREF color, COLORREF colorFrame)
{
	if (pStr == NULL || pDst == NULL) {
		return;
	}
	CTextRenderer::Instance().DrawTextOutlinedThick(pDst, (FontId)nFontId, x, y, pStr, colorFrame, color);
}
