/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharModifyTypePUTNPC.cpp							 */
/* ���e			:�L������� �ړ���ʂ̐ݒ�(NPC����)�_�C�A���O�N���X �����t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/07/12													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoCharCli.h"
#include "MgrData.h"
#include "DlgAdminCharModifyTypePUTNPC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminCharModifyTypePUTNPC::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminCharModifyTypeBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharModifyTypePUTNPC)
	DDX_Control(pDX, IDC_MOVETYPE, m_ctlMoveType);
	DDX_Text(pDX, IDC_PUTCYCLE, m_dwPutCycle);
	DDX_Text(pDX, IDC_MAXPUTCOUNT, m_nMaxPutCount);
	DDX_Text(pDX, IDC_PUTAVERAGE, m_nPutAverage);
	DDX_Text(pDX, IDC_PUTAREA_X, m_nPutAreaX);
	DDX_Text(pDX, IDC_PUTAREA_Y, m_nPutAreaY);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharModifyTypePUTNPC, CDlgAdminCharModifyTypeBase)
	//{{AFX_MSG_MAP(CDlgAdminCharModifyTypePUTNPC)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminCharModifyTypePUTNPC::CDlgAdminCharModifyTypePUTNPC	 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

CDlgAdminCharModifyTypePUTNPC::CDlgAdminCharModifyTypePUTNPC(CWnd* pParent /*=NULL*/)
	: CDlgAdminCharModifyTypeBase(CDlgAdminCharModifyTypePUTNPC::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharModifyTypePUTNPC)
	m_dwPutCycle = 0;
	m_nMaxPutCount = 0;
	m_nPutAverage = 0;
	m_nPutAreaX = 0;
	m_nPutAreaY = 0;
	//}}AFX_DATA_INIT

	m_nPutMoveType = 0;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharModifyTypePUTNPC::~CDlgAdminCharModifyTypePUTNPC	 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

CDlgAdminCharModifyTypePUTNPC::~CDlgAdminCharModifyTypePUTNPC()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharModifyTypePUTNPC::Set								 */
/* ���e		:�ݒ�															 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

void CDlgAdminCharModifyTypePUTNPC::Set(CInfoCharCli *pSrc)
{
	m_dwPutCycle	= pSrc->m_dwPutCycle;
	m_nMaxPutCount	= pSrc->m_nMaxPutCount;
	m_nPutAverage	= pSrc->m_nPutAverage;
	m_nPutAreaX		= pSrc->m_ptPutArea.x;
	m_nPutAreaY		= pSrc->m_ptPutArea.y;
	m_nPutMoveType	= pSrc->m_nPutMoveType;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharModifyTypePUTNPC::Get								 */
/* ���e		:�擾															 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

void CDlgAdminCharModifyTypePUTNPC::Get(CInfoCharCli *pDst)
{
	pDst->m_dwPutCycle		= m_dwPutCycle;
	pDst->m_nMaxPutCount	= m_nMaxPutCount;
	pDst->m_nPutAverage		= m_nPutAverage;
	pDst->m_ptPutArea.x		= m_nPutAreaX;
	pDst->m_ptPutArea.y		= m_nPutAreaY;
	pDst->m_nPutMoveType	= m_nPutMoveType;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharModifyTypePUTNPC::OnInitDialog					 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

BOOL CDlgAdminCharModifyTypePUTNPC::OnInitDialog()
{
	int i, nCount, nNo;

	CDlgAdminCharModifyTypeBase::OnInitDialog ();

	m_ctlMoveType.AddString ("�퓬1");
	m_ctlMoveType.SetItemData (0, CHARMOVETYPE_BATTLE1);

	nNo = 0;
	nCount = m_ctlMoveType.GetCount ();
	for (i = 0; i < nCount; i ++) {
		if (m_nPutMoveType == m_ctlMoveType.GetItemData (i)) {
			nNo = i;
			break;
		}
	}
	m_ctlMoveType.SetCurSel (nNo);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharModifyTypePUTNPC::OnOK							 */
/* ���e		:�{�^���n���h��(OK)												 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

void CDlgAdminCharModifyTypePUTNPC::OnOK()
{
	UpdateData ();

	m_nPutMoveType = m_ctlMoveType.GetItemData (m_ctlMoveType.GetCurSel ());

	CDlgAdminCharModifyTypeBase::EndDialog (IDOK);
}

/* Copyright(C)URARA-works 2008 */
