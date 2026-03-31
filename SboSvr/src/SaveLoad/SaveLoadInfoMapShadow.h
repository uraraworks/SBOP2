/// @file SaveLoadInfoMapShadow.h
/// @brief データ保存・読み込みクラス(マップ影情報) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/06/06
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "SaveLoadInfoBase.h"

// クラス宣言

typedef class CSaveLoadInfoMapShadow : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoMapShadow();	// コンストラクタ
	virtual ~CSaveLoadInfoMapShadow();	// デストラクタ

protected:
	void	SetHeaderInfo(PCInfoBase pInfo);	// ヘッダ情報を設定
} CSaveLoadInfoMapShadow, *PCSaveLoadInfoMapShadow;
