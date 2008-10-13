/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:LayerMisty.cpp												 */
/* 内容			:レイヤー描画クラス(雲) 実装ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/10/13													 */
/* ========================================================================= */

#include "stdafx.h"
#include "MgrLayer.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "Img32.h"
#include "InfoMapBase.h"
#include "LayerMisty.h"


/* ========================================================================= */
/* 関数名	:CLayerMisty::CLayerMisty										 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/10/13														 */
/* ========================================================================= */

CLayerMisty::CLayerMisty()
{
	m_nID = WEATHERTYPE_MISTY;
}


/* ========================================================================= */
/* 関数名	:CLayerMisty::~CLayerMisty										 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/10/13														 */
/* ========================================================================= */

CLayerMisty::~CLayerMisty()
{
}


/* ========================================================================= */
/* 関数名	:CLayerMisty::Create											 */
/* 内容		:作成															 */
/* 日付		:2008/10/13														 */
/* ========================================================================= */

void CLayerMisty::Create(
	CMgrData	*pMgrData)		/* [in] データ管理 */
{
	CLayerBase::Create (pMgrData);

	m_pDibBase->FillRect (0, 0, m_pDibBase->Width (), m_pDibBase->Height (), RGB (255, 255, 255));
}


/* ========================================================================= */
/* 関数名	:CLayerMisty::Draw												 */
/* 内容		:描画															 */
/* 日付		:2008/10/13														 */
/* ========================================================================= */

void CLayerMisty::Draw(PCImg32 pDst)
{
	int cx, cy;

	cx = m_pDibBase->Width ();
	cy = m_pDibBase->Height ();

	pDst->BltAlpha (32,		 32,	  cx, cy, m_pDibBase, 0, 0, 50, TRUE);
	pDst->BltAlpha (32 + cx, 32,	  cx, cy, m_pDibBase, 0, 0, 50, TRUE);
	pDst->BltAlpha (32,		 32 + cx, cx, cy, m_pDibBase, 0, 0, 50, TRUE);
	pDst->BltAlpha (32 + cx, 32 + cx, cx, cy, m_pDibBase, 0, 0, 50, TRUE);
}

/* Copyright(C)URARA-works 2008 */
