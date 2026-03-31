/// @file SaveLoadInfoMapObject.h
/// @brief データ保存・読み込みクラス(マップオブジェクト情報) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/11/01
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "SaveLoadInfoBase.h"

// クラス宣言

typedef class CSaveLoadInfoMapObject : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoMapObject();	// コンストラクタ
	virtual ~CSaveLoadInfoMapObject();	// デストラクタ
} CSaveLoadInfoMapObject, *PCSaveLoadInfoMapObject;
