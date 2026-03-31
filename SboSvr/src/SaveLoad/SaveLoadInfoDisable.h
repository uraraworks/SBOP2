/// @file SaveLoadInfoDisable.h
/// @brief データ保存・読み込みクラス(拒否情報) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2009/04/11
/// @copyright Copyright(C)URARA-works 2009

#pragma once

#include "SaveLoadInfoBase.h"

// クラス宣言

typedef class CSaveLoadInfoDisable : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoDisable();	// コンストラクタ
	virtual ~CSaveLoadInfoDisable();	// デストラクタ
} CSaveLoadInfoDisable, *PCSaveLoadInfoDisable;
