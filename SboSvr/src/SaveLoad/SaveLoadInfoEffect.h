/// @file SaveLoadInfoEffect.h
/// @brief データ保存・読み込みクラス(エフェクト情報) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/07/26
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "SaveLoadInfoBase.h"

// クラス宣言

typedef class CSaveLoadInfoEffect : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoEffect();	// コンストラクタ
	virtual ~CSaveLoadInfoEffect();	// デストラクタ

protected:
	void	SetHeaderInfo(PCInfoBase pInfo);	// ヘッダ情報を設定
} CSaveLoadInfoEffect, *PCSaveLoadInfoEffect;
