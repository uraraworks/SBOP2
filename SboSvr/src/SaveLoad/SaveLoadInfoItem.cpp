/// @file SaveLoadInfoItem.cpp
/// @brief データ保存・読み込みクラス(アイテム情報) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/08/19
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "SaveLoadInfoItem.h"

CSaveLoadInfoItem::CSaveLoadInfoItem()
{
	// ファイル名の設定
	SetFileName("SBODATA\\SboDataItem.dat");
}

CSaveLoadInfoItem::~CSaveLoadInfoItem()
{
}
