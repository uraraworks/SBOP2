/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminItemTypeNewARMS.cpp								 */
/* ���e			:�A�C�e�����(������)�ݒ�_�C�A���O�N���X �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/12/08													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoItemTypeBase.h"
#include "LibInfoItemWeapon.h"
#include "InfoMotion.h"
#include "MgrData.h"
#include "DlgAdminItemTypeNewARMSBow.h"
#include "DlgAdminItemTypeNewARMS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminItemTypeNewARMS::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminItemTypeNewARMS)
	DDX_Control(pDX, IDC_WEAPONTYPE, m_ctlWeaponType);
	DDX_Text(pDX, IDC_VALUE, m_dwValue);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminItemTypeNewARMS, CDlgAdminBase)
	ON_CBN_SELCHANGE(IDC_WEAPONTYPE, &CDlgAdminItemTypeNewARMS::OnCbnSelchangeWeapontype)
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminItemTypeNewARMS::CDlgAdminItemTypeNewARMS				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/12/08														 */
/* ========================================================================= */

CDlgAdminItemTypeNewARMS::CDlgAdminItemTypeNewARMS(CWnd* pParent /*=NULL*/)
: CDlgAdminBase(CDlgAdminItemTypeNewARMS::IDD, pParent)
, m_pSrc(NULL)
, m_dwValue(0)
, m_dwWeaponInfoID(0)
, m_pDlgWeaponType(NULL)
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemTypeNewARMS::~CDlgAdminItemTypeNewARMS			 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/12/08														 */
/* ========================================================================= */

CDlgAdminItemTypeNewARMS::~CDlgAdminItemTypeNewARMS()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemTypeNewARMS::Set									 */
/* ���e		:�ҏW���e��ݒ�													 */
/* ���t		:2007/12/08														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewARMS::Set(CInfoItemTypeBase *pSrc)
{
	m_pSrc = pSrc;
	m_dwWeaponInfoID	= pSrc->m_dwWeaponInfoID;		/* ������ID */
	m_dwValue			= pSrc->m_dwValue;				/* �U���� */
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemTypeNewARMS::Get									 */
/* ���e		:�ҏW���e���擾													 */
/* ���t		:2007/12/08														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewARMS::Get(CInfoItemTypeBase *&pDst)
{
	pDst->m_dwWeaponInfoID	= m_dwWeaponInfoID;		/* ������ID */
	pDst->m_dwValue			= m_dwValue;			/* �U���� */
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemTypeNewARMS::OnInitDialog							 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2007/12/08														 */
/* ========================================================================= */

BOOL CDlgAdminItemTypeNewARMS::OnInitDialog()
{
	int i, nCount;
	PCLibInfoItemWeapon pLibInfoItemWeapon;
	PCInfoItemWeapon pInfoItemWeapon;

	CDlgAdminBase::OnInitDialog();

	pLibInfoItemWeapon = m_pMgrData->GetLibInfoItemWeapon ();

	m_ctlWeaponType.InsertString (0, "���w��");
	nCount = pLibInfoItemWeapon->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoItemWeapon = (PCInfoItemWeapon)pLibInfoItemWeapon->GetPtr (i);
		m_ctlWeaponType.InsertString (i + 1, (LPCSTR)pInfoItemWeapon->m_strName);
		m_ctlWeaponType.SetItemData (i + 1, pInfoItemWeapon->m_dwWeaponInfoID);
	}

	SetWeaponType (m_dwWeaponInfoID);
	OnCbnSelchangeWeapontype();

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemTypeNewARMS::PostNcDestroy						 */
/* ���e		:�I������														 */
/* ���t		:2007/09/30														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewARMS::PostNcDestroy()
{
	CDialog::PostNcDestroy ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemTypeNewARMS::OnOK									 */
/* ���e		:�{�^���n���h��(OK)												 */
/* ���t		:2007/12/08														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewARMS::OnOK()
{
	UpdateData ();

	if (m_pDlgWeaponType) {
		m_pDlgWeaponType->Get(m_pSrc);
	}
	m_dwWeaponInfoID = GetWeaponType ();

	CDlgAdminBase::EndDialog (IDOK);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemTypeNewARMS::SetWeaponType						 */
/* ���e		:�����ʂ�ݒ�													 */
/* ���t		:2007/12/09														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewARMS::SetWeaponType(DWORD dwWeaponInfoID)
{
	int i, nCount, nSelect;
	DWORD dwItemData;

	nSelect = 0;

	nCount = m_ctlWeaponType.GetCount ();
	for (i = 0; i < nCount; i ++) {
		dwItemData = m_ctlWeaponType.GetItemData (i);
		if (dwItemData != dwWeaponInfoID) {
			continue;
		}
		nSelect = i;
		break;
	}
	m_ctlWeaponType.SetCurSel (nSelect);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemTypeNewARMS::GetWeaponType						 */
/* ���e		:�����ʂ��擾													 */
/* ���t		:2007/12/09														 */
/* ========================================================================= */

DWORD CDlgAdminItemTypeNewARMS::GetWeaponType(void)
{
	int nSelect;
	DWORD dwRet;

	dwRet = 0;

	nSelect = m_ctlWeaponType.GetCurSel ();
	if (nSelect < 0) {
		goto Exit;
	}

	dwRet = m_ctlWeaponType.GetItemData (nSelect);
Exit:
	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemTypeNewARMS::OnCbnSelchangeWeapontype				 */
/* ���e		:�C�x���g�n���h��(CBN_SELCHANGE)								 */
/* ���t		:2010/01/05														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewARMS::OnCbnSelchangeWeapontype()
{
	if (m_pDlgWeaponType) {
		m_pDlgWeaponType->DestroyWindow();
		m_pDlgWeaponType = NULL;
	}

	DWORD dwWeaponInfoID = GetWeaponType();
	PCLibInfoItemWeapon pLibInfoItemWeapon = m_pMgrData->GetLibInfoItemWeapon ();
	PCInfoItemWeapon pInfoItemWeapon = (PCInfoItemWeapon)pLibInfoItemWeapon->GetPtr(dwWeaponInfoID);
	if (pInfoItemWeapon == NULL) {
		return;
	}

	if (pInfoItemWeapon->m_dwMotionType & INFOITEMARMS_MOTION_BOW) {
		m_pDlgWeaponType = new CDlgAdminItemTypeNewARMSBow(this);
		m_pDlgWeaponType->Create(CDlgAdminItemTypeNewARMSBow::IDD, this);
		m_pDlgWeaponType->Set(m_pSrc);
	}

	if (m_pDlgWeaponType == NULL) {
		return;
	}
	CRect rcTmp;
	GetDlgItem(IDC_DLGSUB)->GetWindowRect(rcTmp);
	ScreenToClient(rcTmp);
	m_pDlgWeaponType->ShowWindow(SW_SHOW);
	m_pDlgWeaponType->MoveWindow(&rcTmp);
}

/* Copyright(C)URARA-works 2007 */
