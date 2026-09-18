/// @file TestAccountAuthStore.cpp
/// @brief ログインコード・端末トークンの SQLite 永続化(CAccountAuthStore)のテスト
/// @copyright Copyright(C)URARA-works
///
/// 実際に SboSvrTest.exe 自身の SBODATA\SboData.db(本番のSboSvrとは別ファイル、
/// exe相対パスで分離される)へ読み書きする。テストごとに専用のアカウントIDを
/// 使い、テスト開始時に前回分を掃除してから検証する。
///
/// 確認する項目: 発行・照合・再発行で旧トークンが失効すること・ログアウト。

#include "StdAfx.h"
#include "TestFramework.h"
#include "Account/AccountAuthStore.h"
#include "Account/LoginCode.h"
#include <string>
#include <vector>

namespace
{
	// 実データと衝突しないよう、テスト専用の大きなアカウントIDを使う
	const unsigned int kTestAccountA = 900001;
	const unsigned int kTestAccountB = 900002;

	void CleanupAccount(CAccountAuthStore &store, unsigned int dwAccountID)
	{
		store.DeleteAllDevicesForAccount(dwAccountID);
		// sys_account_code は AccountID が PRIMARY KEY なので、
		// ダミーのコードで上書き発行しておけば次のテストの邪魔にならない。
		store.IssueCode(dwAccountID, "cleanup-placeholder-hash", 1);
	}
}

TEST(発行_コードのハッシュからアカウントIDを引ける)
{
	CAccountAuthStore store;
	CleanupAccount(store, kTestAccountA);

	std::string strCode = LoginCode::GenerateCode();
	std::string strHash = LoginCode::HashCode(strCode);

	CHECK(store.IssueCode(kTestAccountA, strHash, 1000) != false);

	unsigned int dwFound = 0;
	CHECK(store.FindAccountIDByCodeHash(strHash, dwFound) != false);
	CHECK_EQ((int)kTestAccountA, (int)dwFound);

	CleanupAccount(store, kTestAccountA);
}

TEST(発行_未知のハッシュは見つからない)
{
	CAccountAuthStore store;
	unsigned int dwFound = 0;
	CHECK(store.FindAccountIDByCodeHash("no-such-hash-xyz", dwFound) == false);
}

TEST(再発行_古いコードは無効になる)
{
	CAccountAuthStore store;
	CleanupAccount(store, kTestAccountA);

	std::string strOldCode = LoginCode::GenerateCode();
	std::string strOldHash = LoginCode::HashCode(strOldCode);
	CHECK(store.IssueCode(kTestAccountA, strOldHash, 1000) != false);

	std::string strNewCode = LoginCode::GenerateCode();
	std::string strNewHash = LoginCode::HashCode(strNewCode);
	CHECK(store.IssueCode(kTestAccountA, strNewHash, 2000) != false);

	unsigned int dwFound = 0;
	CHECK(store.FindAccountIDByCodeHash(strOldHash, dwFound) == false);
	CHECK(store.FindAccountIDByCodeHash(strNewHash, dwFound) != false);
	CHECK_EQ((int)kTestAccountA, (int)dwFound);

	CleanupAccount(store, kTestAccountA);
}

TEST(端末トークン_発行したトークンでアカウントを引ける)
{
	CAccountAuthStore store;
	CleanupAccount(store, kTestAccountA);

	std::string strToken = LoginCode::GenerateDeviceToken();
	std::string strTokenHash = LoginCode::HashDeviceToken(strToken);
	CHECK(store.CreateDevice(kTestAccountA, strTokenHash, "テスト端末", 1000) != false);

	unsigned int dwFound = 0;
	CHECK(store.TouchDevice(strTokenHash, 1500, dwFound) != false);
	CHECK_EQ((int)kTestAccountA, (int)dwFound);

	CleanupAccount(store, kTestAccountA);
}

TEST(端末トークン_未知のトークンは見つからない)
{
	CAccountAuthStore store;
	unsigned int dwFound = 0;
	CHECK(store.TouchDevice("no-such-token-hash-xyz", 1000, dwFound) == false);
}

TEST(再発行_全端末トークンが失効する)
{
	CAccountAuthStore store;
	CleanupAccount(store, kTestAccountA);

	std::string strCode = LoginCode::GenerateCode();
	CHECK(store.IssueCode(kTestAccountA, LoginCode::HashCode(strCode), 1000) != false);

	std::string strTokenA = LoginCode::GenerateDeviceToken();
	std::string strTokenB = LoginCode::GenerateDeviceToken();
	std::string strHashA = LoginCode::HashDeviceToken(strTokenA);
	std::string strHashB = LoginCode::HashDeviceToken(strTokenB);
	CHECK(store.CreateDevice(kTestAccountA, strHashA, "端末A", 1000) != false);
	CHECK(store.CreateDevice(kTestAccountA, strHashB, "端末B", 1000) != false);

	unsigned int dwFound = 0;
	CHECK(store.TouchDevice(strHashA, 1100, dwFound) != false);
	CHECK(store.TouchDevice(strHashB, 1100, dwFound) != false);

	// 再発行(パスワードでの issue-code 相当)で両方失効するはず
	std::string strNewCode = LoginCode::GenerateCode();
	CHECK(store.IssueCode(kTestAccountA, LoginCode::HashCode(strNewCode), 2000) != false);

	CHECK(store.TouchDevice(strHashA, 2100, dwFound) == false);
	CHECK(store.TouchDevice(strHashB, 2100, dwFound) == false);

	CleanupAccount(store, kTestAccountA);
}

TEST(ログアウト_該当トークンだけ失効しほかは残る)
{
	CAccountAuthStore store;
	CleanupAccount(store, kTestAccountA);
	CleanupAccount(store, kTestAccountB);

	std::string strTokenA = LoginCode::GenerateDeviceToken();
	std::string strTokenB = LoginCode::GenerateDeviceToken();
	std::string strHashA = LoginCode::HashDeviceToken(strTokenA);
	std::string strHashB = LoginCode::HashDeviceToken(strTokenB);
	CHECK(store.CreateDevice(kTestAccountA, strHashA, "端末A", 1000) != false);
	CHECK(store.CreateDevice(kTestAccountB, strHashB, "端末B", 1000) != false);

	CHECK(store.DeleteDeviceByTokenHash(strHashA) != false);

	unsigned int dwFound = 0;
	CHECK(store.TouchDevice(strHashA, 1100, dwFound) == false);
	CHECK(store.TouchDevice(strHashB, 1100, dwFound) != false);

	CleanupAccount(store, kTestAccountA);
	CleanupAccount(store, kTestAccountB);
}

TEST(ログアウト_存在しないトークンでも失敗にしない)
{
	CAccountAuthStore store;
	CHECK(store.DeleteDeviceByTokenHash("no-such-token-for-logout") != false);
}

//////////////////////////////////////////////////////////////////////
// 孤児行の掃除(PruneOrphanedAuth)
//
// register はメモリへの Add のみで sys_account への保存は次の定期保存まで
// 行われない。その前にサーバーが落ちると sys_account_code 側にだけ行が
// 残り、再起動後に同じ AccountID が別の新規アカウントに割り当てられて、
// 古いコードで他人のアカウントに入れてしまう恐れがある。起動時にこれで
// 掃除する(呼び出し元は MgrData.cpp のアカウント読込直後)。
//////////////////////////////////////////////////////////////////////

TEST(孤児行掃除_有効なAccountIDに無い行は削除される)
{
	CAccountAuthStore store;
	const unsigned int kOrphan = 900101;
	CleanupAccount(store, kOrphan);

	std::string strCode = LoginCode::GenerateCode();
	std::string strCodeHash = LoginCode::HashCode(strCode);
	CHECK(store.IssueCode(kOrphan, strCodeHash, 1000) != false);

	std::string strToken = LoginCode::GenerateDeviceToken();
	std::string strTokenHash = LoginCode::HashDeviceToken(strToken);
	CHECK(store.CreateDevice(kOrphan, strTokenHash, "孤児端末", 1000) != false);

	// kOrphan を含まない有効一覧で掃除する
	std::vector<unsigned int> validIDs;
	validIDs.push_back(1);	// kOrphan とは無関係な適当なID
	CHECK(store.PruneOrphanedAuth(validIDs) != false);

	unsigned int dwFound = 0;
	CHECK(store.FindAccountIDByCodeHash(strCodeHash, dwFound) == false);
	CHECK(store.TouchDevice(strTokenHash, 1100, dwFound) == false);
}

TEST(孤児行掃除_有効なAccountIDの行は残る)
{
	CAccountAuthStore store;
	const unsigned int kKeep = 900102;
	CleanupAccount(store, kKeep);

	std::string strCode = LoginCode::GenerateCode();
	std::string strCodeHash = LoginCode::HashCode(strCode);
	CHECK(store.IssueCode(kKeep, strCodeHash, 1000) != false);

	std::string strToken = LoginCode::GenerateDeviceToken();
	std::string strTokenHash = LoginCode::HashDeviceToken(strToken);
	CHECK(store.CreateDevice(kKeep, strTokenHash, "端末", 1000) != false);

	std::vector<unsigned int> validIDs;
	validIDs.push_back(kKeep);
	CHECK(store.PruneOrphanedAuth(validIDs) != false);

	unsigned int dwFound = 0;
	CHECK(store.FindAccountIDByCodeHash(strCodeHash, dwFound) != false);
	CHECK_EQ((int)kKeep, (int)dwFound);
	CHECK(store.TouchDevice(strTokenHash, 1100, dwFound) != false);

	CleanupAccount(store, kKeep);
}

TEST(孤児行掃除_有効一覧が空なら全行削除される)
{
	CAccountAuthStore store;
	const unsigned int kOrphan = 900103;
	CleanupAccount(store, kOrphan);

	std::string strCode = LoginCode::GenerateCode();
	std::string strCodeHash = LoginCode::HashCode(strCode);
	CHECK(store.IssueCode(kOrphan, strCodeHash, 1000) != false);

	std::vector<unsigned int> emptyValidIDs;
	CHECK(store.PruneOrphanedAuth(emptyValidIDs) != false);

	unsigned int dwFound = 0;
	CHECK(store.FindAccountIDByCodeHash(strCodeHash, dwFound) == false);
}
