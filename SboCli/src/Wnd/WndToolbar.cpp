/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:WndToolbar.cpp												 */
/* ���e			:�c�[���o�[�R���g���[���N���X �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/09/13													 */
/* ========================================================================= */

#include "stdafx.h"
#include "afxpriv.h"
#include "WndToolbar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/* ========================================================================= */
/* �N���X�ݒ�																 */
/* ========================================================================= */

BEGIN_MESSAGE_MAP(CWndToolbar, CToolBar)
	//{{AFX_MSG_MAP(CWndToolbar)
	ON_MESSAGE(WM_IDLEUPDATECMDUI,OnIdleUpdateCmdUI)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CWndToolbar::CWndToolbar										 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/09/13														 */
/* ========================================================================= */

CWndToolbar::CWndToolbar()
{
}


/* ========================================================================= */
/* �֐���	:CWndToolbar::~CWndToolbar										 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/09/13														 */
/* ========================================================================= */

CWndToolbar::~CWndToolbar()
{
}


/* ========================================================================= */
/* �֐���	:CWndToolbar::OnIdleUpdateCmdUI									 */
/* ���e		:���b�Z�[�W�n���h��(WM_IDLEUPDATECMDUI)							 */
/* ���t		:2008/09/13														 */
/* ========================================================================= */

LRESULT CWndToolbar::OnIdleUpdateCmdUI(WPARAM wParam,LPARAM lParam)
{
	if (IsWindowVisible ()){
		CFrameWnd *pParent = (CFrameWnd *)GetParent ();
		if (pParent) {
			/* ON_UPDATE_COMMAND_UI���g����悤�Ɋ֐��Ăяo�� */
			OnUpdateCmdUI (pParent,(BOOL)wParam);
		}
	}
	return 0;
}

/* Copyright(C)URARA-works 2008 */
