/// @file SboLaunch.h
/// @brief SBOランチャーアプリケーションクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/01/27
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

// クラス宣言

class CSboLaunchApp : public CWinApp
{
public:
	CSboLaunchApp();			// コンストラクタ



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
