/// @file SaveLoadInfoMapObject.cpp
/// @brief データ保存・読み込みクラス(マップパーツ情報) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/11/01
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "SaveLoadInfoMapObject.h"

CSaveLoadInfoMapObject::CSaveLoadInfoMapObject()
{
	// ファイル名の設定
	SetFileName("SBODATA\\SboDataMapObject.dat");
}

CSaveLoadInfoMapObject::~CSaveLoadInfoMapObject()
{
}
