/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:MakeFileList.h												 */
/* 内容			:アプリケーションクラス 定義ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/08/26													 */
/* ========================================================================= */

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

class CMakeFileListApp : public CWinApp
{
public:
	CMakeFileListApp();

	//{{AFX_VIRTUAL(CMakeFileListApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMakeFileListApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
