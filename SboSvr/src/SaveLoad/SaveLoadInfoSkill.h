/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:SaveLoadInfoSkill.h										 */
/* 内容			:データ保存・読み込みクラス(スキル情報) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/04													 */
/* ========================================================================= */

#pragma once

#include "SaveLoadInfoBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CSaveLoadInfoSkill : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoSkill();				/* コンストラクタ */
	virtual ~CSaveLoadInfoSkill();				/* デストラクタ */
} CSaveLoadInfoSkill, *PCSaveLoadInfoSkill;

/* Copyright(C)URARA-works 2008 */
