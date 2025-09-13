/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:SaveLoadInfoItem.h											 */
/* 内容			:データ保存・読み込みクラス(アイテム情報) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/08/19													 */
/* ========================================================================= */

#pragma once

#include "SaveLoadInfoBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CSaveLoadInfoItem : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoItem();						/* コンストラクタ */
	virtual ~CSaveLoadInfoItem();						/* デストラクタ */
} CSaveLoadInfoItem, *PCSaveLoadInfoItem;

/* Copyright(C)URARA-works 2007 */
