/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:WindowOPTION_VIEWSET.h										 */
/* 内容			:オプション-表示設定ウィンドウクラス 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/27													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CWindowOPTION_VIEWSET : public CWindowBase
{
public:
			CWindowOPTION_VIEWSET();						/* コンストラクタ */
	virtual ~CWindowOPTION_VIEWSET();						/* デストラクタ */

	void	Create		(CMgrData *pMgrData);						/* 作成 */
	void	Draw		(CImg32 *pDst);								/* 描画 */
	BOOL	TimerProc	(void);										/* 時間処理 */


protected:
	BOOL	OnUp		(void);								/* キーハンドラ(↑) */
	BOOL	OnDown		(void);								/* キーハンドラ(↓) */
	BOOL	OnLeft		(void);								/* キーハンドラ(←) */
	BOOL	OnRight		(void);								/* キーハンドラ(→) */
	BOOL	OnX			(BOOL bDown);						/* キーハンドラ(X) */
	BOOL	OnZ			(BOOL bDown);						/* キーハンドラ(Z) */
	BOOL	GetCheck	(int nNo);							/* チェック状態を取得 */


protected:
	ARRAYINT	m_anCheck;			/* チェックボックス描画用 */
	ARRAYDWORD	m_adwCheckTime;		/* チェックボックス描画用 */
} CWindowOPTION_VIEWSET, *PCWindowOPTION_VIEWSET;

/* Copyright(C)URARA-works 2008 */
