#include "StdAfx.h"
#include "AccountAdminHandler.h"

#include <ctime>
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>

#include "Web/AuthProvider.h"
#include "Web/JsonUtils.h"
#include "MgrData.h"
#include "LibInfo/LibInfoAccount.h"
#include "LibInfo/LibInfoCharSvr.h"
#include "Info/InfoAccount.h"
#include "Info/InfoCharBase.h"
#include "Account/AccountAdminStore.h"
#include "Account/AccountAuthStore.h"

namespace
{

// URL パスから accountId とサブリソース名を抽出する
// /api/accounts/123        → accountId=123, subResource=""
// /api/accounts/123/trash  → accountId=123, subResource="trash"
// 取得できなければ false を返す
bool ExtractAccountIdAndSub(const std::string &path, unsigned int &outAccountId, std::string &outSub)
{
        const std::string prefix = "/api/accounts/";
        if (path.size() <= prefix.size()) {
                return false;
        }

        std::string rest = path.substr(prefix.size());
        size_t nQuery = rest.find('?');
        if (nQuery != std::string::npos) {
                rest = rest.substr(0, nQuery);
        }

        size_t nSlash = rest.find('/');
        std::string idStr;
        if (nSlash != std::string::npos) {
                idStr  = rest.substr(0, nSlash);
                outSub = rest.substr(nSlash + 1);
        } else {
                idStr  = rest;
                outSub = "";
        }

        if (idStr.empty()) {
                return false;
        }

        char *pEnd = NULL;
        unsigned long val = std::strtoul(idStr.c_str(), &pEnd, 10);
        if (pEnd == NULL || *pEnd != '\0' || val == 0) {
                return false;
        }

        outAccountId = static_cast<unsigned int>(val);
        return true;
}

// 破壊的操作(trash/ban/完全削除)に共通の安全ガード。
// 実際に admin(AccountID=1) を BAN したところ、その場で管理API全体が401を
// 返すようになり管理画面から解除できなくなった事故があった(SQLite直編集でのみ
// 復旧可能だった)。同じ事故を防ぐため、以下は409で拒否する。
//   1. 本人アカウントを対象にした操作(自分自身を締め出せてしまう)
//   2. 管理者権限(m_nAdminLevel != ADMINLEVEL_NONE)を持つアカウントを対象にした操作
// 解除系(DELETE .../trash, DELETE .../ban)は復旧手段を塞がないよう対象外。
// 管理者アカウントを整理したい場合は、先に「管理者ロール設定」画面でロールを
// 外してから trash/ban/完全削除を行う運用とする。
bool CheckMutationGuard(CLibInfoAccount *pAccountLib, unsigned int dwAccountID, unsigned int dwActorAccountID, HttpResponse &response)
{
        if ((dwActorAccountID != 0) && (dwActorAccountID == dwAccountID)) {
                response.statusLine = "HTTP/1.1 409 Conflict";
                response.SetJsonBody("{\"error\":\"cannot_target_self\"}");
                return false;
        }

        pAccountLib->Enter();
        PCInfoAccount pAcc = pAccountLib->GetPtr(static_cast<DWORD>(dwAccountID));
        bool bIsAdmin = (pAcc != NULL) && (pAcc->m_nAdminLevel != ADMINLEVEL_NONE);
        pAccountLib->Leave();

        if (bIsAdmin) {
                response.statusLine = "HTTP/1.1 409 Conflict";
                response.SetJsonBody("{\"error\":\"cannot_target_admin\"}");
                return false;
        }

        return true;
}

} // namespace

CAccountAdminHandler::CAccountAdminHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CAccountAdminHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        AuthProvider::AuthContext authContext;
        AuthProvider::AuthStatus authStatus = AuthProvider::Authenticate(request, m_pMgrData, authContext);
        if (authStatus == AuthProvider::AuthStatusBackendUnavailable) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }
        if (authStatus != AuthProvider::AuthStatusOk) {
                response.statusLine = "HTTP/1.1 401 Unauthorized";
                response.SetJsonBody("{\"error\":\"unauthorized\"}");
                return;
        }
        if (!AuthProvider::HasRole(authContext, "SERVER_ADMIN")) {
                response.statusLine = "HTTP/1.1 403 Forbidden";
                response.SetJsonBody(AuthProvider::BuildForbiddenBody("SERVER_ADMIN"));
                return;
        }

        if (m_pMgrData == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        unsigned int dwAccountID = 0;
        std::string subResource;
        if (!ExtractAccountIdAndSub(request.path, dwAccountID, subResource)) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"invalid_account_id\"}");
                return;
        }

        unsigned int dwActorAccountID = 0;
        {
                char *pEnd = NULL;
                unsigned long val = std::strtoul(authContext.accountId.c_str(), &pEnd, 10);
                if ((pEnd != NULL) && (*pEnd == '\0')) {
                        dwActorAccountID = static_cast<unsigned int>(val);
                }
        }

        if (subResource == "trash") {
                if (request.method == "POST") {
                        HandleTrash(request, response, dwAccountID, dwActorAccountID);
                } else if (request.method == "DELETE") {
                        HandleUntrash(request, response, dwAccountID);
                } else {
                        response.statusLine = "HTTP/1.1 404 Not Found";
                        response.SetJsonBody("{\"error\":\"not_found\"}");
                }
        } else if (subResource == "ban") {
                if (request.method == "POST") {
                        HandleBan(request, response, dwAccountID, dwActorAccountID);
                } else if (request.method == "DELETE") {
                        HandleUnban(request, response, dwAccountID);
                } else {
                        response.statusLine = "HTTP/1.1 404 Not Found";
                        response.SetJsonBody("{\"error\":\"not_found\"}");
                }
        } else if (subResource.empty()) {
                if (request.method == "DELETE") {
                        HandlePurge(request, response, dwAccountID, dwActorAccountID);
                } else {
                        response.statusLine = "HTTP/1.1 404 Not Found";
                        response.SetJsonBody("{\"error\":\"not_found\"}");
                }
        } else {
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
        }
}

// ---------------------------------------------------------------------------
// ゴミ箱へ  POST /api/accounts/{id}/trash
// body: { "reason"?: <string> }
// ---------------------------------------------------------------------------

void CAccountAdminHandler::HandleTrash(const HttpRequest &request, HttpResponse &response, unsigned int dwAccountID, unsigned int dwActorAccountID)
{
        CLibInfoAccount *pAccountLib = m_pMgrData->GetLibInfoAccount();
        if (pAccountLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        if (!CheckMutationGuard(pAccountLib, dwAccountID, dwActorAccountID, response)) {
                return;
        }

        std::string strReason;
        JsonUtils::TryGetString(request.body, "reason", strReason);

        // BAN中のアカウントをゴミ箱に入れる場合、既存行の PrevDisable
        // (BAN前の本来のm_bDisable)を保つ。ここで上書きすると、ゴミ箱から
        // 復帰した際にBAN前の状態(=拒否されていない)に戻ってしまい、
        // BANによる拒否が消えてしまう。
        CAccountAdminStore AdminStore;
        AccountAdminRow existingRow;
        bool bHasExistingRow = AdminStore.Get(dwAccountID, existingRow);

        pAccountLib->Enter();
        PCInfoAccount pAcc = pAccountLib->GetPtr(static_cast<DWORD>(dwAccountID));
        if (pAcc == NULL) {
                pAccountLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        int nPrevDisableForRollback = pAcc->m_bDisable ? 1 : 0;
        int nPrevDisableToStore = bHasExistingRow ? existingRow.nPrevDisable : nPrevDisableForRollback;

        // BAN中のアカウントをゴミ箱に入れる場合、"ゴミ箱に入れる直前の状態"を
        // PrevStatus/PrevReason に控える。これが無いと、ゴミ箱から復帰した時に
        // BANだった事実(と理由)が失われ、BANが解除されたのと同じ結果になる。
        bool bWasBanned = bHasExistingRow && existingRow.strStatus == "banned";
        std::string strPrevStatusToStore = bWasBanned ? "banned" : std::string();
        std::string strPrevReasonToStore = bWasBanned ? existingRow.strReason : std::string();

        pAcc->m_bDisable = TRUE;
        pAccountLib->Leave();

        time_t currentTime = time(NULL);
        if (currentTime < 0) {
                currentTime = 0;
        }

        if (!AdminStore.SetStatus(dwAccountID, "trashed", strReason,
                static_cast<long>(currentTime), dwActorAccountID, nPrevDisableToStore,
                strPrevStatusToStore, strPrevReasonToStore)) {
                // DB更新に失敗した場合はメモリ上のフラグも戻す
                pAccountLib->Enter();
                PCInfoAccount pAccRollback = pAccountLib->GetPtr(static_cast<DWORD>(dwAccountID));
                if (pAccRollback != NULL) {
                        pAccRollback->m_bDisable = (nPrevDisableForRollback != 0) ? TRUE : FALSE;
                }
                pAccountLib->Leave();

                response.statusLine = "HTTP/1.1 500 Internal Server Error";
                response.SetJsonBody("{\"error\":\"admin_store_failed\"}");
                return;
        }

        response.statusLine = "HTTP/1.1 204 No Content";
        response.body = "";
        response.SetHeader("Content-Length", "0");
}

// ---------------------------------------------------------------------------
// ゴミ箱から復帰  DELETE /api/accounts/{id}/trash
// ---------------------------------------------------------------------------

void CAccountAdminHandler::HandleUntrash(const HttpRequest &request, HttpResponse &response, unsigned int dwAccountID)
{
        (void)request;

        CLibInfoAccount *pAccountLib = m_pMgrData->GetLibInfoAccount();
        if (pAccountLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        CAccountAdminStore AdminStore;
        AccountAdminRow adminRow;
        if (!AdminStore.Get(dwAccountID, adminRow)) {
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_trashed\"}");
                return;
        }

        pAccountLib->Enter();
        PCInfoAccount pAcc = pAccountLib->GetPtr(static_cast<DWORD>(dwAccountID));
        if (pAcc == NULL) {
                pAccountLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        bool bRestoreToBanned = (adminRow.strPrevStatus == "banned");

        if (bRestoreToBanned) {
                // ゴミ箱に入れる直前がBANだった場合は、ゴミ箱に入れる前の状態
                // (=BAN中)に戻す。行は消さず Status を "banned" に戻し、
                // Reason も控えていた PrevReason に戻す。m_bDisable は
                // トラッシュ時からずっとTRUEのまま(=拒否継続)。
                pAccountLib->Leave();

                time_t currentTime = time(NULL);
                if (currentTime < 0) {
                        currentTime = 0;
                }

                if (!AdminStore.SetStatus(dwAccountID, "banned", adminRow.strPrevReason,
                        static_cast<long>(currentTime), adminRow.dwActorAccountID, adminRow.nPrevDisable)) {
                        response.statusLine = "HTTP/1.1 500 Internal Server Error";
                        response.SetJsonBody("{\"error\":\"admin_store_failed\"}");
                        return;
                }
        } else {
                pAcc->m_bDisable = (adminRow.nPrevDisable != 0) ? TRUE : FALSE;
                pAccountLib->Leave();

                if (!AdminStore.DeleteRow(dwAccountID)) {
                        response.statusLine = "HTTP/1.1 500 Internal Server Error";
                        response.SetJsonBody("{\"error\":\"admin_store_failed\"}");
                        return;
                }
        }

        response.statusLine = "HTTP/1.1 204 No Content";
        response.body = "";
        response.SetHeader("Content-Length", "0");
}

// ---------------------------------------------------------------------------
// 完全削除  DELETE /api/accounts/{id}
// 事前条件: ゴミ箱に入っていること(誤操作防止) / 接続中でないこと
// ---------------------------------------------------------------------------

void CAccountAdminHandler::HandlePurge(const HttpRequest &request, HttpResponse &response, unsigned int dwAccountID, unsigned int dwActorAccountID)
{
        (void)request;

        CLibInfoAccount *pAccountLib = m_pMgrData->GetLibInfoAccount();
        if (pAccountLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }
        CLibInfoCharSvr *pCharLib = m_pMgrData->GetLibInfoChar();
        if (pCharLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        if (!CheckMutationGuard(pAccountLib, dwAccountID, dwActorAccountID, response)) {
                return;
        }

        // 事前条件1: ゴミ箱に入っていること(誤操作防止。いきなり消させない)
        CAccountAdminStore AdminStore;
        AccountAdminRow adminRow;
        if (!AdminStore.Get(dwAccountID, adminRow)) {
                response.statusLine = "HTTP/1.1 409 Conflict";
                response.SetJsonBody("{\"error\":\"not_trashed\"}");
                return;
        }

        // アカウント側の情報を読む(接続中チェック・削除対象キャラID収集)。
        // pCharLib とネストしないよう、ここで account ロックは一度閉じる。
        DWORD dwCurrentCharID = 0;
        std::vector<DWORD> charIDsToDelete;
        {
                pAccountLib->Enter();
                PCInfoAccount pAcc = pAccountLib->GetPtr(static_cast<DWORD>(dwAccountID));
                if (pAcc == NULL) {
                        pAccountLib->Leave();
                        response.statusLine = "HTTP/1.1 404 Not Found";
                        response.SetJsonBody("{\"error\":\"not_found\"}");
                        return;
                }

                // 事前条件2: 接続中でないこと
                if (pAcc->m_dwSessionID != 0) {
                        pAccountLib->Leave();
                        response.statusLine = "HTTP/1.1 409 Conflict";
                        response.SetJsonBody("{\"error\":\"account_online\"}");
                        return;
                }

                dwCurrentCharID = pAcc->m_dwCharID;
                int nCharTableCount = static_cast<int>(pAcc->m_adwCharID.size());
                for (int i = 0; i < nCharTableCount; ++i) {
                        charIDsToDelete.push_back(pAcc->m_adwCharID[static_cast<size_t>(i)]);
                }
                pAccountLib->Leave();
        }

        if ((dwCurrentCharID != 0)) {
                bool bAlreadyListed = false;
                for (size_t i = 0; i < charIDsToDelete.size(); ++i) {
                        if (charIDsToDelete[i] == dwCurrentCharID) {
                                bAlreadyListed = true;
                                break;
                        }
                }
                if (!bAlreadyListed) {
                        charIDsToDelete.push_back(dwCurrentCharID);
                }
        }

        // 手順1: 全キャラを削除する(キャラ削除のWeb APIは存在しないので直接呼ぶ)
        {
                pCharLib->Enter();
                for (size_t i = 0; i < charIDsToDelete.size(); ++i) {
                        if (charIDsToDelete[i] != 0) {
                                pCharLib->Delete(charIDsToDelete[i]);
                        }
                }
                pCharLib->Leave();
        }

        // 手順2: sys_account_code / sys_account_device の該当行を削除
        {
                CAccountAuthStore AuthStore;
                AuthStore.DeleteCodeForAccount(dwAccountID);
                AuthStore.DeleteAllDevicesForAccount(dwAccountID);
        }

        // 手順3: アカウント本体を削除
        {
                pAccountLib->Enter();
                pAccountLib->Delete(static_cast<DWORD>(dwAccountID));
                pAccountLib->Leave();
        }

        // 手順4: sys_account_admin の行を削除
        AdminStore.DeleteRow(dwAccountID);

        response.statusLine = "HTTP/1.1 204 No Content";
        response.body = "";
        response.SetHeader("Content-Length", "0");
}

// ---------------------------------------------------------------------------
// BAN(ログイン拒否)  POST /api/accounts/{id}/ban
// body: { "reason"?: <string> }
// キャラ側の m_dwAccountID はログイン中しか設定されないため、オフラインの
// キャラに対する「ログイン拒否」(PUT /api/characters/{id}/disabled)が効かない
// 問題を、アカウント側から塞ぐためのAPI。
// ゴミ箱とBANは排他の状態として扱う(sys_account_admin.Statusは1つしか持てない)。
// ---------------------------------------------------------------------------

void CAccountAdminHandler::HandleBan(const HttpRequest &request, HttpResponse &response, unsigned int dwAccountID, unsigned int dwActorAccountID)
{
        CLibInfoAccount *pAccountLib = m_pMgrData->GetLibInfoAccount();
        if (pAccountLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        if (!CheckMutationGuard(pAccountLib, dwAccountID, dwActorAccountID, response)) {
                return;
        }

        std::string strReason;
        JsonUtils::TryGetString(request.body, "reason", strReason);

        CAccountAdminStore AdminStore;
        AccountAdminRow existingRow;
        bool bHasExistingRow = AdminStore.Get(dwAccountID, existingRow);

        // 既にゴミ箱に入っているアカウントは、既に拒否されており状態も1つしか
        // 持てないため二重にBANする意味が無い。409で弾く。
        if (bHasExistingRow && existingRow.strStatus == "trashed") {
                response.statusLine = "HTTP/1.1 409 Conflict";
                response.SetJsonBody("{\"error\":\"already_trashed\"}");
                return;
        }

        pAccountLib->Enter();
        PCInfoAccount pAcc = pAccountLib->GetPtr(static_cast<DWORD>(dwAccountID));
        if (pAcc == NULL) {
                pAccountLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        // 既にBAN中なら理由だけ更新する。PrevDisable(BAN前の本来の値)は
        // 上書きしない。
        int nPrevDisableForRollback = pAcc->m_bDisable ? 1 : 0;
        bool bAlreadyBanned = (bHasExistingRow && existingRow.strStatus == "banned");
        int nPrevDisableToStore = bAlreadyBanned ? existingRow.nPrevDisable : nPrevDisableForRollback;
        pAcc->m_bDisable = TRUE;
        pAccountLib->Leave();

        time_t currentTime = time(NULL);
        if (currentTime < 0) {
                currentTime = 0;
        }

        if (!AdminStore.SetStatus(dwAccountID, "banned", strReason,
                static_cast<long>(currentTime), dwActorAccountID, nPrevDisableToStore)) {
                // DB更新に失敗した場合はメモリ上のフラグも戻す
                pAccountLib->Enter();
                PCInfoAccount pAccRollback = pAccountLib->GetPtr(static_cast<DWORD>(dwAccountID));
                if (pAccRollback != NULL) {
                        pAccRollback->m_bDisable = (nPrevDisableForRollback != 0) ? TRUE : FALSE;
                }
                pAccountLib->Leave();

                response.statusLine = "HTTP/1.1 500 Internal Server Error";
                response.SetJsonBody("{\"error\":\"admin_store_failed\"}");
                return;
        }

        response.statusLine = "HTTP/1.1 204 No Content";
        response.body = "";
        response.SetHeader("Content-Length", "0");
}

// ---------------------------------------------------------------------------
// BAN解除  DELETE /api/accounts/{id}/ban
// ---------------------------------------------------------------------------

void CAccountAdminHandler::HandleUnban(const HttpRequest &request, HttpResponse &response, unsigned int dwAccountID)
{
        (void)request;

        CLibInfoAccount *pAccountLib = m_pMgrData->GetLibInfoAccount();
        if (pAccountLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        CAccountAdminStore AdminStore;
        AccountAdminRow adminRow;
        if (!AdminStore.Get(dwAccountID, adminRow) || adminRow.strStatus != "banned") {
                response.statusLine = "HTTP/1.1 409 Conflict";
                response.SetJsonBody("{\"error\":\"not_banned\"}");
                return;
        }

        pAccountLib->Enter();
        PCInfoAccount pAcc = pAccountLib->GetPtr(static_cast<DWORD>(dwAccountID));
        if (pAcc == NULL) {
                pAccountLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        pAcc->m_bDisable = (adminRow.nPrevDisable != 0) ? TRUE : FALSE;
        pAccountLib->Leave();

        if (!AdminStore.DeleteRow(dwAccountID)) {
                response.statusLine = "HTTP/1.1 500 Internal Server Error";
                response.SetJsonBody("{\"error\":\"admin_store_failed\"}");
                return;
        }

        response.statusLine = "HTTP/1.1 204 No Content";
        response.body = "";
        response.SetHeader("Content-Length", "0");
}
