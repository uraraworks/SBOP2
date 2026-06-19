/// @file SaveLoadInfoMapGenPattern.h
/// @brief 自動生成マップパターン情報の保存・読み込みクラス 定義ファイル
///        sys_map_gen_pattern テーブルへの CRUD を提供する。
///        S1 フェーズ: 生成ロジックは持たず、パラメータを JSON 文字列のまま保存するのみ。
///
///  テーブル構成:
///    sys_map_gen_pattern:
///      PatternID    INTEGER PRIMARY KEY
///      PatternName  TEXT
///      AlgoType     INTEGER  (0=洞窟L3、当面これのみ)
///      ParamsJson   TEXT     (アルゴリズム別パラメータ JSON、S1では中身を解釈しない)
///      RoleMapJson  TEXT     (ロール→マップパーツID JSON、S1では中身を解釈しない)
///
/// @author 年がら年中春うらら(URARA-works)
/// @date 2026/06/19

#pragma once

#include "../../third_party/sqlite/sqlite3.h"
#include <string>
#include <vector>

/// マップ生成パターン 1 件分のデータ構造
struct MapGenPatternRecord
{
    int         nPatternID;
    std::string strPatternName;
    int         nAlgoType;
    std::string strParamsJson;
    std::string strRoleMapJson;
};

/// sys_map_gen_pattern テーブルへの CRUD ヘルパークラス。
/// LibInfoBase は使わず、都度 DB を開いて直接操作する独立型実装。
class CSaveLoadInfoMapGenPattern
{
public:
    CSaveLoadInfoMapGenPattern();
    ~CSaveLoadInfoMapGenPattern();

    /// 全件取得。戻り値: 取得件数（DB エラー時は -1）
    int     LoadAll(std::vector<MapGenPatternRecord> &outRecords);

    /// 新規挿入。outPatternID に採番された PatternID が入る。戻り値: 成功時 true
    bool    Insert(const MapGenPatternRecord &rec, int &outPatternID);

    /// 更新（rec.nPatternID で対象を特定）。戻り値: 成功時 true
    bool    Update(const MapGenPatternRecord &rec);

    /// 削除。戻り値: 成功時 true
    bool    Delete(int nPatternID);

private:
    /// DB を開いて pDb に格納する（MgrData::OpenSboDb と同等ロジック）
    bool    OpenDb(sqlite3 **ppDb);

    /// テーブルが無ければ CREATE TABLE IF NOT EXISTS を実行する
    void    EnsureTable(sqlite3 *pDb);
};
