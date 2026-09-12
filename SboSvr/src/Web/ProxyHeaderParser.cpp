/// @file ProxyHeaderParser.cpp
/// @brief HTTPリクエストヘッダから X-Forwarded-For の右端(直前の逆プロキシが付けた値)を取り出す純粋関数 実装ファイル

#include "StdAfx.h"
#include "ProxyHeaderParser.h"

#include <cctype>
#include <cstring>

namespace ProxyHeaderParser
{

namespace
{

std::string ToLower(const std::string &s)
{
    std::string out = s;
    for (size_t i = 0; i < out.size(); ++i) {
        out[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(out[i])));
    }
    return out;
}

std::string Trim(const std::string &s)
{
    size_t nBegin = s.find_first_not_of(" \t\r\n");
    if (nBegin == std::string::npos) {
        return std::string();
    }
    size_t nEnd = s.find_last_not_of(" \t\r\n");
    return s.substr(nBegin, nEnd - nBegin + 1);
}

/// @brief IPv4のドット区切り表記だけを受け付ける厳格なパーサ。
///        各オクテットは 0-255 の数字のみ(先頭ゼロも許容)、余分な文字があれば失敗。
bool ParseIPv4Strict(const std::string &sToken, unsigned long &outIpNet)
{
    unsigned char abyOctet[4];
    size_t nPos = 0;

    for (int i = 0; i < 4; ++i) {
        if (nPos >= sToken.size() || !std::isdigit(static_cast<unsigned char>(sToken[nPos]))) {
            return false;
        }
        int nValue = 0;
        int nDigits = 0;
        while ((nPos < sToken.size()) && std::isdigit(static_cast<unsigned char>(sToken[nPos]))) {
            nValue = nValue * 10 + (sToken[nPos] - '0');
            ++nPos;
            ++nDigits;
            if (nDigits > 3 || nValue > 255) {
                return false;
            }
        }
        abyOctet[i] = static_cast<unsigned char>(nValue);

        if (i < 3) {
            if ((nPos >= sToken.size()) || (sToken[nPos] != '.')) {
                return false;
            }
            ++nPos;
        }
    }

    // 末尾に余分な文字があれば不正(ポート付き "1.2.3.4:5678" 等はここで弾く)
    if (nPos != sToken.size()) {
        return false;
    }

    // ネットワークバイトオーダー(先頭バイト = 第1オクテット)で組み立てる。
    // htonl に頼らず組み立てることでホストのエンディアンに依存しない。
    unsigned long dwResult = 0;
    unsigned char *p = reinterpret_cast<unsigned char *>(&dwResult);
    p[0] = abyOctet[0];
    p[1] = abyOctet[1];
    p[2] = abyOctet[2];
    p[3] = abyOctet[3];

    outIpNet = dwResult;
    return true;
}

/// @brief ヘッダ名(大文字小文字を区別しない)に対応する値を取り出す。
///        同名ヘッダーが複数行にわたる場合は、プロキシが末尾に追記したものが
///        後段に来る想定で、最後に現れた行の値を採用する。
///        見つからなければ空文字列と false を返す。
bool FindHeaderValue(const std::string &httpRequest, const char *pszHeaderNameLower, std::string &outValue)
{
    std::string sLower = ToLower(httpRequest);
    std::string sNeedle = std::string(pszHeaderNameLower) + ":";

    bool bFound = false;
    size_t nSearchFrom = 0;
    while (true) {
        size_t nPos = sLower.find(sNeedle, nSearchFrom);
        if (nPos == std::string::npos) {
            break;
        }

        // 行頭(文字列先頭、または直前が \r か \n)であることを確認する。
        // ヘッダ名の途中に一致してしまう誤検出(例: "Foo-X-Real-IP:")を防ぐ。
        bool bLineStart = (nPos == 0) || (sLower[nPos - 1] == '\n') || (sLower[nPos - 1] == '\r');
        if (!bLineStart) {
            nSearchFrom = nPos + sNeedle.size();
            continue;
        }

        size_t nValueStart = nPos + sNeedle.size();
        size_t nLineEnd = httpRequest.find("\r\n", nValueStart);
        if (nLineEnd == std::string::npos) {
            nLineEnd = httpRequest.size();
        }
        outValue = Trim(httpRequest.substr(nValueStart, nLineEnd - nValueStart));
        bFound = true;
        nSearchFrom = nLineEnd;
    }
    return bFound;
}

} // anonymous namespace

bool ExtractClientIp(const std::string &httpRequest, unsigned long &outIpNet)
{
    std::string sValue;

    // X-Forwarded-For: "client, proxy1, proxy2" の右端(直前の1段のプロキシが
    // 付け足した実IP)だけを使う。信頼するプロキシは1段だけという前提のため、
    // 右端が不正でも左側の(クライアントが自由に偽装できる)要素へは
    // フォールバックしない。X-Real-IP は付与元を保証できないため参照しない。
    if (!FindHeaderValue(httpRequest, "x-forwarded-for", sValue)) {
        return false;
    }

    size_t nComma = sValue.find_last_of(',');
    std::string sLast = (nComma == std::string::npos) ? sValue : sValue.substr(nComma + 1);
    sLast = Trim(sLast);

    if (sLast.empty()) {
        return false;
    }
    return ParseIPv4Strict(sLast, outIpNet);
}

} // namespace ProxyHeaderParser
