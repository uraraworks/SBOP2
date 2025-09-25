/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharModifyTypePUTNPC.cpp							 */
/* 内容			:キャラ情報 移動種別の設定(NPC発生)ダイアログクラス 実装ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/07/12													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoCharCli.h"
#include "MgrData.h"
#include "DlgAdminCharModifyTypePUTNPC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminCharModifyTypePUTNPC::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminCharModifyTypeBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharModifyTypePUTNPC)
	DDX_Control(pDX, IDC_MOVETYPE, m_ctlMoveType);
	DDX_Text(pDX, IDC_PUTCYCLE, m_dwPutCycle);
	DDX_Text(pDX, IDC_MAXPUTCOUNT, m_nMaxPutCount);
	DDX_Text(pDX, IDC_PUTAVERAGE, m_nPutAverage);
	DDX_Text(pDX, IDC_PUTAREA_X, m_nPutAreaX);
	DDX_Text(pDX, IDC_PUTAREA_Y, m_nPutAreaY);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharModifyTypePUTNPC, CDlgAdminCharModifyTypeBase)
	//{{AFX_MSG_MAP(CDlgAdminCharModifyTypePUTNPC)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifyTypePUTNPC::CDlgAdminCharModifyTypePUTNPC	 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

CDlgAdminCharModifyTypePUTNPC::CDlgAdminCharModifyTypePUTNPC(CWnd* pParent /*=NULL*/)
	: CDlgAdminCharModifyTypeBase(CDlgAdminCharModifyTypePUTNPC::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharModifyTypePUTNPC)
	m_dwPutCycle = 0;
	m_nMaxPutCount = 0;
	m_nPutAverage = 0;
	m_nPutAreaX = 0;
	m_nPutAreaY = 0;
	//}}AFX_DATA_INIT

	m_nPutMoveType = 0;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifyTypePUTNPC::~CDlgAdminCharModifyTypePUTNPC	 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

CDlgAdminCharModifyTypePUTNPC::~CDlgAdminCharModifyTypePUTNPC()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifyTypePUTNPC::Set								 */
/* 内容		:設定															 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

void CDlgAdminCharModifyTypePUTNPC::Set(CInfoCharCli *pSrc)
{
	m_dwPutCycle	= pSrc->m_dwPutCycle;
	m_nMaxPutCount	= pSrc->m_nMaxPutCount;
	m_nPutAverage	= pSrc->m_nPutAverage;
	m_nPutAreaX		= pSrc->m_ptPutArea.x;
	m_nPutAreaY		= pSrc->m_ptPutArea.y;
	m_nPutMoveType	= pSrc->m_nPutMoveType;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifyTypePUTNPC::Get								 */
/* 内容		:取得															 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

void CDlgAdminCharModifyTypePUTNPC::Get(CInfoCharCli *pDst)
{
	pDst->m_dwPutCycle		= m_dwPutCycle;
	pDst->m_nMaxPutCount	= m_nMaxPutCount;
	pDst->m_nPutAverage		= m_nPutAverage;
	pDst->m_ptPutArea.x		= m_nPutAreaX;
	pDst->m_ptPutArea.y		= m_nPutAreaY;
	pDst->m_nPutMoveType	= m_nPutMoveType;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifyTypePUTNPC::OnInitDialog					 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

BOOL CDlgAdminCharModifyTypePUTNPC::OnInitDialog()
{
	int i, nCount, nNo;

	CDlgAdminCharModifyTypeBase::OnInitDialog ();

        m_ctlMoveType.AddString (_T("戦闘1"));
        m_ctlMoveType.SetItemData (0, CHARMOVETYPE_BATTLE1);
        m_ctlMoveType.AddString (_T("戦闘2"));
        m_ctlMoveType.SetItemData (1, CHARMOVETYPE_BATTLE2);

	nNo = 0;
	nCount = m_ctlMoveType.GetCount ();
	for (i = 0; i < nCount; i ++) {
		if (m_nPutMoveType == m_ctlMoveType.GetItemData (i)) {
			nNo = i;
			break;
		}
	}
	m_ctlMoveType.SetCurSel (nNo);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifyTypePUTNPC::OnOK							 */
/* 内容		:ボタンハンドラ(OK)												 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

void CDlgAdminCharModifyTypePUTNPC::OnOK()
{
	UpdateData ();

	m_nPutMoveType = m_ctlMoveType.GetItemData (m_ctlMoveType.GetCurSel ());

	CDlgAdminCharModifyTypeBase::EndDialog (IDOK);
}

/* Copyright(C)URARA-works 2008 */
