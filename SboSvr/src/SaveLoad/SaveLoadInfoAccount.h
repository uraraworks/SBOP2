/// @file SaveLoadInfoAccount.h
/// @brief データ保存・読み込みクラス(アカウント情報) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/30
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "SaveLoadInfoBase.h"

// クラス宣言

typedef class CSaveLoadInfoAccount : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoAccount();	// コンストラクタ
	virtual ~CSaveLoadInfoAccount();	// デストラクタ
} CSaveLoadInfoAccount, *PCSaveLoadInfoAccount;
