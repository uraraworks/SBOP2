/// @file SaveLoadInfoSystem.cpp
/// @brief データ保存・読み込みクラス(システム情報) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/09/28
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "SaveLoadInfoSystem.h"

CSaveLoadInfoSystem::CSaveLoadInfoSystem()
{
	// ファイル名の設定
	SetFileName("SBODATA\\SboDataSystemInfo.dat");
}

CSaveLoadInfoSystem::~CSaveLoadInfoSystem()
{
}
