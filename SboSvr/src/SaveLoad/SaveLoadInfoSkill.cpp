/// @file SaveLoadInfoSkill.cpp
/// @brief データ保存・読み込みクラス(スキル情報) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/04
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "SaveLoadInfoSkill.h"

CSaveLoadInfoSkill::CSaveLoadInfoSkill()
{
	// ファイル名の設定
	SetFileName("SBODATA\\SboDataSkill.dat");
}

CSaveLoadInfoSkill::~CSaveLoadInfoSkill()
{
}
