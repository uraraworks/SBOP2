/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharSkillBase.cpp									 */
/* ���e			:�X�L���ҏW�_�C�A���O�N���X �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/07													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoSkillBase.h"
#include "LibInfoSkill.h"
#include "DlgAdminCharSkillNONE.h"
#include "MgrData.h"
#include "DlgAdminCharSkillBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminCharSkillBase::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharSkillBase)
	DDX_Control(pDX, IDC_TYPE, m_ctlType);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_SP, m_dwSP);
}

BEGIN_MESSAGE_MAP(CDlgAdminCharSkillBase, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharSkillBase)
	ON_CBN_SELCHANGE(IDC_TYPE, OnSelchangeType)
	ON_MESSAGE(WM_ADMINMSG, OnAdminMsg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillBase::CDlgAdminCharSkillBase					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

CDlgAdminCharSkillBase::CDlgAdminCharSkillBase(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminCharSkillBase::IDD, pParent)
	, m_strName(_T(""))
	, m_dwSP(0)
{
	//{{AFX_DATA_INIT(CDlgAdminCharSkillBase)
	//}}AFX_DATA_INIT

	m_nType	= -1;
	m_bModeModify	= FALSE;
	m_pDlgType		= NULL;
	m_pInfo			= NULL;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillBase::~CDlgAdminCharSkillBase				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

CDlgAdminCharSkillBase::~CDlgAdminCharSkillBase()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillBase::Init									 */
/* ���e		:������															 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillBase::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillBase::Get									 */
/* ���e		:�ҏW���e���擾													 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillBase::Get(CInfoSkillBase *&pDst)
{
	CLibInfoSkill LibInfo;

	if (m_pInfo == NULL) {
		return;
	}

	SAFE_DELETE (pDst);
	pDst = (PCInfoSkillBase)LibInfo.GetNew (m_pInfo->m_nType);
	pDst->Copy (m_pInfo);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillBase::SetModify								 */
/* ���e		:�ҏW���[�h�Ƃ��Đݒ�											 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillBase::SetModify(CInfoSkillBase *pSrc)
{
	CLibInfoSkill LibInfo;

	SAFE_DELETE (m_pInfo);
	m_pInfo = (PCInfoSkillBase)LibInfo.GetNew (pSrc->m_nType);
	m_pInfo->Copy (pSrc);

	m_strName	= m_pInfo->m_strName;
	m_dwSP		= m_pInfo->m_dwSP;
	m_nType		= m_pInfo->m_nType;

	m_bModeModify = TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillBase::OnInitDialog							 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

BOOL CDlgAdminCharSkillBase::OnInitDialog()
{
	int i, nNo;

	CDlgAdminBase::OnInitDialog();

	if (m_bModeModify) {
		SetWindowText ("�X�L���̕ҏW");
	}

	m_ctlType.InsertString (0, "���ݒ�");
	m_ctlType.SetItemData (0, SKILLTYPE_NONE);
	m_ctlType.InsertString (1, "�ނ�");
	m_ctlType.SetItemData (1, SKILLTYPE_FISHING);

	nNo = 0;

	if (m_pInfo) {
		for (i = 0; i < SKILLTYPE_MAX; i ++) {
			if (m_pInfo->m_nType == m_ctlType.GetItemData (i)) {
				nNo = i;
				break;
			}
		}
	}

	m_ctlType.SetCurSel (nNo);

	OnSelchangeType ();
	if (m_pDlgType && m_pInfo) {
		m_pDlgType->Set (m_pInfo);
	}

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnAdminMsg										 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2007/03/18														 */
/* ========================================================================= */

LRESULT CDlgAdminCharSkillBase::OnAdminMsg(WPARAM wParam, LPARAM lParam)
{
	switch (wParam) {
	case ADMINMSG_NOTIFYTYPE_LBUTTONDOWN:		/* ���N���b�N�ʒm */
//		m_nPosX = HIWORD (lParam);
//		m_nPosY = LOWORD (lParam);
		UpdateData (FALSE);
		break;
	case ADMINMSG_NOTIFYTYPE_RBUTTONDOWN:		/* �E�N���b�N�ʒm */
		if (m_pDlgType == NULL) {
			break;
		}
		m_pDlgType->PostMessage (WM_ADMINMSG, wParam, lParam);
		break;
	}

	return -1;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillBase::PostNcDestroy							 */
/* ���e		:�I������														 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillBase::PostNcDestroy()
{
	CDialog::PostNcDestroy ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillBase::OnSelchangeType						 */
/* ���e		:�C�x���g�n���h��(CBN_SELCHANGE)								 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillBase::OnSelchangeType()
{
	int nNo, nType;
	CRect rc;
	PCInfoSkillBase pInfoTmp;
	CLibInfoSkill LibInfo;

	nNo = m_ctlType.GetCurSel ();
	nType = m_ctlType.GetItemData (nNo);

	if (m_pDlgType) {
		m_pDlgType->DestroyWindow ();
		m_pDlgType = NULL;
	}

	switch (nType) {
	case SKILLTYPE_FISHING:			/* �ނ� */
		m_pDlgType = new CDlgAdminCharSkillNONE(this);
		break;
	default:
		m_pDlgType = new CDlgAdminCharSkillNONE(this);
		break;
	}
	if (m_pDlgType) {
		m_pDlgType->Init (m_pMgrData);
		GetDlgItem (IDC_FRAME)->GetWindowRect (rc);
		ScreenToClient (rc);
		m_pDlgType->MoveWindow (rc.left, rc.top, rc.Width (), rc.Height ());
	}

	if (m_nType != nType) {
		pInfoTmp = (PCInfoSkillBase)LibInfo.GetNew (nType);
		if (m_pInfo) {
			pInfoTmp->m_dwSkillID	= m_pInfo->m_dwSkillID;
			pInfoTmp->m_dwSP		= m_pInfo->m_dwSP;
		}
		pInfoTmp->m_nType = nType;

		SAFE_DELETE (m_pInfo);
		m_pInfo = pInfoTmp;
	}
}

/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillBase::OnOK									 */
/* ���e		:�{�^���n���h��(OK)												 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillBase::OnOK()
{
	UpdateData ();

	if (m_pInfo) {
		m_pInfo->m_strName	= m_strName;
		m_pInfo->m_dwSP		= m_dwSP;
		if (m_pDlgType) {
			m_pDlgType->Get (m_pInfo);
		}
	}

	CDlgAdminBase::EndDialog (IDOK);
}

/* Copyright(C)URARA-works 2008 */
