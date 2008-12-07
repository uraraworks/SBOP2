/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharSkillNONE.cpp									 */
/* ���e			:�X�L��(���ݒ�)�_�C�A���O�N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/07													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoMapEventBase.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminCharSkillNONE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminCharSkillNONE::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharSkillNONE)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharSkillNONE, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharSkillNONE)
	ON_MESSAGE(WM_ADMINMSG, OnAdminMsg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillNONE::CDlgAdminCharSkillNONE					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

CDlgAdminCharSkillNONE::CDlgAdminCharSkillNONE(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminCharSkillNONE::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharSkillNONE)
	//}}AFX_DATA_INIT

	m_nResourceID = CDlgAdminCharSkillNONE::IDD;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillNONE::~CDlgAdminCharSkillNONE				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

CDlgAdminCharSkillNONE::~CDlgAdminCharSkillNONE()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillNONE::Init									 */
/* ���e		:������															 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillNONE::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	/* �E�B���h�E�쐬 */
	Create (m_nResourceID, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillNONE::Set									 */
/* ���e		:�ݒ�															 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillNONE::Set(CInfoSkillBase *pSrc)
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillNONE::Get									 */
/* ���e		:�擾															 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillNONE::Get(CInfoSkillBase *pDst)
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillNONE::OnInitDialog							 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

BOOL CDlgAdminCharSkillNONE::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillNONE::OnAdminMsg								 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2008/06/28														 */
/* ========================================================================= */

LRESULT CDlgAdminCharSkillNONE::OnAdminMsg(WPARAM wParam, LPARAM lParam)
{
	return -1;
}

/* Copyright(C)URARA-works 2008 */
