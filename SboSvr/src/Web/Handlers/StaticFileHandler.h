#pragma once

#include <string>

#include "Web/ApiHandler.h"

class CStaticFileHandler : public IApiHandler
{
public:
        CStaticFileHandler(const std::wstring &rootDirectory, const std::wstring &defaultDocument, const std::string &mountPath);

        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        bool            BuildFilePath(const std::string &requestPath, std::wstring &outPath, std::string &outRelativePath) const;
        bool            LoadFile(const std::wstring &path, std::string &outContent) const;
        std::string     DetermineContentType(const std::string &relativePath) const;
        static std::string NormalizeRequestPath(const std::string &path);
        static bool     ContainsParentReference(const std::string &path);
        static std::string ToUtf8(const std::wstring &text);

        std::wstring    m_rootDirectory;
        std::wstring    m_defaultDocument;
        std::string     m_mountPath;
};
