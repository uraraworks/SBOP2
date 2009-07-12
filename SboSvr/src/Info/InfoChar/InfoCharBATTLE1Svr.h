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

	virtual void SetTarget		(CInfoCharBase *pCharTarget);	/* ターゲットキャラを設定 */
	virtual void SetMoveState	(int nMoveState);				/* 移動状態を変更 */
	virtual BOOL TimerProc		(DWORD dwTime);					/* 時間処理 */
	virtual void ProcAtack		(void);							/* 処理(攻撃した時) */
	virtual BOOL ProcHit		(CInfoCharSvr *pInfoChar);		/* 処理(攻撃を受けた時) */
	virtual BOOL ProcSWOON		(DWORD dwPara);					/* 行動処理(気絶) */


protected:
	virtual BOOL TimerProcSTAND	(DWORD dwTime);				/* 時間処理(立ち) */
	virtual BOOL TimerProcMOVE	(DWORD dwTime);				/* 時間処理(移動中) */
	virtual BOOL TimerProcBATTLE(DWORD dwTime);				/* 時間処理(戦闘中) */
	virtual BOOL IsMoveDirection(int nDirection);			/* 指定方向に進めるかチェック */


public:
	DWORD			m_dwLastTiemAtack;		/* 最後に攻撃した時間 */
	DWORD			m_dwLastTimeBattleMove;	/* 戦闘モード中に移動した時間 */
	CInfoCharSvr	*m_pInfoCharTarget;		/* ターゲット中のキャラ情報 */
} CInfoCharBATTLE1Svr, *PCInfoCharBATTLE1Svr;

/* Copyright(C)URARA-works 2008 */
