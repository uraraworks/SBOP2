#include "StdAfx.h"
#include "MotionHandler.h"

#include <sstream>
#include <string>
#include <vector>

#include "Web/AuthProvider.h"
#include "Web/JsonUtils.h"
#include "MgrData.h"
#include "UraraSockTCPSBO.h"
#include "LibInfoMotionType.h"
#include "LibInfoMotion.h"
#include "InfoMotionType.h"
#include "InfoMotion.h"
#include "PacketCHAR_MOTIONTYPE.h"
#include "PacketCHAR_MOTION.h"

// ---------------------------------------------------------------------------
// 内部ヘルパー
// ---------------------------------------------------------------------------

namespace
{

// UTF-8 → CmyString（内部文字コード）
bool SetMotionNameFromUtf8(CmyString &out, const std::string &utf8)
{
        if (utf8.empty()) {
                out = _T("");
                return true;
        }
#ifdef _UNICODE
        int wideLen = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), static_cast<int>(utf8.size()), NULL, 0);
        if (wideLen <= 0) { return false; }
        std::vector<wchar_t> wide(static_cast<size_t>(wideLen) + 1, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), static_cast<int>(utf8.size()), &wide[0], wideLen);
        out = &wide[0];
#else
        int wideLen = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), static_cast<int>(utf8.size()), NULL, 0);
        if (wideLen <= 0) { return false; }
        std::vector<wchar_t> wide(static_cast<size_t>(wideLen) + 1, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), static_cast<int>(utf8.size()), &wide[0], wideLen);
        int mbLen = WideCharToMultiByte(932, 0, &wide[0], wideLen, NULL, 0, NULL, NULL);
        if (mbLen <= 0) { return false; }
        std::vector<char> mb(static_cast<size_t>(mbLen) + 1, '\0');
        WideCharToMultiByte(932, 0, &wide[0], wideLen, &mb[0], mbLen, NULL, NULL);
        out = &mb[0];
#endif
        return true;
}

// CmyString → UTF-8
std::string MotionNameToUtf8(const CmyString &value)
{
#ifdef _UNICODE
        LPCWSTR pszWide = static_cast<LPCWSTR>(value);
        if (pszWide == NULL || pszWide[0] == L'\0') { return std::string(); }
        int utf8Len = WideCharToMultiByte(CP_UTF8, 0, pszWide, -1, NULL, 0, NULL, NULL);
        if (utf8Len <= 0) { return std::string(); }
        std::string utf8(static_cast<size_t>(utf8Len), '\0');
        WideCharToMultiByte(CP_UTF8, 0, pszWide, -1, &utf8[0], utf8Len, NULL, NULL);
        if (!utf8.empty() && utf8.back() == '\0') { utf8.resize(utf8.size() - 1); }
        return utf8;
#else
        LPCSTR psz = static_cast<LPCSTR>(value);
        if (psz == NULL || psz[0] == '\0') { return std::string(); }
        int wideLen = MultiByteToWideChar(932, 0, psz, -1, NULL, 0);
        if (wideLen <= 0) { return std::string(psz); }
        std::vector<wchar_t> wide(static_cast<size_t>(wideLen), L'\0');
        MultiByteToWideChar(932, 0, psz, -1, &wide[0], wideLen);
        int utf8Len = WideCharToMultiByte(CP_UTF8, 0, &wide[0], -1, NULL, 0, NULL, NULL);
        if (utf8Len <= 0) { return std::string(); }
        std::string utf8(static_cast<size_t>(utf8Len), '\0');
        WideCharToMultiByte(CP_UTF8, 0, &wide[0], -1, &utf8[0], utf8Len, NULL, NULL);
        if (!utf8.empty() && utf8.back() == '\0') { utf8.resize(utf8.size() - 1); }
        return utf8;
#endif
}

// CInfoMotionType → JSON
void AppendMotionTypeJson(std::ostringstream &oss, const CInfoMotionType *pInfo)
{
        oss << '{';
        oss << "\"motionTypeId\":" << pInfo->m_dwMotionTypeID << ',';
        oss << "\"name\":\""       << JsonUtils::Escape(MotionNameToUtf8(pInfo->m_strName)) << "\",";
        oss << "\"grpIdSub\":"     << pInfo->m_wGrpIDSub;
        oss << '}';
}

// CInfoMotion → JSON（drawList は配列として出力）
void AppendMotionJson(std::ostringstream &oss, const CInfoMotion *pInfo)
{
        oss << '{';
        oss << "\"motionId\":"       << pInfo->m_dwMotionID        << ',';
        oss << "\"motionTypeId\":"   << pInfo->m_dwMotionTypeID    << ',';
        oss << "\"motionListId\":"   << pInfo->m_dwMotionListID    << ',';
        oss << "\"wait\":"           << static_cast<int>(pInfo->m_byWait)   << ',';
        oss << "\"level1\":"         << static_cast<int>(pInfo->m_byLevel1) << ',';
        oss << "\"level2\":"         << static_cast<int>(pInfo->m_byLevel2) << ',';
        oss << "\"level3\":"         << static_cast<int>(pInfo->m_byLevel3) << ',';
        oss << "\"pile\":"           << (pInfo->m_bPile       ? "true" : "false") << ',';
        oss << "\"redrawHand\":"     << (pInfo->m_bRedrawHand ? "true" : "false") << ',';
        oss << "\"loop\":"           << (pInfo->m_bLoop       ? "true" : "false") << ',';
        oss << "\"grpIdMainBase\":"  << pInfo->m_wGrpIDMainBase  << ',';
        oss << "\"grpIdMainPile1\":" << pInfo->m_wGrpIDMainPile1 << ',';
        oss << "\"grpIdMainPile2\":" << pInfo->m_wGrpIDMainPile2 << ',';
        oss << "\"grpIdMainPile3\":" << pInfo->m_wGrpIDMainPile3 << ',';
        oss << "\"grpIdSubBase\":"   << pInfo->m_wGrpIDSubBase   << ',';
        oss << "\"grpIdSubPile1\":"  << pInfo->m_wGrpIDSubPile1  << ',';
        oss << "\"grpIdSubPile2\":"  << pInfo->m_wGrpIDSubPile2  << ',';
        oss << "\"grpIdSubPile3\":"  << pInfo->m_wGrpIDSubPile3  << ',';
        oss << "\"soundId\":"        << pInfo->m_dwSoundID       << ',';
        oss << "\"procId\":"         << pInfo->m_dwProcID        << ',';
        oss << "\"drawPosPile0x\":"  << pInfo->m_ptDrawPosPile0.x << ',';
        oss << "\"drawPosPile0y\":"  << pInfo->m_ptDrawPosPile0.y << ',';
        oss << "\"drawPosPile1x\":"  << pInfo->m_ptDrawPosPile1.x << ',';
        oss << "\"drawPosPile1y\":"  << pInfo->m_ptDrawPosPile1.y << ',';
        oss << "\"drawPosPile2x\":"  << pInfo->m_ptDrawPosPile2.x << ',';
        oss << "\"drawPosPile2y\":"  << pInfo->m_ptDrawPosPile2.y << ',';
        oss << "\"drawPosPile3x\":"  << pInfo->m_ptDrawPosPile3.x << ',';
        oss << "\"drawPosPile3y\":"  << pInfo->m_ptDrawPosPile3.y << ',';

        // 描画順リスト
        oss << "\"drawList\":[";
        for (size_t i = 0; i < pInfo->m_anDrawList.size(); ++i) {
                if (i > 0) { oss << ','; }
                oss << pInfo->m_anDrawList[i];
        }
        oss << ']';
        oss << '}';
}

// JSON から CInfoMotion に値を適用する
void ApplyJsonToMotion(const std::string &json, CInfoMotion *pInfo)
{
        int nVal = 0;
        bool bVal = false;

        if (JsonUtils::TryGetInt(json, "wait", nVal) && nVal >= 0 && nVal <= 255) {
                pInfo->m_byWait = static_cast<BYTE>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "level1", nVal) && nVal >= 0 && nVal <= 255) {
                pInfo->m_byLevel1 = static_cast<BYTE>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "level2", nVal) && nVal >= 0 && nVal <= 255) {
                pInfo->m_byLevel2 = static_cast<BYTE>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "level3", nVal) && nVal >= 0 && nVal <= 255) {
                pInfo->m_byLevel3 = static_cast<BYTE>(nVal);
        }

        if (JsonUtils::TryGetBool(json, "pile", bVal)) {
                pInfo->m_bPile = bVal ? TRUE : FALSE;
        } else if (JsonUtils::TryGetInt(json, "pile", nVal)) {
                pInfo->m_bPile = (nVal != 0) ? TRUE : FALSE;
        }
        if (JsonUtils::TryGetBool(json, "redrawHand", bVal)) {
                pInfo->m_bRedrawHand = bVal ? TRUE : FALSE;
        } else if (JsonUtils::TryGetInt(json, "redrawHand", nVal)) {
                pInfo->m_bRedrawHand = (nVal != 0) ? TRUE : FALSE;
        }
        if (JsonUtils::TryGetBool(json, "loop", bVal)) {
                pInfo->m_bLoop = bVal ? TRUE : FALSE;
        } else if (JsonUtils::TryGetInt(json, "loop", nVal)) {
                pInfo->m_bLoop = (nVal != 0) ? TRUE : FALSE;
        }

        if (JsonUtils::TryGetInt(json, "grpIdMainBase",  nVal) && nVal >= 0) { pInfo->m_wGrpIDMainBase  = static_cast<WORD>(nVal); }
        if (JsonUtils::TryGetInt(json, "grpIdMainPile1", nVal) && nVal >= 0) { pInfo->m_wGrpIDMainPile1 = static_cast<WORD>(nVal); }
        if (JsonUtils::TryGetInt(json, "grpIdMainPile2", nVal) && nVal >= 0) { pInfo->m_wGrpIDMainPile2 = static_cast<WORD>(nVal); }
        if (JsonUtils::TryGetInt(json, "grpIdMainPile3", nVal) && nVal >= 0) { pInfo->m_wGrpIDMainPile3 = static_cast<WORD>(nVal); }
        if (JsonUtils::TryGetInt(json, "grpIdSubBase",   nVal) && nVal >= 0) { pInfo->m_wGrpIDSubBase   = static_cast<WORD>(nVal); }
        if (JsonUtils::TryGetInt(json, "grpIdSubPile1",  nVal) && nVal >= 0) { pInfo->m_wGrpIDSubPile1  = static_cast<WORD>(nVal); }
        if (JsonUtils::TryGetInt(json, "grpIdSubPile2",  nVal) && nVal >= 0) { pInfo->m_wGrpIDSubPile2  = static_cast<WORD>(nVal); }
        if (JsonUtils::TryGetInt(json, "grpIdSubPile3",  nVal) && nVal >= 0) { pInfo->m_wGrpIDSubPile3  = static_cast<WORD>(nVal); }

        if (JsonUtils::TryGetInt(json, "soundId", nVal) && nVal >= 0) { pInfo->m_dwSoundID = static_cast<DWORD>(nVal); }
        if (JsonUtils::TryGetInt(json, "procId",  nVal) && nVal >= 0) { pInfo->m_dwProcID  = static_cast<DWORD>(nVal); }

        if (JsonUtils::TryGetInt(json, "drawPosPile0x", nVal)) { pInfo->m_ptDrawPosPile0.x = nVal; }
        if (JsonUtils::TryGetInt(json, "drawPosPile0y", nVal)) { pInfo->m_ptDrawPosPile0.y = nVal; }
        if (JsonUtils::TryGetInt(json, "drawPosPile1x", nVal)) { pInfo->m_ptDrawPosPile1.x = nVal; }
        if (JsonUtils::TryGetInt(json, "drawPosPile1y", nVal)) { pInfo->m_ptDrawPosPile1.y = nVal; }
        if (JsonUtils::TryGetInt(json, "drawPosPile2x", nVal)) { pInfo->m_ptDrawPosPile2.x = nVal; }
        if (JsonUtils::TryGetInt(json, "drawPosPile2y", nVal)) { pInfo->m_ptDrawPosPile2.y = nVal; }
        if (JsonUtils::TryGetInt(json, "drawPosPile3x", nVal)) { pInfo->m_ptDrawPosPile3.x = nVal; }
        if (JsonUtils::TryGetInt(json, "drawPosPile3y", nVal)) { pInfo->m_ptDrawPosPile3.y = nVal; }
}

// 共通: 認証 & MgrData チェック
bool CheckAuth(const HttpRequest &request, CMgrData *pMgrData, HttpResponse &response)
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

// ===========================================================================
// モーション種別ハンドラ
// ===========================================================================

// ---------------------------------------------------------------------------
// GET /api/motion-types
// ---------------------------------------------------------------------------

CMotionTypeListHandler::CMotionTypeListHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMotionTypeListHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuth(request, m_pMgrData, response)) { return; }

        CLibInfoMotionType *pLib = m_pMgrData->GetLibInfoMotionType();
        if (pLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        std::ostringstream oss;
        oss << "{\"motionTypes\":[";

        pLib->Enter();
        int nCount = pLib->GetCount();
        bool bFirst = true;
        for (int i = 0; i < nCount; ++i) {
                const CInfoMotionType *pInfo =
                        static_cast<const CInfoMotionType *>(pLib->GetPtr(i));
                if (pInfo == NULL) { continue; }
                if (!bFirst) { oss << ','; }
                bFirst = false;
                AppendMotionTypeJson(oss, pInfo);
        }
        pLib->Leave();

        oss << "]}";
        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// POST /api/motion-types
// ---------------------------------------------------------------------------

CMotionTypeCreateHandler::CMotionTypeCreateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMotionTypeCreateHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuth(request, m_pMgrData, response)) { return; }

        CLibInfoMotionType *pLibType = m_pMgrData->GetLibInfoMotionType();
        CLibInfoMotion     *pLibMotion = m_pMgrData->GetLibInfoMotion();
        CUraraSockTCPSBO   *pSock = m_pMgrData->GetSock();
        if (pLibType == NULL || pLibMotion == NULL || pSock == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        pLibType->Enter();
        pLibMotion->Enter();

        // 新規種別を作成
        CInfoMotionType *pInfo =
                static_cast<CInfoMotionType *>(pLibType->GetNew());
        if (pInfo == NULL) {
                pLibMotion->Leave();
                pLibType->Leave();
                response.statusLine = "HTTP/1.1 500 Internal Server Error";
                response.SetJsonBody("{\"error\":\"failed_to_create\"}");
                return;
        }

        // 名前
        std::string name;
        if (JsonUtils::TryGetString(request.body, "name", name)) {
                if (!SetMotionNameFromUtf8(pInfo->m_strName, name)) {
                        delete pInfo;
                        pLibMotion->Leave();
                        pLibType->Leave();
                        response.statusLine = "HTTP/1.1 400 Bad Request";
                        response.SetJsonBody("{\"error\":\"invalid_name_encoding\"}");
                        return;
                }
        }
        int nVal = 0;
        if (JsonUtils::TryGetInt(request.body, "grpIdSub", nVal) && nVal >= 0) {
                pInfo->m_wGrpIDSub = static_cast<WORD>(nVal);
        }

        // 種別を追加（ID は GetNew 内で自動採番）
        pInfo->m_dwMotionTypeID = 0;
        pLibType->Add(pInfo);

        // 種別に対応するモーションフレームを全 listId 分追加
        pLibMotion->AddType(pInfo->m_dwMotionTypeID);

        // クライアントへ通知
        CPacketCHAR_MOTIONTYPE packetType;
        packetType.Make(pInfo->m_dwMotionTypeID, pLibType);
        pSock->SendTo(0, &packetType);

        CPacketCHAR_MOTION packetMotion;
        packetMotion.Make(pInfo->m_dwMotionTypeID, 0, pLibMotion);
        pSock->SendTo(0, &packetMotion);

        std::ostringstream oss;
        AppendMotionTypeJson(oss, pInfo);

        pLibMotion->Leave();
        pLibType->Leave();

        response.statusLine = "HTTP/1.1 201 Created";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// PUT /api/motion-types  （body.motionTypeId 必須）
// ---------------------------------------------------------------------------

CMotionTypeUpdateHandler::CMotionTypeUpdateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMotionTypeUpdateHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuth(request, m_pMgrData, response)) { return; }

        CLibInfoMotionType *pLib = m_pMgrData->GetLibInfoMotionType();
        CUraraSockTCPSBO   *pSock = m_pMgrData->GetSock();
        if (pLib == NULL || pSock == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nTypeId = 0;
        if (!JsonUtils::TryGetInt(request.body, "motionTypeId", nTypeId) || nTypeId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_motionTypeId\"}");
                return;
        }

        pLib->Enter();

        CInfoMotionType *pInfo =
                static_cast<CInfoMotionType *>(pLib->GetPtr(static_cast<DWORD>(nTypeId)));
        if (pInfo == NULL) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        std::string name;
        if (JsonUtils::TryGetString(request.body, "name", name)) {
                if (!SetMotionNameFromUtf8(pInfo->m_strName, name)) {
                        pLib->Leave();
                        response.statusLine = "HTTP/1.1 400 Bad Request";
                        response.SetJsonBody("{\"error\":\"invalid_name_encoding\"}");
                        return;
                }
        }
        int nVal = 0;
        if (JsonUtils::TryGetInt(request.body, "grpIdSub", nVal) && nVal >= 0) {
                pInfo->m_wGrpIDSub = static_cast<WORD>(nVal);
        }

        // クライアントへ通知
        CPacketCHAR_MOTIONTYPE packet;
        packet.Make(static_cast<DWORD>(nTypeId), pLib);
        pSock->SendTo(0, &packet);

        std::ostringstream oss;
        AppendMotionTypeJson(oss, pInfo);

        pLib->Leave();

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// DELETE /api/motion-types  （body.motionTypeId 必須）
// ---------------------------------------------------------------------------

CMotionTypeDeleteHandler::CMotionTypeDeleteHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMotionTypeDeleteHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuth(request, m_pMgrData, response)) { return; }

        CLibInfoMotionType *pLib = m_pMgrData->GetLibInfoMotionType();
        if (pLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nTypeId = 0;
        if (!JsonUtils::TryGetInt(request.body, "motionTypeId", nTypeId) || nTypeId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_motionTypeId\"}");
                return;
        }

        DWORD dwTypeId = static_cast<DWORD>(nTypeId);

        pLib->Enter();

        if (pLib->GetPtr(dwTypeId) == NULL) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        pLib->Delete(dwTypeId);
        pLib->Leave();

        std::ostringstream oss;
        oss << "{\"deleted\":" << dwTypeId << "}";
        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

// ===========================================================================
// モーション個別ハンドラ
// ===========================================================================

// ---------------------------------------------------------------------------
// GET /api/motions  [?motionTypeId=N]
// ---------------------------------------------------------------------------

CMotionListHandler::CMotionListHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMotionListHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuth(request, m_pMgrData, response)) { return; }

        CLibInfoMotion *pLib = m_pMgrData->GetLibInfoMotion();
        if (pLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        // クエリパラメータ motionTypeId を解析（path に含まれる "?..." 部分から取得）
        DWORD dwFilterTypeId = 0;
        {
                const std::string &fullPath = request.path;
                size_t qpos = fullPath.find('?');
                if (qpos != std::string::npos) {
                        std::string query = fullPath.substr(qpos + 1);
                        const std::string prefix = "motionTypeId=";
                        size_t pos = query.find(prefix);
                        if (pos != std::string::npos) {
                                pos += prefix.size();
                                size_t end = query.find('&', pos);
                                std::string val = (end == std::string::npos)
                                        ? query.substr(pos)
                                        : query.substr(pos, end - pos);
                                int n = std::atoi(val.c_str());
                                if (n > 0) { dwFilterTypeId = static_cast<DWORD>(n); }
                        }
                }
        }

        std::ostringstream oss;
        oss << "{\"motions\":[";

        pLib->Enter();
        int nCount = pLib->GetCount();
        bool bFirst = true;
        for (int i = 0; i < nCount; ++i) {
                const CInfoMotion *pInfo =
                        static_cast<const CInfoMotion *>(pLib->GetPtr(i));
                if (pInfo == NULL) { continue; }
                if (dwFilterTypeId != 0 && pInfo->m_dwMotionTypeID != dwFilterTypeId) { continue; }
                if (!bFirst) { oss << ','; }
                bFirst = false;
                AppendMotionJson(oss, pInfo);
        }
        pLib->Leave();

        oss << "]}";
        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// POST /api/motions  （motionTypeId, motionListId 必須）
// ---------------------------------------------------------------------------

CMotionCreateHandler::CMotionCreateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMotionCreateHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuth(request, m_pMgrData, response)) { return; }

        CLibInfoMotion   *pLib  = m_pMgrData->GetLibInfoMotion();
        CUraraSockTCPSBO *pSock = m_pMgrData->GetSock();
        if (pLib == NULL || pSock == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nTypeId = 0, nListId = 0;
        if (!JsonUtils::TryGetInt(request.body, "motionTypeId", nTypeId) || nTypeId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_motionTypeId\"}");
                return;
        }
        if (!JsonUtils::TryGetInt(request.body, "motionListId", nListId) || nListId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_motionListId\"}");
                return;
        }

        pLib->Enter();

        CInfoMotion *pInfo = static_cast<CInfoMotion *>(pLib->GetNew());
        if (pInfo == NULL) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 500 Internal Server Error";
                response.SetJsonBody("{\"error\":\"failed_to_create\"}");
                return;
        }

        pInfo->m_dwMotionTypeID = static_cast<DWORD>(nTypeId);
        pInfo->m_dwMotionListID = static_cast<DWORD>(nListId);
        pInfo->m_dwMotionID = 0; // Add で自動採番

        ApplyJsonToMotion(request.body, pInfo);
        pLib->Add(pInfo);

        // クライアントへ通知
        CPacketCHAR_MOTION packet;
        packet.Make(pInfo->m_dwMotionTypeID, pInfo->m_dwMotionListID, pLib);
        pSock->SendTo(0, &packet);

        std::ostringstream oss;
        AppendMotionJson(oss, pInfo);
        pLib->Leave();

        response.statusLine = "HTTP/1.1 201 Created";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// PUT /api/motions  （body.motionId 必須）
// ---------------------------------------------------------------------------

CMotionUpdateHandler::CMotionUpdateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMotionUpdateHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuth(request, m_pMgrData, response)) { return; }

        CLibInfoMotion   *pLib  = m_pMgrData->GetLibInfoMotion();
        CUraraSockTCPSBO *pSock = m_pMgrData->GetSock();
        if (pLib == NULL || pSock == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nMotionId = 0;
        if (!JsonUtils::TryGetInt(request.body, "motionId", nMotionId) || nMotionId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_motionId\"}");
                return;
        }

        pLib->Enter();

        // motionId で線形探索
        CInfoMotion *pInfo = NULL;
        int nCount = pLib->GetCount();
        for (int i = 0; i < nCount; ++i) {
                CInfoMotion *pTmp = static_cast<CInfoMotion *>(pLib->GetPtr(i));
                if (pTmp != NULL && pTmp->m_dwMotionID == static_cast<DWORD>(nMotionId)) {
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

        ApplyJsonToMotion(request.body, pInfo);

        // クライアントへ通知
        CPacketCHAR_MOTION packet;
        packet.Make(pInfo->m_dwMotionTypeID, pInfo->m_dwMotionListID, pLib);
        pSock->SendTo(0, &packet);

        std::ostringstream oss;
        AppendMotionJson(oss, pInfo);
        pLib->Leave();

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// DELETE /api/motions  （body.motionId 必須）
// ---------------------------------------------------------------------------

CMotionDeleteHandler::CMotionDeleteHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMotionDeleteHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuth(request, m_pMgrData, response)) { return; }

        CLibInfoMotion *pLib = m_pMgrData->GetLibInfoMotion();
        if (pLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nMotionId = 0;
        if (!JsonUtils::TryGetInt(request.body, "motionId", nMotionId) || nMotionId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_motionId\"}");
                return;
        }

        DWORD dwMotionId = static_cast<DWORD>(nMotionId);

        pLib->Enter();

        // 存在確認
        bool bFound = false;
        int nCount = pLib->GetCount();
        for (int i = 0; i < nCount; ++i) {
                CInfoMotion *pTmp = static_cast<CInfoMotion *>(pLib->GetPtr(i));
                if (pTmp != NULL && pTmp->m_dwMotionID == dwMotionId) {
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

        pLib->Delete(dwMotionId);
        pLib->Leave();

        std::ostringstream oss;
        oss << "{\"deleted\":" << dwMotionId << "}";
        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}
