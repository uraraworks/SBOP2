/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:InfoCharATACKANIMECli.h									 */
/* 内容			:キャラ情報(攻撃受けるとアニメーション)クライアントクラス 定義ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/10													 */
/* ========================================================================= */

#pragma once

#include "InfoCharCli.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoCharATACKANIMECli : public CInfoCharCli
{
public:
			CInfoCharATACKANIMECli();					/* コンストラクタ */
	virtual ~CInfoCharATACKANIMECli();					/* デストラクタ */

	void	ChgMoveState	(int nMoveState);					/* 移動状態変更 */


protected:
	BOOL RenewAnime			(DWORD dwTime, int nAdd = 1);		/* アニメーションの更新 */
} CInfoCharATACKANIMECli, *PCInfoCharATACKANIMECli;

/* Copyright(C)URARA-works 2007 */
