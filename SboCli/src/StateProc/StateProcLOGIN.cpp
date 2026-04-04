/// @file StateProcLOGIN.cpp
/// @brief 状態処理クラス(ログイン画面) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/03
/// @copyright Copyright(C)URARA-works 2006

#include "stdafx.h"
#include "WindowLOGIN.h"
#include "LayerTitle.h"
#include "MgrData.h"
#include "MgrSound.h"
#include "MgrWindow.h"
#include "MgrDraw.h"
#include "MgrLayer.h"
#include "MainFrame.h"
#include "Window/ILoginWindow.h"
#include "LayerTitle.h"
#include "StateProcLOGIN.h"

CStateProcLOGIN::CStateProcLOGIN()
{
	m_bFadeIn = FALSE;
}

CStateProcLOGIN::~CStateProcLOGIN()
{
}

void CStateProcLOGIN::Init(void)
{
	ILoginWindow *pWindow;
	PCLayerTitle pLayerTitle;

	m_pMgrWindow->MakeWindowLOGIN();
	m_pMgrLayer->MakeTITLE();
	pLayerTitle = (PCLayerTitle)m_pMgrLayer->Get(LAYERTYPE_TITLE);

	pWindow = m_pMgrWindow->GetLoginWindow();

	if (m_pMgrData->IsLocalTitleMode() && IsBrowserPlatform()) {
		m_pMgrDraw->SetLevel(100);
		if (pLayerTitle) {
			pLayerTitle->SetFadeLevel(200);
		}
		if (pWindow) {
			pWindow->SetShow(TRUE);
		}
		m_bFadeIn = TRUE;
	} else {
		m_pMgrDraw->SetFadeState(FADESTATE_FADEIN);
		if (pWindow) {
			pWindow->SetShow(FALSE);
		}
	}

	m_pMgrSound->PlayBGM(BGMID_HISYOU);
}

BOOL CStateProcLOGIN::TimerProc(void)
{
	PCLayerTitle pLayer;
	ILoginWindow *pWindow;

	if (m_bFadeIn) {
		return FALSE;
	}

	pLayer = (PCLayerTitle)m_pMgrLayer->Get(LAYERTYPE_TITLE);
	if (pLayer->IsFadeInEnd()) {
		m_bFadeIn = TRUE;

		pWindow = m_pMgrWindow->GetLoginWindow();
		if (pWindow) {
			pWindow->SetShow(TRUE);
		}
	}

	return TRUE;
}

void CStateProcLOGIN::OnLButtonDown(int x, int y)
{
	ILoginWindow *pWindow;
	PCLayerTitle pLayer;

	pWindow = m_pMgrWindow->GetLoginWindow();
	if ((m_bFadeIn) && (pWindow != NULL) && pWindow->HandleMouseLeftButtonDown(x, y)) {
		return;
	}
	if (m_pMgrData->IsLocalTitleMode() && IsBrowserPlatform()) {
		return;
	}

	pLayer = (PCLayerTitle)m_pMgrLayer->Get(LAYERTYPE_TITLE);
	pLayer->EndFadeIn();
}

BOOL CStateProcLOGIN::OnX(BOOL bDown)
{
	OnLButtonDown(0, 0);

	return TRUE;
}

void CStateProcLOGIN::OnMgrDrawSTART_FADEIN(DWORD dwPara)
{
	PCLayerTitle pLayer;

	pLayer = (PCLayerTitle)m_pMgrLayer->Get(LAYERTYPE_TITLE);
	pLayer->StartFadeIn();
}
