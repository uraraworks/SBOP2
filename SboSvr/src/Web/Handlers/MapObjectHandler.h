#pragma once

#include "Web/ApiHandler.h"

#include <iosfwd>
#include <string>

class CMgrData;
class CInfoMapBase;
class CInfoMapObjectData;
class CLibInfoMapObject;

class CMapObjectListHandler : public IApiHandler
{
public:
        explicit CMapObjectListHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        std::string BuildResponseJson() const;
        void AppendMapJson(std::ostringstream &oss, const CInfoMapBase *pMap, CLibInfoMapObject *pObjectLib) const;
        void AppendObjectJson(std::ostringstream &oss, const CInfoMapObjectData *pPlacement, CLibInfoMapObject *pObjectLib) const;
        static std::string FormatHex(unsigned long value);
        static std::string ResolveWeatherLabel(unsigned long weatherType);

        CMgrData *m_pMgrData;
};

