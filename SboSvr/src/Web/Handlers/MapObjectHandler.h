#pragma once

#include "Web/ApiHandler.h"

#include <iosfwd>
#include <string>

class CMgrData;
class CInfoMapBase;
class CInfoMapObjectData;
class CLibInfoMapObject;
class CLibInfoMapParts;

class CMapObjectListHandler : public IApiHandler
{
public:
        explicit CMapObjectListHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        std::string BuildResponseJson() const;
        void AppendMapJson(std::ostringstream &oss, const CInfoMapBase *pMap, CLibInfoMapObject *pObjectLib, CLibInfoMapParts *pPartsLib) const;
        void AppendObjectJson(std::ostringstream &oss, const CInfoMapObjectData *pPlacement, CLibInfoMapObject *pObjectLib) const;
        static std::string FormatHex(unsigned long value);
        static std::string ResolveWeatherLabel(unsigned long weatherType);

        CMgrData *m_pMgrData;
};

// オブジェクトテンプレート一覧 GET /api/maps/objects/templates
class CMapObjectTemplateListHandler : public IApiHandler
{
public:
        explicit CMapObjectTemplateListHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        std::string BuildResponseJson() const;

        CMgrData *m_pMgrData;
};

// オブジェクトテンプレート追加 POST /api/maps/objects/templates
class CMapObjectTemplateCreateHandler : public IApiHandler
{
public:
        explicit CMapObjectTemplateCreateHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        CMgrData *m_pMgrData;
};

// オブジェクトテンプレート更新 PUT /api/maps/objects/templates
class CMapObjectTemplateUpdateHandler : public IApiHandler
{
public:
        explicit CMapObjectTemplateUpdateHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        CMgrData *m_pMgrData;
};

// オブジェクトテンプレート削除 DELETE /api/maps/objects/templates
class CMapObjectTemplateDeleteHandler : public IApiHandler
{
public:
        explicit CMapObjectTemplateDeleteHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        CMgrData *m_pMgrData;
};

// オブジェクト配置追加 POST /api/maps/placements
class CMapPlacementCreateHandler : public IApiHandler
{
public:
        explicit CMapPlacementCreateHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        CMgrData *m_pMgrData;
};

// オブジェクト配置更新 PUT /api/maps/placements
class CMapPlacementUpdateHandler : public IApiHandler
{
public:
        explicit CMapPlacementUpdateHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        CMgrData *m_pMgrData;
};

// オブジェクト配置削除 DELETE /api/maps/placements
class CMapPlacementDeleteHandler : public IApiHandler
{
public:
        explicit CMapPlacementDeleteHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        CMgrData *m_pMgrData;
};
