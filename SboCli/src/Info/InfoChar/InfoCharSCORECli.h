/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:InfoCharSCORECli.h											 */
/* 内容			:キャラ情報(得点)クライアントクラス 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/09													 */
/* ========================================================================= */

#pragma once

#include "InfoCharCli.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoCharSCORECli : public CInfoCharCli
{
public:
			CInfoCharSCORECli();						/* コンストラクタ */
	virtual ~CInfoCharSCORECli();						/* デストラクタ */

	void	ChgMoveState	(int nMoveState);					/* 移動状態変更 */
} CInfoCharSCORECli, *PCInfoCharSCORECli;

/* Copyright(C)URARA-works 2007 */
