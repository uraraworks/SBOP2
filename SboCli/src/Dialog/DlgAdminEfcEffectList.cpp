/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminEfcEffectList.cpp									 */
/* 内容			:噴出し一覧ダイアログクラス 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/07/06													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "LibInfoEffect.h"
#include "PacketADMIN_EFC_RENEWEFFECT.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminEfcEffect.h"
#include "DlgAdminEfcEffectList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminEfcEffectList::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminEfcEffectList)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminEfcEffectList, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminEfcEffectList)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_RENEW, OnRenew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcEffectList::CDlgAdminEfcEffectList					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/07/06														 */
/* ========================================================================= */

CDlgAdminEfcEffectList::CDlgAdminEfcEffectList(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminEfcEffectList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminEfcEffectList)
	//}}AFX_DATA_INIT
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcEffectList::~CDlgAdminEfcEffectList				 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/07/06														 */
/* ========================================================================= */

CDlgAdminEfcEffectList::~CDlgAdminEfcEffectList()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcEffectList::Init									 */
/* 内容		:初期化															 */
/* 日付		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffectList::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	m_pLibInfo = m_pMgrData->GetLibInfoEffect ();

	/* ウィンドウ作成 */
	Create (CDlgAdminEfcEffectList::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcEffectList::Renew									 */
/* 内容		:一覧を更新														 */
/* 日付		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffectList::Renew(void)
{
	int i, nCount;
	PCInfoEffect pInfoEffect;
	CmyString strTmp;

	m_List.DeleteAllItems ();

	nCount = m_pLibInfo->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoEffect = (PCInfoEffect)m_pLibInfo->GetPtr (i);
		strTmp.Format(_T("%d"), pInfoEffect->m_dwEffectID);
		m_List.InsertItem (i, strTmp);
		m_List.SetItemData (i, pInfoEffect->m_dwEffectID);
		m_List.SetItemText (i, 1, (LPCTSTR)pInfoEffect->m_strName);
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcEffectList::OnAdminMsg								 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffectList::OnAdminMsg(int nType, DWORD dwPara)
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcEffectList::OnInitDialog							 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/07/06														 */
/* ========================================================================= */

BOOL CDlgAdminEfcEffectList::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	m_List.SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn (0, "ID",			LVCFMT_LEFT, 40);
	m_List.InsertColumn (1, "エフェクト名",	LVCFMT_LEFT, 120);

	RegisterControl (IDC_RENEW,	LH_CTRL_X);
	RegisterControl (IDC_LIST,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);

	Renew ();

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcEffectList::OnMainFrame							 */
/* 内容		:メッセージハンドラ(WM_MAINFRAME)								 */
/* 日付		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffectList::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
	switch (dwCommand) {
	case MAINFRAMEMSG_RENEWEFFECT:		/* エフェクト情報更新 */
		Renew ();
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcEffectList::OnAdd									 */
/* 内容		:ボタンハンドラ(新規追加)										 */
/* 日付		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffectList::OnAdd()
{
	int nResult;
	PCInfoEffect pInfoEffect;
	CDlgAdminEfcEffect Dlg(this);
	CPacketADMIN_EFC_RENEWEFFECT Packet;

	Dlg.Init (m_pMgrData, FALSE);
	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		return;
	}

	pInfoEffect = (PCInfoEffect)m_pLibInfo->GetNew ();
	Dlg.GetInfo (pInfoEffect);

	Packet.Make (0, pInfoEffect);
	m_pSock->Send (&Packet);
	SAFE_DELETE (pInfoEffect);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcEffectList::OnModify								 */
/* 内容		:ボタンハンドラ(編集)											 */
/* 日付		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffectList::OnModify()
{
	int nNo, nResult;
	PCInfoEffect pInfoEffect;
	CDlgAdminEfcEffect Dlg(this);
	CPacketADMIN_EFC_RENEWEFFECT Packet;

	nNo = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nNo < 0) {
		return;
	}
	pInfoEffect = (PCInfoEffect)m_pLibInfo->GetPtr (m_List.GetItemData (nNo));

	Dlg.Init (m_pMgrData, TRUE);
	Dlg.SetInfo (pInfoEffect);
	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		return;
	}

	Dlg.GetInfo (pInfoEffect);

	Packet.Make (pInfoEffect->m_dwEffectID, pInfoEffect);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcEffectList::OnDelete								 */
/* 内容		:ボタンハンドラ(削除)											 */
/* 日付		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffectList::OnDelete()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcEffectList::OnRenew								 */
/* 内容		:ボタンハンドラ(更新)											 */
/* 日付		:2008/07/06														 */
/* ========================================================================= */

void CDlgAdminEfcEffectList::OnRenew()
{
}

/* Copyright(C)URARA-works 2008 */
