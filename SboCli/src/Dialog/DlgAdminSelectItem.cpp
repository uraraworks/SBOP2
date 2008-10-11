/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminSelectItem.cpp										 */
/* ���e			:�A�C�e���I���_�C�A���O�N���X �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/24													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoItem.h"
#include "MgrGrpData.h"
#include "MgrData.h"
#include "DlgAdminSelectItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminSelectItem::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminSelectItem)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminSelectItem, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminSelectItem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminSelectItem::CDlgAdminSelectItem						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/09/24														 */
/* ========================================================================= */

CDlgAdminSelectItem::CDlgAdminSelectItem(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminSelectItem::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminSelectItem)
	//}}AFX_DATA_INIT

	m_dwSelectItemID = 0;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminSelectItem::~CDlgAdminSelectItem						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/09/24														 */
/* ========================================================================= */

CDlgAdminSelectItem::~CDlgAdminSelectItem()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminSelectItem::GetSelectItemID							 */
/* ���e		:�I�����ꂽ�A�C�e��ID���擾										 */
/* ���t		:2007/09/24														 */
/* ========================================================================= */

DWORD CDlgAdminSelectItem::GetSelectItemID()
{
	return m_dwSelectItemID;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminSelectItem::OnInitDialog								 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2007/09/24														 */
/* ========================================================================= */

BOOL CDlgAdminSelectItem::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	m_List.Create (this, m_pMgrData);

	RegisterControl (IDC_LIST,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);
	RegisterControl (IDOK,		LH_CTRL_X | LH_CTRL_Y);
	RegisterControl (IDCANCEL,	LH_CTRL_X | LH_CTRL_Y);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminSelectItem::PostNcDestroy								 */
/* ���e		:�I������														 */
/* ���t		:2007/09/24														 */
/* ========================================================================= */

void CDlgAdminSelectItem::PostNcDestroy()
{
	CDialog::PostNcDestroy ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminSelectItem::OnOK										 */
/* ���e		:�{�^���n���h��(OK)												 */
/* ���t		:2007/09/24														 */
/* ========================================================================= */

void CDlgAdminSelectItem::OnOK()
{
	int nNo;

	nNo = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nNo < 0) {
		goto Exit;
	}

	m_dwSelectItemID = m_List.GetItemData (nNo);

Exit:
	CDlgAdminBase::EndDialog (IDOK);
}

/* Copyright(C)URARA-works 2007 */
