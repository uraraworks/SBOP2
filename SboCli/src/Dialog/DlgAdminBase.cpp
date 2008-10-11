/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminBase.h												 */
/* ���e			:�Ǘ��҃E�B���h�E���_�C�A���O�N���X �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/03/18													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "MgrData.h"
#include "DlgAdminBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminBase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminBase)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminBase, CDialog)
	//{{AFX_MSG_MAP(CDlgAdminBase)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminBase::CDlgAdminBase									 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/03/18														 */
/* ========================================================================= */

CDlgAdminBase::CDlgAdminBase(int nResourceID, CWnd* pParent /*=NULL*/)
	: CDialog(nResourceID, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminBase)
	//}}AFX_DATA_INIT

	m_pMgrData		= NULL;
	m_pWndParent	= pParent;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminBase::~CDlgAdminBase									 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/03/18														 */
/* ========================================================================= */

CDlgAdminBase::~CDlgAdminBase()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminBase::Init											 */
/* ���e		:������															 */
/* ���t		:2007/03/18														 */
/* ========================================================================= */

void CDlgAdminBase::Init(CMgrData *pMgrData)
{
	m_pMgrData	= pMgrData;
	m_pSock		= m_pMgrData->GetUraraSockTCP ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminBase::Renew											 */
/* ���e		:�X�V															 */
/* ���t		:2007/03/18														 */
/* ========================================================================= */

void CDlgAdminBase::Renew(void)
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminBase::OnAdminMsg										 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2007/03/18														 */
/* ========================================================================= */

void CDlgAdminBase::OnAdminMsg(int nType, DWORD dwPara)
{
	switch (nType) {
	case ADMINMSG_ACCOUNTINFO:		/* �A�J�E���g���擾 */
		m_pMgrData->Delete (dwPara);
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminBase::OnMainFrame										 */
/* ���e		:���b�Z�[�W�n���h��(WM_MAINFRAME)								 */
/* ���t		:2007/08/16														 */
/* ========================================================================= */

void CDlgAdminBase::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminBase::OnInitDialog									 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2007/03/18														 */
/* ========================================================================= */

BOOL CDlgAdminBase::OnInitDialog()
{
	CDialog::OnInitDialog ();

	Initialize (m_hWnd);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminBase::OnSize											 */
/* ���e		:���b�Z�[�W�n���h��(WM_SIZE)									 */
/* ���t		:2007/03/18														 */
/* ========================================================================= */

void CDlgAdminBase::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize (nType, cx, cy);

	DoLayout (cx, cy);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminBase::PostNcDestroy									 */
/* ���e		:�I������														 */
/* ���t		:2007/03/18														 */
/* ========================================================================= */

void CDlgAdminBase::PostNcDestroy()
{
	delete this;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminBase::SelectCmb										 */
/* ���e		:���ڂ�I��														 */
/* ���t		:2007/11/25														 */
/* ========================================================================= */

void CDlgAdminBase::SelectCmb(CComboBox *pCmb, DWORD dwID)
{
	int i, nCount, nNo;
	DWORD dwItemData;

	nNo = 0;

	nCount = pCmb->GetCount ();
	for (i = 0; i < nCount; i ++) {
		dwItemData = pCmb->GetItemData (i);
		if (dwItemData != dwID) {
			continue;
		}
		nNo = i;
		break;
	}
	pCmb->SetCurSel (nNo);
}

/* Copyright(C)URARA-works 2007 */
