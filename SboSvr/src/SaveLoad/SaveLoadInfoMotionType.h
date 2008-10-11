/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:SaveLoadInfoMotionType.h									 */
/* 内容			:データ保存・読み込みクラス(モーション種別情報) 定義ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/05/31													 */
/* ========================================================================= */

#pragma once

#include "SaveLoadInfoBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CSaveLoadInfoMotionType : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoMotionType();			/* コンストラクタ */
	virtual ~CSaveLoadInfoMotionType();			/* デストラクタ */
} CSaveLoadInfoMotionType, *PCSaveLoadInfoMotionType;

/* Copyright(C)URARA-works 2008 */
