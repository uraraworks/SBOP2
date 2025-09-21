/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* ファイル名：	DlgMsgLog.cpp												 */
/* 内容：		メッセージログダイアログクラス 実装ファイル					 */
/* 作成：		年がら年中春うらら(URARA-works)								 */
/* 作成開始日：	2005/09/25													 */
/* ========================================================================= */

#include "stdafx.h"
#include "PacketCHAR_REQ_CHAT.h"
#include "TextOutput.h"
#include "UraraSockTCPSBO.h"
#include "MgrData.h"
#include "MgrWindow.h"
#include "MainFrame.h"
#include "DlgMsgLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/* ========================================================================= */
/* クラス設定																 */
/* ========================================================================= */

void CDlgMsgLog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMsgLog)
	DDX_Control(pDX, IDC_LOG, m_wndLogViewCtrl);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_TOPMOST, m_bTopMost);
	DDX_Check(pDX, IDC_HIDE_MAINFRAME, m_bHideMainFrame);
	DDX_Text(pDX, IDC_CHAT, m_strChat);
}

BEGIN_MESSAGE_MAP(CDlgMsgLog, CDialog)
	//{{AFX_MSG_MAP(CDlgMsgLog)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVCN_LINK, IDC_LOG, OnLink)
	ON_BN_CLICKED(IDC_TOPMOST, &CDlgMsgLog::OnBnClickedTopMost)
	ON_BN_CLICKED(IDC_HIDE_MAINFRAME, &CDlgMsgLog::OnBnClickedHideMainframe)
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名：	CDlgMsgLog::CDlgMsgLog											 */
/* 内容：	コンストラクタ													 */
/* 日付：	2005/09/25														 */
/* ========================================================================= */

CDlgMsgLog::CDlgMsgLog(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMsgLog::IDD, pParent)
	, m_bTopMost(FALSE)
	, m_bHideMainFrame(FALSE)
	, m_strChat(_T(""))
{
	//{{AFX_DATA_INIT(CDlgMsgLog)
	//}}AFX_DATA_INIT

	m_pMgrData			= NULL;
	m_pLog				= new CTextOutput;
	m_hWndChat			= NULL;
	m_OrgWndProcChat	= NULL;
	m_bPushEnter		= FALSE;
}


/* ========================================================================= */
/* 関数名：	CDlgMsgLog::~CDlgMsgLog											 */
/* 内容：	デストラクタ													 */
/* 日付：	2005/09/25														 */
/* ========================================================================= */

CDlgMsgLog::~CDlgMsgLog()
{
	SAFE_DELETE (m_pLog);
}


/* ========================================================================= */
/* 関数名：	CDlgMsgLog::Create												 */
/* 内容：	作成															 */
/* 日付：	2005/09/25														 */
/* ========================================================================= */

BOOL CDlgMsgLog::Create(HWND hWndParent, CMgrData *pMgrData)
{
	CWnd *pWnd;

	m_pMgrData	= pMgrData;
	pWnd		= CWnd::FromHandle (hWndParent);

	CDialog::Create (IDD_MSGLOG, pWnd);

	ShowWindow (SW_SHOW);
	pWnd->SetFocus ();

	/* ログファイルの作成 */
	MakeLogFile ();

	return TRUE;
}


/* ========================================================================= */
/* 関数名：	CDlgMsgLog::Add													 */
/* 内容：	追加															 */
/* 日付：	2005/09/25														 */
/* ========================================================================= */

void CDlgMsgLog::Add(LPCSTR pszLog, COLORREF cl)
{
	CString strTmp;
	CTime time, timeTmp;

	if ((pszLog == NULL) || (strlen (pszLog) <= 0)) {
		return;
	}

	time = CTime::GetCurrentTime ();
	timeTmp = CTime::CTime (time.GetYear (), time.GetMonth (), time.GetDay (), 0, 0, 0);
	if (m_timeMakeLog != timeTmp) {
		/* 日付が変わったのでログファイルを再作成 */
		MakeLogFile ();
	}

	/* ログファイルに書き込み */
	strTmp.Format ("[%02d:%02d:%02d] %s", time.GetHour (), time.GetMinute (), time.GetSecond (), pszLog);
	m_pLog->Write ("%s", strTmp);

	m_wndLogViewCtrl.AddLine (pszLog, cl, RGB (40, 40, 40));
}


/* ========================================================================= */
/* 関数名：	CDlgMsgLog::MakeLogFile											 */
/* 内容：	現在時刻でログファイルを作成									 */
/* 日付：	2005/10/01														 */
/* ========================================================================= */

void CDlgMsgLog::MakeLogFile(void)
{
	char szName[MAX_PATH];
	LPSTR pszTmp;
	CString strTmp;
	CTime time;

	time = CTime::GetCurrentTime ();
	m_timeMakeLog = CTime::CTime (time.GetYear (), time.GetMonth (), time.GetDay (), 0, 0, 0);

//Todo:
	GetModuleFileName (NULL, szName, MAX_PATH);
	pszTmp	= PathFindFileName (szName);
	*pszTmp	= 0;
	PathAddBackslash (szName);

	strTmp.Format ("%sLog\\SBOログ(%d年%02d月%02d日).txt",
		szName,
		time.GetYear (),
		time.GetMonth (),
		time.GetDay ());
	/* ログファイルの作成 */
	m_pLog->Destroy ();
	m_pLog->Create (strTmp, FALSE, TRUE);
}


/* ========================================================================= */
/* 関数名：	CDlgMsgLog::OnInitDialog										 */
/* 内容：	メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付：	2005/09/26														 */
/* ========================================================================= */

BOOL CDlgMsgLog::OnInitDialog()
{
	CRect rc;
	LOGVIEWCTRLSETTING stLOGVIEWCTRLSETTING;

	CDialog::OnInitDialog ();

	::GetWindowRect (m_pMgrData->GetMainWindow (), rc);
	SetWindowPos (NULL, rc.right, rc.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);

	/* ログ表示コントロールの設定 */
	m_wndLogViewCtrl.GetSetting (&stLOGVIEWCTRLSETTING);
	strcpy (stLOGVIEWCTRLSETTING.szFontText,	"ＭＳ Ｐゴシック");
	strcpy (stLOGVIEWCTRLSETTING.szFontHeader,	"ＭＳ ゴシック");
	stLOGVIEWCTRLSETTING.crBack				= RGB (40, 40, 40);
	stLOGVIEWCTRLSETTING.crSelectBack		= RGB (40, 40, 255);
	stLOGVIEWCTRLSETTING.crSelectText		= RGB (255, 255, 255);
	stLOGVIEWCTRLSETTING.crLinkBack			= RGB (40, 40, 40);
	stLOGVIEWCTRLSETTING.crLinkText			= RGB (255, 255, 255);
	stLOGVIEWCTRLSETTING.nLinkNotifyType	= LVC_LINK_NOTIFY_DOUBLE_CLICK;
	m_wndLogViewCtrl.SetSetting (&stLOGVIEWCTRLSETTING);
	m_wndLogViewCtrl.SetTextLimit (LVC_TEXT_LIMIT_LINE, 3000);

	CLayoutHelper::Initialize (m_hWnd);
	RegisterControl (m_wndLogViewCtrl.m_hWnd,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);
	RegisterControl (IDC_ONLINE,				LH_CTRL_Y);
	RegisterControl (IDC_CHARCOUNT,				LH_CTRL_Y);
	RegisterControl (IDC_TOPMOST,				LH_CTRL_X | LH_CTRL_Y);
	RegisterControl (IDC_HIDE_MAINFRAME,		LH_CTRL_X | LH_CTRL_Y);
	RegisterControl (IDC_CHAT,					LH_CTRL_WIDTH | LH_CTRL_Y);

	m_hWndChat = GetDlgItem (IDC_CHAT)->GetSafeHwnd ();
	m_OrgWndProcChat = (WNDPROC)GetWindowLong (m_hWndChat, GWL_WNDPROC);
	::SendMessage		(m_hWndChat, WM_SETFONT, (WPARAM)GetStockObject (DEFAULT_GUI_FONT), 0);
	SetWindowLong		(m_hWndChat, GWL_USERDATA, (LONG)this);
	SetWindowLong		(m_hWndChat, GWL_WNDPROC, (LONG)ChatWndProc);

	return TRUE;
}


/* ========================================================================= */
/* 関数名：	CDlgMsgLog::OnSize												 */
/* 内容：	メッセージハンドラ(WM_SIZE)										 */
/* 日付：	2005/09/25														 */
/* ========================================================================= */

void CDlgMsgLog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize (nType, cx, cy);

	DoLayout (cx, cy);
}


/* ========================================================================= */
/* 関数名	:CDlgMsgLog::PostNcDestroy										 */
/* 内容		:後処理															 */
/* 日付		:2007/04/12														 */
/* ========================================================================= */

void CDlgMsgLog::PostNcDestroy()
{
	HIMC hImc;

	if (m_hWndChat) {
		/* IMEをオフにする */
		hImc = ImmGetContext (m_hWndChat);
		ImmSetOpenStatus (hImc, FALSE);
		ImmReleaseContext (m_hWndChat, hImc);

		SetWindowLong (m_hWndChat, GWL_WNDPROC, (LONG)m_OrgWndProcChat);
		m_OrgWndProcChat = NULL;
	}
	delete this;
}


/* ========================================================================= */
/* 関数名	:CDlgMsgLog::OnLink												 */
/* 内容		:ログコントロールからのリンク通知								 */
/* 日付		:2007/04/12														 */
/* ========================================================================= */

void CDlgMsgLog::OnLink(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCLINK* pnmLink = reinterpret_cast<NMLVCLINK*>(pNMHDR);

	ShellExecute (NULL, "open", pnmLink->pszLink, NULL, NULL, SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CDlgMsgLog::OnBnClickedTopMost									 */
/* 内容		:ボタンハンドラ(最前面)											 */
/* 日付		:2008/08/30														 */
/* ========================================================================= */

void CDlgMsgLog::OnBnClickedTopMost()
{
	UpdateData ();

	if (m_bTopMost) {
		SetWindowPos (&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	} else {
		SetWindowPos (&CWnd::wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
}


/* ========================================================================= */
/* 関数名	:CDlgMsgLog::OnBnClickedHideMainframe							 */
/* 内容		:ボタンハンドラ(メインウィンドウ非表示)							 */
/* 日付		:2008/06/08														 */
/* ========================================================================= */

void CDlgMsgLog::OnBnClickedHideMainframe()
{
	int nCmdShow;
	HWND hWnd;

	UpdateData ();
	nCmdShow = (m_bHideMainFrame) ? SW_HIDE : SW_SHOW;

	::ShowWindow (m_pMgrData->GetMainWindow (), nCmdShow);
	hWnd = m_pMgrData->GetAdminWindow ();
	if (hWnd) {
		::ShowWindow (hWnd, nCmdShow);
	}
	hWnd = m_pMgrData->GetDebugWindow ();
	if (hWnd) {
		::ShowWindow (hWnd, nCmdShow);
	}
}


/* ========================================================================= */
/* 関数名	:CDlgMsgLog::ChatWndProc										 */
/* 内容		:チャット入力欄プロシージャ										 */
/* 日付		:2008/06/08														 */
/* ========================================================================= */

LRESULT CALLBACK CDlgMsgLog::ChatWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char szTmp[256];
	PCDlgMsgLog pThis;
	LRESULT hResult;
	CMainFrame *pMainFrame;
	CmyString strTmp;

	pThis	= (PCDlgMsgLog)GetWindowLong (hWnd, GWL_USERDATA);
	hResult	= -1;

	switch (message) {
	case WM_CHAR:
		if (wParam == VK_RETURN) {
			hResult = 0;

		} else if (wParam == VK_ESCAPE) {
			hResult = 0;
		}
		break;

	case WM_KEYDOWN:
		switch (wParam) {
		case VK_RETURN:
			pThis->m_bPushEnter = TRUE;
			hResult = 0;
			break;
		case VK_ESCAPE:
			hResult = 0;
			break;
		}
		break;

	case WM_KEYUP:
		switch (wParam) {
		case VK_RETURN:
			if (pThis->m_bPushEnter == FALSE) {
				break;
			}
			pThis->UpdateData ();
			if (pThis->m_strChat.GetLength () > 0) {
				pMainFrame = pThis->m_pMgrData->GetMainFrame ();
				ZeroMemory (szTmp, sizeof (szTmp));
				_tcsnccat (szTmp, pThis->m_strChat, 100);
				TrimViewString (strTmp, szTmp);
				pThis->m_strChat = strTmp;
				::PostMessage (pThis->m_pMgrData->GetMainWindow (), WM_WINDOWMSG, WINDOWTYPE_CHAT, 0);

				pMainFrame->SendChat (0, (LPCSTR)strTmp, NULL);
			}
			pThis->m_strChat.Empty ();
			pThis->UpdateData (FALSE);
			pThis->m_bPushEnter = FALSE;
			hResult = 0;
			break;
		case VK_ESCAPE:
			hResult = 0;
			break;
		}
		break;
	}

	if (hResult != 0) {
		hResult = CallWindowProc (pThis->m_OrgWndProcChat, hWnd, message, wParam, lParam);
	}
	return hResult;
}

/* Copyright(C)URARA-works 2005 */
