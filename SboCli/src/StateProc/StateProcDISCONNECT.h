/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:StateProcDISCONNECT.h										 */
/* 内容			:状態処理クラス(切断) 定義ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/08													 */
/* ========================================================================= */

#pragma once

#include "StateProcBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CStateProcDISCONNECT : public CStateProcBase
{
public:
			CStateProcDISCONNECT();						/* コンストラクタ */
	virtual ~CStateProcDISCONNECT();					/* デストラクタ */

	void Init	(void);											/* 初期化 */


protected:
} CStateProcDISCONNECT, *PCStateProcDISCONNECT;

/* Copyright(C)URARA-works 2006 */
