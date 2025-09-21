/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:StateProcLOGIN.h											 */
/* 内容			:状態処理クラス(ログイン画面) 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/03													 */
/* ========================================================================= */

#pragma once

#include "StateProcBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CStateProcLOGIN : public CStateProcBase
{
public:
			CStateProcLOGIN();							/* コンストラクタ */
	virtual ~CStateProcLOGIN();							/* デストラクタ */

	void Init			(void);									/* 初期化 */
	BOOL TimerProc		(void);									/* 時間処理 */
	void OnLButtonDown	(int x, int y);							/* メッセージハンドラ(WM_LBUTTONDOWN) */


protected:
	BOOL OnX					(BOOL bDown);					/* キーハンドラ(X) */
	void OnMgrDrawSTART_FADEIN	(DWORD dwPara);					/* フェードイン開始 */


protected:
	BOOL	m_bFadeIn;					/* フェードイン完了？ */
} CStateProcLOGIN, *PCStateProcLOGIN;

/* Copyright(C)URARA-works 2006 */
