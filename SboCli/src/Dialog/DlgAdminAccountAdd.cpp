/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminAccountAdd.cpp										 */
/* ���e			:�A�J�E���g�ǉ��_�C�A���O�N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/01/17													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "Packet.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminAccountAdd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminAccountAdd::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminAccountAdd)
	DDX_Text(pDX, IDC_ACCOUNT, m_strAccount);
	DDX_Text(pDX, IDC_PASSWORD, m_strPassword);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminAccountAdd, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminAccountAdd)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminAccountAdd::CDlgAdminAccountAdd						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2009/01/17														 */
/* ========================================================================= */

CDlgAdminAccountAdd::CDlgAdminAccountAdd(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminAccountAdd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminAccountAdd)
	m_strAccount = _T("");
	m_strPassword = _T("");
	//}}AFX_DATA_INIT
}


/* ========================================================================= */
/* �֐���	:CDlgAdminAccountAdd::~CDlgAdminAccountAdd						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2009/01/17														 */
/* ========================================================================= */

CDlgAdminAccountAdd::~CDlgAdminAccountAdd()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminAccountAdd::Init										 */
/* ���e		:������															 */
/* ���t		:2009/01/17														 */
/* ========================================================================= */

void CDlgAdminAccountAdd::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	/* �E�B���h�E�쐬 */
	Create (CDlgAdminAccountAdd::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminAccountAdd::OnSend									 */
/* ���e		:�{�^���n���h��(���M)											 */
/* ���t		:2009/01/17														 */
/* ========================================================================= */

void CDlgAdminAccountAdd::OnSend()
{
	int nResult;
	CPacketADMIN_ACCOUNT_REQ_ADD Packet;

	nResult = MessageBox ("�A�J�E���g���𑗐M���܂����H", "�m�F", MB_YESNO | MB_ICONQUESTION);
	if (nResult != IDYES) {
		return;
	}

	UpdateData ();

	Packet.Make ((LPCSTR)m_strAccount, (LPCSTR)m_strPassword);
	m_pSock->Send (&Packet);
}

/* Copyright(C)URARA-works 2009 */
