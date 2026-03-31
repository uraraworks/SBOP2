/// @file InfoCharSCORECli.h
/// @brief キャラ情報(得点)クライアントクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/09
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "InfoCharCli.h"

// クラス宣言

typedef class CInfoCharSCORECli : public CInfoCharCli
{
public:
			CInfoCharSCORECli();	// コンストラクタ
	virtual ~CInfoCharSCORECli();	// デストラクタ

	void	ChgMoveState	(int nMoveState);	// 移動状態変更
} CInfoCharSCORECli, *PCInfoCharSCORECli;

