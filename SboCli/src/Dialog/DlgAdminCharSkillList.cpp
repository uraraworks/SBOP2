/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharSkillList.cpp									 */
/* ���e			:�X�L�����X�g�_�C�A���O�N���X �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/07													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "PacketADMIN_SKILL_RENEWSKILL.h"
#include "LibInfoSkill.h"
#include "MgrData.h"
#include "DlgAdminCharSkillBase.h"
#include "DlgAdminCharSkillList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminCharSkillList::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharSkillList)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharSkillList, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharSkillList)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillList::CDlgAdminCharSkillList					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

CDlgAdminCharSkillList::CDlgAdminCharSkillList(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminCharSkillList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharSkillList)
	//}}AFX_DATA_INIT

	m_pLibInfoSkill = NULL;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillList::~CDlgAdminCharSkillList				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

CDlgAdminCharSkillList::~CDlgAdminCharSkillList()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillList::Init									 */
/* ���e		:������															 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillList::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	m_pLibInfoSkill = m_pMgrData->GetLibInfoSkill ();

	/* �E�B���h�E�쐬 */
	Create (CDlgAdminCharSkillList::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillList::Renew									 */
/* ���e		:���X�g�X�V														 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillList::Renew(void)
{
	int i, nCount;
	PCInfoSkillBase pInfo;
	CmyString strTmp;

	m_List.DeleteAllItems ();

	nCount = m_pLibInfoSkill->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoSkillBase)m_pLibInfoSkill->GetPtr (i);

		strTmp.Format ("%d", pInfo->m_dwSkillID);
		m_List.InsertItem (i, strTmp);
		m_List.SetItemText (i, 1, pInfo->m_strName);
		m_List.SetItemData (i, pInfo->m_dwSkillID);
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillList::OnMainFrame							 */
/* ���e		:���b�Z�[�W�n���h��(WM_MAINFRAME)								 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillList::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
	switch (dwCommand) {
	case MAINFRAMEMSG_RENEWSKILLINFO:		/* �X�L�����X�V */
		Renew ();
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillList::OnInitDialog							 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

BOOL CDlgAdminCharSkillList::OnInitDialog()
{
	CmyString strTmp;

	CDlgAdminBase::OnInitDialog ();

	m_List.SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn (0, "ID",		LVCFMT_LEFT, 40);
	m_List.InsertColumn (1, "�X�L����",	LVCFMT_LEFT, 300);

	Renew ();

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillList::OnAdd									 */
/* ���e		:�{�^���n���h��(�ǉ�)											 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillList::OnAdd()
{
	int nResult;
	PCInfoSkillBase pInfo;
	CPacketADMIN_SKILL_RENEWSKILL Packet;
	CDlgAdminCharSkillBase Dlg(this);

	pInfo = NULL;

	Dlg.Init (m_pMgrData);
	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		goto Exit;
	}

	Dlg.Get (pInfo);

	Packet.Make (pInfo);
	m_pSock->Send (&Packet);

Exit:
	SAFE_DELETE (pInfo);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillList::OnModify								 */
/* ���e		:�{�^���n���h��(�ҏW)											 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillList::OnModify()
{
	int nResult;
	DWORD dwSkillID;
	CDlgAdminCharSkillBase Dlg(this);
	PCInfoSkillBase pInfo, pInfoTmp;
	CPacketADMIN_SKILL_RENEWSKILL Packet;

	nResult = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nResult < 0) {
		goto Exit;
	}
	dwSkillID	= m_List.GetItemData (nResult);
	pInfo		= (PCInfoSkillBase)m_pLibInfoSkill->GetPtr (dwSkillID);
	if (pInfo == NULL) {
		goto Exit;
	}

	Dlg.Init (m_pMgrData);
	Dlg.SetModify (pInfo);

	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		goto Exit;
	}

	pInfoTmp = NULL;
	Dlg.Get (pInfoTmp);
	pInfo = m_pLibInfoSkill->Renew (pInfoTmp);
	SAFE_DELETE (pInfoTmp);

	Packet.Make (pInfo);
	m_pSock->Send (&Packet);

Exit:
	return;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillList::OnDelete								 */
/* ���e		:�{�^���n���h��(�폜)											 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillList::OnDelete()
{
}

/* Copyright(C)URARA-works 2008 */
