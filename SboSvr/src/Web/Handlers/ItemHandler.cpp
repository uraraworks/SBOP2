#include "stdafx.h"
#include "ItemHandler.h"

#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

#include "Web/AuthProvider.h"
#include "Web/JsonUtils.h"
#include "MgrData.h"
#include "LibInfoItem.h"
#include "LibInfoItemType.h"
#include "InfoItem.h"
#include "InfoItemTypeBase.h"
#include "UraraSockTCPSBO.h"
#include "PacketITEM_RES_ITEMINFO.h"
#include "PacketITEM_DELETEITEMINFO.h"
#include "PacketCHAR_ITEMINFO.h"
// LibInfoCharSvr.h は CInfoMapEventBase の前方宣言を持たないため
// InfoMapEventBase.h を先に include する必要がある
#include "InfoMapEventBase.h"
#include "LibInfo/LibInfoCharSvr.h"
#include "Info/InfoCharSvr.h"

// ---------------------------------------------------------------------------
// 内部ヘルパー
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

// CInfoItem → JSON 出力
void AppendItemJson(std::ostringstream &oss, const CInfoItem *pInfo, CLibInfoItemType *pLibItemType)
{
        oss << '{';
        oss << "\"itemId\":"      << pInfo->m_dwItemID      << ',';
        oss << "\"itemTypeId\":"  << pInfo->m_dwItemTypeID  << ',';
        oss << "\"name\":\""      << JsonUtils::Escape(ItemNameToUtf8(pInfo->m_strName)) << "\",";
        oss << "\"grpId\":"       << pInfo->m_dwGrpID       << ',';
        oss << "\"iconGrpId\":"   << pInfo->m_dwIconGrpID   << ',';
        oss << "\"mapId\":"       << pInfo->m_dwMapID       << ',';
        oss << "\"charId\":"      << pInfo->m_dwCharID      << ',';
        oss << "\"dropSoundId\":" << pInfo->m_dwDropSoundID << ',';
        oss << "\"putOn\":"       << (pInfo->m_bPutOn ? "true" : "false") << ',';
        oss << "\"posX\":"        << pInfo->m_ptPos.x       << ',';
        oss << "\"posY\":"        << pInfo->m_ptPos.y       << ',';
        oss << "\"posZ\":"        << pInfo->m_nPosZ         << ',';
        oss << "\"backPackX\":"   << pInfo->m_ptBackPack.x  << ',';
        oss << "\"backPackY\":"   << pInfo->m_ptBackPack.y;

        // アイテム種別名（参考情報）
        if (pLibItemType != NULL) {
                const CInfoItemTypeBase *pType =
                        static_cast<const CInfoItemTypeBase *>(pLibItemType->GetPtr(pInfo->m_dwItemTypeID));
                if (pType != NULL) {
                        oss << ",\"itemTypeName\":\""
                            << JsonUtils::Escape(ItemNameToUtf8(pType->m_strName))
                            << "\"";
                }
        }
        oss << '}';
}

// JSON → CInfoItem のフィールドを上書き（m_dwItemID は呼び出し側が明示的に扱う）
bool ApplyJsonToItem(const std::string &json, CInfoItem *pInfo, std::string &outErr)
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

        if (JsonUtils::TryGetInt(json, "itemTypeId", nVal) && nVal >= 0) {
                pInfo->m_dwItemTypeID = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "grpId", nVal) && nVal >= 0) {
                pInfo->m_dwGrpID = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "iconGrpId", nVal) && nVal >= 0) {
                pInfo->m_dwIconGrpID = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "mapId", nVal) && nVal >= 0) {
                pInfo->m_dwMapID = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "charId", nVal) && nVal >= 0) {
                pInfo->m_dwCharID = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "dropSoundId", nVal) && nVal >= 0) {
                pInfo->m_dwDropSoundID = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetBool(json, "putOn", bVal)) {
                pInfo->m_bPutOn = bVal ? TRUE : FALSE;
        } else if (JsonUtils::TryGetInt(json, "putOn", nVal)) {
                pInfo->m_bPutOn = (nVal != 0) ? TRUE : FALSE;
        }
        if (JsonUtils::TryGetInt(json, "posX", nVal)) {
                pInfo->m_ptPos.x = nVal;
        }
        if (JsonUtils::TryGetInt(json, "posY", nVal)) {
                pInfo->m_ptPos.y = nVal;
        }
        if (JsonUtils::TryGetInt(json, "posZ", nVal)) {
                pInfo->m_nPosZ = nVal;
        }
        if (JsonUtils::TryGetInt(json, "backPackX", nVal)) {
                pInfo->m_ptBackPack.x = nVal;
        }
        if (JsonUtils::TryGetInt(json, "backPackY", nVal)) {
                pInfo->m_ptBackPack.y = nVal;
        }

        return true;
}

// アイテム種別 ID から CInfoItemTypeBase* を取得（Enter/Leave は呼び出し側でロック済み想定）
CInfoItemTypeBase *FindItemTypeByTypeId(CLibInfoItemType *pLib, DWORD dwTypeId)
{
        if (pLib == NULL) {
                return NULL;
        }
        return static_cast<CInfoItemTypeBase *>(pLib->GetPtr(dwTypeId));
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

// クエリ文字列から整数パラメータを取り出す。存在しなければ false
bool TryGetQueryInt(const std::string &path, const std::string &key, int &outVal)
{
        size_t nQ = path.find('?');
        if (nQ == std::string::npos) {
                return false;
        }
        std::string query = path.substr(nQ + 1);
        std::string needle = key + "=";
        size_t nPos = 0;
        while (nPos < query.size()) {
                size_t nAmp = query.find('&', nPos);
                std::string part = (nAmp == std::string::npos)
                        ? query.substr(nPos)
                        : query.substr(nPos, nAmp - nPos);
                if (part.compare(0, needle.size(), needle) == 0) {
                        std::string val = part.substr(needle.size());
                        if (val.empty()) {
                                return false;
                        }
                        char *pEnd = NULL;
                        long v = std::strtol(val.c_str(), &pEnd, 10);
                        if (pEnd == NULL || *pEnd != '\0') {
                                return false;
                        }
                        outVal = static_cast<int>(v);
                        return true;
                }
                if (nAmp == std::string::npos) {
                        break;
                }
                nPos = nAmp + 1;
        }
        return false;
}

// 所持キャラにアイテム一覧パケットを通知する（ログイン中の時のみ）
void NotifyCharItems(CMgrData *pMgrData, DWORD dwCharId)
{
        if (dwCharId == 0) {
                return;
        }
        CLibInfoCharSvr *pCharLib = pMgrData->GetLibInfoChar();
        CUraraSockTCPSBO *pSock   = pMgrData->GetSock();
        if (pCharLib == NULL || pSock == NULL) {
                return;
        }
        CInfoCharSvr *pInfoChar =
                static_cast<CInfoCharSvr *>(pCharLib->GetPtrLogIn(dwCharId));
        if (pInfoChar == NULL) {
                return;
        }
        CPacketCHAR_ITEMINFO packet;
        packet.Make(pInfoChar->m_dwCharID, &pInfoChar->m_adwItemID);
        pSock->SendTo(pInfoChar->m_dwSessionID, &packet);
}

} // namespace

// ---------------------------------------------------------------------------
// GET /api/items  [?mapId=N] [?charId=N] [?drop=1]
// ---------------------------------------------------------------------------

CItemListHandler::CItemListHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CItemListHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuthAndBackend(request, m_pMgrData, response)) {
                return;
        }

        CLibInfoItem *pLib = m_pMgrData->GetLibInfoItem();
        CLibInfoItemType *pLibType = m_pMgrData->GetLibInfoItemType();
        if (pLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nFilterMapId = -1;
        int nFilterCharId = -1;
        int nDropOnly = 0;
        bool bHasMapFilter = TryGetQueryInt(request.path, "mapId", nFilterMapId);
        bool bHasCharFilter = TryGetQueryInt(request.path, "charId", nFilterCharId);
        bool bDropFilter = TryGetQueryInt(request.path, "drop", nDropOnly) && (nDropOnly != 0);

        std::ostringstream oss;
        oss << "{\"items\":[";

        pLib->Enter();
        if (pLibType != NULL) {
                pLibType->Enter();
        }

        int nCount = pLib->GetCount();
        bool bFirst = true;
        for (int i = 0; i < nCount; ++i) {
                const CInfoItem *pInfo =
                        static_cast<const CInfoItem *>(pLib->GetPtr(i));
                if (pInfo == NULL) {
                        continue;
                }
                if (bHasMapFilter && static_cast<int>(pInfo->m_dwMapID) != nFilterMapId) {
                        continue;
                }
                if (bHasCharFilter && static_cast<int>(pInfo->m_dwCharID) != nFilterCharId) {
                        continue;
                }
                if (bDropFilter && pInfo->m_dwMapID == 0) {
                        continue;
                }
                if (!bFirst) { oss << ','; }
                bFirst = false;
                AppendItemJson(oss, pInfo, pLibType);
        }

        if (pLibType != NULL) {
                pLibType->Leave();
        }
        pLib->Leave();

        oss << "]}";

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// POST /api/items
// 必須: itemTypeId（アイテム種別情報ライブラリに存在すること）
// 任意: name / grpId / iconGrpId / mapId / charId / dropSoundId / putOn /
//       posX / posY / posZ / backPackX / backPackY
// 成功時: 201 Created + 作成したアイテムの JSON
// ---------------------------------------------------------------------------

CItemCreateHandler::CItemCreateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CItemCreateHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuthAndBackend(request, m_pMgrData, response)) {
                return;
        }

        CLibInfoItem *pLib = m_pMgrData->GetLibInfoItem();
        CLibInfoItemType *pLibType = m_pMgrData->GetLibInfoItemType();
        CUraraSockTCPSBO *pSock = m_pMgrData->GetSock();
        if (pLib == NULL || pLibType == NULL || pSock == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nItemTypeId = 0;
        if (!JsonUtils::TryGetInt(request.body, "itemTypeId", nItemTypeId) || nItemTypeId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_itemTypeId\"}");
                return;
        }

        // アイテム種別の実在チェック、及び初期値として種別の name/grpId/iconGrpId/putOn/dropSoundId を借用
        pLibType->Enter();
        const CInfoItemTypeBase *pType =
                FindItemTypeByTypeId(pLibType, static_cast<DWORD>(nItemTypeId));
        if (pType == NULL) {
                pLibType->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"item_type_not_found\"}");
                return;
        }

        CmyString strDefaultName   = pType->m_strName;
        DWORD dwDefaultGrpId       = pType->m_dwGrpID;
        DWORD dwDefaultIconGrpId   = pType->m_dwIconGrpID;
        DWORD dwDefaultDropSoundId = pType->m_dwDropSoundID;
        BOOL  bDefaultPutOn        = pType->m_bPutOn;
        pLibType->Leave();

        pLib->Enter();

        CInfoItem *pInfo = static_cast<CInfoItem *>(pLib->GetNew());
        if (pInfo == NULL) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 500 Internal Server Error";
                response.SetJsonBody("{\"error\":\"failed_to_create\"}");
                return;
        }

        // 種別からの初期値
        pInfo->m_dwItemTypeID  = static_cast<DWORD>(nItemTypeId);
        pInfo->m_strName       = strDefaultName;
        pInfo->m_dwGrpID       = dwDefaultGrpId;
        pInfo->m_dwIconGrpID   = dwDefaultIconGrpId;
        pInfo->m_dwDropSoundID = dwDefaultDropSoundId;
        pInfo->m_bPutOn        = bDefaultPutOn;

        // 任意フィールドで上書き
        std::string errReason;
        if (!ApplyJsonToItem(request.body, pInfo, errReason)) {
                delete pInfo;
                pLib->Leave();
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody(std::string("{\"error\":\"") + errReason + "\"}");
                return;
        }

        // Add 時に m_dwItemID == 0 なら新規 ID が採番される
        pInfo->m_dwItemID = 0;
        pLib->Add(pInfo);

        // 所持キャラが指定されている場合は所持リストへ追加
        if (pInfo->m_dwCharID != 0) {
                CLibInfoCharSvr *pCharLib = m_pMgrData->GetLibInfoChar();
                if (pCharLib != NULL) {
                        CInfoCharSvr *pInfoChar =
                                static_cast<CInfoCharSvr *>(pCharLib->GetPtr(pInfo->m_dwCharID));
                        if (pInfoChar != NULL) {
                                pLib->AddItem(pInfoChar->m_dwCharID, pInfo->m_dwItemID, &pInfoChar->m_adwItemID);
                        }
                }
        }

        // クライアントへ変更を通知
        CPacketITEM_RES_ITEMINFO packet;
        packet.Make(pInfo);
        pSock->SendTo(0, &packet);

        NotifyCharItems(m_pMgrData, pInfo->m_dwCharID);

        std::ostringstream oss;
        AppendItemJson(oss, pInfo, pLibType);

        pLib->Leave();

        response.statusLine = "HTTP/1.1 201 Created";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// PUT /api/items
// 必須: itemId
// 任意: 各フィールド
// ---------------------------------------------------------------------------

CItemUpdateHandler::CItemUpdateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CItemUpdateHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuthAndBackend(request, m_pMgrData, response)) {
                return;
        }

        CLibInfoItem *pLib = m_pMgrData->GetLibInfoItem();
        CLibInfoItemType *pLibType = m_pMgrData->GetLibInfoItemType();
        CUraraSockTCPSBO *pSock = m_pMgrData->GetSock();
        if (pLib == NULL || pSock == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nItemId = 0;
        if (!JsonUtils::TryGetInt(request.body, "itemId", nItemId) || nItemId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_itemId\"}");
                return;
        }

        pLib->Enter();

        CInfoItem *pInfo =
                static_cast<CInfoItem *>(pLib->GetPtr(static_cast<DWORD>(nItemId)));
        if (pInfo == NULL) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        DWORD dwOldCharId = pInfo->m_dwCharID;

        std::string errReason;
        if (!ApplyJsonToItem(request.body, pInfo, errReason)) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody(std::string("{\"error\":\"") + errReason + "\"}");
                return;
        }

        // クライアントへ変更を通知
        CPacketITEM_RES_ITEMINFO packet;
        packet.Make(pInfo);
        pSock->SendTo(0, &packet);

        DWORD dwNewCharId = pInfo->m_dwCharID;

        std::ostringstream oss;
        AppendItemJson(oss, pInfo, pLibType);

        pLib->Leave();

        // 所持キャラ変化時は旧・新の両方に通知
        if (dwOldCharId != dwNewCharId) {
                NotifyCharItems(m_pMgrData, dwOldCharId);
                NotifyCharItems(m_pMgrData, dwNewCharId);
        }

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// DELETE /api/items
// 必須: itemId または itemIds (数値配列)
// ---------------------------------------------------------------------------

CItemDeleteHandler::CItemDeleteHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CItemDeleteHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuthAndBackend(request, m_pMgrData, response)) {
                return;
        }

        CLibInfoItem *pLib = m_pMgrData->GetLibInfoItem();
        CUraraSockTCPSBO *pSock = m_pMgrData->GetSock();
        if (pLib == NULL || pSock == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        // 単一 itemId の削除のみ対応（複数削除はクライアント側でループ）
        std::vector<int> ids;
        int nItemId = 0;
        if (!JsonUtils::TryGetInt(request.body, "itemId", nItemId) || nItemId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_itemId\"}");
                return;
        }
        ids.push_back(nItemId);

        // 通知対象のキャラ ID を収集（所持アイテム通知用）
        std::vector<DWORD> affectedCharIds;

        std::ostringstream oss;
        oss << "{\"deleted\":[";
        bool bFirst = true;

        pLib->Enter();
        CLibInfoCharSvr *pCharLib = m_pMgrData->GetLibInfoChar();

        for (size_t i = 0; i < ids.size(); ++i) {
                int nItemId = ids[i];
                if (nItemId <= 0) {
                        continue;
                }
                DWORD dwItemId = static_cast<DWORD>(nItemId);

                CInfoItem *pInfo =
                        static_cast<CInfoItem *>(pLib->GetPtr(dwItemId));
                if (pInfo == NULL) {
                        continue;
                }

                DWORD dwCharId = pInfo->m_dwCharID;

                // キャラが所持している場合は所持リストからも外す
                if (dwCharId != 0 && pCharLib != NULL) {
                        CInfoCharSvr *pInfoChar =
                                static_cast<CInfoCharSvr *>(pCharLib->GetPtr(dwCharId));
                        if (pInfoChar != NULL) {
                                pInfoChar->DeleteItem(dwItemId);
                                pLib->DeleteItem(dwItemId, pInfoChar);
                                affectedCharIds.push_back(dwCharId);
                        }
                }

                // 削除通知をクライアントへ
                CPacketITEM_DELETEITEMINFO packetDel;
                packetDel.Make(dwItemId);
                pSock->SendTo(0, &packetDel);

                pLib->Delete(dwItemId);

                if (!bFirst) { oss << ','; }
                bFirst = false;
                oss << dwItemId;
        }
        pLib->Leave();

        oss << "]}";

        // 所持アイテム情報を該当キャラに通知（重複除外）
        for (size_t i = 0; i < affectedCharIds.size(); ++i) {
                DWORD dwCharId = affectedCharIds[i];
                bool bAlreadyNotified = false;
                for (size_t j = 0; j < i; ++j) {
                        if (affectedCharIds[j] == dwCharId) {
                                bAlreadyNotified = true;
                                break;
                        }
                }
                if (!bAlreadyNotified) {
                        NotifyCharItems(m_pMgrData, dwCharId);
                }
        }

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}
