#pragma once

#include "Web/ApiHandler.h"

#include <string>

class CMgrData;

// キャラクター一覧検索  GET /api/characters
class CCharacterListHandler : public IApiHandler
{
public:
        explicit CCharacterListHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        CMgrData *m_pMgrData;
};
