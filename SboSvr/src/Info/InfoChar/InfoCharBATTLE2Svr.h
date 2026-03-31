/// @file InfoCharBATTLE2Svr.h
/// @brief キャラ情報(戦闘2)サーバークラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2009/07/17
/// @copyright Copyright(C)URARA-works 2009

#pragma once

#include "InfoCharBATTLE1Svr.h"

// クラス宣言

typedef class CInfoCharBATTLE2Svr : public CInfoCharBATTLE1Svr
{
public:
			CInfoCharBATTLE2Svr();	// コンストラクタ
	virtual ~CInfoCharBATTLE2Svr();	// デストラクタ

protected:
	virtual BOOL TimerProcSTAND(DWORD dwTime);	// 時間処理(立ち)

public:
} CInfoCharBATTLE2Svr, *PCInfoCharBATTLE2Svr;
