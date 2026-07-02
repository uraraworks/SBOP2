#include "StdAfx.h"
#include "MapGenAutoTile.h"

#include <cctype>

#include "Web/JsonUtils.h"

namespace
{

// -----------------------------------------------------------------------
// ExtractBraceObject — key の直後に現れる '{' から対応する '}' までの
// 部分文字列を切り出す（ネスト深さを追跡）。
// MapGenPreviewHandler.cpp の "params"/"roleMap" 抽出と同じ流儀。
// -----------------------------------------------------------------------
bool ExtractBraceObject(const std::string &json, const std::string &key, std::string &outObj)
{
    size_t keyPos = JsonUtils::FindKey(json, key);
    if (keyPos == std::string::npos) {
        return false;
    }

    size_t braceOpen = json.find('{', keyPos);
    if (braceOpen == std::string::npos) {
        return false;
    }

    int depth = 0;
    size_t braceClose = std::string::npos;
    for (size_t i = braceOpen; i < json.size(); ++i) {
        if      (json[i] == '{') ++depth;
        else if (json[i] == '}') { --depth; if (depth == 0) { braceClose = i; break; } }
    }
    if (braceClose == std::string::npos) {
        return false;
    }

    outObj = json.substr(braceOpen, braceClose - braceOpen + 1);
    return true;
}

// roleJson が「オブジェクト」か「数値」かを判定する。
// key（ロール名）の直後の最初の非空白文字が '{' ならオブジェクト。
bool IsRoleValueObject(const std::string &json, const std::string &key)
{
    size_t keyPos = JsonUtils::FindKey(json, key);
    if (keyPos == std::string::npos) {
        return false;
    }
    size_t colonPos = json.find(':', keyPos + key.size() + 2);
    if (colonPos == std::string::npos) {
        return false;
    }
    size_t p = colonPos + 1;
    size_t nSize = json.size();
    while (p < nSize && std::isspace(static_cast<unsigned char>(json[p]))) {
        ++p;
    }
    return (p < nSize && json[p] == '{');
}

// tiles オブジェクトから1キー分の grpId を読む。未設定(欠落 or 0)なら c にフォールバック。
int ReadTileValue(const std::string &tilesJson, const std::string &key, int cValue)
{
    int v = 0;
    if (JsonUtils::TryGetInt(tilesJson, key, v) && v != 0) {
        return v;
    }
    return cValue;
}

// RoleTileConfig 1つ分をパースする（roleJson は該当ロールの値部分の文字列）。
// roleMapJson 内に roleKey が存在しない場合は outConfig を変更しない
// （呼び出し側の初期値/既定値をそのまま維持する。複数の roleMapJson を
//   段階的にマージ適用できるようにするための挙動）。
void ParseRoleTileConfig(const std::string &roleMapJson, const std::string &roleKey,
                         MapGenAutoTile::RoleTileConfig &outConfig)
{
    if (JsonUtils::FindKey(roleMapJson, roleKey) == std::string::npos) {
        return;
    }

    if (!IsRoleValueObject(roleMapJson, roleKey)) {
        // 数値 → single モード（従来どおり）
        int v = 0;
        if (!JsonUtils::TryGetInt(roleMapJson, roleKey, v)) {
            return;
        }
        outConfig.bEdge13     = false;
        outConfig.singleGrpId = v;
        return;
    }

    // オブジェクト → edge13 モード（を想定してパース。mode 不明でも tiles があれば edge13 扱い）
    std::string roleObj;
    if (!ExtractBraceObject(roleMapJson, roleKey, roleObj)) {
        // 取り出し失敗時は変更しない
        return;
    }

    std::string modeStr;
    JsonUtils::TryGetString(roleObj, "mode", modeStr);

    std::string tilesObj;
    bool hasTiles = ExtractBraceObject(roleObj, "tiles", tilesObj);

    outConfig.bEdge13 = (modeStr == "edge13") || hasTiles;

    if (!outConfig.bEdge13) {
        outConfig.singleGrpId = 0;
        return;
    }

    if (!hasTiles) {
        // tiles がない edge13 指定は全チップ 0（未設定）扱い
        return;
    }

    // c（中央）をまず読む。欠落 or 0 なら 0 のまま。
    int cValue = 0;
    JsonUtils::TryGetInt(tilesObj, "c", cValue);

    outConfig.edge.c     = cValue;
    outConfig.edge.n     = ReadTileValue(tilesObj, "n",  cValue);
    outConfig.edge.s     = ReadTileValue(tilesObj, "s",  cValue);
    outConfig.edge.w     = ReadTileValue(tilesObj, "w",  cValue);
    outConfig.edge.e     = ReadTileValue(tilesObj, "e",  cValue);
    outConfig.edge.nw    = ReadTileValue(tilesObj, "nw", cValue);
    outConfig.edge.ne    = ReadTileValue(tilesObj, "ne", cValue);
    outConfig.edge.sw    = ReadTileValue(tilesObj, "sw", cValue);
    outConfig.edge.se    = ReadTileValue(tilesObj, "se", cValue);
    outConfig.edge.inNW  = ReadTileValue(tilesObj, "inNW", cValue);
    outConfig.edge.inNE  = ReadTileValue(tilesObj, "inNE", cValue);
    outConfig.edge.inSW  = ReadTileValue(tilesObj, "inSW", cValue);
    outConfig.edge.inSE  = ReadTileValue(tilesObj, "inSE", cValue);
}

const char *const kRoleKeys[MapGenAutoTile::ROLE_COUNT] = {
    "floor", "wall", "door", "hall", "stairs"
};

} // namespace

namespace MapGenAutoTile
{

bool ParseRoleMapJson(const std::string &roleMapJson, RoleTileSet &outTileSet)
{
    for (int i = 0; i < ROLE_COUNT; ++i) {
        ParseRoleTileConfig(roleMapJson, kRoleKeys[i], outTileSet.roles[i]);
    }
    return true;
}

namespace
{

// 座標 (x,y) のロールを取得する。範囲外は wall 扱い。
inline RoleKind RoleAt(const std::vector<int> &grid, int width, int height,
                       CellToRoleFunc cellToRole, int x, int y)
{
    if (x < 0 || y < 0 || x >= width || y >= height) {
        return ROLE_WALL;
    }
    return cellToRole(grid[static_cast<size_t>(y) * width + x]);
}

// edge13 判定: 周囲8マスの同ロール/異ロールから1チップを決める。
int ResolveEdge13Tile(const std::vector<int> &grid, int width, int height,
                      CellToRoleFunc cellToRole, int x, int y, RoleKind selfRole,
                      const EdgeTiles &tiles)
{
    bool bN = (RoleAt(grid, width, height, cellToRole, x,     y - 1) == selfRole);
    bool bS = (RoleAt(grid, width, height, cellToRole, x,     y + 1) == selfRole);
    bool bW = (RoleAt(grid, width, height, cellToRole, x - 1, y    ) == selfRole);
    bool bE = (RoleAt(grid, width, height, cellToRole, x + 1, y    ) == selfRole);

    int sameCount = (bN ? 1 : 0) + (bS ? 1 : 0) + (bW ? 1 : 0) + (bE ? 1 : 0);

    if (sameCount == 4) {
        // 4方向すべて同ロール → 斜め4方向をチェックして内角判定
        bool bNW = (RoleAt(grid, width, height, cellToRole, x - 1, y - 1) == selfRole);
        bool bNE = (RoleAt(grid, width, height, cellToRole, x + 1, y - 1) == selfRole);
        bool bSW = (RoleAt(grid, width, height, cellToRole, x - 1, y + 1) == selfRole);
        bool bSE = (RoleAt(grid, width, height, cellToRole, x + 1, y + 1) == selfRole);

        int diffDiagCount = (bNW ? 0 : 1) + (bNE ? 0 : 1) + (bSW ? 0 : 1) + (bSE ? 0 : 1);

        if (diffDiagCount == 0) {
            return tiles.c;
        }
        if (diffDiagCount == 1) {
            if (!bNW) return tiles.inNW;
            if (!bNE) return tiles.inNE;
            if (!bSW) return tiles.inSW;
            /* !bSE */ return tiles.inSE;
        }
        // 2個以上異ロール → 表現不能につき c にフォールバック
        return tiles.c;
    }

    if (sameCount == 3) {
        // 1方向だけ異ロール → その方角の辺チップ
        if (!bN) return tiles.n;
        if (!bS) return tiles.s;
        if (!bW) return tiles.w;
        /* !bE */ return tiles.e;
    }

    if (sameCount == 2) {
        // 隣接する2方向が同ロールで残り2方向（隣接側）が異ロール → 外角
        // 例: 南と東が同ロール、北と西が異ロール → nw（異ロール側の角の名前）
        if (bS && bE && !bN && !bW) return tiles.nw;
        if (bS && bW && !bN && !bE) return tiles.ne;
        if (bN && bE && !bS && !bW) return tiles.sw;
        if (bN && bW && !bS && !bE) return tiles.se;
        // 対向2方向のみ同ロール（bN&&bS または bW&&bE）は表現不能 → c
        return tiles.c;
    }

    // sameCount が 1以下 → 表現不能につき c にフォールバック
    return tiles.c;
}

} // namespace

void GenerateTilesBase(
    const std::vector<int> &grid,
    int width,
    int height,
    CellToRoleFunc cellToRole,
    const RoleTileSet &tileSet,
    std::vector<int> &outTilesBase)
{
    int total = width * height;
    outTilesBase.assign(static_cast<size_t>(total), 0);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int idx = y * width + x;
            RoleKind role = cellToRole(grid[static_cast<size_t>(idx)]);
            if (role < 0 || role >= ROLE_COUNT) {
                role = ROLE_WALL;
            }
            const RoleTileConfig &cfg = tileSet.roles[role];

            int grpId;
            if (!cfg.bEdge13) {
                // single モード: 従来どおりそのまま敷く
                grpId = cfg.singleGrpId;
            } else {
                grpId = ResolveEdge13Tile(grid, width, height, cellToRole, x, y, role, cfg.edge);
                if (grpId == 0) {
                    grpId = cfg.edge.c;
                }
            }

            outTilesBase[static_cast<size_t>(idx)] = grpId;
        }
    }
}

} // namespace MapGenAutoTile
