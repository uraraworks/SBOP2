/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminTalkEventPAGE.cpp									 */
/* ���e			:��b�C�x���g�ݒ�(�y�[�W�؂�ւ�)�_�C�A���O�N���X �����t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/23													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "LibInfoItemType.h"
#include "InfoTalkEventPAGE.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminTalkEventPAGE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminTalkEventPAGE::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminTalkEventNONE::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTalkEventPAGE)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO, m_Combo);
	DDX_Control(pDX, IDC_ITEM_CMB, m_cmbItem);
	DDX_Control(pDX, IDC_JUMP_CMB, m_cmbJump);
}

BEGIN_MESSAGE_MAP(CDlgAdminTalkEventPAGE, CDlgAdminTalkEventNONE)
	//{{AFX_MSG_MAP(CDlgAdminTalkEventPAGE)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO, &CDlgAdminTalkEventPAGE::OnCbnSelchangeCombo)
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventPAGE::CDlgAdminTalkEventPAGE					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventPAGE::CDlgAdminTalkEventPAGE(CWnd* pParent /*=NULL*/)
	: CDlgAdminTalkEventNONE(pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminTalkEventPAGE)
	//}}AFX_DATA_INIT

	m_nResourceID = CDlgAdminTalkEventPAGE::IDD;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventPAGE::~CDlgAdminTalkEventPAGE				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventPAGE::~CDlgAdminTalkEventPAGE()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventPAGE::Set									 */
/* ���e		:�ݒ肩���ʂɔ��f												 */
/* ���t		:2008/12/26														 */
/* ========================================================================= */

void CDlgAdminTalkEventPAGE::Set(CInfoTalkEventBase *pSrc)
{
	int i, nCount;
	PCInfoTalkEventPAGE pSrcTmp = (PCInfoTalkEventPAGE)pSrc;

	UpdateData (FALSE);

	m_Combo.SetCurSel (pSrcTmp->m_nPageChgCondition);
	OnCbnSelchangeCombo ();
	m_cmbJump.SetCurSel (pSrcTmp->m_nPageJump);

	nCount = m_cmbItem.GetCount ();
	for (i = 0; i < nCount; i ++) {
		if (m_cmbItem.GetItemData (i) == pSrcTmp->m_dwData) {
			m_cmbItem.SetCurSel (i);
			break;
		}
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventPAGE::Get									 */
/* ���e		:��ʂ���ݒ�ɔ��f												 */
/* ���t		:2008/12/26														 */
/* ========================================================================= */

void CDlgAdminTalkEventPAGE::Get(CInfoTalkEventBase *pDst)
{
	PCInfoTalkEventPAGE pDstTmp = (PCInfoTalkEventPAGE)pDst;

	UpdateData ();

	pDstTmp->m_nPageChgCondition	= m_Combo.GetItemData (m_Combo.GetCurSel ());
	pDstTmp->m_nPageJump			= m_cmbJump.GetCurSel ();
	pDstTmp->m_dwData				= m_cmbItem.GetItemData (m_cmbItem.GetCurSel ());
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventPAGE::OnInitDialog							 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

BOOL CDlgAdminTalkEventPAGE::OnInitDialog()
{
	int i, nCount;
	PCLibInfoItemType pLibInfoItemType;
	PCInfoItemTypeBase pInfoItem;
	CString strTmp;

	CDlgAdminTalkEventNONE::OnInitDialog();

	m_Combo.InsertString (0, "���ݒ�");
	m_Combo.SetItemData (0, CHGPAGECONDITION_NONE);
	m_Combo.InsertString (1, "�A�C�e������");
	m_Combo.SetItemData (1, CHGPAGECONDITION_ITEM);
	m_Combo.InsertString (2, "�A�C�e���Ȃ�");
	m_Combo.SetItemData (2, CHGPAGECONDITION_NOITEM);
	m_Combo.SetCurSel (0);
	OnCbnSelchangeCombo ();

	for (i = 0; i < m_nPageCount; i ++) {
		strTmp.Format ("�y�[�W%d", i + 1);
		m_cmbJump.InsertString (i, strTmp);
	}
	m_cmbJump.SetCurSel (0);

	pLibInfoItemType = m_pMgrData->GetLibInfoItemType ();
	nCount = pLibInfoItemType->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoItem = (PCInfoItemTypeBase)pLibInfoItemType->GetPtr (i);
		m_cmbItem.InsertString (i, (LPCSTR)pInfoItem->m_strName);
		m_cmbItem.SetItemData (i, pInfoItem->m_dwTypeID);
	}
	m_cmbItem.SetCurSel (0);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventPAGE::OnCbnSelchangeCombo					 */
/* ���e		:�C�x���g�n���h��(CBN_SELCHANGE)								 */
/* ���t		:2008/12/29														 */
/* ========================================================================= */

void CDlgAdminTalkEventPAGE::OnCbnSelchangeCombo()
{
	int nResult, nCmdShow;

	nCmdShow = SW_HIDE;
	nResult  = m_Combo.GetCurSel ();
	switch (nResult) {
	case CHGPAGECONDITION_ITEM:		/* �A�C�e������ */
	case CHGPAGECONDITION_NOITEM:	/* �A�C�e���Ȃ� */
		nCmdShow = SW_SHOW;
		break;
	}
	GetDlgItem (IDC_ITEM_STA)->ShowWindow (nCmdShow);
	GetDlgItem (IDC_ITEM_CMB)->ShowWindow (nCmdShow);
}

/* Copyright(C)URARA-works 2008 */
