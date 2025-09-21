/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:InfoCharMOVEATACKCli.h										 */
/* 内容			:キャラ情報(移動して攻撃)クライアントクラス 定義ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/17													 */
/* ========================================================================= */

#pragma once

#include "InfoCharCli.h"

class CInfoMotion;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoCharMOVEATACKCli : public CInfoCharCli
{
public:
			CInfoCharMOVEATACKCli();					/* コンストラクタ */
	virtual ~CInfoCharMOVEATACKCli();					/* デストラクタ */

	void	ChgMoveState	(int nMoveState);					/* 移動状態変更 */
	void	SetViewState	(int nViewState);					/* 表示状態設定 */

	CInfoMotion *GetMotionInfo	(int *pnCount = NULL);			/* モーション情報を取得 */
} CInfoCharMOVEATACKCli, *PCInfoCharMOVEATACKCli;

/* Copyright(C)URARA-works 2007 */
