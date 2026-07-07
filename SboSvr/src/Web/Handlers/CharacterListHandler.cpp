#include "StdAfx.h"
#include "CharacterListHandler.h"

#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cstring>
#include <cstdlib>
#include <algorithm>

#include "Web/AuthProvider.h"
#include "Web/JsonUtils.h"
#include "MgrData.h"
#include "LibInfo/LibInfoCharSvr.h"
#include "LibInfo/LibInfoAccount.h"
#include "Info/InfoCharBase.h"
#include "myLib/myString.h"

// ---------------------------------------------------------------------------
// 内部ヘルパー
// ---------------------------------------------------------------------------

namespace
{

// UTF-8 BOM を除去する
std::string RemoveUtf8Bom(std::string text)
{
        if (text.size() >= 3 &&
            static_cast<unsigned char>(text[0]) == 0xEF &&
            static_cast<unsigned char>(text[1]) == 0xBB &&
            static_cast<unsigned char>(text[2]) == 0xBF) {
                text.erase(0, 3);
        }
        return text;
}

// 制御文字（改行・タブ除く）を除去する
std::string RemoveControlCharacters(std::string text)
{
        text.erase(
                std::remove_if(
                        text.begin(),
                        text.end(),
                        [](unsigned char ch) {
                                if (ch == '\n' || ch == '\r' || ch == '\t') {
                                        return false;
                                }
                                return ch < 0x20;
                        }),
                text.end());
        return text;
}

#ifndef _UNICODE
// MBCS 環境用: 任意コードページ → UTF-8 変換を試みる
bool TryConvertToUtf8(const char *pszSource, int srcLength, UINT codePage, std::string &out)
{
        if ((pszSource == NULL) || (srcLength <= 0) || (codePage == 0)) {
                return false;
        }

        DWORD flags = (codePage == CP_UTF8) ? MB_ERR_INVALID_CHARS : 0;
        int wideLength = MultiByteToWideChar(codePage, flags, pszSource, srcLength, NULL, 0);
        if (wideLength <= 0) {
                return false;
        }

        std::vector<wchar_t> wide(static_cast<size_t>(wideLength));
        int converted = MultiByteToWideChar(codePage, flags, pszSource, srcLength, &wide[0], wideLength);
        if (converted <= 0) {
                return false;
        }

        if (!wide.empty() && wide.back() == L'\0') {
                wide.pop_back();
                converted = static_cast<int>(wide.size());
        }

        int utf8Length = WideCharToMultiByte(CP_UTF8, 0, &wide[0], converted, NULL, 0, NULL, NULL);
        if (utf8Length <= 0) {
                return false;
        }

        out.resize(static_cast<size_t>(utf8Length));
        WideCharToMultiByte(CP_UTF8, 0, &wide[0], converted, &out[0], utf8Length, NULL, NULL);
        return true;
}
#endif

// CmyString → UTF-8 std::string 変換
std::string ToUtf8String(const CmyString &value)
{
#ifdef _UNICODE
        CStringA utf8 = TStringToUtf8(static_cast<LPCTSTR>(value));
        if (utf8.IsEmpty()) {
                return std::string();
        }
        std::string converted(utf8.GetString(), static_cast<size_t>(utf8.GetLength()));
        return RemoveControlCharacters(RemoveUtf8Bom(converted));
#else
        LPCSTR pszSource = static_cast<LPCSTR>(value);
        if ((pszSource == NULL) || (pszSource[0] == '\0')) {
                return std::string();
        }

        int srcLength = static_cast<int>(std::strlen(pszSource));
        if (srcLength <= 0) {
                return std::string();
        }

        const UINT candidates[] = { CP_UTF8, 932u, CP_ACP };
        std::string utf8;
        for (size_t i = 0; i < sizeof(candidates) / sizeof(candidates[0]); ++i) {
                if (TryConvertToUtf8(pszSource, srcLength, candidates[i], utf8)) {
                        return RemoveControlCharacters(RemoveUtf8Bom(utf8));
                }
        }

        return RemoveControlCharacters(RemoveUtf8Bom(std::string(pszSource, static_cast<size_t>(srcLength))));
#endif
}

// URL クエリ文字列から int 値を取得する
bool TryGetQueryInt(const std::string &path, const std::string &key, int &outValue)
{
        size_t nQueryPos = path.find('?');
        if (nQueryPos == std::string::npos) {
                return false;
        }

        size_t nPos = nQueryPos + 1;
        while (nPos < path.size()) {
                size_t nAmp = path.find('&', nPos);
                size_t nEnd = (nAmp == std::string::npos) ? path.size() : nAmp;
                size_t nEqual = path.find('=', nPos);
                if ((nEqual != std::string::npos) && (nEqual < nEnd)) {
                        std::string k = path.substr(nPos, nEqual - nPos);
                        if (k == key) {
                                std::string v = path.substr(nEqual + 1, nEnd - (nEqual + 1));
                                if (!v.empty()) {
                                        char *pEnd = NULL;
                                        long val = std::strtol(v.c_str(), &pEnd, 10);
                                        if (pEnd != NULL && *pEnd == '\0') {
                                                outValue = static_cast<int>(val);
                                                return true;
                                        }
                                }
                        }
                }
                if (nAmp == std::string::npos) {
                        break;
                }
                nPos = nAmp + 1;
        }
        return false;
}

// URL クエリ文字列から文字列値を取得する（簡易デコード）
bool TryGetQueryString(const std::string &path, const std::string &key, std::string &outValue)
{
        size_t nQueryPos = path.find('?');
        if (nQueryPos == std::string::npos) {
                return false;
        }

        size_t nPos = nQueryPos + 1;
        while (nPos < path.size()) {
                size_t nAmp = path.find('&', nPos);
                size_t nEnd = (nAmp == std::string::npos) ? path.size() : nAmp;
                size_t nEqual = path.find('=', nPos);
                if ((nEqual != std::string::npos) && (nEqual < nEnd)) {
                        std::string k = path.substr(nPos, nEqual - nPos);
                        if (k == key) {
                                outValue = path.substr(nEqual + 1, nEnd - (nEqual + 1));
                                // URL デコード: %XX を文字に変換
                                std::string decoded;
                                decoded.reserve(outValue.size());
                                for (size_t i = 0; i < outValue.size(); ++i) {
                                        if (outValue[i] == '%' && i + 2 < outValue.size()) {
                                                char hi = outValue[i + 1];
                                                char lo = outValue[i + 2];
                                                auto hexVal = [](char c) -> int {
                                                        if (c >= '0' && c <= '9') { return c - '0'; }
                                                        if (c >= 'A' && c <= 'F') { return c - 'A' + 10; }
                                                        if (c >= 'a' && c <= 'f') { return c - 'a' + 10; }
                                                        return -1;
                                                };
                                                int hiVal = hexVal(hi);
                                                int loVal = hexVal(lo);
                                                if (hiVal >= 0 && loVal >= 0) {
                                                        decoded += static_cast<char>((hiVal << 4) | loVal);
                                                        i += 2;
                                                } else {
                                                        decoded += outValue[i];
                                                }
                                        } else if (outValue[i] == '+') {
                                                decoded += ' ';
                                        } else {
                                                decoded += outValue[i];
                                        }
                                }
                                outValue = decoded;
                                return true;
                        }
                }
                if (nAmp == std::string::npos) {
                        break;
                }
                nPos = nAmp + 1;
        }
        return false;
}

// キャラ名に部分一致するか判定（大文字小文字を区別しない ASCII 比較）
bool NameContains(const std::string &charName, const std::string &filter)
{
        if (filter.empty()) {
                return true;
        }
        // 単純な部分一致探索
        if (charName.size() < filter.size()) {
                return false;
        }
        for (size_t i = 0; i <= charName.size() - filter.size(); ++i) {
                bool match = true;
                for (size_t j = 0; j < filter.size(); ++j) {
                        unsigned char a = static_cast<unsigned char>(charName[i + j]);
                        unsigned char b = static_cast<unsigned char>(filter[j]);
                        if (std::tolower(a) != std::tolower(b)) {
                                match = false;
                                break;
                        }
                }
                if (match) {
                        return true;
                }
        }
        return false;
}

} // namespace

// ---------------------------------------------------------------------------
// CCharacterListHandler  GET /api/characters
// ---------------------------------------------------------------------------

CCharacterListHandler::CCharacterListHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CCharacterListHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        // 認証チェック
        AuthProvider::AuthContext authContext;
        AuthProvider::AuthStatus authStatus = AuthProvider::Authenticate(request, m_pMgrData, authContext);
        if (authStatus == AuthProvider::AuthStatusBackendUnavailable) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        if (m_pMgrData == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        CLibInfoCharSvr *pCharLib = m_pMgrData->GetLibInfoChar();
        if (pCharLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        // クエリパラメータを取得する
        std::string filterName;
        TryGetQueryString(request.path, "name", filterName);

        int filterAccountId = -1;
        TryGetQueryInt(request.path, "accountId", filterAccountId);

        int filterMapId = -1;
        TryGetQueryInt(request.path, "mapId", filterMapId);

        int filterIsNpc = -1;  // -1 = 未指定、0 = PC のみ、1 = NPC のみ
        TryGetQueryInt(request.path, "isNpc", filterIsNpc);

        int nLimit = 20;
        {
                int tmp = 0;
                if (TryGetQueryInt(request.path, "limit", tmp) && tmp > 0 && tmp <= 200) {
                        nLimit = tmp;
                }
        }

        int nOffset = 0;
        {
                int tmp = 0;
                if (TryGetQueryInt(request.path, "offset", tmp) && tmp >= 0) {
                        nOffset = tmp;
                }
        }

        // アカウントライブラリから charID→accountID の逆引きマップを構築する
        // （キャラ側の m_dwAccountID はログイン中のみ設定されるため、オフラインキャラの補完に使う）
        // pCharLib のロックとネストしないよう、pCharLib->Enter() の前に構築を済ませる
        std::map<DWORD, DWORD> charToAccount;
        {
                CLibInfoAccount *pAccountLib = m_pMgrData->GetLibInfoAccount();
                if (pAccountLib != NULL) {
                        pAccountLib->Enter();
                        int nAccCount = pAccountLib->GetCount();
                        for (int i = 0; i < nAccCount; ++i) {
                                PCInfoAccount pAcc = static_cast<PCInfoAccount>(pAccountLib->GetPtr(i));
                                if (pAcc == NULL) {
                                        continue;
                                }
                                // 使用中のキャラID
                                if (pAcc->m_dwCharID != 0) {
                                        charToAccount[pAcc->m_dwCharID] = pAcc->m_dwAccountID;
                                }
                                // キャラIDテーブルの全要素
                                int nCharCount = pAcc->m_adwCharID.size();
                                for (int j = 0; j < nCharCount; ++j) {
                                        DWORD dwCharID = pAcc->m_adwCharID[j];
                                        if (dwCharID != 0) {
                                                charToAccount[dwCharID] = pAcc->m_dwAccountID;
                                        }
                                }
                        }
                        pAccountLib->Leave();
                }
        }

        // キャラライブラリをロックして全件スキャン・フィルタリング
        pCharLib->Enter();

        int nTotal = pCharLib->GetCount();

        // フィルタ通過したキャラのポインタを収集する
        std::vector<const CInfoCharBase *> filtered;
        filtered.reserve(static_cast<size_t>(nTotal));

        for (int i = 0; i < nTotal; ++i) {
                const CInfoCharBase *pChar = static_cast<const CInfoCharBase *>(pCharLib->GetPtr(i));
                if (pChar == NULL) {
                        continue;
                }

                // name 部分一致フィルタ
                if (!filterName.empty()) {
                        std::string charName = ToUtf8String(pChar->m_strCharName);
                        if (!NameContains(charName, filterName)) {
                                continue;
                        }
                }

                // accountId 完全一致フィルタ（オフラインキャラは逆引きマップで補完して比較）
                if (filterAccountId >= 0) {
                        DWORD dwAccountId = pChar->m_dwAccountID;
                        if (dwAccountId == 0 && !pChar->m_bNPC) {
                                std::map<DWORD, DWORD>::const_iterator it = charToAccount.find(pChar->m_dwCharID);
                                if (it != charToAccount.end()) {
                                        dwAccountId = it->second;
                                }
                        }
                        if (static_cast<int>(dwAccountId) != filterAccountId) {
                                continue;
                        }
                }

                // mapId 完全一致フィルタ
                if (filterMapId >= 0) {
                        if (static_cast<int>(pChar->m_dwMapID) != filterMapId) {
                                continue;
                        }
                }

                // isNpc フィルタ
                if (filterIsNpc == 0 && pChar->m_bNPC) {
                        continue;
                }
                if (filterIsNpc == 1 && !pChar->m_bNPC) {
                        continue;
                }

                filtered.push_back(pChar);
        }

        // ページング
        int nFilteredTotal = static_cast<int>(filtered.size());
        int nStart = (nOffset < nFilteredTotal) ? nOffset : nFilteredTotal;
        int nEnd   = (nStart + nLimit < nFilteredTotal) ? (nStart + nLimit) : nFilteredTotal;

        // JSON レスポンスを構築する
        std::ostringstream oss;
        oss << '{';
        oss << "\"total\":" << nFilteredTotal << ',';
        oss << "\"limit\":" << nLimit << ',';
        oss << "\"offset\":" << nOffset << ',';
        oss << "\"characters\":[";

        bool bFirst = true;
        for (int i = nStart; i < nEnd; ++i) {
                const CInfoCharBase *pChar = filtered[static_cast<size_t>(i)];
                if (!bFirst) {
                        oss << ',';
                }
                bFirst = false;

                std::string charName = ToUtf8String(pChar->m_strCharName);

                oss << '{';
                oss << "\"charId\":"    << pChar->m_dwCharID << ',';
                oss << "\"charName\":\"" << JsonUtils::Escape(charName) << "\",";
                oss << "\"level\":"     << static_cast<int>(pChar->m_wLevel) << ',';
                oss << "\"mapId\":"     << pChar->m_dwMapID << ',';
                oss << "\"x\":"         << pChar->m_nMapX << ',';
                oss << "\"y\":"         << pChar->m_nMapY << ',';
                // m_dwAccountID はログイン中のみ設定されるため、0 の場合は逆引きマップで補完する
                DWORD dwAccountId = pChar->m_dwAccountID;
                if (dwAccountId == 0 && !pChar->m_bNPC) {
                        std::map<DWORD, DWORD>::const_iterator it = charToAccount.find(pChar->m_dwCharID);
                        if (it != charToAccount.end()) {
                                dwAccountId = it->second;
                        }
                }
                oss << "\"accountId\":" << dwAccountId << ',';
                oss << "\"isNpc\":"     << (pChar->m_bNPC ? "true" : "false");
                oss << '}';
        }

        oss << "]}";

        pCharLib->Leave();

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}
