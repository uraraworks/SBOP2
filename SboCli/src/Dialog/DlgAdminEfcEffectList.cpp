/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminEfcEffectList.cpp									 */
/* ���e			:���o���ꗗ�_�C�A���O�N���X �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/07/06													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "LibInfoEffect.h"
#include "PacketADMIN_EFC_RENEWEFFECT.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminEfcEffect.h"
#include "DlgAdminEfcEffectList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminEfcEffectList::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminEfcEffectList)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminEfcEffectList, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminEfcEffectList)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_RENEW, OnRenew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcEffectList::CDlgAdminEfcEffectList					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

CDlgAdminEfcEffectList::CDlgAdminEfcEffectList(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminEfcEffectList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminEfcEffectList)
	//}}AFX_DATA_INIT
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcEffectList::~CDlgAdminEfcEffectList				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

CDlgAdminEfcEffectList::~CDlgAdminEfcEffectList()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcEffectList::Init									 */
/* ���e		:������															 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffectList::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	m_pLibInfo = m_pMgrData->GetLibInfoEffect ();

	/* �E�B���h�E�쐬 */
	Create (CDlgAdminEfcEffectList::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcEffectList::Renew									 */
/* ���e		:�ꗗ���X�V														 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffectList::Renew(void)
{
	int i, nCount;
	PCInfoEffect pInfoEffect;
	CmyString strTmp;

	m_List.DeleteAllItems ();

	nCount = m_pLibInfo->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoEffect = (PCInfoEffect)m_pLibInfo->GetPtr (i);
		strTmp.Format ("%d", pInfoEffect->m_dwEffectID);
		m_List.InsertItem (i, strTmp);
		m_List.SetItemData (i, pInfoEffect->m_dwEffectID);
		m_List.SetItemText (i, 1, (LPCTSTR)pInfoEffect->m_strName);
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcEffectList::OnAdminMsg								 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffectList::OnAdminMsg(int nType, DWORD dwPara)
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcEffectList::OnInitDialog							 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

BOOL CDlgAdminEfcEffectList::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	m_List.SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn (0, "ID",			LVCFMT_LEFT, 40);
	m_List.InsertColumn (1, "�G�t�F�N�g��",	LVCFMT_LEFT, 120);

	RegisterControl (IDC_RENEW,	LH_CTRL_X);
	RegisterControl (IDC_LIST,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);

	Renew ();

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcEffectList::OnMainFrame							 */
/* ���e		:���b�Z�[�W�n���h��(WM_MAINFRAME)								 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffectList::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
	switch (dwCommand) {
	case MAINFRAMEMSG_RENEWEFFECT:		/* �G�t�F�N�g���X�V */
		Renew ();
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcEffectList::OnAdd									 */
/* ���e		:�{�^���n���h��(�V�K�ǉ�)										 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffectList::OnAdd()
{
	int nResult;
	PCInfoEffect pInfoEffect;
	CDlgAdminEfcEffect Dlg(this);
	CPacketADMIN_EFC_RENEWEFFECT Packet;

	Dlg.Init (m_pMgrData, FALSE);
	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		return;
	}

	pInfoEffect = (PCInfoEffect)m_pLibInfo->GetNew ();
	Dlg.GetInfo (pInfoEffect);

	Packet.Make (0, pInfoEffect);
	m_pSock->Send (&Packet);
	SAFE_DELETE (pInfoEffect);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcEffectList::OnModify								 */
/* ���e		:�{�^���n���h��(�ҏW)											 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffectList::OnModify()
{
	int nNo, nResult;
	PCInfoEffect pInfoEffect;
	CDlgAdminEfcEffect Dlg(this);
	CPacketADMIN_EFC_RENEWEFFECT Packet;

	nNo = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nNo < 0) {
		return;
	}
	pInfoEffect = (PCInfoEffect)m_pLibInfo->GetPtr (m_List.GetItemData (nNo));

	Dlg.Init (m_pMgrData, TRUE);
	Dlg.SetInfo (pInfoEffect);
	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		return;
	}

	Dlg.GetInfo (pInfoEffect);

	Packet.Make (pInfoEffect->m_dwEffectID, pInfoEffect);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcEffectList::OnDelete								 */
/* ���e		:�{�^���n���h��(�폜)											 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffectList::OnDelete()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminEfcEffectList::OnRenew								 */
/* ���e		:�{�^���n���h��(�X�V)											 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffectList::OnRenew()
{
}

/* Copyright(C)URARA-works 2008 */
