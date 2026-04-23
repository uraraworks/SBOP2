#pragma once

#include "Web/ApiHandler.h"

#include <string>

class CMgrData;

// 影カタログ一覧取得 GET /api/maps/shadows
class CMapShadowListHandler : public IApiHandler
{
public:
        explicit CMapShadowListHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        std::string BuildResponseJson() const;

        CMgrData *m_pMgrData;
};

// 影カタログ新規作成 POST /api/maps/shadows
class CMapShadowCreateHandler : public IApiHandler
{
public:
        explicit CMapShadowCreateHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        CMgrData *m_pMgrData;
};

// 影カタログ更新 PUT /api/maps/shadows
class CMapShadowUpdateHandler : public IApiHandler
{
public:
        explicit CMapShadowUpdateHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        CMgrData *m_pMgrData;
};

// 影カタログ削除 DELETE /api/maps/shadows?id=<id>
class CMapShadowDeleteHandler : public IApiHandler
{
public:
        explicit CMapShadowDeleteHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        CMgrData *m_pMgrData;
};
