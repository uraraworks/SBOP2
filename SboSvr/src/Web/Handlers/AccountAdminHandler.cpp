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
        } else if (subResource.empty()) {
                if (request.method == "DELETE") {
                        HandlePurge(request, response, dwAccountID);
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

        std::string strReason;
        JsonUtils::TryGetString(request.body, "reason", strReason);

        pAccountLib->Enter();
        PCInfoAccount pAcc = pAccountLib->GetPtr(static_cast<DWORD>(dwAccountID));
        if (pAcc == NULL) {
                pAccountLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        int nPrevDisable = pAcc->m_bDisable ? 1 : 0;
        pAcc->m_bDisable = TRUE;
        pAccountLib->Leave();

        time_t currentTime = time(NULL);
        if (currentTime < 0) {
                currentTime = 0;
        }

        CAccountAdminStore AdminStore;
        if (!AdminStore.SetStatus(dwAccountID, "trashed", strReason,
                static_cast<long>(currentTime), dwActorAccountID, nPrevDisable)) {
                // DB更新に失敗した場合はメモリ上のフラグも戻す
                pAccountLib->Enter();
                PCInfoAccount pAccRollback = pAccountLib->GetPtr(static_cast<DWORD>(dwAccountID));
                if (pAccRollback != NULL) {
                        pAccRollback->m_bDisable = (nPrevDisable != 0) ? TRUE : FALSE;
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

// ---------------------------------------------------------------------------
// 完全削除  DELETE /api/accounts/{id}
// 事前条件: ゴミ箱に入っていること(誤操作防止) / 接続中でないこと
// ---------------------------------------------------------------------------

void CAccountAdminHandler::HandlePurge(const HttpRequest &request, HttpResponse &response, unsigned int dwAccountID)
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
