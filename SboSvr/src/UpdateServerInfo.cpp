/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:UpdateServerInfo.cpp										 */
/* 内容			:サーバー情報更新クラス 実装ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/05/22													 */
/* ========================================================================= */

#include "StdAfx.h"
#include <process.h>
#include <wininet.h>
#include <time.h>
#include "SBOGlobal.h"
#include "UpdateServerInfo.h"

#define SERVERINFOFILENAME	"ServerInfo.csv"		/* サーバー情報ファイル名 */

/* ========================================================================= */
/* 関数名	:CUpdateServerInfo::CUpdateServerInfo							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/05/22														 */
/* ========================================================================= */

CUpdateServerInfo::CUpdateServerInfo()
{
	m_hThread		= NULL;
	m_hInitEvent	= NULL;
	m_hExitEvent	= NULL;
	m_hUpdate		= NULL;
	m_nCount		= 0;
}


/* ========================================================================= */
/* 関数名	:CUpdateServerInfo::~CUpdateServerInfo							 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/05/22														 */
/* ========================================================================= */

CUpdateServerInfo::~CUpdateServerInfo()
{
	Destroy ();
}


/* ========================================================================= */
/* 関数名	:CUpdateServerInfo::Create										 */
/* 内容		:作成															 */
/* 日付		:2008/05/22														 */
/* ========================================================================= */

BOOL CUpdateServerInfo::Create(
	LPCSTR pszAccount,			/* [in] FTPアカウント */
	LPCSTR pszPassword,			/* [in] FTPパスワード */
	LPCSTR pszServerAddr,		/* [in] サーバーアドレス */
	LPCSTR pszUploadPath)		/* [in] アップロード先 */
{
	BOOL bRet;

	bRet = FALSE;

	/* すでに作成されている */
	if(m_hThread != NULL){
		return TRUE;
	}

	/* イベントを作成 */
	m_hInitEvent = CreateEvent (NULL, FALSE, FALSE, NULL);
	if (m_hInitEvent == NULL) {
		goto Exit;
	}
	m_hExitEvent = CreateEvent (NULL, FALSE, FALSE, NULL);
	if (m_hExitEvent == NULL) {
		goto Exit;
	}
	m_hUpdate = CreateEvent (NULL, FALSE, FALSE, NULL);
	if (m_hUpdate == NULL) {
		goto Exit;
	}

	m_strAccount	= pszAccount;
	m_strPassword	= pszPassword;
	m_strServerAddr	= pszServerAddr;
	m_strUploadPath	= pszUploadPath;

	/* スレッドを起動 */
	UINT threadID = 0;
	m_hThread = (HANDLE)_beginthreadex (NULL, 0, ThreadEntry, (PVOID)this, 0, &threadID);
	if (m_hThread == NULL) {
		goto Exit;
	}

	/* 初期化完了を待機 */
	WaitForSingleObject (m_hInitEvent, INFINITE);

	bRet = TRUE;
Exit:
	if (bRet == FALSE) {
		Destroy ();
	}

	return bRet;
}


/* ========================================================================= */
/* 関数名	:CUpdateServerInfo::Destroy										 */
/* 内容		:破棄															 */
/* 日付		:2008/05/22														 */
/* ========================================================================= */

void CUpdateServerInfo::Destroy(void)
{
	/* スレッドを停止 */
	if (m_hThread != NULL) {
		SetEvent (m_hExitEvent);
		WaitForSingleObject (m_hThread, INFINITE);
	}

	/* イベントを破棄 */
	if (m_hExitEvent) {
		CloseHandle (m_hExitEvent);
		m_hExitEvent = NULL;
	}
	if (m_hInitEvent) {
		CloseHandle (m_hInitEvent);
		m_hInitEvent = NULL;
	}
	if (m_hThread) {
		CloseHandle (m_hThread);
		m_hThread = NULL;
	}
	if (m_hUpdate) {
		CloseHandle (m_hUpdate);
		m_hUpdate = NULL;
	}

	m_strAccount.	Empty ();
	m_strPassword.	Empty ();
	m_strServerAddr.Empty ();
	m_strUploadPath.Empty ();
}


/* ========================================================================= */
/* 関数名	:CUpdateServerInfo::Update										 */
/* 内容		:更新															 */
/* 日付		:2008/05/22														 */
/* ========================================================================= */

void CUpdateServerInfo::Update(
	int nCount,				/* [in] ログイン人数 */
	LPCSTR pszPlace,		/* [in] 最も集まっている場所 */
	BOOL bUpload/*=FALSE*/)	/* [in] TRUE:すぐにアップロード */
{
	m_csUpdate.Enter ();

	m_nCount	= nCount;
	m_strPlace	= pszPlace;

	m_csUpdate.Leave ();
	if (bUpload) {
		SetEvent (m_hUpdate);
	}
}


/* ========================================================================= */
/* 関数名	:CUpdateServerInfo::ThreadEntry									 */
/* 内容		:スレッドメイン処理												 */
/* 日付		:2008/05/22														 */
/* ========================================================================= */

UINT __stdcall CUpdateServerInfo::ThreadEntry(LPVOID lpParam)
{
	CUpdateServerInfo *pThis = (CUpdateServerInfo *)lpParam;

	pThis->ThreadMain ();

	return 0;
}


/* ========================================================================= */
/* 関数名	:CUpdateServerInfo::ThreadMain									 */
/* 内容		:スレッドメイン処理												 */
/* 日付		:2008/05/23														 */
/* ========================================================================= */

void CUpdateServerInfo::ThreadMain(void)
{
	DWORD dwResult;
	HANDLE aHandle[] = {m_hExitEvent, m_hUpdate};

	/* 開始完了 */
	SetEvent (m_hInitEvent);

	MakeServerInfo ();
	Upload ();

	while (1) {
		dwResult = WaitForMultipleObjects (
						sizeof (aHandle) / sizeof (HANDLE),
						aHandle,
						FALSE,
						1000 * 60 * 10);
		switch (dwResult) {
		case WAIT_OBJECT_0:
			goto Exit;

		case WAIT_OBJECT_0 + 1:
			ResetEvent (m_hUpdate);
			MakeServerInfo ();
			Upload ();
			break;

		case WAIT_TIMEOUT:
			MakeServerInfo ();
			Upload ();
			break;
		}
	}
Exit:
	return;
}


/* ========================================================================= */
/* 関数名	:CUpdateServerInfo::MakeServerInfo								 */
/* 内容		:サーバー情報作成												 */
/* 日付		:2008/05/22														 */
/* ========================================================================= */

void CUpdateServerInfo::MakeServerInfo(void)
{
	HANDLE hFile;
	DWORD dwBytes;
	time_t timeTmp;
	CmyString strFileName, strTmp;

	m_csUpdate.Enter ();

	GetFileName (strFileName);
	hFile = CreateFile (strFileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE) {
		goto Exit;
	}

	time (&timeTmp);
	strTmp.Format(_T("%d,%d,%s\r\n"),
			(int)timeTmp,			/* 0:保存されたUNIX時間 */
			m_nCount,				/* 1:ログイン人数 */
			(LPCSTR)m_strPlace);	/* 2:最も集まっている場所 */

	dwBytes = 0;
	WriteFile (hFile, (LPCSTR)strTmp, strTmp.GetLength (), &dwBytes, NULL);
	CloseHandle (hFile);

Exit:
	m_csUpdate.Leave ();
}


/* ========================================================================= */
/* 関数名	:CUpdateServerInfo::Upload										 */
/* 内容		:アップロード													 */
/* 日付		:2008/05/22														 */
/* ========================================================================= */

void CUpdateServerInfo::Upload(void)
{
	BOOL bResult;
	HINTERNET hHost, hInternet;
	TCHAR szUpAddr[MAX_PATH + 1];
	CmyString strFileName;

	hHost		= NULL;
	hInternet	= NULL;

	if (m_strServerAddr.IsEmpty ()) {
		goto Exit;
	}

	GetFileName (strFileName);
	/* ファイル名の作成 */
	wsprintf (szUpAddr, _T("%s/%s"), (LPCTSTR)m_strUploadPath, _T(SERVERINFOFILENAME));

	/* インターネットを開く */
	hInternet = InternetOpen (_T("ScrapBookOnlineServer"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (hInternet == NULL) {
		goto Exit;
	}

	/* サーバーに接続 */
	hHost = InternetConnect(
		hInternet,
		m_strServerAddr,
		INTERNET_INVALID_PORT_NUMBER,
		m_strAccount,
		m_strPassword,
		INTERNET_SERVICE_FTP,
		INTERNET_FLAG_PASSIVE,
		0);
   if (hHost == NULL) {
		goto Exit;
   }

   /* ファイルをアップロード */
   bResult = FtpPutFile(
		hHost,
                (LPCTSTR)strFileName,
                szUpAddr,
		INTERNET_FLAG_TRANSFER_ASCII,
		0);

Exit:
   if (hHost) {
	   InternetCloseHandle (hHost);
   }
   if (hInternet) {
	   InternetCloseHandle (hInternet);
   }
}


/* ========================================================================= */
/* 関数名	:CUpdateServerInfo::GetFileName									 */
/* 内容		:サーバー情報ファイル名を取得									 */
/* 日付		:2008/05/22														 */
/* ========================================================================= */

void CUpdateServerInfo::GetFileName(CmyString &strDst)
{
	char szPath[MAX_PATH];

	/* ファイル名の作成 */
	GetModuleFilePath (szPath, sizeof (szPath));
    CString strBasePath = Utf8ToTString (szPath);
    CString strFileName = Utf8ToTString (SERVERINFOFILENAME);
    strDst.Format(_T("%sSBODATA\\%s"), (LPCTSTR)strBasePath, (LPCTSTR)strFileName);
}

/* Copyright(C)URARA-works 2008 */
