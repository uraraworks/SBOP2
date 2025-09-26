/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:LayerBase.h												 */
/* 内容			:レイヤー描画基底クラス 実装ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/09/24													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoMapBase.h"
#include "LibInfoCharCli.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "LayerBase.h"


/* ========================================================================= */
/* 関数名	:CLayerBase::CLayerBase											 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

CLayerBase::CLayerBase()
{
	m_nID			= 0;
	m_pMgrData		= NULL;
	m_pMgrGrpData	= NULL;
	m_pMgrDraw		= NULL;
	m_pLibInfoChar	= NULL;
	m_pLibInfoMap	= NULL;
	m_pDibSystem	= NULL;

	m_pDib			= new CImg32;
	m_pDibBase		= new CImg32;

        m_hFont = CreateFont (12, 0, 0, 0, FW_NORMAL,
                        FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
                        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("ＭＳ ゴシック"));
}


/* ========================================================================= */
/* 関数名	:CLayerBase::~CLayerBase										 */
/* 内容		:デストラクタ													 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

CLayerBase::~CLayerBase()
{
	if (m_hFont) {
		DeleteObject (m_hFont);
		m_hFont = NULL;
	}
	SAFE_DELETE (m_pDib);
	SAFE_DELETE (m_pDibBase);
}


/* ========================================================================= */
/* 関数名	:CLayerBase::Create												 */
/* 内容		:作成															 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

void CLayerBase::Create(
	CMgrData	*pMgrData)		/* [in] データ管理 */
{
	m_pMgrData		= pMgrData;
	m_pMgrGrpData	= m_pMgrData->GetMgrGrpData ();
	m_pMgrDraw		= m_pMgrData->GetMgrDraw ();
	m_pDibSystem	= m_pMgrGrpData->GetDibSystem ();

	m_pLibInfoChar	= m_pMgrData->GetLibInfoChar ();
	m_pLibInfoMap	= m_pMgrData->GetLibInfoMap ();

	m_pDibBase->Create (16 * DRAW_PARTS_X, 16 * DRAW_PARTS_Y);
	m_pDibBase->SetColorKey (RGB (255, 0, 255));
}


/* ========================================================================= */
/* 関数名	:CLayerBase::Destroy											 */
/* 内容		:破棄															 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

void CLayerBase::Destroy(void)
{
}


/* ========================================================================= */
/* 関数名	:CLayerBase::Draw												 */
/* 内容		:描画															 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

void CLayerBase::Draw(PCImg32 pDst)
{
}


/* ========================================================================= */
/* 関数名	:CLayerBase::TimerProc											 */
/* 内容		:時間処理														 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

BOOL CLayerBase::TimerProc(void)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:TextOut1														 */
/* 内容		:文字描画														 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

void CLayerBase::TextOut1(HDC hDC, int x, int y, LPCSTR pStr, COLORREF color)
{
        if (pStr == NULL) {
                return;
        }

        CString strText = AnsiToTString (pStr);
        int nLen = strText.GetLength ();
        if (nLen <= 0) {
                return;
        }
        SetTextColor (hDC, color);
        ::TextOut (hDC, x, y, strText, nLen);
}


/* ========================================================================= */
/* 関数名	:TextOut2														 */
/* 内容		:縁取り文字描画													 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

void CLayerBase::TextOut2(HDC hDC, int x, int y, LPCSTR pStr, COLORREF color, COLORREF colorFrame)
{
        if (pStr == NULL) {
                return;
        }

        CString strText = AnsiToTString (pStr);
        int nLen = strText.GetLength ();
        if (nLen <= 0) {
                return;
        }
        SetTextColor (hDC, colorFrame);
        ::TextOut (hDC, x - 1, y, strText, nLen);
        ::TextOut (hDC, x + 1, y, strText, nLen);
        ::TextOut (hDC, x, y - 1, strText, nLen);
        ::TextOut (hDC, x, y + 1, strText, nLen);
        SetTextColor (hDC, color);
        ::TextOut (hDC, x, y, strText, nLen);
}


/* ========================================================================= */
/* 関数名	:TextOut3														 */
/* 内容		:縁取り文字描画													 */
/* 日付		:2008/11/22														 */
/* ========================================================================= */

void CLayerBase::TextOut3(HDC hDC, int x, int y, LPCSTR pStr, COLORREF color, COLORREF colorFrame)
{
        if (pStr == NULL) {
                return;
        }

        CString strText = AnsiToTString (pStr);
        int nLen = strText.GetLength ();
        if (nLen <= 0) {
                return;
        }
        SetTextColor (hDC, colorFrame);
        ::TextOut (hDC, x - 2, y, strText, nLen);
        ::TextOut (hDC, x - 1, y, strText, nLen);
        ::TextOut (hDC, x - 1, y - 2, strText, nLen);
        ::TextOut (hDC, x - 2, y - 1, strText, nLen);
        ::TextOut (hDC, x - 1, y - 1, strText, nLen);
        ::TextOut (hDC, x - 2, y + 1, strText, nLen);
        ::TextOut (hDC, x - 1, y + 1, strText, nLen);
        ::TextOut (hDC, x - 1, y + 2, strText, nLen);

        ::TextOut (hDC, x + 2, y, strText, nLen);
        ::TextOut (hDC, x + 1, y, strText, nLen);
        ::TextOut (hDC, x + 1, y - 2, strText, nLen);
        ::TextOut (hDC, x + 2, y - 1, strText, nLen);
        ::TextOut (hDC, x + 1, y - 1, strText, nLen);
        ::TextOut (hDC, x + 2, y + 1, strText, nLen);
        ::TextOut (hDC, x + 1, y + 1, strText, nLen);
        ::TextOut (hDC, x + 1, y + 2, strText, nLen);

        ::TextOut (hDC, x, y - 2, strText, nLen);
        ::TextOut (hDC, x, y - 1, strText, nLen);
        ::TextOut (hDC, x, y + 2, strText, nLen);
        ::TextOut (hDC, x, y + 1, strText, nLen);

        SetTextColor (hDC, color);
        ::TextOut (hDC, x, y, strText, nLen);
}

/* Copyright(C)URARA-works 2005 */
