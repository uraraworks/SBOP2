/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:InfoCharBALLCli.h											 */
/* 内容			:キャラ情報(ボール)クライアントクラス 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/02													 */
/* ========================================================================= */

#pragma once

#include "InfoCharCli.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoCharBALLCli : public CInfoCharCli
{
public:
			CInfoCharBALLCli();							/* コンストラクタ */
	virtual ~CInfoCharBALLCli();						/* デストラクタ */

	void		ChgMoveState	(int nMoveState);				/* 移動状態変更 */
} CInfoCharBALLCli, *PCInfoCharBALLCli;

/* Copyright(C)URARA-works 2007 */
