/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* ファイル名	:SaveLoadInfoMap.h											 */
/* 内容			:データ保存・読み込みクラス(マップ情報) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2005/11/08													 */
/* ========================================================================= */

#pragma once

#include "SaveLoadInfoBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CSaveLoadInfoMap : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoMap();							/* コンストラクタ */
	virtual ~CSaveLoadInfoMap();						/* デストラクタ */
} CSaveLoadInfoMap, *PCSaveLoadInfoMap;

/* Copyright(C)URARA-works 2005 */
