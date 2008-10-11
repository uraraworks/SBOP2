/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:SaveLoadInfoMapParts.h										 */
/* 内容			:データ保存・読み込みクラス(マップパーツ情報) 定義ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/04/30													 */
/* ========================================================================= */

#pragma once

#include "SaveLoadInfoBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CSaveLoadInfoMapParts : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoMapParts();						/* コンストラクタ */
	virtual ~CSaveLoadInfoMapParts();						/* デストラクタ */


protected:
	void	SetHeaderInfo	(PCInfoBase pInfo);		/* ヘッダ情報を設定 */
} CSaveLoadInfoMapParts, *PCSaveLoadInfoMapParts;

/* Copyright(C)URARA-works 2007 */
