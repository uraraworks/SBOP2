/// @file ProxyIpRegistry.h
/// @brief WebSocketブリッジのローカルポート→実クライアントIP対応表 定義ファイル
///
/// ブラウザ版クライアントはゲームサーバーへ直接TCP接続せず、同一プロセス内の
/// CWebSocketBridge が WebSocket を受けて 127.0.0.1 のゲームTCPポートへ
/// 接続し直して中継する。そのためゲーム側の accept() から見える相手IPは
/// 常に loopback になり、実クライアントのIPアドレスが失われてしまう。
///
/// この対応表はブリッジ側で「ゲームTCPへ接続する際に使ったローカルポート」を
/// キーに実IPアドレスを登録しておき、ゲーム側(MainFrame)がそのポート番号
/// (accept() した相手のポート = ブリッジ側の接続元ポート)から実IPを引けるように
/// する。プロセス内だけで完結するため、プロトコルや保存データへの変更は不要。

#pragma once

namespace ProxyIpRegistry
{
    /// @brief 対応関係を登録する
    /// @param wLocalPort ブリッジがゲームTCPへ接続した際のローカルポート番号(ホストバイトオーダー)
    /// @param dwRealIp   実クライアントのIPアドレス(ネットワークバイトオーダー。sockaddr_in.sin_addr.s_addr と同じ形式)
    void Register(unsigned short wLocalPort, unsigned long dwRealIp);

    /// @brief 対応関係を削除する。セッション終了時に必ず呼ぶこと。
    /// @param wLocalPort Register() と同じ値
    void Unregister(unsigned short wLocalPort);

    /// @brief 対応関係を検索する
    /// @param wLocalPort 検索キー(ホストバイトオーダー)
    /// @param outRealIp  見つかった場合、実IPアドレス(ネットワークバイトオーダー)を格納
    /// @retval true  見つかった
    /// @retval false 見つからなかった
    bool Lookup(unsigned short wLocalPort, unsigned long &outRealIp);

    /// @brief 現在の登録件数を取得する(テスト用)
    int Count(void);

    /// @brief IPv4アドレス(ネットワークバイトオーダー)が 127.0.0.0/8 の loopback かどうかを判定する
    bool IsLoopbackIPv4(unsigned long dwIpNet);
}
