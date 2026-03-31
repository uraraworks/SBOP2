/// @file SaveLoadInfoMap.cpp
/// @brief データ保存・読み込みクラス(マップ情報) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/11/08
/// @copyright Copyright(C)URARA-works 2005

#include "stdafx.h"
#include "SaveLoadInfoMap.h"

CSaveLoadInfoMap::CSaveLoadInfoMap()
{
	// ファイル名の設定
	SetFileName("SBODATA\\SboDataMap.dat");
}

CSaveLoadInfoMap::~CSaveLoadInfoMap()
{
}
