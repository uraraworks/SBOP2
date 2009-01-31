/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharSkillHEAL.cpp									 */
/* ���e			:�X�L��(�ړ����čU��)�_�C�A���O�N���X �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/01/22													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoSkillHEAL.h"
#include "LibInfoEffect.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminCharSkillHEAL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminCharSkillHEAL::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminCharSkillNONE::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharSkillHEAL)
	DDX_Control(pDX, IDC_AREA, m_ctlArea);
	DDX_Control(pDX, IDC_TYPE, m_ctlHealType);
	DDX_Control(pDX, IDC_HITEFFECT, m_ctlHitEffect);
	DDX_Text(pDX, IDC_VALUE1, m_dwValue1);
	DDX_Text(pDX, IDC_VALUE2, m_dwValue2);
	DDX_Text(pDX, IDC_DISTANCE, m_dwDistance);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharSkillHEAL, CDlgAdminCharSkillNONE)
	//{{AFX_MSG_MAP(CDlgAdminCharSkillHEAL)
	ON_MESSAGE(WM_ADMINMSG, OnAdminMsg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillHEAL::CDlgAdminCharSkillHEAL					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2009/01/22														 */
/* ========================================================================= */

CDlgAdminCharSkillHEAL::CDlgAdminCharSkillHEAL(CWnd* pParent /*=NULL*/)
	: CDlgAdminCharSkillNONE(pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharSkillHEAL)
	m_dwValue1 = 0;
	m_dwValue2 = 0;
	m_dwDistance = 0;
	//}}AFX_DATA_INIT

	m_nResourceID = CDlgAdminCharSkillHEAL::IDD;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillHEAL::~CDlgAdminCharSkillHEAL				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2009/01/22														 */
/* ========================================================================= */

CDlgAdminCharSkillHEAL::~CDlgAdminCharSkillHEAL()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillHEAL::Init									 */
/* ���e		:������															 */
/* ���t		:2009/01/22														 */
/* ========================================================================= */

void CDlgAdminCharSkillHEAL::Init(CMgrData *pMgrData)
{
	CDlgAdminCharSkillNONE::Init (pMgrData);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillHEAL::Set									 */
/* ���e		:�ݒ�															 */
/* ���t		:2009/01/22														 */
/* ========================================================================= */

void CDlgAdminCharSkillHEAL::Set(CInfoSkillBase *pSrc)
{
	int i, nNo, nCount;
	DWORD dwTmp;
	CComboBox *pCombo;
	PCInfoSkillHEAL pSrcTmp = (PCInfoSkillHEAL)pSrc;

	m_dwValue1		= pSrcTmp->m_dwValue1;		/* ����1 */
	m_dwValue2		= pSrcTmp->m_dwValue2;		/* ����2 */
	m_dwDistance	= pSrcTmp->m_dwDistance;	/* �˒����� */

	/* �q�b�g���̕\���G�t�F�N�g */
	pCombo = &m_ctlHitEffect;
	nNo = 0;
	nCount = pCombo->GetCount ();
	for (i = 0; i < nCount; i ++) {
		dwTmp = pCombo->GetItemData (i);
		if (pSrcTmp->m_dwHitEffectID == dwTmp) {
			nNo = i;
			break;
		}
	}
	pCombo->SetCurSel (nNo);

	/* �񕜎�� */
	pCombo = &m_ctlHealType;
	nNo = 0;
	nCount = pCombo->GetCount ();
	for (i = 0; i < nCount; i ++) {
		dwTmp = pCombo->GetItemData (i);
		if (pSrcTmp->m_dwHealType == dwTmp) {
			nNo = i;
			break;
		}
	}
	pCombo->SetCurSel (nNo);

	/* �͈� */
	pCombo = &m_ctlArea;
	nNo = 0;
	nCount = pCombo->GetCount ();
	for (i = 0; i < nCount; i ++) {
		dwTmp = pCombo->GetItemData (i);
		if (pSrcTmp->m_dwArea == dwTmp) {
			nNo = i;
			break;
		}
	}
	pCombo->SetCurSel (nNo);

	UpdateData (FALSE);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillHEAL::Get									 */
/* ���e		:�擾															 */
/* ���t		:2009/01/22														 */
/* ========================================================================= */

void CDlgAdminCharSkillHEAL::Get(CInfoSkillBase *pDst)
{
	int nNo;
	PCInfoSkillHEAL pDstTmp = (PCInfoSkillHEAL)pDst;

	UpdateData ();

	pDstTmp->m_dwValue1			= m_dwValue1;		/* ����1 */
	pDstTmp->m_dwValue2			= m_dwValue2;		/* ����2 */
	pDstTmp->m_dwDistance		= m_dwDistance;		/* �˒����� */

	/* �q�b�g���̕\���G�t�F�N�g */
	nNo = m_ctlHitEffect.GetCurSel ();
	pDstTmp->m_dwHitEffectID = m_ctlHitEffect.GetItemData (nNo);
	/* �񕜎�� */
	nNo = m_ctlHealType.GetCurSel ();
	pDstTmp->m_dwHealType = m_ctlHealType.GetItemData (nNo);
	/* �͈� */
	nNo = m_ctlArea.GetCurSel ();
	pDstTmp->m_dwArea = m_ctlArea.GetItemData (nNo);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillHEAL::OnInitDialog							 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2009/01/22														 */
/* ========================================================================= */

BOOL CDlgAdminCharSkillHEAL::OnInitDialog()
{
	int i, nCount;
	PCInfoEffect pInfoEffect;
	PCLibInfoEffect pLibInfoEffect;
	STINTLPCSTR *pIntLpcstr,
		astArea[] = {
			SKILLHEAL_AREA_NONE,		"�����ʒu",
			SKILLHEAL_AREA_FRONT,		"�O��",
			SKILLHEAL_AREA_AREA,		"����",
			-1, NULL
		},
		astHealType[] = {
			SKILLHEAL_HEALTYPE_NONE,	"���ݒ�",
			SKILLHEAL_HEALTYPE_HP,		"HP",
			SKILLHEAL_HEALTYPE_SP,		"SP",
			-1, NULL
		};

	CDlgAdminCharSkillNONE::OnInitDialog();
	pLibInfoEffect = m_pMgrData->GetLibInfoEffect ();

	/* �G�t�F�N�gID */
	m_ctlHitEffect.InsertString (0, "����");
	nCount = pLibInfoEffect->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoEffect = (PCInfoEffect)pLibInfoEffect->GetPtr (i);
		m_ctlHitEffect.InsertString (i + 1, (LPCSTR)pInfoEffect->m_strName);
		m_ctlHitEffect.SetItemData (i + 1, pInfoEffect->m_dwEffectID);
	}
	m_ctlHitEffect.SetCurSel (0);

	/* �񕜎�� */
	for (i = 0; ; i ++) {
		pIntLpcstr = &astHealType[i];
		if (pIntLpcstr->nValue == -1) {
			break;
		}
		m_ctlHealType.InsertString (i, pIntLpcstr->pszText);
		m_ctlHealType.SetItemData (i, pIntLpcstr->nValue);
	}
	m_ctlHealType.SetCurSel (0);

	/* �͈� */
	for (i = 0; ; i ++) {
		pIntLpcstr = &astArea[i];
		if (pIntLpcstr->nValue == -1) {
			break;
		}
		m_ctlArea.InsertString (i, pIntLpcstr->pszText);
		m_ctlArea.SetItemData (i, pIntLpcstr->nValue);
	}
	m_ctlArea.SetCurSel (0);


	return TRUE;
}

/* Copyright(C)URARA-works 2009 */
