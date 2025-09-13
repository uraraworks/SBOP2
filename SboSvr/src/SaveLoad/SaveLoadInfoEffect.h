/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:SaveLoadInfoEffect.h										 */
/* 内容			:データ保存・読み込みクラス(エフェクト情報) 定義ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/26													 */
/* ========================================================================= */

#pragma once

#include "SaveLoadInfoBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CSaveLoadInfoEffect : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoEffect();					/* コンストラクタ */
	virtual ~CSaveLoadInfoEffect();					/* デストラクタ */


protected:
	void	SetHeaderInfo	(PCInfoBase pInfo);		/* ヘッダ情報を設定 */
} CSaveLoadInfoEffect, *PCSaveLoadInfoEffect;

/* Copyright(C)URARA-works 2007 */
