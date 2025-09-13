/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:SaveLoadInfoTalkEvent.h									 */
/* 内容			:データ保存・読み込みクラス(会話イベント情報) 定義ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/27													 */
/* ========================================================================= */

#pragma once

#include "SaveLoadInfoBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CSaveLoadInfoTalkEvent : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoTalkEvent();				/* コンストラクタ */
	virtual ~CSaveLoadInfoTalkEvent();				/* デストラクタ */
} CSaveLoadInfoTalkEvent, *PCSaveLoadInfoTalkEvent;

/* Copyright(C)URARA-works 2008 */
