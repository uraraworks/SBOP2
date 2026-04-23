#pragma once

#include <map>
#include <vector>
#include <string>
#include <mutex>
#include <functional>

// SelectionHandler.h で定義されている Selection 構造体を前方参照するため、
// ここでは前方宣言のみ使用できないので直接インクルードする。
#include "Handlers/SelectionHandler.h"

/// @brief 管理画面向け WebSocket 接続を管理するシングルトン Hub
/// 各接続は AddConnection で登録され、recv ループスレッドが切断を検知したら
/// RemoveConnection で登録解除する。
class CAdminWsHub
{
public:
    /// @brief シングルトンインスタンスを返す
    static CAdminWsHub &Instance();

    /// @brief ハンドシェイク完了後に接続を登録する
    /// @param hSocket    WebSocket ソケット（Hub が所有権を持つ）
    /// @param sessionId  SESSID Cookie から取得したセッション ID
    void AddConnection(SOCKET hSocket, const std::string &sessionId);

    /// @brief 切断時に接続を登録解除する
    /// @param hSocket 切断されたソケット
    void RemoveConnection(SOCKET hSocket);

    /// @brief 全接続に JSON テキストフレームをブロードキャストする
    /// ミューテックス外で送信するため、送信中の RemoveConnection と安全に共存できる。
    void BroadcastJson(const std::string &jsonText);

    /// @brief selection_changed イベントを JSON 化してブロードキャストする
    /// @param sessionId  変更が発生したセッション ID
    /// @param pSel       変更後の選択状態（null なら削除された）
    void BroadcastSelectionChanged(const std::string &sessionId, const Selection *pSel);

    /// @brief サーバー停止時に全接続に Close フレームを送り、スレッドが終了するのを待つ
    void Shutdown();

private:
    CAdminWsHub();
    CAdminWsHub(const CAdminWsHub &);
    CAdminWsHub &operator=(const CAdminWsHub &);

    /// @brief 個々の WebSocket 接続情報
    struct Connection
    {
        SOCKET      hSocket;     ///< ソケット
        std::string sessionId;   ///< 管理者セッション ID
        HANDLE      hThread;     ///< recv ループスレッドハンドル
    };

    /// @brief recv ループスレッドエントリポイント
    static unsigned __stdcall RecvThreadProc(void *lpParam);

    /// @brief 1接続の recv ループ本体
    void RunRecvLoop(SOCKET hSocket);

    /// @brief Selection 構造体を JSON 文字列化する（BuildSelectionJson の共通化）
    static std::string BuildSelectionJson(const Selection &sel);

    mutable std::mutex              m_mutex;       ///< m_connections 保護用
    std::vector<Connection>         m_connections; ///< 接続リスト
    bool                            m_bShutdown;   ///< シャットダウン中フラグ
};

/// @brief RecvThreadProc への引数
struct AdminWsRecvArgs
{
    CAdminWsHub *pHub;    ///< Hub オブジェクト
    SOCKET       hSocket; ///< 担当ソケット
};
