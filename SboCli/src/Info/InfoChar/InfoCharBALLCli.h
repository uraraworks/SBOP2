/// @file InfoCharBALLCli.h
/// @brief キャラ情報(ボール)クライアントクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/02
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "InfoCharCli.h"

// クラス宣言

typedef class CInfoCharBALLCli : public CInfoCharCli
{
public:
			CInfoCharBALLCli();	// コンストラクタ
	virtual ~CInfoCharBALLCli();	// デストラクタ

	void	ChgMoveState	(int nMoveState);	// 移動状態変更
} CInfoCharBALLCli, *PCInfoCharBALLCli;

