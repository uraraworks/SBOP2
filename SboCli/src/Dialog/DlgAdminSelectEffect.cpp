/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminSelectEffect.cpp									 */
/* ���e			:�G�t�F�N�g�I���_�C�A���O�N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/08/11													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "MgrData.h"
#include "DlgAdminSelectEffect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminSelectEffect::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminSelectEffect)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminSelectEffect, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminSelectEffect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminSelectEffect::CDlgAdminSelectEffect					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

CDlgAdminSelectEffect::CDlgAdminSelectEffect(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminSelectEffect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminSelectEffect)
	//}}AFX_DATA_INIT

	m_dwSelectID = 0;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminSelectEffect::~CDlgAdminSelectEffect					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

CDlgAdminSelectEffect::~CDlgAdminSelectEffect()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminSelectEffect::GetSelectID								 */
/* ���e		:�I�����ꂽ�G�t�F�N�gID���擾									 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

DWORD CDlgAdminSelectEffect::GetSelectID()
{
	return m_dwSelectID;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminSelectEffect::OnInitDialog							 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

BOOL CDlgAdminSelectEffect::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	m_List.Create (this, m_pMgrData);

	RegisterControl (IDC_LIST,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);
	RegisterControl (IDOK,		LH_CTRL_X | LH_CTRL_Y);
	RegisterControl (IDCANCEL,	LH_CTRL_X | LH_CTRL_Y);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminSelectEffect::PostNcDestroy							 */
/* ���e		:�I������														 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

void CDlgAdminSelectEffect::PostNcDestroy()
{
	CDialog::PostNcDestroy ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminSelectEffect::OnOK									 */
/* ���e		:�{�^���n���h��(OK)												 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

void CDlgAdminSelectEffect::OnOK()
{
	int nNo;

	nNo = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nNo < 0) {
		goto Exit;
	}

	m_dwSelectID = m_List.GetItemData (nNo);

Exit:
	CDlgAdminBase::EndDialog (IDOK);
}

/* Copyright(C)URARA-works 2008 */
