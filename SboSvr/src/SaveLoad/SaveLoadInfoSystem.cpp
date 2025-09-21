/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:SaveLoadInfoSystem.cpp										 */
/* 内容			:データ保存・読み込みクラス(システム情報) 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/09/28													 */
/* ========================================================================= */

#include "stdafx.h"
#include "SaveLoadInfoSystem.h"


/* ========================================================================= */
/* 関数名	:CSaveLoadInfoSystem::CSaveLoadInfoSystem						 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/09/28														 */
/* ========================================================================= */

CSaveLoadInfoSystem::CSaveLoadInfoSystem()
{
	/* ファイル名の設定 */
	SetFileName ("SBODATA\\SboDataSystemInfo.dat");
}


/* ========================================================================= */
/* 関数名	:CSaveLoadInfoSystem::~CSaveLoadInfoSystem						 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/09/28														 */
/* ========================================================================= */

CSaveLoadInfoSystem::~CSaveLoadInfoSystem()
{
}

/* Copyright(C)URARA-works 2008 */
