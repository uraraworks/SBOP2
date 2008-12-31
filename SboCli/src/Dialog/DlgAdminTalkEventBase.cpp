/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminTalkEventBase.cpp									 */
/* ���e			:��b�C�x���g�̐ݒ�_�C�A���O�N���X �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/23													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "InfoTalkEvent.h"
#include "DlgAdminTalkEventNONE.h"
#include "DlgAdminTalkEventPAGE.h"
#include "DlgAdminTalkEventMSG.h"
#include "DlgAdminTalkEventMENU.h"
#include "DlgAdminTalkEventADDSKILL.h"
#include "DlgAdminTalkEventBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminTalkEventBase::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTalkEventBase)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_TYPE, m_Type);
}

BEGIN_MESSAGE_MAP(CDlgAdminTalkEventBase, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminTalkEventBase)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_TYPE, &CDlgAdminTalkEventBase::OnCbnSelchangeType)
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventBase::CDlgAdminTalkEventBase					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventBase::CDlgAdminTalkEventBase(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminTalkEventBase::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminTalkEventBase)
	//}}AFX_DATA_INIT

	m_pDlgType	= NULL;
	m_pInfo		= NULL;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventBase::~CDlgAdminTalkEventBase				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventBase::~CDlgAdminTalkEventBase()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventBase::Init									 */
/* ���e		:������															 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventBase::Init(CMgrData *pMgrData, int nPage, CInfoTalkEventBase *pInfo/*NULL*/)
{
	int nType;
	CInfoTalkEvent InfoTalkEvent;

	CDlgAdminBase::Init (pMgrData);

	m_nPageCount = nPage;
	nType	= (pInfo) ? pInfo->m_nEventType : TALKEVENTTYPE_NONE;
	m_pInfo	= InfoTalkEvent.GetNew (nType);
	if (pInfo) {
		m_pInfo->Copy (pInfo);
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventBase::Get									 */
/* ���e		:�擾															 */
/* ���t		:2008/12/26														 */
/* ========================================================================= */

void CDlgAdminTalkEventBase::Get(CInfoTalkEventBase *&pDst)
{
	CInfoTalkEvent InfoTalkEvent;

	SAFE_DELETE (pDst);

	pDst = InfoTalkEvent.GetNew (m_pInfo->m_nEventType);
	pDst->Copy (m_pInfo);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventBase::OnAdminMsg								 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventBase::OnAdminMsg(int nType, DWORD dwPara)
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventBase::OnInitDialog							 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

BOOL CDlgAdminTalkEventBase::OnInitDialog()
{
	int i, nCount, nNo;

	CDlgAdminBase::OnInitDialog();

	m_Type.InsertString (0, "���ݒ�");
	m_Type.SetItemData (0, TALKEVENTTYPE_NONE);
	m_Type.InsertString (1, "�y�[�W�؂�ւ�");
	m_Type.SetItemData (1, TALKEVENTTYPE_PAGE);
	m_Type.InsertString (2, "���b�Z�[�W�\��");
	m_Type.SetItemData (2, TALKEVENTTYPE_MSG);
	m_Type.InsertString (3, "���ڑI��");
	m_Type.SetItemData (3, TALKEVENTTYPE_MENU);
	m_Type.InsertString (4, "�X�L���ǉ�");
	m_Type.SetItemData (4, TALKEVENTTYPE_ADDSKILL);

	nNo = 0;
	nCount = m_Type.GetCount ();
	for (i = 0; i < nCount; i ++) {
		if (m_pInfo->m_nEventType == m_Type.GetItemData (i)) {
			nNo = i;
			break;
		}
	}
	m_Type.SetCurSel (nNo);

	OnCbnSelchangeType ();

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventBase::OnCbnSelchangeType						 */
/* ���e		:�C�x���g�n���h��(CBN_SELCHANGE)								 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventBase::OnCbnSelchangeType()
{
	int nNo, nType;
	CRect rc;
	PCInfoTalkEventBase pInfoTmp;
	CInfoTalkEvent InfoTalkEvent;

	nNo		= m_Type.GetCurSel ();
	nType	= m_Type.GetItemData (nNo);

	if (m_pDlgType) {
		m_pDlgType->DestroyWindow ();
		m_pDlgType = NULL;
	}
	switch (nType) {
	case TALKEVENTTYPE_PAGE:			/* �y�[�W�؂�ւ� */
		m_pDlgType = new CDlgAdminTalkEventPAGE(this);
		break;
	case TALKEVENTTYPE_MSG:				/* ���b�Z�[�W�\�� */
		m_pDlgType = new CDlgAdminTalkEventMSG(this);
		break;
	case TALKEVENTTYPE_MENU:			/* ���ڑI�� */
		m_pDlgType = new CDlgAdminTalkEventMENU(this);
		break;
	case TALKEVENTTYPE_ADDSKILL:		/* �X�L���ǉ� */
		m_pDlgType = new CDlgAdminTalkEventADDSKILL(this);
		break;
	default:
		m_pDlgType = new CDlgAdminTalkEventNONE(this);
		break;
	}
	/* ��ʂ��ς�����H */
	if (m_pInfo->m_nEventType != nType) {
		pInfoTmp = InfoTalkEvent.GetNew (nType);
		pInfoTmp->m_nEventType = nType;
		SAFE_DELETE (m_pInfo);
		m_pInfo = pInfoTmp;
	}

	GetDlgItem (IDC_FRAME)->GetWindowRect (rc);
	ScreenToClient (rc);

	m_pDlgType->Init (m_pMgrData, m_nPageCount);
	m_pDlgType->MoveWindow (rc.left, rc.top, rc.Width (), rc.Height ());
	m_pDlgType->Set (m_pInfo);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventBase::PostNcDestroy							 */
/* ���e		:�I������														 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventBase::PostNcDestroy()
{
	/* ���[�h���X�_�C�A���O���͂��̊֐������� */
	CDialog::PostNcDestroy ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventBase::OnOK									 */
/* ���e		:�{�^���n���h��(OK)												 */
/* ���t		:2008/12/25														 */
/* ========================================================================= */

void CDlgAdminTalkEventBase::OnOK()
{
	UpdateData ();

	if (m_pDlgType) {
		m_pDlgType->Get (m_pInfo);
	}

	CDlgAdminBase::OnOK ();
}

/* Copyright(C)URARA-works 2008 */
