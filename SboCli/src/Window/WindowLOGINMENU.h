/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:WindowLOGINMENU.h											 */
/* 内容			:ログインメニューウィンドウクラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/05													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CMgrGrpData;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CWindowLOGINMENU : public CWindowBase
{
public:
			CWindowLOGINMENU();								/* コンストラクタ */
	virtual ~CWindowLOGINMENU();							/* デストラクタ */

	void	Create		(CMgrData *pMgrData);						/* 作成 */
	void	Update		(void);										/* 更新 */
	void	Draw		(CImg32 *pDst);								/* 描画 */
	BOOL	TimerProc	(void);										/* 時間処理 */
	void	SetEnable	(int nNo, BOOL bEnable);					/* メニュー項目の入力状態設定 */


private:
	BOOL	OnUp		(void);								/* キーハンドラ(↑) */
	BOOL	OnDown		(void);								/* キーハンドラ(↓) */
	BOOL	OnLeft		(void);								/* キーハンドラ(←) */
	BOOL	OnRight		(void);								/* キーハンドラ(→) */
	BOOL	OnX			(BOOL bDown);						/* キーハンドラ(X) */


private:
	BOOL	m_abEnable[4];
} CWindowLOGINMENU, *PCWindowLOGINMENU;

/* Copyright(C)URARA-works 2006 */
