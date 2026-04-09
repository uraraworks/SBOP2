/// @file UraraSockTCPSBO.cpp
/// @brief 汎用TCP通信クラス 実装ファイル
/// @date 2006/11/05
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
// SDL2 が使えるなら SDL_LoadObject 経由で動的ロードする。
// SboSockLib (_LIB) のように SDL2 を持たないビルドでは Win32 API にフォールバックする。
#if __has_include(<SDL.h>)
#include <SDL.h>
#define SBO_USE_SDL_LOADOBJECT 1
#endif
#include "Packet/PacketBase.h"
#include "UraraSockTCPSBO.h"

// コンストラクタ

CUraraSockTCPSBO::CUraraSockTCPSBO(void)
{
	m_pSock = NULL;
	m_hDll = NULL;
	m_pfRelease = nullptr;
	m_pfNotify = NULL;
	m_pNotifyUserData = NULL;

#ifdef __EMSCRIPTEN__
	// Emscripten環境: DLLは存在しないのでWebSocket版を直接生成
	m_pSock = GetUraraSockTCP();
	// m_pfRelease は nullptr のまま（デストラクタで delete する）
#else
	TCHAR szFileName[MAX_PATH];
	PFGETURARASOCKTCP pfGetUraraSockTCP;
	PFRELEASEURARASOCKTCP pfReleaseUraraSockTCP = nullptr;

	ZeroMemory(szFileName, sizeof(szFileName));

	GetModuleFilePath(szFileName, _countof(szFileName));
	CString strFileName(szFileName);
	strFileName += _T("UraraSockTCP.dll");

#if defined(SBO_USE_SDL_LOADOBJECT)
	// SDL_LoadObject でクロスプラットフォーム対応
	{
		std::string ansiFileName = TStringToAnsiStd(strFileName);
		m_hDll = SDL_LoadObject(ansiFileName.c_str());
	}
#else
	m_hDll = (void*)LoadLibrary(strFileName);
#endif
	if (m_hDll) {
#if defined(SBO_USE_SDL_LOADOBJECT)
		pfGetUraraSockTCP = (PFGETURARASOCKTCP)SDL_LoadFunction(m_hDll, "GetUraraSockTCP");
		pfReleaseUraraSockTCP = (PFRELEASEURARASOCKTCP)SDL_LoadFunction(m_hDll, "ReleaseUraraSockTCP");
#else
		pfGetUraraSockTCP = (PFGETURARASOCKTCP)GetProcAddress((HMODULE)m_hDll, "GetUraraSockTCP");
		pfReleaseUraraSockTCP = (PFRELEASEURARASOCKTCP)GetProcAddress((HMODULE)m_hDll, "ReleaseUraraSockTCP");
#endif
		if (pfGetUraraSockTCP) {
			m_pSock = pfGetUraraSockTCP();
		}
		if (pfReleaseUraraSockTCP) {
			m_pfRelease = pfReleaseUraraSockTCP;
		}
	}
#endif // __EMSCRIPTEN__
}

// デストラクタ

CUraraSockTCPSBO::~CUraraSockTCPSBO(void)
{
	// Use the DLL provided release function when available, because the
	// object was created inside the DLL and must be freed there to avoid
	// CRT heap mismatch.
	if (m_pfRelease && m_pSock) {
		m_pfRelease(m_pSock);
		m_pSock = nullptr;
	} else {
		SAFE_DELETE(m_pSock);
	}

	if (m_hDll) {
#if defined(SBO_USE_SDL_LOADOBJECT)
		SDL_UnloadObject(m_hDll);
#else
		FreeLibrary((HMODULE)m_hDll);
#endif
		m_hDll = NULL;
	}
}

void CUraraSockTCPSBO::DeleteRecvData(PBYTE pData)
{
	if (m_pSock == NULL) {
		return;
	}

	m_pSock->DeleteRecvData(pData);
}

// 後始末

void CUraraSockTCPSBO::Destroy(void)
{
	if (m_pSock == NULL) {
		return;
	}

	m_pSock->Destroy();
}


void CUraraSockTCPSBO::SetNotifySink(PFURARASOCKNOTIFY pfNotify, void *pUserData)
{
	// DLLのvtableを経由せず、ラッパー側でコールバックを保持するだけ
	// （2008年ビルドのDLLはSetNotifySinkをvtableに持たないため直接呼び出し不可）
	m_pfNotify = pfNotify;
	m_pNotifyUserData = pUserData;
#ifdef __EMSCRIPTEN__
	// WebSocket版はm_pSock自身がコールバックを管理するため転送する
	if (m_pSock) {
		CUraraSockTCPWebSocket* pWs = static_cast<CUraraSockTCPWebSocket*>(m_pSock);
		pWs->SetNotifySink(pfNotify, pUserData);
	}
#endif
}

// 接続待ち開始

BOOL CUraraSockTCPSBO::Host(HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, DWORD dwCount)
{
	if (m_pSock == NULL) {
		return FALSE;
	}

	return m_pSock->Host(hWndParent, dwMsgBase, dwKey, wPort, dwCount);
}

// サーバーへ接続

BOOL CUraraSockTCPSBO::Connect(HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, LPCSTR pszAddr)
{
	if (m_pSock == NULL) {
		return FALSE;
	}

	return m_pSock->Connect(hWndParent, dwMsgBase, dwKey, wPort, pszAddr);
}

// クライアントを切断

void CUraraSockTCPSBO::DeleteClient(DWORD dwID)
{
	if (m_pSock == NULL) {
		return;
	}

	m_pSock->DeleteClient(dwID);
}

// 送信キャンセル

void CUraraSockTCPSBO::SendCancel(DWORD dwID)
{
	if (m_pSock == NULL) {
		return;
	}

	m_pSock->SendCancel(dwID);
}

// サーバーへデータ送信

void CUraraSockTCPSBO::Send(CPacketBase *pPacket)
{
	if (m_pSock == NULL) {
		return;
	}

	m_pSock->Send(pPacket->m_pPacket, pPacket->m_dwPacketSize);
}

// クライアントへデータ送信

void CUraraSockTCPSBO::SendTo(DWORD dwID, CPacketBase *pPacket)
{
	if (m_pSock == NULL) {
		return;
	}

	m_pSock->SendTo(dwID, pPacket->m_pPacket, pPacket->m_dwPacketSize);
}

// リンク・スループット量を取得(送信)

DWORD CUraraSockTCPSBO::GetThrowghPutSend(DWORD dwID)
{
	if (m_pSock == NULL) {
		return 0;
	}

	return m_pSock->GetThrowghPutSend(dwID);
}

// リンク・スループット量を取得(受信)

DWORD CUraraSockTCPSBO::GetThrowghPutRecv(DWORD dwID)
{
	if (m_pSock == NULL) {
		return 0;
	}

	return m_pSock->GetThrowghPutRecv(dwID);
}

// キュー数を取得

DWORD CUraraSockTCPSBO::GetQueCount(DWORD dwID)
{
	if (m_pSock == NULL) {
		return 0;
	}

	return m_pSock->GetQueCount(dwID);
}

// IPアドレスを取得

DWORD CUraraSockTCPSBO::GetIPAddress(DWORD dwID)
{
	if (m_pSock == NULL) {
		return 0;
	}

	return m_pSock->GetIPAddress(dwID);
}
