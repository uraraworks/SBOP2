#pragma once

#include "Web/ApiHandler.h"

class CMgrData;

// NPC モーションペア一覧取得  GET /api/npc-motion-pairs
class CNpcMotionPairsHandler : public IApiHandler
{
public:
        explicit CNpcMotionPairsHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        CMgrData *m_pMgrData;
};
