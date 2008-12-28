/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminTalkEventMSG.cpp									 */
/* ���e			:��b�C�x���g�ݒ�(���b�Z�[�W�\��)�_�C�A���O�N���X �����t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/23													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoTalkEventBase.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminTalkEventMSG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminTalkEventMSG::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminTalkEventNONE::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTalkEventMSG)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_MSG, m_strMsg);
}

BEGIN_MESSAGE_MAP(CDlgAdminTalkEventMSG, CDlgAdminTalkEventNONE)
	//{{AFX_MSG_MAP(CDlgAdminTalkEventMSG)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventMSG::CDlgAdminTalkEventMSG					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventMSG::CDlgAdminTalkEventMSG(CWnd* pParent /*=NULL*/)
	: CDlgAdminTalkEventNONE(pParent)
	, m_strMsg(_T(""))
{
	//{{AFX_DATA_INIT(CDlgAdminTalkEventMSG)
	//}}AFX_DATA_INIT

	m_nResourceID = CDlgAdminTalkEventMSG::IDD;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventMSG::~CDlgAdminTalkEventMSG					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventMSG::~CDlgAdminTalkEventMSG()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventMSG::Set										 */
/* ���e		:�ݒ肩���ʂɔ��f												 */
/* ���t		:2008/12/26														 */
/* ========================================================================= */

void CDlgAdminTalkEventMSG::Set(CInfoTalkEventBase *pSrc)
{
	m_strMsg = pSrc->m_strText;

	UpdateData (FALSE);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventMSG::Get										 */
/* ���e		:��ʂ���ݒ�ɔ��f												 */
/* ���t		:2008/12/26														 */
/* ========================================================================= */

void CDlgAdminTalkEventMSG::Get(CInfoTalkEventBase *pDst)
{
	UpdateData ();

	pDst->m_strText = m_strMsg;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventMSG::OnInitDialog							 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

BOOL CDlgAdminTalkEventMSG::OnInitDialog()
{
	CDlgAdminTalkEventNONE::OnInitDialog();

	RegisterControl (IDC_MSG, LH_CTRL_WIDTH | LH_CTRL_HEIGHT);

	return TRUE;
}

/* Copyright(C)URARA-works 2008 */
