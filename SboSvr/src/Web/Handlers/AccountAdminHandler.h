#pragma once

#include "Web/ApiHandler.h"

#include <string>

class CMgrData;

// アカウントのゴミ箱・復帰・完全削除
//   POST   /api/accounts/{id}/trash    ゴミ箱へ
//   DELETE /api/accounts/{id}/trash    ゴミ箱から復帰
//   DELETE /api/accounts/{id}          完全削除
class CAccountAdminHandler : public IApiHandler
{
public:
        explicit CAccountAdminHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        void HandleTrash(const HttpRequest &request, HttpResponse &response, unsigned int dwAccountID, unsigned int dwActorAccountID);
        void HandleUntrash(const HttpRequest &request, HttpResponse &response, unsigned int dwAccountID);
        void HandlePurge(const HttpRequest &request, HttpResponse &response, unsigned int dwAccountID);

        CMgrData *m_pMgrData;
};
