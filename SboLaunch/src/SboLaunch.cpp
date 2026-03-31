/// @file SboLaunch.cpp
/// @brief SBOランチャーアプリケーションクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/01/27
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "SboLaunch.h"
#include "SboLaunchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CSboLaunchApp, CWinApp)
	//{{AFX_MSG_MAP(CSboLaunchApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

// 唯一の CSboLaunchApp オブジェクト
CSboLaunchApp theApp;


CSboLaunchApp::CSboLaunchApp()
{
}


BOOL CSboLaunchApp::InitInstance()
{
	WSADATA wsaData;

	// winSockを初期化
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	AfxEnableControlContainer();

	CSboLaunchDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;
}
