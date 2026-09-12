/// @file ServerSessionsHandler.cpp
/// @brief 接続中プレイヤー一覧 API(GET /api/server/sessions) 実装ファイル
/// @copyright Copyright(C)URARA-works

#include "StdAfx.h"
#include "ServerSessionsHandler.h"

#include <algorithm>
#include <cstdio>
#include <ctime>

#include "Web/AuthProvider.h"
#include "MgrData.h"
#include "LibInfo/LibInfoCharSvr.h"
#include "Info/InfoCharBase.h"
#include "LibInfo/LibInfoAccount.h"
#include "Info/InfoAccount.h"
#include "../../Platform/SvrPlatform.h"

CServerSessionsHandler::CServerSessionsHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CServerSessionsHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        // 認証・権限のゲートは ServerInfoHandler(/api/server) と同じにする
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
        if (!AuthProvider::HasRole(authContext, "SERVER_VIEW")) {
                response.statusLine = "HTTP/1.1 403 Forbidden";
                response.SetJsonBody(AuthProvider::BuildForbiddenBody("SERVER_VIEW"));
                return;
        }

        if (m_pMgrData == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        std::vector<SboSessionRow> rows = CollectSessions();
        std::sort(rows.begin(), rows.end(), [](const SboSessionRow &a, const SboSessionRow &b) {
                return a.nAccountId < b.nAccountId;
        });

        std::time_t tNow;
        std::time(&tNow);

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(SessionsJsonBuilder::Build(rows, GetTimestampNow(), tNow));
}

// ---------------------------------------------------------------------------
// アカウント/キャラライブラリから接続中セッションのデータを集める
//
// CharacterListHandler.cpp と同様、pCharLib と pAccountLib のロックを
// ネストさせないため、pAccountLib で集めてから Leave() し、
// 改めて pCharLib を Enter() してキャラ情報を補完する。
// ---------------------------------------------------------------------------

std::vector<SboSessionRow> CServerSessionsHandler::CollectSessions() const
{
        std::vector<SboSessionRow> rows;

        CLibInfoAccount *pAccountLib = m_pMgrData->GetLibInfoAccount();
        if (pAccountLib == NULL) {
                return rows;
        }

        pAccountLib->Enter();
        int nAccCount = pAccountLib->GetCount();
        for (int i = 0; i < nAccCount; ++i) {
                PCInfoAccount pAcc = static_cast<PCInfoAccount>(pAccountLib->GetPtr(i));
                if ((pAcc == NULL) || (pAcc->m_dwSessionID == 0)) {
                        continue;
                }

                SboSessionRow row;
                row.nAccountId = pAcc->m_dwAccountID;
                row.strAccount = pAcc->m_strAccount.GetUtf8Pointer() != NULL
                        ? pAcc->m_strAccount.GetUtf8Pointer() : "";
                row.nSessionId = pAcc->m_dwSessionID;
                // CInfoAccount::m_dwCharID(使用中のキャラID)はサーバー側で設定されていない未使用フィールドのため
                // 依存しない。charId等はこの後 CLibInfoCharSvr のログイン中キャラ一覧から補完する。
                row.nCharId = 0;
                row.bHasCharName = false;
                row.bHasMapId = false;
                row.nMapId = 0;
                row.bHasPos = false;
                row.nX = 0;
                row.nY = 0;

                // m_dwIP はサーバー側では設定されていない(常に0)ため、値がある時だけ出す。
                // 将来 MainFrame 側で埋めるようになれば自動的に反映される。
                row.bHasIp = (pAcc->m_dwIP != 0);
                if (row.bHasIp) {
                        DWORD dwIp = pAcc->m_dwIP;
                        char szIp[32];
                        std::snprintf(szIp, sizeof(szIp), "%u.%u.%u.%u",
                                (unsigned int)((dwIp >> 24) & 0xFF), (unsigned int)((dwIp >> 16) & 0xFF),
                                (unsigned int)((dwIp >> 8) & 0xFF), (unsigned int)(dwIp & 0xFF));
                        row.strIp = szIp;
                }

                row.bHasLastKeepalive = (pAcc->m_dwLastKeepalive != 0);
                row.tLastKeepaliveEpoch = (std::time_t)pAcc->m_dwLastKeepalive;

                // lastLoginAt ではなく「今回のログイン時刻」(MainFrame.cpp でログアウト時に
                // now - m_dwTimeLastLogin を接続時間として使っている値)なので loginAt として返す。
                row.bHasLoginAt = (pAcc->m_dwTimeLastLogin != 0);
                row.tLoginAtEpoch = (std::time_t)pAcc->m_dwTimeLastLogin;

                rows.push_back(row);
        }
        pAccountLib->Leave();

        // キャラ名・マップ座標はログイン中キャラ一覧(CLibInfoCharSvr)から補完する。
        // CInfoAccount::m_dwCharID は設定されていない未使用フィールドのため使わない。
        CLibInfoCharSvr *pCharLib = m_pMgrData->GetLibInfoChar();
        if (pCharLib != NULL) {
                std::vector<SboLoginCharRow> loginChars;

                pCharLib->Enter();
                int nLoginCount = pCharLib->GetCountLogIn();
                for (int i = 0; i < nLoginCount; ++i) {
                        CInfoCharBase *pChar = static_cast<CInfoCharBase *>(pCharLib->GetPtrLogIn(i));
                        if (pChar == NULL) {
                                continue;
                        }

                        SboLoginCharRow loginChar;
                        loginChar.nSessionId = pChar->m_dwSessionID;
                        loginChar.nAccountId = pChar->m_dwAccountID;
                        loginChar.nCharId = pChar->m_dwCharID;
                        loginChar.strCharName = pChar->m_strCharName.GetUtf8Pointer() != NULL
                                ? pChar->m_strCharName.GetUtf8Pointer() : "";
                        loginChar.nMapId = pChar->m_dwMapID;
                        loginChar.nX = pChar->m_nMapX;
                        loginChar.nY = pChar->m_nMapY;
                        loginChars.push_back(loginChar);
                }
                pCharLib->Leave();

                SessionsJsonBuilder::ApplyLoginChars(rows, loginChars);
        }

        return rows;
}

std::string CServerSessionsHandler::GetTimestampNow()
{
        SboPlatform::LOCALTIME st;
        SboPlatform::GetSystemTime(&st);
        char szBuf[32];
        std::snprintf(szBuf, sizeof(szBuf), "%04d-%02d-%02dT%02d:%02d:%02dZ",
                st.nYear, st.nMonth, st.nDay, st.nHour, st.nMinute, st.nSecond);
        return szBuf;
}
