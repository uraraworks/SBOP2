/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharSkillMOVEATACK.cpp								 */
/* 内容			:スキル(移動して攻撃)ダイアログクラス 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/01/07													 */
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
/* クラスの設定																 */
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
/* 関数名	:CDlgAdminCharSkillMOVEATACK::CDlgAdminCharSkillMOVEATACK		 */
/* 内容		:コンストラクタ													 */
/* 日付		:2009/01/07														 */
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
/* 関数名	:CDlgAdminCharSkillMOVEATACK::~CDlgAdminCharSkillMOVEATACK		 */
/* 内容		:デストラクタ													 */
/* 日付		:2009/01/07														 */
/* ========================================================================= */

CDlgAdminCharSkillMOVEATACK::~CDlgAdminCharSkillMOVEATACK()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillMOVEATACK::Init								 */
/* 内容		:初期化															 */
/* 日付		:2009/01/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillMOVEATACK::Init(CMgrData *pMgrData)
{
	CDlgAdminCharSkillNONE::Init (pMgrData);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillMOVEATACK::Set								 */
/* 内容		:設定															 */
/* 日付		:2009/01/07														 */
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

	m_dwAliveTime	= pSrcTmp->m_dwAliveTime;	/* 耐久時間 */
	m_dwWaitTime	= pSrcTmp->m_dwWaitTime;	/* 移動速度 */
	m_dwValue		= pSrcTmp->m_dwValue;		/* 効果 */
	m_dwDistance	= pSrcTmp->m_dwDistance;	/* 射程距離 */
	m_bHitQuit		= pSrcTmp->m_bHitQuit;		/* ヒットすると消滅 */

	/* 向きによる表示エフェクト */
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
/* 関数名	:CDlgAdminCharSkillMOVEATACK::Get								 */
/* 内容		:取得															 */
/* 日付		:2009/01/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillMOVEATACK::Get(CInfoSkillBase *pDst)
{
	int i, nNo;
	CComboBox *pCombo, *apCtlEffectID[] = { &m_ctlUp, &m_ctlDown, &m_ctlLeft, &m_ctlRight };
	PCInfoSkillMOVEATACK pDstTmp = (PCInfoSkillMOVEATACK)pDst;

	UpdateData ();

	nNo = m_ctlTarget.GetCurSel ();
	pDstTmp->m_dwTartgetType	= m_ctlTarget.GetItemData (nNo);	/* 攻撃対象 */
	pDstTmp->m_dwAliveTime		= m_dwAliveTime;	/* 耐久時間 */
	pDstTmp->m_dwWaitTime		= m_dwWaitTime;		/* 移動速度 */
	pDstTmp->m_dwValue			= m_dwValue;		/* 効果 */
	pDstTmp->m_dwDistance		= m_dwDistance;		/* 射程距離 */
	pDstTmp->m_bHitQuit			= m_bHitQuit;		/* ヒットすると消滅 */

	/* 向きによる表示エフェクト */
	for (i = 0; i < 4; i ++) {
		pCombo = apCtlEffectID[i];
		nNo = pCombo->GetCurSel ();
		pDstTmp->m_adwEffectID[i] = pCombo->GetItemData (nNo);
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillMOVEATACK::OnInitDialog						 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2009/01/07														 */
/* ========================================================================= */

BOOL CDlgAdminCharSkillMOVEATACK::OnInitDialog()
{
	int i, j, nCount;
	PCInfoEffect pInfoEffect;
	PCLibInfoEffect pLibInfoEffect;
	STINTLPCSTR *pIntLpcstr,
		astTarget[] = {
			SKILLMOVEATACKTARGETTYPE_NONE,	"未設定",
			SKILLMOVEATACKTARGETTYPE_PC,	"プレイヤー",
			SKILLMOVEATACKTARGETTYPE_NPC,	"NPC",
			SKILLMOVEATACKTARGETTYPE_ALL,	"全て",
			-1, NULL
		},
		astPutType[] = {
			SKILLMOVEATACKPUTTYPE_NONE,		"未設定",
			SKILLMOVEATACKPUTTYPE_FRONT,	"前方",
			SKILLMOVEATACKPUTTYPE_CROSS,	"上下左右",
			-1, NULL
		};
	CComboBox *pCombo, *apCtlEffectID[] = { &m_ctlUp, &m_ctlDown, &m_ctlLeft, &m_ctlRight };

	CDlgAdminCharSkillNONE::OnInitDialog();
	pLibInfoEffect = m_pMgrData->GetLibInfoEffect ();

	/* 攻撃対象 */
	for (i = 0; ; i ++) {
		pIntLpcstr = &astTarget[i];
		if (pIntLpcstr->nValue == -1) {
			break;
		}
		m_ctlTarget.InsertString (i, pIntLpcstr->pszText);
		m_ctlTarget.SetItemData (i, pIntLpcstr->nValue);
	}
	m_ctlTarget.SetCurSel (0);

	/* エフェクトID */
	for (j = 0; j < 4; j ++) {
		pCombo = apCtlEffectID[j];
		pCombo->InsertString (0, "無し");
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

	/* 発射種別 */
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
/* 関数名	:CDlgAdminCharSkillMOVEATACK::OnAdminMsg						 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2009/01/07														 */
/* ========================================================================= */

LRESULT CDlgAdminCharSkillMOVEATACK::OnAdminMsg(WPARAM wParam, LPARAM lParam)
{
	return -1;
}

/* Copyright(C)URARA-works 2009 */
