/// @file SaveLoadInfoMapGenPattern.cpp
/// @brief 自動生成マップパターン情報の保存・読み込みクラス 実装ファイル
///        S1 フェーズ: パラメータを JSON 文字列のまま保存するのみ。生成ロジックは持たない。
///
/// @author 年がら年中春うらら(URARA-works)
/// @date 2026/06/19

#include "StdAfx.h"
#include "../../third_party/sqlite/sqlite3.h"
#include "SaveLoadInfoMapGenPattern.h"

// テーブル名
static const char* s_pszTableName = "sys_map_gen_pattern";

// ============================================================
// コンストラクタ / デストラクタ
// ============================================================

CSaveLoadInfoMapGenPattern::CSaveLoadInfoMapGenPattern()
{
}

CSaveLoadInfoMapGenPattern::~CSaveLoadInfoMapGenPattern()
{
}

// ============================================================
// 内部ヘルパー: DB を開く（MgrData::OpenSboDb と同等ロジック）
// ============================================================
bool CSaveLoadInfoMapGenPattern::OpenDb(sqlite3 **ppDb)
{
    if (ppDb == NULL) return false;

    char szDbPath[MAX_PATH];
    char szDir[MAX_PATH];
    LPSTR pszTmp;

    // 実行ファイルのディレクトリを取得
    GetModuleFileNameA(NULL, szDir, MAX_PATH);
    pszTmp = strrchr(szDir, '\\');
    if (pszTmp != NULL) {
        pszTmp[1] = '\0';
    }

    // SBODATA ディレクトリが無ければ作成
    strcpy_s(szDbPath, szDir);
    strcat_s(szDbPath, "SBODATA");
    CreateDirectoryA(szDbPath, NULL);

    // DB ファイルパス
    strcat_s(szDbPath, "\\SboData.db");

    int nRet = sqlite3_open(szDbPath, ppDb);
    if (nRet != SQLITE_OK) {
        OutputDebugStringA("CSaveLoadInfoMapGenPattern::OpenDb: sqlite3_open failed\n");
        *ppDb = NULL;
        return false;
    }

    // WAL モードを有効化
    sqlite3_exec(*ppDb, "PRAGMA journal_mode=WAL; PRAGMA synchronous=NORMAL;", NULL, NULL, NULL);
    return true;
}

// ============================================================
// 内部ヘルパー: テーブルが無ければ CREATE TABLE IF NOT EXISTS
// ============================================================
void CSaveLoadInfoMapGenPattern::EnsureTable(sqlite3 *pDb)
{
    if (pDb == NULL) return;

    const char* pszSql =
        "CREATE TABLE IF NOT EXISTS sys_map_gen_pattern("
        "  PatternID    INTEGER PRIMARY KEY,"   // 自動採番
        "  PatternName  TEXT,"                  // パターン名
        "  AlgoType     INTEGER,"               // アルゴリズム種別 (0=洞窟L3)
        "  ParamsJson   TEXT,"                  // アルゴリズム別パラメータ (JSON 文字列)
        "  RoleMapJson  TEXT"                   // ロール→マップパーツID マッピング (JSON 文字列)
        ");";

    sqlite3_exec(pDb, pszSql, NULL, NULL, NULL);
}

// ============================================================
// 全件取得
// ============================================================
int CSaveLoadInfoMapGenPattern::LoadAll(std::vector<MapGenPatternRecord> &outRecords)
{
    outRecords.clear();

    sqlite3 *pDb = NULL;
    if (!OpenDb(&pDb)) return -1;

    EnsureTable(pDb);

    const char* pszSql =
        "SELECT PatternID, PatternName, AlgoType, ParamsJson, RoleMapJson"
        " FROM sys_map_gen_pattern"
        " ORDER BY PatternID;";

    sqlite3_stmt* pStmt = NULL;
    int nRet = sqlite3_prepare_v2(pDb, pszSql, -1, &pStmt, NULL);
    if (nRet != SQLITE_OK) {
        sqlite3_close(pDb);
        return -1;
    }

    int nCount = 0;
    while (sqlite3_step(pStmt) == SQLITE_ROW) {
        MapGenPatternRecord rec;
        rec.nPatternID = sqlite3_column_int(pStmt, 0);

        const char* pszName = (const char*)sqlite3_column_text(pStmt, 1);
        rec.strPatternName = (pszName != NULL) ? pszName : "";

        rec.nAlgoType = sqlite3_column_int(pStmt, 2);

        const char* pszParams = (const char*)sqlite3_column_text(pStmt, 3);
        rec.strParamsJson = (pszParams != NULL) ? pszParams : "";

        const char* pszRoleMap = (const char*)sqlite3_column_text(pStmt, 4);
        rec.strRoleMapJson = (pszRoleMap != NULL) ? pszRoleMap : "";

        outRecords.push_back(rec);
        nCount++;
    }

    sqlite3_finalize(pStmt);
    sqlite3_close(pDb);
    return nCount;
}

// ============================================================
// 新規挿入（PatternID は AUTOINCREMENT 採番、outPatternID に返す）
// ============================================================
bool CSaveLoadInfoMapGenPattern::Insert(const MapGenPatternRecord &rec, int &outPatternID)
{
    outPatternID = 0;

    sqlite3 *pDb = NULL;
    if (!OpenDb(&pDb)) return false;

    EnsureTable(pDb);

    const char* pszSql =
        "INSERT INTO sys_map_gen_pattern(PatternName, AlgoType, ParamsJson, RoleMapJson)"
        " VALUES(?, ?, ?, ?);";

    sqlite3_stmt* pStmt = NULL;
    int nRet = sqlite3_prepare_v2(pDb, pszSql, -1, &pStmt, NULL);
    if (nRet != SQLITE_OK) {
        sqlite3_close(pDb);
        return false;
    }

    sqlite3_bind_text(pStmt, 1, rec.strPatternName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int (pStmt, 2, rec.nAlgoType);
    sqlite3_bind_text(pStmt, 3, rec.strParamsJson.c_str(),  -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(pStmt, 4, rec.strRoleMapJson.c_str(), -1, SQLITE_TRANSIENT);

    bool bOk = (sqlite3_step(pStmt) == SQLITE_DONE);
    if (bOk) {
        outPatternID = (int)sqlite3_last_insert_rowid(pDb);
    }

    sqlite3_finalize(pStmt);
    sqlite3_close(pDb);
    return bOk;
}

// ============================================================
// 更新（rec.nPatternID で対象を特定）
// ============================================================
bool CSaveLoadInfoMapGenPattern::Update(const MapGenPatternRecord &rec)
{
    sqlite3 *pDb = NULL;
    if (!OpenDb(&pDb)) return false;

    EnsureTable(pDb);

    const char* pszSql =
        "UPDATE sys_map_gen_pattern"
        " SET PatternName=?, AlgoType=?, ParamsJson=?, RoleMapJson=?"
        " WHERE PatternID=?;";

    sqlite3_stmt* pStmt = NULL;
    int nRet = sqlite3_prepare_v2(pDb, pszSql, -1, &pStmt, NULL);
    if (nRet != SQLITE_OK) {
        sqlite3_close(pDb);
        return false;
    }

    sqlite3_bind_text(pStmt, 1, rec.strPatternName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int (pStmt, 2, rec.nAlgoType);
    sqlite3_bind_text(pStmt, 3, rec.strParamsJson.c_str(),  -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(pStmt, 4, rec.strRoleMapJson.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int (pStmt, 5, rec.nPatternID);

    bool bOk = (sqlite3_step(pStmt) == SQLITE_DONE);

    // 対象行が無かった場合も false を返す
    if (bOk && sqlite3_changes(pDb) == 0) {
        bOk = false;
    }

    sqlite3_finalize(pStmt);
    sqlite3_close(pDb);
    return bOk;
}

// ============================================================
// 削除
// ============================================================
bool CSaveLoadInfoMapGenPattern::Delete(int nPatternID)
{
    sqlite3 *pDb = NULL;
    if (!OpenDb(&pDb)) return false;

    EnsureTable(pDb);

    const char* pszSql =
        "DELETE FROM sys_map_gen_pattern WHERE PatternID=?;";

    sqlite3_stmt* pStmt = NULL;
    int nRet = sqlite3_prepare_v2(pDb, pszSql, -1, &pStmt, NULL);
    if (nRet != SQLITE_OK) {
        sqlite3_close(pDb);
        return false;
    }

    sqlite3_bind_int(pStmt, 1, nPatternID);

    bool bOk = (sqlite3_step(pStmt) == SQLITE_DONE);

    if (bOk && sqlite3_changes(pDb) == 0) {
        bOk = false;
    }

    sqlite3_finalize(pStmt);
    sqlite3_close(pDb);
    return bOk;
}
