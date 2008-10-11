/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:InfoCharMOVEATACKSvr.h										 */
/* 内容			:キャラ情報(移動して攻撃)サーバークラス 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/17													 */
/* ========================================================================= */

#pragma once

#include "InfoCharSvr.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoCharMOVEATACKSvr : public CInfoCharSvr
{
public:
			CInfoCharMOVEATACKSvr();					/* コンストラクタ */
	virtual ~CInfoCharMOVEATACKSvr();					/* デストラクタ */

	void SetMoveState	(int nMoveState);						/* 移動状態を変更 */
	BOOL IsLogoutDelete	(void);									/* ログアウト時に削除するか判定 */
	BOOL TimerProc		(DWORD dwTime);							/* 時間処理 */
	void ProcAtack		(void);									/* 処理(攻撃した時) */


public:
	int		m_nMoveCount;					/* 移動した歩数 */
} CInfoCharMOVEATACKSvr, *PCInfoCharMOVEATACKSvr;

/* Copyright(C)URARA-works 2007 */
