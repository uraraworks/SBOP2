/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* ファイル名：	StateProcLOGO.h												 */
/* 内容：		状態処理クラス(ロゴ) 定義ファイル							 */
/* 作成：		年がら年中春うらら(URARA-works)								 */
/* 作成開始日：	2005/05/28													 */
/* ========================================================================= */

#pragma once

#include "StateProcBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CStateProcLOGO : public CStateProcBase
{
public:
			CStateProcLOGO();					/* コンストラクタ */
	virtual ~CStateProcLOGO();					/* デストラクタ */

	void Init			(void);							/* 初期化 */
	BOOL TimerProc		(void);							/* 時間処理 */
	void OnLButtonDown	(int x, int y);					/* メッセージハンドラ(WM_LBUTTONDOWN) */


protected:
	BOOL OnX					(BOOL bDown);			/* キーハンドラ(X) */
	BOOL OnZ					(BOOL bDown);			/* キーハンドラ(Z) */
	void OnMgrDrawEND_FADEIN	(DWORD dwPara);			/* フェードイン完了 */
	void OnMgrDrawEND_FADEOUT	(DWORD dwPara);			/* フェードアウト完了 */


protected:
	DWORD	m_dwTimeWait;					/* 待ち時間用 */
} CStateProcLOGO, *PCStateProcLOGO;

/* Copyright(C)URARA-works 2005 */
