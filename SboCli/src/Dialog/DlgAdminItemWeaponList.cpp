/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminItemWeaponList.cpp									 */
/* ���e			:�A�C�e��������ꗗ�_�C�A���O�N���X �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/08/10													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "PacketADMIN_ITEMWEAPON_ADD.h"
#include "PacketADMIN_ITEMWEAPON_RENEW.h"
#include "PacketITEM_REQ_ITEMWEAPONINFO.h"
#include "LibInfoItemWeapon.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminItemWeaponNew.h"
#include "DlgAdminItemWeaponList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminItemWeaponList::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminItemWeaponList)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminItemWeaponList, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminItemWeaponList)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_COPY, OnCopy)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminItemWeaponList::CDlgAdminItemWeaponList				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

CDlgAdminItemWeaponList::CDlgAdminItemWeaponList(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminItemWeaponList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminItemWeaponList)
	//}}AFX_DATA_INIT

	m_pLibInfoItemWeapon = NULL;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemWeaponList::~CDlgAdminItemWeaponList				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

CDlgAdminItemWeaponList::~CDlgAdminItemWeaponList()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemWeaponList::Init									 */
/* ���e		:������															 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

void CDlgAdminItemWeaponList::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	m_pLibInfoItemWeapon = m_pMgrData->GetLibInfoItemWeapon ();

	/* �E�B���h�E�쐬 */
	Create (CDlgAdminItemWeaponList::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemWeaponList::Renew									 */
/* ���e		:�ꗗ���X�V														 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

void CDlgAdminItemWeaponList::Renew(void)
{
	int i, nCount, nSelect;
	PCInfoItemWeapon pInfoItemWeapon;
	CString strTmp;

	nSelect = m_List.GetNextItem (-1, LVNI_SELECTED);
	m_List.SetRedraw (FALSE);
	m_List.DeleteAllItems ();

	nCount = m_pLibInfoItemWeapon->GetCount ();
	m_List.SetItemCount (nCount);

	for (i = 0; i < nCount; i ++) {
		pInfoItemWeapon = (PCInfoItemWeapon)m_pLibInfoItemWeapon->GetPtr (i);
		if (pInfoItemWeapon == NULL) {
			continue;
		}
		strTmp.Format ("%d", pInfoItemWeapon->m_dwWeaponInfoID);
		m_List.InsertItem (i, strTmp);
		m_List.SetItemData (i, pInfoItemWeapon->m_dwWeaponInfoID);	/* ID */
		m_List.SetItemText (i, 1, pInfoItemWeapon->m_strName);		/* �����ʖ� */
	}

	if (nSelect >= 0) {
		m_List.SetItemState (nSelect, LVNI_SELECTED, LVNI_SELECTED);
		m_List.EnsureVisible (nSelect, FALSE);
	}
	m_List.SetRedraw ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemWeaponList::OnAdminMsg							 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2007/08/23														 */
/* ========================================================================= */

void CDlgAdminItemWeaponList::OnAdminMsg(int nType, DWORD dwPara)
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemWeaponList::OnInitDialog							 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

BOOL CDlgAdminItemWeaponList::OnInitDialog()
{
	CPacketITEM_REQ_ITEMWEAPONINFO Packet;

	CDlgAdminBase::OnInitDialog();

	m_List.SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn (0, "ID",			LVCFMT_LEFT, 40);
	m_List.InsertColumn (1, "�����ʖ�",	LVCFMT_LEFT, 120);

	RegisterControl (IDC_LIST, LH_CTRL_WIDTH | LH_CTRL_HEIGHT);

	/* �S�������v�� */
	Packet.Make (0);
	m_pSock->Send (&Packet);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemWeaponList::OnMainFrame							 */
/* ���e		:���b�Z�[�W�n���h��(WM_MAINFRAME)								 */
/* ���t		:2007/08/16														 */
/* ========================================================================= */

void CDlgAdminItemWeaponList::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
	switch (dwCommand) {
	case MAINFRAMEMSG_RENEWITEMWEAPONINFO:		/* ������X�V */
		Renew ();
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemWeaponList::OnAdd									 */
/* ���e		:�{�^���n���h��(�V�K�ǉ�)										 */
/* ���t		:2007/08/15														 */
/* ========================================================================= */

void CDlgAdminItemWeaponList::OnAdd()
{
	int nResult;
	PCInfoItemWeapon pInfoItemWeapon;
	CPacketADMIN_ITEMWEAPON_ADD Packet;
	CDlgAdminItemWeaponNew Dlg(this);

	pInfoItemWeapon = NULL;

	Dlg.Init (m_pMgrData);
	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		goto Exit;
	}

	pInfoItemWeapon = (PCInfoItemWeapon)m_pLibInfoItemWeapon->GetNew ();
	Dlg.Get (pInfoItemWeapon);

	Packet.Make (pInfoItemWeapon);
	m_pSock->Send (&Packet);

Exit:
	SAFE_DELETE (pInfoItemWeapon);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemWeaponList::OnModify								 */
/* ���e		:�{�^���n���h��(�ҏW)											 */
/* ���t		:2007/08/16														 */
/* ========================================================================= */

void CDlgAdminItemWeaponList::OnModify()
{
	int nResult;
	DWORD dwWeaponInfoID;
	CDlgAdminItemWeaponNew Dlg(this);
	PCInfoItemWeapon pInfoItemWeapon;
	CPacketADMIN_ITEMWEAPON_RENEW Packet;

	nResult = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nResult < 0) {
		goto Exit;
	}
	dwWeaponInfoID	= m_List.GetItemData (nResult);
	pInfoItemWeapon	= (PCInfoItemWeapon)m_pLibInfoItemWeapon->GetPtr (dwWeaponInfoID);
	if (pInfoItemWeapon == NULL) {
		goto Exit;
	}

	Dlg.Init (m_pMgrData);
	Dlg.Set (pInfoItemWeapon);

	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		goto Exit;
	}

	Dlg.Get (pInfoItemWeapon);

	Packet.Make (pInfoItemWeapon);
	m_pSock->Send (&Packet);

Exit:
	return;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemWeaponList::OnCopy								 */
/* ���e		:�{�^���n���h��(�R�s�[)											 */
/* ���t		:2007/09/22														 */
/* ========================================================================= */

void CDlgAdminItemWeaponList::OnCopy()
{
#if 0
	int nNo, nResult;
	DWORD dwItemID;
	PCInfoItem pInfoItem;
	CPacketADMIN_ITEM_COPY Packet;

	nNo = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nNo < 0) {
		return;
	}
	dwItemID	= m_List.GetItemData (nNo);
	pInfoItem	= (PCInfoItem)m_pLibInfoItem->GetPtr (dwItemID);
	if (pInfoItem == NULL) {
		return;
	}

	nResult = MessageBox ("�I������Ă���A�C�e�����R�s�[���܂����H", "�m�F", MB_YESNO | MB_ICONQUESTION);
	if (nResult != IDYES) {
		return;
	}

	Packet.Make (dwItemID);
	m_pSock->Send (&Packet);
#endif
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemWeaponList::OnDelete								 */
/* ���e		:�{�^���n���h��(�폜)											 */
/* ���t		:2007/08/19														 */
/* ========================================================================= */

void CDlgAdminItemWeaponList::OnDelete()
{
#if 0
	int nResult;
	DWORD dwItemID;
	PCInfoItem pInfoItem;
	CString strTmp;
	CPacketADMIN_ITEM_DELETE Packet;

	nResult = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nResult < 0) {
		return;
	}
	dwItemID	= m_List.GetItemData (nResult);
	pInfoItem	= (PCInfoItem)m_pLibInfoItem->GetPtr (dwItemID);
	if (pInfoItem == NULL) {
		return;
	}

	strTmp.Format ("[%s]���폜���܂����H", (LPCSTR)pInfoItem->m_strName);
	nResult = MessageBox (strTmp, "�m�F", MB_YESNO | MB_ICONQUESTION);
	if (nResult != IDYES) {
		return;
	}

	Packet.Make (dwItemID);
	m_pSock->Send (&Packet);
#endif
}

/* Copyright(C)URARA-works 2008 */
