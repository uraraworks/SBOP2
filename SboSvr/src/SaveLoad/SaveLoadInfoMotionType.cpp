/// @file SaveLoadInfoMotionType.cpp
/// @brief データ保存・読み込みクラス(モーション種別情報) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/05/31
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "SaveLoadInfoMotionType.h"

CSaveLoadInfoMotionType::CSaveLoadInfoMotionType()
{
	// ファイル名の設定
	SetFileName("SBODATA\\SboDataMotionType.dat");
}

CSaveLoadInfoMotionType::~CSaveLoadInfoMotionType()
{
}
