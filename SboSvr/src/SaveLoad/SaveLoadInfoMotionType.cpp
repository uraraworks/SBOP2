/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:SaveLoadInfoMotionType.cpp									 */
/* 内容			:データ保存・読み込みクラス(モーション種別情報) 実装ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/05/31													 */
/* ========================================================================= */

#include "stdafx.h"
#include "SaveLoadInfoMotionType.h"


/* ========================================================================= */
/* 関数名	:CSaveLoadInfoMotionType::CSaveLoadInfoMotionType				 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/05/31														 */
/* ========================================================================= */

CSaveLoadInfoMotionType::CSaveLoadInfoMotionType()
{
	/* ファイル名の設定 */
	SetFileName ("SBODATA\\SboDataMotionType.dat");
}


/* ========================================================================= */
/* 関数名	:CSaveLoadInfoMotionType::~CSaveLoadInfoMotionType				 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/05/31														 */
/* ========================================================================= */

CSaveLoadInfoMotionType::~CSaveLoadInfoMotionType()
{
}

/* Copyright(C)URARA-works 2008 */
