/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminTmp.cpp											 */
/* ���e			:�_�C�A���O�N���X �����t�@�C��								 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/23													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminTmp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminTmp::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTmp)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminTmp, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminTmp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminTmp::CDlgAdminTmp										 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTmp::CDlgAdminTmp(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminTmp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminTmp)
	//}}AFX_DATA_INIT
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTmp::~CDlgAdminTmp									 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTmp::~CDlgAdminTmp()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTmp::Init												 */
/* ���e		:������															 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTmp::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	/* �ȉ��̓��[�h���X�_�C�A���O�p */
	/* �E�B���h�E�쐬 */
	Create (CDlgAdminTmp::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTmp::OnAdminMsg										 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTmp::OnAdminMsg(int nType, DWORD dwPara)
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTmp::OnInitDialog										 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

BOOL CDlgAdminTmp::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTmp::PostNcDestroy									 */
/* ���e		:�I������														 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTmp::PostNcDestroy()
{
	/* ���[�h���X�_�C�A���O���͂��̊֐������� */
	CDialog::PostNcDestroy ();
}

/* Copyright(C)URARA-works 2008 */
