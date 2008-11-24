/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapEventBase.cpp									 */
/* ���e			:�}�b�v�C�x���g�ҏW�_�C�A���O�N���X �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/24													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoMapEventBase.h"
#include "LibInfoMapEvent.h"
#include "MgrData.h"
#include "DlgAdminMapEventMOVE.h"
#include "DlgAdminMapEventMAPMOVE.h"
#include "DlgAdminMapEventINITSTATUS.h"
#include "DlgAdminMapEventGRPIDTMP.h"
#include "DlgAdminMapEventBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminMapEventBase::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminMapEventBase)
	DDX_Control(pDX, IDC_TYPE, m_ctlType);
	DDX_Control(pDX, IDC_HITTYPE, m_ctlHitType);
	DDX_Control(pDX, IDC_DIRECTION, m_ctlDirection);
	DDX_Text(pDX, IDC_POSX, m_nPosX);
	DDX_Text(pDX, IDC_POSY, m_nPosY);
	DDX_Text(pDX, IDC_POSX2, m_nPosX2);
	DDX_Text(pDX, IDC_POSY2, m_nPosY2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminMapEventBase, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminMapEventBase)
	ON_CBN_SELCHANGE(IDC_TYPE, OnSelchangeType)
	ON_CBN_SELCHANGE(IDC_HITTYPE, OnSelchangeHitType)
	ON_CBN_SELCHANGE(IDC_DIRECTION, OnSelchangeDirection)
	ON_MESSAGE(WM_ADMINMSG, OnAdminMsg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventBase::CDlgAdminMapEventBase					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/06/24														 */
/* ========================================================================= */

CDlgAdminMapEventBase::CDlgAdminMapEventBase(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminMapEventBase::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminMapEventBase)
	m_nPosX = 0;
	m_nPosY = 0;
	m_nPosX2 = 0;
	m_nPosY2 = 0;
	//}}AFX_DATA_INIT

	m_nEventType	= -1;
	m_nHitType		= -1;
	m_nHitDirection	= -1;
	m_bModeModify	= FALSE;
	m_ppWndNotify	= NULL;
	m_pDlgType		= NULL;
	m_pInfo			= NULL;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventBase::~CDlgAdminMapEventBase					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/06/24														 */
/* ========================================================================= */

CDlgAdminMapEventBase::~CDlgAdminMapEventBase()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventBase::Init									 */
/* ���e		:������															 */
/* ���t		:2008/06/24														 */
/* ========================================================================= */

void CDlgAdminMapEventBase::Init(CMgrData *pMgrData, CWnd **pWndNotify)
{
	CDlgAdminBase::Init (pMgrData);

	m_ppWndNotify = pWndNotify;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventBase::Get										 */
/* ���e		:�ҏW���e���擾													 */
/* ���t		:2008/06/25														 */
/* ========================================================================= */

void CDlgAdminMapEventBase::Get(CInfoMapEventBase *&pDst)
{
	CLibInfoMapEvent LibInfo;

	if (m_pInfo == NULL) {
		return;
	}

	SAFE_DELETE (pDst);
	pDst = (PCInfoMapEventBase)LibInfo.GetNew (m_pInfo->m_nType);
	pDst->Copy (m_pInfo);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventBase::SetModify								 */
/* ���e		:�ҏW���[�h�Ƃ��Đݒ�											 */
/* ���t		:2007/08/16														 */
/* ========================================================================= */

void CDlgAdminMapEventBase::SetModify(CInfoMapEventBase *pSrc)
{
	CLibInfoMapEvent LibInfo;

	SAFE_DELETE (m_pInfo);
	m_pInfo = (PCInfoMapEventBase)LibInfo.GetNew (pSrc->m_nType);
	m_pInfo->Copy (pSrc);

	m_nPosX  = m_pInfo->m_ptPos.x;
	m_nPosY  = m_pInfo->m_ptPos.y;
	m_nPosX2 = m_pInfo->m_ptPos2.x;
	m_nPosY2 = m_pInfo->m_ptPos2.y;

	m_bModeModify = TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventBase::OnInitDialog							 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/06/24														 */
/* ========================================================================= */

BOOL CDlgAdminMapEventBase::OnInitDialog()
{
	int i, nNo;
//	LPCSTR pszTmp;

	CDlgAdminBase::OnInitDialog();

	if (m_ppWndNotify) {
		*m_ppWndNotify = this;
	}

	if (m_bModeModify) {
		SetWindowText ("�}�b�v�C�x���g�̕ҏW");
	}

	m_ctlType.InsertString (0, "���ݒ�");
	m_ctlType.SetItemData (0, MAPEVENTTYPE_NONE);
	m_ctlType.InsertString (1, "�}�b�v���ړ�");
	m_ctlType.SetItemData (1, MAPEVENTTYPE_MOVE);
	m_ctlType.InsertString (2, "�}�b�v�Ԉړ�");
	m_ctlType.SetItemData (2, MAPEVENTTYPE_MAPMOVE);
	m_ctlType.InsertString (3, "�S�~��");
	m_ctlType.SetItemData (3, MAPEVENTTYPE_TRASHBOX);
	m_ctlType.InsertString (4, "�X�e�[�^�X������");
	m_ctlType.SetItemData (4, MAPEVENTTYPE_INITSTATUS);
	m_ctlType.InsertString (5, "�ꎞ�摜�ݒ�");
	m_ctlType.SetItemData (5, MAPEVENTTYPE_GRPIDTMP);

	m_ctlHitType.InsertString (0, "�}�b�v���W�Ŕ���");
	m_ctlHitType.SetItemData (0, MAPEVENTHITTYPE_MAPPOS);
	m_ctlHitType.InsertString (1, "�L�������W�Ŕ���");
	m_ctlHitType.SetItemData (1, MAPEVENTHITTYPE_CHARPOS);
	m_ctlHitType.InsertString (2, "�͈͂Ŕ���");
	m_ctlHitType.SetItemData (2, MAPEVENTHITTYPE_AREA);

	m_ctlDirection.InsertString (0, "��");
	m_ctlDirection.InsertString (1, "��");
	m_ctlDirection.InsertString (2, "��");
	m_ctlDirection.InsertString (3, "�E");
	m_ctlDirection.InsertString (4, "�w�薳��");

	nNo = 0;
	if (m_pInfo) {
		for (i = 0; i < MAPEVENTTYPE_MAX; i ++) {
			if (m_pInfo->m_nType == m_ctlType.GetItemData (i)) {
				nNo = i;
				m_nEventType = m_pInfo->m_nType;
				break;
			}
		}
	}
	m_ctlType.SetCurSel (nNo);

	nNo = 0;
	if (m_pInfo) {
		for (i = 0; i < MAPEVENTHITTYPE_MAX; i ++) {
			if (m_pInfo->m_nHitType == m_ctlHitType.GetItemData (i)) {
				nNo = i;
				m_nHitType = m_pInfo->m_nHitType;
				break;
			}
		}
	}
	m_ctlHitType.SetCurSel (nNo);

	nNo = 4;
	if (m_pInfo) {
		nNo = m_pInfo->m_nHitDirection;
		if (nNo < 0) {
			nNo = 4;
		}
	}
	m_ctlDirection.SetCurSel (nNo);

	OnSelchangeType ();
	OnSelchangeHitType ();
	OnSelchangeDirection ();
	if (m_pDlgType && m_pInfo) {
		m_pDlgType->Set (m_pInfo);
	}

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnAdminMsg										 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2007/03/18														 */
/* ========================================================================= */

LRESULT CDlgAdminMapEventBase::OnAdminMsg(WPARAM wParam, LPARAM lParam)
{
	switch (wParam) {
	case ADMINMSG_NOTIFYTYPE_LBUTTONDOWN:		/* ���N���b�N�ʒm */
		m_nPosX = HIWORD (lParam);
		m_nPosY = LOWORD (lParam);
		UpdateData (FALSE);
		break;
	case ADMINMSG_NOTIFYTYPE_RBUTTONDOWN:		/* �E�N���b�N�ʒm */
		if (m_pDlgType == NULL) {
			break;
		}
		m_pDlgType->PostMessage (WM_ADMINMSG, wParam, lParam);
		break;
	}

	return -1;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventBase::PostNcDestroy							 */
/* ���e		:�I������														 */
/* ���t		:2008/06/24														 */
/* ========================================================================= */

void CDlgAdminMapEventBase::PostNcDestroy()
{
	CDialog::PostNcDestroy ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventBase::OnSelchangeType							 */
/* ���e		:�C�x���g�n���h��(CBN_SELCHANGE)								 */
/* ���t		:2008/06/24														 */
/* ========================================================================= */

void CDlgAdminMapEventBase::OnSelchangeType()
{
	int nNo, nEventType;
	CRect rc;
	PCInfoMapEventBase pInfoTmp;
	CLibInfoMapEvent LibInfo;

	nNo = m_ctlType.GetCurSel ();
	nEventType = m_ctlType.GetItemData (nNo);

	if (m_pDlgType) {
		m_pDlgType->DestroyWindow ();
		m_pDlgType = NULL;
	}
	switch (nEventType) {
	case MAPEVENTTYPE_MOVE:			/* �}�b�v���ړ� */
		m_pDlgType = new CDlgAdminMapEventMOVE(this);
		break;
	case MAPEVENTTYPE_MAPMOVE:		/* �}�b�v�Ԉړ� */
		m_pDlgType = new CDlgAdminMapEventMAPMOVE(this);
		break;
	case MAPEVENTTYPE_INITSTATUS:	/* �X�e�[�^�X������ */
		m_pDlgType = new CDlgAdminMapEventINITSTATUS(this);
		break;
	case MAPEVENTTYPE_GRPIDTMP:		/* �ꎞ�摜�ݒ� */
		m_pDlgType = new CDlgAdminMapEventGRPIDTMP(this);
		break;
	default:
		m_pDlgType = new CDlgAdminMapEventNONE(this);
		break;
	}
	if (m_pDlgType) {
		m_pDlgType->Init (m_pMgrData);
		GetDlgItem (IDC_EVENT)->GetWindowRect (rc);
		ScreenToClient (rc);
		m_pDlgType->MoveWindow (rc.left, rc.top, rc.Width (), rc.Height ());
	}

	if (m_nEventType != nEventType) {
		pInfoTmp = (PCInfoMapEventBase)LibInfo.GetNew (nEventType);
		if (m_pInfo) {
			pInfoTmp->m_dwMapEventID	= m_pInfo->m_dwMapEventID;
			pInfoTmp->m_ptPos			= m_pInfo->m_ptPos;
		}
		pInfoTmp->m_nType = nEventType;

		SAFE_DELETE (m_pInfo);
		m_pInfo = pInfoTmp;
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventBase::OnSelchangeHitType						 */
/* ���e		:�C�x���g�n���h��(CBN_SELCHANGE)								 */
/* ���t		:2008/10/26														 */
/* ========================================================================= */

void CDlgAdminMapEventBase::OnSelchangeHitType()
{
	int nNo;

	if (m_pInfo == NULL) {
		return;
	}

	nNo = m_ctlHitType.GetCurSel ();
	m_nHitType = m_ctlHitType.GetItemData (nNo);
	m_pInfo->m_nHitType = m_nHitType;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventBase::OnSelchangeDirection					 */
/* ���e		:�C�x���g�n���h��(CBN_SELCHANGE)								 */
/* ���t		:2008/11/24														 */
/* ========================================================================= */

void CDlgAdminMapEventBase::OnSelchangeDirection()
{
	int nNo;

	if (m_pInfo == NULL) {
		return;
	}

	nNo = m_ctlDirection.GetCurSel ();
	m_nHitDirection = m_ctlDirection.GetCurSel ();
	m_pInfo->m_nHitDirection = m_nHitDirection;
	if (m_nHitDirection > 3) {
		m_pInfo->m_nHitDirection = -1;
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventBase::OnOK									 */
/* ���e		:�{�^���n���h��(OK)												 */
/* ���t		:2007/08/16														 */
/* ========================================================================= */

void CDlgAdminMapEventBase::OnOK()
{
	UpdateData ();

	if (m_pInfo) {
		m_pInfo->m_ptPos.x  = m_nPosX;
		m_pInfo->m_ptPos.y  = m_nPosY;
		m_pInfo->m_ptPos2.x = m_nPosX2;
		m_pInfo->m_ptPos2.y = m_nPosY2;
		if (m_pDlgType) {
			m_pDlgType->Get (m_pInfo);
		}
	}

	CDlgAdminBase::EndDialog (IDOK);
}

/* Copyright(C)URARA-works 2008 */
