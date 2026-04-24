/// @file TalkEventHandler.cpp
/// @brief 会話イベント情報 Web API ハンドラ 実装ファイル
///
/// 【扱うコマンド種別】
///   NONE     : 何もしない（新規スロットの初期値など）
///   PAGE     : ページ切り替え（m_nPageChgCondition / m_nPageJump）
///   MSG      : メッセージ表示（m_strText）
///   MENU     : 選択肢（menuItems[].{page,name}）
///   ADDSKILL : スキル付与（m_dwData = スキル ID）
///
/// 【JSON モデル】
///   {
///     "id": <TalkEventID>,
///     "events": [
///       { "slot":0, "type":1, "typeLabel":"PAGE", "page":0, "data":0, "text":"",
///         "pageChgCondition":0, "pageJump":0,
///         "menuItems":[ {"page":1, "name":"項目名"} ] }
///     ]
///   }
#include "stdafx.h"
#include "TalkEventHandler.h"

#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

#include "Web/AuthProvider.h"
#include "Web/JsonUtils.h"
#include "MgrData.h"
#include "LibInfoTalkEvent.h"
#include "InfoTalkEvent.h"
#include "InfoTalkEventBase.h"
#include "InfoTalkEventPAGE.h"
#include "InfoTalkEventMENU.h"

namespace
{

// ---------------------------------------------------------------------------
// 文字列変換ヘルパ（UTF-8 <=> CmyString 内部形式 = SJIS）
// ---------------------------------------------------------------------------

// UTF-8 → CmyString
bool Utf8ToCmyString(const std::string &utf8, CmyString &out)
{
        if (utf8.empty()) {
                out = _T("");
                return true;
        }
        int wideLen = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), static_cast<int>(utf8.size()), NULL, 0);
        if (wideLen <= 0) { return false; }
        std::vector<wchar_t> wide(static_cast<size_t>(wideLen) + 1, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), static_cast<int>(utf8.size()), &wide[0], wideLen);
#ifdef _UNICODE
        out = &wide[0];
#else
        int mbLen = WideCharToMultiByte(932, 0, &wide[0], wideLen, NULL, 0, NULL, NULL);
        if (mbLen <= 0) { return false; }
        std::vector<char> mb(static_cast<size_t>(mbLen) + 1, '\0');
        WideCharToMultiByte(932, 0, &wide[0], wideLen, &mb[0], mbLen, NULL, NULL);
        out = &mb[0];
#endif
        return true;
}

// CmyString → UTF-8
std::string CmyStringToUtf8(const CmyString &value)
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

// クエリ文字列から指定キーの整数値を取得する
bool TryGetQueryParam(const std::string &path, const std::string &key, int &outValue)
{
        size_t nQueryPos = path.find('?');
        if (nQueryPos == std::string::npos) { return false; }
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
                if (nAmp == std::string::npos) { break; }
                nPos = nAmp + 1;
        }
        return false;
}

// イベント種別ラベル
const char *GetEventTypeLabel(int nType)
{
        switch (nType) {
        case TALKEVENTTYPE_NONE:     return "NONE";
        case TALKEVENTTYPE_PAGE:     return "PAGE";
        case TALKEVENTTYPE_MSG:      return "MSG";
        case TALKEVENTTYPE_MENU:     return "MENU";
        case TALKEVENTTYPE_ADDSKILL: return "ADDSKILL";
        default:                     return "UNKNOWN";
        }
}

// JSON の "key":[...] の角括弧内文字列を取り出す（雑パース）
// ネストや文字列中の ']' 等は考慮しないが、会話イベントの events / menuItems は
// 単純な配列のため、これで十分。外側 '[' の位置と中身の substring を返す。
bool FindArrayBody(const std::string &json, const std::string &key, std::string &outBody, size_t &outEnd)
{
        size_t nKey = JsonUtils::FindKey(json, key);
        if (nKey == std::string::npos) { return false; }
        size_t nOpen = json.find('[', nKey);
        if (nOpen == std::string::npos) { return false; }

        // 括弧の深さを追いつつ、対応する ']' を探す
        int depth = 0;
        size_t i = nOpen;
        bool inStr = false;
        for (; i < json.size(); ++i) {
                char c = json[i];
                if (inStr) {
                        if (c == '\\' && i + 1 < json.size()) { ++i; continue; }
                        if (c == '"') { inStr = false; }
                        continue;
                }
                if (c == '"') { inStr = true; continue; }
                if (c == '[') { ++depth; continue; }
                if (c == ']') {
                        --depth;
                        if (depth == 0) { break; }
                }
        }
        if (depth != 0 || i >= json.size()) { return false; }
        outBody = json.substr(nOpen + 1, i - (nOpen + 1));
        outEnd = i;
        return true;
}

// "{...},{...}" の文字列を要素ごとの JSON オブジェクトに分割する
void SplitJsonObjects(const std::string &body, std::vector<std::string> &outObjects)
{
        int depth = 0;
        bool inStr = false;
        size_t start = std::string::npos;
        for (size_t i = 0; i < body.size(); ++i) {
                char c = body[i];
                if (inStr) {
                        if (c == '\\' && i + 1 < body.size()) { ++i; continue; }
                        if (c == '"') { inStr = false; }
                        continue;
                }
                if (c == '"') { inStr = true; continue; }
                if (c == '{') {
                        if (depth == 0) { start = i; }
                        ++depth;
                } else if (c == '}') {
                        --depth;
                        if (depth == 0 && start != std::string::npos) {
                                outObjects.push_back(body.substr(start, i - start + 1));
                                start = std::string::npos;
                        }
                }
        }
}

// ------------------------------------------------------------------
// JSON 出力: 会話イベント 1 件（= 1 TalkEvent = 1 キャラクター）
// ------------------------------------------------------------------
void AppendTalkEventJson(std::ostringstream &oss, CInfoTalkEvent *pInfo)
{
        oss << '{';
        oss << "\"id\":" << pInfo->m_dwTalkEventID << ',';
        oss << "\"pageCount\":" << pInfo->GetPageCount() << ',';
        oss << "\"eventCount\":" << static_cast<int>(pInfo->m_apTalkEvent.size()) << ',';
        oss << "\"events\":[";

        int nCount = static_cast<int>(pInfo->m_apTalkEvent.size());
        for (int i = 0; i < nCount; ++i) {
                PCInfoTalkEventBase pEvt = pInfo->m_apTalkEvent[i];
                if (pEvt == NULL) { continue; }
                if (i > 0) { oss << ','; }
                oss << '{';
                oss << "\"slot\":" << i << ',';
                oss << "\"type\":" << pEvt->m_nEventType << ',';
                oss << "\"typeLabel\":\"" << GetEventTypeLabel(pEvt->m_nEventType) << "\",";
                oss << "\"page\":" << pEvt->m_nPage << ',';
                oss << "\"data\":" << pEvt->m_dwData << ',';
                oss << "\"text\":\"" << JsonUtils::Escape(CmyStringToUtf8(pEvt->m_strText)) << "\"";

                if (pEvt->m_nEventType == TALKEVENTTYPE_PAGE) {
                        CInfoTalkEventPAGE *pPage = static_cast<CInfoTalkEventPAGE *>(pEvt);
                        oss << ",\"pageChgCondition\":" << pPage->m_nPageChgCondition;
                        oss << ",\"pageJump\":" << pPage->m_nPageJump;
                }
                if (pEvt->m_nEventType == TALKEVENTTYPE_MENU) {
                        CInfoTalkEventMENU *pMenu = static_cast<CInfoTalkEventMENU *>(pEvt);
                        oss << ",\"menuItems\":[";
                        int nMenu = pMenu->GetMenuInfoCount();
                        for (int k = 0; k < nMenu; ++k) {
                                PSTTALKEVENTMENUINFO pItem = pMenu->GetPtr(k);
                                if (pItem == NULL) { continue; }
                                if (k > 0) { oss << ','; }
                                oss << "{\"page\":" << pItem->nPage
                                    << ",\"name\":\"" << JsonUtils::Escape(CmyStringToUtf8(pItem->strName)) << "\"}";
                        }
                        oss << ']';
                }
                oss << '}';
        }
        oss << "]}";
}

// ------------------------------------------------------------------
// JSON → CInfoTalkEvent への適用（events 配列を丸ごと差し替える）
// ------------------------------------------------------------------
void ApplyEventsJson(const std::string &json, CInfoTalkEvent *pInfo)
{
        // 既存スロットを全削除
        pInfo->DeleteAllTalkEvent();

        std::string arrBody;
        size_t dummy = 0;
        if (!FindArrayBody(json, "events", arrBody, dummy)) {
                return;
        }

        std::vector<std::string> objs;
        SplitJsonObjects(arrBody, objs);

        for (size_t i = 0; i < objs.size(); ++i) {
                const std::string &obj = objs[i];

                int nType = TALKEVENTTYPE_NONE;
                JsonUtils::TryGetInt(obj, "type", nType);
                if (nType < TALKEVENTTYPE_NONE || nType >= TALKEVENTTYPE_MAX) {
                        nType = TALKEVENTTYPE_NONE;
                }

                PCInfoTalkEventBase pEvt = pInfo->GetNew(nType);
                if (pEvt == NULL) { continue; }

                pEvt->m_nEventType = nType;

                int nVal = 0;
                if (JsonUtils::TryGetInt(obj, "page", nVal)) { pEvt->m_nPage = nVal; }
                if (JsonUtils::TryGetInt(obj, "data", nVal)) { pEvt->m_dwData = static_cast<DWORD>(nVal); }

                std::string strVal;
                if (JsonUtils::TryGetString(obj, "text", strVal)) {
                        Utf8ToCmyString(strVal, pEvt->m_strText);
                }

                if (nType == TALKEVENTTYPE_PAGE) {
                        CInfoTalkEventPAGE *pPage = static_cast<CInfoTalkEventPAGE *>(pEvt);
                        if (JsonUtils::TryGetInt(obj, "pageChgCondition", nVal)) { pPage->m_nPageChgCondition = nVal; }
                        if (JsonUtils::TryGetInt(obj, "pageJump", nVal)) { pPage->m_nPageJump = nVal; }
                }
                if (nType == TALKEVENTTYPE_MENU) {
                        CInfoTalkEventMENU *pMenu = static_cast<CInfoTalkEventMENU *>(pEvt);
                        std::string menuBody;
                        size_t menuEnd = 0;
                        if (FindArrayBody(obj, "menuItems", menuBody, menuEnd)) {
                                std::vector<std::string> menuObjs;
                                SplitJsonObjects(menuBody, menuObjs);
                                for (size_t k = 0; k < menuObjs.size(); ++k) {
                                        int nPage = 0;
                                        std::string strName;
                                        JsonUtils::TryGetInt(menuObjs[k], "page", nPage);
                                        JsonUtils::TryGetString(menuObjs[k], "name", strName);
                                        CmyString cmyName;
                                        Utf8ToCmyString(strName, cmyName);
                                        pMenu->AddMenuInfo(nPage, static_cast<LPCSTR>(cmyName));
                                }
                        }
                }

                pInfo->AddTalkEvent(pEvt);
        }
}

} // namespace

// ---------------------------------------------------------------------------
// CTalkEventListHandler  GET /api/talk-events[?id=<talkEventId>]
// ---------------------------------------------------------------------------

CTalkEventListHandler::CTalkEventListHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CTalkEventListHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        AuthProvider::AuthContext authContext;
        AuthProvider::AuthStatus authStatus = AuthProvider::Authenticate(request, m_pMgrData, authContext);
        if (authStatus == AuthProvider::AuthStatusBackendUnavailable) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        if (m_pMgrData == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        CLibInfoTalkEvent *pLib = m_pMgrData->GetLibInfoTalkEvent();
        if (pLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nId = 0;
        bool bHasId = TryGetQueryParam(request.path, "id", nId);

        std::ostringstream oss;

        if (bHasId) {
                // 単一取得
                CInfoTalkEvent *pInfo = static_cast<CInfoTalkEvent *>(pLib->GetPtr(static_cast<DWORD>(nId)));
                if (pInfo == NULL) {
                        response.statusLine = "HTTP/1.1 404 Not Found";
                        response.SetJsonBody("{\"error\":\"talk_event_not_found\"}");
                        return;
                }
                AppendTalkEventJson(oss, pInfo);
                response.statusLine = "HTTP/1.1 200 OK";
                response.SetJsonBody(oss.str());
                return;
        }

        // 一覧取得（概要のみ: id / pageCount / eventCount）
        oss << "{\"talkEvents\":[";
        int nCount = pLib->GetCount();
        bool first = true;
        for (int i = 0; i < nCount; ++i) {
                CInfoTalkEvent *pInfo = static_cast<CInfoTalkEvent *>(pLib->GetPtr(i));
                if (pInfo == NULL) { continue; }
                if (!first) { oss << ','; }
                first = false;
                oss << "{\"id\":" << pInfo->m_dwTalkEventID
                    << ",\"pageCount\":" << pInfo->GetPageCount()
                    << ",\"eventCount\":" << static_cast<int>(pInfo->m_apTalkEvent.size())
                    << '}';
        }
        oss << "]}";
        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// CTalkEventUpdateHandler  PUT /api/talk-events
//   ボディ: { "id": <TalkEventID>, "events":[...] }
//   - 指定 id の会話イベントが存在すれば events[] で置き換え
//   - 存在しなければ新規追加して events[] を適用
// ---------------------------------------------------------------------------

CTalkEventUpdateHandler::CTalkEventUpdateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CTalkEventUpdateHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        AuthProvider::AuthContext authContext;
        AuthProvider::AuthStatus authStatus = AuthProvider::Authenticate(request, m_pMgrData, authContext);
        if (authStatus == AuthProvider::AuthStatusBackendUnavailable) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        if (m_pMgrData == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        CLibInfoTalkEvent *pLib = m_pMgrData->GetLibInfoTalkEvent();
        if (pLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nId = 0;
        if (!JsonUtils::TryGetInt(request.body, "id", nId) || nId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_id\"}");
                return;
        }

        CInfoTalkEvent *pInfo = static_cast<CInfoTalkEvent *>(pLib->GetPtr(static_cast<DWORD>(nId)));
        if (pInfo == NULL) {
                // 新規追加
                pInfo = static_cast<CInfoTalkEvent *>(pLib->GetNew());
                if (pInfo == NULL) {
                        response.statusLine = "HTTP/1.1 500 Internal Server Error";
                        response.SetJsonBody("{\"error\":\"failed_to_create_talk_event\"}");
                        return;
                }
                pInfo->m_dwTalkEventID = static_cast<DWORD>(nId);
                pLib->Add(pInfo);
        }

        ApplyEventsJson(request.body, pInfo);

        std::ostringstream oss;
        AppendTalkEventJson(oss, pInfo);

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// CTalkEventDeleteHandler  DELETE /api/talk-events?id=<talkEventId>
// ---------------------------------------------------------------------------

CTalkEventDeleteHandler::CTalkEventDeleteHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CTalkEventDeleteHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        AuthProvider::AuthContext authContext;
        AuthProvider::AuthStatus authStatus = AuthProvider::Authenticate(request, m_pMgrData, authContext);
        if (authStatus == AuthProvider::AuthStatusBackendUnavailable) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        if (m_pMgrData == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        CLibInfoTalkEvent *pLib = m_pMgrData->GetLibInfoTalkEvent();
        if (pLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nId = 0;
        if (!TryGetQueryParam(request.path, "id", nId) || nId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_id\"}");
                return;
        }

        if (pLib->GetPtr(static_cast<DWORD>(nId)) == NULL) {
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"talk_event_not_found\"}");
                return;
        }

        pLib->Delete(static_cast<DWORD>(nId));

        std::ostringstream oss;
        oss << "{\"deleted\":" << nId << '}';
        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}
