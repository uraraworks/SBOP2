/// @file LayerTitle.cpp
/// @brief レイヤー描画クラス(タイトル) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/03/26
/// @copyright Copyright(C)URARA-works 2005

#include "stdafx.h"
#define SDL_MAIN_HANDLED
#include <SDL.h>
#if defined(__EMSCRIPTEN__)
#include <emscripten/em_js.h>
#endif
#include "MgrData.h"
#include "MgrGrpData.h"
#include "Img32.h"
#include "LayerTitle.h"

#if defined(__EMSCRIPTEN__)
// LayerTitle 専用テキストキュー: WindowBase の SBOP2_QueueCanvasText と同じ仕組みで push する
// 重複定義を避けるため関数名を SBOP2_QueueLayerText として定義
EM_JS(void, SBOP2_QueueLayerText, (int x, int y, const char *pszText, int r, int g, int b, int size, int outline, int frameR, int frameG, int frameB, int bold), {
    Module.sbop2TextQueue = Module.sbop2TextQueue || [];
    Module.sbop2TextQueue.push({
        x: x,
        y: y,
        text: UTF8ToString(pszText),
        r: r,
        g: g,
        b: b,
        size: size,
        outline: !!outline,
        frameR: frameR,
        frameG: frameG,
        frameB: frameB,
        bold: !!bold
    });
});
#endif


CLayerTitle::CLayerTitle()
{
	m_nID = LAYERTYPE_TITLE;
	m_nAnime = 0;
	m_nFadeLevel = 0;
	m_nCloudPos = 1000;
	m_dwLastTimeProc = 0;
	m_dwLastTimeFadeIn = 0;
	m_pDibTitle = NULL;
	m_pDibTitleBack = NULL;
	m_pDibTitleCloud = NULL;
}


CLayerTitle::~CLayerTitle()
{
	SAFE_DELETE(m_pDibTitle);
	SAFE_DELETE(m_pDibTitleBack);
	SAFE_DELETE(m_pDibTitleCloud);
}


void CLayerTitle::Create(
	CMgrData *pMgrData) // [in] データ管理
{
	CLayerBase::Create(pMgrData);

	m_pDibTitle = m_pMgrGrpData->GetDibTmpTitle();
	m_pDibTitleBack = m_pMgrGrpData->GetDibTmpTitleBack();
	m_pDibTitleCloud = m_pMgrGrpData->GetDibTmpTitleCloud();
}


void CLayerTitle::Draw(PCImg32 pDst)
{
	int i, cx, cy, nTmp, x;
	HDC hDCTmp;
	CmyString strTmp;

	nTmp = 100 - (m_nFadeLevel / 2);
	if (m_nFadeLevel > 100) {
		nTmp = 0;
	}
	pDst->BltFrom256(32, 32, 480, 480, m_pDibTitleBack, 0, 0);

	for (i = 0; i < 6; i ++) {
		x = (m_nCloudPos / (6 - i));
		x = (x % 780);
		x -= 260;
		x += 32;
		pDst->BltFrom256(x, 32 + 96, 260, 112, m_pDibTitleCloud, 0, i * 112, TRUE);
	}

	if (m_nFadeLevel > 100) {
		cx = m_pDibTitle->Width();
		cy = m_pDibTitle->Height();
		nTmp = 100 - (m_nFadeLevel - 100);
		pDst->BltAlphaFrom256(SCRSIZEX / 2 - (cx / 2) + 32, 80, cx, cy, m_pDibTitle, 0, 0, nTmp, TRUE);
	}

	if ((m_dwLastTimeFadeIn == 0) && m_hFont) {
#if defined(_WIN32)
		hDCTmp = pDst->Lock();
		strTmp = "Copyright (C)2003-2010 URARA-WORKS. All rights reserved.";
		TextOut1(hDCTmp, m_hFont, (480 - (strTmp.GetLength() * 6)) / 2 + 32, SCRSIZEY - 12 + 32, strTmp, RGB(255, 255, 255));
		pDst->Unlock();
#elif defined(__EMSCRIPTEN__)
		// Emscripten では canvas への直接描画が使えないため、テキストキューに積んで JS 側で描画
		strTmp = "Copyright (C)2003-2010 URARA-WORKS. All rights reserved.";
		SBOP2_QueueLayerText(
			(480 - strTmp.GetLength() * 6) / 2,
			SCRSIZEY - 12,
			(LPCSTR)strTmp,
			255, 255, 255, // 白
			12,            // フォントサイズ (TextOut1 相当)
			0, 0, 0, 0,    // outline なし / frame なし
			0);            // bold なし
#endif
	}
}


BOOL CLayerTitle::TimerProc(void)
{
	BOOL bRet;
	DWORD dwTime, dwTmp;

	bRet = CLayerBase::TimerProc();
	dwTime = SDL_GetTicks();

	if (m_dwLastTimeFadeIn != 0) {
		dwTmp = dwTime - m_dwLastTimeFadeIn;
		if (dwTmp > 50) {
			m_nFadeLevel += (dwTmp / 10);
			if (m_nFadeLevel > 200) {
				m_nFadeLevel = 200;
				m_dwLastTimeFadeIn = 0;
			} else {
				m_dwLastTimeFadeIn = dwTime;
			}
			bRet = TRUE;
		}
	}
	dwTmp = dwTime - m_dwLastTimeProc;
	if (dwTmp > 100) {
		m_dwLastTimeProc = dwTime;
		m_nCloudPos ++;
		if (m_nCloudPos <= 0) {
			m_nCloudPos = 0;
		}
		bRet = TRUE;
	}

	return bRet;
}


void CLayerTitle::StartFadeIn(void)
{
	m_dwLastTimeFadeIn = SDL_GetTicks();
}


void CLayerTitle::EndFadeIn(void)
{
	m_nFadeLevel = 200;
	m_dwLastTimeFadeIn = 0;
}


void CLayerTitle::SetFadeLevel(int nFadeLevel)
{
	m_nFadeLevel = max(0, min(200, nFadeLevel));
	m_dwLastTimeFadeIn = 0;
}


BOOL CLayerTitle::IsFadeInEnd(void)
{
	BOOL bRet;

	bRet = FALSE;
	if (m_nFadeLevel >= 200) {
		bRet = TRUE;
	}
	return bRet;
}
