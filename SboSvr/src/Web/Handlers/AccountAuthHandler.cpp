/// @file AccountAuthHandler.cpp
/// @brief ログインコード方式のアカウントAPI(/api/account/*) 実装ファイル
/// @details パスワード・コード・端末トークンは平文は元よりハッシュ値も
///          ログに出さないこと(推測の手掛かりを残さないため)。

#include "StdAfx.h"
#include "AccountAuthHandler.h"

#include <ctime>
#include <sstream>
#include <cctype>

#include "Web/JsonUtils.h"
#include "Web/IpRateLimiter.h"
#include "MgrData.h"
#include "GlobalDefine.h"
#include "SBOGlobal.h"
#include "LibInfo/LibInfoAccount.h"
#include "Info/InfoAccount.h"
#include "PasswordHash.h"
#include "Account/LoginCode.h"
#include "Account/AccountAuthStore.h"
#include "TextOutput.h"

namespace
{
	// register / issue-code / redeem で共有する IP 単位の試行回数制限。
	// コードはアカウント名を伴わずに照合するため、総当たりはアカウントを
	// またいで行える(docs/login-code-auth-plan.md 参照)。5回失敗で300秒ロックは
	// AuthSessionHandler(管理画面ログイン)と同じ値にそろえてある。
	const int kMaxFailCount = 5;
	const long kLockoutSeconds = 300;

	CIpRateLimiter &GetAccountApiLimiter(void)
	{
		static CIpRateLimiter limiter(kMaxFailCount, kLockoutSeconds);
		return limiter;
	}

	// おまかせ登録で1クリック登録が可能になったための連打対策。上の失敗回数制限
	// (5回失敗で300秒ロック)とは別物で、"成功"した登録の回数を IP 単位で数える。
	// 1時間に10件を超える新規登録(成功)は 429 にする。CIpRateLimiter は失敗記録の
	// 仕組みだが、"登録成功を1回記録する"用途にそのまま転用できるので使い回す。
	const int kMaxRegisterSuccessPerHour = 10;
	const long kRegisterSuccessWindowSeconds = 3600;

	CIpRateLimiter &GetAccountRegisterSuccessLimiter(void)
	{
		static CIpRateLimiter limiter(kMaxRegisterSuccessPerHour, kRegisterSuccessWindowSeconds);
		return limiter;
	}

	std::string TrimCopy(const std::string &strText)
	{
		size_t nStart = 0;
		while ((nStart < strText.size()) && std::isspace((unsigned char)strText[nStart])) {
			++nStart;
		}
		size_t nEnd = strText.size();
		while ((nEnd > nStart) && std::isspace((unsigned char)strText[nEnd - 1])) {
			--nEnd;
		}
		return strText.substr(nStart, nEnd - nStart);
	}

	std::string RateLimitKey(const HttpRequest &request)
	{
		return request.clientIp.empty() ? std::string("unknown") : request.clientIp;
	}

	/// @brief 429応答を組み立てる。
	void SetTooManyAttemptsResponse(HttpResponse &response, int nRetryAfterSeconds)
	{
		response.statusLine = "HTTP/1.1 429 Too Many Requests";
		response.SetHeader("Retry-After", std::to_string(nRetryAfterSeconds));
		std::ostringstream oss;
		oss << "{\"error\":\"too_many_attempts\",\"retryAfterSeconds\":" << nRetryAfterSeconds << "}";
		response.SetJsonBody(oss.str());
	}

	void SetErrorResponse(HttpResponse &response, const char *pszStatusLine, const char *pszError)
	{
		response.statusLine = pszStatusLine;
		std::ostringstream oss;
		oss << "{\"error\":\"" << JsonUtils::Escape(pszError) << "\"}";
		response.SetJsonBody(oss.str());
	}

	/// @brief アカウントIDからアカウント名を取得する(排他を取って読む)。
	bool FetchAccountName(CLibInfoAccount *pAccountLib, unsigned int dwAccountID, std::string &outName)
	{
		pAccountLib->Enter();
		PCInfoAccount pAccount = pAccountLib->GetPtr((DWORD)dwAccountID);
		bool bFound = (pAccount != NULL);
		if (bFound) {
			outName = (LPCSTR)pAccount->m_strAccount;
		}
		pAccountLib->Leave();
		return bFound;
	}

	std::string BuildAccountBody(const std::string &strAccount)
	{
		std::ostringstream oss;
		oss << "{\"account\":\"" << JsonUtils::Escape(strAccount) << "\"}";
		return oss.str();
	}
}

// ============================================================
// CAccountRegisterHandler : account, password -> code(新規作成)
// ============================================================

CAccountRegisterHandler::CAccountRegisterHandler(CMgrData *pMgrData)
	: m_pMgrData(pMgrData)
{
}

void CAccountRegisterHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
	if (m_pMgrData == NULL) {
		SetErrorResponse(response, "HTTP/1.1 503 Service Unavailable", "backend_unavailable");
		return;
	}

	std::string strKey = RateLimitKey(request);
	time_t now = time(NULL);
	int nRetryAfterSeconds = 0;
	if (GetAccountApiLimiter().IsLockedOut(strKey, now, nRetryAfterSeconds)) {
		SetTooManyAttemptsResponse(response, nRetryAfterSeconds);
		return;
	}
	// 連打対策: おまかせ登録で1クリック登録できるようになったため、成功回数も別途制限する。
	if (GetAccountRegisterSuccessLimiter().IsLockedOut(strKey, now, nRetryAfterSeconds)) {
		SetTooManyAttemptsResponse(response, nRetryAfterSeconds);
		return;
	}

	std::string strAccountInput, strPasswordInput;
	bool bHasAccount = JsonUtils::TryGetString(request.body, "account", strAccountInput);
	bool bHasPassword = JsonUtils::TryGetString(request.body, "password", strPasswordInput);
	strAccountInput = TrimCopy(strAccountInput);

	// account・password の両方が省略/空なら「おまかせ登録」(名前・パスワードをサーバーが
	// 自動生成する)。片方だけの指定は不正(400)。両方指定なら従来どおりの動作。
	bool bAccountEmpty = (!bHasAccount) || strAccountInput.empty();
	bool bPasswordEmpty = (!bHasPassword) || strPasswordInput.empty();
	bool bAutoGenerate = bAccountEmpty && bPasswordEmpty;
	if (!bAutoGenerate && (bAccountEmpty || bPasswordEmpty)) {
		SetErrorResponse(response, "HTTP/1.1 400 Bad Request", "invalid_request");
		return;
	}

	CLibInfoAccount *pAccountLib = m_pMgrData->GetLibInfoAccount();
	if (pAccountLib == NULL) {
		SetErrorResponse(response, "HTTP/1.1 503 Service Unavailable", "backend_unavailable");
		return;
	}

	// 管理者名と同じ名前の登録は拒否する(管理者名側も小文字化してから比較する)
	std::string strAdminNormalized = LoginCode::NormalizeAccountName((LPCSTR)m_pMgrData->GetAdminAccount());

	std::string strAccount;
	std::string strPassword;

	if (!bAutoGenerate) {
		// クライアント(WindowLOGINBrowser::NormalizeLoginText)はアカウント名を小文字化して
		// 送るが、CLibInfoAccount::GetPtr(LPCSTR) は大小文字を区別する。検証・検索・保存の
		// 前に必ず小文字化しておく(表記ゆれで別アカウント扱いになるのを防ぐ)。
		strAccount = LoginCode::NormalizeAccountName(strAccountInput);
		strPassword = strPasswordInput;

		// 名前の検証: ゲームの自動作成と同じ条件(ASCII 0x21〜0x7E)にそろえる。
		if (!LoginCode::IsAcceptableAccountName(strAccount)) {
			GetAccountApiLimiter().RecordFailure(strKey, now);
			SetErrorResponse(response, "HTTP/1.1 400 Bad Request", "invalid_account_name");
			return;
		}
		// パスワードの検証(空不可・ASCII表示可能文字のみ)
		if (!PasswordHash::IsAcceptable(strPassword.c_str())) {
			GetAccountApiLimiter().RecordFailure(strKey, now);
			SetErrorResponse(response, "HTTP/1.1 400 Bad Request", "invalid_password");
			return;
		}
		if (strAccount == strAdminNormalized) {
			SetErrorResponse(response, "HTTP/1.1 409 Conflict", "account_exists");
			return;
		}
	} else {
		// おまかせ登録: パスワードは1回生成すればよい(名前と違って一意性は不要)。
		strPassword = LoginCode::GenerateAutoPassword();
		if (strPassword.empty()) {
			SetErrorResponse(response, "HTTP/1.1 500 Internal Server Error", "password_generate_failed");
			return;
		}
	}

	pAccountLib->Enter();

	if (bAutoGenerate) {
		// 名前の一意性はサーバー側で担保する: 衝突したら排他内で作り直す(クライアントに
		// 409 は返さない)。管理者名との衝突もここでまとめて弾く。
		const int kMaxNameAttempts = 10;
		bool bNameFound = false;
		for (int i = 0; i < kMaxNameAttempts; ++i) {
			std::string strCandidate = LoginCode::GenerateAutoAccountName();
			if (strCandidate.empty() || (strCandidate == strAdminNormalized)) {
				continue;
			}
			if (pAccountLib->GetPtr(strCandidate.c_str()) == NULL) {
				strAccount = strCandidate;
				bNameFound = true;
				break;
			}
		}
		if (!bNameFound) {
			pAccountLib->Leave();
			SetErrorResponse(response, "HTTP/1.1 500 Internal Server Error", "account_name_generate_failed");
			return;
		}
	} else {
		PCInfoAccount pExisting = pAccountLib->GetPtr(strAccount.c_str());
		if (pExisting != NULL) {
			pAccountLib->Leave();
			SetErrorResponse(response, "HTTP/1.1 409 Conflict", "account_exists");
			return;
		}
	}

	std::string strHashedPassword = PasswordHash::Hash(strPassword.c_str());
	if (strHashedPassword.empty()) {
		pAccountLib->Leave();
		SetErrorResponse(response, "HTTP/1.1 500 Internal Server Error", "password_hash_failed");
		return;
	}

	PCInfoAccount pAccount = (PCInfoAccount)pAccountLib->GetNew();
	if (pAccount == NULL) {
		pAccountLib->Leave();
		SetErrorResponse(response, "HTTP/1.1 500 Internal Server Error", "allocation_failed");
		return;
	}

	TrimViewString(pAccount->m_strAccount, strAccount.c_str());
	if (pAccount->m_strAccount.IsEmpty()) {
		SAFE_DELETE(pAccount);
		pAccountLib->Leave();
		SetErrorResponse(response, "HTTP/1.1 400 Bad Request", "invalid_account_name");
		return;
	}

	pAccount->m_strPassword = strHashedPassword.c_str();
	time_t currentTime = (now < 0) ? 0 : now;
	pAccount->m_dwTimeMakeAccount = (DWORD)currentTime;
	pAccount->m_dwTimeLastLogin = 0;
	pAccount->m_dwLoginCount = 0;
	pAccount->m_bDisable = FALSE;
	pAccount->m_nAdminLevel = ADMINLEVEL_NONE;

	pAccountLib->Add((PCInfoBase)pAccount);
	unsigned int dwAccountID = (unsigned int)pAccount->m_dwAccountID;
	std::string strResolvedAccount = (LPCSTR)pAccount->m_strAccount;

	pAccountLib->Leave();

	// コードを発行する
	std::string strCode = LoginCode::GenerateCode();
	if (strCode.empty()) {
		SetErrorResponse(response, "HTTP/1.1 500 Internal Server Error", "code_generate_failed");
		return;
	}
	std::string strCodeHash = LoginCode::HashCode(strCode);

	CAccountAuthStore Store;
	if (!Store.IssueCode(dwAccountID, strCodeHash, (long)currentTime)) {
		SetErrorResponse(response, "HTTP/1.1 500 Internal Server Error", "code_store_failed");
		return;
	}

	GetAccountApiLimiter().ClearFailure(strKey);
	// 成功回数を記録する(連打対策)。CIpRateLimiter は失敗記録の仕組みだが、
	// "登録成功を1回記録する"用途にそのまま転用している(クラスコメント参照)。
	GetAccountRegisterSuccessLimiter().RecordFailure(strKey, now);
	if (m_pMgrData->GetLog() != NULL) {
		m_pMgrData->GetLog()->Write("[AccountRegister] success account=%s", strResolvedAccount.c_str());
	}

	// おまかせ登録した名前・パスワードは、画面に出せるようここで応答に含める
	// (自動生成した値はログに出さない。パスワードは平文のまま返す必要があるため
	// ハッシュ化前の strPassword を使う)。
	std::ostringstream oss;
	oss << "{\"account\":\"" << JsonUtils::Escape(strResolvedAccount) << "\"";
	if (bAutoGenerate) {
		oss << ",\"password\":\"" << JsonUtils::Escape(strPassword) << "\"";
	}
	oss << ",\"code\":\"" << JsonUtils::Escape(LoginCode::FormatCodeForDisplay(strCode)) << "\"}";
	response.statusLine = "HTTP/1.1 201 Created";
	response.SetJsonBody(oss.str());
}

// ============================================================
// CAccountIssueCodeHandler : account, password -> code(再発行)
// ============================================================

CAccountIssueCodeHandler::CAccountIssueCodeHandler(CMgrData *pMgrData)
	: m_pMgrData(pMgrData)
{
}

void CAccountIssueCodeHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
	if (m_pMgrData == NULL) {
		SetErrorResponse(response, "HTTP/1.1 503 Service Unavailable", "backend_unavailable");
		return;
	}

	std::string strKey = RateLimitKey(request);
	time_t now = time(NULL);
	int nRetryAfterSeconds = 0;
	if (GetAccountApiLimiter().IsLockedOut(strKey, now, nRetryAfterSeconds)) {
		SetTooManyAttemptsResponse(response, nRetryAfterSeconds);
		return;
	}

	std::string strAccount, strPassword;
	if (!JsonUtils::TryGetString(request.body, "account", strAccount) ||
	    !JsonUtils::TryGetString(request.body, "password", strPassword)) {
		SetErrorResponse(response, "HTTP/1.1 400 Bad Request", "invalid_request");
		return;
	}
	strAccount = TrimCopy(strAccount);
	// register と同様、GetPtr(LPCSTR) は大小文字を区別するため小文字化してから検索する。
	strAccount = LoginCode::NormalizeAccountName(strAccount);

	CLibInfoAccount *pAccountLib = m_pMgrData->GetLibInfoAccount();
	if (pAccountLib == NULL) {
		SetErrorResponse(response, "HTTP/1.1 503 Service Unavailable", "backend_unavailable");
		return;
	}

	pAccountLib->Enter();
	PCInfoAccount pAccount = pAccountLib->GetPtr(strAccount.c_str());
	bool bPasswordOk = (pAccount != NULL) &&
		PasswordHash::Verify(pAccount->m_strPassword.GetUtf8Pointer(), strPassword.c_str());
	if (!bPasswordOk || (pAccount->m_bDisable)) {
		pAccountLib->Leave();
		GetAccountApiLimiter().RecordFailure(strKey, now);
		SetErrorResponse(response, "HTTP/1.1 401 Unauthorized", "unauthorized");
		return;
	}

	unsigned int dwAccountID = (unsigned int)pAccount->m_dwAccountID;
	pAccountLib->Leave();

	std::string strCode = LoginCode::GenerateCode();
	if (strCode.empty()) {
		SetErrorResponse(response, "HTTP/1.1 500 Internal Server Error", "code_generate_failed");
		return;
	}
	std::string strCodeHash = LoginCode::HashCode(strCode);

	CAccountAuthStore Store;
	// IssueCode は再発行時、旧コードと全端末トークンを失効させたうえで新しいコードを保存する。
	if (!Store.IssueCode(dwAccountID, strCodeHash, (long)now)) {
		SetErrorResponse(response, "HTTP/1.1 500 Internal Server Error", "code_store_failed");
		return;
	}

	GetAccountApiLimiter().ClearFailure(strKey);
	if (m_pMgrData->GetLog() != NULL) {
		m_pMgrData->GetLog()->Write("[AccountIssueCode] success account=%s", strAccount.c_str());
	}

	std::ostringstream oss;
	oss << "{\"code\":\"" << JsonUtils::Escape(LoginCode::FormatCodeForDisplay(strCode)) << "\"}";
	response.statusLine = "HTTP/1.1 200 OK";
	response.SetJsonBody(oss.str());
}

// ============================================================
// CAccountRedeemHandler : code, label -> deviceToken, account
// ============================================================

CAccountRedeemHandler::CAccountRedeemHandler(CMgrData *pMgrData)
	: m_pMgrData(pMgrData)
{
}

void CAccountRedeemHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
	if (m_pMgrData == NULL) {
		SetErrorResponse(response, "HTTP/1.1 503 Service Unavailable", "backend_unavailable");
		return;
	}

	std::string strKey = RateLimitKey(request);
	time_t now = time(NULL);
	int nRetryAfterSeconds = 0;
	if (GetAccountApiLimiter().IsLockedOut(strKey, now, nRetryAfterSeconds)) {
		SetTooManyAttemptsResponse(response, nRetryAfterSeconds);
		return;
	}

	std::string strCodeInput, strLabel;
	if (!JsonUtils::TryGetString(request.body, "code", strCodeInput)) {
		SetErrorResponse(response, "HTTP/1.1 400 Bad Request", "invalid_request");
		return;
	}
	// label は無くてもよい(空扱い)
	JsonUtils::TryGetString(request.body, "label", strLabel);

	bool bValid = false;
	std::string strNormalized = LoginCode::NormalizeCode(strCodeInput, bValid);
	if (!bValid) {
		GetAccountApiLimiter().RecordFailure(strKey, now);
		SetErrorResponse(response, "HTTP/1.1 401 Unauthorized", "invalid_code");
		return;
	}
	std::string strCodeHash = LoginCode::HashCode(strNormalized);

	CAccountAuthStore Store;
	unsigned int dwAccountID = 0;
	if (!Store.FindAccountIDByCodeHash(strCodeHash, dwAccountID)) {
		GetAccountApiLimiter().RecordFailure(strKey, now);
		SetErrorResponse(response, "HTTP/1.1 401 Unauthorized", "invalid_code");
		return;
	}

	CLibInfoAccount *pAccountLib = m_pMgrData->GetLibInfoAccount();
	if (pAccountLib == NULL) {
		SetErrorResponse(response, "HTTP/1.1 503 Service Unavailable", "backend_unavailable");
		return;
	}

	std::string strAccountName;
	if (!FetchAccountName(pAccountLib, dwAccountID, strAccountName)) {
		// コードは残っているがアカウント本体が無い(削除済み等)
		GetAccountApiLimiter().RecordFailure(strKey, now);
		SetErrorResponse(response, "HTTP/1.1 401 Unauthorized", "invalid_code");
		return;
	}

	std::string strToken = LoginCode::GenerateDeviceToken();
	if (strToken.empty()) {
		SetErrorResponse(response, "HTTP/1.1 500 Internal Server Error", "token_generate_failed");
		return;
	}
	std::string strTokenHash = LoginCode::HashDeviceToken(strToken);

	if (!Store.CreateDevice(dwAccountID, strTokenHash, strLabel, (long)now)) {
		SetErrorResponse(response, "HTTP/1.1 500 Internal Server Error", "device_store_failed");
		return;
	}

	GetAccountApiLimiter().ClearFailure(strKey);
	if (m_pMgrData->GetLog() != NULL) {
		m_pMgrData->GetLog()->Write("[AccountRedeem] success account=%s", strAccountName.c_str());
	}

	std::ostringstream oss;
	oss << "{\"deviceToken\":\"" << JsonUtils::Escape(strToken) << "\","
	    << "\"account\":\"" << JsonUtils::Escape(strAccountName) << "\"}";
	response.statusLine = "HTTP/1.1 200 OK";
	response.SetJsonBody(oss.str());
}

// ============================================================
// CAccountMeHandler : deviceToken -> account
// ============================================================

CAccountMeHandler::CAccountMeHandler(CMgrData *pMgrData)
	: m_pMgrData(pMgrData)
{
}

void CAccountMeHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
	if (m_pMgrData == NULL) {
		SetErrorResponse(response, "HTTP/1.1 503 Service Unavailable", "backend_unavailable");
		return;
	}

	std::string strToken;
	if (!JsonUtils::TryGetString(request.body, "deviceToken", strToken) || strToken.empty()) {
		SetErrorResponse(response, "HTTP/1.1 400 Bad Request", "invalid_request");
		return;
	}

	std::string strTokenHash = LoginCode::HashDeviceToken(strToken);

	CAccountAuthStore Store;
	unsigned int dwAccountID = 0;
	time_t now = time(NULL);
	if (!Store.TouchDevice(strTokenHash, (long)now, dwAccountID)) {
		SetErrorResponse(response, "HTTP/1.1 401 Unauthorized", "invalid_token");
		return;
	}

	CLibInfoAccount *pAccountLib = m_pMgrData->GetLibInfoAccount();
	std::string strAccountName;
	if ((pAccountLib == NULL) || !FetchAccountName(pAccountLib, dwAccountID, strAccountName)) {
		SetErrorResponse(response, "HTTP/1.1 401 Unauthorized", "invalid_token");
		return;
	}

	response.statusLine = "HTTP/1.1 200 OK";
	response.SetJsonBody(BuildAccountBody(strAccountName));
}

// ============================================================
// CAccountLogoutHandler : deviceToken -> ok
// ============================================================

CAccountLogoutHandler::CAccountLogoutHandler(CMgrData *pMgrData)
	: m_pMgrData(pMgrData)
{
}

void CAccountLogoutHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
	std::string strToken;
	if (JsonUtils::TryGetString(request.body, "deviceToken", strToken) && !strToken.empty()) {
		std::string strTokenHash = LoginCode::HashDeviceToken(strToken);
		CAccountAuthStore Store;
		Store.DeleteDeviceByTokenHash(strTokenHash);
	}

	response.statusLine = "HTTP/1.1 200 OK";
	response.SetJsonBody("{\"ok\":true}");
}
