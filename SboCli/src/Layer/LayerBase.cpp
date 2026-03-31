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

        m_hFont = CreateFont(12, 0, 0, 0, FW_NORMAL,
                        FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
                        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("ＭＳ ゴシック"));
}


CLayerBase::~CLayerBase()
{
	if (m_hFont) {
		DeleteObject(m_hFont);
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


void CLayerBase::TextOut1(HDC hDC, int x, int y, LPCTSTR pStr, COLORREF color)
{
        if (pStr == NULL) {
                return;
        }

        int nLen = lstrlen(pStr);
        if (nLen <= 0) {
                return;
        }
        SetTextColor(hDC, color);
        ::TextOut(hDC, x, y, pStr, nLen);
}


void CLayerBase::TextOut2(HDC hDC, int x, int y, LPCTSTR pStr, COLORREF color, COLORREF colorFrame)
{
        if (pStr == NULL) {
                return;
        }

        int nLen = lstrlen(pStr);
        if (nLen <= 0) {
                return;
        }
        SetTextColor(hDC, colorFrame);
        ::TextOut(hDC, x - 1, y, pStr, nLen);
        ::TextOut(hDC, x + 1, y, pStr, nLen);
        ::TextOut(hDC, x, y - 1, pStr, nLen);
        ::TextOut(hDC, x, y + 1, pStr, nLen);
        SetTextColor(hDC, color);
        ::TextOut(hDC, x, y, pStr, nLen);
}


void CLayerBase::TextOut3(HDC hDC, int x, int y, LPCTSTR pStr, COLORREF color, COLORREF colorFrame)
{
        if (pStr == NULL) {
                return;
        }

        int nLen = lstrlen(pStr);
        if (nLen <= 0) {
                return;
        }
        SetTextColor(hDC, colorFrame);
        ::TextOut(hDC, x - 2, y, pStr, nLen);
        ::TextOut(hDC, x - 1, y, pStr, nLen);
        ::TextOut(hDC, x - 1, y - 2, pStr, nLen);
        ::TextOut(hDC, x - 2, y - 1, pStr, nLen);
        ::TextOut(hDC, x - 1, y - 1, pStr, nLen);
        ::TextOut(hDC, x - 2, y + 1, pStr, nLen);
        ::TextOut(hDC, x - 1, y + 1, pStr, nLen);
        ::TextOut(hDC, x - 1, y + 2, pStr, nLen);

        ::TextOut(hDC, x + 2, y, pStr, nLen);
        ::TextOut(hDC, x + 1, y, pStr, nLen);
        ::TextOut(hDC, x + 1, y - 2, pStr, nLen);
        ::TextOut(hDC, x + 2, y - 1, pStr, nLen);
        ::TextOut(hDC, x + 1, y - 1, pStr, nLen);
        ::TextOut(hDC, x + 2, y + 1, pStr, nLen);
        ::TextOut(hDC, x + 1, y + 1, pStr, nLen);
        ::TextOut(hDC, x + 1, y + 2, pStr, nLen);

        ::TextOut(hDC, x, y - 2, pStr, nLen);
        ::TextOut(hDC, x, y - 1, pStr, nLen);
        ::TextOut(hDC, x, y + 2, pStr, nLen);
        ::TextOut(hDC, x, y + 1, pStr, nLen);

        SetTextColor(hDC, color);
        ::TextOut(hDC, x, y, pStr, nLen);
}
