/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapObjectEdit.cpp									 */
/* ���e			:�}�b�v�p�[�c�ҏW�_�C�A���O�N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/09/07													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "Img32.h"
#include "MgrGrpData.h"
#include "MgrData.h"
#include "DlgAdminMapObjectEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�ݒ�																 */
/* ========================================================================= */

void CDlgAdminMapObjectEdit::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminMapObjectEdit)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminMapObjectEdit, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminMapObjectEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminMapObjectEdit::CDlgAdminMapObjectEdit					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/09/07														 */
/* ========================================================================= */

CDlgAdminMapObjectEdit::CDlgAdminMapObjectEdit(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminMapObjectEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminMapObjectEdit)
	//}}AFX_DATA_INIT
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapObjectEdit::~CDlgAdminMapObjectEdit				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/09/07														 */
/* ========================================================================= */

CDlgAdminMapObjectEdit::~CDlgAdminMapObjectEdit()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapObjectEdit::DoModal								 */
/* ���e		:���[�_���_�C�A���O�\��											 */
/* ���t		:2008/09/07														 */
/* ========================================================================= */

int CDlgAdminMapObjectEdit::DoModal(CMgrData *pMgrData)
{
	m_pMgrData		= pMgrData;

	return CDlgAdminBase::DoModal ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapObjectEdit::OnInitDialog							 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/09/07														 */
/* ========================================================================= */

BOOL CDlgAdminMapObjectEdit::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapObjectEdit::OnOK									 */
/* ���e		:�{�^���n���h��(OK)												 */
/* ���t		:2008/09/07														 */
/* ========================================================================= */

void CDlgAdminMapObjectEdit::OnOK()
{
	CDlgAdminBase::OnOK();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapObjectEdit::PostNcDestroy							 */
/* ���e		:�I������														 */
/* ���t		:2008/09/07														 */
/* ========================================================================= */

void CDlgAdminMapObjectEdit::PostNcDestroy()
{
}

/* Copyright(C)URARA-works 2008 */
