/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:SaveLoadInfoItemType.cpp									 */
/* 内容			:データ保存・読み込みクラス(アイテム種別情報) 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/10/02													 */
/* ========================================================================= */

#include "stdafx.h"
#include "SaveLoadInfoItemType.h"


/* ========================================================================= */
/* 関数名	:CSaveLoadInfoItemType::CSaveLoadInfoItemType					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/10/02														 */
/* ========================================================================= */

CSaveLoadInfoItemType::CSaveLoadInfoItemType()
{
	/* ファイル名の設定 */
	SetFileName ("SBODATA\\SboDataItemType.dat");
}


/* ========================================================================= */
/* 関数名	:CSaveLoadInfoItemType::~CSaveLoadInfoItemType					 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/10/02														 */
/* ========================================================================= */

CSaveLoadInfoItemType::~CSaveLoadInfoItemType()
{
}

/* Copyright(C)URARA-works 2007 */
