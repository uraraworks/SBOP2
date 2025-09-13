/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:SaveLoadInfoTalkEvent.cpp									 */
/* 内容			:データ保存・読み込みクラス(会話イベント情報) 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/27													 */
/* ========================================================================= */

#include "stdafx.h"
#include "SaveLoadInfoTalkEvent.h"


/* ========================================================================= */
/* 関数名	:CSaveLoadInfoTalkEvent::CSaveLoadInfoTalkEvent					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/27														 */
/* ========================================================================= */

CSaveLoadInfoTalkEvent::CSaveLoadInfoTalkEvent()
{
	/* ファイル名の設定 */
	SetFileName ("SBODATA\\SboDataTalkEvent.dat");
}


/* ========================================================================= */
/* 関数名	:CSaveLoadInfoTalkEvent::~CSaveLoadInfoTalkEvent				 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/27														 */
/* ========================================================================= */

CSaveLoadInfoTalkEvent::~CSaveLoadInfoTalkEvent()
{
}

/* Copyright(C)URARA-works 2008 */
