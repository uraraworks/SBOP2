/// @file SaveLoadInfoTalkEvent.h
/// @brief データ保存・読み込みクラス(会話イベント情報) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/27
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "SaveLoadInfoBase.h"

// クラス宣言

typedef class CSaveLoadInfoTalkEvent : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoTalkEvent();	// コンストラクタ
	virtual ~CSaveLoadInfoTalkEvent();	// デストラクタ
} CSaveLoadInfoTalkEvent, *PCSaveLoadInfoTalkEvent;
