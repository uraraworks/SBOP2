/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharModify.cpp										 */
/* 内容			:選択キャラ情報編集ダイアログクラス 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/03/17													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "PacketADMIN_CHARINFO.h"
#include "LibInfoMotionType.h"
#include "LayoutHelper.h"
#include "LibInfoCharCli.h"
#include "InfoCharCli.h"
#include "MgrData.h"
#include "DlgAdminCharModifyTypePUTNPC.h"
#include "DlgAdminTalkSet.h"
#include "DlgAdminCharModify.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminCharModify::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharModify)
	DDX_Control(pDX, IDC_MOVETYPE, m_ctlMoveType);
	DDX_Control(pDX, IDC_MOTIONTYPE, m_ctlMotionType);
	DDX_Control(pDX, IDC_SEX, m_ctlSex);
	DDX_Text(pDX, IDC_CHARNAME, m_strCharName);
	DDX_Text(pDX, IDC_STATIC_CHARID, m_strCharID);
	DDX_Check(pDX, IDC_BLOCK, m_bBlock);
	DDX_Check(pDX, IDC_PUSH, m_bPush);
	DDX_Text(pDX, IDC_POSMAPID, m_dwMapID);
	DDX_Text(pDX, IDC_POSX, m_nPosX);
	DDX_Text(pDX, IDC_POSY, m_nPosY);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharModify, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharModify)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	ON_BN_CLICKED(IDC_TALK, OnTalk)
	ON_BN_CLICKED(IDC_SET_MOVETYPE, OnSetMoveType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModify::CDlgAdminCharModify						 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/03/17														 */
/* ========================================================================= */

CDlgAdminCharModify::CDlgAdminCharModify(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminCharModify::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharModify)
	m_strCharName = _T("");
	m_strCharID = _T("");
	m_bBlock = FALSE;
	m_bPush = FALSE;
	m_dwMapID = 0;
	m_nPosX = 0;
	m_nPosY = 0;
	//}}AFX_DATA_INIT

	m_pInfoChar = NULL;
	Create (CDlgAdminCharModify::IDD, pParent);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModify::~CDlgAdminCharModify						 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/03/17														 */
/* ========================================================================= */

CDlgAdminCharModify::~CDlgAdminCharModify()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModify::Init										 */
/* 内容		:初期化															 */
/* 日付		:2007/03/17														 */
/* ========================================================================= */

void CDlgAdminCharModify::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	/* ウィンドウ作成 */
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModify::Renew										 */
/* 内容		:更新															 */
/* 日付		:2007/03/17														 */
/* ========================================================================= */

void CDlgAdminCharModify::Renew(void)
{
	int i, nCount, nNo;
	PCLibInfoMotionType pLibInfoMotionType;
	PCInfoMotionType pInfoMotionType;

	pLibInfoMotionType = m_pMgrData->GetLibInfoMotionType ();
	m_strCharID.	Empty ();
	m_strCharName.	Empty ();
	m_bBlock	= FALSE;
	m_bPush		= FALSE;
	m_dwMapID	= 0;
	m_nPosX		= 0;
	m_nPosY		= 0;
	m_ctlMoveType.SetCurSel (0);
	m_ctlMotionType.SetCurSel (0);
	m_ctlSex.SetCurSel (0);

	if (m_pInfoChar == NULL) {
		goto Exit;
	}

	m_strCharID.Format ("%u", m_pInfoChar->m_dwCharID);
	m_strCharName	= m_pInfoChar->m_strCharName;
	m_bBlock		= m_pInfoChar->m_bBlock;
	m_bPush			= m_pInfoChar->m_bPush;
	for (i = 0; i < CHARMOVETYPE_MAX; i ++) {
		if (m_ctlMoveType.GetItemData (i) == m_pInfoChar->m_nMoveType) {
			m_ctlMoveType.SetCurSel (i);
			break;
		}
	}
	if (i >= CHARMOVETYPE_MAX) {
		m_ctlMoveType.SetCurSel (0);
	}

	m_dwMapID	= m_pInfoChar->m_dwMapID;
	m_nPosX		= m_pInfoChar->m_nMapX;
	m_nPosY		= m_pInfoChar->m_nMapY;

	m_ctlMotionType.ResetContent ();
	nCount = pLibInfoMotionType->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoMotionType = (PCInfoMotionType)pLibInfoMotionType->GetPtr (i);
		m_ctlMotionType.AddString ((LPCSTR)pInfoMotionType->m_strName);
		m_ctlMotionType.SetItemData (i, pInfoMotionType->m_dwMotionTypeID);
	}

	nNo = 0;
	nCount = m_ctlMotionType.GetCount ();
	for (i = 0; i < nCount; i ++) {
		if (m_pInfoChar->m_dwMotionTypeID == m_ctlMotionType.GetItemData (i)) {
			nNo = i;
			break;
		}
	}
	m_ctlMotionType.SetCurSel (nNo);

	m_ctlSex.ResetContent ();
	m_ctlSex.AddString ("男性");
	m_ctlSex.SetItemData (0, SEX_MALE);
	m_ctlSex.AddString ("女性");
	m_ctlSex.SetItemData (1, SEX_FEMALE);
	nNo = 0;
	if (m_pInfoChar->m_nSex == SEX_FEMALE) {
		nNo = 1;
	}
	m_ctlSex.SetCurSel (nNo);

Exit:
	UpdateData (FALSE);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModify::OnAdminMsg								 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2007/03/18														 */
/* ========================================================================= */

void CDlgAdminCharModify::OnAdminMsg(int nType, DWORD dwPara)
{
	PCLibInfoCharCli pLibInfoChar;

	switch (nType) {
	case ADMINMSG_NOTIFYTYPE_LBUTTONDOWN:	/* 左クリック通知 */
		pLibInfoChar	= m_pMgrData->GetLibInfoChar ();
		m_pInfoChar		= (PCInfoCharCli)pLibInfoChar->GetPtr (dwPara);
		Renew ();
		break;
	case ADMINMSG_NOTIFYTYPE_RBUTTONDOWN:	/* 右クリック通知 */
		m_nPosX = HIWORD (dwPara);
		m_nPosY = LOWORD (dwPara);
		UpdateData (FALSE);
		break;
	case ADMINMSG_NOTIFYTYPE_RBUTTONDBLCLK:	/* 右ダブルクリック通知 */
		m_nPosX = HIWORD (dwPara);
		m_nPosY = LOWORD (dwPara);
		UpdateData (FALSE);
		Send (TRUE);
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModify::OnInitDialog								 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2007/03/17														 */
/* ========================================================================= */

BOOL CDlgAdminCharModify::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog ();

	m_ctlMoveType.AddString ("プレイヤーキャラ");
	m_ctlMoveType.SetItemData (0, CHARMOVETYPE_PC);
	m_ctlMoveType.AddString ("移動しない");
	m_ctlMoveType.SetItemData (1, CHARMOVETYPE_STAND);
	m_ctlMoveType.AddString ("ボール");
	m_ctlMoveType.SetItemData (2, CHARMOVETYPE_BALL);
	m_ctlMoveType.AddString ("得点");
	m_ctlMoveType.SetItemData (3, CHARMOVETYPE_SCORE);
	m_ctlMoveType.AddString ("容姿コピー(取り込み)");
	m_ctlMoveType.SetItemData (4, CHARMOVETYPE_STYLECOPY_PUT);
	m_ctlMoveType.AddString ("攻撃受けるとアニメーション");
	m_ctlMoveType.SetItemData (5, CHARMOVETYPE_ATACKANIME);
	m_ctlMoveType.AddString ("容姿コピー(反映)");
	m_ctlMoveType.SetItemData (6, CHARMOVETYPE_STYLECOPY_GET);
	m_ctlMoveType.AddString ("NPC発生");
	m_ctlMoveType.SetItemData (7, CHARMOVETYPE_PUTNPC);
	m_ctlMoveType.AddString ("戦闘1");
	m_ctlMoveType.SetItemData (8, CHARMOVETYPE_BATTLE1);
	m_ctlMoveType.SetCurSel (0);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModify::OnSend									 */
/* 内容		:ボタンハンドラ(送信)											 */
/* 日付		:2007/03/18														 */
/* ========================================================================= */

void CDlgAdminCharModify::OnSend()
{
	Send (FALSE);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModify::OnTalk									 */
/* 内容		:ボタンハンドラ(会話データの編集)								 */
/* 日付		:2008/11/22														 */
/* ========================================================================= */

void CDlgAdminCharModify::OnTalk()
{
	int nResult;
	CDlgAdminTalkSet Dlg;

	if (m_pInfoChar == NULL) {
		return;
	}

	Dlg.m_strTalk = m_pInfoChar->m_strTalk;
	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		return;
	}
	m_pInfoChar->m_strTalk = Dlg.m_strTalk;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModify::OnSetMoveType								 */
/* 内容		:ボタンハンドラ(移動種別の設定)									 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

void CDlgAdminCharModify::OnSetMoveType()
{
	int nMoveType, nResult;
	PCDlgAdminCharModifyTypeBase pDlg;

	pDlg = NULL;

	nMoveType = m_ctlMoveType.GetItemData (m_ctlMoveType.GetCurSel ());
	switch (nMoveType) {
	case CHARMOVETYPE_PUTNPC:	/* NPC発生 */
		pDlg = new CDlgAdminCharModifyTypePUTNPC(this);
		break;
	}

	if (pDlg) {
		pDlg->Set (m_pInfoChar);
		nResult = pDlg->DoModal ();
		if (nResult == IDOK) {
			pDlg->Get (m_pInfoChar);
		}
	}

	SAFE_DELETE (pDlg);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModify::Send										 */
/* 内容		:送信処理														 */
/* 日付		:2007/09/16														 */
/* ========================================================================= */

void CDlgAdminCharModify::Send(BOOL bChgScreenPos)
{
	CInfoCharCli InfoCharTmp;
	CPacketADMIN_CHARINFO Packet;

	if (m_pInfoChar == NULL) {
		return;
	}

	UpdateData ();

	InfoCharTmp.Create (m_pMgrData);
	InfoCharTmp.Copy (m_pInfoChar);
	InfoCharTmp.m_strCharName		= m_strCharName;
	InfoCharTmp.m_bBlock			= m_bBlock;
	InfoCharTmp.m_bPush				= m_bPush;
	InfoCharTmp.m_nMoveType			= m_ctlMoveType.GetItemData (m_ctlMoveType.GetCurSel ());
	InfoCharTmp.m_dwMapID			= m_dwMapID;
	InfoCharTmp.m_nMapX				= m_nPosX;
	InfoCharTmp.m_nMapY				= m_nPosY;
	InfoCharTmp.m_dwMotionTypeID	= m_ctlMotionType.GetItemData (m_ctlMotionType.GetCurSel ());
	InfoCharTmp.m_nSex				= m_ctlSex.GetItemData (m_ctlSex.GetCurSel ());

	Packet.Make (&InfoCharTmp, bChgScreenPos);
	m_pSock->Send (&Packet);
}

/* Copyright(C)URARA-works 2007 */
