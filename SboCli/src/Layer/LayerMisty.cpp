/// @file LayerMisty.cpp
/// @brief レイヤー描画クラス(雲) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/10/13
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "MgrLayer.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrDraw.h"
#include "Img32.h"
#include "InfoMapBase.h"
#include "LayerMisty.h"


CLayerMisty::CLayerMisty()
{
	m_nID = WEATHERTYPE_MISTY;
}


CLayerMisty::~CLayerMisty()
{
}


void CLayerMisty::Create(
	CMgrData *pMgrData) // [in] データ管理
{
	CLayerBase::Create(pMgrData);

	m_pDibBase->FillRect(0, 0, m_pDibBase->Width(), m_pDibBase->Height(), RGB(255, 255, 255));
}


void CLayerMisty::Draw(PCImg32 pDst)
{
	int cx, cy, nScale;

	cx = m_pDibBase->Width();
	cy = m_pDibBase->Height();
	nScale = m_pMgrDraw->GetDrawScale(pDst);

	pDst->BltAlphaScaled(32 * nScale,        32 * nScale,        cx, cy, m_pDibBase, 0, 0, 50, TRUE, nScale);
	pDst->BltAlphaScaled((32 + cx) * nScale, 32 * nScale,        cx, cy, m_pDibBase, 0, 0, 50, TRUE, nScale);
	pDst->BltAlphaScaled(32 * nScale,        (32 + cx) * nScale, cx, cy, m_pDibBase, 0, 0, 50, TRUE, nScale);
	pDst->BltAlphaScaled((32 + cx) * nScale, (32 + cx) * nScale, cx, cy, m_pDibBase, 0, 0, 50, TRUE, nScale);
}
