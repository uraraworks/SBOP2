#pragma once

#include "Web/ApiHandler.h"

#include <string>

class CMgrData;

// キャラクター詳細取得  GET /api/characters/{charId}
class CCharacterDetailHandler : public IApiHandler
{
public:
        explicit CCharacterDetailHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        CMgrData *m_pMgrData;
};
