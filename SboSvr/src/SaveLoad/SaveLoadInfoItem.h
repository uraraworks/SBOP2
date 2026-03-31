/// @file SaveLoadInfoItem.h
/// @brief データ保存・読み込みクラス(アイテム情報) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/08/19
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "SaveLoadInfoBase.h"

// クラス宣言

typedef class CSaveLoadInfoItem : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoItem();	// コンストラクタ
	virtual ~CSaveLoadInfoItem();	// デストラクタ
} CSaveLoadInfoItem, *PCSaveLoadInfoItem;
