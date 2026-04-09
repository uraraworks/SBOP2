/// @file UraraSockTCPSBO.cpp
/// @brief 汎用TCP通信クラス 実装ファイル
/// @date 2006/11/05
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
#include "Packet/PacketBase.h"
#include "UraraSockTCPSBO.h"

// コンストラクタ

CUraraSockTCPSBO::CUraraSockTCPSBO(void)
{
	m_pSock = NULL;

	// SboSockLib は static lib として直接リンクするため GetUraraSockTCP() を直接呼ぶ
	m_pSock = GetUraraSockTCP();
}

// デストラクタ

CUraraSockTCPSBO::~CUraraSockTCPSBO(void)
{
	SAFE_DELETE(m_pSock);
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
	// m_pSock（SboSockLib.lib から静的リンクで取得したインスタンス）に転送する。
	// CUraraSockTCPImpl は vtable 経由で SetNotifySink を持つため直接呼び出し可能。
	if (m_pSock) {
		m_pSock->SetNotifySink(pfNotify, pUserData);
	}
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
