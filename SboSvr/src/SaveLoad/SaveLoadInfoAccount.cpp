/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:SaveLoadInfoAccount.cpp									 */
/* 内容			:データ保存・読み込みクラス(アカウント情報) 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/04/30													 */
/* ========================================================================= */

#include "stdafx.h"
#include "SaveLoadInfoAccount.h"


/* ========================================================================= */
/* 関数名	:CSaveLoadInfoAccount::CSaveLoadInfoAccount						 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

CSaveLoadInfoAccount::CSaveLoadInfoAccount()
{
	/* ファイル名の設定 */
	SetFileName ("SBODATA\\SboDataAccount.dat");
}


/* ========================================================================= */
/* 関数名	:CSaveLoadInfoAccount::~CSaveLoadInfoAccount					 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

CSaveLoadInfoAccount::~CSaveLoadInfoAccount()
{
}

/* Copyright(C)URARA-works 2007 */
