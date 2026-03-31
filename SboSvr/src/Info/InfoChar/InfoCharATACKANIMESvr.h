/// @file InfoCharATACKANIMESvr.h
/// @brief キャラ情報(攻撃受けるとアニメーション)サーバークラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/17
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "InfoCharSvr.h"

// クラス宣言

typedef class CInfoCharATACKANIMESvr : public CInfoCharSvr
{
public:
			CInfoCharATACKANIMESvr();	// コンストラクタ
	virtual ~CInfoCharATACKANIMESvr();	// デストラクタ

	BOOL ProcHit(CInfoCharSvr *pInfoChar);	// 処理(攻撃を受けた時)
	BOOL TimerProc(DWORD dwTime);	// 時間処理

public:
} CInfoCharATACKANIMESvr, *PCInfoCharATACKANIMESvr;
