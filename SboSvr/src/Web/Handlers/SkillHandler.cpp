#include "StdAfx.h"
#include "SkillHandler.h"

#include <sstream>
#include <string>
#include <vector>

#include "Web/AuthProvider.h"
#include "Web/JsonUtils.h"
#include "MgrData.h"
#include "UraraSockTCPSBO.h"
#include "LibInfoSkill.h"
#include "InfoSkillBase.h"
#include "InfoSkillMOVEATACK.h"
#include "InfoSkillHEAL.h"
#include "PacketSKILL_SKILLINFO.h"

// ---------------------------------------------------------------------------
// 内部ヘルパー（ファイル内 static 共通化）
// ---------------------------------------------------------------------------

namespace
{

// UTF-8 → CmyString（内部文字コード）
bool SetSkillNameFromUtf8(CmyString &out, const std::string &utf8)
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
std::string SkillNameToUtf8(const CmyString &value)
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

// InfoSkillBase → JSON 出力（派生クラスフィールドも含む）
void AppendSkillJson(std::ostringstream &oss, const CInfoSkillBase *pInfo)
{
        oss << '{';
        oss << "\"skillId\":"   << pInfo->m_dwSkillID    << ',';
        oss << "\"name\":\""    << JsonUtils::Escape(SkillNameToUtf8(pInfo->m_strName)) << "\",";
        oss << "\"sp\":"        << pInfo->m_dwSP         << ',';
        oss << "\"iconId\":"    << pInfo->m_dwIconID     << ',';
        oss << "\"typeMain\":"  << pInfo->m_nTypeMain    << ',';
        oss << "\"typeSub\":"   << pInfo->m_nTypeSub     << ',';
        oss << "\"use\":"       << pInfo->m_nUse         << ',';
        oss << "\"classType\":" << pInfo->m_nClassType;

        // 派生クラス固有フィールド
        if (pInfo->m_nClassType == INFOSKILLTYPE_MOVEATACK) {
                const CInfoSkillMOVEATACK *pMA = static_cast<const CInfoSkillMOVEATACK *>(pInfo);
                oss << ',';
                oss << "\"targetType\":"    << pMA->m_dwTartgetType   << ',';
                oss << "\"hitEffectId\":"   << pMA->m_dwHitEffectID   << ',';
                oss << "\"effectId0\":"     << pMA->m_adwEffectID[0]  << ',';
                oss << "\"effectId1\":"     << pMA->m_adwEffectID[1]  << ',';
                oss << "\"effectId2\":"     << pMA->m_adwEffectID[2]  << ',';
                oss << "\"effectId3\":"     << pMA->m_adwEffectID[3]  << ',';
                oss << "\"putType\":"       << pMA->m_dwPutType       << ',';
                oss << "\"aliveTime\":"     << pMA->m_dwAliveTime     << ',';
                oss << "\"waitTime\":"      << pMA->m_dwWaitTime      << ',';
                oss << "\"value1\":"        << pMA->m_dwValue1        << ',';
                oss << "\"value2\":"        << pMA->m_dwValue2        << ',';
                oss << "\"distance\":"      << pMA->m_dwDistance      << ',';
                oss << "\"hitQuit\":"       << (pMA->m_bHitQuit ? "true" : "false") << ',';
                oss << "\"distanceDelete\":" << (pMA->m_bDistanceDelete ? "true" : "false");
        } else if (pInfo->m_nClassType == INFOSKILLTYPE_HEAL) {
                const CInfoSkillHEAL *pHeal = static_cast<const CInfoSkillHEAL *>(pInfo);
                oss << ',';
                oss << "\"area\":"        << pHeal->m_dwArea        << ',';
                oss << "\"healType\":"    << pHeal->m_dwHealType    << ',';
                oss << "\"hitEffectId\":" << pHeal->m_dwHitEffectID << ',';
                oss << "\"value1\":"      << pHeal->m_dwValue1      << ',';
                oss << "\"value2\":"      << pHeal->m_dwValue2      << ',';
                oss << "\"distance\":"    << pHeal->m_dwDistance;
        }

        oss << '}';
}

// JSON から InfoSkillBase の基底フィールドを上書きする
bool ApplyJsonToSkillBase(const std::string &json, CInfoSkillBase *pInfo, std::string &outErr)
{
        outErr.clear();

        std::string name;
        if (JsonUtils::TryGetString(json, "name", name)) {
                if (!SetSkillNameFromUtf8(pInfo->m_strName, name)) {
                        outErr = "invalid_name_encoding";
                        return false;
                }
        }

        int nVal = 0;
        if (JsonUtils::TryGetInt(json, "sp", nVal) && nVal >= 0) {
                pInfo->m_dwSP = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "iconId", nVal) && nVal >= 0) {
                pInfo->m_dwIconID = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "use", nVal) && nVal >= 0 && nVal < SKILLUSE_MAX) {
                pInfo->m_nUse = nVal;
        }

        return true;
}

// JSON から CInfoSkillMOVEATACK 固有フィールドを上書きする
void ApplyJsonToSkillMOVEATACK(const std::string &json, CInfoSkillMOVEATACK *pMA)
{
        int nVal = 0;
        bool bVal = false;

        if (JsonUtils::TryGetInt(json, "targetType", nVal) && nVal >= 0) {
                pMA->m_dwTartgetType = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "hitEffectId", nVal) && nVal >= 0) {
                pMA->m_dwHitEffectID = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "effectId0", nVal) && nVal >= 0) {
                pMA->m_adwEffectID[0] = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "effectId1", nVal) && nVal >= 0) {
                pMA->m_adwEffectID[1] = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "effectId2", nVal) && nVal >= 0) {
                pMA->m_adwEffectID[2] = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "effectId3", nVal) && nVal >= 0) {
                pMA->m_adwEffectID[3] = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "putType", nVal) && nVal >= 0) {
                pMA->m_dwPutType = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "aliveTime", nVal) && nVal >= 0) {
                pMA->m_dwAliveTime = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "waitTime", nVal) && nVal >= 0) {
                pMA->m_dwWaitTime = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "value1", nVal) && nVal >= 0) {
                pMA->m_dwValue1 = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "value2", nVal) && nVal >= 0) {
                pMA->m_dwValue2 = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "distance", nVal) && nVal >= 0) {
                pMA->m_dwDistance = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetBool(json, "hitQuit", bVal)) {
                pMA->m_bHitQuit = bVal ? TRUE : FALSE;
        } else if (JsonUtils::TryGetInt(json, "hitQuit", nVal)) {
                pMA->m_bHitQuit = (nVal != 0) ? TRUE : FALSE;
        }
        if (JsonUtils::TryGetBool(json, "distanceDelete", bVal)) {
                pMA->m_bDistanceDelete = bVal ? TRUE : FALSE;
        } else if (JsonUtils::TryGetInt(json, "distanceDelete", nVal)) {
                pMA->m_bDistanceDelete = (nVal != 0) ? TRUE : FALSE;
        }
}

// JSON から CInfoSkillHEAL 固有フィールドを上書きする
void ApplyJsonToSkillHEAL(const std::string &json, CInfoSkillHEAL *pHeal)
{
        int nVal = 0;
        if (JsonUtils::TryGetInt(json, "area", nVal) && nVal >= 0) {
                pHeal->m_dwArea = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "healType", nVal) && nVal >= 0) {
                pHeal->m_dwHealType = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "hitEffectId", nVal) && nVal >= 0) {
                pHeal->m_dwHitEffectID = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "value1", nVal) && nVal >= 0) {
                pHeal->m_dwValue1 = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "value2", nVal) && nVal >= 0) {
                pHeal->m_dwValue2 = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "distance", nVal) && nVal >= 0) {
                pHeal->m_dwDistance = static_cast<DWORD>(nVal);
        }
}

// 共通: 認証チェック & MgrData null チェック
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
// GET /api/skills
// ---------------------------------------------------------------------------

CSkillListHandler::CSkillListHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CSkillListHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuthAndBackend(request, m_pMgrData, response)) {
                return;
        }

        CLibInfoSkill *pLib = m_pMgrData->GetLibInfoSkill();
        if (pLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        std::ostringstream oss;
        oss << "{\"skills\":[";

        pLib->Enter();
        int nCount = pLib->GetCount();
        bool bFirst = true;
        for (int i = 0; i < nCount; ++i) {
                const CInfoSkillBase *pInfo =
                        static_cast<const CInfoSkillBase *>(pLib->GetPtr(i));
                if (pInfo == NULL) {
                        continue;
                }
                if (!bFirst) { oss << ','; }
                bFirst = false;
                AppendSkillJson(oss, pInfo);
        }
        pLib->Leave();

        oss << "]}";

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// POST /api/skills
// 必須: typeMain, typeSub（派生クラス決定のため）
// 任意: 基底フィールド + 派生フィールド
// 成功時: 201 Created + 作成したスキルの JSON
// ---------------------------------------------------------------------------

CSkillCreateHandler::CSkillCreateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CSkillCreateHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuthAndBackend(request, m_pMgrData, response)) {
                return;
        }

        CLibInfoSkill *pLib = m_pMgrData->GetLibInfoSkill();
        CUraraSockTCPSBO *pSock = m_pMgrData->GetSock();
        if (pLib == NULL || pSock == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nTypeMain = 0;
        int nTypeSub = 0;
        if (!JsonUtils::TryGetInt(request.body, "typeMain", nTypeMain) ||
            nTypeMain < 0 || nTypeMain >= SKILLTYPEMAIN_MAX) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"invalid_typeMain\"}");
                return;
        }
        JsonUtils::TryGetInt(request.body, "typeSub", nTypeSub);

        pLib->Enter();

        CInfoSkillBase *pInfo =
                static_cast<CInfoSkillBase *>(pLib->GetNew(nTypeMain, nTypeSub));
        if (pInfo == NULL) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 500 Internal Server Error";
                response.SetJsonBody("{\"error\":\"failed_to_create\"}");
                return;
        }

        pInfo->m_nTypeMain = nTypeMain;
        pInfo->m_nTypeSub  = nTypeSub;

        std::string errReason;
        if (!ApplyJsonToSkillBase(request.body, pInfo, errReason)) {
                delete pInfo;
                pLib->Leave();
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody(std::string("{\"error\":\"") + errReason + "\"}");
                return;
        }

        // 派生フィールド適用
        if (pInfo->m_nClassType == INFOSKILLTYPE_MOVEATACK) {
                ApplyJsonToSkillMOVEATACK(request.body,
                        static_cast<CInfoSkillMOVEATACK *>(pInfo));
        } else if (pInfo->m_nClassType == INFOSKILLTYPE_HEAL) {
                ApplyJsonToSkillHEAL(request.body,
                        static_cast<CInfoSkillHEAL *>(pInfo));
        }

        // Add 時に dwSkillID == 0 なら新規 ID が採番される
        pInfo->m_dwSkillID = 0;
        pLib->Add(pInfo);

        // クライアント（ゲーム側）へ変更を通知
        CPacketSKILL_SKILLINFO packet;
        packet.Make(pInfo);
        pSock->SendTo(0, &packet);

        std::ostringstream oss;
        AppendSkillJson(oss, pInfo);

        pLib->Leave();

        response.statusLine = "HTTP/1.1 201 Created";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// PUT /api/skills
// 必須: skillId
// 任意: 基底フィールド + 派生フィールド
// ---------------------------------------------------------------------------

CSkillUpdateHandler::CSkillUpdateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CSkillUpdateHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuthAndBackend(request, m_pMgrData, response)) {
                return;
        }

        CLibInfoSkill *pLib = m_pMgrData->GetLibInfoSkill();
        CUraraSockTCPSBO *pSock = m_pMgrData->GetSock();
        if (pLib == NULL || pSock == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nSkillId = 0;
        if (!JsonUtils::TryGetInt(request.body, "skillId", nSkillId) || nSkillId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_skillId\"}");
                return;
        }

        pLib->Enter();

        CInfoSkillBase *pInfo =
                static_cast<CInfoSkillBase *>(pLib->GetPtr(static_cast<DWORD>(nSkillId)));
        if (pInfo == NULL) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        std::string errReason;
        if (!ApplyJsonToSkillBase(request.body, pInfo, errReason)) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody(std::string("{\"error\":\"") + errReason + "\"}");
                return;
        }

        // 派生フィールド適用
        if (pInfo->m_nClassType == INFOSKILLTYPE_MOVEATACK) {
                ApplyJsonToSkillMOVEATACK(request.body,
                        static_cast<CInfoSkillMOVEATACK *>(pInfo));
        } else if (pInfo->m_nClassType == INFOSKILLTYPE_HEAL) {
                ApplyJsonToSkillHEAL(request.body,
                        static_cast<CInfoSkillHEAL *>(pInfo));
        }

        // クライアントへ変更を通知
        CPacketSKILL_SKILLINFO packet;
        packet.Make(pInfo);
        pSock->SendTo(0, &packet);

        std::ostringstream oss;
        AppendSkillJson(oss, pInfo);

        pLib->Leave();

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// DELETE /api/skills
// 必須: skillId
// ---------------------------------------------------------------------------

CSkillDeleteHandler::CSkillDeleteHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CSkillDeleteHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuthAndBackend(request, m_pMgrData, response)) {
                return;
        }

        CLibInfoSkill *pLib = m_pMgrData->GetLibInfoSkill();
        if (pLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nSkillId = 0;
        if (!JsonUtils::TryGetInt(request.body, "skillId", nSkillId) || nSkillId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_skillId\"}");
                return;
        }

        DWORD dwSkillId = static_cast<DWORD>(nSkillId);

        pLib->Enter();

        CInfoSkillBase *pInfo =
                static_cast<CInfoSkillBase *>(pLib->GetPtr(dwSkillId));
        if (pInfo == NULL) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        pLib->Delete(dwSkillId);
        pLib->Leave();

        std::ostringstream oss;
        oss << "{\"deleted\":" << dwSkillId << "}";

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}
