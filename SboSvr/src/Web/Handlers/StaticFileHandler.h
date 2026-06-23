#pragma once

#include <string>

#if defined(_WIN32)
#include <windows.h>
#endif

#include "Web/ApiHandler.h"

// ファイルのメタ情報（ETag/Last-Modified生成用）
struct FileMetaInfo
{
        unsigned long long  fileSize;   // バイト数
        FILETIME            mtime;      // 最終更新時刻（UTC）
        bool                valid;      // 取得成功フラグ
};

class CStaticFileHandler : public IApiHandler
{
public:
        CStaticFileHandler(const std::wstring &rootDirectory, const std::wstring &defaultDocument, const std::string &mountPath);

        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        bool            BuildFilePath(const std::string &requestPath, std::wstring &outPath, std::string &outRelativePath) const;
        bool            LoadFile(const std::wstring &path, std::string &outContent, FileMetaInfo *outMeta = nullptr) const;
        std::string     DetermineContentType(const std::string &relativePath) const;
        std::string     DetermineCacheControl(const std::string &relativePath) const;
        static std::string NormalizeRequestPath(const std::string &path);
        static bool     ContainsParentReference(const std::string &path);
        static std::string ToUtf8(const std::wstring &text);
        static std::string BuildETag(const FileMetaInfo &meta);
        static std::string BuildLastModified(const FileMetaInfo &meta);
        static bool     ParseHttpDate(const std::string &httpDate, FILETIME &outFt);

        std::wstring    m_rootDirectory;
        std::wstring    m_defaultDocument;
        std::string     m_mountPath;
};
