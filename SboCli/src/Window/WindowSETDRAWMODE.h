/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:WindowSETDRAWMODE.h										 */
/* 内容			:表示内容の設定ウィンドウクラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/24													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CWindowSETDRAWMODE : public CWindowBase
{
public:
			CWindowSETDRAWMODE();							/* コンストラクタ */
	virtual ~CWindowSETDRAWMODE();							/* デストラクタ */

	void	Create		(CMgrData *pMgrData);						/* 作成 */
	void	Draw		(CImg32 *pDst);								/* 描画 */


private:
	BOOL	OnUp		(void);								/* キーハンドラ(↑) */
	BOOL	OnDown		(void);								/* キーハンドラ(↓) */
	BOOL	OnLeft		(void);								/* キーハンドラ(←) */
	BOOL	OnRight		(void);								/* キーハンドラ(→) */
	BOOL	OnX			(BOOL bDown);						/* キーハンドラ(X) */
	BOOL	OnZ			(BOOL bDown);						/* キーハンドラ(Z) */
} CWindowSETDRAWMODE, *PCWindowSETDRAWMODE;

/* Copyright(C)URARA-works 2007 */
