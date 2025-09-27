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

