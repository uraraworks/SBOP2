/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名	:SaveLoadInfoDisable.h										 */
/* 内容			:データ保存・読み込みクラス(拒否情報) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/04/11													 */
/* ========================================================================= */

#pragma once

#include "SaveLoadInfoBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CSaveLoadInfoDisable : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoDisable();					/* コンストラクタ */
	virtual ~CSaveLoadInfoDisable();				/* デストラクタ */
} CSaveLoadInfoDisable, *PCSaveLoadInfoDisable;

/* Copyright(C)URARA-works 2009 */
