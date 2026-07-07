#include "StdAfx.h"
#include "CharacterItemHandler.h"

#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

#include "Web/AuthProvider.h"
#include "Web/JsonUtils.h"
#include "MgrData.h"
#include "LibInfo/LibInfoCharSvr.h"
#include "Info/InfoCharBase.h"
#include "Info/InfoCharSvr.h"
#include "LibInfo/LibInfoAccount.h"
#include "Info/InfoAccount.h"

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

        const std::string &method = request.method;

        // POST /api/characters/npc — NPC 新規追加
        // /api/characters/npc はスラッシュが 1 つしかないため ExtractCharItemPath が
        // false を返す。その前に特別処理として捕捉する。
        {
                const std::string npcPath = "/api/characters/npc";
                // クエリ文字列を除いたパスと比較する
                std::string pathNoQuery = request.path;
                size_t nQ = pathNoQuery.find('?');
                if (nQ != std::string::npos) {
                        pathNoQuery = pathNoQuery.substr(0, nQ);
                }
                if (pathNoQuery == npcPath) {
                        if (method == "POST") {
                                HandlePostNpc(request, response);
                        } else {
                                response.statusLine = "HTTP/1.1 405 Method Not Allowed";
                                response.SetJsonBody("{\"error\":\"method_not_allowed\"}");
                        }
                        return;
                }
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
        } else if (sub == "account") {
                // GET /api/characters/{charId}/account のみ受け付ける
                // PUT はキャラクター更新ハンドラ（CCharacterUpdateHandler）で処理する
                if (method == "GET") {
                        HandleGetAccount(request, response, nCharId);
                } else {
                        response.statusLine = "HTTP/1.1 405 Method Not Allowed";
                        response.SetJsonBody("{\"error\":\"method_not_allowed\"}");
                }
        } else {
                // items/skills/account 以外のサブリソースはフォールバックに委譲
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

// ---------------------------------------------------------------------------
// GET /api/characters/{charId}/account
// キャラクターに紐付くアカウント情報を返す
// Response: { accountId, loginId, ip, mac }
// アカウントが紐付いていない（accountId=0）場合は accountId=0 で他フィールドを空文字返す
// ---------------------------------------------------------------------------

void CCharacterItemHandler::HandleGetAccount(const HttpRequest & /*request*/, HttpResponse &response, int nCharId)
{
        CLibInfoCharSvr *pCharLib = m_pMgrData->GetLibInfoChar();
        if (pCharLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        // キャラクターからアカウントIDを取得する
        pCharLib->Enter();
        CInfoCharBase *pChar = FindChar(pCharLib, nCharId);
        if (pChar == NULL) {
                pCharLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }
        DWORD dwAccountId = pChar->m_dwAccountID;
        pCharLib->Leave();

        // アカウントライブラリを取得する（pCharLib のロックとネストさせない）
        CLibInfoAccount *pAccountLib = m_pMgrData->GetLibInfoAccount();

        // m_dwAccountID はログイン中のみ設定されるため、0 の場合はアカウント側から逆引きする
        if (dwAccountId == 0 && pAccountLib != NULL) {
                pAccountLib->Enter();
                dwAccountId = pAccountLib->FindAccountIDByCharID(static_cast<DWORD>(nCharId));
                pAccountLib->Leave();
        }

        // 逆引きしてもアカウントIDが 0 の場合はアカウント情報なし
        if (dwAccountId == 0) {
                response.statusLine = "HTTP/1.1 200 OK";
                response.SetJsonBody("{\"accountId\":0,\"loginId\":\"\",\"password\":\"\",\"ip\":\"\",\"mac\":\"\"}");
                return;
        }

        // アカウントライブラリからアカウント情報を取得する
        if (pAccountLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        pAccountLib->Enter();
        PCInfoAccount pAcc = pAccountLib->GetPtr(dwAccountId);
        if (pAcc == NULL) {
                pAccountLib->Leave();
                // アカウントIDはあるがアカウントレコードが見つからない場合
                response.statusLine = "HTTP/1.1 200 OK";
                std::ostringstream ossNone;
                ossNone << "{\"accountId\":" << dwAccountId << ",\"loginId\":\"\",\"password\":\"\",\"ip\":\"\",\"mac\":\"\"}";
                response.SetJsonBody(ossNone.str());
                return;
        }

        // IPアドレスを文字列に変換する（IPv4 ドット区切り）
        std::ostringstream ossIp;
        DWORD dwIp = pAcc->m_dwIP;
        ossIp << ((dwIp >> 24) & 0xFF) << '.'
              << ((dwIp >> 16) & 0xFF) << '.'
              << ((dwIp >>  8) & 0xFF) << '.'
              << (dwIp & 0xFF);

        // アカウント名（ログインID）を取得する
        std::string loginId;
        {
                LPCSTR pszAcc = static_cast<LPCSTR>(pAcc->m_strAccount);
                if (pszAcc != NULL) {
                        loginId = pszAcc;
                }
        }

        // パスワードを取得する（UTF-8 で保持されているため GetUtf8Pointer を使う）
        std::string password;
        {
                LPCSTR pszPassword = pAcc->m_strPassword.GetUtf8Pointer();
                if (pszPassword != NULL) {
                        password = pszPassword;
                }
        }

        // MAC アドレス（ログイン時に記録された最新値）を取得する
        std::string macAddr;
        {
                LPCSTR pszMac = static_cast<LPCSTR>(pAcc->m_strLastMacAddr);
                if (pszMac != NULL) {
                        macAddr = pszMac;
                }
        }

        std::ostringstream oss;
        oss << '{';
        oss << "\"accountId\":" << dwAccountId << ',';
        oss << "\"loginId\":\"" << JsonUtils::Escape(loginId) << "\",";
        oss << "\"password\":\"" << JsonUtils::Escape(password) << "\",";
        oss << "\"ip\":\"" << JsonUtils::Escape(ossIp.str()) << "\",";
        oss << "\"mac\":\"" << JsonUtils::Escape(macAddr) << '"';
        oss << '}';

        pAccountLib->Leave();

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// POST /api/characters/npc
// body: {
//   charName  (必須),
//   familyId  (必須),
//   mapId     (必須),
//   x         (必須),
//   y         (必須),
//   moveType  (任意, デフォルト=CHARMOVETYPE_STAND=1),
//   sex       (任意, デフォルト=0),
//   grpIdNpc     (任意, デフォルト=0),
//   grpIdInitNpc (任意, デフォルト=0),
//   motionTypeId (任意, デフォルト=0),
//   block     (任意, デフォルト=false),
//   push      (任意, デフォルト=false)
// }
// 成功時: 201 Created + 作成後キャラの基本 JSON
// ---------------------------------------------------------------------------

// UTF-8 文字列を内部文字コードに変換して CmyString へ代入するローカルヘルパー
// （CharacterItemHandler 内では SetStringFromUtf8 が未定義のため独自実装）
static bool SetCharNameFromUtf8(CmyString &out, const std::string &utf8)
{
        if (utf8.empty()) {
                out = _T("");
                return false;
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

// CmyString → UTF-8 std::string（ローカルヘルパー）
static std::string CharNameToUtf8(const CmyString &value)
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
        return std::string(psz);
#endif
}

void CCharacterItemHandler::HandlePostNpc(const HttpRequest &request, HttpResponse &response)
{
        // --- 必須パラメータを取得する ---
        std::string charName;
        if (!JsonUtils::TryGetString(request.body, "charName", charName) || charName.empty()) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_charName\"}");
                return;
        }

        int nFamilyId = 0;
        if (!JsonUtils::TryGetInt(request.body, "familyId", nFamilyId) || nFamilyId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_familyId\"}");
                return;
        }

        int nMapId = 0;
        if (!JsonUtils::TryGetInt(request.body, "mapId", nMapId) || nMapId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_mapId\"}");
                return;
        }

        int nX = 0;
        if (!JsonUtils::TryGetInt(request.body, "x", nX)) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_x\"}");
                return;
        }

        int nY = 0;
        if (!JsonUtils::TryGetInt(request.body, "y", nY)) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_y\"}");
                return;
        }

        // --- 任意パラメータを取得する ---
        // moveType: デフォルト CHARMOVETYPE_STAND (=1)
        int nMoveType = CHARMOVETYPE_STAND;
        {
                int tmp = 0;
                if (JsonUtils::TryGetInt(request.body, "moveType", tmp) &&
                    tmp >= 0 && tmp < CHARMOVETYPE_MAX) {
                        nMoveType = tmp;
                }
        }

        // sex: デフォルト 0
        int nSex = 0;
        {
                int tmp = 0;
                if (JsonUtils::TryGetInt(request.body, "sex", tmp)) {
                        nSex = tmp;
                }
        }

        // grpIdNpc: NPC 画像 ID (m_wGrpIDNPC)
        int nGrpIdNpc = 0;
        {
                int tmp = 0;
                if (JsonUtils::TryGetInt(request.body, "grpIdNpc", tmp) && tmp >= 0) {
                        nGrpIdNpc = tmp;
                }
        }

        // grpIdInitNpc: 初期 NPC 画像 ID (m_wGrpIDInitNPC)
        int nGrpIdInitNpc = 0;
        {
                int tmp = 0;
                if (JsonUtils::TryGetInt(request.body, "grpIdInitNpc", tmp) && tmp >= 0) {
                        nGrpIdInitNpc = tmp;
                }
        }

        // motionTypeId: モーション種別 ID (m_dwMotionTypeID)
        int nMotionTypeId = 0;
        {
                int tmp = 0;
                if (JsonUtils::TryGetInt(request.body, "motionTypeId", tmp) && tmp >= 0) {
                        nMotionTypeId = tmp;
                }
        }

        // block / push (任意 bool): JSON に "block":true / "push":true で渡す
        // TryGetInt で 0/1 を受け取る形式
        bool bBlock = false;
        {
                int tmp = 0;
                if (JsonUtils::TryGetInt(request.body, "block", tmp)) {
                        bBlock = (tmp != 0);
                }
        }
        bool bPush = false;
        {
                int tmp = 0;
                if (JsonUtils::TryGetInt(request.body, "push", tmp)) {
                        bPush = (tmp != 0);
                }
        }

        // --- Lib を取得する ---
        CLibInfoCharSvr *pCharLib = m_pMgrData->GetLibInfoChar();
        if (pCharLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        // --- テンポラリ CInfoCharSvr に値を設定してから AddNPC を呼ぶ ---
        // AddNPC の内部では GetNew(moveType) → Copy → Add → LogIn の順で処理される。
        // Copy で全フィールドが複製されるので、テンポラリに設定した値がそのまま引き継がれる。
        // （旧 DlgAdminCharAddNPC::OnSend の実装に倣う）
        CInfoCharSvr infoTmp;
        infoTmp.m_nMoveType      = nMoveType;
        infoTmp.m_dwMapID        = static_cast<DWORD>(nMapId);
        // 旧クライアントは m_nMapX = posX * 2 で送信していた。
        // Web API では生の座標 (x, y) をそのまま受け取り、AddNPC 側でそのまま使う。
        infoTmp.m_nMapX          = nX;
        infoTmp.m_nMapY          = nY;
        infoTmp.m_wFamilyID      = static_cast<WORD>(nFamilyId);
        infoTmp.m_nSex           = nSex;
        infoTmp.m_wGrpIDNPC      = static_cast<WORD>(nGrpIdNpc);
        infoTmp.m_wGrpIDInitNPC  = static_cast<WORD>(nGrpIdInitNpc);
        infoTmp.m_dwMotionTypeID = static_cast<DWORD>(nMotionTypeId);
        infoTmp.m_bBlock         = bBlock ? TRUE : FALSE;
        infoTmp.m_bPush          = bPush  ? TRUE : FALSE;

        // キャラ名を内部文字コードに変換してセット
        if (!SetCharNameFromUtf8(infoTmp.m_strCharName, charName)) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"invalid_charName_encoding\"}");
                return;
        }

        // --- AddNPC を呼んで追加する ---
        pCharLib->Enter();
        CInfoCharBase *pAdded = pCharLib->AddNPC(&infoTmp);
        if (pAdded == NULL) {
                pCharLib->Leave();
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"add_npc_failed\"}");
                return;
        }

        // 追加直後の charId など必要な値を読み取っておく
        DWORD dwNewCharId = pAdded->m_dwCharID;
        DWORD dwNewMapId  = pAdded->m_dwMapID;
        int   nNewX       = pAdded->m_nMapX;
        int   nNewY       = pAdded->m_nMapY;
        std::string newCharName = CharNameToUtf8(pAdded->m_strCharName);
        WORD  wNewFamilyId      = pAdded->m_wFamilyID;
        int   nNewMoveType      = pAdded->m_nMoveType;

        pCharLib->Leave();

        // --- 201 Created + 追加したキャラの基本情報を返す ---
        std::ostringstream oss;
        oss << '{';
        oss << "\"charId\":"    << dwNewCharId << ',';
        oss << "\"charName\":\"" << JsonUtils::Escape(newCharName) << "\",";
        oss << "\"mapId\":"     << dwNewMapId << ',';
        oss << "\"x\":"         << nNewX << ',';
        oss << "\"y\":"         << nNewY << ',';
        oss << "\"familyId\":"  << wNewFamilyId << ',';
        oss << "\"moveType\":"  << nNewMoveType << ',';
        oss << "\"isNpc\":true";
        oss << '}';

        response.statusLine = "HTTP/1.1 201 Created";
        response.SetJsonBody(oss.str());
}
