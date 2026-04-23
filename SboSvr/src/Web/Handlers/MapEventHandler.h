#pragma once

#include "Web/ApiHandler.h"

#include <string>

class CMgrData;

// マップイベント一覧取得 GET /api/maps/events?mapId=<id>
class CMapEventListHandler : public IApiHandler
{
public:
        explicit CMapEventListHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        std::string BuildResponseJson(DWORD dwMapId) const;

        CMgrData *m_pMgrData;
};

// マップイベント新規作成 POST /api/maps/events
class CMapEventCreateHandler : public IApiHandler
{
public:
        explicit CMapEventCreateHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        CMgrData *m_pMgrData;
};

// マップイベント更新 PUT /api/maps/events
class CMapEventUpdateHandler : public IApiHandler
{
public:
        explicit CMapEventUpdateHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        CMgrData *m_pMgrData;
};

// マップイベント削除 DELETE /api/maps/events?mapId=<id>&id=<eventId>
class CMapEventDeleteHandler : public IApiHandler
{
public:
        explicit CMapEventDeleteHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        CMgrData *m_pMgrData;
};
