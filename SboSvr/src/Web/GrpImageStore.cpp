#include "StdAfx.h"
#include "GrpImageStore.h"

// SQLite3 を include（このファイルのみ）
#include "../../third_party/sqlite/sqlite3.h"

#include <cstdio>
#include <ctime>

#include <windows.h>

namespace
{
// ETag 用に PNG バイト列を退避する必要はない（revision + size だけで十分にユニーク）。

std::string BuildGetETag(int nRevision, size_t nPngSize)
{
    char szBuf[64];
    std::snprintf(szBuf, sizeof(szBuf), "\"gs-%d-%llu\"",
                  nRevision, static_cast<unsigned long long>(nPngSize));
    return std::string(szBuf);
}
} // namespace

// static
CGrpImageStore &CGrpImageStore::GetInstance()
{
    static CGrpImageStore s_instance;
    return s_instance;
}

CGrpImageStore::CGrpImageStore()
    : m_pDb(NULL)
    , m_bOpenFailed(false)
{
}

CGrpImageStore::~CGrpImageStore()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_pDb != NULL) {
        sqlite3_close(m_pDb);
        m_pDb = NULL;
    }
}

bool CGrpImageStore::ResolveDbPathLocked(std::string &outPath) const
{
    char szDir[MAX_PATH];
    DWORD dwLength = GetModuleFileNameA(NULL, szDir, MAX_PATH);
    if ((dwLength == 0) || (dwLength >= MAX_PATH)) {
        return false;
    }
    char *pszTmp = strrchr(szDir, '\\');
    if (pszTmp != NULL) {
        pszTmp[1] = '\0';
    }

    std::string dirPath = szDir;
    dirPath.append("SBODATA");
    CreateDirectoryA(dirPath.c_str(), NULL);

    outPath = dirPath;
    outPath.append("\\SboGrpData.db");
    return true;
}

bool CGrpImageStore::EnsureOpenLocked()
{
    if (m_pDb != NULL) {
        return true;
    }
    if (m_bOpenFailed) {
        // 一度失敗したら毎回開き直さない
        return false;
    }

    std::string dbPath;
    if (!ResolveDbPathLocked(dbPath)) {
        m_bOpenFailed = true;
        return false;
    }

    sqlite3 *pDb = NULL;
    int nRet = sqlite3_open(dbPath.c_str(), &pDb);
    if (nRet != SQLITE_OK) {
        OutputDebugStringA("CGrpImageStore: sqlite3_open failed\n");
        if (pDb != NULL) {
            sqlite3_close(pDb);
        }
        m_bOpenFailed = true;
        return false;
    }

    // WAL にしない（-wal/-shm が残るとデプロイ時のコピー漏れ事故になる）。
    // 書き込みは稀なので安全側に倒して synchronous=FULL にする。
    const char *pszInit =
        "PRAGMA journal_mode=DELETE;"
        "PRAGMA synchronous=FULL;"
        "CREATE TABLE IF NOT EXISTS grp_meta("
        "  key   TEXT PRIMARY KEY NOT NULL,"
        "  value TEXT NOT NULL"
        ");"
        "CREATE TABLE IF NOT EXISTS grp_sheet("
        "  res_name   TEXT PRIMARY KEY NOT NULL,"
        "  png        BLOB NOT NULL,"
        "  width      INTEGER NOT NULL,"
        "  height     INTEGER NOT NULL,"
        "  revision   INTEGER NOT NULL DEFAULT 1,"
        "  updated_at INTEGER NOT NULL,"
        "  updated_by TEXT"
        ");"
        "CREATE TABLE IF NOT EXISTS grp_sheet_history("
        "  id       INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  res_name TEXT NOT NULL,"
        "  png      BLOB NOT NULL,"
        "  revision INTEGER NOT NULL,"
        "  saved_at INTEGER NOT NULL,"
        "  saved_by TEXT"
        ");"
        "CREATE INDEX IF NOT EXISTS idx_grp_sheet_history_res ON grp_sheet_history(res_name, revision DESC);"
        "INSERT OR IGNORE INTO grp_meta(key, value) VALUES('schema_version', '1');";

    char *pszErr = NULL;
    nRet = sqlite3_exec(pDb, pszInit, NULL, NULL, &pszErr);
    if (nRet != SQLITE_OK) {
        OutputDebugStringA("CGrpImageStore: schema init failed\n");
        if (pszErr != NULL) {
            sqlite3_free(pszErr);
        }
        sqlite3_close(pDb);
        m_bOpenFailed = true;
        return false;
    }

    m_pDb = pDb;
    return true;
}

bool CGrpImageStore::GetPng(const char *pszResName, std::vector<unsigned char> &outPng, std::string &outETag)
{
    if (pszResName == NULL || pszResName[0] == '\0') {
        return false;
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    if (!EnsureOpenLocked()) {
        return false;
    }

    const char *pszSql = "SELECT png, revision FROM grp_sheet WHERE res_name = ?;";
    sqlite3_stmt *pStmt = NULL;
    int nRet = sqlite3_prepare_v2(m_pDb, pszSql, -1, &pStmt, NULL);
    if (nRet != SQLITE_OK) {
        return false;
    }

    bool bFound = false;
    sqlite3_bind_text(pStmt, 1, pszResName, -1, SQLITE_STATIC);
    if (sqlite3_step(pStmt) == SQLITE_ROW) {
        const void *pBlob  = sqlite3_column_blob(pStmt, 0);
        int nBlobSize       = sqlite3_column_bytes(pStmt, 0);
        int nRevision       = sqlite3_column_int(pStmt, 1);
        if (pBlob != NULL && nBlobSize > 0) {
            outPng.assign(
                static_cast<const unsigned char *>(pBlob),
                static_cast<const unsigned char *>(pBlob) + nBlobSize);
            outETag = BuildGetETag(nRevision, outPng.size());
            bFound = true;
        }
    }
    sqlite3_finalize(pStmt);
    return bFound;
}

bool CGrpImageStore::PutPng(const char *pszResName,
                             const unsigned char *pPng, size_t nPngSize,
                             int nWidth, int nHeight,
                             const char *pszUpdatedBy,
                             std::string &outError)
{
    if (pszResName == NULL || pszResName[0] == '\0' || pPng == NULL || nPngSize == 0) {
        outError = "invalid_argument";
        return false;
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    if (!EnsureOpenLocked()) {
        outError = "db_unavailable";
        return false;
    }

    long long llNow = static_cast<long long>(std::time(NULL));

    if (sqlite3_exec(m_pDb, "BEGIN IMMEDIATE TRANSACTION;", NULL, NULL, NULL) != SQLITE_OK) {
        outError = "begin_transaction_failed";
        return false;
    }

    // 既存 revision を確認
    int nExistingRevision = 0;
    {
        const char *pszSelSql = "SELECT revision FROM grp_sheet WHERE res_name = ?;";
        sqlite3_stmt *pSelStmt = NULL;
        if (sqlite3_prepare_v2(m_pDb, pszSelSql, -1, &pSelStmt, NULL) == SQLITE_OK) {
            sqlite3_bind_text(pSelStmt, 1, pszResName, -1, SQLITE_STATIC);
            if (sqlite3_step(pSelStmt) == SQLITE_ROW) {
                nExistingRevision = sqlite3_column_int(pSelStmt, 0);
            }
            sqlite3_finalize(pSelStmt);
        }
    }

    // 既存行があれば履歴へ退避
    if (nExistingRevision > 0) {
        const char *pszHistSql =
            "INSERT INTO grp_sheet_history(res_name, png, revision, saved_at, saved_by) "
            "SELECT res_name, png, revision, ?, ? FROM grp_sheet WHERE res_name = ?;";
        sqlite3_stmt *pHistStmt = NULL;
        if (sqlite3_prepare_v2(m_pDb, pszHistSql, -1, &pHistStmt, NULL) != SQLITE_OK) {
            sqlite3_exec(m_pDb, "ROLLBACK;", NULL, NULL, NULL);
            outError = "history_prepare_failed";
            return false;
        }
        sqlite3_bind_int64(pHistStmt, 1, llNow);
        if (pszUpdatedBy != NULL) {
            sqlite3_bind_text(pHistStmt, 2, pszUpdatedBy, -1, SQLITE_STATIC);
        } else {
            sqlite3_bind_null(pHistStmt, 2);
        }
        sqlite3_bind_text(pHistStmt, 3, pszResName, -1, SQLITE_STATIC);
        int nHistStep = sqlite3_step(pHistStmt);
        sqlite3_finalize(pHistStmt);
        if (nHistStep != SQLITE_DONE) {
            sqlite3_exec(m_pDb, "ROLLBACK;", NULL, NULL, NULL);
            outError = "history_insert_failed";
            return false;
        }
    }

    int nNewRevision = nExistingRevision + 1;

    const char *pszUpsertSql =
        "INSERT INTO grp_sheet(res_name, png, width, height, revision, updated_at, updated_by) "
        "VALUES(?, ?, ?, ?, ?, ?, ?) "
        "ON CONFLICT(res_name) DO UPDATE SET "
        "  png = excluded.png, width = excluded.width, height = excluded.height, "
        "  revision = excluded.revision, updated_at = excluded.updated_at, updated_by = excluded.updated_by;";
    sqlite3_stmt *pUpsertStmt = NULL;
    if (sqlite3_prepare_v2(m_pDb, pszUpsertSql, -1, &pUpsertStmt, NULL) != SQLITE_OK) {
        sqlite3_exec(m_pDb, "ROLLBACK;", NULL, NULL, NULL);
        outError = "upsert_prepare_failed";
        return false;
    }
    sqlite3_bind_text(pUpsertStmt, 1, pszResName, -1, SQLITE_STATIC);
    sqlite3_bind_blob(pUpsertStmt, 2, pPng, static_cast<int>(nPngSize), SQLITE_STATIC);
    sqlite3_bind_int(pUpsertStmt, 3, nWidth);
    sqlite3_bind_int(pUpsertStmt, 4, nHeight);
    sqlite3_bind_int(pUpsertStmt, 5, nNewRevision);
    sqlite3_bind_int64(pUpsertStmt, 6, llNow);
    if (pszUpdatedBy != NULL) {
        sqlite3_bind_text(pUpsertStmt, 7, pszUpdatedBy, -1, SQLITE_STATIC);
    } else {
        sqlite3_bind_null(pUpsertStmt, 7);
    }
    int nUpsertStep = sqlite3_step(pUpsertStmt);
    sqlite3_finalize(pUpsertStmt);
    if (nUpsertStep != SQLITE_DONE) {
        sqlite3_exec(m_pDb, "ROLLBACK;", NULL, NULL, NULL);
        outError = "upsert_failed";
        return false;
    }

    if (sqlite3_exec(m_pDb, "COMMIT;", NULL, NULL, NULL) != SQLITE_OK) {
        sqlite3_exec(m_pDb, "ROLLBACK;", NULL, NULL, NULL);
        outError = "commit_failed";
        return false;
    }

    return true;
}
