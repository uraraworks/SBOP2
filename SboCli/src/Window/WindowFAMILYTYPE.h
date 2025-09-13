/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:WindowFAMILYTYPE.h											 */
/* 内容			:種族選択ウィンドウクラス 定義ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/04/07													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CMgrGrpData;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CWindowFAMILYTYPE : public CWindowBase
{
public:
			CWindowFAMILYTYPE();							/* コンストラクタ */
	virtual ~CWindowFAMILYTYPE();							/* デストラクタ */

	void	Create		(CMgrData *pMgrData);						/* 作成 */
	void	Draw		(CImg32 *pDst);								/* 描画 */
	BOOL	TimerProc	(void);										/* 時間処理 */
	WORD	GetType		(void);										/* 種族を取得 */


private:
	BOOL	OnUp		(void);								/* キーハンドラ(↑) */
	BOOL	OnDown		(void);								/* キーハンドラ(↓) */
	BOOL	OnLeft		(void);								/* キーハンドラ(←) */
	BOOL	OnRight		(void);								/* キーハンドラ(→) */
	BOOL	OnX			(BOOL bDown);						/* キーハンドラ(X) */
	BOOL	OnZ			(BOOL bDown);						/* キーハンドラ(Y) */


private:
	int		m_nPos;									/* 選択位置 */
} CWindowFAMILYTYPE, *PCWindowFAMILYTYPE;

/* Copyright(C)URARA-works 2007 */
