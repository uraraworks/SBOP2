/// @file LayerLogo.cpp
/// @brief レイヤー描画クラス(ロゴ) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/05/17
/// @copyright Copyright(C)URARA-works 2005

#include "stdafx.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "Img32.h"
#include "LayerLogo.h"


CLayerLogo::CLayerLogo()
{
	m_nID = LAYERTYPE_LOGO;
	m_pDibLogo = NULL;
}


CLayerLogo::~CLayerLogo()
{
	SAFE_DELETE(m_pDibLogo);
}


void CLayerLogo::Create(
	CMgrData *pMgrData) // [in] データ管理
{
	CLayerBase::Create(pMgrData);

	m_pDibLogo = m_pMgrGrpData->GetDibTmpLogo();
}


void CLayerLogo::Draw(PCImg32 pDst)
{
	int cx, cy;
	HDC hDCBmp, hDCBmp2;

	cx = m_pDibBase->Width();
	cy = m_pDibBase->Height();
	m_pDibBase->FillRect(0, 0, cx, cy, RGB(255, 255, 255));
	m_pDibBase->BltFrom256(cx / 2 - 104, cy / 2 - 40, 208, 80, m_pDibLogo, 0, 0, TRUE);

	hDCBmp2 = m_pDibBase->Lock();
	hDCBmp = pDst->Lock();

	StretchBlt(hDCBmp, 32, 32, pDst->Width() - 64, pDst->Height() - 64,
			hDCBmp2, 0, 0, m_pDibBase->Width(), m_pDibBase->Height(), SRCCOPY);

	m_pDibBase->Unlock();
	pDst->Unlock();
}
