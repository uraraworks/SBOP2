/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapObjectData.cpp									 */
/* ���e			:�}�b�v�I�u�W�F�N�g�z�u�f�[�^�ꗗ�_�C�A���O�N���X �����t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/11/03													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "PacketADMIN_MAP_RENEWOBJECTDATA.h"
#include "PacketADMIN_PARA2.h"
#include "UraraSockTCPSBO.h"
#include "LibInfoMapObject.h"
#include "LibInfoMapObjectData.h"
#include "InfoMapBase.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "DlgAdminMapObjectDataEdit.h"
#include "DlgAdminMapObjectData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminMapObjectData::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminMapObjectData)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminMapObjectData, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminMapObjectData)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_COPY, OnCopy)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminMapObjectData::CDlgAdminMapObjectData					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

CDlgAdminMapObjectData::CDlgAdminMapObjectData(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminMapObjectData::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminMapObjectData)
	//}}AFX_DATA_INIT

	m_pWndNotify			= NULL;
	m_pLibInfoMapObject		= NULL;
	m_pLibInfoMapObjectData	= NULL;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapObjectData::~CDlgAdminMapObjectData				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

CDlgAdminMapObjectData::~CDlgAdminMapObjectData()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapObjectData::Init									 */
/* ���e		:������															 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

void CDlgAdminMapObjectData::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	m_pInfoMap = m_pMgrData->GetMap ();
	m_pLibInfoMapObject		= m_pMgrData->GetLibInfoMapObject ();
	m_pLibInfoMapObjectData = m_pInfoMap->m_pLibInfoMapObjectData;

	/* �E�B���h�E�쐬 */
	Create (CDlgAdminMapObjectData::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapObjectData::OnAdminMsg								 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

void CDlgAdminMapObjectData::OnAdminMsg(int nType, DWORD dwPara)
{
	PCDlgAdminMapObjectDataEdit pDlg;

	switch (nType) {
	case ADMINMSG_RENEWMAPINFO:					/* �}�b�v���X�V */
		Renew ();
		break;
	case ADMINMSG_NOTIFYTYPE_LBUTTONDOWN:		/* ���N���b�N�ʒm */
		pDlg = (PCDlgAdminMapObjectDataEdit)m_pWndNotify;
		if (pDlg == NULL) {
			break;
		}
		pDlg->PostMessage (WM_ADMINMSG, (WPARAM)nType, (LPARAM)dwPara);
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapObjectData::Renew									 */
/* ���e		:�ꗗ���X�V														 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

void CDlgAdminMapObjectData::Renew(void)
{
	int i, nCount, nCountTmp;
	PCInfoMapObjectData pInfo;
	PCInfoMapObject pInfoMapObject;
	CString strTmp;

	m_pInfoMap = m_pMgrData->GetMap ();
	m_pLibInfoMapObject		= m_pMgrData->GetLibInfoMapObject ();
	m_pLibInfoMapObjectData = m_pInfoMap->m_pLibInfoMapObjectData;

	m_List.DeleteAllItems ();

	nCountTmp = 0;
	nCount = m_pLibInfoMapObjectData->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapObjectData)m_pLibInfoMapObjectData->GetPtr (i);
		pInfoMapObject = (PCInfoMapObject)m_pLibInfoMapObject->GetPtr (pInfo->m_dwObjectID);
		if (pInfoMapObject == NULL) {
			continue;
		}
		strTmp.Format ("%d", pInfo->m_dwDataID);				/* ID */
		m_List.InsertItem (nCountTmp, strTmp);
		m_List.SetItemData (nCountTmp, pInfo->m_dwDataID);
		strTmp.Format ("%d", pInfo->m_ptPos.x);					/* X���W */
		m_List.SetItemText (nCountTmp, 1, strTmp);
		strTmp.Format ("%d", pInfo->m_ptPos.y);					/* Y���W */
		m_List.SetItemText (nCountTmp, 2, strTmp);
		m_List.SetItemText (nCountTmp, 3, (LPCSTR)pInfoMapObject->m_strName);	/* �I�u�W�F�N�g�� */
		nCountTmp ++;
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapObjectData::OnInitDialog							 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

BOOL CDlgAdminMapObjectData::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	m_List.SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn (1, "ID",				LVCFMT_LEFT, 50);
	m_List.InsertColumn (2, "X���W",			LVCFMT_LEFT, 50);
	m_List.InsertColumn (3, "Y���W",			LVCFMT_LEFT, 50);
	m_List.InsertColumn (4, "�I�u�W�F�N�g��",	LVCFMT_LEFT, 200);

	RegisterControl (IDC_RENEW,	LH_CTRL_X);
	RegisterControl (IDC_LIST,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);

	Renew ();
	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapObjectData::OnAdd									 */
/* ���e		:�{�^���n���h��(�V�K�ǉ�)										 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

void CDlgAdminMapObjectData::OnAdd()
{
	int nResult;
	PCInfoMapObjectData pInfo;
	CPacketADMIN_MAP_RENEWOBJECTDATA Packet;
	CDlgAdminMapObjectDataEdit Dlg(this);

	pInfo = NULL;

	Dlg.Init (m_pMgrData, &m_pWndNotify);
	nResult = Dlg.DoModal ();
	m_pWndNotify = NULL;
	if (nResult != IDOK) {
		goto Exit;
	}
	pInfo = (PCInfoMapObjectData)m_pLibInfoMapObjectData->GetNew (); 
	Dlg.Get (pInfo);

	Packet.Make (m_pInfoMap->m_dwMapID, pInfo);
	m_pSock->Send (&Packet);

Exit:
	SAFE_DELETE (pInfo);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapObjectData::OnModify								 */
/* ���e		:�{�^���n���h��(�ҏW)											 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

void CDlgAdminMapObjectData::OnModify()
{
	int nResult;
	DWORD dwDataID;
	PCInfoMapObjectData pInfo;
	CPacketADMIN_MAP_RENEWOBJECTDATA Packet;
	CDlgAdminMapObjectDataEdit Dlg(this);

	nResult = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nResult < 0) {
		return;
	}
	dwDataID	= m_List.GetItemData (nResult);
	pInfo		= (PCInfoMapObjectData)m_pLibInfoMapObjectData->GetPtr (dwDataID);

	Dlg.Init (m_pMgrData, &m_pWndNotify);
	Dlg.SetModify (pInfo);

	nResult = Dlg.DoModal ();
	m_pWndNotify = NULL;
	if (nResult != IDOK) {
		return;
	}
	Dlg.Get (pInfo);

	Packet.Make (m_pInfoMap->m_dwMapID, pInfo);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapObjectData::OnCopy									 */
/* ���e		:�{�^���n���h��(�R�s�[)											 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

void CDlgAdminMapObjectData::OnCopy()
{
#if 0
	int nNo, nResult;
	DWORD dwItemID;
	PCInfoMapEventBase pInfoItem;
	CPacketADMIN_ITEMTYPE_COPY Packet;

	nNo = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nNo < 0) {
		return;
	}
	dwItemID	= m_List.GetItemData (nNo);
	pInfoItem	= (PCInfoMapEventBase)m_pLibInfoMapEvent->GetPtr (dwItemID);
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
/* �֐���	:CDlgAdminMapObjectData::OnDelete								 */
/* ���e		:�{�^���n���h��(�폜)											 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

void CDlgAdminMapObjectData::OnDelete()
{
	int nResult;
	DWORD dwDataID;
	PCInfoMapObjectData pInfo;
	CPacketADMIN_PARA2 Packet;

	nResult = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nResult < 0) {
		return;
	}
	dwDataID	= m_List.GetItemData (nResult);
	pInfo		= (PCInfoMapObjectData)m_pLibInfoMapObjectData->GetPtr (dwDataID);
	if (pInfo == NULL) {
		return;
	}

	nResult = MessageBox ("�I������Ă���f�[�^���폜���܂����H", "�m�F", MB_YESNO | MB_ICONQUESTION);
	if (nResult != IDYES) {
		return;
	}

	Packet.Make (SBOCOMMANDID_SUB_ADMIN_MAP_DELETEOBJECTDATA, m_pInfoMap->m_dwMapID, dwDataID);
	m_pSock->Send (&Packet);
}

/* Copyright(C)URARA-works 2008 */
