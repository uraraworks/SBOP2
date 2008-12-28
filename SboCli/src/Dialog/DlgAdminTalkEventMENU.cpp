/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminTalkEventMENU.cpp									 */
/* ���e			:��b�C�x���g�ݒ�(���ڑI��)�_�C�A���O�N���X �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/23													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoTalkEventMENU.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminTalkEventMENUSet.h"
#include "DlgAdminTalkEventMENU.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminTalkEventMENU::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminTalkEventNONE::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTalkEventMENU)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST, m_List);
}

BEGIN_MESSAGE_MAP(CDlgAdminTalkEventMENU, CDlgAdminTalkEventNONE)
	//{{AFX_MSG_MAP(CDlgAdminTalkEventMENU)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ADD, &CDlgAdminTalkEventMENU::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_MODIFY, &CDlgAdminTalkEventMENU::OnBnClickedModify)
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventMENU::CDlgAdminTalkEventMENU					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventMENU::CDlgAdminTalkEventMENU(CWnd* pParent /*=NULL*/)
	: CDlgAdminTalkEventNONE(pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminTalkEventMENU)
	//}}AFX_DATA_INIT

	m_nResourceID = CDlgAdminTalkEventMENU::IDD;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventMENU::~CDlgAdminTalkEventMENU				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventMENU::~CDlgAdminTalkEventMENU()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventMENU::Set									 */
/* ���e		:�ݒ肩���ʂɔ��f												 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

void CDlgAdminTalkEventMENU::Set(CInfoTalkEventBase *pSrc)
{
	int i, nCount;
	PCInfoTalkEventMENU pInfo;
	PSTTALKEVENTMENUINFO pMenuInfo;
	CString strTmp;

	m_List.DeleteAllItems ();

	pInfo = (PCInfoTalkEventMENU)pSrc;
	nCount = pInfo->GetMenuInfoCount ();
	for (i = 0; i < nCount; i ++) {
		pMenuInfo = pInfo->GetPtr (i);
		m_List.InsertItem (i, pMenuInfo->strName);
		strTmp.Format ("%d", pMenuInfo->nPage + 1);
		m_List.SetItemText (i, 1, strTmp);
	}

	UpdateData (FALSE);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventMENU::Get									 */
/* ���e		:��ʂ���ݒ�ɔ��f												 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

void CDlgAdminTalkEventMENU::Get(CInfoTalkEventBase *pDst)
{
	int i, nCount;
	PCInfoTalkEventMENU pInfo;

	pInfo = (PCInfoTalkEventMENU)pDst;
	pInfo->DeleteAllMenuInfo ();

	UpdateData ();

	nCount = m_List.GetItemCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo->AddMenuInfo (
				atoi (m_List.GetItemText (i, 1)) - 1,
				m_List.GetItemText (i, 0));
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventMENU::OnInitDialog							 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

BOOL CDlgAdminTalkEventMENU::OnInitDialog()
{
	CDlgAdminTalkEventNONE::OnInitDialog();

	RegisterControl (IDC_LIST,		LH_CTRL_WIDTH | LH_CTRL_HEIGHT);
	RegisterControl (IDC_ADD,		LH_CTRL_Y);
	RegisterControl (IDC_MODIFY,	LH_CTRL_Y);
	RegisterControl (IDC_DEL,		LH_CTRL_Y);

	m_List.SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn (0, "���ږ�", 0, 150);
	m_List.InsertColumn (1, "�W�����v��", 0, 150);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventMENU::OnBnClickedAdd							 */
/* ���e		:�{�^���n���h��(�ǉ�)											 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

void CDlgAdminTalkEventMENU::OnBnClickedAdd()
{
	int nResult, nCount;
	CString strTmp;
	CDlgAdminTalkEventMENUSet Dlg(this);

	Dlg.Init (m_pMgrData, m_nPageCount);
	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		return;
	}

	nCount = m_List.GetItemCount ();
	m_List.InsertItem (nCount, Dlg.m_strName);
	strTmp.Format ("%d", Dlg.m_nPage + 1);
	m_List.SetItemText (nCount, 1, strTmp);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventMENU::OnBnClickedModify						 */
/* ���e		:�{�^���n���h��(�ҏW)											 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

void CDlgAdminTalkEventMENU::OnBnClickedModify()
{
	int nResult, nSelect;
	CString strTmp;
	CDlgAdminTalkEventMENUSet Dlg(this);

	nSelect = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nSelect < 0) {
		return;
	}

	Dlg.Init (m_pMgrData, m_nPageCount);
	Dlg.m_strName	= m_List.GetItemText (nSelect, 0);
	Dlg.m_nPage		= atoi (m_List.GetItemText (nSelect, 1));
	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		return;
	}

	m_List.SetItemText (nSelect, 0, Dlg.m_strName);
	strTmp.Format ("%d", Dlg.m_nPage + 1);
	m_List.SetItemText (nSelect, 1, strTmp);
}

/* Copyright(C)URARA-works 2008 */
