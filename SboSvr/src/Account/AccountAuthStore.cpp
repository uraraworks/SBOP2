/// @file AccountAuthStore.cpp
/// @brief ログインコード・端末トークンの SQLite 永続化 実装ファイル

#include "StdAfx.h"
#include "../../third_party/sqlite/sqlite3.h"
#include "AccountAuthStore.h"
#include "../Platform/SvrPlatform.h"

CAccountAuthStore::CAccountAuthStore()
{
}

CAccountAuthStore::~CAccountAuthStore()
{
}

bool CAccountAuthStore::OpenDb(sqlite3 **ppDb)
{
	if (ppDb == NULL) return false;

	// CSaveLoadInfoMapGenPattern::OpenDb と同じ、共有ファイル SboData.db を直接開く。
	std::string strDbPath = SboPlatform::MakeDataFilePath("SboData.db");
	const char *szDbPath = strDbPath.c_str();

	int nRet = sqlite3_open(szDbPath, ppDb);
	if (nRet != SQLITE_OK) {
		SboPlatform::WriteDebugLine("CAccountAuthStore::OpenDb: sqlite3_open failed\n");
		*ppDb = NULL;
		return false;
	}

	// 同じ SboData.db(WAL) にゲーム本体の定期保存(CMgrData::Save/Load)が書き込むため、
	// 相手が書き込み中の瞬間に当たると SQLITE_BUSY になりうる。即エラーにせず、
	// ロックが空くまで最大3秒だけ内部でリトライさせる。
	sqlite3_busy_timeout(*ppDb, 3000);

	sqlite3_exec(*ppDb, "PRAGMA journal_mode=WAL; PRAGMA synchronous=NORMAL;", NULL, NULL, NULL);
	return true;
}

void CAccountAuthStore::EnsureTables(sqlite3 *pDb)
{
	if (pDb == NULL) return;

	const char *pszCodeSql =
		"CREATE TABLE IF NOT EXISTS sys_account_code("
		"  AccountID  INTEGER PRIMARY KEY,"
		"  CodeHash   TEXT NOT NULL,"
		"  TimeIssued INTEGER"
		");";
	sqlite3_exec(pDb, pszCodeSql, NULL, NULL, NULL);

	// CodeHash からアカウントIDを逆引きするための索引(総当たり照合の主経路)
	sqlite3_exec(pDb,
		"CREATE UNIQUE INDEX IF NOT EXISTS idx_sys_account_code_hash ON sys_account_code(CodeHash);",
		NULL, NULL, NULL);

	const char *pszDeviceSql =
		"CREATE TABLE IF NOT EXISTS sys_account_device("
		"  TokenHash    TEXT PRIMARY KEY,"
		"  AccountID    INTEGER NOT NULL,"
		"  TimeCreated  INTEGER,"
		"  TimeLastUsed INTEGER,"
		"  Label        TEXT"
		");";
	sqlite3_exec(pDb, pszDeviceSql, NULL, NULL, NULL);

	sqlite3_exec(pDb,
		"CREATE INDEX IF NOT EXISTS idx_sys_account_device_account ON sys_account_device(AccountID);",
		NULL, NULL, NULL);
}

bool CAccountAuthStore::IssueCode(unsigned int dwAccountID, const std::string &strCodeHash, long lTimeIssued)
{
	sqlite3 *pDb = NULL;
	if (!OpenDb(&pDb)) return false;

	EnsureTables(pDb);

	int nBeginRet = sqlite3_exec(pDb, "BEGIN IMMEDIATE;", NULL, NULL, NULL);
	if (nBeginRet != SQLITE_OK) {
		// busy_timeout(3000ms) の再試行後もロックが取れなかった等。
		// トランザクションに入れていないので ROLLBACK は不要。
		SboPlatform::WriteDebugLine("CAccountAuthStore::IssueCode: BEGIN IMMEDIATE failed\n");
		sqlite3_close(pDb);
		return false;
	}

	bool bOk = true;

	// 既存の端末トークンを全て失効させる(再発行時の要件。新規発行時は0件でも問題ない)
	{
		const char *pszDelDevice = "DELETE FROM sys_account_device WHERE AccountID=?;";
		sqlite3_stmt *pStmt = NULL;
		if (sqlite3_prepare_v2(pDb, pszDelDevice, -1, &pStmt, NULL) == SQLITE_OK) {
			sqlite3_bind_int(pStmt, 1, (int)dwAccountID);
			bOk = (sqlite3_step(pStmt) == SQLITE_DONE);
			sqlite3_finalize(pStmt);
		} else {
			bOk = false;
		}
	}

	// 既存のコードを消してから入れ直す(UPSERTに頼らず DELETE+INSERT で互換性を保つ)
	if (bOk) {
		const char *pszDelCode = "DELETE FROM sys_account_code WHERE AccountID=?;";
		sqlite3_stmt *pStmt = NULL;
		if (sqlite3_prepare_v2(pDb, pszDelCode, -1, &pStmt, NULL) == SQLITE_OK) {
			sqlite3_bind_int(pStmt, 1, (int)dwAccountID);
			bOk = (sqlite3_step(pStmt) == SQLITE_DONE);
			sqlite3_finalize(pStmt);
		} else {
			bOk = false;
		}
	}

	if (bOk) {
		const char *pszInsCode =
			"INSERT INTO sys_account_code(AccountID, CodeHash, TimeIssued) VALUES(?, ?, ?);";
		sqlite3_stmt *pStmt = NULL;
		if (sqlite3_prepare_v2(pDb, pszInsCode, -1, &pStmt, NULL) == SQLITE_OK) {
			sqlite3_bind_int(pStmt, 1, (int)dwAccountID);
			sqlite3_bind_text(pStmt, 2, strCodeHash.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(pStmt, 3, (int)lTimeIssued);
			bOk = (sqlite3_step(pStmt) == SQLITE_DONE);
			sqlite3_finalize(pStmt);
		} else {
			bOk = false;
		}
	}

	sqlite3_exec(pDb, bOk ? "COMMIT;" : "ROLLBACK;", NULL, NULL, NULL);
	sqlite3_close(pDb);
	return bOk;
}

bool CAccountAuthStore::FindAccountIDByCodeHash(const std::string &strCodeHash, unsigned int &outAccountID)
{
	outAccountID = 0;

	sqlite3 *pDb = NULL;
	if (!OpenDb(&pDb)) return false;

	EnsureTables(pDb);

	const char *pszSql = "SELECT AccountID FROM sys_account_code WHERE CodeHash=?;";
	sqlite3_stmt *pStmt = NULL;
	if (sqlite3_prepare_v2(pDb, pszSql, -1, &pStmt, NULL) != SQLITE_OK) {
		sqlite3_close(pDb);
		return false;
	}

	sqlite3_bind_text(pStmt, 1, strCodeHash.c_str(), -1, SQLITE_TRANSIENT);

	bool bFound = false;
	if (sqlite3_step(pStmt) == SQLITE_ROW) {
		outAccountID = (unsigned int)sqlite3_column_int(pStmt, 0);
		bFound = true;
	}

	sqlite3_finalize(pStmt);
	sqlite3_close(pDb);
	return bFound;
}

bool CAccountAuthStore::CreateDevice(unsigned int dwAccountID, const std::string &strTokenHash,
	const std::string &strLabel, long lTimeCreated)
{
	sqlite3 *pDb = NULL;
	if (!OpenDb(&pDb)) return false;

	EnsureTables(pDb);

	const char *pszSql =
		"INSERT INTO sys_account_device(TokenHash, AccountID, TimeCreated, TimeLastUsed, Label)"
		" VALUES(?, ?, ?, ?, ?);";
	sqlite3_stmt *pStmt = NULL;
	if (sqlite3_prepare_v2(pDb, pszSql, -1, &pStmt, NULL) != SQLITE_OK) {
		sqlite3_close(pDb);
		return false;
	}

	sqlite3_bind_text(pStmt, 1, strTokenHash.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(pStmt, 2, (int)dwAccountID);
	sqlite3_bind_int(pStmt, 3, (int)lTimeCreated);
	sqlite3_bind_int(pStmt, 4, (int)lTimeCreated);
	sqlite3_bind_text(pStmt, 5, strLabel.c_str(), -1, SQLITE_TRANSIENT);

	bool bOk = (sqlite3_step(pStmt) == SQLITE_DONE);

	sqlite3_finalize(pStmt);
	sqlite3_close(pDb);
	return bOk;
}

bool CAccountAuthStore::TouchDevice(const std::string &strTokenHash, long lTimeNow, unsigned int &outAccountID)
{
	outAccountID = 0;

	sqlite3 *pDb = NULL;
	if (!OpenDb(&pDb)) return false;

	EnsureTables(pDb);

	bool bFound = false;
	{
		const char *pszSelSql = "SELECT AccountID FROM sys_account_device WHERE TokenHash=?;";
		sqlite3_stmt *pStmt = NULL;
		if (sqlite3_prepare_v2(pDb, pszSelSql, -1, &pStmt, NULL) == SQLITE_OK) {
			sqlite3_bind_text(pStmt, 1, strTokenHash.c_str(), -1, SQLITE_TRANSIENT);
			if (sqlite3_step(pStmt) == SQLITE_ROW) {
				outAccountID = (unsigned int)sqlite3_column_int(pStmt, 0);
				bFound = true;
			}
			sqlite3_finalize(pStmt);
		}
	}

	if (bFound) {
		const char *pszUpdSql = "UPDATE sys_account_device SET TimeLastUsed=? WHERE TokenHash=?;";
		sqlite3_stmt *pStmt = NULL;
		if (sqlite3_prepare_v2(pDb, pszUpdSql, -1, &pStmt, NULL) == SQLITE_OK) {
			sqlite3_bind_int(pStmt, 1, (int)lTimeNow);
			sqlite3_bind_text(pStmt, 2, strTokenHash.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_step(pStmt);
			sqlite3_finalize(pStmt);
		}
	}

	sqlite3_close(pDb);
	return bFound;
}

bool CAccountAuthStore::DeleteDeviceByTokenHash(const std::string &strTokenHash)
{
	sqlite3 *pDb = NULL;
	if (!OpenDb(&pDb)) return false;

	EnsureTables(pDb);

	const char *pszSql = "DELETE FROM sys_account_device WHERE TokenHash=?;";
	sqlite3_stmt *pStmt = NULL;
	if (sqlite3_prepare_v2(pDb, pszSql, -1, &pStmt, NULL) != SQLITE_OK) {
		sqlite3_close(pDb);
		return false;
	}

	sqlite3_bind_text(pStmt, 1, strTokenHash.c_str(), -1, SQLITE_TRANSIENT);
	bool bOk = (sqlite3_step(pStmt) == SQLITE_DONE);

	sqlite3_finalize(pStmt);
	sqlite3_close(pDb);
	return bOk;
}

bool CAccountAuthStore::DeleteAllDevicesForAccount(unsigned int dwAccountID)
{
	sqlite3 *pDb = NULL;
	if (!OpenDb(&pDb)) return false;

	EnsureTables(pDb);

	const char *pszSql = "DELETE FROM sys_account_device WHERE AccountID=?;";
	sqlite3_stmt *pStmt = NULL;
	if (sqlite3_prepare_v2(pDb, pszSql, -1, &pStmt, NULL) != SQLITE_OK) {
		sqlite3_close(pDb);
		return false;
	}

	sqlite3_bind_int(pStmt, 1, (int)dwAccountID);
	bool bOk = (sqlite3_step(pStmt) == SQLITE_DONE);

	sqlite3_finalize(pStmt);
	sqlite3_close(pDb);
	return bOk;
}

bool CAccountAuthStore::DeleteCodeForAccount(unsigned int dwAccountID)
{
	sqlite3 *pDb = NULL;
	if (!OpenDb(&pDb)) return false;

	EnsureTables(pDb);

	const char *pszSql = "DELETE FROM sys_account_code WHERE AccountID=?;";
	sqlite3_stmt *pStmt = NULL;
	if (sqlite3_prepare_v2(pDb, pszSql, -1, &pStmt, NULL) != SQLITE_OK) {
		sqlite3_close(pDb);
		return false;
	}

	sqlite3_bind_int(pStmt, 1, (int)dwAccountID);
	bool bOk = (sqlite3_step(pStmt) == SQLITE_DONE);

	sqlite3_finalize(pStmt);
	sqlite3_close(pDb);
	return bOk;
}

bool CAccountAuthStore::PruneOrphanedAuth(const std::vector<unsigned int> &validAccountIDs,
	int *outDeletedCodeRows, int *outDeletedDeviceRows)
{
	if (outDeletedCodeRows != NULL) *outDeletedCodeRows = 0;
	if (outDeletedDeviceRows != NULL) *outDeletedDeviceRows = 0;

	sqlite3 *pDb = NULL;
	if (!OpenDb(&pDb)) return false;

	EnsureTables(pDb);

	int nBeginRet = sqlite3_exec(pDb, "BEGIN IMMEDIATE;", NULL, NULL, NULL);
	if (nBeginRet != SQLITE_OK) {
		SboPlatform::WriteDebugLine("CAccountAuthStore::PruneOrphanedAuth: BEGIN IMMEDIATE failed\n");
		sqlite3_close(pDb);
		return false;
	}

	// 有効な AccountID の一覧を一時テーブルへ入れ、NOT IN のサブクエリで比較する。
	// validAccountIDs が空(=アカウント0件)でも、空の一時テーブルに対する
	// "NOT IN (SELECT ...)" は全行が対象になり、意図どおり全削除される。
	bool bOk = (sqlite3_exec(pDb,
		"CREATE TEMP TABLE IF NOT EXISTS tmp_valid_account(AccountID INTEGER PRIMARY KEY);",
		NULL, NULL, NULL) == SQLITE_OK);

	if (bOk) {
		bOk = (sqlite3_exec(pDb, "DELETE FROM tmp_valid_account;", NULL, NULL, NULL) == SQLITE_OK);
	}

	if (bOk && !validAccountIDs.empty()) {
		sqlite3_stmt *pInsStmt = NULL;
		if (sqlite3_prepare_v2(pDb, "INSERT OR IGNORE INTO tmp_valid_account(AccountID) VALUES(?);",
			-1, &pInsStmt, NULL) == SQLITE_OK) {
			for (size_t i = 0; i < validAccountIDs.size(); ++i) {
				sqlite3_bind_int(pInsStmt, 1, (int)validAccountIDs[i]);
				if (sqlite3_step(pInsStmt) != SQLITE_DONE) {
					bOk = false;
				}
				sqlite3_reset(pInsStmt);
			}
			sqlite3_finalize(pInsStmt);
		} else {
			bOk = false;
		}
	}

	if (bOk) {
		bOk = (sqlite3_exec(pDb,
			"DELETE FROM sys_account_device WHERE AccountID NOT IN (SELECT AccountID FROM tmp_valid_account);",
			NULL, NULL, NULL) == SQLITE_OK);
		if (bOk && (outDeletedDeviceRows != NULL)) {
			*outDeletedDeviceRows = sqlite3_changes(pDb);
		}
	}

	if (bOk) {
		bOk = (sqlite3_exec(pDb,
			"DELETE FROM sys_account_code WHERE AccountID NOT IN (SELECT AccountID FROM tmp_valid_account);",
			NULL, NULL, NULL) == SQLITE_OK);
		if (bOk && (outDeletedCodeRows != NULL)) {
			*outDeletedCodeRows = sqlite3_changes(pDb);
		}
	}

	sqlite3_exec(pDb, "DROP TABLE IF EXISTS tmp_valid_account;", NULL, NULL, NULL);
	sqlite3_exec(pDb, bOk ? "COMMIT;" : "ROLLBACK;", NULL, NULL, NULL);
	sqlite3_close(pDb);
	return bOk;
}
