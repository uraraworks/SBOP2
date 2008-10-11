/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:UpdateServerInfo.cpp										 */
/* ���e			:�T�[�o�[���X�V�N���X �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/05/22													 */
/* ========================================================================= */

#include "StdAfx.h"
#include <process.h>
#include <wininet.h>
#include <time.h>
#include "SBOGlobal.h"
#include "UpdateServerInfo.h"

#define SERVERINFOFILENAME	"ServerInfo.csv"		/* �T�[�o�[���t�@�C���� */

/* ========================================================================= */
/* �֐���	:CUpdateServerInfo::CUpdateServerInfo							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/05/22														 */
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
/* �֐���	:CUpdateServerInfo::~CUpdateServerInfo							 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/05/22														 */
/* ========================================================================= */

CUpdateServerInfo::~CUpdateServerInfo()
{
	Destroy ();
}


/* ========================================================================= */
/* �֐���	:CUpdateServerInfo::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2008/05/22														 */
/* ========================================================================= */

BOOL CUpdateServerInfo::Create(
	LPCSTR pszAccount,			/* [in] FTP�A�J�E���g */
	LPCSTR pszPassword,			/* [in] FTP�p�X���[�h */
	LPCSTR pszServerAddr,		/* [in] �T�[�o�[�A�h���X */
	LPCSTR pszUploadPath)		/* [in] �A�b�v���[�h�� */
{
	BOOL bRet;

	bRet = FALSE;

	/* ���łɍ쐬����Ă��� */
	if(m_hThread != NULL){
		return TRUE;
	}

	/* �C�x���g���쐬 */
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

	/* �X���b�h���N�� */
	UINT threadID = 0;
	m_hThread = (HANDLE)_beginthreadex (NULL, 0, ThreadEntry, (PVOID)this, 0, &threadID);
	if (m_hThread == NULL) {
		goto Exit;
	}

	/* ������������ҋ@ */
	WaitForSingleObject (m_hInitEvent, INFINITE);

	bRet = TRUE;
Exit:
	if (bRet == FALSE) {
		Destroy ();
	}

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CUpdateServerInfo::Destroy										 */
/* ���e		:�j��															 */
/* ���t		:2008/05/22														 */
/* ========================================================================= */

void CUpdateServerInfo::Destroy(void)
{
	/* �X���b�h���~ */
	if (m_hThread != NULL) {
		SetEvent (m_hExitEvent);
		WaitForSingleObject (m_hThread, INFINITE);
	}

	/* �C�x���g��j�� */
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
/* �֐���	:CUpdateServerInfo::Update										 */
/* ���e		:�X�V															 */
/* ���t		:2008/05/22														 */
/* ========================================================================= */

void CUpdateServerInfo::Update(
	int nCount,				/* [in] ���O�C���l�� */
	LPCSTR pszPlace,		/* [in] �ł��W�܂��Ă���ꏊ */
	BOOL bUpload/*=FALSE*/)	/* [in] TRUE:�����ɃA�b�v���[�h */
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
/* �֐���	:CUpdateServerInfo::ThreadEntry									 */
/* ���e		:�X���b�h���C������												 */
/* ���t		:2008/05/22														 */
/* ========================================================================= */

UINT __stdcall CUpdateServerInfo::ThreadEntry(LPVOID lpParam)
{
	CUpdateServerInfo *pThis = (CUpdateServerInfo *)lpParam;

	pThis->ThreadMain ();

	return 0;
}


/* ========================================================================= */
/* �֐���	:CUpdateServerInfo::ThreadMain									 */
/* ���e		:�X���b�h���C������												 */
/* ���t		:2008/05/23														 */
/* ========================================================================= */

void CUpdateServerInfo::ThreadMain(void)
{
	DWORD dwResult;
	HANDLE aHandle[] = {m_hExitEvent, m_hUpdate};

	/* �J�n���� */
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
/* �֐���	:CUpdateServerInfo::MakeServerInfo								 */
/* ���e		:�T�[�o�[���쐬												 */
/* ���t		:2008/05/22														 */
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
	strTmp.Format ("%d,%d,%s\r\n",
			(int)timeTmp,			/* 0:�ۑ����ꂽUNIX���� */
			m_nCount,				/* 1:���O�C���l�� */
			(LPCSTR)m_strPlace);	/* 2:�ł��W�܂��Ă���ꏊ */

	dwBytes = 0;
	WriteFile (hFile, (LPCSTR)strTmp, strTmp.GetLength (), &dwBytes, NULL);
	CloseHandle (hFile);

Exit:
	m_csUpdate.Leave ();
}


/* ========================================================================= */
/* �֐���	:CUpdateServerInfo::Upload										 */
/* ���e		:�A�b�v���[�h													 */
/* ���t		:2008/05/22														 */
/* ========================================================================= */

void CUpdateServerInfo::Upload(void)
{
	BOOL bResult;
	HINTERNET hHost, hInternet;
	char szUpAddr[MAX_PATH + 1];
	CmyString strFileName;

	hHost		= NULL;
	hInternet	= NULL;

	if (m_strServerAddr.IsEmpty ()) {
		goto Exit;
	}

	GetFileName (strFileName);
	/* �t�@�C�����̍쐬 */
	wsprintf (szUpAddr, "%s/%s", (LPCSTR)m_strUploadPath, SERVERINFOFILENAME);

	/* �C���^�[�l�b�g���J�� */
	hInternet = InternetOpen ("ScrapBookOnlineServer", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (hInternet == NULL) {
		goto Exit;
	}

	/* �T�[�o�[�ɐڑ� */
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

   /* �t�@�C�����A�b�v���[�h */
   bResult = FtpPutFile(
		hHost,
		(LPCSTR)strFileName,
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
/* �֐���	:CUpdateServerInfo::GetFileName									 */
/* ���e		:�T�[�o�[���t�@�C�������擾									 */
/* ���t		:2008/05/22														 */
/* ========================================================================= */

void CUpdateServerInfo::GetFileName(CmyString &strDst)
{
	char szPath[MAX_PATH];

	/* �t�@�C�����̍쐬 */
	GetModuleFilePath (szPath, sizeof (szPath));
	strDst.Format ("%sSBODATA\\%s", szPath, SERVERINFOFILENAME);
}

/* Copyright(C)URARA-works 2008 */
