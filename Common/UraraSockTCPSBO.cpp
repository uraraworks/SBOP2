#include "StdAfx.h"
#include "Packet/PacketBase.h"
#include "UraraSockTCPSBO.h"

// コンストラクタ
CUraraSockTCPSBO::CUraraSockTCPSBO(void)
{
    m_pSock = new CUraraSockTCP;
}

// デストラクタ
CUraraSockTCPSBO::~CUraraSockTCPSBO(void)
{
    SAFE_DELETE(m_pSock);
}

// 受信データの解放
void CUraraSockTCPSBO::DeleteRecvData(PBYTE pData)
{
    SAFE_DELETE_ARRAY(pData);
}

// 後始末
void CUraraSockTCPSBO::Destroy(void)
{
    if (m_pSock == NULL) {
        return;
    }
    m_pSock->Destroy();
}

// サーバー待ち受け開始
BOOL CUraraSockTCPSBO::Host(HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, DWORD dwCount)
{
    if (m_pSock == NULL) {
        return FALSE;
    }
    return m_pSock->Host(hWndParent, wPort, dwCount);
}

// サーバーへ接続
BOOL CUraraSockTCPSBO::Connect(HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, LPCSTR pszAddr)
{
    if (m_pSock == NULL) {
        return FALSE;
    }
    return m_pSock->Connect(hWndParent, wPort, const_cast<LPSTR>(pszAddr));
}

// クライアント切断
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

// データ送信（サーバー）
void CUraraSockTCPSBO::Send(CPacketBase *pPacket)
{
    if (m_pSock == NULL) {
        return;
    }
    m_pSock->Send(pPacket->m_pPacket, pPacket->m_dwPacketSize);
}

// データ送信（クライアント指定）
void CUraraSockTCPSBO::SendTo(DWORD dwID, CPacketBase *pPacket)
{
    if (m_pSock == NULL) {
        return;
    }
    m_pSock->SendTo(dwID, pPacket->m_pPacket, pPacket->m_dwPacketSize);
}

// 送信スループット取得
DWORD CUraraSockTCPSBO::GetThrowghPutSend(DWORD dwID)
{
    if (m_pSock == NULL) {
        return 0;
    }
    return m_pSock->GetThrowghPutSend(dwID);
}

// 受信スループット取得
DWORD CUraraSockTCPSBO::GetThrowghPutRecv(DWORD dwID)
{
    if (m_pSock == NULL) {
        return 0;
    }
    return m_pSock->GetThrowghPutRecv(dwID);
}

// 送信キュー数取得
DWORD CUraraSockTCPSBO::GetQueCount(DWORD dwID)
{
    if (m_pSock == NULL) {
        return 0;
    }
    return m_pSock->GetQueCount(dwID);
}

// IPアドレス取得
DWORD CUraraSockTCPSBO::GetIPAddress(DWORD dwID)
{
    if (m_pSock == NULL) {
        return 0;
    }
    return m_pSock->GetIPAddress(dwID);
}

