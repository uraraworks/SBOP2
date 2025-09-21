/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminTalkEventADDSKILL.cpp								 */
/* 内容			:会話イベント設定(スキル追加)ダイアログクラス 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/31													 */
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
/* クラスの設定																 */
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
/* 関数名	:CDlgAdminTalkEventADDSKILL::CDlgAdminTalkEventADDSKILL			 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/31														 */
/* ========================================================================= */

CDlgAdminTalkEventADDSKILL::CDlgAdminTalkEventADDSKILL(CWnd* pParent /*=NULL*/)
	: CDlgAdminTalkEventNONE(pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminTalkEventADDSKILL)
	//}}AFX_DATA_INIT

	m_nResourceID = CDlgAdminTalkEventADDSKILL::IDD;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventADDSKILL::~CDlgAdminTalkEventADDSKILL		 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/31														 */
/* ========================================================================= */

CDlgAdminTalkEventADDSKILL::~CDlgAdminTalkEventADDSKILL()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventADDSKILL::Set								 */
/* 内容		:設定から画面に反映												 */
/* 日付		:2008/12/31														 */
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
/* 関数名	:CDlgAdminTalkEventADDSKILL::Get								 */
/* 内容		:画面から設定に反映												 */
/* 日付		:2008/12/31														 */
/* ========================================================================= */

void CDlgAdminTalkEventADDSKILL::Get(CInfoTalkEventBase *pDst)
{
	UpdateData ();

	pDst->m_dwData = m_Combo.GetItemData (m_Combo.GetCurSel ());
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventADDSKILL::OnInitDialog						 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/12/31														 */
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
