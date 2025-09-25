/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:SboLaunchDlg.cpp											 */
/* 内容			:メインダイアログ 実装ファイル								 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/01/27													 */
/* ========================================================================= */

#include "stdafx.h"
#include "SboLaunch.h"
#include "GetMD5File.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "InfoFileList.h"
#include "SboLaunchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CSboLaunchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSboLaunchDlg)
	DDX_Control(pDX, IDC_PROGRESS_FILE, m_ctlProgressFile);
	DDX_Control(pDX, IDC_PROGRESS_ALL, m_ctlProgressAll);
	DDX_Control(pDX, IDC_LOG, m_edLog);
	DDX_Text(pDX, IDC_FILE, m_strFile);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSboLaunchDlg, CDialog)
	//{{AFX_MSG_MAP(CSboLaunchDlg)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CSboLaunchDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_HISTORY, &CSboLaunchDlg::OnBnClickedHistory)
	ON_BN_CLICKED(IDC_HOWTO, &CSboLaunchDlg::OnBnClickedHowto)
	ON_BN_CLICKED(IDC_RETRY, &CSboLaunchDlg::OnBnClickedRetry)
END_MESSAGE_MAP()

#define TIMERID_CONNECT		(100)			/* サーバーへ接続するタイマー */
#define TIMERID_CHECKFILE	(101)			/* ファイルリストをチェックするタイマー */
#define TIMERID_GETFILE		(102)			/* ファイルを取得するタイマー */


/* ========================================================================= */
/* 関数名	:CSboLaunchDlg::CSboLaunchDlg									 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/01/27														 */
/* ========================================================================= */

CSboLaunchDlg::CSboLaunchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSboLaunchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSboLaunchDlg)
	m_strFile = _T("");
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bConnect			= FALSE;
	m_nState			= GAMESTATE_NONE;
	m_pSock				= NULL;
	m_wServerPort		= 0;
	m_pInfoFileList		= NULL;
	m_pstCheckFlieList	= NULL;
	m_pstGetFileInfo	= NULL;
}


/* ========================================================================= */
/* 関数名	:CSboLaunchDlg::~CSboLaunchDlg									 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/01/27														 */
/* ========================================================================= */

CSboLaunchDlg::~CSboLaunchDlg()
{
	SAFE_DELETE (m_pSock);
	SAFE_DELETE (m_pInfoFileList);
	SAFE_DELETE (m_pstCheckFlieList);
	SAFE_DELETE (m_pstGetFileInfo);
}


/* ========================================================================= */
/* 関数名	:CSboLaunchDlg::OnInitDialog									 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/01/27														 */
/* ========================================================================= */

BOOL CSboLaunchDlg::OnInitDialog()
{
	CDialog::OnInitDialog ();

	SetIcon (m_hIcon, TRUE);

	m_pSock = new CUraraSockTCPSBO;
	ReadIniData ();

	GetDlgItem (IDC_RETRY)->EnableWindow (FALSE);
	GetDlgItem (IDOK)->EnableWindow (FALSE);

	SetTimer (TIMERID_CONNECT, 500, NULL);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CSboLaunchDlg::OnEraseBkgnd									 */
/* 内容		:メッセージハンドラ(WM_ERASEBKGND)								 */
/* 日付		:2008/01/27														 */
/* ========================================================================= */

BOOL CSboLaunchDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rc, rcMsg;
	CBrush Brush;
	CWnd *pWnd;
	HICON hIcon;

	CDialog::OnEraseBkgnd(pDC);

	GetClientRect (&rc);
	rc.bottom = 50;

	pWnd = GetDlgItem (IDC_MSG);
	if (pWnd == NULL) {
		goto Exit;
	}

	/* メッセージ欄を塗りつぶし */
	pWnd->GetClientRect (&rcMsg);
	pDC->FillSolidRect (rc, RGB (255, 255,255));

	/* セットアップアイコンを表示 */
	hIcon = (HICON)LoadImage (AfxGetInstanceHandle (), MAKEINTRESOURCE (IDR_MAINFRAME), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	pDC->DrawIcon (rc.right - 48, 8, hIcon);
	DestroyIcon (hIcon);

	/* メッセージ欄下の線を引く */
	rc.top = 50;
	rc.bottom = rc.top + 2;
	Brush.CreateSysColorBrush (COLOR_3DHILIGHT);
	pDC->FillRect (rc, &Brush);
	Brush.DeleteObject ();

	rc.bottom -= 1;
	Brush.CreateSysColorBrush (COLOR_3DSHADOW);
	pDC->FillRect (rc, &Brush);
	Brush.DeleteObject ();

Exit:
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CSboLaunchDlg::OnTimer											 */
/* 内容		:メッセージハンドラ(WM_TIMER)									 */
/* 日付		:2008/03/08														 */
/* ========================================================================= */

void CSboLaunchDlg::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent) {
	case TIMERID_CONNECT:		/* サーバーへ接続するタイマー */
		ChgState (GAMESTATE_CONNECT);
		KillTimer (TIMERID_CONNECT);
		break;
	case TIMERID_CHECKFILE:		/* ファイルリストをチェックするタイマー */
		ProcCHECKFILELIST ();
		break;
	}

	CDialog::OnTimer(nIDEvent);
}


/* ========================================================================= */
/* 関数名	:CSboLaunchDlg::OnCtlColor										 */
/* 内容		:メッセージハンドラ(WM_CTLCOLOR)								 */
/* 日付		:2008/01/27														 */
/* ========================================================================= */

HBRUSH CSboLaunchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor) {
	case CTLCOLOR_STATIC:		/* スタティックコントロール */
		/* メッセージ欄？*/
		if (pWnd->m_hWnd == GetDlgItem (IDC_MSG)->m_hWnd) {
			pDC->SetBkMode (TRANSPARENT);
			return (HBRUSH)GetStockObject (WHITE_BRUSH);
		}
		break;
	}
	
	return hbr;
}


/* ========================================================================= */
/* 関数名	:CSboLaunchDlg::OnBnClickedRetry								 */
/* 内容		:ボタンハンドラ(再チェック)										 */
/* 日付		:2008/05/28														 */
/* ========================================================================= */

void CSboLaunchDlg::OnBnClickedRetry()
{
	ChgState (GAMESTATE_CONNECT);
}


/* ========================================================================= */
/* 関数名	:CSboLaunchDlg::OnBnClickedOk									 */
/* 内容		:ボタンハンドラ(ゲーム開始)										 */
/* 日付		:2008/05/25														 */
/* ========================================================================= */

void CSboLaunchDlg::OnBnClickedOk()
{
	BOOL bResult;
	char szPath[MAX_PATH], szTmp[MAX_PATH];
	STARTUPINFO stStartupInfo;
	PROCESS_INFORMATION stProcInfo;
	CmyString strFileName;

        GetModuleFilePath (szPath, sizeof (szPath));
        CString strModulePath = Utf8ToTString (szPath);
        strFileName.Format(_T("%sSboCli.exe"), (LPCTSTR)strModulePath);

	ZeroMemory (&stStartupInfo, sizeof (stStartupInfo));
	ZeroMemory (&stProcInfo, sizeof (stProcInfo));
	stStartupInfo.cb = sizeof (stStartupInfo);

	strcpy (szTmp, strFileName);
	bResult = CreateProcess (NULL, szTmp, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, szPath,  &stStartupInfo, &stProcInfo);
        if (bResult == FALSE) {
                MessageBox (_T("クライアントの起動に失敗しました"), _T("エラー"), MB_OK);
		goto Exit;
	}

Exit:
	if (stProcInfo.hProcess) {
		CloseHandle (stProcInfo.hProcess);
	}
	if (stProcInfo.hThread) {
		CloseHandle (stProcInfo.hThread);
	}

	OnOK();
}


/* ========================================================================= */
/* 関数名	:CSboLaunchDlg::OnBnClickedOk									 */
/* 内容		:ボタンハンドラ(更新履歴)										 */
/* 日付		:2008/05/25														 */
/* ========================================================================= */

void CSboLaunchDlg::OnBnClickedHistory()
{
	char szPath[MAX_PATH];
	CmyString strFileName;

        GetModuleFilePath (szPath, sizeof (szPath));
        CString strModulePath = Utf8ToTString (szPath);
        strFileName.Format(_T("%s更新履歴.txt"), (LPCTSTR)strModulePath);
	ShellExecute (NULL, "open", strFileName, NULL, NULL, SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CSboLaunchDlg::OnBnClickedOk									 */
/* 内容		:ボタンハンドラ(遊び方)											 */
/* 日付		:2008/05/25														 */
/* ========================================================================= */

void CSboLaunchDlg::OnBnClickedHowto()
{
	ShellExecute (NULL, "open", "http://uraran.jp/sbo/key.shtml", NULL, NULL, SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CSboLaunchDlg::DefWindowProc									 */
/* 内容		:メッセージハンドラ												 */
/* 日付		:2008/02/25														 */
/* ========================================================================= */

LRESULT CSboLaunchDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	int nState;

	if ((message >= URARASOCK_MSGBASE) && (message < URARASOCK_MSGBASE + WM_URARASOCK_MAX)) {
		switch (message - URARASOCK_MSGBASE) {
		case WM_URARASOCK_CONNECT:				/* サーバーに接続した */
			AddMsg ("サーバーに接続しました\r\n");
			m_bConnect = TRUE;
			ChgState (GAMESTATE_GETFILELIST);
			break;
		case WM_URARASOCK_DISCONNECT:			/* サーバーと切断した */
			nState = GAMESTATE_DISCONNECT;
			if (m_bConnect == FALSE) {
				nState = GAMESTATE_CONNECTMISS;
			}
			ChgState (nState);
			break;
		case WM_URARASOCK_RECV:					/* データ受信 */
			OnRecv ((PBYTE)wParam);
			break;
		}
		return 0;
	}

	return CDialog::DefWindowProc(message, wParam, lParam);
}


/* ========================================================================= */
/* 関数名	:CSboLaunchDlg::AddMsg											 */
/* 内容		:メッセージ追加													 */
/* 日付		:2008/02/25														 */
/* ========================================================================= */

void CSboLaunchDlg::AddMsg(LPCSTR pszMsg)
{
	m_edLog.SetSel (-1);
	m_edLog.ReplaceSel (pszMsg);
}


/* ========================================================================= */
/* 関数名	:CSboLaunchDlg::ReadIniData										 */
/* 内容		:iniファイルから設定を読み込み									 */
/* 日付		:2008/01/27														 */
/* ========================================================================= */

void CSboLaunchDlg::ReadIniData(void)
{
        TCHAR szFileName[MAX_PATH];
        TCHAR szTmp[128];

        ZeroMemory (szFileName, sizeof (szFileName));
        ZeroMemory (szTmp, sizeof (szTmp));

        GetModuleFilePath (szFileName, _countof (szFileName));
        _tcscat_s (szFileName, _T("SboCli.ini"));

        /* サーバーアドレス */
        GetPrivateProfileString (_T("Setting"), _T("ServerAddr"), _T("127.0.0.1"), szTmp, _countof (szTmp), szFileName);
        m_strServerAddr = szTmp;
        /* 待ちうけポート */
        m_wServerPort = static_cast<WORD>(GetPrivateProfileInt (_T("Setting"), _T("ServerPort"), 2006, szFileName));
}


/* ========================================================================= */
/* 関数名	:CSboLaunchDlg::ChgState										 */
/* 内容		:状態遷移														 */
/* 日付		:2008/02/25														 */
/* ========================================================================= */

void CSboLaunchDlg::ChgState(int nState)
{
	m_nState = nState;
	m_ctlProgressFile.SetRange (0, 0);
	m_ctlProgressAll. SetRange (0, 0);

	switch (nState) {
	case GAMESTATE_NONE:				/* 停止中 */
		ChgStateNONE ();
		break;
	case GAMESTATE_CONNECT:				/* サーバーへ接続 */
		ChgStateCONNECT ();
		break;
	case GAMESTATE_CONNECTMISS:			/* サーバーへ接続失敗 */
		ChgStateCONNECTMISS ();
		break;
	case GAMESTATE_DISCONNECT:			/* サーバーと切断 */
		ChgStateDISCONNECT ();
		break;
	case GAMESTATE_GETFILELIST:			/* ファイルリスト取得 */
		ChgStateGETFILELIST ();
		break;
	case GAMESTATE_CHECKFILELIST:		/* ファイルリストチェック */
		ChgStateCHECKFILELIST ();
		break;
	case GAMESTATE_GETFILE:				/* ファイル取得 */
		ChgStateGETFILE ();
		break;
	case GAMESTATE_MAKEFILELIST:		/* ファイルリスト出力 */
		ChgStateMAKEFILELIST ();
		break;
	case GAMESTATE_RUNUPDATE:			/* アップデータ起動 */
		ChgStateRUNUPDATE ();
		break;
	case GAMESTATE_QUIT:				/* 終了 */
		ChgStateQUIT ();
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CSboLaunchDlg::ChgStateNONE									 */
/* 内容		:状態遷移(停止中)												 */
/* 日付		:2008/03/09														 */
/* ========================================================================= */

void CSboLaunchDlg::ChgStateNONE(void)
{
	SAFE_DELETE (m_pInfoFileList);
	SAFE_DELETE (m_pstCheckFlieList);
	if (m_pstGetFileInfo) {
		SAFE_DELETE_ARRAY (m_pstGetFileInfo->pRecvDataTmp);
	}
	SAFE_DELETE (m_pstGetFileInfo);
	m_pSock->Destroy ();
}


/* ========================================================================= */
/* 関数名	:CSboLaunchDlg::ChgStateCONNECT									 */
/* 内容		:状態遷移(サーバーへ接続)										 */
/* 日付		:2008/02/25														 */
/* ========================================================================= */

void CSboLaunchDlg::ChgStateCONNECT(void)
{
	BOOL bResult;

	m_bConnect = FALSE;
	GetDlgItem (IDC_RETRY)->EnableWindow (FALSE);

	AddMsg ("サーバーへ接続します...\r\n");
	bResult = m_pSock->Connect (m_hWnd, URARASOCK_MSGBASE, URARASOCK_PRECHECK, m_wServerPort, m_strServerAddr);
	if (bResult == FALSE) {
		ChgState (GAMESTATE_CONNECTMISS);
	}
}


/* ========================================================================= */
/* 関数名	:CSboLaunchDlg::ChgStateCONNECTMISS								 */
/* 内容		:状態遷移(サーバーへ接続失敗)									 */
/* 日付		:2008/02/25														 */
/* ========================================================================= */

void CSboLaunchDlg::ChgStateCONNECTMISS(void)
{
	AddMsg ("サーバーに接続できませんでした\r\n");
	ChgState (GAMESTATE_NONE);
	GetDlgItem (IDC_RETRY)->EnableWindow (TRUE);
}


/* ========================================================================= */
/* 関数名	:CSboLaunchDlg::ChgStateDISCONNECT								 */
/* 内容		:状態遷移(サーバーと切断)										 */
/* 日付		:2008/02/25														 */
/* ========================================================================= */

void CSboLaunchDlg::ChgStateDISCONNECT(void)
{
	AddMsg ("サーバーと切断しました\r\n");
	ChgState (GAMESTATE_NONE);
	GetDlgItem (IDC_RETRY)->EnableWindow (TRUE);
}


/* ========================================================================= */
/* 関数名	:CSboLaunchDlg::ChgStateGETFILELIST								 */
/* 内容		:状態遷移(ファイルリスト取得)									 */
/* 日付		:2008/02/25														 */
/* ========================================================================= */

void CSboLaunchDlg::ChgStateGETFILELIST(void)
{
	CPacketVERSION_REQ_FILELIST Packet;

	AddMsg ("ファイルリストを取得しています...\r\n");
	Packet.Make ();
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* 関数名	:CSboLaunchDlg::ChgStateCHECKFILELIST							 */
/* 内容		:状態遷移(ファイルリストチェック)								 */
/* 日付		:2008/03/09														 */
/* ========================================================================= */

void CSboLaunchDlg::ChgStateCHECKFILELIST(void)
{
	int nCount;

	AddMsg ("更新するファイルをチェックしています\r\n");

	SAFE_DELETE (m_pstCheckFlieList);
	m_pstCheckFlieList = new CHECKFILELIST;
	m_pstCheckFlieList->nNo = 0;

	SetTimer (TIMERID_CHECKFILE, 10, NULL);

	nCount = m_pInfoFileList->GetCount ();
	m_ctlProgressFile.SetRange (0, nCount);
	m_ctlProgressAll. SetRange (0, nCount);
}


/* ========================================================================= */
/* 関数名	:CSboLaunchDlg::ChgStateGETFILE									 */
/* 内容		:状態遷移(ファイル取得)											 */
/* 日付		:2008/03/09														 */
/* ========================================================================= */

void CSboLaunchDlg::ChgStateGETFILE(void)
{
	int nCount;

	AddMsg ("ファイルをダウンロードしています...\r\n");

	m_pstGetFileInfo = new GETFILEINFO;
	m_pstGetFileInfo->nNo			= 0;
	m_pstGetFileInfo->dwRecvSize	= 0;
	m_pstGetFileInfo->pRecvDataTmp	= NULL;

	nCount = m_pInfoFileList->GetCount ();
	m_ctlProgressAll.SetRange (0, nCount);

	ProcGETFILE ();
}


/* ========================================================================= */
/* 関数名	:CSboLaunchDlg::ChgStateMAKEFILELIST							 */
/* 内容		:状態遷移(ファイルリスト出力)									 */
/* 日付		:2008/03/15														 */
/* ========================================================================= */

void CSboLaunchDlg::ChgStateMAKEFILELIST(void)
{
	int i, nCount;
	HANDLE hFile;
	DWORD dwBytes;
	char szPath[MAX_PATH];
	PFILELISTINFO pInfo;
	CmyString strFileName, strTmp;

        GetModuleFilePath (szPath, sizeof (szPath));
        CString strModulePath = Utf8ToTString (szPath);
        strFileName.Format(_T("%sUpdate\\FileList.txt"), (LPCTSTR)strModulePath);

	/* ファイルを開く */
	hFile = CreateFile (strFileName, GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE) {
		strTmp.Format(_T("ファイルリストの出力に失敗しました[%d]\r\n"), GetLastError ());
		AddMsg ((LPCSTR)strTmp);
		return;
	}

	nCount = m_pstCheckFlieList->anGetFlieNo.size();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_pInfoFileList->GetPtr (m_pstCheckFlieList->anGetFlieNo[i]);
		strTmp = pInfo->strFileName;
		strTmp += "\r\n";

		dwBytes = 0;
		WriteFile (hFile, (PBYTE)(LPCSTR)strTmp, strTmp.GetLength (), &dwBytes, NULL);
	}

	CloseHandle (hFile);
	ChgState (GAMESTATE_RUNUPDATE);
}


/* ========================================================================= */
/* 関数名	:CSboLaunchDlg::ChgStateRUNUPDATE								 */
/* 内容		:状態遷移(アップデータ起動)										 */
/* 日付		:2008/03/15														 */
/* ========================================================================= */

void CSboLaunchDlg::ChgStateRUNUPDATE(void)
{
	BOOL bResult;
	char szPath[MAX_PATH], szTmp[MAX_PATH];
	STARTUPINFO stStartupInfo;
	PROCESS_INFORMATION stProcInfo;
	CmyString strFileName, strTmp;

	AddMsg ("ファイルを更新しています\r\n");

        GetModuleFilePath (szPath, sizeof (szPath));
        strcat (szPath, "Update\\");
        CString strModulePath = Utf8ToTString (szPath);
        strFileName.Format(_T("%sSboCopy.exe"), (LPCTSTR)strModulePath);

	ZeroMemory (&stStartupInfo, sizeof (stStartupInfo));
	ZeroMemory (&stProcInfo, sizeof (stProcInfo));
	stStartupInfo.cb = sizeof (stStartupInfo);

	strcpy (szTmp, strFileName);
	bResult = CreateProcess (NULL, szTmp, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, szPath,  &stStartupInfo, &stProcInfo);
	if (bResult == FALSE) {
		strTmp.Format(_T("アップデータの起動に失敗しました[%d]\r\n"), GetLastError ());
		AddMsg ((LPCSTR)strTmp);
		goto Exit;
	}

Exit:
	if (stProcInfo.hProcess) {
		CloseHandle (stProcInfo.hProcess);
	}
	if (stProcInfo.hThread) {
		CloseHandle (stProcInfo.hThread);
	}
}


/* ========================================================================= */
/* 関数名	:CSboLaunchDlg::ChgStateQUIT									 */
/* 内容		:状態遷移(終了)													 */
/* 日付		:2008/03/09														 */
/* ========================================================================= */

void CSboLaunchDlg::ChgStateQUIT(void)
{
	char szPath[MAX_PATH];

	AddMsg ("ファイルのチェックが終了しました\r\n");
	m_pSock->Destroy ();

	GetModuleFilePath (szPath, sizeof (szPath));
	strcat (szPath, "Update\\");
	DeleteAllFiles (szPath);

	GetModuleFilePath (szPath, sizeof (szPath));
	strcat (szPath, "SboCopy.exe");
	DeleteFile (szPath);

	GetDlgItem (IDC_RETRY)->EnableWindow (TRUE);
	GetDlgItem (IDOK)->EnableWindow (TRUE);

	ChgState (GAMESTATE_NONE);
	GetDlgItem (IDC_RETRY)->EnableWindow (FALSE);
}


/* ========================================================================= */
/* 関数名	:CSboLaunchDlg::ProcCHECKFILELIST								 */
/* 内容		:処理(ファイルリストチェック)									 */
/* 日付		:2008/03/09														 */
/* ========================================================================= */

void CSboLaunchDlg::ProcCHECKFILELIST(void)
{
	BOOL bGetFile;
	int nCount, nState;
	char szPath[MAX_PATH], szHash[33];
	PFILELISTINFO pInfo;
	CGetMD5File GetMD5;
	CString strFileName, strTmp;

	nCount = m_pInfoFileList->GetCount ();
	/* 終了？ */
	if (m_pstCheckFlieList->nNo >= nCount) {
		KillTimer (TIMERID_CHECKFILE);
		nState = GAMESTATE_GETFILE;
		/* アップデータは毎回あるので2つ以上の場合のみダウンロード対象とする */
		if (m_pstCheckFlieList->anGetFlieNo.size() <= 1) {
			nState = GAMESTATE_QUIT;
		}
		ChgState (nState);
		return;
	}

	bGetFile = FALSE;
	ZeroMemory (szHash, sizeof (szHash));
        GetModuleFilePath (szPath, sizeof (szPath));
        CString strModulePath = Utf8ToTString (szPath);
        pInfo = m_pInfoFileList->GetPtr (m_pstCheckFlieList->nNo);

        strFileName.Format(_T("%s%s"), (LPCTSTR)strModulePath, (LPCTSTR)pInfo->strFileName);
	GetMD5.Init ();
	GetMD5.Update (strFileName);
	GetMD5.GetStr (szHash);
	if (strlen (szHash) <= 0) {
		bGetFile = TRUE;
	}
	if (pInfo->strMD5 != szHash) {
		bGetFile = TRUE;
	}

	if (bGetFile) {
		m_pstCheckFlieList->anGetFlieNo.push_back (m_pstCheckFlieList->nNo);
	}
	m_pstCheckFlieList->nNo ++;
	m_ctlProgressFile.SetPos (m_pstCheckFlieList->nNo);
	m_ctlProgressAll. SetPos (m_pstCheckFlieList->nNo);
}


/* ========================================================================= */
/* 関数名	:CSboLaunchDlg::ProcGETFILE										 */
/* 内容		:処理(ファイル取得)												 */
/* 日付		:2008/03/09														 */
/* ========================================================================= */

void CSboLaunchDlg::ProcGETFILE(void)
{
	DWORD dwRecvSize, dwBytes;
	HANDLE hFile;
	PFILELISTINFO pInfo;
	LPSTR pszFileName;
	char szFileName[MAX_PATH], szPath[MAX_PATH];
	CPacketVERSION_REQ_FILE Packet;
	CmyString strFileName, strTmp;

	if (m_pstCheckFlieList == NULL) {
		return;
	}
	if (m_pstGetFileInfo == NULL) {
		return;
	}

	pInfo = m_pInfoFileList->GetPtr (m_pstCheckFlieList->anGetFlieNo[m_pstGetFileInfo->nNo]);

	/* 受信完了？ */
	if (m_pstGetFileInfo->dwRecvSize == pInfo->dwFileSize) {
		m_ctlProgressAll.SetPos (m_ctlProgressAll.GetPos () + 1);

		GetModuleFilePath (szFileName, sizeof (szFileName));
            CString strBaseName = Utf8ToTString (szFileName);
            strFileName.Format(_T("%sUpdate\\%s"), (LPCTSTR)strBaseName, (LPCTSTR)pInfo->strFileName);
		strcpy (szPath, strFileName);
		pszFileName = PathFindFileName (szPath);
		if (pszFileName) {
			*pszFileName = 0;
		}
		AllCreateDirectory (szPath);
		/* ファイルを開く */
		hFile = CreateFile (strFileName, GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile == INVALID_HANDLE_VALUE) {
			strTmp.Format(_T("ファイルの出力に失敗しました[%d]\r\n"), GetLastError ());
			AddMsg ((LPCSTR)strTmp);
			return;
		}
		dwBytes = 0;
		WriteFile (hFile, m_pstGetFileInfo->pRecvDataTmp, m_pstGetFileInfo->dwRecvSize, &dwBytes, NULL);
		CloseHandle (hFile);

		SAFE_DELETE_ARRAY (m_pstGetFileInfo->pRecvDataTmp);
		m_pstGetFileInfo->nNo ++;
		m_pstGetFileInfo->dwRecvSize	= 0;
		m_pstGetFileInfo->pRecvDataTmp	= NULL;

		/* 全てダウンロード完了？ */
		if (m_pstGetFileInfo->nNo >= m_pstCheckFlieList->anGetFlieNo.size()) {
			m_strFile.Empty ();
			UpdateData (FALSE);
			AddMsg ("ファイルのダウンロードが完了しました\r\n");
			ChgState (GAMESTATE_MAKEFILELIST);
			return;
		}
		pInfo = m_pInfoFileList->GetPtr (m_pstCheckFlieList->anGetFlieNo[m_pstGetFileInfo->nNo]);
	}

	dwRecvSize = 1024 * 8 * 2;
	if (m_pstGetFileInfo->dwRecvSize + dwRecvSize > pInfo->dwFileSize) {
		dwRecvSize = pInfo->dwFileSize - m_pstGetFileInfo->dwRecvSize;
	}
	Packet.Make (m_pstGetFileInfo->dwRecvSize, dwRecvSize, pInfo->strFileName);
	m_pSock->Send (&Packet);

	/* 初回？ */
	if (m_pstGetFileInfo->dwRecvSize == 0) {
            strTmp.Format(_T("%s\r\n"), (LPCTSTR)pInfo->strFileName);
		AddMsg ((LPCSTR)strTmp);

		m_ctlProgressFile.SetRange32 (0, pInfo->dwFileSize);
		m_ctlProgressFile.SetPos (0);
		m_pstGetFileInfo->pRecvDataTmp = ZeroNew (pInfo->dwFileSize);
		m_strFile.Format(_T("0/%d"), pInfo->dwFileSize);
		UpdateData (FALSE);
	}
}


/* ========================================================================= */
/* 関数名	:CSboLaunchDlg::OnRecv											 */
/* 内容		:データ受信														 */
/* 日付		:2008/03/08														 */
/* ========================================================================= */

void CSboLaunchDlg::OnRecv(PBYTE pRecvData)
{
	CPacketBase Packet;

	Packet.Set (pRecvData);

	switch (Packet.m_byCmdMain) {
	case SBOCOMMANDID_MAIN_VERSION:	RecvProcVERSION	(Packet.m_byCmdSub, pRecvData);		break;		/* バージョン系 */
	}

	SAFE_DELETE_ARRAY (pRecvData);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcVERSION									 */
/* 内容		:受信処理(バージョン系)											 */
/* 日付		:2008/03/08														 */
/* ========================================================================= */

void CSboLaunchDlg::RecvProcVERSION(BYTE byCmdSub, PBYTE pData)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_VERSION_RES_FILELIST:	RecvProcVERSION_RES_FILELIST	(pData);	break;	/* ファイルリスト応答 */
	case SBOCOMMANDID_SUB_VERSION_RES_FILE:		RecvProcVERSION_RES_FILE		(pData);	break;	/* ファイル応答 */
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcVERSION_RES_FILELIST						 */
/* 内容		:受信処理(ファイルリスト応答)									 */
/* 日付		:2008/03/08														 */
/* ========================================================================= */

void CSboLaunchDlg::RecvProcVERSION_RES_FILELIST(PBYTE pData)
{
	CPacketVERSION_RES_FILELIST Packet;

	Packet.Set (pData);

	AddMsg ("ファイルリストを取得しました\r\n");

	SAFE_DELETE (m_pInfoFileList);
	m_pInfoFileList = new CInfoFileList;
	m_pInfoFileList->Copy (Packet.m_pInfo);

	ChgState (GAMESTATE_CHECKFILELIST);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcVERSION_RES_FILE							 */
/* 内容		:受信処理(ファイル応答)											 */
/* 日付		:2008/03/09														 */
/* ========================================================================= */

void CSboLaunchDlg::RecvProcVERSION_RES_FILE(PBYTE pData)
{
	PFILELISTINFO pInfo;
	CPacketVERSION_RES_FILE Packet;

	if (m_nState != GAMESTATE_GETFILE) {
		return;
	}

	Packet.Set (pData);

	pInfo = m_pInfoFileList->GetPtr (m_pstCheckFlieList->anGetFlieNo[m_pstGetFileInfo->nNo]);
	CopyMemory (&m_pstGetFileInfo->pRecvDataTmp[m_pstGetFileInfo->dwRecvSize], Packet.m_pFileData, Packet.m_dwSize);
	m_pstGetFileInfo->dwRecvSize += Packet.m_dwSize;

	m_strFile.Format(_T("%d/%d"), m_pstGetFileInfo->dwRecvSize, pInfo->dwFileSize);
	UpdateData (FALSE);

	m_ctlProgressFile.SetPos (m_pstGetFileInfo->dwRecvSize);
	ProcGETFILE ();
}


/* ========================================================================= */
/* 関数名	:CMainFrame::DeleteAllFiles										 */
/* 内容		:指定パス以下のファイルを全て削除								 */
/* 日付		:2008/03/15														 */
/* ========================================================================= */

void CSboLaunchDlg::DeleteAllFiles(LPCSTR pszPath)
{
	int i, nCount;
	WIN32_FIND_DATA stFindData;
	HANDLE hFind;
	CmyString strTmp;
	CmyStringArray astrFolderPath;

	hFind = INVALID_HANDLE_VALUE;
        CString strSearchPath = Utf8ToTString (pszPath);
        strTmp.Format(_T("%s*.*"), (LPCTSTR)strSearchPath);

	/* 最初の検索 */
	hFind = FindFirstFile (strTmp, &stFindData);
	if (hFind == INVALID_HANDLE_VALUE) {
		return;
	}

	DeleteAllFilesTmp (hFind, pszPath, "", astrFolderPath);
	FindClose (hFind);
	nCount = astrFolderPath.size();
	for (i = 0; i < nCount; i ++) {
		RemoveDirectory (astrFolderPath[i]);
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::DeleteAllFilesTmp									 */
/* 内容		:指定パス以下のファイルを全て削除(処理)							 */
/* 日付		:2008/03/15														 */
/* ========================================================================= */

void CSboLaunchDlg::DeleteAllFilesTmp(
	HANDLE hFind,						/* [in] 検索ハンドル */
	LPCSTR pszPath1,					/* [in] 絶対パス */
	LPCSTR pszPath2,					/* [in] 相対パス */
	CmyStringArray &astrFolderPath)		/* [in/ou] 削除するフォルダ */
{
	BOOL bResult;
	char szFullPath[MAX_PATH];
	HANDLE hFindNew;
	WIN32_FIND_DATA stFindData;
	CmyString strTmp, strPathTmp1, strPathTmp2;

	strcpy (szFullPath, pszPath1);
	PathAddBackslash (szFullPath);
	strcat (szFullPath, pszPath2);
	PathRemoveBackslash (szFullPath);

	bResult = TRUE;
	while (bResult) {
		bResult = FindNextFile (hFind, &stFindData);
		if (bResult == FALSE) {
			strTmp = szFullPath;
			astrFolderPath.push_back (strTmp);
			break;
		}

		/* フォルダ？ */
		if (stFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (strcmp (".", stFindData.cFileName) &&
				strcmp ("..", stFindData.cFileName)) {
				/* 相対パスを更新 */
				strPathTmp2  = pszPath2;
				strPathTmp2 += stFindData.cFileName;
				strPathTmp2 += "\\";

				/* 絶対パスを更新 */
				strPathTmp1 = pszPath1;
				strPathTmp1 += pszPath2;
				strPathTmp1 += stFindData.cFileName;
				strPathTmp1 += "\\*.*";

				/* 次のフォルダを検索 */
				hFindNew = FindFirstFile (strPathTmp1, &stFindData);
				if (hFindNew == INVALID_HANDLE_VALUE) {
					goto Exit;
				}
				DeleteAllFilesTmp (hFindNew, pszPath1, strPathTmp2, astrFolderPath);
				FindClose (hFindNew);
			}

		} else {
			PathAddBackslash (szFullPath);
			strcat (szFullPath, stFindData.cFileName);
			DeleteFile (szFullPath);
			/* 次の使用に向けてファイル名を削除しておく */
			PathRemoveFileSpec (szFullPath);
		}
	}

Exit:
	return;
}

/* Copyright(C)URARA-works 2008 */
