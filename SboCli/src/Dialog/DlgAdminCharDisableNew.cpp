/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharDisableNew.cpp									 */
/* ���e			:����MAC�A�h���X�̒ǉ��_�C�A���O�N���X �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/04/11													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "DlgAdminCharDisableNew.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminCharDisableNew::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharDisableNew)
	DDX_Text(pDX, IDC_MACADDRESS, m_strMacAddress);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharDisableNew, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharDisableNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminCharDisableNew::CDlgAdminCharDisableNew				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2009/04/11														 */
/* ========================================================================= */

CDlgAdminCharDisableNew::CDlgAdminCharDisableNew(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminCharDisableNew::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharDisableNew)
	m_strMacAddress = _T("");
	//}}AFX_DATA_INIT
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharDisableNew::~CDlgAdminCharDisableNew				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2009/04/11														 */
/* ========================================================================= */

CDlgAdminCharDisableNew::~CDlgAdminCharDisableNew()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharDisableNew::PostNcDestroy							 */
/* ���e		:�I������														 */
/* ���t		:2009/04/11														 */
/* ========================================================================= */

void CDlgAdminCharDisableNew::PostNcDestroy()
{
	CDialog::PostNcDestroy ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharDisableNew::OnOK									 */
/* ���e		:�{�^���n���h��(OK)												 */
/* ���t		:2009/04/11														 */
/* ========================================================================= */

void CDlgAdminCharDisableNew::OnOK()
{
	UpdateData ();

	CDlgAdminBase::EndDialog (IDOK);
}

/* Copyright(C)URARA-works 2009 */
