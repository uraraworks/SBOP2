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
        // 途中セーブ（下書き）。grp_sheet とは別テーブルにして、
        // ここに入っている間はゲームにも公開マニフェストにも一切出さない。
        "CREATE TABLE IF NOT EXISTS grp_draft("
        "  id          INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  name        TEXT NOT NULL,"
        "  cat_key     TEXT NOT NULL,"
        "  sheet_index INTEGER NOT NULL,"
        "  col         INTEGER NOT NULL,"
        "  row         INTEGER NOT NULL,"
        "  png         BLOB NOT NULL,"
        "  width       INTEGER NOT NULL,"
        "  height      INTEGER NOT NULL,"
        "  updated_at  INTEGER NOT NULL,"
        "  updated_by  TEXT"
        ");"
        "CREATE INDEX IF NOT EXISTS idx_grp_draft_updated ON grp_draft(updated_at DESC);"
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

bool CGrpImageStore::GetCurrentMeta(const char *pszResName, int &outRevision, int &outWidth, int &outHeight,
                                     long long &outUpdatedAt, std::string &outUpdatedBy, size_t &outBytes)
{
    if (pszResName == NULL || pszResName[0] == '\0') {
        return false;
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    if (!EnsureOpenLocked()) {
        return false;
    }

    const char *pszSql =
        "SELECT revision, width, height, updated_at, updated_by, length(png) "
        "FROM grp_sheet WHERE res_name = ?;";
    sqlite3_stmt *pStmt = NULL;
    if (sqlite3_prepare_v2(m_pDb, pszSql, -1, &pStmt, NULL) != SQLITE_OK) {
        return false;
    }

    bool bFound = false;
    sqlite3_bind_text(pStmt, 1, pszResName, -1, SQLITE_STATIC);
    if (sqlite3_step(pStmt) == SQLITE_ROW) {
        outRevision = sqlite3_column_int(pStmt, 0);
        outWidth    = sqlite3_column_int(pStmt, 1);
        outHeight   = sqlite3_column_int(pStmt, 2);
        outUpdatedAt = sqlite3_column_int64(pStmt, 3);
        const unsigned char *pUpdatedBy = sqlite3_column_text(pStmt, 4);
        outUpdatedBy = (pUpdatedBy != NULL) ? reinterpret_cast<const char *>(pUpdatedBy) : std::string();
        outBytes = static_cast<size_t>(sqlite3_column_int64(pStmt, 5));
        bFound = true;
    }
    sqlite3_finalize(pStmt);
    return bFound;
}

bool CGrpImageStore::GetHistory(const char *pszResName, std::vector<SGrpSheetHistoryEntry> &outEntries)
{
    outEntries.clear();
    if (pszResName == NULL || pszResName[0] == '\0') {
        return false;
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    if (!EnsureOpenLocked()) {
        // DB が無ければ「上書き履歴なし」として空を返し、エラー扱いにはしない。
        return true;
    }

    // png 本体は一覧表示に不要なうえ重いので length(png) だけ取る。
    const char *pszSql =
        "SELECT id, revision, saved_at, saved_by, length(png) "
        "FROM grp_sheet_history WHERE res_name = ? ORDER BY revision DESC, id DESC;";
    sqlite3_stmt *pStmt = NULL;
    if (sqlite3_prepare_v2(m_pDb, pszSql, -1, &pStmt, NULL) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(pStmt, 1, pszResName, -1, SQLITE_STATIC);
    while (sqlite3_step(pStmt) == SQLITE_ROW) {
        SGrpSheetHistoryEntry entry;
        entry.nId       = sqlite3_column_int64(pStmt, 0);
        entry.nRevision = sqlite3_column_int(pStmt, 1);
        entry.nSavedAt  = sqlite3_column_int64(pStmt, 2);
        const unsigned char *pSavedBy = sqlite3_column_text(pStmt, 3);
        entry.strSavedBy = (pSavedBy != NULL) ? reinterpret_cast<const char *>(pSavedBy) : std::string();
        entry.nBytes    = static_cast<size_t>(sqlite3_column_int64(pStmt, 4));
        outEntries.push_back(entry);
    }
    sqlite3_finalize(pStmt);
    return true;
}

bool CGrpImageStore::GetAllSummaries(std::vector<SGrpSheetSummary> &outList)
{
    outList.clear();

    std::lock_guard<std::mutex> lock(m_mutex);
    if (!EnsureOpenLocked()) {
        // DB が無ければ「配信対象なし」として空を返し、エラー扱いにはしない。
        return true;
    }

    // png 本体は不要なうえ重いので length(png) だけ取る。
    const char *pszSql =
        "SELECT res_name, revision, length(png), updated_at "
        "FROM grp_sheet ORDER BY res_name ASC;";
    sqlite3_stmt *pStmt = NULL;
    if (sqlite3_prepare_v2(m_pDb, pszSql, -1, &pStmt, NULL) != SQLITE_OK) {
        return false;
    }

    while (sqlite3_step(pStmt) == SQLITE_ROW) {
        SGrpSheetSummary summary;
        const unsigned char *pResName = sqlite3_column_text(pStmt, 0);
        summary.strResName = (pResName != NULL) ? reinterpret_cast<const char *>(pResName) : std::string();
        summary.nRevision  = sqlite3_column_int(pStmt, 1);
        summary.nBytes     = static_cast<size_t>(sqlite3_column_int64(pStmt, 2));
        summary.nUpdatedAt = sqlite3_column_int64(pStmt, 3);
        outList.push_back(summary);
    }
    sqlite3_finalize(pStmt);
    return true;
}

bool CGrpImageStore::GetHistoryPng(const char *pszResName, int nRevision, std::vector<unsigned char> &outPng)
{
    if (pszResName == NULL || pszResName[0] == '\0') {
        return false;
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    if (!EnsureOpenLocked()) {
        return false;
    }

    const char *pszSql =
        "SELECT png FROM grp_sheet_history WHERE res_name = ? AND revision = ? "
        "ORDER BY id DESC LIMIT 1;";
    sqlite3_stmt *pStmt = NULL;
    if (sqlite3_prepare_v2(m_pDb, pszSql, -1, &pStmt, NULL) != SQLITE_OK) {
        return false;
    }

    bool bFound = false;
    sqlite3_bind_text(pStmt, 1, pszResName, -1, SQLITE_STATIC);
    sqlite3_bind_int(pStmt, 2, nRevision);
    if (sqlite3_step(pStmt) == SQLITE_ROW) {
        const void *pBlob = sqlite3_column_blob(pStmt, 0);
        int nBlobSize = sqlite3_column_bytes(pStmt, 0);
        if (pBlob != NULL && nBlobSize > 0) {
            outPng.assign(
                static_cast<const unsigned char *>(pBlob),
                static_cast<const unsigned char *>(pBlob) + nBlobSize);
            bFound = true;
        }
    }
    sqlite3_finalize(pStmt);
    return bFound;
}

bool CGrpImageStore::ClearOverride(const char *pszResName, std::string &outError)
{
    outError.clear();
    if (pszResName == NULL || pszResName[0] == '\0') {
        outError = "invalid_argument";
        return false;
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    if (!EnsureOpenLocked()) {
        // DB が無い = そもそも上書きが存在しない状態と同義なので成功扱い（冪等）。
        return true;
    }

    if (sqlite3_exec(m_pDb, "BEGIN IMMEDIATE TRANSACTION;", NULL, NULL, NULL) != SQLITE_OK) {
        outError = "begin_transaction_failed";
        return false;
    }

    const char *pszDeleteHistorySql = "DELETE FROM grp_sheet_history WHERE res_name = ?;";
    sqlite3_stmt *pDelHistStmt = NULL;
    if (sqlite3_prepare_v2(m_pDb, pszDeleteHistorySql, -1, &pDelHistStmt, NULL) != SQLITE_OK) {
        sqlite3_exec(m_pDb, "ROLLBACK;", NULL, NULL, NULL);
        outError = "delete_history_prepare_failed";
        return false;
    }
    sqlite3_bind_text(pDelHistStmt, 1, pszResName, -1, SQLITE_STATIC);
    int nDelHistStep = sqlite3_step(pDelHistStmt);
    sqlite3_finalize(pDelHistStmt);
    if (nDelHistStep != SQLITE_DONE) {
        sqlite3_exec(m_pDb, "ROLLBACK;", NULL, NULL, NULL);
        outError = "delete_history_failed";
        return false;
    }

    const char *pszDeleteCurrentSql = "DELETE FROM grp_sheet WHERE res_name = ?;";
    sqlite3_stmt *pDelCurStmt = NULL;
    if (sqlite3_prepare_v2(m_pDb, pszDeleteCurrentSql, -1, &pDelCurStmt, NULL) != SQLITE_OK) {
        sqlite3_exec(m_pDb, "ROLLBACK;", NULL, NULL, NULL);
        outError = "delete_current_prepare_failed";
        return false;
    }
    sqlite3_bind_text(pDelCurStmt, 1, pszResName, -1, SQLITE_STATIC);
    int nDelCurStep = sqlite3_step(pDelCurStmt);
    sqlite3_finalize(pDelCurStmt);
    if (nDelCurStep != SQLITE_DONE) {
        sqlite3_exec(m_pDb, "ROLLBACK;", NULL, NULL, NULL);
        outError = "delete_current_failed";
        return false;
    }

    if (sqlite3_exec(m_pDb, "COMMIT;", NULL, NULL, NULL) != SQLITE_OK) {
        sqlite3_exec(m_pDb, "ROLLBACK;", NULL, NULL, NULL);
        outError = "commit_failed";
        return false;
    }

    return true;
}

// ---------------------------------------------------------------------------
// 途中セーブ（下書き）
//
// grp_sheet とは完全に別テーブル。ここに入っているデータは配信経路
// （GetPng / GetAllSummaries）から一切参照されないので、下書きを保存しても
// ゲームには反映されない。
// ---------------------------------------------------------------------------

namespace
{
// sqlite3_column_text は NULL を返しうるので、空文字に丸めて受け取る
std::string ColumnTextOrEmpty(sqlite3_stmt *pStmt, int nColumn)
{
    const unsigned char *pszText = sqlite3_column_text(pStmt, nColumn);
    if (pszText == NULL) {
        return std::string();
    }
    return std::string(reinterpret_cast<const char *>(pszText));
}
} // namespace

bool CGrpImageStore::ListDrafts(std::vector<SGrpDraftEntry> &outList)
{
    outList.clear();

    std::lock_guard<std::mutex> lock(m_mutex);
    if (!EnsureOpenLocked()) {
        // DB 不在は「下書きが無い」と同じ扱い（プロジェクトのデータ不在許容方針）
        return true;
    }

    const char *pszSql =
        "SELECT id, name, cat_key, sheet_index, col, row, width, height, "
        "       updated_at, updated_by, length(png) "
        "FROM grp_draft ORDER BY updated_at DESC, id DESC;";
    sqlite3_stmt *pStmt = NULL;
    if (sqlite3_prepare_v2(m_pDb, pszSql, -1, &pStmt, NULL) != SQLITE_OK) {
        return false;
    }

    while (sqlite3_step(pStmt) == SQLITE_ROW) {
        SGrpDraftEntry entry;
        entry.nId          = sqlite3_column_int64(pStmt, 0);
        entry.strName      = ColumnTextOrEmpty(pStmt, 1);
        entry.strCatKey    = ColumnTextOrEmpty(pStmt, 2);
        entry.nSheetIndex  = sqlite3_column_int(pStmt, 3);
        entry.nCol         = sqlite3_column_int(pStmt, 4);
        entry.nRow         = sqlite3_column_int(pStmt, 5);
        entry.nWidth       = sqlite3_column_int(pStmt, 6);
        entry.nHeight      = sqlite3_column_int(pStmt, 7);
        entry.nUpdatedAt   = sqlite3_column_int64(pStmt, 8);
        entry.strUpdatedBy = ColumnTextOrEmpty(pStmt, 9);
        entry.nBytes       = static_cast<size_t>(sqlite3_column_int(pStmt, 10));
        outList.push_back(entry);
    }
    sqlite3_finalize(pStmt);
    return true;
}

bool CGrpImageStore::GetDraft(long long nId, SGrpDraftEntry &outMeta,
                              std::vector<unsigned char> &outPng)
{
    outPng.clear();

    std::lock_guard<std::mutex> lock(m_mutex);
    if (!EnsureOpenLocked()) {
        return false;
    }

    const char *pszSql =
        "SELECT id, name, cat_key, sheet_index, col, row, width, height, "
        "       updated_at, updated_by, png "
        "FROM grp_draft WHERE id = ?;";
    sqlite3_stmt *pStmt = NULL;
    if (sqlite3_prepare_v2(m_pDb, pszSql, -1, &pStmt, NULL) != SQLITE_OK) {
        return false;
    }
    sqlite3_bind_int64(pStmt, 1, nId);

    bool bFound = false;
    if (sqlite3_step(pStmt) == SQLITE_ROW) {
        outMeta.nId          = sqlite3_column_int64(pStmt, 0);
        outMeta.strName      = ColumnTextOrEmpty(pStmt, 1);
        outMeta.strCatKey    = ColumnTextOrEmpty(pStmt, 2);
        outMeta.nSheetIndex  = sqlite3_column_int(pStmt, 3);
        outMeta.nCol         = sqlite3_column_int(pStmt, 4);
        outMeta.nRow         = sqlite3_column_int(pStmt, 5);
        outMeta.nWidth       = sqlite3_column_int(pStmt, 6);
        outMeta.nHeight      = sqlite3_column_int(pStmt, 7);
        outMeta.nUpdatedAt   = sqlite3_column_int64(pStmt, 8);
        outMeta.strUpdatedBy = ColumnTextOrEmpty(pStmt, 9);

        const void *pBlob = sqlite3_column_blob(pStmt, 10);
        int nBlobSize = sqlite3_column_bytes(pStmt, 10);
        if (pBlob != NULL && nBlobSize > 0) {
            const unsigned char *pBytes = static_cast<const unsigned char *>(pBlob);
            outPng.assign(pBytes, pBytes + nBlobSize);
        }
        outMeta.nBytes = outPng.size();
        bFound = true;
    }
    sqlite3_finalize(pStmt);
    return bFound;
}

bool CGrpImageStore::CreateDraft(const SGrpDraftEntry &meta,
                                 const unsigned char *pPng, size_t nPngSize,
                                 long long &outId, std::string &outError)
{
    if (pPng == NULL || nPngSize == 0 || meta.strCatKey.empty()) {
        outError = "invalid_argument";
        return false;
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    if (!EnsureOpenLocked()) {
        outError = "db_unavailable";
        return false;
    }

    const char *pszSql =
        "INSERT INTO grp_draft(name, cat_key, sheet_index, col, row, png, width, height, updated_at, updated_by) "
        "VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt *pStmt = NULL;
    if (sqlite3_prepare_v2(m_pDb, pszSql, -1, &pStmt, NULL) != SQLITE_OK) {
        outError = "prepare_failed";
        return false;
    }

    sqlite3_bind_text(pStmt, 1, meta.strName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(pStmt, 2, meta.strCatKey.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(pStmt, 3, meta.nSheetIndex);
    sqlite3_bind_int(pStmt, 4, meta.nCol);
    sqlite3_bind_int(pStmt, 5, meta.nRow);
    sqlite3_bind_blob(pStmt, 6, pPng, static_cast<int>(nPngSize), SQLITE_STATIC);
    sqlite3_bind_int(pStmt, 7, meta.nWidth);
    sqlite3_bind_int(pStmt, 8, meta.nHeight);
    sqlite3_bind_int64(pStmt, 9, static_cast<long long>(std::time(NULL)));
    sqlite3_bind_text(pStmt, 10, meta.strUpdatedBy.c_str(), -1, SQLITE_TRANSIENT);

    int nStep = sqlite3_step(pStmt);
    sqlite3_finalize(pStmt);
    if (nStep != SQLITE_DONE) {
        outError = "insert_failed";
        return false;
    }

    outId = sqlite3_last_insert_rowid(m_pDb);
    return true;
}

bool CGrpImageStore::UpdateDraft(long long nId, const SGrpDraftEntry &meta,
                                 const unsigned char *pPng, size_t nPngSize,
                                 std::string &outError)
{
    if (pPng == NULL || nPngSize == 0) {
        outError = "invalid_argument";
        return false;
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    if (!EnsureOpenLocked()) {
        outError = "db_unavailable";
        return false;
    }

    const char *pszSql =
        "UPDATE grp_draft SET name = ?, cat_key = ?, sheet_index = ?, col = ?, row = ?, "
        "  png = ?, width = ?, height = ?, updated_at = ?, updated_by = ? "
        "WHERE id = ?;";
    sqlite3_stmt *pStmt = NULL;
    if (sqlite3_prepare_v2(m_pDb, pszSql, -1, &pStmt, NULL) != SQLITE_OK) {
        outError = "prepare_failed";
        return false;
    }

    sqlite3_bind_text(pStmt, 1, meta.strName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(pStmt, 2, meta.strCatKey.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(pStmt, 3, meta.nSheetIndex);
    sqlite3_bind_int(pStmt, 4, meta.nCol);
    sqlite3_bind_int(pStmt, 5, meta.nRow);
    sqlite3_bind_blob(pStmt, 6, pPng, static_cast<int>(nPngSize), SQLITE_STATIC);
    sqlite3_bind_int(pStmt, 7, meta.nWidth);
    sqlite3_bind_int(pStmt, 8, meta.nHeight);
    sqlite3_bind_int64(pStmt, 9, static_cast<long long>(std::time(NULL)));
    sqlite3_bind_text(pStmt, 10, meta.strUpdatedBy.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(pStmt, 11, nId);

    int nStep = sqlite3_step(pStmt);
    sqlite3_finalize(pStmt);
    if (nStep != SQLITE_DONE) {
        outError = "update_failed";
        return false;
    }
    if (sqlite3_changes(m_pDb) == 0) {
        outError = "draft_not_found";
        return false;
    }
    return true;
}

bool CGrpImageStore::DeleteDraft(long long nId, std::string &outError)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!EnsureOpenLocked()) {
        outError = "db_unavailable";
        return false;
    }

    const char *pszSql = "DELETE FROM grp_draft WHERE id = ?;";
    sqlite3_stmt *pStmt = NULL;
    if (sqlite3_prepare_v2(m_pDb, pszSql, -1, &pStmt, NULL) != SQLITE_OK) {
        outError = "prepare_failed";
        return false;
    }
    sqlite3_bind_int64(pStmt, 1, nId);
    int nStep = sqlite3_step(pStmt);
    sqlite3_finalize(pStmt);
    if (nStep != SQLITE_DONE) {
        outError = "delete_failed";
        return false;
    }
    return true;
}
