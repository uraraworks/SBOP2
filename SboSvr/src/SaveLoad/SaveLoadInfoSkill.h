/// @file SaveLoadInfoSkill.h
/// @brief データ保存・読み込みクラス(スキル情報) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/04
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "SaveLoadInfoBase.h"

// クラス宣言

typedef class CSaveLoadInfoSkill : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoSkill();	// コンストラクタ
	virtual ~CSaveLoadInfoSkill();	// デストラクタ
} CSaveLoadInfoSkill, *PCSaveLoadInfoSkill;
