/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharSkillMOVEATACK.cpp								 */
/* ���e			:�X�L��(�ړ����čU��)�_�C�A���O�N���X �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/01/07													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoSkillMOVEATACK.h"
#include "LibInfoEffect.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminCharSkillMOVEATACK.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminCharSkillMOVEATACK::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminCharSkillNONE::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharSkillMOVEATACK)
	DDX_Control(pDX, IDC_TARGET, m_ctlTarget);
	DDX_Control(pDX, IDC_UP, m_ctlUp);
	DDX_Control(pDX, IDC_DOWN, m_ctlDown);
	DDX_Control(pDX, IDC_LEFT, m_ctlLeft);
	DDX_Control(pDX, IDC_RIGHT, m_ctlRight);
	DDX_Control(pDX, IDC_TYPE, m_ctlPutType);
	DDX_Text(pDX, IDC_TIME, m_dwAliveTime);
	DDX_Text(pDX, IDC_WAITTIME, m_dwWaitTime);
	DDX_Text(pDX, IDC_VALUE, m_dwValue);
	DDX_Text(pDX, IDC_DISTANCE, m_dwDistance);
	DDX_Check(pDX, IDC_HIT, m_bHitQuit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharSkillMOVEATACK, CDlgAdminCharSkillNONE)
	//{{AFX_MSG_MAP(CDlgAdminCharSkillMOVEATACK)
	ON_MESSAGE(WM_ADMINMSG, OnAdminMsg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillMOVEATACK::CDlgAdminCharSkillMOVEATACK		 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2009/01/07														 */
/* ========================================================================= */

CDlgAdminCharSkillMOVEATACK::CDlgAdminCharSkillMOVEATACK(CWnd* pParent /*=NULL*/)
	: CDlgAdminCharSkillNONE(pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharSkillMOVEATACK)
	m_dwAliveTime = 0;
	m_dwWaitTime = 0;
	m_dwValue = 0;
	m_dwDistance = 0;
	m_bHitQuit = FALSE;
	//}}AFX_DATA_INIT

	m_nResourceID = CDlgAdminCharSkillMOVEATACK::IDD;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillMOVEATACK::~CDlgAdminCharSkillMOVEATACK		 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2009/01/07														 */
/* ========================================================================= */

CDlgAdminCharSkillMOVEATACK::~CDlgAdminCharSkillMOVEATACK()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillMOVEATACK::Init								 */
/* ���e		:������															 */
/* ���t		:2009/01/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillMOVEATACK::Init(CMgrData *pMgrData)
{
	CDlgAdminCharSkillNONE::Init (pMgrData);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillMOVEATACK::Set								 */
/* ���e		:�ݒ�															 */
/* ���t		:2009/01/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillMOVEATACK::Set(CInfoSkillBase *pSrc)
{
	int i, j, nNo, nCount;
	DWORD dwTmp;
	CComboBox *pCombo, *apCtlEffectID[] = { &m_ctlUp, &m_ctlDown, &m_ctlLeft, &m_ctlRight };
	PCInfoSkillMOVEATACK pSrcTmp = (PCInfoSkillMOVEATACK)pSrc;

	nNo = 0;
	nCount = m_ctlTarget.GetCount ();
	for (i = 0; i < nCount; i ++) {
		dwTmp = m_ctlTarget.GetItemData (i);
		if (pSrcTmp->m_dwTartgetType == dwTmp) {
			nNo = i;
			break;
		}
	}
	m_ctlTarget.SetCurSel (nNo);

	m_dwAliveTime	= pSrcTmp->m_dwAliveTime;	/* �ϋv���� */
	m_dwWaitTime	= pSrcTmp->m_dwWaitTime;	/* �ړ����x */
	m_dwValue		= pSrcTmp->m_dwValue;		/* ���� */
	m_dwDistance	= pSrcTmp->m_dwDistance;	/* �˒����� */
	m_bHitQuit		= pSrcTmp->m_bHitQuit;		/* �q�b�g����Ə��� */

	/* �����ɂ��\���G�t�F�N�g */
	for (j = 0; j < 4; j ++) {
		pCombo = apCtlEffectID[j];
		nNo = 0;
		nCount = pCombo->GetCount ();
		for (i = 0; i < nCount; i ++) {
			dwTmp = pCombo->GetItemData (i);
			if (pSrcTmp->m_adwEffectID[j] == dwTmp) {
				nNo = i;
				break;
			}
		}
		pCombo->SetCurSel (nNo);
	}

	UpdateData (FALSE);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillMOVEATACK::Get								 */
/* ���e		:�擾															 */
/* ���t		:2009/01/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillMOVEATACK::Get(CInfoSkillBase *pDst)
{
	int i, nNo;
	CComboBox *pCombo, *apCtlEffectID[] = { &m_ctlUp, &m_ctlDown, &m_ctlLeft, &m_ctlRight };
	PCInfoSkillMOVEATACK pDstTmp = (PCInfoSkillMOVEATACK)pDst;

	UpdateData ();

	nNo = m_ctlTarget.GetCurSel ();
	pDstTmp->m_dwTartgetType	= m_ctlTarget.GetItemData (nNo);	/* �U���Ώ� */
	pDstTmp->m_dwAliveTime		= m_dwAliveTime;	/* �ϋv���� */
	pDstTmp->m_dwWaitTime		= m_dwWaitTime;		/* �ړ����x */
	pDstTmp->m_dwValue			= m_dwValue;		/* ���� */
	pDstTmp->m_dwDistance		= m_dwDistance;		/* �˒����� */
	pDstTmp->m_bHitQuit			= m_bHitQuit;		/* �q�b�g����Ə��� */

	/* �����ɂ��\���G�t�F�N�g */
	for (i = 0; i < 4; i ++) {
		pCombo = apCtlEffectID[i];
		nNo = pCombo->GetCurSel ();
		pDstTmp->m_adwEffectID[i] = pCombo->GetItemData (nNo);
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillMOVEATACK::OnInitDialog						 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2009/01/07														 */
/* ========================================================================= */

BOOL CDlgAdminCharSkillMOVEATACK::OnInitDialog()
{
	int i, j, nCount;
	PCInfoEffect pInfoEffect;
	PCLibInfoEffect pLibInfoEffect;
	STINTLPCSTR *pIntLpcstr,
		astTarget[] = {
			SKILLMOVEATACKTARGETTYPE_NONE,	"���ݒ�",
			SKILLMOVEATACKTARGETTYPE_PC,	"�v���C���[",
			SKILLMOVEATACKTARGETTYPE_NPC,	"NPC",
			SKILLMOVEATACKTARGETTYPE_ALL,	"�S��",
			-1, NULL
		},
		astPutType[] = {
			SKILLMOVEATACKPUTTYPE_NONE,		"���ݒ�",
			SKILLMOVEATACKPUTTYPE_FRONT,	"�O��",
			SKILLMOVEATACKPUTTYPE_CROSS,	"�㉺���E",
			-1, NULL
		};
	CComboBox *pCombo, *apCtlEffectID[] = { &m_ctlUp, &m_ctlDown, &m_ctlLeft, &m_ctlRight };

	CDlgAdminCharSkillNONE::OnInitDialog();
	pLibInfoEffect = m_pMgrData->GetLibInfoEffect ();

	/* �U���Ώ� */
	for (i = 0; ; i ++) {
		pIntLpcstr = &astTarget[i];
		if (pIntLpcstr->nValue == -1) {
			break;
		}
		m_ctlTarget.InsertString (i, pIntLpcstr->pszText);
		m_ctlTarget.SetItemData (i, pIntLpcstr->nValue);
	}
	m_ctlTarget.SetCurSel (0);

	/* �G�t�F�N�gID */
	for (j = 0; j < 4; j ++) {
		pCombo = apCtlEffectID[j];
		pCombo->InsertString (0, "����");
	}
	nCount = pLibInfoEffect->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoEffect = (PCInfoEffect)pLibInfoEffect->GetPtr (i);
		for (j = 0; j < 4; j ++) {
			pCombo = apCtlEffectID[j];
			pCombo->InsertString (i + 1, (LPCSTR)pInfoEffect->m_strName);
			pCombo->SetItemData (i + 1, pInfoEffect->m_dwEffectID);
		}
	}
	for (j = 0; j < 4; j ++) {
		pCombo = apCtlEffectID[j];
		pCombo->SetCurSel (0);
	}

	/* ���ˎ�� */
	for (i = 0; ; i ++) {
		pIntLpcstr = &astPutType[i];
		if (pIntLpcstr->nValue == -1) {
			break;
		}
		m_ctlPutType.InsertString (i, pIntLpcstr->pszText);
		m_ctlPutType.SetItemData (i, pIntLpcstr->nValue);
	}
	m_ctlPutType.SetCurSel (0);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharSkillMOVEATACK::OnAdminMsg						 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2009/01/07														 */
/* ========================================================================= */

LRESULT CDlgAdminCharSkillMOVEATACK::OnAdminMsg(WPARAM wParam, LPARAM lParam)
{
	return -1;
}

/* Copyright(C)URARA-works 2009 */
