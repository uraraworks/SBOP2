/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminTalkEventTab.cpp									 */
/* ���e			:��b�C�x���g�^�u�_�C�A���O�N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/23													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminTalkEventBase.h"
#include "DlgAdminTalkEventTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminTalkEventTab::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTalkEventTab)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST, m_List);
}

BEGIN_MESSAGE_MAP(CDlgAdminTalkEventTab, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminTalkEventTab)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ADD, &CDlgAdminTalkEventTab::OnBnClickedAdd)
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventTab::CDlgAdminTalkEventTab					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventTab::CDlgAdminTalkEventTab(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminTalkEventTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminTalkEventTab)
	//}}AFX_DATA_INIT
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventTab::~CDlgAdminTalkEventTab					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventTab::~CDlgAdminTalkEventTab()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventTab::Init									 */
/* ���e		:������															 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventTab::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	/* �E�B���h�E�쐬 */
	Create (CDlgAdminTalkEventTab::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventTab::OnAdminMsg								 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventTab::OnAdminMsg(int nType, DWORD dwPara)
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventTab::OnInitDialog							 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

BOOL CDlgAdminTalkEventTab::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	RegisterControl (IDC_LIST,		LH_CTRL_WIDTH | LH_CTRL_HEIGHT);
	RegisterControl (IDC_ADD,		LH_CTRL_Y);
	RegisterControl (IDC_MODIFY,	LH_CTRL_Y);
	RegisterControl (IDC_DEL,		LH_CTRL_Y);
	RegisterControl (IDC_UP,		LH_CTRL_X | LH_CTRL_Y);
	RegisterControl (IDC_DOWN,		LH_CTRL_X | LH_CTRL_Y);

	m_List.SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn (0, "�C�x���g���", 0, 100);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventTab::OnBnClickedAdd							 */
/* ���e		:�{�^���n���h��(�ǉ�)											 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventTab::OnBnClickedAdd()
{
	int nResult;

	CDlgAdminTalkEventBase Dlg(this);

	Dlg.Init (m_pMgrData);
	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		return;
	}
}

/* Copyright(C)URARA-works 2008 */
