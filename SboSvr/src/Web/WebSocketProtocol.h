#pragma once

#include <string>
#include <vector>

/// @brief WebSocket プロトコル共通ユーティリティ
/// CWebSocketBridge と CAdminWsHub の両方から利用する。
namespace WebSocketProtocol
{

// WebSocket オペコード定数
const int kOpcodeText         = 0x1; ///< テキストフレーム
const int kOpcodeBinary       = 0x2; ///< バイナリフレーム
const int kOpcodeClose        = 0x8; ///< 接続クローズ
const int kOpcodePing         = 0x9; ///< Ping
const int kOpcodePong         = 0xA; ///< Pong

/// @brief Sec-WebSocket-Accept キーを計算する（SHA-1 + Base64）
std::string ComputeAcceptKey(const std::string &clientKey);

/// @brief WebSocket フレームを 1 フレーム読み込む（デマスク済みペイロードを返す）
bool ReadFrame(SOCKET hSocket, std::vector<unsigned char> &outPayload, int &outOpcode);

/// @brief WebSocket テキスト/バイナリフレームを送信する（サーバー→クライアントなのでマスクなし）
bool SendFrame(SOCKET hSocket, const unsigned char *pData, size_t nLength, int opcode);

/// @brief テキストフレームを std::string で送信するショートカット
bool SendTextFrame(SOCKET hSocket, const std::string &text);

/// @brief Close フレームを送信する
bool SendCloseFrame(SOCKET hSocket);

/// @brief 指定バイト数を確実に送信する
bool SendAll(SOCKET hSocket, const char *pData, size_t nLength);

/// @brief 指定バイト数を確実に受信する
bool RecvAll(SOCKET hSocket, unsigned char *pBuf, size_t nLength);

} // namespace WebSocketProtocol
