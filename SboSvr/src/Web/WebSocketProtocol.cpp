#include "stdafx.h"
#include "WebSocketProtocol.h"

#include <string>
#include <vector>
#include <wincrypt.h>
#pragma comment(lib, "advapi32.lib")

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
//  SHA-1 (Windows CryptoAPI 利用)
// ============================================================

static void Sha1(const unsigned char *pData, size_t nLength, unsigned char hash[20])
{
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    DWORD dwHashLen = 20;

    ZeroMemory(hash, 20);
    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
        return;
    }
    if (!CryptCreateHash(hProv, CALG_SHA1, 0, 0, &hHash)) {
        CryptReleaseContext(hProv, 0);
        return;
    }
    CryptHashData(hHash, pData, static_cast<DWORD>(nLength), 0);
    CryptGetHashParam(hHash, HP_HASHVAL, hash, &dwHashLen, 0);
    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
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
