/// @file SaveLoadInfoMotionType.h
/// @brief データ保存・読み込みクラス(モーション種別情報) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/05/31
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "SaveLoadInfoBase.h"

// クラス宣言

typedef class CSaveLoadInfoMotionType : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoMotionType();	// コンストラクタ
	virtual ~CSaveLoadInfoMotionType();	// デストラクタ
} CSaveLoadInfoMotionType, *PCSaveLoadInfoMotionType;
