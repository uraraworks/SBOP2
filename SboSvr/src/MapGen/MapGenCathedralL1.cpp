#include "StdAfx.h"
#include "MapGenCathedralL1.h"

#include <random>
#include <ctime>
#include <algorithm>
#include <cstdint>
#include <vector>

namespace
{

struct L1Context
{
    int W;
    int H;
    int roomMin;
    int roomMax;
    std::vector<int> grid;
    std::mt19937 rng;

    int &Cell(int x, int y) { return grid[y * W + x]; }
    const int &Cell(int x, int y) const { return grid[y * W + x]; }

    bool InBounds(int x, int y) const
    {
        return x >= 0 && x < W && y >= 0 && y < H;
    }

    int RandInt(int lo, int hi)
    {
        std::uniform_int_distribution<int> dist(lo, hi);
        return dist(rng);
    }

    bool Chance(int percent)
    {
        return RandInt(0, 99) < percent;
    }

    void Reset()
    {
        std::fill(grid.begin(), grid.end(), 0);
    }

    int FloorArea() const
    {
        int cnt = 0;
        for (int i = 0; i < W * H; ++i) if (grid[i] == 1) ++cnt;
        return cnt;
    }
};

void FillRect(L1Context &ctx, int x1, int y1, int x2, int y2)
{
    for (int y = y1; y <= y2; ++y)
        for (int x = x1; x <= x2; ++x)
            if (ctx.InBounds(x, y))
                ctx.Cell(x, y) = 1;
}

// 最大連結成分だけ残し、他の床を壁に戻す。戻り値は残した床数。
// 欠けチェンバーで孤立部屋が出ても吸収する。明示スタックで反復。
int KeepLargestComponent(L1Context &ctx)
{
    const int W = ctx.W, H = ctx.H;
    std::vector<int> label(W * H, 0);
    int nextId = 0, bestId = 0, bestSize = 0;
    std::vector<int> stack;
    stack.reserve(W * H);

    for (int s = 0; s < W * H; ++s)
    {
        if (ctx.grid[s] != 1 || label[s] != 0)
            continue;
        int id = ++nextId, sz = 0;
        stack.clear();
        stack.push_back(s);
        label[s] = id;
        while (!stack.empty())
        {
            int idx = stack.back(); stack.pop_back();
            ++sz;
            int x = idx % W, y = idx / W;
            const int nb[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
            for (int k = 0; k < 4; ++k)
            {
                int nx = x + nb[k][0], ny = y + nb[k][1];
                if (nx < 0 || nx >= W || ny < 0 || ny >= H) continue;
                int nidx = ny * W + nx;
                if (ctx.grid[nidx] == 1 && label[nidx] == 0)
                {
                    label[nidx] = id;
                    stack.push_back(nidx);
                }
            }
        }
        if (sz > bestSize) { bestSize = sz; bestId = id; }
    }

    if (bestId == 0) return 0;
    for (int i = 0; i < W * H; ++i)
        if (ctx.grid[i] == 1 && label[i] != bestId)
            ctx.grid[i] = 0;
    return bestSize;
}

bool CheckRoom(L1Context &ctx, int x1, int y1, int x2, int y2)
{
    if (x1 < 1 || y1 < 1 || x2 >= ctx.W - 1 || y2 >= ctx.H - 1)
        return false;
    for (int y = y1; y <= y2; ++y)
        for (int x = x1; x <= x2; ++x)
            if (ctx.Cell(x, y) == 1)
                return false;
    return true;
}

void GenerateRoom(L1Context &ctx, int x1, int y1, int x2, int y2, int depth);

void TrySpawnFromEdge(L1Context &ctx, int ex1, int ey1, int ex2, int ey2, int depth)
{
    int w = ctx.RandInt(ctx.roomMin, ctx.roomMax);
    if (w % 2 != 0) w++;
    int h = ctx.RandInt(ctx.roomMin, ctx.roomMax);
    if (h % 2 != 0) h++;

    int cx = (ex1 + ex2) / 2;
    int cy = (ey1 + ey2) / 2;
    int rx1 = cx - w / 2;
    int ry1 = cy - h / 2;
    int rx2 = rx1 + w - 1;
    int ry2 = ry1 + h - 1;

    if (!CheckRoom(ctx, rx1, ry1, rx2, ry2))
        return;

    FillRect(ctx, rx1, ry1, rx2, ry2);

    // 廊下（辺→新部屋の中心を直線で繋ぐ）
    int sx = (ex1 + ex2) / 2;
    int sy = (ey1 + ey2) / 2;
    int tx = (rx1 + rx2) / 2;
    int ty = (ry1 + ry2) / 2;
    for (int x = (sx < tx ? sx : tx); x <= (sx > tx ? sx : tx); ++x)
        if (ctx.InBounds(x, sy)) ctx.Cell(x, sy) = 1;
    for (int y = (sy < ty ? sy : ty); y <= (sy > ty ? sy : ty); ++y)
        if (ctx.InBounds(tx, y)) ctx.Cell(tx, y) = 1;

    GenerateRoom(ctx, rx1, ry1, rx2, ry2, depth + 1);
}

void GenerateRoom(L1Context &ctx, int x1, int y1, int x2, int y2, int depth)
{
    if (depth > 12) return;

    if (ctx.Chance(60))
        TrySpawnFromEdge(ctx, x1, y1 - 1, x2, y1 - 1, depth);
    if (ctx.Chance(60))
        TrySpawnFromEdge(ctx, x1, y2 + 1, x2, y2 + 1, depth);
    if (ctx.Chance(60))
        TrySpawnFromEdge(ctx, x1 - 1, y1, x1 - 1, y2, depth);
    if (ctx.Chance(60))
        TrySpawnFromEdge(ctx, x2 + 1, y1, x2 + 1, y2, depth);
}

// チェンバー(大部屋)をグリッド全体に格子状に敷き詰め、隣接を廊下で接続する。
// 40×40でも200×200でもサイズに比例して部屋数が増える（旧版は中央1行固定だった）。
void BuildSpine(L1Context &ctx)
{
    ctx.Reset();

    const int cs      = 10;          // チェンバー1辺
    const int gap     = 5;           // チェンバー間の隙間（廊下用）
    const int spacing = cs + gap;    // 格子ピッチ
    const int margin  = 1;           // 外周マージン

    int cols = (ctx.W - 2 * margin + gap) / spacing;
    int rows = (ctx.H - 2 * margin + gap) / spacing;
    if (cols < 1) cols = 1;
    if (rows < 1) rows = 1;

    // 各格子セルのチェンバー中心（無効は cx<0）
    std::vector<int> cxArr(cols * rows, -1);
    std::vector<int> cyArr(cols * rows, -1);

    // チェンバー配置（12%は空きにして単調さを避ける）
    for (int gy = 0; gy < rows; ++gy)
    {
        for (int gx = 0; gx < cols; ++gx)
        {
            int x1 = margin + gx * spacing;
            int y1 = margin + gy * spacing;
            int x2 = x1 + cs - 1;
            int y2 = y1 + cs - 1;
            if (x2 >= ctx.W - 1) x2 = ctx.W - 2;
            if (y2 >= ctx.H - 1) y2 = ctx.H - 2;
            if (x1 >= x2 - 1 || y1 >= y2 - 1) continue; // 小さすぎる端は捨てる

            if (!ctx.Chance(88)) continue; // 12%は空きにして変化をつける

            FillRect(ctx, x1, y1, x2, y2);
            cxArr[gy * cols + gx] = (x1 + x2) / 2;
            cyArr[gy * cols + gx] = (y1 + y2) / 2;
        }
    }

    // 隣接チェンバーを廊下で接続（右隣・下隣）。これで全体が連結する。
    for (int gy = 0; gy < rows; ++gy)
    {
        for (int gx = 0; gx < cols; ++gx)
        {
            int idx = gy * cols + gx;
            if (cxArr[idx] < 0) continue;
            int cx = cxArr[idx], cy = cyArr[idx];

            if (gx + 1 < cols)
            {
                int r = gy * cols + (gx + 1);
                if (cxArr[r] >= 0)
                    for (int x = cx; x <= cxArr[r]; ++x) ctx.Cell(x, cy) = 1;
            }
            if (gy + 1 < rows)
            {
                int d = (gy + 1) * cols + gx;
                if (cxArr[d] >= 0)
                    for (int y = cy; y <= cyArr[d]; ++y) ctx.Cell(cx, y) = 1;
            }
        }
    }

    // 各チェンバーの辺から小部屋を出芽させて賑やかにする
    for (int gy = 0; gy < rows; ++gy)
    {
        for (int gx = 0; gx < cols; ++gx)
        {
            int idx = gy * cols + gx;
            if (cxArr[idx] < 0) continue;
            int cx = cxArr[idx], cy = cyArr[idx];
            GenerateRoom(ctx, cx - cs / 2, cy - cs / 2, cx + cs / 2, cy + cs / 2, 0);
        }
    }
}

} // anonymous namespace

namespace MapGenCathedralL1
{

bool GenerateCathedralL1(
    const MapGen::Params &p,
    std::vector<int> &outGrid,
    int &outW,
    int &outH,
    uint32_t &outSeed)
{
    L1Context ctx;
    ctx.W = p.width;
    ctx.H = p.height;
    ctx.roomMin = (p.blockMin > 2) ? p.blockMin : 2;
    ctx.roomMax = (p.blockMax > ctx.roomMin) ? p.blockMax : ctx.roomMin + 2;
    ctx.grid.resize(ctx.W * ctx.H, 0);

    uint32_t seed = p.seed;
    if (seed == 0)
        seed = static_cast<uint32_t>(std::time(nullptr));

    for (int attempt = 0; attempt < p.maxRetries; ++attempt)
    {
        ctx.rng.seed(seed + static_cast<uint32_t>(attempt));

        BuildSpine(ctx);

        // 最大連結成分だけ残す（欠けチェンバーの孤立を吸収）
        int area = KeepLargestComponent(ctx);
        if (area >= p.floorAreaMin)
        {
            outGrid = ctx.grid;
            outW    = ctx.W;
            outH    = ctx.H;
            outSeed = seed + static_cast<uint32_t>(attempt);
            return true;
        }
    }

    return false;
}

} // namespace MapGenCathedralL1
