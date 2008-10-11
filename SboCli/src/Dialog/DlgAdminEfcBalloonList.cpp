/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminEfcBalloonList.cpp									 */
/* ���e			:���o���ꗗ�_�C�A���O�N���X �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/12/24													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "LibInfoEfcBalloon.h"
#include "PacketADMIN_EFC_RENEWBALLOON.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminEfcBalloon.h"
#include "DlgAdminEfcBalloonList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminEfcBalloonList::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminEfcBalloonList)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminEfcBalloonList, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminEfcBalloonList)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_RENEW, OnRenew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcBalloonList::CDlgAdminEfcBalloonList				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

CDlgAdminEfcBalloonList::CDlgAdminEfcBalloonList(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminEfcBalloonList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminEfcBalloonList)
	//}}AFX_DATA_INIT
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcBalloonList::~CDlgAdminEfcBalloonList				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

CDlgAdminEfcBalloonList::~CDlgAdminEfcBalloonList()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcBalloonList::Init									 */
/* ���e		:������															 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

void CDlgAdminEfcBalloonList::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	m_pLibInfo = m_pMgrData->GetLibInfoEfcBalloon ();

	/* �E�B���h�E�쐬 */
	Create (CDlgAdminEfcBalloonList::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcBalloonList::Renew									 */
/* ���e		:�ꗗ���X�V														 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

void CDlgAdminEfcBalloonList::Renew(void)
{
	int i, nCount;
	DWORD dwListID;
	ARRAYDWORD aIDList;
	CmyString strTmp;

	m_List.DeleteAllItems ();

	m_pLibInfo->GetListID (aIDList);
	nCount = aIDList.GetSize ();
	for (i = 0; i < nCount; i ++) {
		dwListID = aIDList[i];
		strTmp.Format ("%d", dwListID);
		m_List.InsertItem (i, strTmp);
		m_List.SetItemData (i, dwListID);
		m_pLibInfo->GetName (dwListID, strTmp);
		m_List.SetItemText (i, 1, (LPCTSTR)strTmp);
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcBalloonList::OnAdminMsg							 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

void CDlgAdminEfcBalloonList::OnAdminMsg(int nType, DWORD dwPara)
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcBalloonList::OnInitDialog							 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

BOOL CDlgAdminEfcBalloonList::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	m_List.SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn (0, "ID",			LVCFMT_LEFT, 40);
	m_List.InsertColumn (1, "���o����",		LVCFMT_LEFT, 120);

	RegisterControl (IDC_RENEW,	LH_CTRL_X);
	RegisterControl (IDC_LIST,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);

	Renew ();

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcBalloonList::OnMainFrame							 */
/* ���e		:���b�Z�[�W�n���h��(WM_MAINFRAME)								 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

void CDlgAdminEfcBalloonList::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
	switch (dwCommand) {
	case MAINFRAMEMSG_RENEWBALLOON:		/* ���o�����X�V */
		Renew ();
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcBalloonList::OnAdd									 */
/* ���e		:�{�^���n���h��(�V�K�ǉ�)										 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

void CDlgAdminEfcBalloonList::OnAdd()
{
	int nResult;
	CDlgAdminEfcBalloon Dlg(this);
	CPacketADMIN_EFC_RENEWBALLOON Packet;

	Dlg.Init (m_pMgrData, 0);
	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		return;
	}

	Dlg.GetList (m_pLibInfo);

	Packet.Make (Dlg.GetListID (), m_pLibInfo);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcBalloonList::OnModify								 */
/* ���e		:�{�^���n���h��(�ҏW)											 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

void CDlgAdminEfcBalloonList::OnModify()
{
	int nNo, nResult;
	CDlgAdminEfcBalloon Dlg(this);
	CPacketADMIN_EFC_RENEWBALLOON Packet;

	nNo = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nNo < 0) {
		return;
	}

	Dlg.Init (m_pMgrData, m_List.GetItemData (nNo), TRUE);
	Dlg.SetList (m_pLibInfo);
	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		return;
	}

	Dlg.GetList (m_pLibInfo);

	Packet.Make (Dlg.GetListID (), m_pLibInfo);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcBalloonList::OnDelete								 */
/* ���e		:�{�^���n���h��(�폜)											 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

void CDlgAdminEfcBalloonList::OnDelete()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcBalloonList::OnRenew								 */
/* ���e		:�{�^���n���h��(�X�V)											 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

void CDlgAdminEfcBalloonList::OnRenew()
{
}

/* Copyright(C)URARA-works 2007 */
