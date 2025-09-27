#pragma once

#include "HttpTypes.h"

class IApiHandler
{
public:
        virtual ~IApiHandler() {}
        virtual void Handle(const HttpRequest &request, HttpResponse &response) = 0;
};

