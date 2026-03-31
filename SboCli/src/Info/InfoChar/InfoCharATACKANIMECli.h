/// @file InfoCharATACKANIMECli.h
/// @brief キャラ情報(攻撃受けるとアニメーション)クライアントクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/10
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "InfoCharCli.h"

// クラス宣言

typedef class CInfoCharATACKANIMECli : public CInfoCharCli
{
public:
			CInfoCharATACKANIMECli();	// コンストラクタ
	virtual ~CInfoCharATACKANIMECli();	// デストラクタ

	void	ChgMoveState	(int nMoveState);	// 移動状態変更


protected:
	BOOL RenewAnime	(DWORD dwTime, int nAdd = 1);	// アニメーションの更新
} CInfoCharATACKANIMECli, *PCInfoCharATACKANIMECli;

