/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminTalkEventNONE.cpp									 */
/* ���e			:��b�C�x���g�ݒ�(���I��)�_�C�A���O�N���X �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/23													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminTalkEventNONE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminTalkEventNONE::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTalkEventNONE)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminTalkEventNONE, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminTalkEventNONE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventNONE::CDlgAdminTalkEventNONE					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventNONE::CDlgAdminTalkEventNONE(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminTalkEventNONE::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminTalkEventNONE)
	//}}AFX_DATA_INIT
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventNONE::~CDlgAdminTalkEventNONE				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventNONE::~CDlgAdminTalkEventNONE()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventNONE::Init									 */
/* ���e		:������															 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventNONE::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	/* �E�B���h�E�쐬 */
	Create (CDlgAdminTalkEventNONE::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventNONE::OnAdminMsg								 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventNONE::OnAdminMsg(int nType, DWORD dwPara)
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventNONE::OnInitDialog							 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

BOOL CDlgAdminTalkEventNONE::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	return TRUE;
}

/* Copyright(C)URARA-works 2008 */
