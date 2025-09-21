/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:SaveLoadInfoSystem.h										 */
/* 内容			:データ保存・読み込みクラス(システム情報) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/09/28													 */
/* ========================================================================= */

#pragma once

#include "SaveLoadInfoBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CSaveLoadInfoSystem : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoSystem();						/* コンストラクタ */
	virtual ~CSaveLoadInfoSystem();						/* デストラクタ */
} CSaveLoadInfoSystem, *PCSaveLoadInfoSystem;

/* Copyright(C)URARA-works 2008 */
