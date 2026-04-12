/// @file myThread.cpp
/// @brief スレッドクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/28
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#ifdef _WIN32
#include <process.h>
#else
#include <SDL.h>
#endif
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
	if (m_hThread) {
		return FALSE;
	}

#ifdef _WIN32
	// Win32: イベントを作成してスレッド起動
	m_hInitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (m_hInitEvent == NULL) { Destroy(); return FALSE; }
	m_hExitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (m_hExitEvent == NULL) { Destroy(); return FALSE; }

	UINT threadID = 0;
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadEntry, (PVOID)this, 0, &threadID);
	if (m_hThread == NULL) { Destroy(); return FALSE; }

	// 初期化完了待ち
	WaitForSingleObject(m_hInitEvent, INFINITE);
#else
	// SDL2: セマフォ（初期値0）をイベント代わりに使用
	m_hInitEvent = SDL_CreateSemaphore(0);
	if (m_hInitEvent == NULL) { Destroy(); return FALSE; }
	m_hExitEvent = SDL_CreateSemaphore(0);
	if (m_hExitEvent == NULL) { Destroy(); return FALSE; }

	m_hThread = SDL_CreateThread(ThreadEntry, "CmyThread", (void*)this);
	if (m_hThread == NULL) { Destroy(); return FALSE; }

	// 初期化完了待ち
	SDL_SemWait(m_hInitEvent);
#endif

	if (m_dwInitResult) {
		Destroy();
		return FALSE;
	}
	return TRUE;
}

// 破棄

void CmyThread::Destroy(void)
{
#ifdef _WIN32
	// スレッドを停止してハンドルを解放
	if (m_hThread) {
		SetEvent(m_hExitEvent);
		WaitForSingleObject(m_hThread, INFINITE);
	}
	if (m_hExitEvent) { CloseHandle(m_hExitEvent); }
	if (m_hInitEvent) { CloseHandle(m_hInitEvent); }
	if (m_hThread)    { CloseHandle(m_hThread); }
#else
	// スレッドを停止（SDL_WaitThread後はハンドル解放不要）
	if (m_hThread) {
		SDL_SemPost(m_hExitEvent);
		SDL_WaitThread(m_hThread, NULL);
	}
	if (m_hExitEvent) { SDL_DestroySemaphore(m_hExitEvent); }
	if (m_hInitEvent) { SDL_DestroySemaphore(m_hInitEvent); }
#endif

	m_dwInitResult	= 0;
	m_hThread		= NULL;
	m_hInitEvent	= NULL;
	m_hExitEvent	= NULL;
}

// エントリポイント

#ifdef _WIN32
UINT __stdcall CmyThread::ThreadEntry(LPVOID lpParam)
#else
int CmyThread::ThreadEntry(void* lpParam)
#endif
{
	CmyThread* pThis = (CmyThread*) lpParam;

	pThis->m_dwInitResult = pThis->ThreadInit();

#ifdef _WIN32
	SetEvent(pThis->m_hInitEvent);
#else
	SDL_SemPost(pThis->m_hInitEvent);
#endif

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

// メイン処理（派生クラスでオーバーライドされることを想定）

void CmyThread::ThreadMain(void)
{
#ifdef _WIN32
	WaitForSingleObject(m_hExitEvent, INFINITE);
#else
	SDL_SemWait(m_hExitEvent);
#endif
}

// 終了処理

void CmyThread::ThreadExit(void)
{
}
