#include "stdafx.h"
#include "HealthHandler.h"

void CHealthHandler::Handle(const HttpRequest & /*request*/, HttpResponse &response)
{
        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody("{\"status\":\"ok\"}");
}

