/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:WindowEYECOLOR.h											 */
/* 内容			:目色選択ウィンドウクラス 定義ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/04/08													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CMgrGrpData;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CWindowEYECOLOR : public CWindowBase
{
public:
			CWindowEYECOLOR();								/* コンストラクタ */
	virtual ~CWindowEYECOLOR();								/* デストラクタ */

	void	Create		(CMgrData *pMgrData);						/* 作成 */
	void	Draw		(CImg32 *pDst);								/* 描画 */
	BOOL	TimerProc	(void);										/* 時間処理 */
	void	SetType		(WORD wEyeColor);							/* 目色を設定 */
	WORD	GetType		(void);										/* 目色を取得 */


private:
	BOOL	OnUp		(void);								/* キーハンドラ(↑) */
	BOOL	OnDown		(void);								/* キーハンドラ(↓) */
	BOOL	OnLeft		(void);								/* キーハンドラ(←) */
	BOOL	OnRight		(void);								/* キーハンドラ(→) */
	BOOL	OnX			(BOOL bDown);						/* キーハンドラ(X) */
	BOOL	OnZ			(BOOL bDown);						/* キーハンドラ(Y) */


private:
	int		m_nPosX;								/* 桁の選択位置 */
} CWindowEYECOLOR, *PCWindowEYECOLOR;

/* Copyright(C)URARA-works 2007 */
