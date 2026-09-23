/// @file DebugFixtureHandler.cpp
/// @brief /api/debug/fixture (テスト準備API) 実装ファイル
/// @details パスワード・端末トークンは平文はもちろんログにも出さないこと。

#include "StdAfx.h"

#ifdef _DEBUG

#include "DebugFixtureHandler.h"

#include <cctype>
#include <ctime>
#include <sstream>

#include "Web/DebugFixtureGuard.h"
#include "Web/JsonUtils.h"
#include "MgrData.h"
#include "GlobalDefine.h"
#include "Packet.h"
#include "PasswordHash.h"
#include "Account/LoginCode.h"
#include "Account/AccountAuthStore.h"
#include "LibInfo/LibInfoAccount.h"
#include "Info/InfoAccount.h"
#include "LibInfo/LibInfoCharSvr.h"
#include "Info/InfoCharBase.h"
#include "TextOutput.h"
#include "../../Platform/SvrPlatform.h"
#include "myLib/myString.h"

namespace
{
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

	void SetErrorResponse(HttpResponse &response, const char *pszStatusLine, const char *pszError)
	{
		response.statusLine = pszStatusLine;
		std::ostringstream oss;
		oss << "{\"error\":\"" << JsonUtils::Escape(pszError) << "\"}";
		response.SetJsonBody(oss.str());
	}

	/// @brief 乱数バイト列を小文字16進文字列にする。
	std::string ToHexLower(const unsigned char *pData, size_t nLength)
	{
		static const char kHexTable[] = "0123456789abcdef";
		std::string strResult;
		strResult.reserve(nLength * 2);
		for (size_t i = 0; i < nLength; ++i) {
			strResult += kHexTable[(pData[i] >> 4) & 0x0F];
			strResult += kHexTable[pData[i] & 0x0F];
		}
		return strResult;
	}

	/// @brief account 省略時の自動生成名("zzdbg" + 乱数8桁の16進)。
	///        本番運用アカウントと衝突しない見分けやすい接頭辞にしてある。
	std::string GenerateRandomAccountName(void)
	{
		unsigned char pRandom[4];
		if (!SboPlatform::GenerateRandomBytes(pRandom, sizeof(pRandom))) {
			return std::string();
		}
		return "zzdbg" + ToHexLower(pRandom, sizeof(pRandom));
	}

	/// @brief password 省略時の自動生成(英数字12文字)。
	std::string GenerateRandomPassword(void)
	{
		static const char kAlphabet[] =
			"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
		const size_t kLength = 12;
		unsigned char pRandom[kLength];
		if (!SboPlatform::GenerateRandomBytes(pRandom, sizeof(pRandom))) {
			return std::string();
		}
		std::string strResult;
		strResult.reserve(kLength);
		for (size_t i = 0; i < kLength; ++i) {
			strResult += kAlphabet[pRandom[i] % (sizeof(kAlphabet) - 1)];
		}
		return strResult;
	}

	/// @brief charName 省略時の自動生成("DebugChar" + 乱数6桁の16進)。
	std::string GenerateRandomCharName(void)
	{
		unsigned char pRandom[3];
		if (!SboPlatform::GenerateRandomBytes(pRandom, sizeof(pRandom))) {
			return std::string();
		}
		return "DebugChar" + ToHexLower(pRandom, sizeof(pRandom));
	}

	// charId で CInfoCharBase* を検索する(Lock済みの状態で呼ぶこと)。
	// CharacterUpdateHandler.cpp の FindChar と同じ実装(ファイル間で共有していないのは
	// 既存のハンドラ群の流儀にそろえたもの)。
	CInfoCharBase *FindCharByID(CLibInfoCharSvr *pCharLib, DWORD dwCharID)
	{
		int nTotal = pCharLib->GetCount();
		for (int i = 0; i < nTotal; ++i) {
			CInfoCharBase *pTmp = static_cast<CInfoCharBase *>(pCharLib->GetPtr(i));
			if ((pTmp != NULL) && (pTmp->m_dwCharID == dwCharID)) {
				return pTmp;
			}
		}
		return NULL;
	}
}

CDebugFixtureHandler::CDebugFixtureHandler(CMgrData *pMgrData)
	: m_pMgrData(pMgrData)
{
}

void CDebugFixtureHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
	if (m_pMgrData == NULL) {
		SetErrorResponse(response, "HTTP/1.1 503 Service Unavailable", "backend_unavailable");
		return;
	}

	// 安全策: loopbackからの直接リクエスト以外は存在しないAPIとして扱う(404)。
	// IIS等のリバースプロキシ越し・別ホストからのアクセスは弾く。
	if (!DebugFixtureGuard::IsAllowedRequest(request)) {
		response.statusLine = "HTTP/1.1 404 Not Found";
		response.SetJsonBody("{\"error\":\"not_found\"}");
		return;
	}

	std::string strAccountInput, strPasswordInput, strCharNameInput;
	JsonUtils::TryGetString(request.body, "account", strAccountInput);
	JsonUtils::TryGetString(request.body, "password", strPasswordInput);
	JsonUtils::TryGetString(request.body, "charName", strCharNameInput);

	bool bAccountSpecified = !TrimCopy(strAccountInput).empty();
	std::string strAccount = TrimCopy(strAccountInput);
	if (bAccountSpecified) {
		strAccount = LoginCode::NormalizeAccountName(strAccount);
		if (!LoginCode::IsAcceptableAccountName(strAccount)) {
			SetErrorResponse(response, "HTTP/1.1 400 Bad Request", "invalid_account_name");
			return;
		}
	} else {
		strAccount = GenerateRandomAccountName();
		if (strAccount.empty()) {
			SetErrorResponse(response, "HTTP/1.1 500 Internal Server Error", "account_generate_failed");
			return;
		}
	}

	std::string strPassword = strPasswordInput;
	if (!strPassword.empty()) {
		if (!PasswordHash::IsAcceptable(strPassword.c_str())) {
			SetErrorResponse(response, "HTTP/1.1 400 Bad Request", "invalid_password");
			return;
		}
	} else {
		strPassword = GenerateRandomPassword();
		if (strPassword.empty()) {
			SetErrorResponse(response, "HTTP/1.1 500 Internal Server Error", "password_generate_failed");
			return;
		}
	}

	std::string strCharNameUtf8 = TrimCopy(strCharNameInput);
	if (strCharNameUtf8.empty()) {
		strCharNameUtf8 = GenerateRandomCharName();
		if (strCharNameUtf8.empty()) {
			SetErrorResponse(response, "HTTP/1.1 500 Internal Server Error", "charname_generate_failed");
			return;
		}
	}

	CLibInfoAccount *pAccountLib = m_pMgrData->GetLibInfoAccount();
	CLibInfoCharSvr *pCharLib = m_pMgrData->GetLibInfoChar();
	if ((pAccountLib == NULL) || (pCharLib == NULL)) {
		SetErrorResponse(response, "HTTP/1.1 503 Service Unavailable", "backend_unavailable");
		return;
	}

	time_t now = time(NULL);
	unsigned int dwAccountID = 0;
	bool bNewAccount = false;

	// --- アカウント: 既存なら認証、無ければ新規作成(register と同じ流儀) ---
	pAccountLib->Enter();

	PCInfoAccount pExisting = pAccountLib->GetPtr(strAccount.c_str());
	if (pExisting != NULL) {
		bool bPasswordOk = PasswordHash::Verify(pExisting->m_strPassword.GetUtf8Pointer(), strPassword.c_str());
		if (!bPasswordOk || pExisting->m_bDisable) {
			pAccountLib->Leave();
			SetErrorResponse(response, "HTTP/1.1 409 Conflict", "account_exists");
			return;
		}
		dwAccountID = (unsigned int)pExisting->m_dwAccountID;
		pAccountLib->Leave();
	} else {
		// 管理者名と同じ名前の登録は拒否する(register と同様)
		std::string strAdminNormalized = LoginCode::NormalizeAccountName((LPCSTR)m_pMgrData->GetAdminAccount());
		if (strAccount == strAdminNormalized) {
			pAccountLib->Leave();
			SetErrorResponse(response, "HTTP/1.1 409 Conflict", "account_exists");
			return;
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
		dwAccountID = (unsigned int)pAccount->m_dwAccountID;
		bNewAccount = true;

		pAccountLib->Leave();
	}

	// --- 端末トークン発行(register/redeem と同じくコード無しで即発行してよい: ---
	// --- CAccountAuthStore::CreateDevice はコードの存在を前提にしない) ---
	std::string strToken = LoginCode::GenerateDeviceToken();
	if (strToken.empty()) {
		SetErrorResponse(response, "HTTP/1.1 500 Internal Server Error", "token_generate_failed");
		return;
	}
	std::string strTokenHash = LoginCode::HashDeviceToken(strToken);

	CAccountAuthStore Store;
	if (!Store.CreateDevice(dwAccountID, strTokenHash, "debug-fixture", (long)now)) {
		SetErrorResponse(response, "HTTP/1.1 500 Internal Server Error", "device_store_failed");
		return;
	}

	// --- キャラクター: 既にあれば1件目を使う(冪等)。無ければ新規作成 ---
	// CLibInfoAccount と CLibInfoCharSvr を同時にロックしない
	// (既存HTTPハンドラ群の流儀にそろえ、デッドロックの芽を避ける)。
	DWORD dwCharID = 0;
	bool bHasChar = false;

	pAccountLib->Enter();
	PCInfoAccount pAccountForChar = pAccountLib->GetPtr((DWORD)dwAccountID);
	if (pAccountForChar != NULL) {
		bHasChar = !pAccountForChar->m_adwCharID.empty();
		if (bHasChar) {
			dwCharID = pAccountForChar->m_adwCharID[0];
		}
	}
	pAccountLib->Leave();

	std::string strResolvedCharNameUtf8 = strCharNameUtf8;

	if (!bHasChar) {
		CmyString strRawCharName((LPCTSTR)Utf8ToTString(strCharNameUtf8.c_str()));
		CmyString strTrimmedCharName;
		TrimViewString(strTrimmedCharName, (LPCTSTR)strRawCharName);

		// 容姿は初期値相当の固定値(見た目はテストの本質ではないため)。
		// モーション種別は通常のキャラ作成(WindowSTYLESELECT)と同じ 2 にする。
		// 0 だとプレイヤー用のモーションが引けず、歩くと2x2や敵の絵になっていた。
		int nMakeResult = pCharLib->CreatePlayerCharacter(
			strRawCharName, strTrimmedCharName,
			CHARMOVETYPE_PC, SEX_MALE, FAMILYTYPE_HUMAN,
			/*wGrpIDEye*/1, /*wGrpIDEyeColor*/1, /*wGrpIDHairType*/1, /*wGrpIDHairColor*/1,
			/*dwMotionTypeID*/2, NULL, dwCharID);

		if (nMakeResult != MAKECHARRES_OK) {
			const char *pszError = (nMakeResult == MAKECHARRES_NG_USE) ? "char_name_exists" : "invalid_char_name";
			SetErrorResponse(response, "HTTP/1.1 409 Conflict", pszError);
			return;
		}

		// アカウントのキャラ枠へ登録(CLibInfoCharSvr::Enter中には行わない)
		pAccountLib->Enter();
		PCInfoAccount pAccountForAdd = pAccountLib->GetPtr((DWORD)dwAccountID);
		if (pAccountForAdd != NULL) {
			pAccountForAdd->m_adwCharID.push_back(dwCharID);
		}
		pAccountLib->Leave();
	} else {
		pCharLib->Enter();
		CInfoCharBase *pChar = FindCharByID(pCharLib, dwCharID);
		if (pChar != NULL) {
			strResolvedCharNameUtf8 = pChar->m_strCharName.GetUtf8Pointer();
		}
		pCharLib->Leave();
	}

	if (m_pMgrData->GetLog() != NULL) {
		m_pMgrData->GetLog()->Write(
			"[DebugFixture] success account=%s accountId=%u charId=%u newAccount=%d newChar=%d",
			strAccount.c_str(), dwAccountID, (unsigned int)dwCharID, bNewAccount ? 1 : 0, bHasChar ? 0 : 1);
	}

	std::ostringstream oss;
	oss << "{"
	    << "\"account\":\"" << JsonUtils::Escape(strAccount) << "\","
	    << "\"password\":\"" << JsonUtils::Escape(strPassword) << "\","
	    << "\"deviceToken\":\"" << JsonUtils::Escape(strToken) << "\","
	    << "\"accountId\":" << dwAccountID << ","
	    << "\"charId\":" << dwCharID << ","
	    << "\"charName\":\"" << JsonUtils::Escape(strResolvedCharNameUtf8) << "\""
	    << "}";
	response.statusLine = "HTTP/1.1 200 OK";
	response.SetJsonBody(oss.str());
}

#endif // _DEBUG
