/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* UraraSockTCPSBO.cpp					 */
/* 汎用TCP通信クラス					 */
/* 2006/11/05 作成開始					 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Packet/PacketBase.h"
#include "UraraSockTCPSBO.h"

/* ========================================================================= */
/* 関数名	:CUraraSockTCPSBO::CUraraSockTCPSBO		 */
/* 内容		:コンストラクタ					 */
/* 日付		:2006/11/05					 */
/* ========================================================================= */

CUraraSockTCPSBO::CUraraSockTCPSBO(void)
{
        TCHAR szFileName[MAX_PATH];
        PFGETURARASOCKTCP pfGetUraraSockTCP;
        PFRELEASEURARASOCKTCP pfReleaseUraraSockTCP = nullptr;

        ZeroMemory (szFileName, sizeof (szFileName));

        GetModuleFilePath (szFileName, _countof (szFileName));
        CString strFileName (szFileName);
        strFileName += _T("UraraSockTCP.dll");

	m_pSock = NULL;
	m_hDll = NULL;
	m_pfRelease = nullptr;

        m_hDll = LoadLibrary (strFileName);
	if (m_hDll) {
		pfGetUraraSockTCP = (PFGETURARASOCKTCP)GetProcAddress (m_hDll, "GetUraraSockTCP");
		pfReleaseUraraSockTCP = (PFRELEASEURARASOCKTCP)GetProcAddress (m_hDll, "ReleaseUraraSockTCP");
		if (pfGetUraraSockTCP) {
			m_pSock = pfGetUraraSockTCP ();
		}
		if (pfReleaseUraraSockTCP) {
			m_pfRelease = pfReleaseUraraSockTCP;
		}
	}
}


/* ========================================================================= */
/* 関数名	:CUraraSockTCPSBO::~CUraraSockTCPSBO	 */
/* 内容		:デストラクタ					 */
/* 日付		:2006/11/05					 */
/* ========================================================================= */

CUraraSockTCPSBO::~CUraraSockTCPSBO(void)
{
	// Use the DLL provided release function when available, because the
	// object was created inside the DLL and must be freed there to avoid
	// CRT heap mismatch.
	if (m_pfRelease && m_pSock) {
		m_pfRelease(m_pSock);
		m_pSock = nullptr;
	} else {
		SAFE_DELETE (m_pSock);
	}

	if (m_hDll) {
		FreeLibrary (m_hDll);
		m_hDll = NULL;
	}
}


void CUraraSockTCPSBO::DeleteRecvData(PBYTE pData)
{
	if (m_pSock == NULL) {
		return;
	}

	m_pSock->DeleteRecvData (pData);
}


/* ========================================================================= */
/* 関数名	:CUraraSockTCPSBO::Destroy		 */
/* 内容		:後始末					 */
/* 日付		:2003/10/28			 */
/* ========================================================================= */

void CUraraSockTCPSBO::Destroy(void)
{
	if (m_pSock == NULL) {
		return;
	}

	m_pSock->Destroy ();
}


/* ========================================================================= */
/* 関数名	:CUraraSockTCPSBO::Host		 */
/* 内容		:接続待ち開始				 */
/* 日付		:2003/10/28			 */
/* ========================================================================= */

BOOL CUraraSockTCPSBO::Host(HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, DWORD dwCount)
{
	if (m_pSock == NULL) {
		return FALSE;
	}

	return m_pSock->Host (hWndParent, dwMsgBase, dwKey, wPort, dwCount);
}


/* ========================================================================= */
/* 関数名	:CUraraSockTCPSBO::Connect	 */
/* 内容		:サーバーへ接続			 */
/* 日付		:2003/10/29			 */
/* ========================================================================= */

BOOL CUraraSockTCPSBO::Connect(HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, LPCSTR pszAddr)
{
	if (m_pSock == NULL) {
		return FALSE;
	}

	return m_pSock->Connect (hWndParent, dwMsgBase, dwKey, wPort, pszAddr);
}


/* ========================================================================= */
/* 関数名	:CUraraSockTCPSBO::DeleteClient	 */
/* 内容		:クライアントを切断			 */
/* 日付		:2003/11/03			 */
/* ========================================================================= */

void CUraraSockTCPSBO::DeleteClient(DWORD dwID)
{
	if (m_pSock == NULL) {
		return;
	}

	m_pSock->DeleteClient (dwID);
}


/* ========================================================================= */
/* 関数名	:CUraraSockTCPSBO::SendCancel	 */
/* 内容		:送信キャンセル			 */
/* 日付		:2003/11/03			 */
/* ========================================================================= */
void CUraraSockTCPSBO::SendCancel(DWORD dwID)
{
	if (m_pSock == NULL) {
		return;
	}

	m_pSock->SendCancel (dwID);
}


/* ========================================================================= */
/* 関数名	:CUraraSockTCPSBO::Send		 */
/* 内容		:サーバーへデータ送信		 */
/* 日付		:2003/11/01			 */
/* ========================================================================= */

void CUraraSockTCPSBO::Send(CPacketBase *pPacket)
{
	if (m_pSock == NULL) {
		return;
	}

	m_pSock->Send (pPacket->m_pPacket, pPacket->m_dwPacketSize);
}


/* ========================================================================= */
/* 関数名	:CUraraSockTCPSBO::SendTo	 */
/* 内容		:クライアントへデータ送信	 */
/* 日付		:2003/11/02			 */
/* ========================================================================= */

void CUraraSockTCPSBO::SendTo(DWORD dwID, CPacketBase *pPacket)
{
	if (m_pSock == NULL) {
		return;
	}

	m_pSock->SendTo (dwID, pPacket->m_pPacket, pPacket->m_dwPacketSize);
}


/* ========================================================================= */
/* 関数名	:CUraraSockTCPSBO::GetThrowghPutSend */
/* 内容		:リンク・スループット量を取得	 */
/* 日付		:2003/11/08	 */
/* ========================================================================= */

DWORD CUraraSockTCPSBO::GetThrowghPutSend(DWORD dwID)
{
	if (m_pSock == NULL) {
		return 0;
	}

	return m_pSock->GetThrowghPutSend (dwID);
}


/* ========================================================================= */
/* 関数名	:CUraraSockTCPSBO::GetThrowghPutRecv */
/* 内容		:リンク・スループット量を取得	 */
/* 日付		:2003/11/08	 */
/* ========================================================================= */

DWORD CUraraSockTCPSBO::GetThrowghPutRecv(DWORD dwID)
{
	if (m_pSock == NULL) {
		return 0;
	}

	return m_pSock->GetThrowghPutRecv (dwID);
}


/* ========================================================================= */
/* 関数名	:CUraraSockTCPSBO::GetQueCount	 */
/* 内容		:キュー数を取得		 */
/* 日付		:2003/11/08	 */
/* ========================================================================= */

DWORD CUraraSockTCPSBO::GetQueCount(DWORD dwID)
{
	if (m_pSock == NULL) {
		return 0;
	}

	return m_pSock->GetQueCount (dwID);
}


/* ========================================================================= */
/* 関数名	:CUraraSockTCPSBO::GetIPAddress	 */
/* 内容		:IPアドレスを取得		 */
/* 日付		:2005/03/28	 */
/* ========================================================================= */

DWORD CUraraSockTCPSBO::GetIPAddress(DWORD dwID)
{
	if (m_pSock == NULL) {
		return 0;
	}

	return m_pSock->GetIPAddress (dwID);
}

/* Copyright(C)URARA-works 2006 */
