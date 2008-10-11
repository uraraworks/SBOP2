/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:SboLaunch.cpp												 */
/* ���e			:SBO�����`���[�A�v���P�[�V�����N���X �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/01/27													 */
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

/* �B��� CSboLaunchApp �I�u�W�F�N�g */
CSboLaunchApp theApp;


/* ========================================================================= */
/* �֐���	:CSboLaunchApp::CSboLaunchApp									 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/01/27														 */
/* ========================================================================= */

CSboLaunchApp::CSboLaunchApp()
{
}


/* ========================================================================= */
/* �֐���	:CSboLaunchApp::InitInstance									 */
/* ���e		:������															 */
/* ���t		:2008/01/27														 */
/* ========================================================================= */

BOOL CSboLaunchApp::InitInstance()
{
	WSADATA wsaData;

	/* winSock�������� */
	WSAStartup (MAKEWORD (2, 0), &wsaData);

	AfxEnableControlContainer();

	CSboLaunchDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;
}

/* Copyright(C)URARA-works 2008 */
