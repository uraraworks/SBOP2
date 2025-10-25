#pragma once

#include "Web/ApiHandler.h"

#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <cstdlib>
#include <cwchar>

#include <windows.h>

class CMgrData;
class CLibInfoMapParts;
class CInfoMapParts;

class CMapPartsResourceProvider
{
public:
        CMapPartsResourceProvider();
        ~CMapPartsResourceProvider();

        bool GetSheetPng(int sheetIndex, std::vector<unsigned char> &outData);
        int GetSheetCount();
        bool IsAvailable();

private:
        bool EnsureLibraryLocked();
        bool LoadSheetLocked(int sheetIndex, std::vector<unsigned char> &outData);
        bool ResolveLibraryPath(std::wstring &outPath) const;

        std::mutex                                      m_mutex;
        HMODULE                                         m_hModule;
        std::map<int, std::vector<unsigned char> >     m_sheetCache;
        int                                             m_sheetCount;
};

class CMapPartsListHandler : public IApiHandler
{
public:
        CMapPartsListHandler(CMgrData *pMgrData, std::shared_ptr<CMapPartsResourceProvider> provider);

        void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
        std::string BuildResponseJson() const;
        void AppendPartJson(std::ostringstream &oss, const CInfoMapParts *pInfo, std::set<int> &sheetSet) const;
        void AppendSpriteJson(std::ostringstream &oss, const char *pszKey, unsigned int grpId, std::set<int> &sheetSet) const;
        CMgrData                                        *m_pMgrData;
        std::shared_ptr<CMapPartsResourceProvider>       m_provider;
};

class CMapPartsSheetHandler : public IApiHandler
{
public:
        CMapPartsSheetHandler(std::shared_ptr<CMapPartsResourceProvider> provider, std::string pathPrefix);

        void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
        bool TryParseSheetIndex(const std::string &path, int &sheetIndex) const;

        std::shared_ptr<CMapPartsResourceProvider>       m_provider;
        std::string                                      m_pathPrefix;
};
