#include "StdAfx.h"
#include "InitialStatusHandler.h"

#include <sstream>
#include <string>

#include "Web/AuthProvider.h"
#include "Web/JsonUtils.h"
#include "MgrData.h"
#include "LibInfoSystem.h"
#include "InfoSystem.h"
#include "UraraSockTCPSBO.h"
#include "Packet/SYSTEM/PacketSYSTEM_INFO.h"

// ---------------------------------------------------------------------------
// 内部ヘルパー
// ---------------------------------------------------------------------------

namespace
{

// STSYSTEM_INITCHARSTATUS → JSON
void AppendInitialStatusJson(std::ostringstream &oss, const STSYSTEM_INITCHARSTATUS *p)
{
        oss << '{';
        oss << "\"mapId\":"              << p->dwInitPosMapID  << ',';
        oss << "\"posX\":"               << p->ptInitPos.x     << ',';
        oss << "\"posY\":"               << p->ptInitPos.y     << ',';
        oss << "\"maxHp\":"              << p->dwMaxHP         << ',';
        oss << "\"maxSp\":"              << p->dwMaxSP         << ',';
        oss << "\"stamina\":"            << p->wStamina        << ',';
        oss << "\"power\":"              << p->wPower          << ',';
        oss << "\"strength\":"           << p->wStrength       << ',';
        oss << "\"magic\":"              << p->wMagic          << ',';
        oss << "\"skillful\":"           << p->wSkillful       << ',';
        oss << "\"abilityAt\":"          << p->wAbillityAT     << ',';
        oss << "\"abilityDf\":"          << p->wAbillityDF     << ',';
        oss << "\"pAtack\":"             << p->wPAtack         << ',';
        oss << "\"pDefense\":"           << p->wPDefense       << ',';
        oss << "\"pMagic\":"             << p->wPMagic         << ',';
        oss << "\"pMagicDefense\":"      << p->wPMagicDefense  << ',';
        oss << "\"pHitAverage\":"        << p->wPHitAverage    << ',';
        oss << "\"pAvoidAverage\":"      << p->wPAvoidAverage  << ',';
        oss << "\"pCriticalAverage\":"   << p->wPCriticalAverage << ',';
        oss << "\"attrFire\":"           << p->wAttrFire       << ',';
        oss << "\"attrWind\":"           << p->wAttrWind       << ',';
        oss << "\"attrWater\":"          << p->wAttrWater      << ',';
        oss << "\"attrEarth\":"          << p->wAttrEarth      << ',';
        oss << "\"attrLight\":"          << p->wAttrLight      << ',';
        oss << "\"attrDark\":"           << p->wAttrDark;
        oss << '}';
}

// 指定キーが存在すれば WORD 値に反映（負値や範囲外は無視）
void ApplyWord(const std::string &json, const char *key, WORD &target)
{
        int v = 0;
        if (JsonUtils::TryGetInt(json, key, v)) {
                if (v < 0) { return; }
                if (v > 0xFFFF) { v = 0xFFFF; }
                target = static_cast<WORD>(v);
        }
}

// 指定キーが存在すれば DWORD 値に反映（負値は無視）
void ApplyDword(const std::string &json, const char *key, DWORD &target)
{
        int v = 0;
        if (JsonUtils::TryGetInt(json, key, v)) {
                if (v < 0) { return; }
                target = static_cast<DWORD>(v);
        }
}

// JSON → STSYSTEM_INITCHARSTATUS へ値を上書き
void ApplyJsonToInitialStatus(const std::string &json, STSYSTEM_INITCHARSTATUS *p)
{
        ApplyDword(json, "mapId",  p->dwInitPosMapID);
        ApplyDword(json, "maxHp",  p->dwMaxHP);
        ApplyDword(json, "maxSp",  p->dwMaxSP);

        int v = 0;
        if (JsonUtils::TryGetInt(json, "posX", v)) { p->ptInitPos.x = v; }
        if (JsonUtils::TryGetInt(json, "posY", v)) { p->ptInitPos.y = v; }

        ApplyWord(json, "stamina",          p->wStamina);
        ApplyWord(json, "power",            p->wPower);
        ApplyWord(json, "strength",         p->wStrength);
        ApplyWord(json, "magic",            p->wMagic);
        ApplyWord(json, "skillful",         p->wSkillful);
        ApplyWord(json, "abilityAt",        p->wAbillityAT);
        ApplyWord(json, "abilityDf",        p->wAbillityDF);
        ApplyWord(json, "pAtack",           p->wPAtack);
        ApplyWord(json, "pDefense",         p->wPDefense);
        ApplyWord(json, "pMagic",           p->wPMagic);
        ApplyWord(json, "pMagicDefense",    p->wPMagicDefense);
        ApplyWord(json, "pHitAverage",      p->wPHitAverage);
        ApplyWord(json, "pAvoidAverage",    p->wPAvoidAverage);
        ApplyWord(json, "pCriticalAverage", p->wPCriticalAverage);
        ApplyWord(json, "attrFire",         p->wAttrFire);
        ApplyWord(json, "attrWind",         p->wAttrWind);
        ApplyWord(json, "attrWater",        p->wAttrWater);
        ApplyWord(json, "attrEarth",        p->wAttrEarth);
        ApplyWord(json, "attrLight",        p->wAttrLight);
        ApplyWord(json, "attrDark",         p->wAttrDark);
}

// 共通: 認証 & バックエンド準備確認
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
// GET /api/initial-status
// ---------------------------------------------------------------------------

CInitialStatusGetHandler::CInitialStatusGetHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CInitialStatusGetHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuthAndBackend(request, m_pMgrData, response)) {
                return;
        }

        CLibInfoSystem *pLib = m_pMgrData->GetLibInfoSystem();
        if (pLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        pLib->Enter();
        PCInfoSystem pInfo = static_cast<PCInfoSystem>(pLib->GetPtr());
        if (pInfo == NULL || pInfo->m_pInitCharStatus == NULL) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 500 Internal Server Error";
                response.SetJsonBody("{\"error\":\"system_info_unavailable\"}");
                return;
        }

        std::ostringstream oss;
        AppendInitialStatusJson(oss, pInfo->m_pInitCharStatus);
        pLib->Leave();

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// PUT /api/initial-status
// 任意: 各フィールド（未指定キーは据え置き）
// ---------------------------------------------------------------------------

CInitialStatusUpdateHandler::CInitialStatusUpdateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CInitialStatusUpdateHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuthAndBackend(request, m_pMgrData, response)) {
                return;
        }

        CLibInfoSystem *pLib = m_pMgrData->GetLibInfoSystem();
        CUraraSockTCPSBO *pSock = m_pMgrData->GetSock();
        if (pLib == NULL || pSock == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        pLib->Enter();

        PCInfoSystem pInfo = static_cast<PCInfoSystem>(pLib->GetPtr());
        if (pInfo == NULL || pInfo->m_pInitCharStatus == NULL) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 500 Internal Server Error";
                response.SetJsonBody("{\"error\":\"system_info_unavailable\"}");
                return;
        }

        ApplyJsonToInitialStatus(request.body, pInfo->m_pInitCharStatus);

        // ネイティブ版 RecvProcADMIN_SYSTEM_RENEWINFO と同様に全クライアントへ配信
        CPacketSYSTEM_INFO Packet;
        Packet.Make(pInfo);
        pSock->SendTo(0, &Packet);

        std::ostringstream oss;
        AppendInitialStatusJson(oss, pInfo->m_pInitCharStatus);

        pLib->Leave();

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}
