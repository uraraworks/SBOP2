#include "StdAfx.h"
#include "MapInfoHandler.h"

#include <sstream>
#include <vector>
#include <cstring>

#include "Web/AuthProvider.h"
#include "Web/JsonUtils.h"
#include "MgrData.h"
#include "LibInfo/LibInfoMapBase.h"
#include "Info/InfoMapBase.h"
#include "myLib/myString.h"
#include "UraraSockTCPSBO.h"
#include "Packet/ADMIN/PacketADMIN_MAP_SETMAPNAME.h"
#include "Packet/MAP/PacketMAP_SYSTEMMSG.h"
#include "LibInfo/LibInfoMapObject.h"

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

// UTF-8 文字列を内部文字コード (MBCS: CP932 / Unicode: UTF-16) に変換して CmyString へ代入
bool SetStringFromUtf8(CmyString &out, const std::string &utf8)
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

// CmyString をパケット用 MBCS (CP932) LPCSTR に変換してバッファへ格納する
// 戻り値: 変換結果を保持する std::string（LPCSTR として .c_str() を渡す）
std::string ToMbcsString(const CmyString &value)
{
#ifdef _UNICODE
        // Unicode → CP932 変換
        LPCWSTR pszWide = static_cast<LPCWSTR>(value);
        if ((pszWide == NULL) || (pszWide[0] == L'\0')) {
                return std::string();
        }
        int mbLen = WideCharToMultiByte(932, 0, pszWide, -1, NULL, 0, NULL, NULL);
        if (mbLen <= 0) {
                return std::string();
        }
        std::string mb(static_cast<size_t>(mbLen), '\0');
        WideCharToMultiByte(932, 0, pszWide, -1, &mb[0], mbLen, NULL, NULL);
        // null 終端を除去
        if (!mb.empty() && mb.back() == '\0') {
                mb.resize(mb.size() - 1);
        }
        return mb;
#else
        LPCSTR psz = static_cast<LPCSTR>(value);
        if ((psz == NULL) || (psz[0] == '\0')) {
                return std::string();
        }
        return std::string(psz);
#endif
}

// マップ情報 JSON を 1 エントリ分構築する
void BuildMapInfoJson(std::ostringstream &oss, const CInfoMapBase *pMap)
{
        oss << '{';
        oss << "\"id\":" << pMap->m_dwMapID << ',';
        oss << "\"name\":\"" << JsonUtils::Escape(ToUtf8String(pMap->m_strMapName)) << "\",";
        oss << "\"width\":" << pMap->m_sizeMap.cx << ',';
        oss << "\"height\":" << pMap->m_sizeMap.cy << ',';
        oss << "\"bgmId\":" << pMap->m_dwBGMID << ',';
        oss << "\"weatherType\":" << pMap->m_dwWeatherType << ',';
        oss << "\"battleEnabled\":" << (pMap->m_bEnableBattle ? "true" : "false") << ',';
        oss << "\"recoveryEnabled\":" << (pMap->m_bRecovery ? "true" : "false") << ',';
        oss << "\"darknessLevel\":" << static_cast<int>(pMap->m_byLevel);
        oss << '}';
}

} // namespace

// ---------------------------------------------------------------------------
// CMapInfoListHandler  GET /api/maps
// ---------------------------------------------------------------------------

CMapInfoListHandler::CMapInfoListHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMapInfoListHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        AuthProvider::AuthContext authContext;
        AuthProvider::AuthStatus authStatus = AuthProvider::Authenticate(request, m_pMgrData, authContext);
        if (authStatus == AuthProvider::AuthStatusBackendUnavailable) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }
        // TODO: 本番運用時は適切なロール判定を追加すること

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(BuildResponseJson());
}

std::string CMapInfoListHandler::BuildResponseJson() const
{
        if (m_pMgrData == NULL) {
                return "{\"maps\":[]}";
        }

        CLibInfoMapBase *pMapLib = m_pMgrData->GetLibInfoMap();
        if (pMapLib == NULL) {
                return "{\"maps\":[]}";
        }

        pMapLib->Enter();

        std::ostringstream oss;
        oss << "{\"maps\":[";
        bool first = true;
        int nCount = pMapLib->GetCount();
        for (int i = 0; i < nCount; ++i) {
                const CInfoMapBase *pMap = static_cast<const CInfoMapBase *>(pMapLib->GetPtr(i));
                if (pMap == NULL) {
                        continue;
                }
                if (!first) {
                        oss << ',';
                }
                first = false;
                BuildMapInfoJson(oss, pMap);
        }
        oss << "]}";

        pMapLib->Leave();
        return oss.str();
}

// ---------------------------------------------------------------------------
// CMapInfoUpdateHandler  PUT /api/maps
// ---------------------------------------------------------------------------

CMapInfoUpdateHandler::CMapInfoUpdateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMapInfoUpdateHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        AuthProvider::AuthContext authContext;
        AuthProvider::AuthStatus authStatus = AuthProvider::Authenticate(request, m_pMgrData, authContext);
        if (authStatus == AuthProvider::AuthStatusBackendUnavailable) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }
        // TODO: 本番運用時は適切なロール判定を追加すること

        if (m_pMgrData == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        CLibInfoMapBase  *pMapLib = m_pMgrData->GetLibInfoMap();
        CUraraSockTCPSBO *pSock   = m_pMgrData->GetSock();
        if (pMapLib == NULL || pSock == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        // id は必須
        int nMapId = 0;
        if (!JsonUtils::TryGetInt(request.body, "id", nMapId) || nMapId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_id\"}");
                return;
        }

        pMapLib->Enter();

        CInfoMapBase *pMap = static_cast<CInfoMapBase *>(pMapLib->GetPtr(static_cast<DWORD>(nMapId)));
        if (pMap == NULL) {
                pMapLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"map_not_found\"}");
                return;
        }

        // 名前が変更されたかどうかを記録する（通知判断用）
        bool bNameChanged = false;

        // name フィールドが存在すれば更新
        std::string strName;
        if (JsonUtils::TryGetString(request.body, "name", strName)) {
                CmyString newName;
                if (SetStringFromUtf8(newName, strName)) {
                        // 変更があった場合のみフラグを立てる
                        if (ToUtf8String(pMap->m_strMapName) != strName) {
                                bNameChanged = true;
                        }
                        pMap->m_strMapName = newName;
                }
        }

        // bgmId
        int nVal = 0;
        if (JsonUtils::TryGetInt(request.body, "bgmId", nVal)) {
                pMap->m_dwBGMID = static_cast<DWORD>(nVal);
        }

        // weatherType
        if (JsonUtils::TryGetInt(request.body, "weatherType", nVal)) {
                pMap->m_dwWeatherType = static_cast<DWORD>(nVal);
        }

        // battleEnabled
        bool bVal = false;
        if (JsonUtils::TryGetBool(request.body, "battleEnabled", bVal)) {
                pMap->m_bEnableBattle = bVal ? TRUE : FALSE;
        }

        // recoveryEnabled
        if (JsonUtils::TryGetBool(request.body, "recoveryEnabled", bVal)) {
                pMap->m_bRecovery = bVal ? TRUE : FALSE;
        }

        // darknessLevel (0-255)
        if (JsonUtils::TryGetInt(request.body, "darknessLevel", nVal)) {
                if (nVal < 0) { nVal = 0; }
                if (nVal > 255) { nVal = 255; }
                pMap->m_byLevel = static_cast<BYTE>(nVal);
        }

        // 名前変更時は CPacketADMIN_MAP_SETMAPNAME を全クライアントへブロードキャスト
        // このパケットはマップ名以外のフィールド（BGM/天候/戦闘/回復/明るさ）も
        // 一括で通知するため、変更内容を全て含めて送信する
        if (bNameChanged) {
                std::string mbcsName = ToMbcsString(pMap->m_strMapName);
                CPacketADMIN_MAP_SETMAPNAME packet;
                packet.Make(
                        pMap->m_dwMapID,
                        pMap->m_dwBGMID,
                        pMap->m_dwWeatherType,
                        pMap->m_bEnableBattle,
                        pMap->m_bRecovery,
                        pMap->m_byLevel,
                        mbcsName.c_str());
                pSock->SendTo(0, &packet);
        }
        // NOTE: 名前変更なしの場合はサーバー保存のみで即時通知なし（既存踏襲）
        // TODO: 名前変更なしの場合にも他フィールド変更を全クライアントへ通知したい場合、
        //       対応するパケットを追加するか SETMAPNAME を常に送信するよう変更すること

        // 更新後のエントリを返す
        std::ostringstream oss;
        BuildMapInfoJson(oss, pMap);

        pMapLib->Leave();

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// CMapInfoCreateHandler  POST /api/maps
// ---------------------------------------------------------------------------

CMapInfoCreateHandler::CMapInfoCreateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMapInfoCreateHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        AuthProvider::AuthContext authContext;
        AuthProvider::AuthStatus authStatus = AuthProvider::Authenticate(request, m_pMgrData, authContext);
        if (authStatus == AuthProvider::AuthStatusBackendUnavailable) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }
        // TODO: 本番運用時は適切なロール判定を追加すること

        if (m_pMgrData == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        CLibInfoMapBase    *pMapLib    = m_pMgrData->GetLibInfoMap();
        CLibInfoMapObject  *pMapObject = m_pMgrData->GetLibInfoMapObject();
        CUraraSockTCPSBO   *pSock      = m_pMgrData->GetSock();
        if (pMapLib == NULL || pSock == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        // copyFromMapId（省略または 0 で空マップ作成）
        int nCopyFromId = 0;
        JsonUtils::TryGetInt(request.body, "copyFromMapId", nCopyFromId);

        pMapLib->Enter();

        // コピー元マップが指定されていれば存在確認
        CInfoMapBase *pSrcMap = NULL;
        if (nCopyFromId != 0) {
                pSrcMap = static_cast<CInfoMapBase *>(pMapLib->GetPtr(static_cast<DWORD>(nCopyFromId)));
                if (pSrcMap == NULL) {
                        pMapLib->Leave();
                        response.statusLine = "HTTP/1.1 404 Not Found";
                        response.SetJsonBody("{\"error\":\"copy_source_not_found\"}");
                        return;
                }
        }

        // 新規マップを作成して追加
        CInfoMapBase *pNewMap = static_cast<CInfoMapBase *>(pMapLib->GetNew());
        pNewMap->Init(DRAW_PARTS_X, DRAW_PARTS_Y, 1);
        pMapLib->Add(pNewMap);
        pMapLib->SetMapObject(pMapObject);

        // コピー元が指定されていた場合は内容をコピー（MapID は新規 ID を維持）
        if (pSrcMap != NULL) {
                DWORD dwNewId = pNewMap->m_dwMapID;
                pNewMap->Copy(pSrcMap);
                pNewMap->m_dwMapID = dwNewId;
        }

        // 全クライアントへ SYSTEMMSG を送信（既存 RecvProcADMIN_MAP_ADD に倣う）
        CmyString strMsg;
        strMsg.Format(_T("SYSTEM:マップID[%d]が追加されました"), pNewMap->m_dwMapID);
        CPacketMAP_SYSTEMMSG packetMsg;
        packetMsg.Make(strMsg);
        pSock->SendTo(0, &packetMsg);

        // 作成したマップの JSON を返す
        std::ostringstream oss;
        BuildMapInfoJson(oss, pNewMap);

        pMapLib->Leave();

        response.statusLine = "HTTP/1.1 201 Created";
        response.SetJsonBody(oss.str());
}
