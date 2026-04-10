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
// LayerTitle 生成時のデバッグマーク: 背景・タイトル・雲の NULL 有無とサイズを JS に記録
EM_JS(void, SBOP2_DebugMarkLayerTitleCreate, (int backNull, int backW, int backH, int titleNull, int titleW, int titleH, int cloudNull, int cloudW, int cloudH), {
    Module.sbop2LayerTitleCreated = 1;
    Module.sbop2LayerTitleBackNull = backNull;
    Module.sbop2LayerTitleBackSize = backW + 'x' + backH;
    Module.sbop2LayerTitleTitleNull = titleNull;
    Module.sbop2LayerTitleTitleSize = titleW + 'x' + titleH;
    Module.sbop2LayerTitleCloudNull = cloudNull;
    Module.sbop2LayerTitleCloudSize = cloudW + 'x' + cloudH;
});
// LayerTitle 描画毎のデバッグマーク: フェードレベル・雲位置・背景 NULL 有無を JS に蓄積
EM_JS(void, SBOP2_DebugMarkLayerTitleDraw, (int fadeLevel, int cloudPos, int backNull), {
    Module.sbop2LayerTitleDrawCount = (Module.sbop2LayerTitleDrawCount || 0) + 1;
    Module.sbop2LayerTitleLastFade = fadeLevel;
    Module.sbop2LayerTitleLastCloudPos = cloudPos;
    Module.sbop2LayerTitleBackNullAtDraw = backNull;
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

#if defined(__EMSCRIPTEN__)
	// Create 直後の各画像の NULL 有無とサイズをブラウザ JS に記録（描画されない原因調査用）
	SBOP2_DebugMarkLayerTitleCreate(
		(m_pDibTitleBack  == NULL) ? 1 : 0,
		(m_pDibTitleBack  != NULL) ? m_pDibTitleBack->Width()  : 0,
		(m_pDibTitleBack  != NULL) ? m_pDibTitleBack->Height() : 0,
		(m_pDibTitle      == NULL) ? 1 : 0,
		(m_pDibTitle      != NULL) ? m_pDibTitle->Width()      : 0,
		(m_pDibTitle      != NULL) ? m_pDibTitle->Height()     : 0,
		(m_pDibTitleCloud == NULL) ? 1 : 0,
		(m_pDibTitleCloud != NULL) ? m_pDibTitleCloud->Width()  : 0,
		(m_pDibTitleCloud != NULL) ? m_pDibTitleCloud->Height() : 0);
#endif
}


void CLayerTitle::Draw(PCImg32 pDst)
{
	int i, cx, cy, nTmp, x;
	HDC hDCTmp;
	CmyString strTmp;

#if defined(__EMSCRIPTEN__)
	// Draw 呼び出し毎にフェードレベル・雲位置・背景 NULL 有無を JS に記録
	SBOP2_DebugMarkLayerTitleDraw(m_nFadeLevel, m_nCloudPos, (m_pDibTitleBack == NULL) ? 1 : 0);
#endif

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
