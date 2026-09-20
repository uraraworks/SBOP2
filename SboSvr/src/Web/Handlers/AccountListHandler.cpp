#include "StdAfx.h"
#include "AccountListHandler.h"

#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <cctype>

#include "Web/AuthProvider.h"
#include "Web/JsonUtils.h"
#include "MgrData.h"
#include "LibInfo/LibInfoAccount.h"
#include "LibInfo/LibInfoCharSvr.h"
#include "Info/InfoAccount.h"
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

// アカウント名に部分一致するか判定（大文字小文字を区別しない ASCII 比較）
bool NameContains(const std::string &accountName, const std::string &filter)
{
        if (filter.empty()) {
                return true;
        }
        if (accountName.size() < filter.size()) {
                return false;
        }
        for (size_t i = 0; i <= accountName.size() - filter.size(); ++i) {
                bool match = true;
                for (size_t j = 0; j < filter.size(); ++j) {
                        unsigned char a = static_cast<unsigned char>(accountName[i + j]);
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

// 入力文字列が数値（10進整数）かどうか判定する
bool IsNumeric(const std::string &value)
{
        if (value.empty()) {
                return false;
        }
        size_t nStart = 0;
        if (value[0] == '+' || value[0] == '-') {
                nStart = 1;
        }
        if (nStart >= value.size()) {
                return false;
        }
        for (size_t i = nStart; i < value.size(); ++i) {
                if (!std::isdigit(static_cast<unsigned char>(value[i]))) {
                        return false;
                }
        }
        return true;
}

// 検索結果 1 件分の作業用構造体
struct AccountRow
{
        const CInfoAccount *pAccount;
        std::string accountName;
        int charCount;
        bool bOnline;
        std::vector<std::string> charNames;
};

// sort パラメータで指定された項目の比較値を取得する
long long GetSortKey(const AccountRow &row, const std::string &sortField)
{
        if (sortField == "account") {
                // 文字列は別途 std::string 比較するためここでは使わない
                return 0;
        }
        if (sortField == "lastLogin") {
                return static_cast<long long>(row.pAccount->m_dwTimeLastLogin);
        }
        if (sortField == "created") {
                return static_cast<long long>(row.pAccount->m_dwTimeMakeAccount);
        }
        if (sortField == "loginCount") {
                return static_cast<long long>(row.pAccount->m_dwLoginCount);
        }
        if (sortField == "charCount") {
                return static_cast<long long>(row.charCount);
        }
        // 既定・"accountId"
        return static_cast<long long>(row.pAccount->m_dwAccountID);
}

} // namespace

// ---------------------------------------------------------------------------
// CAccountListHandler  GET /api/accounts
// ---------------------------------------------------------------------------

CAccountListHandler::CAccountListHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CAccountListHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        // 認証チェック
        AuthProvider::AuthContext authContext;
        AuthProvider::AuthStatus authStatus = AuthProvider::Authenticate(request, m_pMgrData, authContext);
        if (authStatus == AuthProvider::AuthStatusBackendUnavailable) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }
        if (authStatus != AuthProvider::AuthStatusOk) {
                response.statusLine = "HTTP/1.1 401 Unauthorized";
                response.SetJsonBody("{\"error\":\"unauthorized\"}");
                return;
        }

        if (m_pMgrData == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        CLibInfoAccount *pAccountLib = m_pMgrData->GetLibInfoAccount();
        if (pAccountLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        // クエリパラメータを取得する
        std::string filterName;
        TryGetQueryString(request.path, "q", filterName);

        int filterAccountId = -1;
        TryGetQueryInt(request.path, "accountId", filterAccountId);

        // search: アカウントID完全一致 / アカウント名部分一致 / 所持キャラ名部分一致 の OR 条件
        std::string filterSearch;
        TryGetQueryString(request.path, "search", filterSearch);
        bool bSearchNumeric = IsNumeric(filterSearch);
        long lSearchId = bSearchNumeric ? std::strtol(filterSearch.c_str(), NULL, 10) : 0;

        int nLastLoginBefore = -1;
        bool bHasLastLoginBefore = TryGetQueryInt(request.path, "lastLoginBefore", nLastLoginBefore);

        int nLastLoginAfter = -1;
        bool bHasLastLoginAfter = TryGetQueryInt(request.path, "lastLoginAfter", nLastLoginAfter);

        int nNeverLoggedIn = -1;
        TryGetQueryInt(request.path, "neverLoggedIn", nNeverLoggedIn);

        int nCreatedBefore = -1;
        bool bHasCreatedBefore = TryGetQueryInt(request.path, "createdBefore", nCreatedBefore);

        int nCreatedAfter = -1;
        bool bHasCreatedAfter = TryGetQueryInt(request.path, "createdAfter", nCreatedAfter);

        int nDisabled = -1;  // -1 = 未指定、0 = 通常のみ、1 = ログイン拒否のみ
        TryGetQueryInt(request.path, "disabled", nDisabled);

        int nAdminOnly = -1;
        TryGetQueryInt(request.path, "admin", nAdminOnly);

        int nOnlineOnly = -1;
        TryGetQueryInt(request.path, "online", nOnlineOnly);

        int nMinChars = -1;
        bool bHasMinChars = TryGetQueryInt(request.path, "minChars", nMinChars);

        int nMaxChars = -1;
        bool bHasMaxChars = TryGetQueryInt(request.path, "maxChars", nMaxChars);

        std::string sortParam;
        TryGetQueryString(request.path, "sort", sortParam);
        bool bSortDescending = false;
        std::string sortField = "accountId";
        if (!sortParam.empty()) {
                if (sortParam[0] == '-') {
                        bSortDescending = true;
                        sortField = sortParam.substr(1);
                } else {
                        sortField = sortParam;
                }
        }

        int nLimit = 100;
        {
                int tmp = 0;
                if (TryGetQueryInt(request.path, "limit", tmp) && tmp > 0 && tmp <= 500) {
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

        // charID → キャラ名 のマップを構築する
        // pAccountLib のロックとネストしないよう、pAccountLib->Enter() の前に構築を済ませる
        // （CharacterListHandler.cpp の charToAccount 構築と同じ考え方）
        std::map<DWORD, std::string> charNameById;
        {
                CLibInfoCharSvr *pCharLib = m_pMgrData->GetLibInfoChar();
                if (pCharLib != NULL) {
                        pCharLib->Enter();
                        int nCharTotal = pCharLib->GetCount();
                        for (int i = 0; i < nCharTotal; ++i) {
                                const CInfoCharBase *pChar = static_cast<const CInfoCharBase *>(pCharLib->GetPtr(i));
                                if (pChar == NULL) {
                                        continue;
                                }
                                charNameById[pChar->m_dwCharID] = ToUtf8String(pChar->m_strCharName);
                        }
                        pCharLib->Leave();
                }
        }

        // アカウントライブラリをロックして全件スキャン・フィルタリング
        pAccountLib->Enter();

        int nTotal = pAccountLib->GetCount();

        std::vector<AccountRow> filtered;
        filtered.reserve(static_cast<size_t>(nTotal));

        for (int i = 0; i < nTotal; ++i) {
                PCInfoAccount pAcc = static_cast<PCInfoAccount>(pAccountLib->GetPtr(i));
                if (pAcc == NULL) {
                        continue;
                }

                std::string accountName = ToUtf8String(pAcc->m_strAccount);

                // q: アカウント名部分一致
                if (!filterName.empty() && !NameContains(accountName, filterName)) {
                        continue;
                }

                // accountId 完全一致
                if ((filterAccountId >= 0) && (static_cast<int>(pAcc->m_dwAccountID) != filterAccountId)) {
                        continue;
                }

                // 最終ログイン範囲
                if (bHasLastLoginBefore && !(static_cast<long long>(pAcc->m_dwTimeLastLogin) < nLastLoginBefore)) {
                        continue;
                }
                if (bHasLastLoginAfter && !(static_cast<long long>(pAcc->m_dwTimeLastLogin) > nLastLoginAfter)) {
                        continue;
                }

                // 一度もログインしていない
                if ((nNeverLoggedIn == 1) && (pAcc->m_dwTimeLastLogin != 0)) {
                        continue;
                }

                // 作成日時範囲
                if (bHasCreatedBefore && !(static_cast<long long>(pAcc->m_dwTimeMakeAccount) < nCreatedBefore)) {
                        continue;
                }
                if (bHasCreatedAfter && !(static_cast<long long>(pAcc->m_dwTimeMakeAccount) > nCreatedAfter)) {
                        continue;
                }

                // ログイン拒否フラグ
                if ((nDisabled == 1) && !pAcc->m_bDisable) {
                        continue;
                }
                if ((nDisabled == 0) && pAcc->m_bDisable) {
                        continue;
                }

                // 管理者レベル付きのみ
                if ((nAdminOnly == 1) && (pAcc->m_nAdminLevel <= 0)) {
                        continue;
                }

                bool bOnline = (pAcc->m_dwSessionID != 0);

                // 接続中のみ
                if ((nOnlineOnly == 1) && !bOnline) {
                        continue;
                }

                int nCharCount = pAcc->GetCharCount();

                // 所持キャラ数範囲
                if (bHasMinChars && (nCharCount < nMinChars)) {
                        continue;
                }
                if (bHasMaxChars && (nCharCount > nMaxChars)) {
                        continue;
                }

                // 所持キャラ名一覧（charIds と同じ順序）
                std::vector<std::string> charNames;
                int nCharTableCount = static_cast<int>(pAcc->m_adwCharID.size());
                charNames.reserve(static_cast<size_t>(nCharTableCount));
                for (int j = 0; j < nCharTableCount; ++j) {
                        DWORD dwCharID = pAcc->m_adwCharID[static_cast<size_t>(j)];
                        std::map<DWORD, std::string>::const_iterator it = charNameById.find(dwCharID);
                        charNames.push_back((it != charNameById.end()) ? it->second : std::string());
                }

                // search: アカウントID完全一致 / アカウント名部分一致 / キャラ名部分一致（OR）
                if (!filterSearch.empty()) {
                        bool bMatched = false;
                        if (bSearchNumeric && (static_cast<long>(pAcc->m_dwAccountID) == lSearchId)) {
                                bMatched = true;
                        }
                        if (!bMatched && NameContains(accountName, filterSearch)) {
                                bMatched = true;
                        }
                        if (!bMatched) {
                                for (size_t j = 0; j < charNames.size(); ++j) {
                                        if (NameContains(charNames[j], filterSearch)) {
                                                bMatched = true;
                                                break;
                                        }
                                }
                        }
                        if (!bMatched) {
                                continue;
                        }
                }

                AccountRow row;
                row.pAccount = pAcc;
                row.accountName = accountName;
                row.charCount = nCharCount;
                row.bOnline = bOnline;
                row.charNames = charNames;
                filtered.push_back(row);
        }

        // ソート
        if (sortField == "account") {
                std::sort(filtered.begin(), filtered.end(),
                        [bSortDescending](const AccountRow &a, const AccountRow &b) {
                                if (bSortDescending) {
                                        return a.accountName > b.accountName;
                                }
                                return a.accountName < b.accountName;
                        });
        } else {
                std::sort(filtered.begin(), filtered.end(),
                        [bSortDescending, &sortField](const AccountRow &a, const AccountRow &b) {
                                long long keyA = GetSortKey(a, sortField);
                                long long keyB = GetSortKey(b, sortField);
                                if (bSortDescending) {
                                        return keyA > keyB;
                                }
                                return keyA < keyB;
                        });
        }

        // ページング
        int nFilteredTotal = static_cast<int>(filtered.size());
        int nStart = (nOffset < nFilteredTotal) ? nOffset : nFilteredTotal;
        int nEnd   = (nStart + nLimit < nFilteredTotal) ? (nStart + nLimit) : nFilteredTotal;

        // JSON レスポンスを構築する
        std::ostringstream oss;
        oss << '{';
        oss << "\"total\":" << nFilteredTotal << ',';
        oss << "\"offset\":" << nOffset << ',';
        oss << "\"limit\":" << nLimit << ',';
        oss << "\"items\":[";

        bool bFirst = true;
        for (int i = nStart; i < nEnd; ++i) {
                const AccountRow &row = filtered[static_cast<size_t>(i)];
                const CInfoAccount *pAcc = row.pAccount;

                if (!bFirst) {
                        oss << ',';
                }
                bFirst = false;

                oss << '{';
                oss << "\"accountId\":" << pAcc->m_dwAccountID << ',';
                oss << "\"account\":\"" << JsonUtils::Escape(row.accountName) << "\",";
                oss << "\"adminLevel\":" << pAcc->m_nAdminLevel << ',';
                oss << "\"disabled\":" << (pAcc->m_bDisable ? "true" : "false") << ',';
                oss << "\"online\":" << (row.bOnline ? "true" : "false") << ',';
                oss << "\"charCount\":" << row.charCount << ',';

                oss << "\"charIds\":[";
                int nCharTableCount = static_cast<int>(pAcc->m_adwCharID.size());
                for (int j = 0; j < nCharTableCount; ++j) {
                        if (j != 0) {
                                oss << ',';
                        }
                        oss << pAcc->m_adwCharID[static_cast<size_t>(j)];
                }
                oss << "],";

                oss << "\"charNames\":[";
                for (size_t j = 0; j < row.charNames.size(); ++j) {
                        if (j != 0) {
                                oss << ',';
                        }
                        oss << '"' << JsonUtils::Escape(row.charNames[j]) << '"';
                }
                oss << "],";

                oss << "\"currentCharId\":" << pAcc->m_dwCharID << ',';
                oss << "\"timeMakeAccount\":" << pAcc->m_dwTimeMakeAccount << ',';
                oss << "\"timeLastLogin\":" << pAcc->m_dwTimeLastLogin << ',';
                oss << "\"loginCount\":" << pAcc->m_dwLoginCount;
                oss << '}';
        }

        oss << "]}";

        pAccountLib->Leave();

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}
