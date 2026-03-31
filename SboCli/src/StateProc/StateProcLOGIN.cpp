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
	PCWindowLOGIN pWindow;

	m_pMgrWindow->MakeWindowLOGIN();
	m_pMgrDraw->SetFadeState(FADESTATE_FADEIN);
	m_pMgrLayer->MakeTITLE();

	pWindow = (PCWindowLOGIN)m_pMgrWindow->GetWindow(WINDOWTYPE_LOGIN);
	pWindow->SetShow(FALSE);

	m_pMgrSound->PlayBGM(BGMID_HISYOU);
}

BOOL CStateProcLOGIN::TimerProc(void)
{
	PCLayerTitle pLayer;
	PCWindowLOGIN pWindow;

	if (m_bFadeIn) {
		return FALSE;
	}

	pLayer = (PCLayerTitle)m_pMgrLayer->Get(LAYERTYPE_TITLE);
	if (pLayer->IsFadeInEnd()) {
		m_bFadeIn = TRUE;

		pWindow = (PCWindowLOGIN)m_pMgrWindow->GetWindow(WINDOWTYPE_LOGIN);
		pWindow->SetShow(TRUE);
	}

	return TRUE;
}

void CStateProcLOGIN::OnLButtonDown(int x, int y)
{
	PCLayerTitle pLayer;

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
