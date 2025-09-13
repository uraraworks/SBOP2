/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:MakeFileList.cpp											 */
/* 内容			:アプリケーションクラス 実装ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/08/26													 */
/* ========================================================================= */

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

/* 唯一の CMakeFileListApp オブジェクト */
CMakeFileListApp theApp;


/* ========================================================================= */
/* 関数名	:CMakeFileListApp::CMakeFileListApp								 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/08/26														 */
/* ========================================================================= */

CMakeFileListApp::CMakeFileListApp()
{
}


/* ========================================================================= */
/* 関数名	:CMakeFileListApp::InitInstance									 */
/* 内容		:初期化処理														 */
/* 日付		:2008/08/26														 */
/* ========================================================================= */

BOOL CMakeFileListApp::InitInstance()
{
	CMakeFileListDlg dlg;
	m_pMainWnd = &dlg;

	dlg.DoModal();

	return FALSE;
}

/* Copyright(C)URARA-works 2007 */
