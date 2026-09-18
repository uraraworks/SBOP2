/// @file AccountAuthHandler.h
/// @brief ログインコード方式のアカウントAPI(/api/account/*) 定義ファイル
/// @details docs/login-code-auth-plan.md の S1 節を実装する。
///          管理者認証ゲートの対象外(HttpServer::IsAuthApiPath)であり、
///          IP単位の試行回数制限を掛けたうえで、匿名から直接叩けるAPI。

#pragma once

#include "Web/ApiHandler.h"

class CMgrData;

/// POST /api/account/register : account, password -> code(新規作成)
class CAccountRegisterHandler : public IApiHandler
{
public:
	explicit CAccountRegisterHandler(CMgrData *pMgrData);
	virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
	CMgrData *m_pMgrData;
};

/// POST /api/account/issue-code : account, password -> code(再発行)
class CAccountIssueCodeHandler : public IApiHandler
{
public:
	explicit CAccountIssueCodeHandler(CMgrData *pMgrData);
	virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
	CMgrData *m_pMgrData;
};

/// POST /api/account/redeem : code, label -> deviceToken, account
class CAccountRedeemHandler : public IApiHandler
{
public:
	explicit CAccountRedeemHandler(CMgrData *pMgrData);
	virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
	CMgrData *m_pMgrData;
};

/// POST /api/account/me : deviceToken -> account
class CAccountMeHandler : public IApiHandler
{
public:
	explicit CAccountMeHandler(CMgrData *pMgrData);
	virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
	CMgrData *m_pMgrData;
};

/// POST /api/account/logout : deviceToken -> ok
class CAccountLogoutHandler : public IApiHandler
{
public:
	explicit CAccountLogoutHandler(CMgrData *pMgrData);
	virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
	CMgrData *m_pMgrData;
};
