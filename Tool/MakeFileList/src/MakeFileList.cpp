/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:MakeFileList.cpp											 */
/* ���e			:�A�v���P�[�V�����N���X �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/08/26													 */
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

/* �B��� CMakeFileListApp �I�u�W�F�N�g */
CMakeFileListApp theApp;


/* ========================================================================= */
/* �֐���	:CMakeFileListApp::CMakeFileListApp								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/08/26														 */
/* ========================================================================= */

CMakeFileListApp::CMakeFileListApp()
{
}


/* ========================================================================= */
/* �֐���	:CMakeFileListApp::InitInstance									 */
/* ���e		:����������														 */
/* ���t		:2008/08/26														 */
/* ========================================================================= */

BOOL CMakeFileListApp::InitInstance()
{
	CMakeFileListDlg dlg;
	m_pMainWnd = &dlg;

	dlg.DoModal();

	return FALSE;
}

/* Copyright(C)URARA-works 2007 */
