/// @file SaveLoadInfoChar.cpp
/// @brief データ保存・読み込みクラス(キャラ情報) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/30
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "SaveLoadInfoChar.h"

CSaveLoadInfoChar::CSaveLoadInfoChar()
{
	// ファイル名の設定
	SetFileName("SBODATA\\SboDataChar.dat");
}

CSaveLoadInfoChar::~CSaveLoadInfoChar()
{
}
