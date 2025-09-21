/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:StateProcDISCONNECT.cpp									 */
/* 内容			:状態処理クラス(切断) 実装ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/08													 */
/* ========================================================================= */

#include "stdafx.h"
#include "MgrData.h"
#include "MgrSound.h"
#include "MgrWindow.h"
#include "MgrDraw.h"
#include "MgrLayer.h"
#include "StateProcDISCONNECT.h"

/* ========================================================================= */
/* 関数名	:CStateProcDISCONNECT::CStateProcDISCONNECT						 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/11/08														 */
/* ========================================================================= */

CStateProcDISCONNECT::CStateProcDISCONNECT()
{
}


/* ========================================================================= */
/* 関数名	:CStateProcDISCONNECT::~CStateProcDISCONNECT					 */
/* 内容		:デストラクタ													 */
/* 日付		:2006/11/08														 */
/* ========================================================================= */

CStateProcDISCONNECT::~CStateProcDISCONNECT()
{
}


/* ========================================================================= */
/* 関数名	:CStateProcDISCONNECT::Init										 */
/* 内容		:初期化															 */
/* 日付		:2006/11/08														 */
/* ========================================================================= */

void CStateProcDISCONNECT::Init(void)
{
	m_pMgrWindow->MakeWindowMSG ("サーバーとの接続が切れました");
}

/* Copyright(C)URARA-works 2006 */
