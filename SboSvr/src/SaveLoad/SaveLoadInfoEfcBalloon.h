/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:SaveLoadInfoEfcBalloon.h									 */
/* 内容			:データ保存・読み込みクラス(噴出し情報) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/12/24													 */
/* ========================================================================= */

#pragma once

#include "SaveLoadInfoBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CSaveLoadInfoEfcBalloon : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoEfcBalloon();			/* コンストラクタ */
	virtual ~CSaveLoadInfoEfcBalloon();			/* デストラクタ */
} CSaveLoadInfoEfcBalloon, *PCSaveLoadInfoEfcBalloon;

/* Copyright(C)URARA-works 2007 */
