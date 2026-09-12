#pragma once

#include <ctime>
#include <string>

#include "Web/ApiHandler.h"

// ファイルのメタ情報（ETag/Last-Modified生成用）
struct FileMetaInfo
{
        unsigned long long  fileSize;   // バイト数
        std::time_t         mtime;      // 最終更新時刻（UTC、秒単位）
        bool                valid;      // 取得成功フラグ
};

class CStaticFileHandler : public IApiHandler
{
public:
        CStaticFileHandler(const std::wstring &rootDirectory, const std::wstring &defaultDocument, const std::string &mountPath);

        virtual void Handle(const HttpRequest &request, HttpResponse &response);

        // ETag/Last-Modified の生成・パース。往復一致がキャッシュ(304判定)の
        // 前提になるため、テスト(SboSvrTest)から直接検証できるよう公開する。
        static std::string BuildETag(const FileMetaInfo &meta);
        static std::string BuildLastModified(const FileMetaInfo &meta);
        static bool     ParseHttpDate(const std::string &httpDate, std::time_t &outTime);

private:
        bool            BuildFilePath(const std::string &requestPath, std::wstring &outPath, std::string &outRelativePath) const;
        // ファイル本文を読まず、サイズ・mtime だけを取得する軽量メソッド
        bool            StatFile(const std::wstring &path, FileMetaInfo &outMeta) const;
        bool            LoadFile(const std::wstring &path, std::string &outContent) const;
        std::string     DetermineContentType(const std::string &relativePath) const;
        std::string     DetermineCacheControl(const std::string &relativePath) const;
        static std::string NormalizeRequestPath(const std::string &path);
        static bool     ContainsParentReference(const std::string &path);
        static std::string ToUtf8(const std::wstring &text);
        // URL パーセントエンコーディング(%XX)をデコードする。'+' は空白に
        // 変換しない(パスでは意味を持たないため)。不正なエンコーディング
        // (16進2桁でない、%の直後が無い等)は false を返す。
        static bool     PercentDecode(const std::string &path, std::string &outDecoded);
        // UTF-8 バイト列を wchar_t(UTF-16) 列へ変換する。不正な UTF-8 は
        // false を返す。
        static bool     Utf8ToWide(const std::string &utf8, std::wstring &outWide);

        std::wstring    m_rootDirectory;
        std::wstring    m_defaultDocument;
        std::string     m_mountPath;
};
