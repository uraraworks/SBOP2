/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:SboCli.cpp													 */
/* ���e			:�A�v���P�[�V�����N���X �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/09/24													 */
/* ========================================================================= */

#include "stdafx.h"
#include "MainFrame.h"
#include "SboCli.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

BEGIN_MESSAGE_MAP(CSboCliApp, CWinApp)
	//{{AFX_MSG_MAP(CSboCliApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/* �B��� CSboCliApp �I�u�W�F�N�g */
CSboCliApp theApp;


/* ========================================================================= */
/* �֐���	:CSboCliApp::CSboCliApp											 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

CSboCliApp::CSboCliApp()
{
}


/* ========================================================================= */
/* �֐���	:CSboCliApp::InitInstance										 */
/* ���e		:����������														 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

BOOL CSboCliApp::InitInstance()
{
	BOOL bRet;
	WSADATA wsaData;
	CMainFrame MainFrame;
	HANDLE hMutexMain;

	bRet = FALSE;
	hMutexMain = NULL;

	/* winSock�������� */
	WSAStartup (MAKEWORD (2, 0), &wsaData);

	CWinApp::InitInstance();
	AfxEnableControlContainer();

#if 1
	hMutexMain = CreateMutex (NULL, TRUE, "SboCli");
	if ((hMutexMain == NULL) || GetLastError () == ERROR_ALREADY_EXISTS) {
		goto Exit;
	}
#endif

	MainFrame.MainLoop (GetModuleHandle (NULL));

	bRet = TRUE;
Exit:
	ReleaseMutex (hMutexMain);
	CloseHandle (hMutexMain);

	return FALSE;
}

/* Copyright(C)URARA-works 2006 */
