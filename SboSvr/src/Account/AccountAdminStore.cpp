/// @file AccountAdminStore.cpp
/// @brief アカウントのゴミ箱/BAN状態を保持する sys_account_admin テーブルの永続化 実装ファイル

#include "StdAfx.h"
#include "../../third_party/sqlite/sqlite3.h"
#include "AccountAdminStore.h"
#include "../Platform/SvrPlatform.h"

CAccountAdminStore::CAccountAdminStore()
{
}

CAccountAdminStore::~CAccountAdminStore()
{
}

bool CAccountAdminStore::OpenDb(sqlite3 **ppDb)
{
	if (ppDb == NULL) return false;

	// CAccountAuthStore::OpenDb と同じ、共有ファイル SboData.db を直接開く。
	std::string strDbPath = SboPlatform::MakeDataFilePath("SboData.db");
	const char *szDbPath = strDbPath.c_str();

	int nRet = sqlite3_open(szDbPath, ppDb);
	if (nRet != SQLITE_OK) {
		SboPlatform::WriteDebugLine("CAccountAdminStore::OpenDb: sqlite3_open failed\n");
		*ppDb = NULL;
		return false;
	}

	// ゲーム本体の定期保存(CMgrData::Save/Load)と同じ SboData.db(WAL) を触るため、
	// 相手が書き込み中の瞬間に当たると SQLITE_BUSY になりうる。即エラーにせず、
	// ロックが空くまで最大3秒だけ内部でリトライさせる。
	sqlite3_busy_timeout(*ppDb, 3000);

	sqlite3_exec(*ppDb, "PRAGMA journal_mode=WAL; PRAGMA synchronous=NORMAL;", NULL, NULL, NULL);
	return true;
}

void CAccountAdminStore::EnsureTables(sqlite3 *pDb)
{
	if (pDb == NULL) return;

	const char *pszSql =
		"CREATE TABLE IF NOT EXISTS sys_account_admin("
		"  AccountID      INTEGER PRIMARY KEY,"
		"  Status         TEXT NOT NULL,"
		"  Reason         TEXT,"
		"  TimeChanged    INTEGER,"
		"  ActorAccountID INTEGER,"
		"  PrevDisable    INTEGER"
		");";
	sqlite3_exec(pDb, pszSql, NULL, NULL, NULL);

	// 開発環境の既存DBは列追加前に作成済みのため、CREATE TABLE IF NOT EXISTS
	// だけでは PrevStatus/PrevReason 列が増えない。PRAGMA table_info で
	// 既存列を調べ、無ければ ALTER TABLE で追加する(既存行は壊さない)。
	bool bHasPrevStatus = false;
	bool bHasPrevReason = false;
	{
		sqlite3_stmt *pStmt = NULL;
		if (sqlite3_prepare_v2(pDb, "PRAGMA table_info(sys_account_admin);", -1, &pStmt, NULL) == SQLITE_OK) {
			while (sqlite3_step(pStmt) == SQLITE_ROW) {
				// カラム: cid, name, type, notnull, dflt_value, pk
				const unsigned char *pszName = sqlite3_column_text(pStmt, 1);
				if (pszName == NULL) continue;
				std::string strName(reinterpret_cast<const char *>(pszName));
				if (strName == "PrevStatus") bHasPrevStatus = true;
				if (strName == "PrevReason") bHasPrevReason = true;
			}
			sqlite3_finalize(pStmt);
		}
	}

	if (!bHasPrevStatus) {
		sqlite3_exec(pDb, "ALTER TABLE sys_account_admin ADD COLUMN PrevStatus TEXT;", NULL, NULL, NULL);
	}
	if (!bHasPrevReason) {
		sqlite3_exec(pDb, "ALTER TABLE sys_account_admin ADD COLUMN PrevReason TEXT;", NULL, NULL, NULL);
	}
}

bool CAccountAdminStore::LoadAll(std::vector<AccountAdminRow> &outRows)
{
	outRows.clear();

	sqlite3 *pDb = NULL;
	if (!OpenDb(&pDb)) return false;

	EnsureTables(pDb);

	const char *pszSql =
		"SELECT AccountID, Status, Reason, TimeChanged, ActorAccountID, PrevDisable,"
		" PrevStatus, PrevReason"
		" FROM sys_account_admin;";
	sqlite3_stmt *pStmt = NULL;
	if (sqlite3_prepare_v2(pDb, pszSql, -1, &pStmt, NULL) != SQLITE_OK) {
		sqlite3_close(pDb);
		return false;
	}

	while (sqlite3_step(pStmt) == SQLITE_ROW) {
		AccountAdminRow row;
		row.dwAccountID = (unsigned int)sqlite3_column_int(pStmt, 0);
		const unsigned char *pszStatus = sqlite3_column_text(pStmt, 1);
		row.strStatus = (pszStatus != NULL) ? std::string(reinterpret_cast<const char *>(pszStatus)) : std::string();
		const unsigned char *pszReason = sqlite3_column_text(pStmt, 2);
		row.strReason = (pszReason != NULL) ? std::string(reinterpret_cast<const char *>(pszReason)) : std::string();
		row.lTimeChanged = (long)sqlite3_column_int(pStmt, 3);
		row.dwActorAccountID = (unsigned int)sqlite3_column_int(pStmt, 4);
		row.nPrevDisable = sqlite3_column_int(pStmt, 5);
		const unsigned char *pszPrevStatus = sqlite3_column_text(pStmt, 6);
		row.strPrevStatus = (pszPrevStatus != NULL) ? std::string(reinterpret_cast<const char *>(pszPrevStatus)) : std::string();
		const unsigned char *pszPrevReason = sqlite3_column_text(pStmt, 7);
		row.strPrevReason = (pszPrevReason != NULL) ? std::string(reinterpret_cast<const char *>(pszPrevReason)) : std::string();
		outRows.push_back(row);
	}

	sqlite3_finalize(pStmt);
	sqlite3_close(pDb);
	return true;
}

bool CAccountAdminStore::Get(unsigned int dwAccountID, AccountAdminRow &outRow)
{
	sqlite3 *pDb = NULL;
	if (!OpenDb(&pDb)) return false;

	EnsureTables(pDb);

	const char *pszSql =
		"SELECT AccountID, Status, Reason, TimeChanged, ActorAccountID, PrevDisable,"
		" PrevStatus, PrevReason"
		" FROM sys_account_admin WHERE AccountID=?;";
	sqlite3_stmt *pStmt = NULL;
	if (sqlite3_prepare_v2(pDb, pszSql, -1, &pStmt, NULL) != SQLITE_OK) {
		sqlite3_close(pDb);
		return false;
	}

	sqlite3_bind_int(pStmt, 1, (int)dwAccountID);

	bool bFound = false;
	if (sqlite3_step(pStmt) == SQLITE_ROW) {
		outRow.dwAccountID = (unsigned int)sqlite3_column_int(pStmt, 0);
		const unsigned char *pszStatus = sqlite3_column_text(pStmt, 1);
		outRow.strStatus = (pszStatus != NULL) ? std::string(reinterpret_cast<const char *>(pszStatus)) : std::string();
		const unsigned char *pszReason = sqlite3_column_text(pStmt, 2);
		outRow.strReason = (pszReason != NULL) ? std::string(reinterpret_cast<const char *>(pszReason)) : std::string();
		outRow.lTimeChanged = (long)sqlite3_column_int(pStmt, 3);
		outRow.dwActorAccountID = (unsigned int)sqlite3_column_int(pStmt, 4);
		outRow.nPrevDisable = sqlite3_column_int(pStmt, 5);
		const unsigned char *pszPrevStatus = sqlite3_column_text(pStmt, 6);
		outRow.strPrevStatus = (pszPrevStatus != NULL) ? std::string(reinterpret_cast<const char *>(pszPrevStatus)) : std::string();
		const unsigned char *pszPrevReason = sqlite3_column_text(pStmt, 7);
		outRow.strPrevReason = (pszPrevReason != NULL) ? std::string(reinterpret_cast<const char *>(pszPrevReason)) : std::string();
		bFound = true;
	}

	sqlite3_finalize(pStmt);
	sqlite3_close(pDb);
	return bFound;
}

bool CAccountAdminStore::SetStatus(unsigned int dwAccountID, const std::string &strStatus,
	const std::string &strReason, long lTimeChanged,
	unsigned int dwActorAccountID, int nPrevDisable,
	const std::string &strPrevStatus, const std::string &strPrevReason)
{
	sqlite3 *pDb = NULL;
	if (!OpenDb(&pDb)) return false;

	EnsureTables(pDb);

	int nBeginRet = sqlite3_exec(pDb, "BEGIN IMMEDIATE;", NULL, NULL, NULL);
	if (nBeginRet != SQLITE_OK) {
		SboPlatform::WriteDebugLine("CAccountAdminStore::SetStatus: BEGIN IMMEDIATE failed\n");
		sqlite3_close(pDb);
		return false;
	}

	// UPSERT に頼らず DELETE+INSERT で互換性を保つ(CAccountAuthStore::IssueCode と同じ考え方)
	bool bOk = true;
	{
		const char *pszDel = "DELETE FROM sys_account_admin WHERE AccountID=?;";
		sqlite3_stmt *pStmt = NULL;
		if (sqlite3_prepare_v2(pDb, pszDel, -1, &pStmt, NULL) == SQLITE_OK) {
			sqlite3_bind_int(pStmt, 1, (int)dwAccountID);
			bOk = (sqlite3_step(pStmt) == SQLITE_DONE);
			sqlite3_finalize(pStmt);
		} else {
			bOk = false;
		}
	}

	if (bOk) {
		const char *pszIns =
			"INSERT INTO sys_account_admin(AccountID, Status, Reason, TimeChanged, ActorAccountID, PrevDisable,"
			" PrevStatus, PrevReason)"
			" VALUES(?, ?, ?, ?, ?, ?, ?, ?);";
		sqlite3_stmt *pStmt = NULL;
		if (sqlite3_prepare_v2(pDb, pszIns, -1, &pStmt, NULL) == SQLITE_OK) {
			sqlite3_bind_int(pStmt, 1, (int)dwAccountID);
			sqlite3_bind_text(pStmt, 2, strStatus.c_str(), -1, SQLITE_TRANSIENT);
			if (strReason.empty()) {
				sqlite3_bind_null(pStmt, 3);
			} else {
				sqlite3_bind_text(pStmt, 3, strReason.c_str(), -1, SQLITE_TRANSIENT);
			}
			sqlite3_bind_int(pStmt, 4, (int)lTimeChanged);
			sqlite3_bind_int(pStmt, 5, (int)dwActorAccountID);
			sqlite3_bind_int(pStmt, 6, nPrevDisable);
			if (strPrevStatus.empty()) {
				sqlite3_bind_null(pStmt, 7);
			} else {
				sqlite3_bind_text(pStmt, 7, strPrevStatus.c_str(), -1, SQLITE_TRANSIENT);
			}
			if (strPrevReason.empty()) {
				sqlite3_bind_null(pStmt, 8);
			} else {
				sqlite3_bind_text(pStmt, 8, strPrevReason.c_str(), -1, SQLITE_TRANSIENT);
			}
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

bool CAccountAdminStore::DeleteRow(unsigned int dwAccountID)
{
	sqlite3 *pDb = NULL;
	if (!OpenDb(&pDb)) return false;

	EnsureTables(pDb);

	const char *pszSql = "DELETE FROM sys_account_admin WHERE AccountID=?;";
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
