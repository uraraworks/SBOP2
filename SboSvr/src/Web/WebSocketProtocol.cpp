#include "StdAfx.h"
#include "WebSocketProtocol.h"

#include <string>
#include <vector>
#include <string.h>

namespace WebSocketProtocol
{

// ============================================================
//  内部定数
// ============================================================
namespace
{
/// WebSocket ハンドシェイクで使う GUID（RFC 6455）
const char *kWsGuid = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
} // anonymous namespace

// ============================================================
//  SHA-1 (RFC 3174 準拠の自前実装。外部ライブラリ・OS API 不使用)
//
//  Windows CryptoAPI (wincrypt.h) に依存していた旧実装を置き換えたもの。
//  非Windows でもそのままビルドできるよう、依存ゼロで書く。
// ============================================================

namespace
{

inline unsigned int Sha1Rotl(unsigned int x, int n)
{
    return (x << n) | (x >> (32 - n));
}

/// @brief 1ブロック(64バイト)分の SHA-1 圧縮関数
void Sha1ProcessBlock(unsigned int state[5], const unsigned char block[64])
{
    unsigned int w[80];
    int i;

    for (i = 0; i < 16; ++i) {
        w[i] = (static_cast<unsigned int>(block[i * 4 + 0]) << 24)
             | (static_cast<unsigned int>(block[i * 4 + 1]) << 16)
             | (static_cast<unsigned int>(block[i * 4 + 2]) << 8)
             |  static_cast<unsigned int>(block[i * 4 + 3]);
    }
    for (i = 16; i < 80; ++i) {
        w[i] = Sha1Rotl(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
    }

    unsigned int a = state[0];
    unsigned int b = state[1];
    unsigned int c = state[2];
    unsigned int d = state[3];
    unsigned int e = state[4];

    for (i = 0; i < 80; ++i) {
        unsigned int f;
        unsigned int k;

        if (i < 20) {
            f = (b & c) | ((~b) & d);
            k = 0x5A827999u;
        } else if (i < 40) {
            f = b ^ c ^ d;
            k = 0x6ED9EBA1u;
        } else if (i < 60) {
            f = (b & c) | (b & d) | (c & d);
            k = 0x8F1BBCDCu;
        } else {
            f = b ^ c ^ d;
            k = 0xCA62C1D6u;
        }

        unsigned int temp = Sha1Rotl(a, 5) + f + e + k + w[i];
        e = d;
        d = c;
        c = Sha1Rotl(b, 30);
        b = a;
        a = temp;
    }

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
}

} // anonymous namespace

/// @brief SHA-1 ダイジェスト(20バイト)を計算する
void Sha1(const unsigned char *pData, size_t nLength, unsigned char hash[20])
{
    unsigned int state[5] = { 0x67452301u, 0xEFCDAB89u, 0x98BADCFEu, 0x10325476u, 0xC3D2E1F0u };
    unsigned long long nBitLen = static_cast<unsigned long long>(nLength) * 8ULL;
    size_t nFullBlocks = nLength / 64;
    size_t i;

    for (i = 0; i < nFullBlocks; ++i) {
        Sha1ProcessBlock(state, pData + i * 64);
    }

    // 末尾の端数 + パディング(0x80 に続けて 0 埋め、末尾8バイトにビット長)
    unsigned char tail[128];
    size_t nRemain = nLength - nFullBlocks * 64;

    memset(tail, 0, sizeof(tail));
    memcpy(tail, pData + nFullBlocks * 64, nRemain);
    tail[nRemain] = 0x80;

    size_t nTailLen = (nRemain < 56) ? 64 : 128;
    for (i = 0; i < 8; ++i) {
        tail[nTailLen - 1 - i] = static_cast<unsigned char>((nBitLen >> (i * 8)) & 0xFF);
    }

    Sha1ProcessBlock(state, tail);
    if (nTailLen == 128) {
        Sha1ProcessBlock(state, tail + 64);
    }

    for (i = 0; i < 5; ++i) {
        hash[i * 4 + 0] = static_cast<unsigned char>((state[i] >> 24) & 0xFF);
        hash[i * 4 + 1] = static_cast<unsigned char>((state[i] >> 16) & 0xFF);
        hash[i * 4 + 2] = static_cast<unsigned char>((state[i] >> 8) & 0xFF);
        hash[i * 4 + 3] = static_cast<unsigned char>( state[i]       & 0xFF);
    }
}

// ============================================================
//  Base64 エンコード
// ============================================================

static std::string Base64Encode(const unsigned char *pData, size_t nLength)
{
    static const char kTable[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string result;
    result.reserve(((nLength + 2) / 3) * 4);

    for (size_t i = 0; i < nLength; i += 3) {
        unsigned int val = static_cast<unsigned int>(pData[i]) << 16;
        if (i + 1 < nLength) {
            val |= static_cast<unsigned int>(pData[i + 1]) << 8;
        }
        if (i + 2 < nLength) {
            val |= static_cast<unsigned int>(pData[i + 2]);
        }
        result += kTable[(val >> 18) & 0x3F];
        result += kTable[(val >> 12) & 0x3F];
        result += (i + 1 < nLength) ? kTable[(val >> 6) & 0x3F] : '=';
        result += (i + 2 < nLength) ? kTable[ val       & 0x3F] : '=';
    }
    return result;
}

// ============================================================
//  公開 API
// ============================================================

std::string ComputeAcceptKey(const std::string &clientKey)
{
    std::string combined = clientKey + kWsGuid;
    unsigned char hash[20];
    Sha1(reinterpret_cast<const unsigned char *>(combined.c_str()), combined.size(), hash);
    return Base64Encode(hash, 20);
}

std::string Sha1Hex(const std::string &data)
{
    static const char kHex[] = "0123456789abcdef";
    unsigned char hash[20];
    std::string result;

    Sha1(reinterpret_cast<const unsigned char *>(data.data()), data.size(), hash);
    result.reserve(40);
    for (int i = 0; i < 20; ++i) {
        result += kHex[(hash[i] >> 4) & 0x0F];
        result += kHex[ hash[i]       & 0x0F];
    }
    return result;
}

bool RecvAll(SOCKET hSocket, unsigned char *pBuf, size_t nLength)
{
    size_t nTotalRecv = 0;
    while (nTotalRecv < nLength) {
        int nRecv = recv(hSocket,
                         reinterpret_cast<char *>(pBuf) + nTotalRecv,
                         static_cast<int>(nLength - nTotalRecv),
                         0);
        if (nRecv == 0) {
            return false; // 接続が閉じられた
        }
        if (nRecv == SOCKET_ERROR) {
            int nErr = WSAGetLastError();
            if ((nErr == WSAEINTR) || (nErr == WSAEWOULDBLOCK)) {
                continue;
            }
            return false;
        }
        nTotalRecv += static_cast<size_t>(nRecv);
    }
    return true;
}

bool SendAll(SOCKET hSocket, const char *pData, size_t nLength)
{
    size_t nTotalSent = 0;
    while (nTotalSent < nLength) {
        int nSent = send(hSocket,
                         pData + nTotalSent,
                         static_cast<int>(nLength - nTotalSent),
                         0);
        if (nSent == SOCKET_ERROR) {
            int nErr = WSAGetLastError();
            if ((nErr == WSAEINTR) || (nErr == WSAEWOULDBLOCK)) {
                continue;
            }
            return false;
        }
        if (nSent == 0) {
            return false;
        }
        nTotalSent += static_cast<size_t>(nSent);
    }
    return true;
}

bool ReadFrame(SOCKET hSocket, std::vector<unsigned char> &outPayload, int &outOpcode)
{
    outPayload.clear();
    outOpcode = 0;

    // 先頭 2 バイト
    unsigned char header[2];
    if (!RecvAll(hSocket, header, 2)) {
        return false;
    }

    outOpcode = header[0] & 0x0F;
    bool bMask = (header[1] & 0x80) != 0;
    unsigned long long payloadLen = header[1] & 0x7F;

    if (payloadLen == 126) {
        unsigned char ext[2];
        if (!RecvAll(hSocket, ext, 2)) {
            return false;
        }
        payloadLen = (static_cast<unsigned long long>(ext[0]) << 8)
                   |  static_cast<unsigned long long>(ext[1]);
    } else if (payloadLen == 127) {
        unsigned char ext[8];
        if (!RecvAll(hSocket, ext, 8)) {
            return false;
        }
        payloadLen = 0;
        for (int i = 0; i < 8; ++i) {
            payloadLen = (payloadLen << 8) | static_cast<unsigned long long>(ext[i]);
        }
    }

    // マスキングキー
    unsigned char maskKey[4] = { 0, 0, 0, 0 };
    if (bMask) {
        if (!RecvAll(hSocket, maskKey, 4)) {
            return false;
        }
    }

    // ペイロードサイズ上限（64MB）
    if (payloadLen > 64 * 1024 * 1024ULL) {
        return false;
    }

    if (payloadLen > 0) {
        outPayload.resize(static_cast<size_t>(payloadLen));
        if (!RecvAll(hSocket, &outPayload[0], static_cast<size_t>(payloadLen))) {
            return false;
        }
        if (bMask) {
            for (size_t i = 0; i < outPayload.size(); ++i) {
                outPayload[i] ^= maskKey[i % 4];
            }
        }
    }

    return true;
}

bool SendFrame(SOCKET hSocket, const unsigned char *pData, size_t nLength, int opcode)
{
    // サーバー→クライアントはマスクなし
    unsigned char headerBuf[10];
    size_t nHeaderLen = 0;

    headerBuf[nHeaderLen++] = static_cast<unsigned char>(0x80 | (opcode & 0x0F));

    if (nLength < 126) {
        headerBuf[nHeaderLen++] = static_cast<unsigned char>(nLength);
    } else if (nLength <= 0xFFFF) {
        headerBuf[nHeaderLen++] = 126;
        headerBuf[nHeaderLen++] = static_cast<unsigned char>((nLength >> 8) & 0xFF);
        headerBuf[nHeaderLen++] = static_cast<unsigned char>( nLength       & 0xFF);
    } else {
        headerBuf[nHeaderLen++] = 127;
        for (int i = 7; i >= 0; --i) {
            headerBuf[nHeaderLen++] = static_cast<unsigned char>((nLength >> (i * 8)) & 0xFF);
        }
    }

    if (!SendAll(hSocket, reinterpret_cast<const char *>(headerBuf), nHeaderLen)) {
        return false;
    }
    if ((nLength > 0) && (pData != NULL)) {
        if (!SendAll(hSocket, reinterpret_cast<const char *>(pData), nLength)) {
            return false;
        }
    }
    return true;
}

bool SendTextFrame(SOCKET hSocket, const std::string &text)
{
    return SendFrame(hSocket,
                     reinterpret_cast<const unsigned char *>(text.c_str()),
                     text.size(),
                     kOpcodeText);
}

bool SendCloseFrame(SOCKET hSocket)
{
    return SendFrame(hSocket, NULL, 0, kOpcodeClose);
}

} // namespace WebSocketProtocol
