/// @file StateProcLOGIN.h
/// @brief 状態処理クラス(ログイン画面) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/03
/// @copyright Copyright(C)URARA-works 2006

#pragma once

#include "StateProcBase.h"

// クラス宣言

typedef class CStateProcLOGIN : public CStateProcBase
{
public:
			CStateProcLOGIN();  // コンストラクタ
	virtual ~CStateProcLOGIN(); // デストラクタ

	void Init(void);                    // 初期化
	BOOL TimerProc(void);               // 時間処理
	void OnLButtonDown(int x, int y);   // メッセージハンドラ(WM_LBUTTONDOWN)


protected:
	BOOL OnX(BOOL bDown);                     // キーハンドラ(X)
	void OnMgrDrawSTART_FADEIN(DWORD dwPara); // フェードイン開始


protected:
	BOOL m_bFadeIn; // フェードイン完了？
} CStateProcLOGIN, *PCStateProcLOGIN;
