/// @file TestLoginCode.cpp
/// @brief ログインコード・端末トークンの生成/正規化/ハッシュ のテスト
/// @copyright Copyright(C)URARA-works
///
/// docs/login-code-auth-plan.md の「コードとトークンの仕様」節を確認する。
/// 確認する項目: 生成の文字種と長さ / 正規化 / ハッシュ照合 / NULL等の異常系。
/// 再発行で旧トークンが失効することと試行制限は TestAccountAuthStore.cpp /
/// TestIpRateLimiter.cpp で確認する。

#include "StdAfx.h"
#include "TestFramework.h"
#include "Account/LoginCode.h"
#include "PasswordHash.h"
#include <string>
#include <cstring>

namespace
{
	bool IsCrockfordBase32Char(char c)
	{
		static const char kAlphabet[] = "0123456789ABCDEFGHJKMNPQRSTVWXYZ";
		return std::strchr(kAlphabet, c) != NULL;
	}
}

//////////////////////////////////////////////////////////////////////
// コード生成
//////////////////////////////////////////////////////////////////////

TEST(コード生成_長さは16文字)
{
	std::string strCode = LoginCode::GenerateCode();
	CHECK(strCode.empty() == false);
	CHECK_EQ(16, (int)strCode.size());
}

TEST(コード生成_文字種はCrockfordBase32のみ)
{
	std::string strCode = LoginCode::GenerateCode();
	for (size_t i = 0; i < strCode.size(); i ++) {
		CHECK(IsCrockfordBase32Char(strCode[i]) != false);
	}
	// I, L, O, U は使わない
	CHECK(strCode.find('I') == std::string::npos);
	CHECK(strCode.find('L') == std::string::npos);
	CHECK(strCode.find('O') == std::string::npos);
	CHECK(strCode.find('U') == std::string::npos);
}

TEST(コード生成_毎回変わる)
{
	std::string strA = LoginCode::GenerateCode();
	std::string strB = LoginCode::GenerateCode();
	CHECK(strA.empty() == false);
	CHECK(strB.empty() == false);
	CHECK(strA != strB);
}

TEST(コード生成_表示用フォーマットは4文字ごとにハイフン)
{
	std::string strCode = LoginCode::GenerateCode();
	std::string strDisplay = LoginCode::FormatCodeForDisplay(strCode);

	CHECK_EQ(19, (int)strDisplay.size());	// 16文字 + ハイフン3個
	CHECK_EQ('-', strDisplay[4]);
	CHECK_EQ('-', strDisplay[9]);
	CHECK_EQ('-', strDisplay[14]);
}

//////////////////////////////////////////////////////////////////////
// 正規化
//////////////////////////////////////////////////////////////////////

TEST(正規化_ハイフンと空白を除去する)
{
	bool bValid = false;
	std::string strResult = LoginCode::NormalizeCode("K7QM-3XRA-9PWD-HT2F", bValid);
	CHECK(bValid != false);
	CHECK(strResult == "K7QM3XRA9PWDHT2F");
}

TEST(正規化_小文字を大文字にそろえる)
{
	bool bValid = false;
	std::string strResult = LoginCode::NormalizeCode("k7qm3xra9pwdht2f", bValid);
	CHECK(bValid != false);
	CHECK(strResult == "K7QM3XRA9PWDHT2F");
}

TEST(正規化_Oは0にIとLは1に同一視する)
{
	bool bValid = false;
	// "O" -> "0", "I" / "L" -> "1" になるはずの入力を作る
	std::string strResult = LoginCode::NormalizeCode("OOOOIIIILLLL0000", bValid);
	CHECK(bValid != false);
	CHECK(strResult == "0000111111110000");
}

TEST(正規化_空白タブ改行も除去する)
{
	bool bValid = false;
	std::string strResult = LoginCode::NormalizeCode(" K7QM 3XRA\t9PWD\nHT2F ", bValid);
	CHECK(bValid != false);
	CHECK(strResult == "K7QM3XRA9PWDHT2F");
}

TEST(正規化_長さが16文字でなければ無効)
{
	bool bValid = true;
	std::string strResult = LoginCode::NormalizeCode("K7QM3XRA9PWDHT2", bValid);	// 15文字
	CHECK(bValid == false);
	CHECK(strResult.empty() != false);

	bValid = true;
	strResult = LoginCode::NormalizeCode("K7QM3XRA9PWDHT2FF", bValid);	// 17文字
	CHECK(bValid == false);
}

TEST(正規化_Uは使えない文字なので無効)
{
	bool bValid = true;
	std::string strResult = LoginCode::NormalizeCode("UUUU3XRA9PWDHT2F", bValid);
	CHECK(bValid == false);
}

TEST(正規化_記号や全角混じりは無効)
{
	bool bValid = true;
	std::string strResult = LoginCode::NormalizeCode("K7QM!3XRA9PWDHT2", bValid);
	CHECK(bValid == false);
}

TEST(正規化_空文字列は無効)
{
	bool bValid = true;
	std::string strResult = LoginCode::NormalizeCode("", bValid);
	CHECK(bValid == false);
}

//////////////////////////////////////////////////////////////////////
// ハッシュ
//////////////////////////////////////////////////////////////////////

TEST(コードハッシュ_同じ正規化済みコードは同じハッシュになる)
{
	std::string strHashA = LoginCode::HashCode("K7QM3XRA9PWDHT2F");
	std::string strHashB = LoginCode::HashCode("K7QM3XRA9PWDHT2F");
	CHECK(strHashA == strHashB);
	CHECK_EQ(64, (int)strHashA.size());	// SHA-256の16進表記
}

TEST(コードハッシュ_異なる入力は異なるハッシュになる)
{
	std::string strHashA = LoginCode::HashCode("K7QM3XRA9PWDHT2F");
	std::string strHashB = LoginCode::HashCode("K7QM3XRA9PWDHT2G");
	CHECK(strHashA != strHashB);
}

TEST(コードハッシュ_正規化前後の表記ゆれは同じハッシュに揃う)
{
	bool bValid1 = false, bValid2 = false;
	std::string strNorm1 = LoginCode::NormalizeCode("K7QM-3XRA-9PWD-HT2F", bValid1);
	std::string strNorm2 = LoginCode::NormalizeCode("k7qm3xra9pwdht2f", bValid2);
	CHECK(bValid1 != false);
	CHECK(bValid2 != false);
	CHECK(LoginCode::HashCode(strNorm1) == LoginCode::HashCode(strNorm2));
}

//////////////////////////////////////////////////////////////////////
// 端末トークン
//////////////////////////////////////////////////////////////////////

TEST(トークン生成_16進64文字)
{
	std::string strToken = LoginCode::GenerateDeviceToken();
	CHECK(strToken.empty() == false);
	CHECK_EQ(64, (int)strToken.size());
	for (size_t i = 0; i < strToken.size(); i ++) {
		char c = strToken[i];
		bool bHexDigit = (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f');
		CHECK(bHexDigit != false);
	}
}

TEST(トークン生成_毎回変わる)
{
	std::string strA = LoginCode::GenerateDeviceToken();
	std::string strB = LoginCode::GenerateDeviceToken();
	CHECK(strA != strB);
}

TEST(トークンハッシュ_同じトークンは同じハッシュになる)
{
	std::string strToken = LoginCode::GenerateDeviceToken();
	CHECK(LoginCode::HashDeviceToken(strToken) == LoginCode::HashDeviceToken(strToken));
}

TEST(トークンハッシュ_前後の空白は無視する)
{
	std::string strToken = LoginCode::GenerateDeviceToken();
	CHECK(LoginCode::HashDeviceToken(strToken) == LoginCode::HashDeviceToken("  " + strToken + "\n"));
}

//////////////////////////////////////////////////////////////////////
// アカウント名の文字種チェック
//////////////////////////////////////////////////////////////////////

TEST(アカウント名_半角英数字と記号は使える)
{
	CHECK(LoginCode::IsAcceptableAccountName("player1") != false);
	CHECK(LoginCode::IsAcceptableAccountName("Player_01!") != false);
}

TEST(アカウント名_全角と空白は使えない)
{
	CHECK(LoginCode::IsAcceptableAccountName("ぷれいや") == false);
	CHECK(LoginCode::IsAcceptableAccountName("player 1") == false);
	CHECK(LoginCode::IsAcceptableAccountName("") == false);
}

TEST(アカウント名正規化_大文字は小文字になる)
{
	CHECK(LoginCode::NormalizeAccountName("Player1") == "player1");
	CHECK(LoginCode::NormalizeAccountName("PLAYER") == "player");
}

TEST(アカウント名正規化_既に小文字ならそのまま)
{
	CHECK(LoginCode::NormalizeAccountName("player1") == "player1");
}

TEST(アカウント名正規化_ASCII以外の記号は変更しない)
{
	CHECK(LoginCode::NormalizeAccountName("Player_01!") == "player_01!");
}

//////////////////////////////////////////////////////////////////////
// 未登録名でのログイン自動作成廃止(docs/login-code-auth-plan.md S4)
//////////////////////////////////////////////////////////////////////

TEST(未登録名ログイン_自動作成は廃止済み)
{
	// MainFrameRecvProcCONNECT.cpp の RecvProcCONNECT_REQ_LOGIN は、この値が
	// false であることを前提に「未登録なら LOGINRES_NG_PASSWORD」を返す。
	// ここを true に戻すとこのテストが落ちる。
	CHECK(LoginCode::ShouldAutoCreateAccountOnUnknownLogin() == false);
}

//////////////////////////////////////////////////////////////////////
// おまかせ登録: 自動生成の名前・パスワード
//////////////////////////////////////////////////////////////////////

TEST(自動生成アカウント名_形式はplayerハイフン英数字6文字)
{
	std::string strName = LoginCode::GenerateAutoAccountName();
	CHECK(strName.empty() == false);
	CHECK_EQ(13, (int)strName.size());	// "player-" 7文字 + 6文字
	CHECK(strName.substr(0, 7) == "player-");

	std::string strSuffix = strName.substr(7);
	CHECK_EQ(6, (int)strSuffix.size());
	for (size_t i = 0; i < strSuffix.size(); i ++) {
		char c = strSuffix[i];
		bool bDigit = (c >= '0') && (c <= '9');
		bool bLower = (c >= 'a') && (c <= 'z');
		CHECK(bDigit || bLower);
	}
	// 紛らわしい文字(I,L,O,Uの小文字)は使わない
	CHECK(strSuffix.find('i') == std::string::npos);
	CHECK(strSuffix.find('l') == std::string::npos);
	CHECK(strSuffix.find('o') == std::string::npos);
	CHECK(strSuffix.find('u') == std::string::npos);
}

TEST(自動生成アカウント名_登録API入口の検証を通る)
{
	std::string strName = LoginCode::GenerateAutoAccountName();
	CHECK(LoginCode::IsAcceptableAccountName(strName) != false);
	// 既に小文字化済みのはず(正規化しても変わらない)
	CHECK(LoginCode::NormalizeAccountName(strName) == strName);
}

TEST(自動生成アカウント名_毎回変わる)
{
	std::string strA = LoginCode::GenerateAutoAccountName();
	std::string strB = LoginCode::GenerateAutoAccountName();
	CHECK(strA.empty() == false);
	CHECK(strB.empty() == false);
	CHECK(strA != strB);
}

TEST(自動生成パスワード_長さは12文字)
{
	std::string strPassword = LoginCode::GenerateAutoPassword();
	CHECK(strPassword.empty() == false);
	CHECK_EQ(12, (int)strPassword.size());
}

TEST(自動生成パスワード_PasswordHashの検証を通る)
{
	std::string strPassword = LoginCode::GenerateAutoPassword();
	CHECK(PasswordHash::IsAcceptable(strPassword.c_str()) != false);
}

TEST(自動生成パスワード_毎回変わる)
{
	std::string strA = LoginCode::GenerateAutoPassword();
	std::string strB = LoginCode::GenerateAutoPassword();
	CHECK(strA.empty() == false);
	CHECK(strB.empty() == false);
	CHECK(strA != strB);
}
