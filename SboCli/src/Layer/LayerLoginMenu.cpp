/// @file LayerLoginMenu.cpp
/// @brief レイヤー描画クラス(ログインメニュー) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/15
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
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

	// 操作案内は絵ではなく文字で描く（1 行 22px）
	DrawKeyHelp(pDst, 32 + 2, 32 + SCRSIZEY - 22 * 3, _T("← ↑ ↓ →"), _T("移動"));
	DrawKeyHelp(pDst, 32 + 2, 32 + SCRSIZEY - 22 * 2, _T("X"), _T("決定"));
	DrawKeyHelp(pDst, 32 + 2, 32 + SCRSIZEY - 22 * 1, _T("Z"), _T("取消"));
}
