/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:SboCli.h													 */
/* 内容			:アプリケーションクラス 定義ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/09/24													 */
/* ========================================================================= */

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

class CSboCliApp : public CWinApp
{
public:
	CSboCliApp();								/* コンストラクタ */



public:
	//{{AFX_VIRTUAL(CSboCliApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSboCliApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2006 */
