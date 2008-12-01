/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharList.cpp										 */
/* ���e			:�L�����ꗗ�_�C�A���O�N���X �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/01													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "PacketADMIN_PARA2.h"
#include "InfoCharCli.h"
#include "LibInfoCharCli.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminCharList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminCharList::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharList)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharList, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharList)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RENEW, &CDlgAdminCharList::OnBnClickedRenew)
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminCharList::CDlgAdminCharList							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/01														 */
/* ========================================================================= */

CDlgAdminCharList::CDlgAdminCharList(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminCharList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharList)
	//}}AFX_DATA_INIT

	m_pWndNotify	= NULL;
	m_pLibInfoChar	= NULL;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharList::~CDlgAdminCharList							 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/01														 */
/* ========================================================================= */

CDlgAdminCharList::~CDlgAdminCharList()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharList::Init										 */
/* ���e		:������															 */
/* ���t		:2008/12/01														 */
/* ========================================================================= */

void CDlgAdminCharList::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	m_pLibInfoChar = m_pMgrData->GetLibInfoCharOnline ();

	/* �E�B���h�E�쐬 */
	Create (CDlgAdminCharList::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharList::Renew										 */
/* ���e		:�ꗗ���X�V														 */
/* ���t		:2008/12/01														 */
/* ========================================================================= */

void CDlgAdminCharList::Renew(void)
{
	int i, nCount;
	PCInfoCharCli pInfo;
	CString strTmp;

	m_pLibInfoChar = m_pMgrData->GetLibInfoCharOnline ();

	m_List.DeleteAllItems ();

	nCount = m_pLibInfoChar->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoCharCli)m_pLibInfoChar->GetPtr (i);
		strTmp.Format ("%d", pInfo->m_dwCharID);					/* �L����ID */
		m_List.InsertItem (i, strTmp);
		m_List.SetItemData (i, pInfo->m_dwCharID);
		m_List.SetItemText (i, 1, (LPCSTR)pInfo->m_strCharName);	/* �L������ */
		strTmp.Format ("%d", pInfo->m_dwMapID);						/* �}�b�vID */
		m_List.SetItemText (i, 2, strTmp);
		strTmp.Format ("%d", pInfo->m_nMapX);						/* X���W */
		m_List.SetItemText (i, 3, strTmp);
		strTmp.Format ("%d", pInfo->m_nMapY);						/* Y���W */
		m_List.SetItemText (i, 4, strTmp);
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharList::OnAdminMsg									 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2008/12/01														 */
/* ========================================================================= */

void CDlgAdminCharList::OnAdminMsg(int nType, DWORD dwPara)
{
	switch (nType) {
	case ADMINMSG_CHAR_ONLINE:	/* �I�����C�����L�����ꗗ�X�V */
		Renew ();
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharList::OnInitDialog								 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/12/01														 */
/* ========================================================================= */

BOOL CDlgAdminCharList::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	m_List.SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn (1, "�L����ID",		LVCFMT_LEFT, 60);
	m_List.InsertColumn (2, "�L������",		LVCFMT_LEFT, 130);
	m_List.InsertColumn (3, "�}�b�vID",		LVCFMT_LEFT, 60);
	m_List.InsertColumn (4, "X���W",		LVCFMT_LEFT, 50);
	m_List.InsertColumn (5, "Y���W",		LVCFMT_LEFT, 50);

	RegisterControl (IDC_LIST,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);

	OnBnClickedRenew ();
	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharList::OnBnClickedRenew							 */
/* ���e		:�{�^���n���h��(�X�V)											 */
/* ���t		:2008/12/01														 */
/* ========================================================================= */

void CDlgAdminCharList::OnBnClickedRenew()
{
	CPacketADMIN_PARA2 Packet;

	Packet.Make (SBOCOMMANDID_SUB_ADMIN_CHAR_REQ_ONLINE);
	m_pSock->Send (&Packet);
}

/* Copyright(C)URARA-works 2008 */
