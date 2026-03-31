/// @file SaveLoadInfoAccount.cpp
/// @brief データ保存・読み込みクラス(アカウント情報) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/30
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "SaveLoadInfoAccount.h"

CSaveLoadInfoAccount::CSaveLoadInfoAccount()
{
	// ファイル名の設定
	SetFileName("SBODATA\\SboDataAccount.dat");
}

CSaveLoadInfoAccount::~CSaveLoadInfoAccount()
{
}
