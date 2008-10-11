/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapEventMOVE.cpp									 */
/* ���e			:�}�b�v�C�x���g�ꗗ�_�C�A���O�N���X �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/24													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoMapEventMOVE.h"
#include "MgrData.h"
#include "DlgAdminMapEventMOVE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminMapEventMOVE::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminMapEventNONE::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminMapEventMOVE)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_POSX, m_nPosX);
	DDX_Text(pDX, IDC_POSY, m_nPosY);
}

BEGIN_MESSAGE_MAP(CDlgAdminMapEventMOVE, CDlgAdminMapEventNONE)
	//{{AFX_MSG_MAP(CDlgAdminMapEventMOVE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventMOVE::CDlgAdminMapEventMOVE					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/06/24														 */
/* ========================================================================= */

CDlgAdminMapEventMOVE::CDlgAdminMapEventMOVE(CWnd* pParent /*=NULL*/)
	: CDlgAdminMapEventNONE(pParent)
	, m_nPosX(0)
	, m_nPosY(0)
{
	//{{AFX_DATA_INIT(CDlgAdminMapEventMOVE)
	//}}AFX_DATA_INIT

	m_nResourceID = CDlgAdminMapEventMOVE::IDD;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventMOVE::~CDlgAdminMapEventMOVE					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/06/24														 */
/* ========================================================================= */

CDlgAdminMapEventMOVE::~CDlgAdminMapEventMOVE()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventMOVE::Set										 */
/* ���e		:�ݒ�															 */
/* ���t		:2008/06/26														 */
/* ========================================================================= */

void CDlgAdminMapEventMOVE::Set(CInfoMapEventBase *pSrc)
{
	PCInfoMapEventMOVE pSrcTmp = (PCInfoMapEventMOVE)pSrc;

	m_nPosX = pSrcTmp->m_ptDst.x;
	m_nPosY = pSrcTmp->m_ptDst.y;

	UpdateData (FALSE);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventMOVE::Get										 */
/* ���e		:�擾															 */
/* ���t		:2008/06/26														 */
/* ========================================================================= */

void CDlgAdminMapEventMOVE::Get(CInfoMapEventBase *pDst)
{
	PCInfoMapEventMOVE pDstTmp = (PCInfoMapEventMOVE)pDst;

	UpdateData ();

	pDstTmp->m_ptDst.x = m_nPosX;
	pDstTmp->m_ptDst.y = m_nPosY;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventMOVE::OnInitDialog							 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/06/24														 */
/* ========================================================================= */

BOOL CDlgAdminMapEventMOVE::OnInitDialog()
{
	CDlgAdminMapEventNONE::OnInitDialog();

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventMOVE::OnAdminMsg								 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2008/06/28														 */
/* ========================================================================= */

LRESULT CDlgAdminMapEventMOVE::OnAdminMsg(WPARAM wParam, LPARAM lParam)
{
	switch (wParam) {
	case ADMINMSG_NOTIFYTYPE_RBUTTONDOWN:		/* �E�N���b�N�ʒm */
		m_nPosX = HIWORD (lParam);
		m_nPosY = LOWORD (lParam);
		UpdateData (FALSE);
		break;
	}

	return -1;
}

/* Copyright(C)URARA-works 2008 */
