/// @file SboCli.h
/// @brief アプリケーションクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/09/24
/// @copyright Copyright(C)URARA-works 2006

#pragma once

#if !defined(__EMSCRIPTEN__)
#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

// クラス宣言

class CSboCliApp : public CWinApp
{
public:
	CSboCliApp();								// コンストラクタ



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
#endif
