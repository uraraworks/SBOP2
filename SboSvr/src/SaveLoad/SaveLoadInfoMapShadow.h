/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:SaveLoadInfoMapShadow.h									 */
/* 内容			:データ保存・読み込みクラス(マップ影情報) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/06/06													 */
/* ========================================================================= */

#pragma once

#include "SaveLoadInfoBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CSaveLoadInfoMapShadow : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoMapShadow();						/* コンストラクタ */
	virtual ~CSaveLoadInfoMapShadow();						/* デストラクタ */


protected:
	void	SetHeaderInfo	(PCInfoBase pInfo);		/* ヘッダ情報を設定 */
} CSaveLoadInfoMapShadow, *PCSaveLoadInfoMapShadow;

/* Copyright(C)URARA-works 2007 */
