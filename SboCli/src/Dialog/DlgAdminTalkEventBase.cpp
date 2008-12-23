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
#include "InfoTalkEventBase.h"
#include "DlgAdminTalkEventNONE.h"
#include "DlgAdminTalkEventPAGE.h"
#include "DlgAdminTalkEventMSG.h"
#include "DlgAdminTalkEventMENU.h"
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

	m_pDlgType = NULL;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventBase::~CDlgAdminTalkEventBase				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventBase::~CDlgAdminTalkEventBase()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventBase::Init									 */
/* ���e		:������															 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventBase::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);
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
	CDlgAdminBase::OnInitDialog();

	m_Type.InsertString (0, "���ݒ�");
	m_Type.SetItemData (0, TALKEVENTTYPE_NONE);
	m_Type.InsertString (1, "�y�[�W�؂�ւ�");
	m_Type.SetItemData (1, TALKEVENTTYPE_PAGE);
	m_Type.InsertString (2, "���b�Z�[�W�\��");
	m_Type.SetItemData (2, TALKEVENTTYPE_MSG);
	m_Type.InsertString (3, "���ڑI��");
	m_Type.SetItemData (3, TALKEVENTTYPE_MENU);

	m_Type.SetCurSel (0);

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
//	PCInfoMapEventBase pInfoTmp;
//	CLibInfoMapEvent LibInfo;

	nNo = m_Type.GetCurSel ();
	nType = m_Type.GetItemData (nNo);

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
	default:
		m_pDlgType = new CDlgAdminTalkEventNONE(this);
		break;
	}
	if (m_pDlgType) {
		m_pDlgType->Init (m_pMgrData);
		GetDlgItem (IDC_FRAME)->GetWindowRect (rc);
		ScreenToClient (rc);
		m_pDlgType->MoveWindow (rc.left, rc.top, rc.Width (), rc.Height ());
	}
#if 0
	if (m_nEventType != nEventType) {
		pInfoTmp = (PCInfoMapEventBase)LibInfo.GetNew (nEventType);
		if (m_pInfo) {
			pInfoTmp->m_dwMapEventID	= m_pInfo->m_dwMapEventID;
			pInfoTmp->m_ptPos			= m_pInfo->m_ptPos;
		}
		pInfoTmp->m_nType = nEventType;

		SAFE_DELETE (m_pInfo);
		m_pInfo = pInfoTmp;
	}
#endif
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

/* Copyright(C)URARA-works 2008 */
