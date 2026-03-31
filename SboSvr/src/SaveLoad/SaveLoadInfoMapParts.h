/// @file SaveLoadInfoMapParts.h
/// @brief データ保存・読み込みクラス(マップパーツ情報) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/30
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "SaveLoadInfoBase.h"

// クラス宣言

typedef class CSaveLoadInfoMapParts : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoMapParts();	// コンストラクタ
	virtual ~CSaveLoadInfoMapParts();	// デストラクタ

protected:
	void	SetHeaderInfo(PCInfoBase pInfo);	// ヘッダ情報を設定
} CSaveLoadInfoMapParts, *PCSaveLoadInfoMapParts;
