#pragma once

#include "Web/ApiHandler.h"

class CMgrData;

class CAuthMeHandler : public IApiHandler
{
public:
        explicit CAuthMeHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        CMgrData *m_pMgrData;
};

class CAdminLoginHandler : public IApiHandler
{
public:
        explicit CAdminLoginHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        CMgrData *m_pMgrData;
};

class CAuthLogoutHandler : public IApiHandler
{
public:
        virtual void Handle(const HttpRequest &request, HttpResponse &response);
};
