/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapObjectDataEdit.cpp								 */
/* ���e			:�}�b�v�I�u�W�F�N�g�z�u�f�[�^�ҏW�_�C�A���O�N���X �����t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/11/03													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "LibInfoMapObject.h"
#include "LibInfoMapObjectData.h"
#include "MgrData.h"
#include "DlgAdminMapObjectDataEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminMapObjectDataEdit::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminMapObjectDataEdit)
	DDX_Control(pDX, IDC_TYPE, m_ctlType);
	DDX_Text(pDX, IDC_POSX, m_nPosX);
	DDX_Text(pDX, IDC_POSY, m_nPosY);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminMapObjectDataEdit, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminMapObjectDataEdit)
	ON_CBN_SELCHANGE(IDC_TYPE, OnSelchangeType)
	ON_MESSAGE(WM_ADMINMSG, OnAdminMsg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminMapObjectDataEdit::CDlgAdminMapObjectDataEdit			 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

CDlgAdminMapObjectDataEdit::CDlgAdminMapObjectDataEdit(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminMapObjectDataEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminMapObjectDataEdit)
	m_nPosX = 0;
	m_nPosY = 0;
	//}}AFX_DATA_INIT

	m_bModeModify		= FALSE;
	m_ppWndNotify		= NULL;
	m_pLibInfoMapObject	= NULL;
	m_pInfo				= NULL;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapObjectDataEdit::~CDlgAdminMapObjectDataEdit		 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

CDlgAdminMapObjectDataEdit::~CDlgAdminMapObjectDataEdit()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapObjectDataEdit::Init								 */
/* ���e		:������															 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

void CDlgAdminMapObjectDataEdit::Init(CMgrData *pMgrData, CWnd **pWndNotify)
{
	CDlgAdminBase::Init (pMgrData);

	m_pLibInfoMapObject = m_pMgrData->GetLibInfoMapObject ();
	m_pInfo = (PCInfoMapObjectData)m_pLibInfoMapObject->GetNew ();

	m_ppWndNotify = pWndNotify;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapObjectDataEdit::Get								 */
/* ���e		:�ҏW���e���擾													 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

void CDlgAdminMapObjectDataEdit::Get(CInfoMapObjectData *&pDst)
{
	if (m_pInfo == NULL) {
		return;
	}

	pDst->Copy (m_pInfo);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapObjectDataEdit::SetModify							 */
/* ���e		:�ҏW���[�h�Ƃ��Đݒ�											 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

void CDlgAdminMapObjectDataEdit::SetModify(CInfoMapObjectData *pSrc)
{
	CLibInfoMapObjectData LibInfo;

	SAFE_DELETE (m_pInfo);
	m_pInfo = (PCInfoMapObjectData)LibInfo.GetNew ();
	m_pInfo->Copy (pSrc);

	m_nPosX  = m_pInfo->m_ptPos.x;
	m_nPosY  = m_pInfo->m_ptPos.y;

	m_bModeModify = TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapObjectDataEdit::OnInitDialog						 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

BOOL CDlgAdminMapObjectDataEdit::OnInitDialog()
{
	int i, nNo, nCount;
	PCInfoMapObject pInfo;

	CDlgAdminBase::OnInitDialog();

	if (m_ppWndNotify) {
		*m_ppWndNotify = this;
	}

	if (m_bModeModify) {
		SetWindowText ("�}�b�v�I�u�W�F�N�g�z�u�f�[�^�̕ҏW");
	}

	nCount = m_pLibInfoMapObject->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapObject)m_pLibInfoMapObject->GetPtr (i);
		m_ctlType.InsertString (i, (LPCSTR)pInfo->m_strName);
		m_ctlType.SetItemData (i, pInfo->m_dwObjectID);
	}

	nNo = 0;
	if (m_pInfo) {
		for (i = 0; i < nCount; i ++) {
			if (m_pInfo->m_dwObjectID == m_ctlType.GetItemData (i)) {
				nNo = i;
				break;
			}
		}
	}
	m_ctlType.SetCurSel (nNo);

	OnSelchangeType ();

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnAdminMsg										 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2007/03/18														 */
/* ========================================================================= */

LRESULT CDlgAdminMapObjectDataEdit::OnAdminMsg(WPARAM wParam, LPARAM lParam)
{
	switch (wParam) {
	case ADMINMSG_NOTIFYTYPE_LBUTTONDOWN:		/* ���N���b�N�ʒm */
		m_nPosX = HIWORD (lParam);
		m_nPosY = LOWORD (lParam);
		UpdateData (FALSE);
		break;
	case ADMINMSG_NOTIFYTYPE_RBUTTONDOWN:		/* �E�N���b�N�ʒm */
		break;
	}

	return -1;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapObjectDataEdit::PostNcDestroy						 */
/* ���e		:�I������														 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

void CDlgAdminMapObjectDataEdit::PostNcDestroy()
{
	CDialog::PostNcDestroy ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapObjectDataEdit::OnSelchangeType					 */
/* ���e		:�C�x���g�n���h��(CBN_SELCHANGE)								 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

void CDlgAdminMapObjectDataEdit::OnSelchangeType()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapObjectDataEdit::OnOK								 */
/* ���e		:�{�^���n���h��(OK)												 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

void CDlgAdminMapObjectDataEdit::OnOK()
{
	int nNo;

	UpdateData ();

	nNo = m_ctlType.GetCurSel ();

	if (m_pInfo) {
		m_pInfo->m_ptPos.x		= m_nPosX;
		m_pInfo->m_ptPos.y		= m_nPosY;
		m_pInfo->m_dwObjectID	= m_ctlType.GetItemData (nNo);
	}

	CDlgAdminBase::EndDialog (IDOK);
}

/* Copyright(C)URARA-works 2008 */
