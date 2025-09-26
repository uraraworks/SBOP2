/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharSkillList.cpp									 */
/* 内容			:スキルリストダイアログクラス 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/07													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "PacketADMIN_SKILL_RENEWSKILL.h"
#include "LibInfoSkill.h"
#include "MgrData.h"
#include "DlgAdminCharSkillBase.h"
#include "DlgAdminCharSkillList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminCharSkillList::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharSkillList)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharSkillList, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharSkillList)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillList::CDlgAdminCharSkillList					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

CDlgAdminCharSkillList::CDlgAdminCharSkillList(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminCharSkillList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharSkillList)
	//}}AFX_DATA_INIT

	m_pLibInfoSkill = NULL;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillList::~CDlgAdminCharSkillList				 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

CDlgAdminCharSkillList::~CDlgAdminCharSkillList()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillList::Init									 */
/* 内容		:初期化															 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillList::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	m_pLibInfoSkill = m_pMgrData->GetLibInfoSkill ();

	/* ウィンドウ作成 */
	Create (CDlgAdminCharSkillList::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillList::Renew									 */
/* 内容		:リスト更新														 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillList::Renew(void)
{
	int i, nCount;
	PCInfoSkillBase pInfo;
	CmyString strTmp;

	m_List.DeleteAllItems ();

	nCount = m_pLibInfoSkill->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoSkillBase)m_pLibInfoSkill->GetPtr (i);

		strTmp.Format(_T("%d"), pInfo->m_dwSkillID);
		m_List.InsertItem (i, strTmp);
		CString strName = (LPCTSTR)pInfo->m_strName;
		m_List.SetItemText (i, 1, strName);
		m_List.SetItemData (i, pInfo->m_dwSkillID);
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillList::OnMainFrame							 */
/* 内容		:メッセージハンドラ(WM_MAINFRAME)								 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillList::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
	switch (dwCommand) {
	case MAINFRAMEMSG_RENEWSKILLINFO:		/* スキル情報更新 */
		Renew ();
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillList::OnInitDialog							 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

BOOL CDlgAdminCharSkillList::OnInitDialog()
{
	CmyString strTmp;

	CDlgAdminBase::OnInitDialog ();

	m_List.SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn (0, _T("ID"),		LVCFMT_LEFT, 40);
	m_List.InsertColumn (1, _T("スキル名"),	LVCFMT_LEFT, 300);

	Renew ();

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillList::OnAdd									 */
/* 内容		:ボタンハンドラ(追加)											 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillList::OnAdd()
{
	int nResult;
	PCInfoSkillBase pInfo;
	CPacketADMIN_SKILL_RENEWSKILL Packet;
	CDlgAdminCharSkillBase Dlg(this);

	pInfo = NULL;

	Dlg.Init (m_pMgrData);
	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		goto Exit;
	}

	Dlg.Get (pInfo);

	Packet.Make (pInfo);
	m_pSock->Send (&Packet);

Exit:
	SAFE_DELETE (pInfo);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillList::OnModify								 */
/* 内容		:ボタンハンドラ(編集)											 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillList::OnModify()
{
	int nResult;
	DWORD dwSkillID;
	CDlgAdminCharSkillBase Dlg(this);
	PCInfoSkillBase pInfo, pInfoTmp;
	CPacketADMIN_SKILL_RENEWSKILL Packet;

	nResult = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nResult < 0) {
		goto Exit;
	}
	dwSkillID	= m_List.GetItemData (nResult);
	pInfo		= (PCInfoSkillBase)m_pLibInfoSkill->GetPtr (dwSkillID);
	if (pInfo == NULL) {
		goto Exit;
	}

	Dlg.Init (m_pMgrData);
	Dlg.SetModify (pInfo);

	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		goto Exit;
	}

	pInfoTmp = NULL;
	Dlg.Get (pInfoTmp);
	pInfo = m_pLibInfoSkill->Renew (pInfoTmp);
	SAFE_DELETE (pInfoTmp);

	Packet.Make (pInfo);
	m_pSock->Send (&Packet);

Exit:
	return;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillList::OnDelete								 */
/* 内容		:ボタンハンドラ(削除)											 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillList::OnDelete()
{
}

/* Copyright(C)URARA-works 2008 */
