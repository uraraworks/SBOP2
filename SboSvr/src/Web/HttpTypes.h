#pragma once

#include <string>
#include <vector>

struct HttpHeader
{
        std::string     name;
        std::string     value;
};

struct HttpRequest
{
        std::string             method;
        std::string             path;
        std::string             version;
        std::vector<HttpHeader> headers;
        std::string             body;

        // クライアントの実IP(ドット区切り10進表記)。HttpServer::HandleClient が
        // ParseHttpRequest 成功後に設定する。ループバック越し(WebSocketBridge等)
        // では X-Forwarded-For の右端で差し替え済み。取得できなければ空文字列。
        std::string             clientIp;

        const char *FindHeader(const char *pszName) const;
};

struct HttpResponse
{
        HttpResponse();

        void            SetHeader(const std::string &name, const std::string &value);
        void            SetJsonBody(const std::string &jsonBody);
        void            EnsureContentLength();

        std::string             statusLine;
        std::vector<HttpHeader> headers;
        std::string             body;

private:
        HttpHeader *FindHeaderInternal(const char *pszName);
};
