/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:SboLaunch.h												 */
/* 内容			:SBOランチャーアプリケーションクラス 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/01/27													 */
/* ========================================================================= */

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

class CSboLaunchApp : public CWinApp
{
public:
	CSboLaunchApp();			/* コンストラクタ */



public:
	//{{AFX_VIRTUAL(CSboLaunchApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSboLaunchApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
/* Copyright(C)URARA-works 2008 */
