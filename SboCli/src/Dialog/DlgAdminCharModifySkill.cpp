/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharModifySkill.cpp								 */
/* 内容			:選択キャラ情報編集[所持スキルの設定]ダイアログクラス 実装ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/01/18													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "InfoSkillBase.h"
#include "LibInfoSkill.h"
#include "Packet.h"
#include "LibInfoCharCli.h"
#include "InfoCharCli.h"
#include "MgrData.h"
#include "DlgAdminCharModifySkill.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminCharModifySkill::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharModifySkill)
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Control(pDX, IDC_COMBO, m_Combo);
	DDX_Text(pDX, IDC_STATIC_CHARNAME, m_strCharName);
	DDX_Text(pDX, IDC_STATIC_CHARID, m_strCharID);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharModifySkill, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharModifySkill)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifySkill::CDlgAdminCharModifySkill				 */
/* 内容		:コンストラクタ													 */
/* 日付		:2009/01/18														 */
/* ========================================================================= */

CDlgAdminCharModifySkill::CDlgAdminCharModifySkill(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminCharModifySkill::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharModifySkill)
	m_strCharName = _T("");
	m_strCharID = _T("");
	//}}AFX_DATA_INIT

	m_pInfoChar		= NULL;
	m_pLibInfoSkill	= NULL;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifySkill::~CDlgAdminCharModifySkill			 */
/* 内容		:デストラクタ													 */
/* 日付		:2009/01/18														 */
/* ========================================================================= */

CDlgAdminCharModifySkill::~CDlgAdminCharModifySkill()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifySkill::Init									 */
/* 内容		:初期化															 */
/* 日付		:2009/01/18														 */
/* ========================================================================= */

void CDlgAdminCharModifySkill::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	m_pLibInfoSkill = m_pMgrData->GetLibInfoSkill ();

	/* ウィンドウ作成 */
	Create (CDlgAdminCharModifySkill::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifySkill::Renew								 */
/* 内容		:更新															 */
/* 日付		:2009/01/18														 */
/* ========================================================================= */

void CDlgAdminCharModifySkill::Renew(void)
{
	int i, nCount;
	PCInfoSkillBase pInfoSkill;
	CString strTmp;

	m_strCharID.	Empty ();
	m_strCharName.	Empty ();
	m_List.DeleteAllItems ();

	if (m_pInfoChar == NULL) {
		goto Exit;
	}

	m_strCharID.Format(_T("%u"), m_pInfoChar->m_dwCharID);
	m_strCharName = m_pInfoChar->m_strCharName;

	nCount = m_pInfoChar->m_adwSkillID.size();
	for (i = 0; i < nCount; i ++) {
		pInfoSkill = (PCInfoSkillBase)m_pLibInfoSkill->GetPtr (m_pInfoChar->m_adwSkillID[i]);
		if (pInfoSkill == NULL) {
			continue;
		}
		strTmp.Format(_T("%u"), pInfoSkill->m_dwSkillID);
		m_List.InsertItem (i, strTmp, 0);			/* ID */
		m_List.SetItemData (i, pInfoSkill->m_dwSkillID);
		strTmp.Format(_T("%s"), (LPCTSTR)pInfoSkill->m_strName);
		m_List.SetItemText (i, 1, strTmp);			/* スキル名 */
	}

Exit:
	UpdateData (FALSE);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifySkill::OnAdminMsg							 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2007/03/18														 */
/* ========================================================================= */

void CDlgAdminCharModifySkill::OnAdminMsg(int nType, DWORD dwPara)
{
	PCLibInfoCharCli pLibInfoChar;
	CPacketCHAR_REQ_CHARINFO PacketCHAR_REQ_CHARINFO;

	pLibInfoChar	= m_pMgrData->GetLibInfoChar ();
	m_pInfoChar		= (PCInfoCharCli)pLibInfoChar->GetPtr (dwPara);

	PacketCHAR_REQ_CHARINFO.Make (dwPara);
	m_pSock->Send (&PacketCHAR_REQ_CHARINFO);

	Renew ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifySkill::OnMainFrame							 */
/* 内容		:メッセージハンドラ(WM_MAINFRAME)								 */
/* 日付		:2009/01/18														 */
/* ========================================================================= */

void CDlgAdminCharModifySkill::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
	switch (dwCommand) {
	case MAINFRAMEMSG_RENEWCHARINFO:		/* キャラ情報更新 */
		if (m_pInfoChar == NULL) {
			break;
		}
		if (m_pInfoChar->m_dwCharID != dwParam) {
			break;
		}
		Renew ();
		break;
	case MAINFRAMEMSG_RENEWSKILLINFO:		/* スキル情報更新 */
		Renew ();
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifySkill::OnInitDialog							 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2009/01/18														 */
/* ========================================================================= */

BOOL CDlgAdminCharModifySkill::OnInitDialog()
{
	int i, nCount;
	PCInfoSkillBase pInfoSkill;

	CDlgAdminBase::OnInitDialog ();

	m_List.SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn (0, _T("ID"), LVCFMT_LEFT, 40);
	m_List.InsertColumn (1, _T("スキル名"), LVCFMT_LEFT, 120);

	nCount = m_pLibInfoSkill->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoSkill = (PCInfoSkillBase)m_pLibInfoSkill->GetPtr (i);
		m_Combo.InsertString (i, Utf8ToTString ((LPCSTR)pInfoSkill->m_strName));
		m_Combo.SetItemData (i, pInfoSkill->m_dwSkillID);
	}
	m_Combo.SetCurSel (0);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifySkill::OnAdd								 */
/* 内容		:ボタンハンドラ(追加)											 */
/* 日付		:2009/01/18														 */
/* ========================================================================= */

void CDlgAdminCharModifySkill::OnAdd()
{
	int nResult;
	DWORD dwSkillID;
	CPacketADMIN_CHAR_MODIFYSKILL Packet;

	if (m_pInfoChar == NULL) {
		return;
	}
	nResult = m_Combo.GetCurSel ();
	if (nResult < 0) {
		return;
	}
	dwSkillID = m_Combo.GetItemData (nResult);
	if (dwSkillID == 0) {
		return;
	}

	Packet.Make (m_pInfoChar->m_dwCharID, dwSkillID, CHARMODIFYSKILLTYPE_ADD);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifySkill::OnDelete								 */
/* 内容		:ボタンハンドラ(削除)											 */
/* 日付		:2009/01/18														 */
/* ========================================================================= */

void CDlgAdminCharModifySkill::OnDelete()
{
	int nNo;
	DWORD dwSkillID;
	CPacketADMIN_CHAR_MODIFYSKILL Packet;

	if (m_pInfoChar == NULL) {
		return;
	}

	nNo = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nNo < 0) {
		return;
	}
	dwSkillID = m_List.GetItemData (nNo);

	Packet.Make (m_pInfoChar->m_dwCharID, dwSkillID, CHARMODIFYSKILLTYPE_DELETE);
	m_pSock->Send (&Packet);
}

/* Copyright(C)URARA-works 2007 */
