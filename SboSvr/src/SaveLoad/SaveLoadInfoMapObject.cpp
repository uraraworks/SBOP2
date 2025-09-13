/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:SaveLoadInfoMapObject.cpp									 */
/* 内容			:データ保存・読み込みクラス(マップパーツ情報) 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/11/01													 */
/* ========================================================================= */

#include "stdafx.h"
#include "SaveLoadInfoMapObject.h"


/* ========================================================================= */
/* 関数名	:CSaveLoadInfoMapObject::CSaveLoadInfoMapObject					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

CSaveLoadInfoMapObject::CSaveLoadInfoMapObject()
{
	/* ファイル名の設定 */
	SetFileName ("SBODATA\\SboDataMapObject.dat");
}


/* ========================================================================= */
/* 関数名	:CSaveLoadInfoMapObject::~CSaveLoadInfoMapObject				 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

CSaveLoadInfoMapObject::~CSaveLoadInfoMapObject()
{
}

/* Copyright(C)URARA-works 2008 */
