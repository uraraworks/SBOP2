/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapEventMAPMOVE.cpp								 */
/* ���e			:�}�b�v�C�x���g(�}�b�v�Ԉړ�)�_�C�A���O�N���X �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/07/27													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoMapEventMAPMOVE.h"
#include "MgrData.h"
#include "DlgAdminMapEventMAPMOVE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminMapEventMAPMOVE::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminMapEventNONE::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminMapEventMAPMOVE)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_MAPID, m_dwMapID);
	DDX_Text(pDX, IDC_POSX, m_nPosX);
	DDX_Text(pDX, IDC_POSY, m_nPosY);
	DDX_Control(pDX, IDC_DIRECTION, m_ctlDirection);
	DDX_CBIndex(pDX, IDC_DIRECTION, m_nDirection);
}

BEGIN_MESSAGE_MAP(CDlgAdminMapEventMAPMOVE, CDlgAdminMapEventNONE)
	//{{AFX_MSG_MAP(CDlgAdminMapEventMAPMOVE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventMAPMOVE::CDlgAdminMapEventMAPMOVE				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/07/27														 */
/* ========================================================================= */

CDlgAdminMapEventMAPMOVE::CDlgAdminMapEventMAPMOVE(CWnd* pParent /*=NULL*/)
	: CDlgAdminMapEventNONE(pParent)
	, m_dwMapID(0)
	, m_nPosX(0)
	, m_nPosY(0)
	, m_nDirection(-1)
{
	//{{AFX_DATA_INIT(CDlgAdminMapEventMAPMOVE)
	//}}AFX_DATA_INIT

	m_nResourceID = CDlgAdminMapEventMAPMOVE::IDD;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventMAPMOVE::~CDlgAdminMapEventMAPMOVE			 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/07/27														 */
/* ========================================================================= */

CDlgAdminMapEventMAPMOVE::~CDlgAdminMapEventMAPMOVE()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventMAPMOVE::Set									 */
/* ���e		:�ݒ�															 */
/* ���t		:2008/06/26														 */
/* ========================================================================= */

void CDlgAdminMapEventMAPMOVE::Set(CInfoMapEventBase *pSrc)
{
	PCInfoMapEventMAPMOVE pSrcTmp = (PCInfoMapEventMAPMOVE)pSrc;

	m_dwMapID		= pSrcTmp->m_dwMapID;
	m_nPosX			= pSrcTmp->m_ptDst.x;
	m_nPosY			= pSrcTmp->m_ptDst.y;
	m_nDirection	= pSrcTmp->m_nDirection;
	if (m_nDirection < 0) {
		m_nDirection = 4;
	}

	UpdateData (FALSE);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventMAPMOVE::Get									 */
/* ���e		:�擾															 */
/* ���t		:2008/06/26														 */
/* ========================================================================= */

void CDlgAdminMapEventMAPMOVE::Get(CInfoMapEventBase *pDst)
{
	PCInfoMapEventMAPMOVE pDstTmp = (PCInfoMapEventMAPMOVE)pDst;

	UpdateData ();

	pDstTmp->m_dwMapID		= m_dwMapID;
	pDstTmp->m_ptDst.x		= m_nPosX;
	pDstTmp->m_ptDst.y		= m_nPosY;
	pDstTmp->m_nDirection	= m_nDirection;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventMAPMOVE::OnInitDialog							 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/07/27														 */
/* ========================================================================= */

BOOL CDlgAdminMapEventMAPMOVE::OnInitDialog()
{
	CDlgAdminMapEventNONE::OnInitDialog();

	m_ctlDirection.InsertString (0, "��");
	m_ctlDirection.InsertString (1, "��");
	m_ctlDirection.InsertString (2, "��");
	m_ctlDirection.InsertString (3, "�E");
	m_ctlDirection.InsertString (4, "�w�薳��");

	if (m_nDirection < 0) {
		m_nDirection = 4;
	}
	m_ctlDirection.SetCurSel (m_nDirection);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventMAPMOVE::OnAdminMsg							 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2008/06/28														 */
/* ========================================================================= */

LRESULT CDlgAdminMapEventMAPMOVE::OnAdminMsg(WPARAM wParam, LPARAM lParam)
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
