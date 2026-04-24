#include "stdafx.h"
#include "ItemTypeHandler.h"

#include <sstream>
#include <string>
#include <vector>

#include "Web/AuthProvider.h"
#include "Web/JsonUtils.h"
#include "MgrData.h"
#include "LibInfoItemType.h"
#include "InfoItemTypeBase.h"
#include "UraraSockTCPSBO.h"
#include "PacketITEM_RES_ITEMTYPEINFO.h"
#include "PacketITEM_DELETEITEMTYPEINFO.h"
// LibInfoCharSvr.h は CInfoMapEventBase の前方宣言を持たないため
// InfoMapEventBase.h を先に include する必要がある
#include "InfoMapEventBase.h"
#include "LibInfo/LibInfoCharSvr.h"

// ---------------------------------------------------------------------------
// 内部ヘルパー（ファイル内 static 共通化）
// ---------------------------------------------------------------------------

namespace
{

// UTF-8 → CmyString（内部文字コード）
bool SetItemNameFromUtf8(CmyString &out, const std::string &utf8)
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
std::string ItemNameToUtf8(const CmyString &value)
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
        // 内部が CP932 の場合は UTF-8 へ変換する
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

// アイテム種別 ID → 表示用種別名（UI 側の種別プルダウンに使える簡易英字キー）
const char *GetItemTypeKey(DWORD dwItemTypeID)
{
        switch (dwItemTypeID) {
        case ITEMTYPEID_NONE:   return "none";
        case ITEMTYPEID_CLOTH:  return "cloth";
        case ITEMTYPEID_ACCE:   return "acce";
        case ITEMTYPEID_ARMS:   return "arms";
        case ITEMTYPEID_SHIELD: return "shield";
        case ITEMTYPEID_HP:     return "hp";
        case ITEMTYPEID_LIGHT:  return "light";
        }
        return "unknown";
}

// InfoItemTypeBase → JSON 出力
void AppendItemTypeJson(std::ostringstream &oss, const CInfoItemTypeBase *pInfo)
{
        oss << '{';
        oss << "\"typeId\":"       << pInfo->m_dwTypeID       << ',';
        oss << "\"itemTypeId\":"   << pInfo->m_dwItemTypeID   << ',';
        oss << "\"itemTypeKey\":\"" << GetItemTypeKey(pInfo->m_dwItemTypeID) << "\",";
        oss << "\"name\":\""       << JsonUtils::Escape(ItemNameToUtf8(pInfo->m_strName)) << "\",";
        oss << "\"delAverage\":"   << static_cast<unsigned int>(pInfo->m_byDelAverage) << ',';
        oss << "\"target\":"       << static_cast<unsigned int>(pInfo->m_byTarget)     << ',';
        oss << "\"area\":"         << static_cast<unsigned int>(pInfo->m_byArea)       << ',';
        oss << "\"putOn\":"        << (pInfo->m_bPutOn ? "true" : "false")             << ',';
        oss << "\"grpIdMain\":"    << static_cast<unsigned int>(pInfo->m_wGrpIDMain)   << ',';
        oss << "\"grpIdSub\":"     << static_cast<unsigned int>(pInfo->m_wGrpIDSub)    << ',';
        oss << "\"grpId\":"        << pInfo->m_dwGrpID        << ',';
        oss << "\"iconGrpId\":"    << pInfo->m_dwIconGrpID    << ',';
        oss << "\"dropSoundId\":"  << pInfo->m_dwDropSoundID  << ',';
        oss << "\"useEffectId\":"  << pInfo->m_dwUseEffectID  << ',';
        oss << "\"useSoundId\":"   << pInfo->m_dwUseSoundID   << ',';
        oss << "\"weaponInfoId\":" << pInfo->m_dwWeaponInfoID << ',';
        oss << "\"value\":"        << pInfo->m_dwValue        << ',';
        oss << "\"value2\":"       << pInfo->m_dwValue2       << ',';
        oss << "\"moveWait\":"     << pInfo->m_dwMoveWait     << ',';
        oss << "\"moveCount\":"    << pInfo->m_dwMoveCount;
        oss << '}';
}

// JSON から InfoItemTypeBase の各フィールドを上書きする
// （dwTypeID / dwItemTypeID は呼び出し側で明示的に扱う）
bool ApplyJsonToItemType(const std::string &json, CInfoItemTypeBase *pInfo, std::string &outErr)
{
        outErr.clear();

        std::string name;
        if (JsonUtils::TryGetString(json, "name", name)) {
                if (!SetItemNameFromUtf8(pInfo->m_strName, name)) {
                        outErr = "invalid_name_encoding";
                        return false;
                }
        }

        int nVal = 0;
        bool bVal = false;

        if (JsonUtils::TryGetInt(json, "delAverage", nVal) && nVal >= 0 && nVal <= 255) {
                pInfo->m_byDelAverage = static_cast<BYTE>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "target", nVal) && nVal >= 0 && nVal <= 255) {
                pInfo->m_byTarget = static_cast<BYTE>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "area", nVal) && nVal >= 0 && nVal <= 255) {
                pInfo->m_byArea = static_cast<BYTE>(nVal);
        }
        if (JsonUtils::TryGetBool(json, "putOn", bVal)) {
                pInfo->m_bPutOn = bVal ? TRUE : FALSE;
        } else if (JsonUtils::TryGetInt(json, "putOn", nVal)) {
                pInfo->m_bPutOn = (nVal != 0) ? TRUE : FALSE;
        }
        if (JsonUtils::TryGetInt(json, "grpIdMain", nVal) && nVal >= 0) {
                pInfo->m_wGrpIDMain = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "grpIdSub", nVal) && nVal >= 0) {
                pInfo->m_wGrpIDSub = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "grpId", nVal) && nVal >= 0) {
                pInfo->m_dwGrpID = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "iconGrpId", nVal) && nVal >= 0) {
                pInfo->m_dwIconGrpID = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "dropSoundId", nVal) && nVal >= 0) {
                pInfo->m_dwDropSoundID = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "useEffectId", nVal) && nVal >= 0) {
                pInfo->m_dwUseEffectID = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "useSoundId", nVal) && nVal >= 0) {
                pInfo->m_dwUseSoundID = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "weaponInfoId", nVal) && nVal >= 0) {
                pInfo->m_dwWeaponInfoID = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "value", nVal) && nVal >= 0) {
                pInfo->m_dwValue = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "value2", nVal) && nVal >= 0) {
                pInfo->m_dwValue2 = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "moveWait", nVal) && nVal >= 0) {
                pInfo->m_dwMoveWait = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "moveCount", nVal) && nVal >= 0) {
                pInfo->m_dwMoveCount = static_cast<DWORD>(nVal);
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
// GET /api/item-types
// ---------------------------------------------------------------------------

CItemTypeListHandler::CItemTypeListHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CItemTypeListHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuthAndBackend(request, m_pMgrData, response)) {
                return;
        }

        CLibInfoItemType *pLib = m_pMgrData->GetLibInfoItemType();
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
                const CInfoItemTypeBase *pInfo =
                        static_cast<const CInfoItemTypeBase *>(pLib->GetPtr(i));
                if (pInfo == NULL) {
                        continue;
                }
                if (!bFirst) { oss << ','; }
                bFirst = false;
                AppendItemTypeJson(oss, pInfo);
        }
        pLib->Leave();

        oss << "]}";

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// POST /api/item-types
// 必須: itemTypeId（0..ITEMTYPEID_MAX-1）
// 任意: 上記 ApplyJsonToItemType で扱うフィールド
// 成功時: 201 Created + 作成したアイテム種別の JSON
// ---------------------------------------------------------------------------

CItemTypeCreateHandler::CItemTypeCreateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CItemTypeCreateHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuthAndBackend(request, m_pMgrData, response)) {
                return;
        }

        CLibInfoItemType *pLib = m_pMgrData->GetLibInfoItemType();
        CUraraSockTCPSBO *pSock = m_pMgrData->GetSock();
        if (pLib == NULL || pSock == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nItemTypeId = 0;
        if (!JsonUtils::TryGetInt(request.body, "itemTypeId", nItemTypeId) ||
            nItemTypeId < 0 || nItemTypeId >= ITEMTYPEID_MAX) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"invalid_itemTypeId\"}");
                return;
        }

        pLib->Enter();

        CInfoItemTypeBase *pInfo =
                static_cast<CInfoItemTypeBase *>(pLib->GetNew(static_cast<DWORD>(nItemTypeId)));
        if (pInfo == NULL) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 500 Internal Server Error";
                response.SetJsonBody("{\"error\":\"failed_to_create\"}");
                return;
        }

        std::string errReason;
        if (!ApplyJsonToItemType(request.body, pInfo, errReason)) {
                delete pInfo;
                pLib->Leave();
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody(std::string("{\"error\":\"") + errReason + "\"}");
                return;
        }

        // Add 時に dwTypeID == 0 なら新規 ID が採番される
        pInfo->m_dwTypeID = 0;
        pLib->Add(pInfo);

        // クライアント（ゲーム側）へ変更を通知
        CPacketITEM_RES_ITEMTYPEINFO packet;
        packet.Make(pInfo);
        pSock->SendTo(0, &packet);

        std::ostringstream oss;
        AppendItemTypeJson(oss, pInfo);

        pLib->Leave();

        response.statusLine = "HTTP/1.1 201 Created";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// PUT /api/item-types
// 必須: typeId
// 任意: 各フィールド
// ---------------------------------------------------------------------------

CItemTypeUpdateHandler::CItemTypeUpdateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CItemTypeUpdateHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuthAndBackend(request, m_pMgrData, response)) {
                return;
        }

        CLibInfoItemType *pLib = m_pMgrData->GetLibInfoItemType();
        CUraraSockTCPSBO *pSock = m_pMgrData->GetSock();
        if (pLib == NULL || pSock == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nTypeId = 0;
        if (!JsonUtils::TryGetInt(request.body, "typeId", nTypeId) || nTypeId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_typeId\"}");
                return;
        }

        pLib->Enter();

        CInfoItemTypeBase *pInfo =
                static_cast<CInfoItemTypeBase *>(pLib->GetPtr(static_cast<DWORD>(nTypeId)));
        if (pInfo == NULL) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        // itemTypeId の変更（種別切替）にも任意で対応
        int nItemTypeId = 0;
        if (JsonUtils::TryGetInt(request.body, "itemTypeId", nItemTypeId) &&
            nItemTypeId >= 0 && nItemTypeId < ITEMTYPEID_MAX) {
                pInfo->m_dwItemTypeID = static_cast<DWORD>(nItemTypeId);
        }

        std::string errReason;
        if (!ApplyJsonToItemType(request.body, pInfo, errReason)) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody(std::string("{\"error\":\"") + errReason + "\"}");
                return;
        }

        // クライアントへ変更を通知
        CPacketITEM_RES_ITEMTYPEINFO packet;
        packet.Make(pInfo);
        pSock->SendTo(0, &packet);

        // キャラクターが持つアイテムの画像を種別変更に追従させる
        // （旧 RecvProcITEM_RENEWITEMTYPEINFO 相当）
        CLibInfoCharSvr *pCharLib = m_pMgrData->GetLibInfoChar();
        if (pCharLib != NULL) {
                pCharLib->RenewItemGrp(pInfo->m_dwTypeID);
        }

        std::ostringstream oss;
        AppendItemTypeJson(oss, pInfo);

        pLib->Leave();

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// DELETE /api/item-types
// 必須: typeId
// ---------------------------------------------------------------------------

CItemTypeDeleteHandler::CItemTypeDeleteHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CItemTypeDeleteHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuthAndBackend(request, m_pMgrData, response)) {
                return;
        }

        CLibInfoItemType *pLib = m_pMgrData->GetLibInfoItemType();
        CUraraSockTCPSBO *pSock = m_pMgrData->GetSock();
        if (pLib == NULL || pSock == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nTypeId = 0;
        if (!JsonUtils::TryGetInt(request.body, "typeId", nTypeId) || nTypeId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_typeId\"}");
                return;
        }

        DWORD dwTypeId = static_cast<DWORD>(nTypeId);

        pLib->Enter();

        CInfoItemTypeBase *pInfo =
                static_cast<CInfoItemTypeBase *>(pLib->GetPtr(dwTypeId));
        if (pInfo == NULL) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        // 削除通知をクライアントへ送信（削除より先に送るのが旧サーバー実装）
        CPacketITEM_DELETEITEMTYPEINFO packet;
        packet.Make(dwTypeId);
        pSock->SendTo(0, &packet);

        pLib->Delete(dwTypeId);
        pLib->Leave();

        std::ostringstream oss;
        oss << "{\"deleted\":" << dwTypeId << "}";

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}
