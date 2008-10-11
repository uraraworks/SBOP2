/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:SaveLoadInfoChar.h											 */
/* 内容			:データ保存・読み込みクラス(キャラ情報) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/04/30													 */
/* ========================================================================= */

#pragma once

#include "SaveLoadInfoBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CSaveLoadInfoChar : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoChar();						/* コンストラクタ */
	virtual ~CSaveLoadInfoChar();						/* デストラクタ */
} CSaveLoadInfoChar, *PCSaveLoadInfoChar;

/* Copyright(C)URARA-works 2007 */
