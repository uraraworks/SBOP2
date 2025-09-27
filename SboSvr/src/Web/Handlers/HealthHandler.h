#pragma once

#include "Web/ApiHandler.h"

class CHealthHandler : public IApiHandler
{
public:
        virtual void Handle(const HttpRequest &request, HttpResponse &response);
};

