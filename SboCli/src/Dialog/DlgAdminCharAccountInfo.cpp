/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharAccountInfo.cpp								 */
/* ���e			:�A�J�E���g���_�C�A���O�N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/06													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoAccount.h"
#include "UraraSockTCPSBO.h"
#include "Packet.h"
#include "LayoutHelper.h"
#include "InfoCharCli.h"
#include "LibInfoCharCli.h"
#include "MgrData.h"
#include "DlgAdminCharAccountInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminCharAccountInfo::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharAccountInfo)
	DDX_Text(pDX, IDC_CHARNAME, m_strCharName);
	DDX_Text(pDX, IDC_ACCOUNTID, m_strAccountID);
	DDX_Text(pDX, IDC_ACCOUNT, m_strAccount);
	DDX_Text(pDX, IDC_PASSWORD, m_strPassword);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharAccountInfo, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharAccountInfo)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminCharAccountInfo::CDlgAdminCharAccountInfo				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/06/06														 */
/* ========================================================================= */

CDlgAdminCharAccountInfo::CDlgAdminCharAccountInfo(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminCharAccountInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharAccountInfo)
	m_strCharName = _T("");
	m_strAccountID = _T("");
	m_strAccount = _T("");
	m_strPassword = _T("");
	//}}AFX_DATA_INIT
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharAccountInfo::~CDlgAdminCharAccountInfo			 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/06/06														 */
/* ========================================================================= */

CDlgAdminCharAccountInfo::~CDlgAdminCharAccountInfo()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharAccountInfo::Init									 */
/* ���e		:������															 */
/* ���t		:2008/06/06														 */
/* ========================================================================= */

void CDlgAdminCharAccountInfo::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	/* �E�B���h�E�쐬 */
	Create (CDlgAdminCharAccountInfo::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharAccountInfo::Renew								 */
/* ���e		:�X�V															 */
/* ���t		:2008/06/06														 */
/* ========================================================================= */

void CDlgAdminCharAccountInfo::Renew(void)
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharAccountInfo::OnAdminMsg							 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2008/06/06														 */
/* ========================================================================= */

void CDlgAdminCharAccountInfo::OnAdminMsg(int nType, DWORD dwPara)
{
	PCLibInfoCharCli pLibInfoChar;

	pLibInfoChar = m_pMgrData->GetLibInfoChar ();

	switch (nType) {
	case ADMINMSG_NOTIFYTYPE_LBUTTONDOWN:	/* ���N���b�N�ʒm */
		switch (m_pMgrData->GetAdminNotifyTypeL ()) {
		case ADMINNOTIFYTYPE_CHARID:		/* �L����ID */
			{
				PCInfoCharCli pInfoChar;
				CPacketADMIN_CHAR_REQ_ACCOUNT Packet;

				pInfoChar = (PCInfoCharCli)pLibInfoChar->GetPtr (dwPara);
				m_dwAccountID = 0;
				m_strCharName.	Empty ();
				m_strAccount.	Empty ();
				m_strPassword.	Empty ();
				m_strAccountID.	Empty ();
				if (pInfoChar) {
					m_dwAccountID = pInfoChar->m_dwAccountID;
					m_strCharName = pInfoChar->m_strCharName;
					m_strAccountID.Format ("%d", m_dwAccountID);
					Packet.Make (m_dwAccountID);
					m_pSock->Send (&Packet);
				}
				UpdateData (FALSE);
			}
			break;
		}
		break;
	case ADMINMSG_ACCOUNTINFO:		/* �A�J�E���g���擾 */
		{
			PBYTE pData;
			CInfoAccount InfoAccount;

			pData = m_pMgrData->GetPtr (dwPara);
			InfoAccount.SetTmpData (pData);
			m_dwAccountID	= InfoAccount.m_dwAccountID;
			m_strAccount	= InfoAccount.m_strAccount;
			m_strPassword	= InfoAccount.m_strPassword;
			m_strAccountID.Format ("%d", m_dwAccountID);
			m_pMgrData->Delete (dwPara);
			UpdateData (FALSE);
		}
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharAccountInfo::OnInitDialog							 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/06/06														 */
/* ========================================================================= */

BOOL CDlgAdminCharAccountInfo::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog ();

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharAccountInfo::OnSend								 */
/* ���e		:�{�^���n���h��(���M)											 */
/* ���t		:2008/06/06														 */
/* ========================================================================= */

void CDlgAdminCharAccountInfo::OnSend()
{
	int nResult;
	CPacketADMIN_CHAR_RENEW_ACCOUNT Packet;
	CInfoAccount InfoAccount;

	if (m_dwAccountID == 0) {
		return;
	}
	nResult = MessageBox ("�A�J�E���g���𑗐M���܂����H", "�m�F", MB_YESNO | MB_ICONQUESTION);
	if (nResult != IDYES) {
		return;
	}

	UpdateData ();

	InfoAccount.m_dwAccountID	= m_dwAccountID;
	InfoAccount.m_strAccount	= m_strAccount;
	InfoAccount.m_strPassword	= m_strPassword;
	Packet.Make (&InfoAccount);
	m_pSock->Send (&Packet);
}

/* Copyright(C)URARA-works 2008 */
