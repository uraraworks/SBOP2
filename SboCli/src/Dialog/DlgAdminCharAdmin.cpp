/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharAdmin.cpp										 */
/* ���e			:�Ǘ��Ҍ����̐ݒ�_�C�A���O�N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/05													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "PacketADMIN_REQ_ADMINLEVEL.h"
#include "PacketADMIN_RENEWADMINLEVEL.h"
#include "LayoutHelper.h"
#include "LibInfoCharCli.h"
#include "InfoCharCli.h"
#include "MgrData.h"
#include "DlgAdminCharAdmin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminCharAdmin::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharAdmin)
	DDX_Control(pDX, IDC_ADMINLEVEL, m_ctAdminLevel);
	DDX_Text(pDX, IDC_STATIC_CHARNAME, m_strCharName);
	DDX_Text(pDX, IDC_STATIC_ACCOUNTID, m_strAccountID);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharAdmin, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharAdmin)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminCharAdmin::CDlgAdminCharAdmin							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/07/05														 */
/* ========================================================================= */

CDlgAdminCharAdmin::CDlgAdminCharAdmin(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminCharAdmin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharAdmin)
	m_strAccountID = _T("");
	//}}AFX_DATA_INIT

	m_nAdminLevel = 0;
	m_dwAccountID = 0;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharAdmin::~CDlgAdminCharAdmin						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/07/05														 */
/* ========================================================================= */

CDlgAdminCharAdmin::~CDlgAdminCharAdmin()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharAdmin::Init										 */
/* ���e		:������															 */
/* ���t		:2007/07/05														 */
/* ========================================================================= */

void CDlgAdminCharAdmin::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	/* �E�B���h�E�쐬 */
	Create (CDlgAdminCharAdmin::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharAdmin::Renew										 */
/* ���e		:�X�V															 */
/* ���t		:2007/07/05														 */
/* ========================================================================= */

void CDlgAdminCharAdmin::Renew(void)
{
	PCLibInfoCharCli pLibInfoChar;
	PCInfoCharCli pInfoChar;

	m_strAccountID.	Empty ();
	m_strCharName.	Empty ();
	m_ctAdminLevel.	SetCurSel (0);

	pLibInfoChar	= m_pMgrData->GetLibInfoChar ();
	pInfoChar		= (PCInfoCharCli)pLibInfoChar->GetPtrAccountID (m_dwAccountID);
	if (pInfoChar == NULL) {
		goto Exit;
	}

	m_strAccountID.Format ("%u", m_dwAccountID);
	m_strCharName = pInfoChar->m_strCharName;
	m_ctAdminLevel.SetCurSel (m_nAdminLevel);

Exit:
	UpdateData (FALSE);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharAdmin::OnAdminMsg									 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2007/03/18														 */
/* ========================================================================= */

void CDlgAdminCharAdmin::OnAdminMsg(int nType, DWORD dwPara)
{
	CPacketADMIN_REQ_ADMINLEVEL Packet;

	switch (nType) {
	case ADMINMSG_NOTIFYTYPE_LBUTTONDOWN:		/* ���N���b�N�ʒm */
		m_dwAccountID = dwPara;
		if (m_dwAccountID == 0) {
			break;
		}

		/* �I�����ꂽ�A�J�E���g�̊Ǘ��Ҍ������x����v�� */
		Packet.Make (m_dwAccountID);
		m_pSock->Send (&Packet);
		break;
	case ADMINMSG_ADMINLEVEL:					/* �Ǘ��҃��x���ʒm */
		m_nAdminLevel = dwPara;
		break;
	default:
		return;
	}

	Renew ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharAdmin::OnInitDialog								 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2007/07/05														 */
/* ========================================================================= */

BOOL CDlgAdminCharAdmin::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog ();

//	m_pLayoutHelper->RegisterControl (GetDlgItem (IDC_LIST)->m_hWnd, LH_CTRL_WIDTH | LH_CTRL_HEIGHT);
	m_ctAdminLevel.SetCurSel (0);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharAdmin::OnSend										 */
/* ���e		:�{�^���n���h��(���M)											 */
/* ���t		:2007/03/18														 */
/* ========================================================================= */

void CDlgAdminCharAdmin::OnSend()
{
	CPacketADMIN_RENEWADMINLEVEL Packet;

	if (m_dwAccountID == 0) {
		return;
	}

	Packet.Make (m_dwAccountID, m_ctAdminLevel.GetCurSel ());
	m_pSock->Send (&Packet);
}

/* Copyright(C)URARA-works 2007 */
