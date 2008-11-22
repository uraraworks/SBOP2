/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名：	WindowTEXTMSG.h												 */
/* 内容：		テキストメッセージ表示ウィンドウクラス 定義ファイル			 */
/* 作成：		年がら年中春うらら(URARA-works)								 */
/* 作成開始日：	2008/11/22													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CWindowTEXTMSG : public CWindowBase
{
public:
			CWindowTEXTMSG();						/* コンストラクタ */
	virtual ~CWindowTEXTMSG();						/* デストラクタ */

	void	Create		(CMgrData *pMgrData);				/* 作成 */

	void	Draw		(CImg32 *pDst);						/* 描画 */
	BOOL	TimerProc	(void);								/* 時間処理 */
	void	SetMsg		(LPCSTR pszMsg);					/* メッセージ設定 */


protected:
	BOOL OnX	(BOOL bDown);						/* キーハンドラ(X) */
	BOOL OnZ	(BOOL bDown);						/* キーハンドラ(Z) */


protected:
	BOOL		m_bInputWait;				/* 入力待ち */
	POINT		m_ptDraw;					/* 描画位置 */
	int			m_nType,					/* ウィンドウ種別 */
				m_nProcPos;					/* 処理中の文字位置 */
	DWORD		m_dwLastProc;				/* 前回の処理時間 */
	CmyString	m_strMsg;					/* メッセージ */
	CImg32		*m_pDibText;				/* 文字描画テンポラリ */
} CWindowTEXTMSG, *PCWindowTEXTMSG;

/* Copyright(C)URARA-works 2008 */
