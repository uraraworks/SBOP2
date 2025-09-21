/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoCharPUTNPC.h											 */
/* 内容			:キャラ情報(NPC発生)サーバークラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/07/12													 */
/* ========================================================================= */

#pragma once

#include "InfoCharSvr.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoCharPUTNPC : public CInfoCharSvr
{
public:
			CInfoCharPUTNPC();							/* コンストラクタ */
	virtual ~CInfoCharPUTNPC();							/* デストラクタ */

	void SetMoveState	(int nMoveState);						/* 移動状態を変更 */
	BOOL TimerProc		(DWORD dwTime);							/* 時間処理 */

	void IncPutCount	(void);									/* 発生NPC数を増加 */
	void DecPutCount	(void);									/* 発生NPC数を減少 */


public:
	DWORD	m_dwLastTimePut;			/* 前回の発生時刻 */
	int		m_nPutCount;				/* 発生中の数 */
} CInfoCharPUTNPC, *PCInfoCharPUTNPC;

/* Copyright(C)URARA-works 2008 */
