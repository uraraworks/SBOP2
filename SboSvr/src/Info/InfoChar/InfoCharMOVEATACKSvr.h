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

	void  SetMoveState	(int nMoveState);						/* 移動状態を変更 */
	BOOL  TimerProc		(DWORD dwTime);							/* 時間処理 */
	void  ProcAtack		(void);									/* 処理(攻撃した時) */
	DWORD GetHitEffectID(void);									/* ヒット時に相手に表示するエフェクトIDを取得 */
	DWORD GetDamage		(void);									/* ダメージ値を取得 */


public:
	BOOL	m_bHitQuit,						/* ヒットすると消滅 */
			m_bDistanceDelete;				/* 射程距離まで行くと消える */
	DWORD	m_dwLastAtackTime,				/* 最後に攻撃した時間 */
			m_dwQuitTime,					/* 終了時間 */
			m_dwHitEffectID,				/* ヒット時に相手に表示するエフェクトID */
			m_dwValue1,						/* 効果1 */
			m_dwValue2;						/* 効果2 */
	int		m_nMoveCount;					/* 移動した歩数 */
} CInfoCharMOVEATACKSvr, *PCInfoCharMOVEATACKSvr;

/* Copyright(C)URARA-works 2007 */
