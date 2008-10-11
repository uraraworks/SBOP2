/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminItemNew.cpp										 */
/* ���e			:�A�C�e���V�K�ǉ��_�C�A���O�N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/08/15													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoItem.h"
#include "LibInfoItemType.h"
#include "WndSelectItemGrp.h"
#include "MgrGrpData.h"
#include "MgrData.h"
#include "MgrSound.h"
#include "LibSboSoundLoader.h"
#include "DlgAdminItemNew.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminItemNew::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminItemNew)
	DDX_Control(pDX, IDC_DROPSOUNDID, m_ctlDropSoundID);
	DDX_Control(pDX, IDC_TYPE, m_ctlType);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_POSMAPID, m_nMapID);
	DDX_Text(pDX, IDC_POSX, m_nPosX);
	DDX_Text(pDX, IDC_POSY, m_nPosY);
	DDX_Text(pDX, IDC_GRPID, m_nGrpID);
	DDX_Check(pDX, IDC_PUTON, m_bPutOn);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminItemNew, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminItemNew)
	ON_BN_CLICKED(IDC_SELECT_ITEMGRP, OnSelectItemgrp)
	ON_BN_CLICKED(IDC_PLAY, OnPlay)
	ON_CBN_SELCHANGE(IDC_TYPE, OnSelchangeType)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_WNDCLOSE, OnWndClose)
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminItemNew::CDlgAdminItemNew								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/08/15														 */
/* ========================================================================= */

CDlgAdminItemNew::CDlgAdminItemNew(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminItemNew::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminItemNew)
	m_strName = _T("");
	m_nMapID = 0;
	m_nPosX = 0;
	m_nPosY = 0;
	m_nGrpID = 0;
	m_bPutOn = FALSE;
	//}}AFX_DATA_INIT

	m_bPosSet				= TRUE;
	m_dwSelectItemTypeID	= 0;
	m_dwSelectSoundID		= 0;
	m_bModeModify			= FALSE;
	m_ppWndNotify			= NULL;
	m_pMgrGrpData			= NULL;
	m_pWndSelectItemGrp		= NULL;
	m_pLibInfoItemType		= NULL;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemNew::~CDlgAdminItemNew							 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/08/15														 */
/* ========================================================================= */

CDlgAdminItemNew::~CDlgAdminItemNew()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemNew::Init											 */
/* ���e		:������															 */
/* ���t		:2007/08/23														 */
/* ========================================================================= */

void CDlgAdminItemNew::Init(CMgrData *pMgrData, CWnd **pWndNotify, BOOL bPosSet)
{
	CDlgAdminBase::Init (pMgrData);

	m_ppWndNotify	= pWndNotify;
	m_bPosSet		= bPosSet;

	m_pMgrGrpData		= m_pMgrData->GetMgrGrpData ();
	m_pLibInfoItemType	= m_pMgrData->GetLibInfoItemType ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemNew::Get											 */
/* ���e		:�ҏW���e���擾													 */
/* ���t		:2007/08/16														 */
/* ========================================================================= */

void CDlgAdminItemNew::Get(CInfoItem *pInfoItem)
{
	PCInfoItemTypeBase pInfoItemType;

	pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr (m_dwSelectItemTypeID);
	if (pInfoItemType == NULL) {
		return;
	}

	pInfoItem->m_dwItemTypeID	= m_dwSelectItemTypeID;
	pInfoItem->m_dwDropSoundID	= m_dwSelectSoundID;
	pInfoItem->m_dwMapID		= m_nMapID;
	pInfoItem->m_ptPos.x		= m_nPosX;
	pInfoItem->m_ptPos.y		= m_nPosY;
	pInfoItem->m_strName		= m_strName;
	pInfoItem->m_dwGrpID		= m_nGrpID;
	pInfoItem->m_dwIconGrpID	= pInfoItemType->m_dwIconGrpID;
	pInfoItem->m_bPutOn			= m_bPutOn;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemNew::SetModify									 */
/* ���e		:�ҏW���[�h�Ƃ��Đݒ�											 */
/* ���t		:2007/08/16														 */
/* ========================================================================= */

void CDlgAdminItemNew::SetModify(CInfoItem *pInfoItem)
{
	m_dwSelectItemTypeID	= pInfoItem->m_dwItemTypeID;
	m_dwSelectSoundID		= pInfoItem->m_dwDropSoundID;
	m_nMapID				= pInfoItem->m_dwMapID;
	m_nPosX					= pInfoItem->m_ptPos.x;
	m_nPosY					= pInfoItem->m_ptPos.y;
	m_strName				= pInfoItem->m_strName;
	m_nGrpID				= pInfoItem->m_dwGrpID;
	m_bPutOn				= pInfoItem->m_bPutOn;

	m_bModeModify = TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemNew::OnInitDialog									 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2007/08/15														 */
/* ========================================================================= */

BOOL CDlgAdminItemNew::OnInitDialog()
{
	int i, nNo, nCount;
	DWORD dwSoundID;
	LPCSTR pszTmp;
	PCInfoItemTypeBase pInfoItem;
	CLibSboSoundLoader LibSboSoundLoader;

	CDlgAdminBase::OnInitDialog();

	if (m_ppWndNotify) {
		*m_ppWndNotify = this;
	}
	m_pMgrGrpData->ReadItemTmp ();

	if (m_bModeModify) {
		SetWindowText ("�A�C�e�����̕ҏW");
	}
	/* �ݒu�ʒu�͎w�肵�Ȃ����[�h�H */
	if (m_bPosSet == FALSE) {
		GetDlgItem (IDC_POSMAPID)->	EnableWindow (FALSE);
		GetDlgItem (IDC_POSX)->		EnableWindow (FALSE);
		GetDlgItem (IDC_POSY)->		EnableWindow (FALSE);
	}

	m_ctlDropSoundID.InsertString (0, "����");
	nNo = 0;

	nCount = LibSboSoundLoader.GetSoundCount ();
	for (i = 0; i < nCount; i ++) {
		dwSoundID	= LibSboSoundLoader.GetSoundID (i);
		pszTmp		= LibSboSoundLoader.GetSoundName (dwSoundID);
		m_ctlDropSoundID.InsertString (i + 1, pszTmp);
		m_ctlDropSoundID.SetItemData (i + 1, dwSoundID);
	}

	m_ctlType.InsertString (0, "���ݒ�");
	m_ctlType.SetCurSel (0);

	nCount = m_pLibInfoItemType->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoItem = (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr (i);
		m_ctlType.InsertString (i + 1, pInfoItem->m_strName);
		m_ctlType.SetItemData (i + 1, pInfoItem->m_dwTypeID);
		if (m_dwSelectItemTypeID == pInfoItem->m_dwTypeID) {
			m_ctlType.SetCurSel (i + 1);
		}
	}

	SelectSound (m_dwSelectSoundID);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemNew::PostNcDestroy								 */
/* ���e		:�I������														 */
/* ���t		:2007/08/15														 */
/* ========================================================================= */

void CDlgAdminItemNew::PostNcDestroy()
{
	if (m_pMgrGrpData) {
		m_pMgrGrpData->DeleteItemTmp ();
	}

	CDialog::PostNcDestroy ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemNew::OnSelectItemgrp								 */
/* ���e		:�{�^���n���h��(�摜�I��)										 */
/* ���t		:2007/08/16														 */
/* ========================================================================= */

void CDlgAdminItemNew::OnSelectItemgrp()
{
	m_pWndSelectItemGrp = new CWndSelectItemGrp;
	m_pWndSelectItemGrp->Create (this, m_pMgrData);
	m_pWndSelectItemGrp->ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemNew::OnWndClose									 */
/* ���e		:���b�Z�[�W�n���h��(WM_WNDCLOSE)								 */
/* ���t		:2007/05/14														 */
/* ========================================================================= */

LRESULT CDlgAdminItemNew::OnWndClose(WPARAM wParam, LPARAM lParam)
{
	switch (wParam) {
	case WINDOWID_SELECTMAPPARTSGRP:	/* �A�C�e���摜�I�� */
		if (lParam < 0) {
			break;
		}
		UpdateData ();
		m_nGrpID = lParam;
		UpdateData (FALSE);
		break;
	}

	return -1;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemNew::OnSelchangeType								 */
/* ���e		:�C�x���g�n���h��(CBN_SELCHANGE)								 */
/* ���t		:2007/10/06														 */
/* ========================================================================= */

void CDlgAdminItemNew::OnSelchangeType()
{
	int nNo;
	DWORD dwTypeID;
	PCInfoItemTypeBase pInfoItem;

	nNo = m_ctlType.GetCurSel ();

	dwTypeID = m_ctlType.GetItemData (nNo);
	pInfoItem = (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr (dwTypeID);
	if (pInfoItem == NULL) {
		return;
	}

	UpdateData ();

	m_strName	= pInfoItem->m_strName;
	m_nGrpID	= pInfoItem->m_dwGrpID;
	m_bPutOn	= pInfoItem->m_bPutOn;
	SelectSound (pInfoItem->m_dwDropSoundID);

	UpdateData (FALSE);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemNew::OnPlay										 */
/* ���e		:�{�^���n���h��(�Đ�)											 */
/* ���t		:2007/09/19														 */
/* ========================================================================= */

void CDlgAdminItemNew::OnPlay()
{
	int nNo;
	DWORD dwSoundID;
	PCMgrSound pMgrSound;

	nNo = m_ctlDropSoundID.GetCurSel ();
	if (nNo < 0) {
		goto Exit;
	}

	pMgrSound = m_pMgrData->GetMgrSound ();
	dwSoundID = m_ctlDropSoundID.GetItemData (nNo);
	if (dwSoundID == 0) {
		goto Exit;
	}
	pMgrSound->PlaySound (dwSoundID);

Exit:
	m_ctlDropSoundID.SetFocus ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemNew::OnOK											 */
/* ���e		:�{�^���n���h��(OK)												 */
/* ���t		:2007/08/16														 */
/* ========================================================================= */

void CDlgAdminItemNew::OnOK()
{
	int nNo;

	UpdateData ();

	nNo = m_ctlDropSoundID.GetCurSel ();
	m_dwSelectItemTypeID	= m_ctlType.GetItemData (m_ctlType.GetCurSel ());
	m_dwSelectSoundID		= m_ctlDropSoundID.GetItemData (nNo);
	CDlgAdminBase::EndDialog (IDOK);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemNew::SelectSound									 */
/* ���e		:���Ƃ����Ƃ��̌��ʉ�ID��I��									 */
/* ���t		:2007/10/06														 */
/* ========================================================================= */

void CDlgAdminItemNew::SelectSound(DWORD dwSoundID)
{
	int i, nCount;
	DWORD dwResult;

	nCount = m_ctlDropSoundID.GetCount ();
	for (i = 0; i < nCount; i ++) {
		dwResult = m_ctlDropSoundID.GetItemData (i);
		if (dwResult == dwSoundID) {
			m_ctlDropSoundID.SetCurSel (i);
			break;
		}
	}
}

/* Copyright(C)URARA-works 2007 */
