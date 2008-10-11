/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapEventNONE.cpp									 */
/* ���e			:�}�b�v�C�x���g(���ݒ�)�_�C�A���O�N���X �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/26													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoMapEventBase.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminMapEventNONE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminMapEventNONE::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminMapEventNONE)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminMapEventNONE, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminMapEventNONE)
	ON_MESSAGE(WM_ADMINMSG, OnAdminMsg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventNONE::CDlgAdminMapEventNONE					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/06/26														 */
/* ========================================================================= */

CDlgAdminMapEventNONE::CDlgAdminMapEventNONE(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminMapEventNONE::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminMapEventNONE)
	//}}AFX_DATA_INIT

	m_nResourceID = CDlgAdminMapEventNONE::IDD;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventNONE::~CDlgAdminMapEventNONE					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/06/26														 */
/* ========================================================================= */

CDlgAdminMapEventNONE::~CDlgAdminMapEventNONE()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventNONE::Init									 */
/* ���e		:������															 */
/* ���t		:2008/06/26														 */
/* ========================================================================= */

void CDlgAdminMapEventNONE::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	/* �E�B���h�E�쐬 */
	Create (m_nResourceID, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventNONE::Set										 */
/* ���e		:�ݒ�															 */
/* ���t		:2008/06/26														 */
/* ========================================================================= */

void CDlgAdminMapEventNONE::Set(CInfoMapEventBase *pSrc)
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventNONE::Get										 */
/* ���e		:�擾															 */
/* ���t		:2008/06/26														 */
/* ========================================================================= */

void CDlgAdminMapEventNONE::Get(CInfoMapEventBase *pDst)
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventNONE::OnInitDialog							 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/06/26														 */
/* ========================================================================= */

BOOL CDlgAdminMapEventNONE::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventNONE::OnAdminMsg								 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2008/06/28														 */
/* ========================================================================= */

LRESULT CDlgAdminMapEventNONE::OnAdminMsg(WPARAM wParam, LPARAM lParam)
{
	return -1;
}

/* Copyright(C)URARA-works 2008 */
