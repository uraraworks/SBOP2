/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:SaveLoadInfoMapObject.h									 */
/* 内容			:データ保存・読み込みクラス(マップオブジェクト情報) 定義ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/11/01													 */
/* ========================================================================= */

#pragma once

#include "SaveLoadInfoBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CSaveLoadInfoMapObject : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoMapObject();				/* コンストラクタ */
	virtual ~CSaveLoadInfoMapObject();				/* デストラクタ */
} CSaveLoadInfoMapObject, *PCSaveLoadInfoMapObject;

/* Copyright(C)URARA-works 2008 */
