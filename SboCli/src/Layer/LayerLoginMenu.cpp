/// @file LayerLoginMenu.cpp
/// @brief レイヤー描画クラス(ログインメニュー) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/15
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "Img32.h"
#include "LayerLoginMenu.h"


CLayerLoginMenu::CLayerLoginMenu()
{
	m_nID = LAYERTYPE_LOGINMENU;
	m_dwLastTimeProc = 0;
	m_pDibBack = NULL;
}


CLayerLoginMenu::~CLayerLoginMenu()
{
	SAFE_DELETE(m_pDibBack);
}


void CLayerLoginMenu::Create(
	CMgrData *pMgrData) // [in] データ管理
{
	CLayerCloud::Create(pMgrData);

	m_pDibBack = m_pMgrGrpData->GetDibTmpLoginMenuBack();
}


void CLayerLoginMenu::Draw(PCImg32 pDst)
{
	pDst->Blt(32, 32, m_pDibBack->Width(), m_pDibBack->Height(), m_pDibBack, 0, 0);
	CLayerCloud::Draw(pDst);

	pDst->BltFrom256(32, SCRSIZEY - 32, 112, 32, m_pDibSystem, 688,  80, TRUE);
	pDst->BltFrom256(32, SCRSIZEY,       43, 16, m_pDibSystem, 688, 112, TRUE);
	pDst->BltFrom256(32, SCRSIZEY + 16,  43, 16, m_pDibSystem, 688, 128, TRUE);
}
