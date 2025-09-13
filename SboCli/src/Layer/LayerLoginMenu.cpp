/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:LayerLoginMenu.cpp											 */
/* 内容			:レイヤー描画クラス(ログインメニュー) 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/04/15													 */
/* ========================================================================= */

#include "stdafx.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "Img32.h"
#include "LayerLoginMenu.h"


/* ========================================================================= */
/* 関数名	:CLayerLoginMenu::CLayerLoginMenu								 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/04/15														 */
/* ========================================================================= */

CLayerLoginMenu::CLayerLoginMenu()
{
	m_nID				= LAYERTYPE_LOGINMENU;
	m_dwLastTimeProc	= 0;
	m_pDibBack			= NULL;
}


/* ========================================================================= */
/* 関数名	:CLayerLoginMenu::~CLayerLoginMenu								 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/04/15														 */
/* ========================================================================= */

CLayerLoginMenu::~CLayerLoginMenu()
{
	SAFE_DELETE (m_pDibBack);
}


/* ========================================================================= */
/* 関数名	:CLayerLoginMenu::Create										 */
/* 内容		:作成															 */
/* 日付		:2005/05/21														 */
/* ========================================================================= */

void CLayerLoginMenu::Create(
	CMgrData	*pMgrData)		/* [in] データ管理 */
{
	CLayerCloud::Create (pMgrData);

	m_pDibBack = m_pMgrGrpData->GetDibTmpLoginMenuBack ();
}


/* ========================================================================= */
/* 関数名	:CLayerLoginMenu::Draw											 */
/* 内容		:描画															 */
/* 日付		:2007/04/15														 */
/* ========================================================================= */

void CLayerLoginMenu::Draw(PCImg32 pDst)
{
	pDst->Blt (32, 32, m_pDibBack->Width (), m_pDibBack->Height (), m_pDibBack, 0, 0);
	CLayerCloud::Draw (pDst);

	pDst->BltFrom256 (32, SCRSIZEY - 32, 112, 32, m_pDibSystem, 688,  80, TRUE);
	pDst->BltFrom256 (32, SCRSIZEY	   ,  43, 16, m_pDibSystem, 688, 112, TRUE);
	pDst->BltFrom256 (32, SCRSIZEY + 16,  43, 16, m_pDibSystem, 688, 128, TRUE);
}

/* Copyright(C)URARA-works 2007 */
