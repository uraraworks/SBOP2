/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名	:InfoCharBATTLE2Svr.h										 */
/* 内容			:キャラ情報(戦闘2)サーバークラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/07/17													 */
/* ========================================================================= */

#pragma once

#include "InfoCharBATTLE1Svr.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoCharBATTLE2Svr : public CInfoCharBATTLE1Svr
{
public:
			CInfoCharBATTLE2Svr();						/* コンストラクタ */
	virtual ~CInfoCharBATTLE2Svr();						/* デストラクタ */


protected:
	virtual BOOL TimerProcSTAND	(DWORD dwTime);					/* 時間処理(立ち) */


public:
} CInfoCharBATTLE2Svr, *PCInfoCharBATTLE2Svr;

/* Copyright(C)URARA-works 2009 */
