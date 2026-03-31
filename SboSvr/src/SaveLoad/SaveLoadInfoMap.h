/// @file SaveLoadInfoMap.h
/// @brief データ保存・読み込みクラス(マップ情報) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/11/08
/// @copyright Copyright(C)URARA-works 2005

#pragma once

#include "SaveLoadInfoBase.h"

// クラス宣言

typedef class CSaveLoadInfoMap : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoMap();	// コンストラクタ
	virtual ~CSaveLoadInfoMap();	// デストラクタ
} CSaveLoadInfoMap, *PCSaveLoadInfoMap;
