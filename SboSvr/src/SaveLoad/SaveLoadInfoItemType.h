/// @file SaveLoadInfoItemType.h
/// @brief データ保存・読み込みクラス(アイテム種別情報) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/10/02
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "SaveLoadInfoBase.h"

// クラス宣言

typedef class CSaveLoadInfoItemType : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoItemType();	// コンストラクタ
	virtual ~CSaveLoadInfoItemType();	// デストラクタ
} CSaveLoadInfoItemType, *PCSaveLoadInfoItemType;
