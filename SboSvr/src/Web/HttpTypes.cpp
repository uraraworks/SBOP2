#include "stdafx.h"
#include "HttpTypes.h"

#include <cctype>
#include <sstream>

namespace
{
static bool EqualIgnoreCase(const std::string &lhs, const char *pszRhs)
{
        if (pszRhs == NULL) {
                return false;
        }
        size_t nLen = lhs.size();
        for (size_t i = 0; i < nLen; ++i) {
                char chL = lhs[i];
                char chR = pszRhs[i];
                if (chR == '\0') {
                        return false;
                }
                if (std::toupper(static_cast<unsigned char>(chL)) != std::toupper(static_cast<unsigned char>(chR))) {
                        return false;
                }
        }
        return pszRhs[nLen] == '\0';
}
}

const char *HttpRequest::FindHeader(const char *pszName) const
{
        if (pszName == NULL) {
                return NULL;
        }
        for (std::vector<HttpHeader>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
                if (EqualIgnoreCase(it->name, pszName)) {
                        return it->value.c_str();
                }
        }
        return NULL;
}

HttpResponse::HttpResponse()
        : statusLine("HTTP/1.1 200 OK")
{
        HttpHeader connection;
        connection.name = "Connection";
        connection.value = "close";
        headers.push_back(connection);
}

void HttpResponse::SetHeader(const std::string &name, const std::string &value)
{
        HttpHeader *pHeader = FindHeaderInternal(name.c_str());
        if (pHeader != NULL) {
                pHeader->value = value;
        } else {
                HttpHeader header;
                header.name = name;
                header.value = value;
                headers.push_back(header);
        }
}

void HttpResponse::SetJsonBody(const std::string &jsonBody)
{
        body = jsonBody;
        SetHeader("Content-Type", "application/json; charset=utf-8");
}

void HttpResponse::EnsureContentLength()
{
        std::ostringstream oss;
        oss << body.size();
        SetHeader("Content-Length", oss.str());
}

HttpHeader *HttpResponse::FindHeaderInternal(const char *pszName)
{
        if (pszName == NULL) {
                return NULL;
        }
        for (std::vector<HttpHeader>::iterator it = headers.begin(); it != headers.end(); ++it) {
                if (EqualIgnoreCase(it->name, pszName)) {
                        return &(*it);
                }
        }
        return NULL;
}

