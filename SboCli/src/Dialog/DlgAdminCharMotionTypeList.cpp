/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharMotionTypeList.cpp								 */
/* ���e			:�L�������[�V������ʃ��X�g�_�C�A���O�N���X �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/05/26													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "LibInfoMotion.h"
#include "LibInfoMotionType.h"
#include "PacketADMIN_CHAR_ADDMOTIONTYPE.h"
#include "PacketADMIN_CHAR_RENEWMOTIONTYPE.h"
#include "MgrData.h"
#include "DlgAdminCharMotionList.h"
#include "DlgAdminCharMotion.h"
#include "DlgAdminCharMotionTypeList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminCharMotionTypeList::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharMotionTypeList)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharMotionTypeList, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharMotionTypeList)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotionTypeList::CDlgAdminCharMotionTypeList		 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

CDlgAdminCharMotionTypeList::CDlgAdminCharMotionTypeList(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminCharMotionTypeList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharMotionTypeList)
	//}}AFX_DATA_INIT

	m_pLibInfoMotionType = NULL;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotionTypeList::~CDlgAdminCharMotionTypeList		 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

CDlgAdminCharMotionTypeList::~CDlgAdminCharMotionTypeList()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotionTypeList::Init								 */
/* ���e		:������															 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

void CDlgAdminCharMotionTypeList::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	m_pLibInfoMotionType = m_pMgrData->GetLibInfoMotionType ();

	/* �E�B���h�E�쐬 */
	Create (CDlgAdminCharMotionTypeList::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotionTypeList::Renew								 */
/* ���e		:���X�g�X�V														 */
/* ���t		:2008/05/27														 */
/* ========================================================================= */

void CDlgAdminCharMotionTypeList::Renew(void)
{
	int i, nCount;
	PCInfoMotionType pInfo;
	CmyString strTmp;

	m_List.DeleteAllItems ();

	nCount = m_pLibInfoMotionType->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMotionType)m_pLibInfoMotionType->GetPtr (i);

		strTmp.Format ("%d", pInfo->m_dwMotionTypeID);
		m_List.InsertItem (i, strTmp);
		m_List.SetItemText (i, 1, pInfo->m_strName);
		m_List.SetItemData (i, pInfo->m_dwMotionTypeID);
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotionTypeList::OnMainFrame						 */
/* ���e		:���b�Z�[�W�n���h��(WM_MAINFRAME)								 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

void CDlgAdminCharMotionTypeList::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
	switch (dwCommand) {
	case MAINFRAMEMSG_RENEWMOTIONTYPE:		/* ���[�V������ʏ��X�V */
		Renew ();
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotionTypeList::OnInitDialog						 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

BOOL CDlgAdminCharMotionTypeList::OnInitDialog()
{
	CmyString strTmp;

	CDlgAdminBase::OnInitDialog ();

	m_List.SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn (0, "ID",			LVCFMT_LEFT, 40);
	m_List.InsertColumn (1, "���[�V������",	LVCFMT_LEFT, 300);

	Renew ();

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotionTypeList::OnAdd								 */
/* ���e		:�{�^���n���h��(�ǉ�)											 */
/* ���t		:2008/05/27														 */
/* ========================================================================= */

void CDlgAdminCharMotionTypeList::OnAdd()
{
	CPacketADMIN_CHAR_ADDMOTIONTYPE Packet;

	Packet.Make ();
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotionTypeList::OnModify							 */
/* ���e		:�{�^���n���h��(�ҏW)											 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

void CDlgAdminCharMotionTypeList::OnModify()
{
	int nNo, nResult;
	DWORD dwMotionTypeID;
	PCInfoMotionType pInfo;
	CDlgAdminCharMotionList Dlg(this);
	CPacketADMIN_CHAR_RENEWMOTIONTYPE Packet;

	nNo = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nNo < 0) {
		return;
	}
	dwMotionTypeID = m_List.GetItemData (nNo);
	pInfo = (PCInfoMotionType)m_pLibInfoMotionType->GetPtr (dwMotionTypeID);
	if (pInfo == NULL) {
		return;
	}

	Dlg.Init (m_pMgrData, dwMotionTypeID);
	Dlg.m_strMotionName	= pInfo->m_strName;
	Dlg.m_nGrpIDSub		= pInfo->m_wGrpIDSub;

	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		return;
	}
	pInfo->m_strName	= Dlg.m_strMotionName;
	pInfo->m_wGrpIDSub	= Dlg.m_nGrpIDSub;
	Renew ();

	Packet.Make (dwMotionTypeID, m_pLibInfoMotionType);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharMotionTypeList::OnDelete							 */
/* ���e		:�{�^���n���h��(�폜)											 */
/* ���t		:2008/05/27														 */
/* ========================================================================= */

void CDlgAdminCharMotionTypeList::OnDelete()
{
}

/* Copyright(C)URARA-works 2008 */
