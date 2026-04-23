#include "stdafx.h"
#include "CharacterItemHandler.h"

#include <sstream>
#include <string>
#include <cstdlib>

#include "Web/AuthProvider.h"
#include "Web/JsonUtils.h"
#include "MgrData.h"
#include "LibInfo/LibInfoCharSvr.h"
#include "Info/InfoCharBase.h"
#include "Info/InfoCharSvr.h"

// スキルスロット上限（専用フィールドが無いため実装上の固定値）
static const int MAX_CHAR_SKILL_SLOTS = 64;

// ---------------------------------------------------------------------------
// 内部ヘルパー
// ---------------------------------------------------------------------------

namespace
{

// URL パスから charId・サブリソース種別・スロット番号を抽出する
//
// /api/characters/123/items         → charId=123, sub="items", slot=-1
// /api/characters/123/items/5       → charId=123, sub="items", slot=5
// /api/characters/123/skills        → charId=123, sub="skills", slot=-1
// /api/characters/123/skills/2      → charId=123, sub="skills", slot=2
//
// 解析失敗時は false を返す
bool ExtractCharItemPath(const std::string &path, int &outCharId, std::string &outSub, int &outSlot)
{
        outSlot = -1;
        const std::string prefix = "/api/characters/";
        if (path.size() <= prefix.size()) {
                return false;
        }

        // クエリ文字列除去
        std::string rest = path.substr(prefix.size());
        {
                size_t nQuery = rest.find('?');
                if (nQuery != std::string::npos) {
                        rest = rest.substr(0, nQuery);
                }
        }

        // {charId}/{sub}[/{slot}] を分割
        size_t nSlash1 = rest.find('/');
        if (nSlash1 == std::string::npos) {
                return false;   // サブリソースが無い
        }
        std::string idStr = rest.substr(0, nSlash1);
        std::string after = rest.substr(nSlash1 + 1);  // "items" or "items/5"

        // charId を変換
        if (idStr.empty()) {
                return false;
        }
        char *pEnd = NULL;
        long val = std::strtol(idStr.c_str(), &pEnd, 10);
        if (pEnd == NULL || *pEnd != '\0' || val <= 0) {
                return false;
        }
        outCharId = static_cast<int>(val);

        // サブリソース名とスロット番号に分割
        size_t nSlash2 = after.find('/');
        if (nSlash2 != std::string::npos) {
                outSub = after.substr(0, nSlash2);
                std::string slotStr = after.substr(nSlash2 + 1);
                if (!slotStr.empty()) {
                        char *pSlotEnd = NULL;
                        long slotVal = std::strtol(slotStr.c_str(), &pSlotEnd, 10);
                        if (pSlotEnd == NULL || *pSlotEnd != '\0' || slotVal < 0) {
                                return false;
                        }
                        outSlot = static_cast<int>(slotVal);
                }
        } else {
                outSub = after;
        }

        return (!outSub.empty());
}

// charId で CInfoCharBase* を検索して返す（Lock 済みの状態で呼ぶこと）
CInfoCharBase *FindChar(CLibInfoCharSvr *pCharLib, int nCharId)
{
        int nTotal = pCharLib->GetCount();
        for (int i = 0; i < nTotal; ++i) {
                CInfoCharBase *pTmp = static_cast<CInfoCharBase *>(pCharLib->GetPtr(i));
                if (pTmp != NULL && static_cast<int>(pTmp->m_dwCharID) == nCharId) {
                        return pTmp;
                }
        }
        return NULL;
}

// アイテム一覧 JSON を構築する
// [{slot:0, itemId:1001}, ...]
std::string BuildItemListJson(const CInfoCharBase *pChar)
{
        std::ostringstream oss;
        oss << '[';
        int nSize = pChar->m_adwItemID.GetSize();
        bool bFirst = true;
        for (int i = 0; i < nSize; ++i) {
                DWORD dwItemId = pChar->m_adwItemID.GetAt(i);
                if (dwItemId == 0) {
                        continue;   // 空スロットはスキップ
                }
                if (!bFirst) { oss << ','; }
                oss << "{\"slot\":" << i << ",\"itemId\":" << dwItemId << '}';
                bFirst = false;
        }
        oss << ']';
        return oss.str();
}

// スキル一覧 JSON を構築する
std::string BuildSkillListJson(const CInfoCharBase *pChar)
{
        std::ostringstream oss;
        oss << '[';
        int nSize = pChar->m_adwSkillID.GetSize();
        bool bFirst = true;
        for (int i = 0; i < nSize; ++i) {
                DWORD dwSkillId = pChar->m_adwSkillID.GetAt(i);
                if (dwSkillId == 0) {
                        continue;   // 空スロットはスキップ
                }
                if (!bFirst) { oss << ','; }
                oss << "{\"slot\":" << i << ",\"skillId\":" << dwSkillId << '}';
                bFirst = false;
        }
        oss << ']';
        return oss.str();
}

} // namespace

// ---------------------------------------------------------------------------
// CCharacterItemHandler
// ---------------------------------------------------------------------------

CCharacterItemHandler::CCharacterItemHandler(CMgrData *pMgrData, IApiHandler *pFallbackHandler)
        : m_pMgrData(pMgrData)
        , m_pFallbackHandler(pFallbackHandler)
{
}

CCharacterItemHandler::~CCharacterItemHandler()
{
        // フォールバックハンドラの所有権を持っているので解放する
        delete m_pFallbackHandler;
        m_pFallbackHandler = NULL;
}

void CCharacterItemHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        // 認証チェック
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

        // URL を解析して charId・サブリソース・スロット番号を取り出す
        int nCharId = 0;
        std::string sub;
        int nSlot = -1;
        if (!ExtractCharItemPath(request.path, nCharId, sub, nSlot)) {
                // サブリソース名が無い（例: /api/characters/123）場合は
                // フォールバックハンドラに委譲する（キャラクター詳細 GET）
                if (m_pFallbackHandler != NULL) {
                        m_pFallbackHandler->Handle(request, response);
                        return;
                }
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"invalid_path\"}");
                return;
        }

        const std::string &method = request.method;

        if (sub == "items") {
                if (method == "GET") {
                        HandleGetItems(request, response, nCharId);
                } else if (method == "POST") {
                        HandlePostItem(request, response, nCharId);
                } else if (method == "DELETE") {
                        if (nSlot < 0) {
                                response.statusLine = "HTTP/1.1 400 Bad Request";
                                response.SetJsonBody("{\"error\":\"slot_required\"}");
                        } else {
                                HandleDeleteItem(request, response, nCharId, nSlot);
                        }
                } else {
                        response.statusLine = "HTTP/1.1 405 Method Not Allowed";
                        response.SetJsonBody("{\"error\":\"method_not_allowed\"}");
                }
        } else if (sub == "skills") {
                if (method == "GET") {
                        HandleGetSkills(request, response, nCharId);
                } else if (method == "POST") {
                        HandlePostSkill(request, response, nCharId);
                } else if (method == "DELETE") {
                        if (nSlot < 0) {
                                response.statusLine = "HTTP/1.1 400 Bad Request";
                                response.SetJsonBody("{\"error\":\"slot_required\"}");
                        } else {
                                HandleDeleteSkill(request, response, nCharId, nSlot);
                        }
                } else {
                        response.statusLine = "HTTP/1.1 405 Method Not Allowed";
                        response.SetJsonBody("{\"error\":\"method_not_allowed\"}");
                }
        } else {
                // items/skills 以外のサブリソースはフォールバックに委譲
                if (m_pFallbackHandler != NULL) {
                        m_pFallbackHandler->Handle(request, response);
                        return;
                }
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
        }
}

// ---------------------------------------------------------------------------
// GET /api/characters/{charId}/items
// ---------------------------------------------------------------------------

void CCharacterItemHandler::HandleGetItems(const HttpRequest & /*request*/, HttpResponse &response, int nCharId)
{
        CLibInfoCharSvr *pCharLib = m_pMgrData->GetLibInfoChar();
        if (pCharLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        pCharLib->Enter();

        CInfoCharBase *pChar = FindChar(pCharLib, nCharId);
        if (pChar == NULL) {
                pCharLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        std::string json = BuildItemListJson(pChar);
        pCharLib->Leave();

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(json);
}

// ---------------------------------------------------------------------------
// POST /api/characters/{charId}/items
// body: { "itemId": <number> }
// 空きスロット（値 0）を先頭から探して挿入。満杯なら 409。
// ---------------------------------------------------------------------------

void CCharacterItemHandler::HandlePostItem(const HttpRequest &request, HttpResponse &response, int nCharId)
{
        int nItemId = 0;
        if (!JsonUtils::TryGetInt(request.body, "itemId", nItemId) || nItemId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"invalid_itemId\"}");
                return;
        }

        CLibInfoCharSvr *pCharLib = m_pMgrData->GetLibInfoChar();
        if (pCharLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        pCharLib->Enter();

        CInfoCharBase *pChar = FindChar(pCharLib, nCharId);
        if (pChar == NULL) {
                pCharLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        int nMaxSlot = pChar->m_nMaxItemCount;
        // m_nMaxItemCount が 0 以下の場合はデフォルト上限を使う
        if (nMaxSlot <= 0) {
                nMaxSlot = 64;
        }

        // 空きスロットを先頭から探す
        int nFoundSlot = -1;
        int nCurSize = pChar->m_adwItemID.GetSize();

        for (int i = 0; i < nCurSize; ++i) {
                if (pChar->m_adwItemID.GetAt(i) == 0) {
                        nFoundSlot = i;
                        break;
                }
        }

        // 配列末尾に空き枠があれば拡張
        if (nFoundSlot < 0 && nCurSize < nMaxSlot) {
                nFoundSlot = nCurSize;
        }

        if (nFoundSlot < 0) {
                pCharLib->Leave();
                response.statusLine = "HTTP/1.1 409 Conflict";
                response.SetJsonBody("{\"error\":\"item_slots_full\"}");
                return;
        }

        // スロットに書き込む（配列拡張が必要な場合）
        if (nFoundSlot >= nCurSize) {
                // 0 埋めで拡張してから末尾に追加
                while (pChar->m_adwItemID.GetSize() < nFoundSlot) {
                        pChar->m_adwItemID.Add(0);
                }
                pChar->m_adwItemID.Add(static_cast<DWORD>(nItemId));
        } else {
                pChar->m_adwItemID.SetAt(nFoundSlot, static_cast<DWORD>(nItemId));
        }

        // CInfoCharBase* は m_bChgInfo を持たないが、実体は CInfoCharSvr* なのでキャストして設定
        {
                CInfoCharSvr *pSvr = static_cast<CInfoCharSvr *>(pChar);
                if (pSvr != NULL) { pSvr->m_bChgInfo = TRUE; }
        }

        std::ostringstream oss;
        oss << "{\"slot\":" << nFoundSlot << ",\"itemId\":" << nItemId << '}';
        std::string json = oss.str();

        pCharLib->Leave();

        response.statusLine = "HTTP/1.1 201 Created";
        response.SetJsonBody(json);
}

// ---------------------------------------------------------------------------
// DELETE /api/characters/{charId}/items/{slot}
// ---------------------------------------------------------------------------

void CCharacterItemHandler::HandleDeleteItem(const HttpRequest & /*request*/, HttpResponse &response, int nCharId, int nSlot)
{
        CLibInfoCharSvr *pCharLib = m_pMgrData->GetLibInfoChar();
        if (pCharLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        pCharLib->Enter();

        CInfoCharBase *pChar = FindChar(pCharLib, nCharId);
        if (pChar == NULL) {
                pCharLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        if (nSlot >= pChar->m_adwItemID.GetSize() || pChar->m_adwItemID.GetAt(nSlot) == 0) {
                pCharLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"slot_not_found\"}");
                return;
        }

        // スロットを 0 クリア（空きスロットとして扱う）
        pChar->m_adwItemID.SetAt(nSlot, 0);
        // CInfoCharBase* は m_bChgInfo を持たないが、実体は CInfoCharSvr* なのでキャストして設定
        {
                CInfoCharSvr *pSvr = static_cast<CInfoCharSvr *>(pChar);
                if (pSvr != NULL) { pSvr->m_bChgInfo = TRUE; }
        }

        pCharLib->Leave();

        response.statusLine = "HTTP/1.1 204 No Content";
        response.body = "";
        response.SetHeader("Content-Length", "0");
}

// ---------------------------------------------------------------------------
// GET /api/characters/{charId}/skills
// ---------------------------------------------------------------------------

void CCharacterItemHandler::HandleGetSkills(const HttpRequest & /*request*/, HttpResponse &response, int nCharId)
{
        CLibInfoCharSvr *pCharLib = m_pMgrData->GetLibInfoChar();
        if (pCharLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        pCharLib->Enter();

        CInfoCharBase *pChar = FindChar(pCharLib, nCharId);
        if (pChar == NULL) {
                pCharLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        std::string json = BuildSkillListJson(pChar);
        pCharLib->Leave();

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(json);
}

// ---------------------------------------------------------------------------
// POST /api/characters/{charId}/skills
// body: { "skillId": <number> }
// 空きスロット（値 0）を先頭から探して挿入。満杯なら 409。
// ---------------------------------------------------------------------------

void CCharacterItemHandler::HandlePostSkill(const HttpRequest &request, HttpResponse &response, int nCharId)
{
        int nSkillId = 0;
        if (!JsonUtils::TryGetInt(request.body, "skillId", nSkillId) || nSkillId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"invalid_skillId\"}");
                return;
        }

        CLibInfoCharSvr *pCharLib = m_pMgrData->GetLibInfoChar();
        if (pCharLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        pCharLib->Enter();

        CInfoCharBase *pChar = FindChar(pCharLib, nCharId);
        if (pChar == NULL) {
                pCharLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        // 空きスロットを先頭から探す
        int nFoundSlot = -1;
        int nCurSize = pChar->m_adwSkillID.GetSize();

        for (int i = 0; i < nCurSize; ++i) {
                if (pChar->m_adwSkillID.GetAt(i) == 0) {
                        nFoundSlot = i;
                        break;
                }
        }

        // 配列末尾に空き枠があれば拡張
        if (nFoundSlot < 0 && nCurSize < MAX_CHAR_SKILL_SLOTS) {
                nFoundSlot = nCurSize;
        }

        if (nFoundSlot < 0) {
                pCharLib->Leave();
                response.statusLine = "HTTP/1.1 409 Conflict";
                response.SetJsonBody("{\"error\":\"skill_slots_full\"}");
                return;
        }

        // スロットに書き込む（配列拡張が必要な場合）
        if (nFoundSlot >= nCurSize) {
                while (pChar->m_adwSkillID.GetSize() < nFoundSlot) {
                        pChar->m_adwSkillID.Add(0);
                }
                pChar->m_adwSkillID.Add(static_cast<DWORD>(nSkillId));
        } else {
                pChar->m_adwSkillID.SetAt(nFoundSlot, static_cast<DWORD>(nSkillId));
        }

        // CInfoCharBase* は m_bChgInfo を持たないが、実体は CInfoCharSvr* なのでキャストして設定
        {
                CInfoCharSvr *pSvr = static_cast<CInfoCharSvr *>(pChar);
                if (pSvr != NULL) { pSvr->m_bChgInfo = TRUE; }
        }

        std::ostringstream oss;
        oss << "{\"slot\":" << nFoundSlot << ",\"skillId\":" << nSkillId << '}';
        std::string json = oss.str();

        pCharLib->Leave();

        response.statusLine = "HTTP/1.1 201 Created";
        response.SetJsonBody(json);
}

// ---------------------------------------------------------------------------
// DELETE /api/characters/{charId}/skills/{slot}
// ---------------------------------------------------------------------------

void CCharacterItemHandler::HandleDeleteSkill(const HttpRequest & /*request*/, HttpResponse &response, int nCharId, int nSlot)
{
        CLibInfoCharSvr *pCharLib = m_pMgrData->GetLibInfoChar();
        if (pCharLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        pCharLib->Enter();

        CInfoCharBase *pChar = FindChar(pCharLib, nCharId);
        if (pChar == NULL) {
                pCharLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        if (nSlot >= pChar->m_adwSkillID.GetSize() || pChar->m_adwSkillID.GetAt(nSlot) == 0) {
                pCharLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"slot_not_found\"}");
                return;
        }

        // スロットを 0 クリア（空きスロットとして扱う）
        pChar->m_adwSkillID.SetAt(nSlot, 0);
        // CInfoCharBase* は m_bChgInfo を持たないが、実体は CInfoCharSvr* なのでキャストして設定
        {
                CInfoCharSvr *pSvr = static_cast<CInfoCharSvr *>(pChar);
                if (pSvr != NULL) { pSvr->m_bChgInfo = TRUE; }
        }

        pCharLib->Leave();

        response.statusLine = "HTTP/1.1 204 No Content";
        response.body = "";
        response.SetHeader("Content-Length", "0");
}
