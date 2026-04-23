#pragma once

#include "Web/ApiHandler.h"

#include <string>

class CMgrData;

// マップ情報一覧取得 GET /api/maps
class CMapInfoListHandler : public IApiHandler
{
public:
        explicit CMapInfoListHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        std::string BuildResponseJson() const;

        CMgrData *m_pMgrData;
};

// マップ情報更新 PUT /api/maps
class CMapInfoUpdateHandler : public IApiHandler
{
public:
        explicit CMapInfoUpdateHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        CMgrData *m_pMgrData;
};
