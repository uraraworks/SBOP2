/// @file LayerInfo.cpp
/// @brief レイヤー描画クラス(お知らせ) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/08/16
/// @copyright Copyright(C)URARA-works 2005

#include "stdafx.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "Img32.h"
#include "LayerInfo.h"


CLayerInfo::CLayerInfo()
{
	m_nID = LAYERTYPE_INFO;
	m_nX = 64;
	m_nY = 64;
	m_nCx = 24;
	m_nCy = 24;
	m_pDibSystem = NULL;
}


CLayerInfo::~CLayerInfo()
{
}


void CLayerInfo::Create(
	CMgrData *pMgrData) // [in] データ管理
{
	CLayerBase::Create(pMgrData);
}


void CLayerInfo::Draw(PCImg32 pDst)
{
	HDC hDCTmp;

	DrawFrame(pDst);

	hDCTmp = pDst->Lock();

	TextOut1(hDCTmp, m_hFont, m_nX + 16, m_nY + 32, _T("お知らせ"), RGB(0, 0, 0));

	pDst->Unlock();
}


void CLayerInfo::DrawFrame(PCImg32 pDst)
{
	int i, x, y, cx, cy;

	x = m_nX;
	y = m_nY;
	cx = m_nCx;
	cy = m_nCy;

	pDst->BltFrom256(x, y, 16, 16, m_pDibSystem, 0, 48, TRUE);
	for (i = 0; i < cx; i ++) {
		pDst->BltFrom256(x + 16 + (i * 16), y, 16, 16, m_pDibSystem, 16, 48, TRUE);
	}
	pDst->BltFrom256(x + 16 + (i * 16), y, 16, 16, m_pDibSystem, 32, 48, TRUE);

	y += 16;
	for (i = 0; i < cy; i ++) {
		pDst->BltFrom256(x, y + (i * 16), 16, 16, m_pDibSystem, 0, 64, TRUE);
		pDst->BltFrom256(x + 16 + (cx * 16), y + (i * 16), 16, 16, m_pDibSystem, 32, 64, TRUE);
	}

	y += (i * 16);
	pDst->BltFrom256(x, y, 16, 16, m_pDibSystem, 0, 80, TRUE);
	for (i = 0; i < cx; i ++) {
		pDst->BltFrom256(x + 16 + (i * 16), y, 16, 16, m_pDibSystem, 16, 80, TRUE);
	}
	pDst->BltFrom256(x + 16 + (i * 16), y, 16, 16, m_pDibSystem, 32, 80, TRUE);

	pDst->FillRect(m_nX + 16, m_nY + 16, cx * 16, cy * 16, RGB(255, 235, 200));
}
