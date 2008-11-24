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
	void	SetTitle	(LPCSTR pszTitle);					/* 肩書きを設定 */
	void	SetName		(LPCSTR pszName);					/* 名前を設定 */
	void	SetMsg		(LPCSTR pszMsg);					/* メッセージ設定 */


protected:
	BOOL OnX		(BOOL bDown);					/* キーハンドラ(X) */
	BOOL OnZ		(BOOL bDown);					/* キーハンドラ(Z) */
	void DrawChar	(LPCSTR pszText);				/* １文字表示 */
	void RenewTitle	(void);							/* 肩書と名前画像を更新 */


protected:
	BOOL		m_bInputWait;				/* 入力待ち */
	POINT		m_ptDraw;					/* 描画位置 */
	int			m_nType,					/* ウィンドウ種別 */
				m_nProcPos,					/* 処理中の文字位置 */
				m_nSpaceHeight;				/* ウィンドウ上の余白の高さ */
	DWORD		m_dwLastProc;				/* 前回の処理時間 */
	CmyString	m_strTitle,					/* 肩書き */
				m_strName,					/* 名前 */
				m_strMsg;					/* メッセージ */
	CImg32		*m_pDibTitle,				/* 肩書きと名前部分 */
				*m_pDibText;				/* 文字描画テンポラリ */
} CWindowTEXTMSG, *PCWindowTEXTMSG;

/* Copyright(C)URARA-works 2008 */
