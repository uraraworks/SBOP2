#include "StdAfx.h"
#include "EfcBalloonHandler.h"

#include <sstream>
#include <string>
#include <vector>

#include "Web/AuthProvider.h"
#include "Web/JsonUtils.h"
#include "MgrData.h"
#include "LibInfoEfcBalloon.h"
#include "InfoEfcBalloon.h"
#include "UraraSockTCPSBO.h"
#include "PacketEFFECT_BALLOONINFO.h"

// ---------------------------------------------------------------------------
// 内部ヘルパー
// ---------------------------------------------------------------------------

namespace
{

// UTF-8 → CmyString（内部文字コード）
bool SetBalloonNameFromUtf8(CmyString &out, const std::string &utf8)
{
        if (utf8.empty()) {
                out = _T("");
                return true;
        }
#ifdef _UNICODE
        int wideLen = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), static_cast<int>(utf8.size()), NULL, 0);
        if (wideLen <= 0) {
                return false;
        }
        std::vector<wchar_t> wide(static_cast<size_t>(wideLen) + 1, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), static_cast<int>(utf8.size()), &wide[0], wideLen);
        out = &wide[0];
#else
        int wideLen = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), static_cast<int>(utf8.size()), NULL, 0);
        if (wideLen <= 0) {
                return false;
        }
        std::vector<wchar_t> wide(static_cast<size_t>(wideLen) + 1, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), static_cast<int>(utf8.size()), &wide[0], wideLen);
        int mbLen = WideCharToMultiByte(932, 0, &wide[0], wideLen, NULL, 0, NULL, NULL);
        if (mbLen <= 0) {
                return false;
        }
        std::vector<char> mb(static_cast<size_t>(mbLen) + 1, '\0');
        WideCharToMultiByte(932, 0, &wide[0], wideLen, &mb[0], mbLen, NULL, NULL);
        out = &mb[0];
#endif
        return true;
}

// CmyString → UTF-8
std::string BalloonNameToUtf8(const CmyString &value)
{
#ifdef _UNICODE
        LPCWSTR pszWide = static_cast<LPCWSTR>(value);
        if (pszWide == NULL || pszWide[0] == L'\0') {
                return std::string();
        }
        int utf8Len = WideCharToMultiByte(CP_UTF8, 0, pszWide, -1, NULL, 0, NULL, NULL);
        if (utf8Len <= 0) {
                return std::string();
        }
        std::string utf8(static_cast<size_t>(utf8Len), '\0');
        WideCharToMultiByte(CP_UTF8, 0, pszWide, -1, &utf8[0], utf8Len, NULL, NULL);
        if (!utf8.empty() && utf8.back() == '\0') {
                utf8.resize(utf8.size() - 1);
        }
        return utf8;
#else
        LPCSTR psz = static_cast<LPCSTR>(value);
        if (psz == NULL || psz[0] == '\0') {
                return std::string();
        }
        int wideLen = MultiByteToWideChar(932, 0, psz, -1, NULL, 0);
        if (wideLen <= 0) {
                return std::string(psz);
        }
        std::vector<wchar_t> wide(static_cast<size_t>(wideLen), L'\0');
        MultiByteToWideChar(932, 0, psz, -1, &wide[0], wideLen);
        int utf8Len = WideCharToMultiByte(CP_UTF8, 0, &wide[0], -1, NULL, 0, NULL, NULL);
        if (utf8Len <= 0) {
                return std::string();
        }
        std::string utf8(static_cast<size_t>(utf8Len), '\0');
        WideCharToMultiByte(CP_UTF8, 0, &wide[0], -1, &utf8[0], utf8Len, NULL, NULL);
        if (!utf8.empty() && utf8.back() == '\0') {
                utf8.resize(utf8.size() - 1);
        }
        return utf8;
#endif
}

// CInfoEfcBalloon → JSON 出力
void AppendBalloonJson(std::ostringstream &oss, const CInfoEfcBalloon *pInfo)
{
        oss << '{';
        oss << "\"efcBalloonId\":" << pInfo->m_dwEfcBalloonID << ',';
        oss << "\"listId\":"       << pInfo->m_dwListID       << ',';
        oss << "\"animeId\":"      << pInfo->m_dwAnimeID      << ',';
        oss << "\"wait\":"         << static_cast<int>(pInfo->m_byWait) << ',';
        oss << "\"loop\":"         << (pInfo->m_bLoop ? "true" : "false") << ',';
        oss << "\"soundId\":"      << pInfo->m_dwSoundID      << ',';
        oss << "\"grpId\":"        << pInfo->m_dwGrpID        << ',';
        oss << "\"name\":\""       << JsonUtils::Escape(BalloonNameToUtf8(pInfo->m_strName)) << "\"";
        oss << '}';
}

// JSON → CInfoEfcBalloon のフィールドを上書き（m_dwEfcBalloonID は呼び出し側が扱う）
bool ApplyJsonToBalloon(const std::string &json, CInfoEfcBalloon *pInfo, std::string &outErr)
{
        outErr.clear();

        std::string name;
        if (JsonUtils::TryGetString(json, "name", name)) {
                if (!SetBalloonNameFromUtf8(pInfo->m_strName, name)) {
                        outErr = "invalid_name_encoding";
                        return false;
                }
        }

        int nVal = 0;
        if (JsonUtils::TryGetInt(json, "listId", nVal) && nVal >= 0) {
                pInfo->m_dwListID = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "animeId", nVal) && nVal >= 0) {
                pInfo->m_dwAnimeID = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "wait", nVal) && nVal >= 0 && nVal <= 255) {
                pInfo->m_byWait = static_cast<BYTE>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "soundId", nVal) && nVal >= 0) {
                pInfo->m_dwSoundID = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "grpId", nVal) && nVal >= 0) {
                pInfo->m_dwGrpID = static_cast<DWORD>(nVal);
        }

        bool bVal = false;
        if (JsonUtils::TryGetBool(json, "loop", bVal)) {
                pInfo->m_bLoop = bVal ? TRUE : FALSE;
        }

        return true;
}

// 指定 listId の噴出し情報を CPacketEFFECT_BALLOONINFO で全クライアントへ配信
// ネイティブ版 RecvProcADMIN_EFC_RENEWBALLOON と同等の挙動。
void BroadcastBalloonList(CMgrData *pMgrData, DWORD dwListID)
{
        if (pMgrData == NULL) {
                return;
        }
        CLibInfoEfcBalloon *pLib = pMgrData->GetLibInfoEfcBalloon();
        CUraraSockTCPSBO   *pSock = pMgrData->GetSock();
        if (pLib == NULL || pSock == NULL) {
                return;
        }

        CPacketEFFECT_BALLOONINFO packet;
        packet.Make(dwListID, pLib);
        pSock->SendTo(0, &packet);
}

// 共通: 認証チェック & MgrData null チェック（通っていれば true 返却）
bool CheckAuthAndBackend(const HttpRequest &request, CMgrData *pMgrData, HttpResponse &response)
{
        AuthProvider::AuthContext authContext;
        AuthProvider::AuthStatus authStatus =
                AuthProvider::Authenticate(request, pMgrData, authContext);
        if (authStatus == AuthProvider::AuthStatusBackendUnavailable) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return false;
        }
        if (pMgrData == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return false;
        }
        return true;
}

} // namespace

// ---------------------------------------------------------------------------
// GET /api/efc-balloons
// ---------------------------------------------------------------------------

CEfcBalloonListHandler::CEfcBalloonListHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CEfcBalloonListHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuthAndBackend(request, m_pMgrData, response)) {
                return;
        }

        CLibInfoEfcBalloon *pLib = m_pMgrData->GetLibInfoEfcBalloon();
        if (pLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        std::ostringstream oss;
        oss << "{\"items\":[";

        pLib->Enter();
        int nCount = pLib->GetCount();
        bool bFirst = true;
        for (int i = 0; i < nCount; ++i) {
                const CInfoEfcBalloon *pInfo =
                        static_cast<const CInfoEfcBalloon *>(pLib->GetPtr(i));
                if (pInfo == NULL) {
                        continue;
                }
                if (!bFirst) { oss << ','; }
                bFirst = false;
                AppendBalloonJson(oss, pInfo);
        }
        pLib->Leave();

        oss << "]}";

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// POST /api/efc-balloons
// 必須: listId (>0)
// 任意: animeId / wait / loop / soundId / grpId / name
// 成功時: 201 Created + 作成した噴出し情報 JSON
// ---------------------------------------------------------------------------

CEfcBalloonCreateHandler::CEfcBalloonCreateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CEfcBalloonCreateHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuthAndBackend(request, m_pMgrData, response)) {
                return;
        }

        CLibInfoEfcBalloon *pLib = m_pMgrData->GetLibInfoEfcBalloon();
        if (pLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nListId = 0;
        if (!JsonUtils::TryGetInt(request.body, "listId", nListId) || nListId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_listId\"}");
                return;
        }

        pLib->Enter();

        CInfoEfcBalloon *pInfo =
                static_cast<CInfoEfcBalloon *>(pLib->GetNew());
        if (pInfo == NULL) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 500 Internal Server Error";
                response.SetJsonBody("{\"error\":\"failed_to_create\"}");
                return;
        }

        std::string errReason;
        if (!ApplyJsonToBalloon(request.body, pInfo, errReason)) {
                delete pInfo;
                pLib->Leave();
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody(std::string("{\"error\":\"") + errReason + "\"}");
                return;
        }

        // Add 時に m_dwEfcBalloonID == 0 なら新規 ID が採番される
        pInfo->m_dwEfcBalloonID = 0;
        pLib->Add(pInfo);
        pLib->Sort();

        DWORD dwListID = pInfo->m_dwListID;

        std::ostringstream oss;
        AppendBalloonJson(oss, pInfo);

        pLib->Leave();

        // クライアントへ変更を配信（該当 listId の全コマ）
        BroadcastBalloonList(m_pMgrData, dwListID);

        response.statusLine = "HTTP/1.1 201 Created";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// PUT /api/efc-balloons
// 必須: efcBalloonId
// 任意: 各フィールド
// ---------------------------------------------------------------------------

CEfcBalloonUpdateHandler::CEfcBalloonUpdateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CEfcBalloonUpdateHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuthAndBackend(request, m_pMgrData, response)) {
                return;
        }

        CLibInfoEfcBalloon *pLib = m_pMgrData->GetLibInfoEfcBalloon();
        if (pLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nEfcBalloonId = 0;
        if (!JsonUtils::TryGetInt(request.body, "efcBalloonId", nEfcBalloonId) || nEfcBalloonId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_efcBalloonId\"}");
                return;
        }

        DWORD dwEfcBalloonID = static_cast<DWORD>(nEfcBalloonId);

        pLib->Enter();

        // ID から該当コマを線形探索
        CInfoEfcBalloon *pInfo = NULL;
        int nCount = pLib->GetCount();
        for (int i = 0; i < nCount; ++i) {
                CInfoEfcBalloon *pTmp =
                        static_cast<CInfoEfcBalloon *>(pLib->GetPtr(i));
                if (pTmp == NULL) { continue; }
                if (pTmp->m_dwEfcBalloonID == dwEfcBalloonID) {
                        pInfo = pTmp;
                        break;
                }
        }
        if (pInfo == NULL) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        DWORD dwOldListID = pInfo->m_dwListID;

        std::string errReason;
        if (!ApplyJsonToBalloon(request.body, pInfo, errReason)) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody(std::string("{\"error\":\"") + errReason + "\"}");
                return;
        }

        pLib->Sort();

        DWORD dwNewListID = pInfo->m_dwListID;

        std::ostringstream oss;
        AppendBalloonJson(oss, pInfo);

        pLib->Leave();

        // クライアントへ配信（listId が変わった場合は新旧両方）
        BroadcastBalloonList(m_pMgrData, dwNewListID);
        if (dwOldListID != dwNewListID) {
                BroadcastBalloonList(m_pMgrData, dwOldListID);
        }

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// DELETE /api/efc-balloons
// 必須: efcBalloonId
// ---------------------------------------------------------------------------

CEfcBalloonDeleteHandler::CEfcBalloonDeleteHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CEfcBalloonDeleteHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuthAndBackend(request, m_pMgrData, response)) {
                return;
        }

        CLibInfoEfcBalloon *pLib = m_pMgrData->GetLibInfoEfcBalloon();
        if (pLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nEfcBalloonId = 0;
        if (!JsonUtils::TryGetInt(request.body, "efcBalloonId", nEfcBalloonId) || nEfcBalloonId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_efcBalloonId\"}");
                return;
        }

        DWORD dwEfcBalloonID = static_cast<DWORD>(nEfcBalloonId);

        pLib->Enter();

        // ID から listId を取得（配信時に必要）
        DWORD dwListID = 0;
        bool bFound = false;
        int nCount = pLib->GetCount();
        for (int i = 0; i < nCount; ++i) {
                CInfoEfcBalloon *pTmp =
                        static_cast<CInfoEfcBalloon *>(pLib->GetPtr(i));
                if (pTmp == NULL) { continue; }
                if (pTmp->m_dwEfcBalloonID == dwEfcBalloonID) {
                        dwListID = pTmp->m_dwListID;
                        bFound = true;
                        break;
                }
        }
        if (!bFound) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        pLib->Delete(dwEfcBalloonID);

        pLib->Leave();

        // クライアントへ該当 listId の残りコマを配信（空になった場合は終端のみ）
        BroadcastBalloonList(m_pMgrData, dwListID);

        std::ostringstream oss;
        oss << "{\"deleted\":" << dwEfcBalloonID << "}";

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}
