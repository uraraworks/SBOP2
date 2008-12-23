/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminTalkSet.cpp										 */
/* ���e			:��b�f�[�^�ݒ�_�C�A���O�N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/11/22													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "MgrData.h"
#include "LayoutHelper.h"
#include "DlgAdminTalkEventSet.h"
#include "DlgAdminTalkSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminTalkSet::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTalkSet)
	DDX_Text(pDX, IDC_TALK, m_strTalk);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminTalkSet, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminTalkSet)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_TALKEVENT, &CDlgAdminTalkSet::OnBnClickedTalkevent)
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkSet::CDlgAdminTalkSet								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/11/22														 */
/* ========================================================================= */

CDlgAdminTalkSet::CDlgAdminTalkSet(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminTalkSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminTalkSet)
	m_strTalk = _T("");
	//}}AFX_DATA_INIT
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkSet::~CDlgAdminTalkSet							 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/11/22														 */
/* ========================================================================= */

CDlgAdminTalkSet::~CDlgAdminTalkSet()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkSet::OnInitDialog									 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/11/23														 */
/* ========================================================================= */

BOOL CDlgAdminTalkSet::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	GetDlgItem (IDCANCEL)->SetFocus ();

	RegisterControl (IDC_TALK,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);
	RegisterControl (IDOK,		LH_CTRL_X | LH_CTRL_Y);
	RegisterControl (IDCANCEL,	LH_CTRL_X | LH_CTRL_Y);

	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkSet::PostNcDestroy								 */
/* ���e		:�I������														 */
/* ���t		:2008/11/22														 */
/* ========================================================================= */

void CDlgAdminTalkSet::PostNcDestroy()
{
	CDialog::PostNcDestroy ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkSet::OnOK											 */
/* ���e		:�{�^���n���h��(OK)												 */
/* ���t		:2008/11/22														 */
/* ========================================================================= */

void CDlgAdminTalkSet::OnOK()
{
	UpdateData ();

	CDialog::OnOK ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkSet::OnBnClickedTalkevent							 */
/* ���e		:�{�^���n���h��(��b�C�x���g�̐ݒ�)								 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkSet::OnBnClickedTalkevent()
{
	CDlgAdminTalkEventSet Dlg(this);

	Dlg.Init (m_pMgrData);
	Dlg.DoModal ();
}

/* Copyright(C)URARA-works 2008 */
