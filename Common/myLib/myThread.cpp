/// @file myThread.cpp
/// @brief スレッドクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/28
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include <process.h>
#include "myThread.h"

// コンストラクタ

CmyThread::CmyThread()
{
	m_dwInitResult	= 0;
	m_hThread		= NULL;
	m_hInitEvent	= NULL;
	m_hExitEvent	= NULL;
}

// デストラクタ

CmyThread::~CmyThread()
{
	Destroy();
}

// 作成

BOOL CmyThread::Create(void)
{
	UINT threadID;

	if (m_hThread) {
		return FALSE;
	}

	// イベントを作成
	m_hInitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (m_hInitEvent == NULL) {
		Destroy();
		return FALSE;
	}
	m_hExitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (m_hExitEvent == NULL) {
		Destroy();
		return FALSE;
	}

	// スレッドを起動
	threadID = 0;
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadEntry, (PVOID)this, 0, &threadID);
	if (m_hThread == NULL) {
		Destroy();
		return FALSE;
	}

	// 初期化完了待ち
	WaitForSingleObject(m_hInitEvent, INFINITE);
	if (m_dwInitResult) {
		Destroy();
		return FALSE;
	}
	return TRUE;
}

// 破棄

void CmyThread::Destroy(void)
{
	// スレッドを停止
	if (m_hThread) {
		SetEvent(m_hExitEvent);
		WaitForSingleObject(m_hThread, INFINITE);
	}

	// イベントを破棄
	if (m_hExitEvent) {
		CloseHandle(m_hExitEvent);
	}
	if (m_hInitEvent) {
		CloseHandle(m_hInitEvent);
	}
	if (m_hThread) {
		CloseHandle(m_hThread);
	}

	m_dwInitResult	= 0;
	m_hThread		= NULL;
	m_hInitEvent	= NULL;
	m_hExitEvent	= NULL;
}

// エントリポイント

UINT __stdcall CmyThread::ThreadEntry(LPVOID lpParam)
{
	CmyThread* pThis = (CmyThread*) lpParam;

	pThis->m_dwInitResult = pThis->ThreadInit();
	SetEvent(pThis->m_hInitEvent);

	if (pThis->m_dwInitResult == 0) {
		pThis->ThreadMain();
	}

	pThis->ThreadExit();
	return 0;
}

// 初期化

DWORD CmyThread::ThreadInit(void)
{
	return 0;
}

// メイン処理

void CmyThread::ThreadMain(void)
{
	WaitForSingleObject(m_hExitEvent, INFINITE);
}

// 終了処理

void CmyThread::ThreadExit(void)
{
}
