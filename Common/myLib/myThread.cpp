/* Copyright(C)URARA-works 2007 */

/* ========================================================================= */
/* ファイル名	:myThread.cpp												 */
/* 内容			:スレッドクラス 実装ファイル								 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/04/28													 */
/* ========================================================================= */

#include "stdafx.h"
#include <process.h>
#include "myThread.h"


/* ========================================================================= */
/* 関数名	:CmyString::CmyString											 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

CmyThread::CmyThread()
{
	m_dwInitResult	= 0;
	m_hThread		= NULL;
	m_hInitEvent	= NULL;
	m_hExitEvent	= NULL;
}


/* ========================================================================= */
/* 関数名	:CmyThread::~CmyThread											 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

CmyThread::~CmyThread()
{
	Destroy();
}


/* ========================================================================= */
/* 関数名	:CmyThread::Create												 */
/* 内容		:作成															 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

BOOL CmyThread::Create(void)
{
	UINT threadID;

	if (m_hThread) {
		return FALSE;
	}

	/* イベントを作成 */
	m_hInitEvent = CreateEvent (NULL, FALSE, FALSE, NULL);
	if (m_hInitEvent == NULL) {
		Destroy ();
		return FALSE;
	}
	m_hExitEvent = CreateEvent (NULL, FALSE, FALSE, NULL);
	if (m_hExitEvent == NULL) {
		Destroy ();
		return FALSE;
	}

	/* スレッドを起動 */
	threadID = 0;
	m_hThread = (HANDLE)_beginthreadex (NULL, 0, ThreadEntry, (PVOID)this, 0, &threadID);
	if (m_hThread == NULL) {
		Destroy ();
		return FALSE;
	}

	/* 初期化完了待ち */
	WaitForSingleObject (m_hInitEvent, INFINITE);
	if (m_dwInitResult) {
		Destroy ();
		return FALSE;
	}
	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CmyThread::Destroy												 */
/* 内容		:破棄															 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

void CmyThread::Destroy(void)
{
	/* スレッドを停止 */
	if (m_hThread) {
		SetEvent (m_hExitEvent);
		WaitForSingleObject (m_hThread, INFINITE);
	}

	/* イベントを破棄 */
	if (m_hExitEvent) {
		CloseHandle (m_hExitEvent);
	}
	if (m_hInitEvent) {
		CloseHandle (m_hInitEvent);
	}
	if (m_hThread) {
		CloseHandle (m_hThread);
	}

	m_dwInitResult	= 0;
	m_hThread		= NULL;
	m_hInitEvent	= NULL;
	m_hExitEvent	= NULL;
}


/* ========================================================================= */
/* 関数名	:CmyThread::ThreadEntry											 */
/* 内容		:エントリポイント												 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

UINT __stdcall CmyThread::ThreadEntry(LPVOID lpParam)
{
	CmyThread* pThis = (CmyThread*) lpParam;

	pThis->m_dwInitResult = pThis->ThreadInit ();
	SetEvent (pThis->m_hInitEvent);

	if (pThis->m_dwInitResult == 0) {
		pThis->ThreadMain ();
	}

	pThis->ThreadExit ();
	return 0;
}


/* ========================================================================= */
/* 関数名	:CmyThread::ThreadInit											 */
/* 内容		:初期化															 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

DWORD CmyThread::ThreadInit(void)
{
	return 0;
}


/* ========================================================================= */
/* 関数名	:CmyThread::ThreadMain											 */
/* 内容		:メイン処理														 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

void CmyThread::ThreadMain(void)
{
	WaitForSingleObject (m_hExitEvent, INFINITE);
}


/* ========================================================================= */
/* 関数名	:CmyThread::ThreadExit											 */
/* 内容		:終了処理														 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

void CmyThread::ThreadExit(void)
{
}

/* Copyright(C)URARA-works 2007 */
