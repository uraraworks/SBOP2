/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminTalkEventPAGE.cpp									 */
/* ���e			:��b�C�x���g�ݒ�(�y�[�W�؂�ւ�)�_�C�A���O�N���X �����t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/23													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminTalkEventPAGE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminTalkEventPAGE::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTalkEventPAGE)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminTalkEventPAGE, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminTalkEventPAGE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventPAGE::CDlgAdminTalkEventPAGE					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventPAGE::CDlgAdminTalkEventPAGE(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminTalkEventPAGE::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminTalkEventPAGE)
	//}}AFX_DATA_INIT
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventPAGE::~CDlgAdminTalkEventPAGE				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventPAGE::~CDlgAdminTalkEventPAGE()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventPAGE::Init									 */
/* ���e		:������															 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventPAGE::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	/* �E�B���h�E�쐬 */
	Create (CDlgAdminTalkEventPAGE::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventPAGE::OnAdminMsg								 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventPAGE::OnAdminMsg(int nType, DWORD dwPara)
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventPAGE::OnInitDialog							 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

BOOL CDlgAdminTalkEventPAGE::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	return TRUE;
}

/* Copyright(C)URARA-works 2008 */
