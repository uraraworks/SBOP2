/// @file SaveLoadInfoItemWeapon.h
/// @brief データ保存・読み込みクラス(武器情報) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/08/11
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "SaveLoadInfoBase.h"

// クラス宣言

typedef class CSaveLoadInfoItemWeapon : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoItemWeapon();	// コンストラクタ
	virtual ~CSaveLoadInfoItemWeapon();	// デストラクタ
} CSaveLoadInfoItemWeapon, *PCSaveLoadInfoItemWeapon;
