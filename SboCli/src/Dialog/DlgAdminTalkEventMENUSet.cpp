/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminTalkEventMENUSet.cpp								 */
/* ���e			:��b�C�x���g�I�����ڂ̐ݒ�_�C�A���O�N���X �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/28													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminTalkEventMENUSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminTalkEventMENUSet::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTalkEventMENUSet)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Control(pDX, IDC_COMBO, m_Combo);
}

BEGIN_MESSAGE_MAP(CDlgAdminTalkEventMENUSet, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminTalkEventMENUSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventMENUSet::CDlgAdminTalkEventMENUSet			 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

CDlgAdminTalkEventMENUSet::CDlgAdminTalkEventMENUSet(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminTalkEventMENUSet::IDD, pParent)
	, m_strName(_T(""))
{
	//{{AFX_DATA_INIT(CDlgAdminTalkEventMENUSet)
	//}}AFX_DATA_INIT

	m_nPage		 = 0;
	m_nPageCount = 0;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventMENUSet::~CDlgAdminTalkEventMENUSet			 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

CDlgAdminTalkEventMENUSet::~CDlgAdminTalkEventMENUSet()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventMENUSet::Init								 */
/* ���e		:������															 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

void CDlgAdminTalkEventMENUSet::Init(CMgrData *pMgrData, int nPage)
{
	CDlgAdminBase::Init (pMgrData);

	m_nPageCount = nPage;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventMENUSet::OnInitDialog						 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

BOOL CDlgAdminTalkEventMENUSet::OnInitDialog()
{
	int i;
	CString strTmp;

	CDlgAdminBase::OnInitDialog();

	for (i = 0; i < m_nPageCount; i ++) {
		strTmp.Format ("�y�[�W%d", i + 1);
		m_Combo.InsertString (i, strTmp);
	}
	m_Combo.SetCurSel (m_nPage);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventMENUSet::PostNcDestroy						 */
/* ���e		:�I������														 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

void CDlgAdminTalkEventMENUSet::PostNcDestroy()
{
	/* ���[�h���X�_�C�A���O���͂��̊֐������� */
	CDialog::PostNcDestroy ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventMENUSet::OnOK								 */
/* ���e		:�{�^���n���h��(OK)												 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

void CDlgAdminTalkEventMENUSet::OnOK()
{
	UpdateData ();

	m_nPage = m_Combo.GetCurSel ();

	CDlgAdminBase::OnOK ();
}

/* Copyright(C)URARA-works 2008 */
