/// @file StateProcLOGO.h
/// @brief 状態処理クラス(ロゴ) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/05/28
/// @copyright Copyright(C)URARA-works 2005

#pragma once

#include "StateProcBase.h"

// クラス宣言

typedef class CStateProcLOGO : public CStateProcBase
{
public:
			CStateProcLOGO();  // コンストラクタ
	virtual ~CStateProcLOGO(); // デストラクタ

	void Init(void);                    // 初期化
	BOOL TimerProc(void);               // 時間処理
	void OnLButtonDown(int x, int y);   // メッセージハンドラ(WM_LBUTTONDOWN)


protected:
	BOOL OnX(BOOL bDown);                      // キーハンドラ(X)
	BOOL OnZ(BOOL bDown);                      // キーハンドラ(Z)
	void OnMgrDrawEND_FADEIN(DWORD dwPara);    // フェードイン完了
	void OnMgrDrawEND_FADEOUT(DWORD dwPara);   // フェードアウト完了


protected:
	DWORD m_dwTimeWait; // 待ち時間用
} CStateProcLOGO, *PCStateProcLOGO;
