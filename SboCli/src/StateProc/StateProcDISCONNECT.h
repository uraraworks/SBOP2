/// @file StateProcDISCONNECT.h
/// @brief 状態処理クラス(切断) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/08
/// @copyright Copyright(C)URARA-works 2006

#pragma once

#include "StateProcBase.h"

// クラス宣言

typedef class CStateProcDISCONNECT : public CStateProcBase
{
public:
			CStateProcDISCONNECT();  // コンストラクタ
	virtual ~CStateProcDISCONNECT(); // デストラクタ

	void Init(void); // 初期化


protected:
} CStateProcDISCONNECT, *PCStateProcDISCONNECT;
