/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminTalkEventSet.cpp									 */
/* ���e			:��b�C�x���g�ݒ�_�C�A���O�N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/23													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoTalkEvent.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminTalkEventTab.h"
#include "DlgAdminTalkEventSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminTalkEventSet::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTalkEventSet)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_TAB, m_Tab);
}

BEGIN_MESSAGE_MAP(CDlgAdminTalkEventSet, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminTalkEventSet)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CDlgAdminTalkEventSet::OnTcnSelchangeTab)
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventSet::CDlgAdminTalkEventSet					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventSet::CDlgAdminTalkEventSet(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminTalkEventSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminTalkEventSet)
	//}}AFX_DATA_INIT

	m_pInfo		= NULL;
	m_pDlgTab	= NULL;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventSet::~CDlgAdminTalkEventSet					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventSet::~CDlgAdminTalkEventSet()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventSet::Init									 */
/* ���e		:������															 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventSet::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	m_pInfo = new CInfoTalkEvent;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventSet::OnAdminMsg								 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventSet::OnAdminMsg(int nType, DWORD dwPara)
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventSet::OnInitDialog							 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

BOOL CDlgAdminTalkEventSet::OnInitDialog()
{
	CRect rc;

	CDlgAdminBase::OnInitDialog();

	AddPage ();
	m_Tab.GetWindowRect (rc);
	m_Tab.AdjustRect (FALSE, rc);
	ScreenToClient (rc);

	m_pDlgTab = new CDlgAdminTalkEventTab(this);
	m_pDlgTab->Init (m_pMgrData);
	m_pDlgTab->MoveWindow (rc.left, rc.top, rc.Width (), rc.Height ());

	RegisterControl (IDC_ADD,	LH_CTRL_X);
	RegisterControl (IDC_DEL,	LH_CTRL_X);
	RegisterControl (IDC_TAB,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);
	RegisterControl (m_pDlgTab->m_hWnd,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);
	RegisterControl (IDOK,		LH_CTRL_X | LH_CTRL_Y);
	RegisterControl (IDCANCEL,	LH_CTRL_X | LH_CTRL_Y);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventSet::AddPage									 */
/* ���e		:�C�x���g�n���h��(TCN_SELCHANGE)								 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventSet::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventSet::PostNcDestroy							 */
/* ���e		:�I������														 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventSet::PostNcDestroy()
{
	CDialog::PostNcDestroy ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventSet::AddPage									 */
/* ���e		:�y�[�W�̒ǉ�													 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventSet::AddPage()
{
	int nCount;
	PCInfoTalkEventBase pInfo;
	CString strTmp;

	nCount = m_pInfo->GetPageCount ();
	strTmp.Format ("�y�[�W%d", nCount + 1);
	m_Tab.InsertItem(nCount, strTmp);

	pInfo = m_pInfo->GetNew (TALKEVENTTYPE_NONE);
	pInfo->m_nPage = nCount;
	m_pInfo->AddTalkEvent(pInfo);
}

/* Copyright(C)URARA-works 2008 */
