/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:SboLaunch.cpp												 */
/* 内容			:SBOランチャーアプリケーションクラス 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/01/27													 */
/* ========================================================================= */

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

/* 唯一の CSboLaunchApp オブジェクト */
CSboLaunchApp theApp;


/* ========================================================================= */
/* 関数名	:CSboLaunchApp::CSboLaunchApp									 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/01/27														 */
/* ========================================================================= */

CSboLaunchApp::CSboLaunchApp()
{
}


/* ========================================================================= */
/* 関数名	:CSboLaunchApp::InitInstance									 */
/* 内容		:初期化															 */
/* 日付		:2008/01/27														 */
/* ========================================================================= */

BOOL CSboLaunchApp::InitInstance()
{
	WSADATA wsaData;

	/* winSockを初期化 */
	WSAStartup (MAKEWORD (2, 0), &wsaData);

	AfxEnableControlContainer();

	CSboLaunchDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;
}

/* Copyright(C)URARA-works 2008 */
