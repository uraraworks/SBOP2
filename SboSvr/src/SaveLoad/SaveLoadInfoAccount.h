/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:SaveLoadInfoAccount.h										 */
/* 内容			:データ保存・読み込みクラス(アカウント情報) 定義ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/04/30													 */
/* ========================================================================= */

#pragma once

#include "SaveLoadInfoBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CSaveLoadInfoAccount : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoAccount();						/* コンストラクタ */
	virtual ~CSaveLoadInfoAccount();					/* デストラクタ */
} CSaveLoadInfoAccount, *PCSaveLoadInfoAccount;

/* Copyright(C)URARA-works 2007 */
