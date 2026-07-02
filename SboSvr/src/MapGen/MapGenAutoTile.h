#pragma once

#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// MapGenAutoTile — マップ自動生成のオートタイル（縁取り）後処理
//
// 生成グリッド（セル値配列）を、ロールごとのチップ設定に基づいて
// tilesBase 相当の grpId 配列へ変換する。
//
// ロールごとのチップ設定は2種類:
//   - single モード: 従来どおり単一の grpId をそのまま敷く
//   - edge13 モード : 周囲8マスとの同ロール/異ロール判定で
//                     13種類（中央/辺4/外角4/内角4）のチップを使い分ける
//
// RoleMapJson v2 のパース（後方互換）もここで提供する。
//   - 値が数値                → single モード（従来形式）
//   - 値がオブジェクト        → { "mode": "edge13", "tiles": { ... } }
// ---------------------------------------------------------------------------

namespace MapGenAutoTile
{

// -----------------------------------------------------------------------
// edge13 モードで使う13種のチップ grpId
// 0 は「未設定」を意味し、未設定時は c（中央）へフォールバックする。
// -----------------------------------------------------------------------
struct EdgeTiles
{
    int c;    // 中央（周囲すべて同ロール、または内角判定不能）
    int n;    // 北側だけ異ロール（辺）
    int s;    // 南側だけ異ロール（辺）
    int w;    // 西側だけ異ロール（辺）
    int e;    // 東側だけ異ロール（辺）
    int nw;   // 北と西が異ロール（外角）
    int ne;   // 北と東が異ロール（外角）
    int sw;   // 南と西が異ロール（外角）
    int se;   // 南と東が異ロール（外角）
    int inNW; // 北西の斜めだけ異ロール（内角）
    int inNE; // 北東の斜めだけ異ロール（内角）
    int inSW; // 南西の斜めだけ異ロール（内角）
    int inSE; // 南東の斜めだけ異ロール（内角）

    EdgeTiles()
        : c(0), n(0), s(0), w(0), e(0)
        , nw(0), ne(0), sw(0), se(0)
        , inNW(0), inNE(0), inSW(0), inSE(0)
    {
    }
};

// ロール1つ分のチップ設定
struct RoleTileConfig
{
    bool bEdge13;      // true: edge13 モード / false: single モード
    int  singleGrpId;  // single モード時に敷く grpId
    EdgeTiles edge;    // edge13 モード時のチップ設定

    RoleTileConfig()
        : bEdge13(false)
        , singleGrpId(0)
        , edge()
    {
    }
};

// ロール種別（floor/wall/door/hall/stairs の5種）
enum RoleKind
{
    ROLE_FLOOR = 0,
    ROLE_WALL,
    ROLE_DOOR,
    ROLE_HALL,
    ROLE_STAIRS,
    ROLE_COUNT
};

// ロールごとのチップ設定一式
struct RoleTileSet
{
    RoleTileConfig roles[ROLE_COUNT];
};

// -----------------------------------------------------------------------
// ParseRoleMapJson
//
// roleMap の JSON 文字列（"floor":..., "wall":..., "door":..., "hall":...,
// "stairs":... を含むオブジェクトの中身）をパースして RoleTileSet を得る。
// 各キーは省略可。数値なら single、オブジェクトなら edge13 として解釈する。
// JsonUtils と同じ「対応括弧を手繰って部分文字列を切り出す」流儀を踏襲する。
//
// outTileSet は呼び出し前の値をベースにマージ上書きする
// （roleMapJson 内に存在しないロールキーは outTileSet の値をそのまま維持する）。
// これにより「DB既定値 → リクエスト上書き」のような複数段階の適用が可能。
// 呼び出し側は事前に outTileSet へフォールバック既定値をセットしておくこと。
//
// 戻り値: 常に true（パース失敗しても既定値のまま返すだけで良いため）
// -----------------------------------------------------------------------
bool ParseRoleMapJson(const std::string &roleMapJson, RoleTileSet &outTileSet);

// -----------------------------------------------------------------------
// GenerateTilesBase
//
// 生成グリッド（セル値配列、row-major, width*height）を
// セル値→ロール名変換関数 CellToRole を通してロール判定し、
// ロールごとのチップ設定に基づいて tilesBase 相当の grpId 配列を作る。
//
// CellToRole(cellValue) はセル値からロール種別を返す関数（または関数ポインタ）。
// グリッド範囲外は wall ロール扱いとする。
// -----------------------------------------------------------------------
typedef RoleKind (*CellToRoleFunc)(int cellValue);

void GenerateTilesBase(
    const std::vector<int> &grid,
    int width,
    int height,
    CellToRoleFunc cellToRole,
    const RoleTileSet &tileSet,
    std::vector<int> &outTilesBase);

} // namespace MapGenAutoTile
