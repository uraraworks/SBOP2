#pragma once

#include "Web/ApiHandler.h"

#include <string>

class CMgrData;

// アカウント一覧 + 検索  GET /api/accounts
class CAccountListHandler : public IApiHandler
{
public:
        explicit CAccountListHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        CMgrData *m_pMgrData;
};
