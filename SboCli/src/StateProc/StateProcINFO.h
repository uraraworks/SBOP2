/// @file StateProcINFO.h
/// @brief 状態処理クラス(お知らせ) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/08/16
/// @copyright Copyright(C)URARA-works 2005

#pragma once

#include "StateProcBase.h"

// クラス宣言

typedef class CStateProcINFO : public CStateProcBase
{
public:
			CStateProcINFO();  // コンストラクタ
	virtual ~CStateProcINFO(); // デストラクタ

	void Init(void);                    // 初期化
	void OnLButtonDown(int x, int y);   // メッセージハンドラ(WM_LBUTTONDOWN)


protected:
	BOOL OnX(BOOL bDown); // キーハンドラ(X)
} CStateProcINFO, *PCStateProcINFO;
