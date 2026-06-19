#include "StdAfx.h"
#include "MapGenCaveL3.h"

#include <random>
#include <ctime>
#include <algorithm>
#include <functional>
#include <cstdint>

// ---------------------------------------------------------------------------
// 実装詳細（ファイルスコープ）
// ---------------------------------------------------------------------------

namespace
{

// ---------------------------------------------------------------------------
// GeneratorContext — 1回の生成試行で共有する状態
// ---------------------------------------------------------------------------
struct GeneratorContext
{
    int W;             // グリッド幅
    int H;             // グリッド高
    int blockMin;      // ブロック辺最小
    int blockMax;      // ブロック辺最大
    int cutoffPercent; // 打ち切り確率 %

    std::vector<int> grid; // 0=壁, 1=床

    std::mt19937 rng;  // 乱数エンジン

    // grid[y][x] アクセサ
    int &Cell(int x, int y) { return grid[y * W + x]; }
    const int &Cell(int x, int y) const { return grid[y * W + x]; }

    // 範囲内チェック
    bool InBounds(int x, int y) const
    {
        return x >= 0 && x < W && y >= 0 && y < H;
    }

    // [lo, hi] の整数乱数（両端含む）
    int RandInt(int lo, int hi)
    {
        std::uniform_int_distribution<int> dist(lo, hi);
        return dist(rng);
    }

    // p% の確率で true を返す
    bool Chance(int percent)
    {
        return RandInt(0, 99) < percent;
    }
};

// ---------------------------------------------------------------------------
// FillRoom — 矩形 (x1,y1)-(x2,y2) を床で塗る（内部+縁で50%）
//
// devilutionX の FillRoom(x1,y1,x2,y2):
//   - 内部 (x1+1..x2-1, y1+1..y2-1) は常に床
//   - 縁は 50% 確率で床
// 戻り値: 範囲が完全に bounds 内なら true、一部でもはみ出すなら false
// ---------------------------------------------------------------------------
bool FillRoom(GeneratorContext &ctx, int x1, int y1, int x2, int y2)
{
    // 範囲チェック（境界マスを 1 マス空けて配置できるか確認）
    if (x1 < 0 || y1 < 0 || x2 >= ctx.W || y2 >= ctx.H)
        return false;

    // 重複チェック: 内部に既存の床があれば配置しない
    for (int y = y1 + 1; y <= y2 - 1; ++y)
    {
        for (int x = x1 + 1; x <= x2 - 1; ++x)
        {
            if (ctx.Cell(x, y) == 1)
                return false;
        }
    }

    // 内部を床で塗る
    for (int y = y1 + 1; y <= y2 - 1; ++y)
        for (int x = x1 + 1; x <= x2 - 1; ++x)
            ctx.Cell(x, y) = 1;

    // 縁を 50% 確率で床に
    for (int x = x1; x <= x2; ++x)
    {
        if (ctx.Chance(50)) ctx.Cell(x, y1) = 1;
        if (ctx.Chance(50)) ctx.Cell(x, y2) = 1;
    }
    for (int y = y1 + 1; y <= y2 - 1; ++y)
    {
        if (ctx.Chance(50)) ctx.Cell(x1, y) = 1;
        if (ctx.Chance(50)) ctx.Cell(x2, y) = 1;
    }

    return true;
}

// 方向定数（devilutionX の CreateBlock dir 引数に対応）
enum Dir { DIR_NORTH = 0, DIR_SOUTH = 1, DIR_WEST = 2, DIR_EAST = 3 };

// ---------------------------------------------------------------------------
// CreateBlock — 再帰的にブロックを生やす
//
// point: 親ブロックの接続辺上の座標
// obs:   接続辺の方向（親から見て「来た辺」）
// dir:   今回伸ばす方向（obs と反対が多い）
// ---------------------------------------------------------------------------
void CreateBlock(GeneratorContext &ctx, int px, int py, int obs)
{
    // 打ち切り判定（25%）
    if (ctx.Chance(ctx.cutoffPercent))
        return;

    // ブロック辺サイズをランダムに決める（blockMin〜blockMax）
    int bw = ctx.RandInt(ctx.blockMin, ctx.blockMax); // 幅
    int bh = ctx.RandInt(ctx.blockMin, ctx.blockMax); // 高さ

    // 接続辺(obs)に基づいて新ブロックの位置を計算
    // devilutionX: obs は「どの辺から来たか」を表す
    //   親の SOUTH 辺 → 新ブロックは下(南)に伸ばす
    //   obs=NORTH → 新ブロックは (px±rand, py-1) から上向きに bh 段
    int x1, y1, x2, y2;

    if (obs == DIR_NORTH)
    {
        // 接続点は親の北辺 → 新ブロックは接続点の上側
        int offset = ctx.RandInt(0, bw - 1);
        x1 = px - offset;
        x2 = x1 + bw;
        y2 = py - 1; // 親の北辺の1つ上が新ブロックの南辺
        y1 = y2 - bh;
    }
    else if (obs == DIR_SOUTH)
    {
        // 接続点は親の南辺 → 新ブロックは接続点の下側
        int offset = ctx.RandInt(0, bw - 1);
        x1 = px - offset;
        x2 = x1 + bw;
        y1 = py + 1;
        y2 = y1 + bh;
    }
    else if (obs == DIR_WEST)
    {
        // 接続点は親の西辺 → 新ブロックは接続点の左側
        int offset = ctx.RandInt(0, bh - 1);
        y1 = py - offset;
        y2 = y1 + bh;
        x2 = px - 1;
        x1 = x2 - bw;
    }
    else // DIR_EAST
    {
        // 接続点は親の東辺 → 新ブロックは接続点の右側
        int offset = ctx.RandInt(0, bh - 1);
        y1 = py - offset;
        y2 = y1 + bh;
        x1 = px + 1;
        x2 = x1 + bw;
    }

    // 配置試行（範囲外 or 重複なら終了）
    if (!FillRoom(ctx, x1, y1, x2, y2))
        return;

    // 来た辺(obs)以外の3辺に再帰
    // 各辺の中点を接続点として渡す
    int midX = (x1 + x2) / 2;
    int midY = (y1 + y2) / 2;

    if (obs != DIR_SOUTH)  CreateBlock(ctx, midX, y1, DIR_NORTH);
    if (obs != DIR_NORTH)  CreateBlock(ctx, midX, y2, DIR_SOUTH);
    if (obs != DIR_EAST)   CreateBlock(ctx, x1, midY, DIR_WEST);
    if (obs != DIR_WEST)   CreateBlock(ctx, x2, midY, DIR_EAST);
}

// ---------------------------------------------------------------------------
// CountFloor — グリッド内の床セル数を数える
// ---------------------------------------------------------------------------
int CountFloor(const GeneratorContext &ctx)
{
    int count = 0;
    for (int v : ctx.grid)
        if (v == 1) ++count;
    return count;
}

// ---------------------------------------------------------------------------
// FloodFill — 再帰フラッドフィルで到達床セルを visited にマークし数を返す
// ---------------------------------------------------------------------------
void FloodFill(const GeneratorContext &ctx, int x, int y,
               std::vector<bool> &visited, int &count)
{
    if (!ctx.InBounds(x, y))     return;
    if (visited[y * ctx.W + x]) return;
    if (ctx.Cell(x, y) != 1)    return;

    visited[y * ctx.W + x] = true;
    ++count;

    FloodFill(ctx, x + 1, y, visited, count);
    FloodFill(ctx, x - 1, y, visited, count);
    FloodFill(ctx, x, y + 1, visited, count);
    FloodFill(ctx, x, y - 1, visited, count);
}

// ---------------------------------------------------------------------------
// Lockout — 全床セルが連結しているか確認
//
// 任意の床1マスからフラッドフィルし、到達数 == 全床数なら true。
// ---------------------------------------------------------------------------
bool Lockout(const GeneratorContext &ctx)
{
    int totalFloor = CountFloor(ctx);
    if (totalFloor == 0)
        return false;

    // 最初の床セルを探す
    int startX = -1, startY = -1;
    for (int y = 0; y < ctx.H && startX < 0; ++y)
        for (int x = 0; x < ctx.W && startX < 0; ++x)
            if (ctx.Cell(x, y) == 1)
            {
                startX = x;
                startY = y;
            }

    std::vector<bool> visited(ctx.W * ctx.H, false);
    int reached = 0;
    FloodFill(ctx, startX, startY, visited, reached);

    return reached == totalFloor;
}

// ---------------------------------------------------------------------------
// 起点ブロックを配置して CreateBlock を4辺に呼ぶ
// ---------------------------------------------------------------------------
void GenerateOnce(GeneratorContext &ctx)
{
    // グリッドをすべて壁でクリア
    std::fill(ctx.grid.begin(), ctx.grid.end(), 0);

    // 起点座標: GenerateRnd(20)+10 相当
    // 有効範囲を考慮して中央寄りにランダム配置
    int startX = ctx.RandInt(10, ctx.W - 14); // x1 が 10〜W-14 に収まるよう
    int startY = ctx.RandInt(10, ctx.H - 14);

    // 3×3 起点ブロックを配置（x2=x1+2, y2=y1+2 で有効内部3×3）
    // 縁の50%処理なしで強制的に床にする（devilutionX GenerateLevel側で直接塗る）
    for (int y = startY; y <= startY + 2; ++y)
        for (int x = startX; x <= startX + 2; ++x)
            ctx.Cell(x, y) = 1;

    // 起点ブロックの4辺に CreateBlock を呼ぶ
    int midX = startX + 1;
    int midY = startY + 1;

    CreateBlock(ctx, midX, startY,     DIR_NORTH); // 上辺
    CreateBlock(ctx, midX, startY + 2, DIR_SOUTH); // 下辺
    CreateBlock(ctx, startX,     midY, DIR_WEST);  // 左辺
    CreateBlock(ctx, startX + 2, midY, DIR_EAST);  // 右辺
}

} // namespace (anonymous)

// ---------------------------------------------------------------------------
// 公開関数実装
// ---------------------------------------------------------------------------

namespace MapGenCaveL3
{

bool GenerateCaveL3(
    const Params &p,
    std::vector<int> &outGrid,
    int &outW,
    int &outH,
    int &outSeed)
{
    // パラメータ検証
    if (p.width < 10 || p.height < 10)         return false;
    if (p.blockMin < 2 || p.blockMax < p.blockMin) return false;
    if (p.floorAreaMin <= 0)                    return false;
    if (p.maxRetries <= 0)                      return false;

    // シードを確定
    uint32_t usedSeed = p.seed;
    if (usedSeed == 0)
        usedSeed = static_cast<uint32_t>(std::time(NULL));

    GeneratorContext ctx;
    ctx.W            = p.width;
    ctx.H            = p.height;
    ctx.blockMin     = p.blockMin;
    ctx.blockMax     = p.blockMax;
    ctx.cutoffPercent = p.cutoffPercent;
    ctx.grid.resize(p.width * p.height, 0);
    ctx.rng.seed(usedSeed);

    // リトライループ（安全上限: p.maxRetries）
    for (int attempt = 0; attempt < p.maxRetries; ++attempt)
    {
        GenerateOnce(ctx);

        // 床面積チェック
        if (CountFloor(ctx) < p.floorAreaMin)
            continue;

        // 連結チェック（Lockout）
        if (!Lockout(ctx))
            continue;

        // 成功
        outW    = p.width;
        outH    = p.height;
        outSeed = static_cast<int>(usedSeed);
        outGrid = ctx.grid;
        return true;
    }

    // リトライ上限超過
    return false;
}

} // namespace MapGenCaveL3
