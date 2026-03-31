/// @file SaveLoadInfoEfcBalloon.h
/// @brief データ保存・読み込みクラス(噴出し情報) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/12/24
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "SaveLoadInfoBase.h"

// クラス宣言

typedef class CSaveLoadInfoEfcBalloon : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoEfcBalloon();	// コンストラクタ
	virtual ~CSaveLoadInfoEfcBalloon();	// デストラクタ
} CSaveLoadInfoEfcBalloon, *PCSaveLoadInfoEfcBalloon;
