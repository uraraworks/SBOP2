/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* ファイル名	:DlgDbg.cpp													 */
/* 内容			:デバッグ情報ダイアログクラス 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/04/05													 */
/* ========================================================================= */

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "MgrData.h"
#include "InfoCharCli.h"
#include "MgrWindow.h"
#include "MainFrame.h"
#include "DlgDbg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/* ========================================================================= */
/* クラス設定																 */
/* ========================================================================= */

void CDlgDbg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDbg)
	DDX_Text(pDX, IDC_ONLINE, m_strOnline);
	DDX_Text(pDX, IDC_CHARCOUNT, m_strCharCount);
	DDX_Text(pDX, IDC_PING, m_strPing);
	DDX_Text(pDX, IDC_POS, m_strPos);
	DDX_Text(pDX, IDC_THROWGHPUT, m_strThrowghput);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgDbg, CDialog)
	//{{AFX_MSG_MAP(CDlgDbg)
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgDbg::CDlgDbg												 */
/* 内容		:コンストラクタ													 */
/* 日付		:2009/04/05														 */
/* ========================================================================= */

CDlgDbg::CDlgDbg(CWnd* pParent /*=NULL*/)
: CDialog(CDlgDbg::IDD, pParent)
, m_pSock(NULL)
, m_strOnline(_T(""))
, m_strCharCount(_T(""))
, m_strPing(_T(""))
, m_strPos(_T(""))
, m_strThrowghput(_T(""))
{
	//{{AFX_DATA_INIT(CDlgDbg)
	//}}AFX_DATA_INIT

	m_pMgrData 	= NULL;
}


/* ========================================================================= */
/* 関数名	:CDlgDbg::~CDlgDbg												 */
/* 内容		:デストラクタ													 */
/* 日付		:2009/04/05														 */
/* ========================================================================= */

CDlgDbg::~CDlgDbg()
{
}


/* ========================================================================= */
/* 関数名	:CDlgDbg::Create												 */
/* 内容		:作成															 */
/* 日付		:2009/04/05														 */
/* ========================================================================= */

BOOL CDlgDbg::Create(HWND hWndParent, CMgrData *pMgrData)
{
	CWnd *pWnd;

	m_pMgrData	= pMgrData;
	m_pSock		= m_pMgrData->GetUraraSockTCP ();
	pWnd		= CWnd::FromHandle (hWndParent);

	CDialog::Create (IDD, pWnd);

	ShowWindow (SW_SHOW);
	pWnd->SetFocus ();

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgDbg::Renew													 */
/* 内容		:更新															 */
/* 日付		:2009/04/05														 */
/* ========================================================================= */

void CDlgDbg::Renew(void)
{
	SetTimer(100, 100, NULL);
}


/* ========================================================================= */
/* 関数名	:CDlgDbg::OnInitDialog											 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2009/04/05														 */
/* ========================================================================= */

BOOL CDlgDbg::OnInitDialog()
{
	char szFileName[MAX_PATH];
	CRect rc;
	POINT pt;

	CDialog::OnInitDialog ();

	::GetWindowRect (m_pMgrData->GetMainWindow (), rc);
	SetWindowPos (NULL,
		rc.right + GetSystemMetrics (SM_CXFIXEDFRAME) * 2, rc.top,
		0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);

	Initialize (m_hWnd);
	SetTimer(100, 1000, NULL);

	ZeroMemory (szFileName, sizeof (szFileName));
	GetModuleFileName (NULL, szFileName, MAX_PATH);
	strcpy (szFileName + strlen (szFileName) - 3, "ini");

	m_pMgrData->SetDebugWindow (m_hWnd);
	pt.x = GetPrivateProfileInt ("Pos", "DebugX", -1, szFileName);
	pt.y = GetPrivateProfileInt ("Pos", "DebugY", -1, szFileName);
	if (!((pt.x == -1) && (pt.y == -1))) {
		SetWindowPos (NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
	}

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgDbg::OnSize												 */
/* 内容		:メッセージハンドラ(WM_SIZE)									 */
/* 日付		:2009/04/05														 */
/* ========================================================================= */

void CDlgDbg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize (nType, cx, cy);

	DoLayout (cx, cy);
}


/* ========================================================================= */
/* 関数名	:CDlgDbg::OnTimer												 */
/* 内容		:メッセージハンドラ(WM_TIMER)									 */
/* 日付		:2009/04/05														 */
/* ========================================================================= */

void CDlgDbg::OnTimer(UINT nIDEvent)
{
	PCInfoCharCli pChar;

	m_strOnline.Format("オンライン：%d", m_pMgrData->GetOnlineCount ());
	m_strCharCount.Format("キャラ数：%d", m_pMgrData->GetCharCount ());
	m_strPing.Format("Ping：%dms 描画時間:%4dms", m_pMgrData->GetPing (), m_pMgrData->GetDrawTime ());
	m_strPos = "座標：";
	pChar = m_pMgrData->GetPlayerChar ();
	if (pChar) {
		m_strPos.Format("座標：MAP:%d X:%3d Y:%3d", pChar->m_dwMapID, pChar->m_nMapX, pChar->m_nMapY);
	}
	m_strThrowghput.Format("送信：%5dBps 受信：%5dBps", m_pSock->GetThrowghPutSend (0), m_pSock->GetThrowghPutRecv (0));

	UpdateData(FALSE);
	SetTimer(100, 1000, NULL);
}


/* ========================================================================= */
/* 関数名	:CDlgDbg::PostNcDestroy											 */
/* 内容		:後処理															 */
/* 日付		:2009/04/05														 */
/* ========================================================================= */

void CDlgDbg::PostNcDestroy()
{
	delete this;
}

/* Copyright(C)URARA-works 2009 */
