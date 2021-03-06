/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:WindowSYSTEMMENU.h											 */
/* 内容			:システムメニューウィンドウクラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/06/19													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CWindowSYSTEMMENU : public CWindowBase
{
public:
			CWindowSYSTEMMENU();							/* コンストラクタ */
	virtual ~CWindowSYSTEMMENU();							/* デストラクタ */

	void	Create		(CMgrData *pMgrData);						/* 作成 */
	void	Draw		(CImg32 *pDst);								/* 描画 */


private:
	BOOL	OnUp		(void);								/* キーハンドラ(↑) */
	BOOL	OnDown		(void);								/* キーハンドラ(↓) */
	BOOL	OnLeft		(void);								/* キーハンドラ(←) */
	BOOL	OnRight		(void);								/* キーハンドラ(→) */
	BOOL	OnX			(BOOL bDown);						/* キーハンドラ(X) */
	BOOL	OnZ			(BOOL bDown);						/* キーハンドラ(Z) */
	BOOL	OnEscape	(BOOL bDown);						/* キーハンドラ(Escape) */
} CWindowSYSTEMMENU, *PCWindowSYSTEMMENU;

/* Copyright(C)URARA-works 2007 */
