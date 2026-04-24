#include "stdafx.h"
#include "WeaponHandler.h"

#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

#include "Web/AuthProvider.h"
#include "Web/JsonUtils.h"
#include "MgrData.h"
#include "LibInfoItemWeapon.h"
#include "InfoItemWeapon.h"
#include "UraraSockTCPSBO.h"
#include "PacketITEM_RES_ITEMWEAPONINFO.h"

// ---------------------------------------------------------------------------
// 内部ヘルパー
// ---------------------------------------------------------------------------

namespace
{

// UTF-8 → CmyString（内部文字コード）
bool SetWeaponNameFromUtf8(CmyString &out, const std::string &utf8)
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
std::string WeaponNameToUtf8(const CmyString &value)
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

// JSON の "key":[n1,n2,...] を雑にパースして ARRAYDWORD へ格納する。
// 取得できれば true（該当キーが無い場合や null は false、空配列は true）。
// 単純実装: キー検索後の角括弧内をカンマで数値に分解するのみ。
bool TryGetDwordArray(const std::string &json, const std::string &key, ARRAYDWORD &out)
{
        size_t nKey = JsonUtils::FindKey(json, key);
        if (nKey == std::string::npos) {
                return false;
        }
        // 値開始位置まで移動
        size_t n = json.find(':', nKey);
        if (n == std::string::npos) {
                return false;
        }
        ++n;
        while (n < json.size() && (json[n] == ' ' || json[n] == '\t' || json[n] == '\r' || json[n] == '\n')) {
                ++n;
        }
        if (n >= json.size() || json[n] != '[') {
                return false;
        }
        ++n;
        out.clear();
        std::string num;
        for (; n < json.size(); ++n) {
                char c = json[n];
                if (c == ']') {
                        if (!num.empty()) {
                                out.push_back(static_cast<DWORD>(std::strtoul(num.c_str(), NULL, 10)));
                                num.clear();
                        }
                        return true;
                }
                if (c == ',') {
                        if (!num.empty()) {
                                out.push_back(static_cast<DWORD>(std::strtoul(num.c_str(), NULL, 10)));
                                num.clear();
                        }
                        continue;
                }
                if ((c >= '0' && c <= '9') || c == '-' || c == '+') {
                        num.push_back(c);
                        continue;
                }
                if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
                        continue;
                }
                // 想定外の文字（文字列要素など）が来たら失敗とする
                return false;
        }
        return false;
}

// CInfoItemWeapon → JSON 出力
void AppendWeaponJson(std::ostringstream &oss, const CInfoItemWeapon *pInfo)
{
        oss << '{';
        oss << "\"weaponInfoId\":"    << pInfo->m_dwWeaponInfoID    << ',';
        oss << "\"name\":\""          << JsonUtils::Escape(WeaponNameToUtf8(pInfo->m_strName)) << "\",";
        oss << "\"motionType\":"      << pInfo->m_dwMotionType      << ',';
        oss << "\"motionTypeStand\":" << pInfo->m_dwMotionTypeStand << ',';
        oss << "\"motionTypeWalk\":"  << pInfo->m_dwMotionTypeWalk  << ',';

        oss << "\"effectIdAtack\":[";
        for (size_t i = 0; i < pInfo->m_adwEffectIDAtack.size(); ++i) {
                if (i > 0) { oss << ','; }
                oss << pInfo->m_adwEffectIDAtack[i];
        }
        oss << "],";

        oss << "\"effectIdCritical\":[";
        for (size_t i = 0; i < pInfo->m_adwEffectIDCritical.size(); ++i) {
                if (i > 0) { oss << ','; }
                oss << pInfo->m_adwEffectIDCritical[i];
        }
        oss << ']';

        oss << '}';
}

// JSON → CInfoItemWeapon のフィールドを上書き（m_dwWeaponInfoID は呼び出し側が扱う）
bool ApplyJsonToWeapon(const std::string &json, CInfoItemWeapon *pInfo, std::string &outErr)
{
        outErr.clear();

        std::string name;
        if (JsonUtils::TryGetString(json, "name", name)) {
                if (!SetWeaponNameFromUtf8(pInfo->m_strName, name)) {
                        outErr = "invalid_name_encoding";
                        return false;
                }
        }

        int nVal = 0;
        if (JsonUtils::TryGetInt(json, "motionType", nVal) && nVal >= 0) {
                pInfo->m_dwMotionType = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "motionTypeStand", nVal) && nVal >= 0) {
                pInfo->m_dwMotionTypeStand = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "motionTypeWalk", nVal) && nVal >= 0) {
                pInfo->m_dwMotionTypeWalk = static_cast<DWORD>(nVal);
        }

        ARRAYDWORD tmpArr;
        if (TryGetDwordArray(json, "effectIdAtack", tmpArr)) {
                pInfo->m_adwEffectIDAtack = tmpArr;
        }
        if (TryGetDwordArray(json, "effectIdCritical", tmpArr)) {
                pInfo->m_adwEffectIDCritical = tmpArr;
        }

        return true;
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
// GET /api/weapons
// ---------------------------------------------------------------------------

CWeaponListHandler::CWeaponListHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CWeaponListHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuthAndBackend(request, m_pMgrData, response)) {
                return;
        }

        CLibInfoItemWeapon *pLib = m_pMgrData->GetLibInfoItemWeapon();
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
                const CInfoItemWeapon *pInfo =
                        static_cast<const CInfoItemWeapon *>(pLib->GetPtr(i));
                if (pInfo == NULL) {
                        continue;
                }
                if (!bFirst) { oss << ','; }
                bFirst = false;
                AppendWeaponJson(oss, pInfo);
        }
        pLib->Leave();

        oss << "]}";

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// POST /api/weapons
// 任意: name / motionType / motionTypeStand / motionTypeWalk /
//       effectIdAtack[] / effectIdCritical[]
// 成功時: 201 Created + 作成した武器情報の JSON
// ---------------------------------------------------------------------------

CWeaponCreateHandler::CWeaponCreateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CWeaponCreateHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuthAndBackend(request, m_pMgrData, response)) {
                return;
        }

        CLibInfoItemWeapon *pLib = m_pMgrData->GetLibInfoItemWeapon();
        CUraraSockTCPSBO *pSock = m_pMgrData->GetSock();
        if (pLib == NULL || pSock == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        pLib->Enter();

        CInfoItemWeapon *pInfo =
                static_cast<CInfoItemWeapon *>(pLib->GetNew());
        if (pInfo == NULL) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 500 Internal Server Error";
                response.SetJsonBody("{\"error\":\"failed_to_create\"}");
                return;
        }

        std::string errReason;
        if (!ApplyJsonToWeapon(request.body, pInfo, errReason)) {
                delete pInfo;
                pLib->Leave();
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody(std::string("{\"error\":\"") + errReason + "\"}");
                return;
        }

        // Add 時に m_dwWeaponInfoID == 0 なら新規 ID が採番される
        pInfo->m_dwWeaponInfoID = 0;
        pLib->Add(pInfo);

        // クライアント（ゲーム側）へ変更を通知
        CPacketITEM_RES_ITEMWEAPONINFO packet;
        packet.Make(pInfo);
        pSock->SendTo(0, &packet);

        std::ostringstream oss;
        AppendWeaponJson(oss, pInfo);

        pLib->Leave();

        response.statusLine = "HTTP/1.1 201 Created";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// PUT /api/weapons
// 必須: weaponInfoId
// 任意: 各フィールド
// ---------------------------------------------------------------------------

CWeaponUpdateHandler::CWeaponUpdateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CWeaponUpdateHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuthAndBackend(request, m_pMgrData, response)) {
                return;
        }

        CLibInfoItemWeapon *pLib = m_pMgrData->GetLibInfoItemWeapon();
        CUraraSockTCPSBO *pSock = m_pMgrData->GetSock();
        if (pLib == NULL || pSock == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nWeaponInfoId = 0;
        if (!JsonUtils::TryGetInt(request.body, "weaponInfoId", nWeaponInfoId) || nWeaponInfoId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_weaponInfoId\"}");
                return;
        }

        pLib->Enter();

        CInfoItemWeapon *pInfo =
                static_cast<CInfoItemWeapon *>(pLib->GetPtr(static_cast<DWORD>(nWeaponInfoId)));
        if (pInfo == NULL) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        std::string errReason;
        if (!ApplyJsonToWeapon(request.body, pInfo, errReason)) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody(std::string("{\"error\":\"") + errReason + "\"}");
                return;
        }

        // クライアントへ変更を通知
        CPacketITEM_RES_ITEMWEAPONINFO packet;
        packet.Make(pInfo);
        pSock->SendTo(0, &packet);

        std::ostringstream oss;
        AppendWeaponJson(oss, pInfo);

        pLib->Leave();

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// DELETE /api/weapons
// 必須: weaponInfoId
// ※ネイティブ版に対応する通知パケット（RES_ITEMWEAPONINFO の delete 版）は
//   未実装のため、ここではサーバー内ライブラリからの削除のみ行う。
//   ゲームクライアント側は再接続時に全件要求で同期される。
// ---------------------------------------------------------------------------

CWeaponDeleteHandler::CWeaponDeleteHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CWeaponDeleteHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuthAndBackend(request, m_pMgrData, response)) {
                return;
        }

        CLibInfoItemWeapon *pLib = m_pMgrData->GetLibInfoItemWeapon();
        if (pLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nWeaponInfoId = 0;
        if (!JsonUtils::TryGetInt(request.body, "weaponInfoId", nWeaponInfoId) || nWeaponInfoId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_weaponInfoId\"}");
                return;
        }

        DWORD dwWeaponInfoId = static_cast<DWORD>(nWeaponInfoId);

        pLib->Enter();

        CInfoItemWeapon *pInfo =
                static_cast<CInfoItemWeapon *>(pLib->GetPtr(dwWeaponInfoId));
        if (pInfo == NULL) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        pLib->Delete(dwWeaponInfoId);
        pLib->Leave();

        std::ostringstream oss;
        oss << "{\"deleted\":" << dwWeaponInfoId << "}";

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}
