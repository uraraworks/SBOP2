/// @file SaveLoadInfoMotion.cpp
/// @brief データ保存・読み込みクラス(モーション情報) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/11/23
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "SaveLoadInfoMotion.h"

CSaveLoadInfoMotion::CSaveLoadInfoMotion()
{
	// ファイル名の設定
	SetFileName("SBODATA\\SboDataMotion.dat");
}

CSaveLoadInfoMotion::~CSaveLoadInfoMotion()
{
}
