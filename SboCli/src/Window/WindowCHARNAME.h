/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:WindowCHARNAME.h											 */
/* 内容			:キャラ名入力ウィンドウクラス 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/08													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CMgrGrpData;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CWindowCHARNAME : public CWindowBase
{
public:
			CWindowCHARNAME();								/* コンストラクタ */
	virtual ~CWindowCHARNAME();								/* デストラクタ */

	void	Create		(CMgrData *pMgrData);						/* 作成 */
	void	Draw		(CImg32 *pDst);								/* 描画 */
	void	SetActive	(BOOL bActive);								/* アクティブか設定 */


private:
	void	MakeWindow	(void);										/* ウィンドウ作成 */
	static LRESULT CALLBACK CharNameWndProc		(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	/* キャラ名入力欄プロシージャ */


public:
	CmyString	m_strName;						/* 入力されたキャラ名 */


private:
	HWND	m_hWndCharName;							/* キャラ名入力欄 */

	WNDPROC	m_OrgWndProcCharName;					/* キャラ名入力欄の元ウィンドウプロシージャ */
} CWindowCHARNAME, *PCWindowCHARNAME;

/* Copyright(C)URARA-works 2006 */
