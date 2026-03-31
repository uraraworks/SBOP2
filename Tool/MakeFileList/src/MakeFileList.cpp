/// @file MakeFileList.cpp
/// @brief アプリケーションクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/08/26
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "MakeFileList.h"
#include "MakeFileListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CMakeFileListApp, CWinApp)
	//{{AFX_MSG_MAP(CMakeFileListApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

// 唯一の CMakeFileListApp オブジェクト
CMakeFileListApp theApp;


CMakeFileListApp::CMakeFileListApp()
{
}


BOOL CMakeFileListApp::InitInstance()
{
	CMakeFileListDlg dlg;
	m_pMainWnd = &dlg;

	dlg.DoModal();

	return FALSE;
}
