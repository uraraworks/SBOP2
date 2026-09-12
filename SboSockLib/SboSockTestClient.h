/// @file SboSockTestClient.h
/// @brief SBO プロトコルを喋る同期クライアント 定義ファイル
/// @copyright Copyright(C)URARA-works
///
/// UraraSockTCP のワイヤプロトコルを、ブロッキングの素直な手順で喋る。
/// 通信層のテストに使うのが第一の目的だが、プレイヤーと同じように
/// サーバーへ繋ぐ用途(仮想プレイヤー等)にもそのまま使える。
///
/// 本物のクライアント実装とは独立させてある。テスト対象と同じコードで
/// 検証しても意味が無いため、フレーミングと CRC は自前で組み立てる。

#pragma once

#ifdef _WIN32

#include <vector>

class CCRC;
class CmyZlib;

class CSboSockTestClient
{
public:
    CSboSockTestClient(void);
    ~CSboSockTestClient(void);

    /// サーバーへ接続し、プリチェックまで済ませる
    /// @param pszAddr 接続先
    /// @param wPort ポート
    /// @param dwKey プリチェックキー(URARASOCK_PRECHECK)
    /// @param bWrongAnswer TRUE ならわざと誤った応答を返す(拒否の検証用)
    BOOL Connect(LPCSTR pszAddr, WORD wPort, DWORD dwKey, BOOL bWrongAnswer = FALSE);

    /// 切断する(SD_BOTH のため相手には RST が届くことがある)
    void Close(void);

    /// 送信方向だけ閉じる
    ///
    /// 相手には FIN が届き、相手側の recv() は 0 を返す。
    /// 行儀の良いクライアントの切断はこちらの形になる。
    void ShutdownSend(void);

    BOOL IsConnected(void) const;

    /// 接続時に使ったローカルポート番号を取得する(ホストバイトオーダー)
    /// サーバー側 GetPeerPort() の検証用。未接続時は 0。
    WORD GetLocalPort(void) const;

    /// パケットを送る(必要なら zlib 圧縮し、フレーム化して送出)
    BOOL SendPacket(const BYTE *pData, DWORD dwSize);

    /// パケットを1つ受け取る(必要なら zlib 展開)
    /// @param vecOut 受信した本体
    /// @param nTimeoutMs 待ち時間
    /// @retval TRUE 受信できた
    BOOL RecvPacket(std::vector<BYTE> *pvecOut, int nTimeoutMs = 3000);

    /// 相手が切断したかを調べる
    BOOL WaitForDisconnect(int nTimeoutMs = 3000);

private:
    BOOL RecvExact(BYTE *pBuf, DWORD dwSize, int nTimeoutMs);
    BOOL SendAll(const BYTE *pBuf, DWORD dwSize);
    BOOL SendFrame(const BYTE *pBody, DWORD dwSize);

private:
    SOCKET   m_socket;
    CCRC    *m_pCrc;
    CmyZlib *m_pZlib;
};

#endif // _WIN32
