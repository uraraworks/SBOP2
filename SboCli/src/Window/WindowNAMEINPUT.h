/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:WindowNAMEINPUT.h											 */
/* 内容			:名前入力ウィンドウクラス 定義ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/04/10													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CMgrWindow;
class CMgrDraw;
class CWindowCHARNAME;
class CInfoCharCli;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CWindowNAMEINPUT : public CWindowBase
{
public:
			CWindowNAMEINPUT();								/* コンストラクタ */
	virtual ~CWindowNAMEINPUT();							/* デストラクタ */

	void	Create		(CMgrData *pMgrData);						/* 作成 */
	void	OnWindowMsg	(int nType, DWORD dwPara);					/* メッセージハンドラ(WM_WINDOWMSG) */
	void	Draw		(CImg32 *pDst);								/* 描画 */
	BOOL	TimerProc	(void);										/* 時間処理 */


private:
	BOOL	OnUp		(void);								/* キーハンドラ(↑) */
	BOOL	OnDown		(void);								/* キーハンドラ(↓) */
	BOOL	OnLeft		(void);								/* キーハンドラ(←) */
	BOOL	OnRight		(void);								/* キーハンドラ(→) */
	BOOL	OnX			(BOOL bDown);						/* キーハンドラ(X) */
	BOOL	OnZ			(BOOL bDown);						/* キーハンドラ(Y) */


public:
	CMgrWindow			*m_pMgrWindow;				/* ウィンドウマネージャ */
	CMgrDraw			*m_pMgrDraw;				/* 描画マネージャ */
	CWindowCHARNAME		*m_pWindowCHARNAME;			/* キャラ名入力ウィンドウ */
	CInfoCharCli		*m_pInfoCharCli;			/* 作成中のキャラ情報 */
} CWindowNAMEINPUT, *PCWindowNAMEINPUT;

/* Copyright(C)URARA-works 2007 */
