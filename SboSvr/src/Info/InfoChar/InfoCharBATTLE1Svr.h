/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoCharBATTLE1Svr.h										 */
/* 内容			:キャラ情報(戦闘1)サーバークラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/07/12													 */
/* ========================================================================= */

#pragma once

#include "InfoCharSvr.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoCharBATTLE1Svr : public CInfoCharSvr
{
public:
			CInfoCharBATTLE1Svr();						/* コンストラクタ */
	virtual ~CInfoCharBATTLE1Svr();						/* デストラクタ */

	void SetMoveState	(int nMoveState);						/* 移動状態を変更 */
	BOOL IsLogoutDelete	(void);									/* ログアウト時に削除するか判定 */
	BOOL TimerProc		(DWORD dwTime);							/* 時間処理 */
	void ProcAtack		(void);									/* 処理(攻撃した時) */
	BOOL ProcHit		(CInfoCharSvr *pInfoChar);				/* 処理(攻撃を受けた時) */
	BOOL ProcSWOON		(DWORD dwPara);							/* 行動処理(気絶) */


protected:
	BOOL TimerProcSTAND	(DWORD dwTime);							/* 時間処理(立ち) */
	BOOL TimerProcMOVE	(DWORD dwTime);							/* 時間処理(移動中) */
	BOOL TimerProcBATTLE(DWORD dwTime);							/* 時間処理(戦闘中) */


public:
	DWORD	m_dwLastTiemAtack;				/* 最後に攻撃した時間 */
} CInfoCharBATTLE1Svr, *PCInfoCharBATTLE1Svr;

/* Copyright(C)URARA-works 2008 */
