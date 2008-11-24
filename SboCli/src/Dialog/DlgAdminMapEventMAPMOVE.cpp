/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminMapEventMAPMOVE.cpp								 */
/* 内容			:マップイベント(マップ間移動)ダイアログクラス 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/07/27													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoMapEventMAPMOVE.h"
#include "MgrData.h"
#include "DlgAdminMapEventMAPMOVE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminMapEventMAPMOVE::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminMapEventNONE::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminMapEventMAPMOVE)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_MAPID, m_dwMapID);
	DDX_Text(pDX, IDC_POSX, m_nPosX);
	DDX_Text(pDX, IDC_POSY, m_nPosY);
	DDX_Control(pDX, IDC_DIRECTION, m_ctlDirection);
	DDX_CBIndex(pDX, IDC_DIRECTION, m_nDirection);
}

BEGIN_MESSAGE_MAP(CDlgAdminMapEventMAPMOVE, CDlgAdminMapEventNONE)
	//{{AFX_MSG_MAP(CDlgAdminMapEventMAPMOVE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventMAPMOVE::CDlgAdminMapEventMAPMOVE				 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/07/27														 */
/* ========================================================================= */

CDlgAdminMapEventMAPMOVE::CDlgAdminMapEventMAPMOVE(CWnd* pParent /*=NULL*/)
	: CDlgAdminMapEventNONE(pParent)
	, m_dwMapID(0)
	, m_nPosX(0)
	, m_nPosY(0)
	, m_nDirection(-1)
{
	//{{AFX_DATA_INIT(CDlgAdminMapEventMAPMOVE)
	//}}AFX_DATA_INIT

	m_nResourceID = CDlgAdminMapEventMAPMOVE::IDD;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventMAPMOVE::~CDlgAdminMapEventMAPMOVE			 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/07/27														 */
/* ========================================================================= */

CDlgAdminMapEventMAPMOVE::~CDlgAdminMapEventMAPMOVE()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventMAPMOVE::Set									 */
/* 内容		:設定															 */
/* 日付		:2008/06/26														 */
/* ========================================================================= */

void CDlgAdminMapEventMAPMOVE::Set(CInfoMapEventBase *pSrc)
{
	PCInfoMapEventMAPMOVE pSrcTmp = (PCInfoMapEventMAPMOVE)pSrc;

	m_dwMapID		= pSrcTmp->m_dwMapID;
	m_nPosX			= pSrcTmp->m_ptDst.x;
	m_nPosY			= pSrcTmp->m_ptDst.y;
	m_nDirection	= pSrcTmp->m_nDirection;
	if (m_nDirection < 0) {
		m_nDirection = 4;
	}

	UpdateData (FALSE);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventMAPMOVE::Get									 */
/* 内容		:取得															 */
/* 日付		:2008/06/26														 */
/* ========================================================================= */

void CDlgAdminMapEventMAPMOVE::Get(CInfoMapEventBase *pDst)
{
	PCInfoMapEventMAPMOVE pDstTmp = (PCInfoMapEventMAPMOVE)pDst;

	UpdateData ();

	pDstTmp->m_dwMapID		= m_dwMapID;
	pDstTmp->m_ptDst.x		= m_nPosX;
	pDstTmp->m_ptDst.y		= m_nPosY;
	pDstTmp->m_nDirection	= m_nDirection;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventMAPMOVE::OnInitDialog							 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/07/27														 */
/* ========================================================================= */

BOOL CDlgAdminMapEventMAPMOVE::OnInitDialog()
{
	CDlgAdminMapEventNONE::OnInitDialog();

	m_ctlDirection.InsertString (0, "上");
	m_ctlDirection.InsertString (1, "下");
	m_ctlDirection.InsertString (2, "左");
	m_ctlDirection.InsertString (3, "右");
	m_ctlDirection.InsertString (4, "指定無し");

	if (m_nDirection < 0) {
		m_nDirection = 4;
	}
	m_ctlDirection.SetCurSel (m_nDirection);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventMAPMOVE::OnAdminMsg							 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2008/06/28														 */
/* ========================================================================= */

LRESULT CDlgAdminMapEventMAPMOVE::OnAdminMsg(WPARAM wParam, LPARAM lParam)
{
	switch (wParam) {
	case ADMINMSG_NOTIFYTYPE_RBUTTONDOWN:		/* 右クリック通知 */
		m_nPosX = HIWORD (lParam);
		m_nPosY = LOWORD (lParam);
		UpdateData (FALSE);
		break;
	}

	return -1;
}

/* Copyright(C)URARA-works 2008 */
