/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:InfoCharATACKANIMESvr.h									 */
/* 内容			:キャラ情報(攻撃受けるとアニメーション)サーバークラス 定義ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/17													 */
/* ========================================================================= */

#pragma once

#include "InfoCharSvr.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoCharATACKANIMESvr : public CInfoCharSvr
{
public:
			CInfoCharATACKANIMESvr();					/* コンストラクタ */
	virtual ~CInfoCharATACKANIMESvr();					/* デストラクタ */

	BOOL ProcHit	(CInfoCharSvr *pInfoChar);					/* 処理(攻撃を受けた時) */
	BOOL TimerProc	(DWORD dwTime);								/* 時間処理 */


public:
} CInfoCharATACKANIMESvr, *PCInfoCharATACKANIMESvr;

/* Copyright(C)URARA-works 2007 */
