/// @file SaveLoadInfoTalkEvent.cpp
/// @brief データ保存・読み込みクラス(会話イベント情報) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/27
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "SaveLoadInfoTalkEvent.h"

CSaveLoadInfoTalkEvent::CSaveLoadInfoTalkEvent()
{
	// ファイル名の設定
	SetFileName("SBODATA\\SboDataTalkEvent.dat");
}

CSaveLoadInfoTalkEvent::~CSaveLoadInfoTalkEvent()
{
}
