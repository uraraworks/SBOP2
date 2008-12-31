/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminTalkEventADDSKILL.cpp								 */
/* ���e			:��b�C�x���g�ݒ�(�X�L���ǉ�)�_�C�A���O�N���X �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/31													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "LibInfoSkill.h"
#include "InfoTalkEventBase.h"
#include "MgrData.h"
#include "DlgAdminTalkEventADDSKILL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminTalkEventADDSKILL::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminTalkEventNONE::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTalkEventADDSKILL)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SKILL, m_Combo);
}

BEGIN_MESSAGE_MAP(CDlgAdminTalkEventADDSKILL, CDlgAdminTalkEventNONE)
	//{{AFX_MSG_MAP(CDlgAdminTalkEventADDSKILL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventADDSKILL::CDlgAdminTalkEventADDSKILL			 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/31														 */
/* ========================================================================= */

CDlgAdminTalkEventADDSKILL::CDlgAdminTalkEventADDSKILL(CWnd* pParent /*=NULL*/)
	: CDlgAdminTalkEventNONE(pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminTalkEventADDSKILL)
	//}}AFX_DATA_INIT

	m_nResourceID = CDlgAdminTalkEventADDSKILL::IDD;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventADDSKILL::~CDlgAdminTalkEventADDSKILL		 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/31														 */
/* ========================================================================= */

CDlgAdminTalkEventADDSKILL::~CDlgAdminTalkEventADDSKILL()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventADDSKILL::Set								 */
/* ���e		:�ݒ肩���ʂɔ��f												 */
/* ���t		:2008/12/31														 */
/* ========================================================================= */

void CDlgAdminTalkEventADDSKILL::Set(CInfoTalkEventBase *pSrc)
{
	int i, nCount;

	UpdateData (FALSE);

	nCount = m_Combo.GetCount ();
	for (i = 0; i < nCount; i ++) {
		if (m_Combo.GetItemData (i) == pSrc->m_dwData) {
			m_Combo.SetCurSel (i);
			break;
		}
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventADDSKILL::Get								 */
/* ���e		:��ʂ���ݒ�ɔ��f												 */
/* ���t		:2008/12/31														 */
/* ========================================================================= */

void CDlgAdminTalkEventADDSKILL::Get(CInfoTalkEventBase *pDst)
{
	UpdateData ();

	pDst->m_dwData = m_Combo.GetItemData (m_Combo.GetCurSel ());
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventADDSKILL::OnInitDialog						 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/12/31														 */
/* ========================================================================= */

BOOL CDlgAdminTalkEventADDSKILL::OnInitDialog()
{
	int i, nCount;
	PCLibInfoSkill pLibInfoSkill;
	PCInfoSkillBase pInfoSkill;
	CString strTmp;

	CDlgAdminTalkEventNONE::OnInitDialog();

	pLibInfoSkill = m_pMgrData->GetLibInfoSkill ();
	nCount = pLibInfoSkill->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoSkill = (PCInfoSkillBase)pLibInfoSkill->GetPtr (i);
		m_Combo.InsertString (i, (LPCSTR)pInfoSkill->m_strName);
		m_Combo.SetItemData (i, pInfoSkill->m_dwSkillID);
	}
	m_Combo.SetCurSel (0);

	return TRUE;
}

/* Copyright(C)URARA-works 2008 */
