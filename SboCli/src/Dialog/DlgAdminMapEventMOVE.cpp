/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminMapEventMOVE.cpp									 */
/* 内容			:マップイベント一覧ダイアログクラス 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/24													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoMapEventMOVE.h"
#include "MgrData.h"
#include "DlgAdminMapEventMOVE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminMapEventMOVE::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminMapEventNONE::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminMapEventMOVE)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_POSX, m_nPosX);
	DDX_Text(pDX, IDC_POSY, m_nPosY);
	DDX_Control(pDX, IDC_DIRECTION, m_ctlDirection);
	DDX_CBIndex(pDX, IDC_DIRECTION, m_nDirection);
}

BEGIN_MESSAGE_MAP(CDlgAdminMapEventMOVE, CDlgAdminMapEventNONE)
	//{{AFX_MSG_MAP(CDlgAdminMapEventMOVE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventMOVE::CDlgAdminMapEventMOVE					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/06/24														 */
/* ========================================================================= */

CDlgAdminMapEventMOVE::CDlgAdminMapEventMOVE(CWnd* pParent /*=NULL*/)
	: CDlgAdminMapEventNONE(pParent)
	, m_nPosX(0)
	, m_nPosY(0)
	, m_nDirection(-1)
{
	//{{AFX_DATA_INIT(CDlgAdminMapEventMOVE)
	//}}AFX_DATA_INIT

	m_nResourceID = CDlgAdminMapEventMOVE::IDD;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventMOVE::~CDlgAdminMapEventMOVE					 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/06/24														 */
/* ========================================================================= */

CDlgAdminMapEventMOVE::~CDlgAdminMapEventMOVE()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventMOVE::Set										 */
/* 内容		:設定															 */
/* 日付		:2008/06/26														 */
/* ========================================================================= */

void CDlgAdminMapEventMOVE::Set(CInfoMapEventBase *pSrc)
{
	PCInfoMapEventMOVE pSrcTmp = (PCInfoMapEventMOVE)pSrc;

	m_nPosX			= pSrcTmp->m_ptDst.x;
	m_nPosY			= pSrcTmp->m_ptDst.y;
	m_nDirection	= pSrcTmp->m_nDirection;
	if (m_nDirection < 0) {
		m_nDirection = 4;
	}

	UpdateData (FALSE);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventMOVE::Get										 */
/* 内容		:取得															 */
/* 日付		:2008/06/26														 */
/* ========================================================================= */

void CDlgAdminMapEventMOVE::Get(CInfoMapEventBase *pDst)
{
	PCInfoMapEventMOVE pDstTmp = (PCInfoMapEventMOVE)pDst;

	UpdateData ();

	pDstTmp->m_ptDst.x		= m_nPosX;
	pDstTmp->m_ptDst.y		= m_nPosY;
	pDstTmp->m_nDirection	= m_nDirection;
	if (m_nDirection > 3) {
		pDstTmp->m_nDirection = -1;
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventMOVE::OnInitDialog							 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/06/24														 */
/* ========================================================================= */

BOOL CDlgAdminMapEventMOVE::OnInitDialog()
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
/* 関数名	:CDlgAdminMapEventMOVE::OnAdminMsg								 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2008/06/28														 */
/* ========================================================================= */

LRESULT CDlgAdminMapEventMOVE::OnAdminMsg(WPARAM wParam, LPARAM lParam)
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
