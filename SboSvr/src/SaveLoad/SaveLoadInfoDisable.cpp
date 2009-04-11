/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名	:SaveLoadInfoDisable.cpp									 */
/* 内容			:データ保存・読み込みクラス(拒否情報) 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/04/11													 */
/* ========================================================================= */

#include "stdafx.h"
#include "SaveLoadInfoDisable.h"


/* ========================================================================= */
/* 関数名	:CSaveLoadInfoDisable::CSaveLoadInfoDisable						 */
/* 内容		:コンストラクタ													 */
/* 日付		:2009/04/11														 */
/* ========================================================================= */

CSaveLoadInfoDisable::CSaveLoadInfoDisable()
{
	/* ファイル名の設定 */
	SetFileName ("SBODATA\\SboDataDisable.dat");
}


/* ========================================================================= */
/* 関数名	:CSaveLoadInfoDisable::~CSaveLoadInfoDisable					 */
/* 内容		:デストラクタ													 */
/* 日付		:2009/04/11														 */
/* ========================================================================= */

CSaveLoadInfoDisable::~CSaveLoadInfoDisable()
{
}

/* Copyright(C)URARA-works 2009 */
