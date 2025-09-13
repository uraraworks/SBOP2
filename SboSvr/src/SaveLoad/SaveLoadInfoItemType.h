/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:SaveLoadInfoItemType.h										 */
/* 内容			:データ保存・読み込みクラス(アイテム種別情報) 定義ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/10/02													 */
/* ========================================================================= */

#pragma once

#include "SaveLoadInfoBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CSaveLoadInfoItemType : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoItemType();						/* コンストラクタ */
	virtual ~CSaveLoadInfoItemType();						/* デストラクタ */
} CSaveLoadInfoItemType, *PCSaveLoadInfoItemType;

/* Copyright(C)URARA-works 2007 */
