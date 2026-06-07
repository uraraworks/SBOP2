#include "StdAfx.h"
#include "EfcHandler.h"

#include <sstream>
#include <string>
#include <vector>

#include "Web/AuthProvider.h"
#include "Web/JsonUtils.h"
#include "MgrData.h"
#include "LibInfoEffect.h"
#include "InfoEffect.h"
#include "InfoAnime.h"
#include "UraraSockTCPSBO.h"
#include "PacketEFFECT_RES_EFFECTINFO.h"
#include "PacketEFFECT_DELETEEFFECTINFO.h"

// ---------------------------------------------------------------------------
// 内部ヘルパー
// ---------------------------------------------------------------------------

namespace
{

// UTF-8 → CmyString（内部文字コード）
bool SetEffectNameFromUtf8(CmyString &out, const std::string &utf8)
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
std::string EffectNameToUtf8(const CmyString &value)
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

// --- 簡易 JSON オブジェクト配列パーサ ---
// 入力 JSON の中から "key":[{...},{...}] を抽出し、要素ごとの「{…}」文字列を返す。
// 文字列内の '{' '}' '[' ']' はスキップする。
// 返り値: 見つかれば true、配列が存在しない場合は false、[] なら true（空配列）。
bool ExtractObjectArray(const std::string &json, const std::string &key,
                        std::vector<std::string> &outElements, bool &outFound)
{
        outElements.clear();
        outFound = false;

        // "key": を探す
        std::string pat = "\"" + key + "\"";
        size_t nPos = 0;
        while (true) {
                size_t nFind = json.find(pat, nPos);
                if (nFind == std::string::npos) {
                        return false;
                }
                // 前方が "(キー部) であることを確認（簡易）
                size_t nAfter = nFind + pat.size();
                // コロンまでスペース飛ばし
                while (nAfter < json.size() && (json[nAfter] == ' ' || json[nAfter] == '\t' ||
                                                 json[nAfter] == '\n' || json[nAfter] == '\r')) {
                        ++nAfter;
                }
                if (nAfter < json.size() && json[nAfter] == ':') {
                        ++nAfter;
                        while (nAfter < json.size() && (json[nAfter] == ' ' || json[nAfter] == '\t' ||
                                                         json[nAfter] == '\n' || json[nAfter] == '\r')) {
                                ++nAfter;
                        }
                        if (nAfter < json.size() && json[nAfter] == '[') {
                                nPos = nAfter;
                                break;
                        }
                }
                nPos = nFind + 1;
        }

        outFound = true;
        // nPos は '[' の位置
        size_t i = nPos + 1;
        int depthBrace = 0;
        int depthBracket = 0;
        bool bInString = false;
        bool bEscape = false;
        size_t nElemStart = std::string::npos;

        while (i < json.size()) {
                char c = json[i];
                if (bInString) {
                        if (bEscape) {
                                bEscape = false;
                        } else if (c == '\\') {
                                bEscape = true;
                        } else if (c == '"') {
                                bInString = false;
                        }
                        ++i;
                        continue;
                }
                if (c == '"') {
                        bInString = true;
                        ++i;
                        continue;
                }
                if (c == '{') {
                        if (depthBrace == 0 && depthBracket == 0) {
                                nElemStart = i;
                        }
                        ++depthBrace;
                        ++i;
                        continue;
                }
                if (c == '}') {
                        --depthBrace;
                        if (depthBrace == 0 && depthBracket == 0 && nElemStart != std::string::npos) {
                                outElements.push_back(json.substr(nElemStart, i - nElemStart + 1));
                                nElemStart = std::string::npos;
                        }
                        ++i;
                        continue;
                }
                if (c == '[') {
                        ++depthBracket;
                        ++i;
                        continue;
                }
                if (c == ']') {
                        if (depthBracket == 0) {
                                // 外側配列の終端
                                return true;
                        }
                        --depthBracket;
                        ++i;
                        continue;
                }
                ++i;
        }
        return true;
}

// CInfoAnime → JSON 出力
void AppendAnimeJson(std::ostringstream &oss, const CInfoAnime *pAnime)
{
        oss << '{';
        oss << "\"wait\":"      << static_cast<int>(pAnime->m_byWait)  << ',';
        oss << "\"level\":"     << static_cast<int>(pAnime->m_byLevel) << ',';
        oss << "\"grpIdBase\":" << pAnime->m_wGrpIDBase                << ',';
        oss << "\"grpIdPile\":" << pAnime->m_wGrpIDPile;
        oss << '}';
}

// CInfoEffect → JSON 出力（animes 配列を nested で含む）
void AppendEffectJson(std::ostringstream &oss, CInfoEffect *pInfo)
{
        oss << '{';
        oss << "\"effectId\":"   << pInfo->m_dwEffectID   << ',';
        oss << "\"name\":\""     << JsonUtils::Escape(EffectNameToUtf8(pInfo->m_strName)) << "\",";
        oss << "\"soundId\":"    << pInfo->m_dwSoundID    << ',';
        oss << "\"grpIdMain\":"  << pInfo->m_dwGrpIDMain  << ',';
        oss << "\"loop\":"       << (pInfo->m_bLoop ? "true" : "false") << ',';
        oss << "\"loopSound\":"  << (pInfo->m_bLoopSound ? "true" : "false") << ',';
        oss << "\"animeCount\":" << static_cast<int>(pInfo->m_byAnimeCount) << ',';
        oss << "\"animes\":[";
        int nAnimeCnt = pInfo->GetAnimeCount();
        for (int i = 0; i < nAnimeCnt; ++i) {
                PCInfoAnime pAnime = pInfo->GetAnimePtr(i);
                if (pAnime == NULL) { continue; }
                if (i > 0) { oss << ','; }
                AppendAnimeJson(oss, pAnime);
        }
        oss << "]}";
}

// JSON(アニメ単体) → CInfoAnime へ適用
void ApplyJsonToAnime(const std::string &json, CInfoAnime *pAnime)
{
        int nVal = 0;
        if (JsonUtils::TryGetInt(json, "wait", nVal) && nVal >= 0 && nVal <= 255) {
                pAnime->m_byWait = static_cast<BYTE>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "level", nVal) && nVal >= 0 && nVal <= 255) {
                pAnime->m_byLevel = static_cast<BYTE>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "grpIdBase", nVal) && nVal >= 0 && nVal <= 0xFFFF) {
                pAnime->m_wGrpIDBase = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "grpIdPile", nVal) && nVal >= 0 && nVal <= 0xFFFF) {
                pAnime->m_wGrpIDPile = static_cast<WORD>(nVal);
        }
}

// JSON → CInfoEffect のフィールドを上書き（m_dwEffectID は呼び出し側が扱う）
// animes が body にあれば全コマ差し替え、なければ既存維持。
bool ApplyJsonToEffect(const std::string &json, CInfoEffect *pInfo, std::string &outErr)
{
        outErr.clear();

        std::string name;
        if (JsonUtils::TryGetString(json, "name", name)) {
                if (!SetEffectNameFromUtf8(pInfo->m_strName, name)) {
                        outErr = "invalid_name_encoding";
                        return false;
                }
        }

        int nVal = 0;
        if (JsonUtils::TryGetInt(json, "soundId", nVal) && nVal >= 0) {
                pInfo->m_dwSoundID = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "grpIdMain", nVal) && nVal >= 0) {
                pInfo->m_dwGrpIDMain = static_cast<DWORD>(nVal);
        }

        bool bVal = false;
        if (JsonUtils::TryGetBool(json, "loop", bVal)) {
                pInfo->m_bLoop = bVal ? TRUE : FALSE;
        }
        if (JsonUtils::TryGetBool(json, "loopSound", bVal)) {
                pInfo->m_bLoopSound = bVal ? TRUE : FALSE;
        }

        // animes 配列（指定時のみ差し替え）
        std::vector<std::string> animeElems;
        bool bAnimesFound = false;
        ExtractObjectArray(json, "animes", animeElems, bAnimesFound);
        if (bAnimesFound) {
                if (animeElems.size() > 255) {
                        outErr = "too_many_animes";
                        return false;
                }
                pInfo->DeleteAllAnime();
                for (size_t i = 0; i < animeElems.size(); ++i) {
                        pInfo->AddAnime();
                        PCInfoAnime pAnime = pInfo->GetAnimePtr(static_cast<int>(i));
                        if (pAnime != NULL) {
                                ApplyJsonToAnime(animeElems[i], pAnime);
                        }
                }
                pInfo->m_byAnimeCount = static_cast<BYTE>(pInfo->GetAnimeCount());
        }

        return true;
}

// 1エフェクトを CPacketEFFECT_RES_EFFECTINFO で全クライアントへ配信
// ネイティブ版 RecvProcADMIN_EFC_RENEWEFFECT と同等の挙動。
void BroadcastEffect(CMgrData *pMgrData, CInfoEffect *pInfo)
{
        if (pMgrData == NULL || pInfo == NULL) {
                return;
        }
        CUraraSockTCPSBO *pSock = pMgrData->GetSock();
        if (pSock == NULL) {
                return;
        }

        CPacketEFFECT_RES_EFFECTINFO packet;
        packet.Make(pInfo);
        pSock->SendTo(0, &packet);
}

// 削除通知を全クライアントへ配信
void BroadcastEffectDelete(CMgrData *pMgrData, DWORD dwEffectID)
{
        if (pMgrData == NULL) {
                return;
        }
        CUraraSockTCPSBO *pSock = pMgrData->GetSock();
        if (pSock == NULL) {
                return;
        }

        CPacketEFFECT_DELETEEFFECTINFO packet;
        packet.Make(dwEffectID);
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
// GET /api/effects
// ---------------------------------------------------------------------------

CEfcListHandler::CEfcListHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CEfcListHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuthAndBackend(request, m_pMgrData, response)) {
                return;
        }

        CLibInfoEffect *pLib = m_pMgrData->GetLibInfoEffect();
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
                CInfoEffect *pInfo = static_cast<CInfoEffect *>(pLib->GetPtr(i));
                if (pInfo == NULL) {
                        continue;
                }
                if (!bFirst) { oss << ','; }
                bFirst = false;
                AppendEffectJson(oss, pInfo);
        }
        pLib->Leave();

        oss << "]}";

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// POST /api/effects
// 任意: name / soundId / grpIdMain / loop / loopSound / animes
// 成功時: 201 Created + 作成したエフェクト情報 JSON
// ---------------------------------------------------------------------------

CEfcCreateHandler::CEfcCreateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CEfcCreateHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuthAndBackend(request, m_pMgrData, response)) {
                return;
        }

        CLibInfoEffect *pLib = m_pMgrData->GetLibInfoEffect();
        if (pLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        pLib->Enter();

        CInfoEffect *pInfo = static_cast<CInfoEffect *>(pLib->GetNew());
        if (pInfo == NULL) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 500 Internal Server Error";
                response.SetJsonBody("{\"error\":\"failed_to_create\"}");
                return;
        }

        std::string errReason;
        if (!ApplyJsonToEffect(request.body, pInfo, errReason)) {
                delete pInfo;
                pLib->Leave();
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody(std::string("{\"error\":\"") + errReason + "\"}");
                return;
        }

        // Add 時に m_dwEffectID == 0 なら新規 ID が採番される
        pInfo->m_dwEffectID = 0;
        pLib->Add(pInfo);

        std::ostringstream oss;
        AppendEffectJson(oss, pInfo);

        pLib->Leave();

        // クライアントへ変更を配信
        BroadcastEffect(m_pMgrData, pInfo);

        response.statusLine = "HTTP/1.1 201 Created";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// PUT /api/effects
// 必須: effectId
// 任意: 各フィールド（animes は指定時のみ全件差し替え）
// ---------------------------------------------------------------------------

CEfcUpdateHandler::CEfcUpdateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CEfcUpdateHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuthAndBackend(request, m_pMgrData, response)) {
                return;
        }

        CLibInfoEffect *pLib = m_pMgrData->GetLibInfoEffect();
        if (pLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nEffectId = 0;
        if (!JsonUtils::TryGetInt(request.body, "effectId", nEffectId) || nEffectId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_effectId\"}");
                return;
        }

        DWORD dwEffectID = static_cast<DWORD>(nEffectId);

        pLib->Enter();

        CInfoEffect *pInfo = static_cast<CInfoEffect *>(pLib->GetPtr(dwEffectID));
        if (pInfo == NULL) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        std::string errReason;
        if (!ApplyJsonToEffect(request.body, pInfo, errReason)) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody(std::string("{\"error\":\"") + errReason + "\"}");
                return;
        }

        std::ostringstream oss;
        AppendEffectJson(oss, pInfo);

        pLib->Leave();

        // クライアントへ配信
        BroadcastEffect(m_pMgrData, pInfo);

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// DELETE /api/effects
// 必須: effectId
// ---------------------------------------------------------------------------

CEfcDeleteHandler::CEfcDeleteHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CEfcDeleteHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!CheckAuthAndBackend(request, m_pMgrData, response)) {
                return;
        }

        CLibInfoEffect *pLib = m_pMgrData->GetLibInfoEffect();
        if (pLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nEffectId = 0;
        if (!JsonUtils::TryGetInt(request.body, "effectId", nEffectId) || nEffectId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_effectId\"}");
                return;
        }

        DWORD dwEffectID = static_cast<DWORD>(nEffectId);

        pLib->Enter();

        CInfoEffect *pInfo = static_cast<CInfoEffect *>(pLib->GetPtr(dwEffectID));
        if (pInfo == NULL) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        pLib->Delete(dwEffectID);

        pLib->Leave();

        // クライアントへ削除通知を配信
        BroadcastEffectDelete(m_pMgrData, dwEffectID);

        std::ostringstream oss;
        oss << "{\"deleted\":" << dwEffectID << "}";

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}
