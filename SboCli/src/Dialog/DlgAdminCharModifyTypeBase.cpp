/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharModifyTypeBase.cpp								 */
/* ���e			:�L������� �ړ���ʂ̐ݒ�_�C�A���O�N���X �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/07/12													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoCharCli.h"
#include "MgrData.h"
#include "DlgAdminCharModifyTypeBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminCharModifyTypeBase::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharModifyTypeBase)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharModifyTypeBase, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharModifyTypeBase)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminCharModifyTypeBase::CDlgAdminCharModifyTypeBase		 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

CDlgAdminCharModifyTypeBase::CDlgAdminCharModifyTypeBase(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminCharModifyTypeBase::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharModifyTypeBase)
	//}}AFX_DATA_INIT
}
CDlgAdminCharModifyTypeBase::CDlgAdminCharModifyTypeBase(int nResourceID, CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(nResourceID, pParent)
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharModifyTypeBase::~CDlgAdminCharModifyTypeBase		 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

CDlgAdminCharModifyTypeBase::~CDlgAdminCharModifyTypeBase()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharModifyTypeBase::Set								 */
/* ���e		:�ݒ�															 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

void CDlgAdminCharModifyTypeBase::Set(CInfoCharCli *pSrc)
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharModifyTypeBase::Get								 */
/* ���e		:�擾															 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

void CDlgAdminCharModifyTypeBase::Get(CInfoCharCli *pDst)
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharModifyTypeBase::PostNcDestroy						 */
/* ���e		:�I������														 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

void CDlgAdminCharModifyTypeBase::PostNcDestroy()
{
	CDialog::PostNcDestroy ();
}

/* Copyright(C)URARA-works 2008 */
