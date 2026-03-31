/// @file SaveLoadInfoSystem.h
/// @brief データ保存・読み込みクラス(システム情報) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/09/28
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "SaveLoadInfoBase.h"

// クラス宣言

typedef class CSaveLoadInfoSystem : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoSystem();	// コンストラクタ
	virtual ~CSaveLoadInfoSystem();	// デストラクタ
} CSaveLoadInfoSystem, *PCSaveLoadInfoSystem;
