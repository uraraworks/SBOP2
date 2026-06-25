#include "StdAfx.h"
#include "MapGenHellL4.h"

#include <random>
#include <ctime>
#include <algorithm>
#include <vector>
#include <cstdint>

namespace
{

struct L4Context
{
    int W;
    int H;
    int HW;
    int HH;
    int roomMin;
    int roomMax;
    std::vector<int> grid;
    std::mt19937 rng;

    int &Cell(int x, int y) { return grid[y * W + x]; }
    const int &Cell(int x, int y) const { return grid[y * W + x]; }

    bool InQ1(int x, int y) const
    {
        return x >= 1 && x < HW - 1 && y >= 1 && y < HH - 1;
    }

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

void FillRect(L4Context &ctx, int x1, int y1, int x2, int y2)
{
    for (int y = y1; y <= y2; ++y)
        for (int x = x1; x <= x2; ++x)
            if (ctx.InBounds(x, y))
                ctx.Cell(x, y) = 1;
}

// 最大連結成分だけ残し、他の床を壁に戻す。戻り値は残した床数。
// ミラー4分割が継ぎ目で非連結になる問題を吸収する。明示スタックで反復。
int KeepLargestComponent(L4Context &ctx)
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

bool CheckRoomQ1(L4Context &ctx, int x1, int y1, int x2, int y2)
{
    if (x1 < 1 || y1 < 1 || x2 >= ctx.HW - 1 || y2 >= ctx.HH - 1)
        return false;
    for (int y = y1; y <= y2; ++y)
        for (int x = x1; x <= x2; ++x)
            if (ctx.Cell(x, y) == 1)
                return false;
    return true;
}

void GenerateRoom(L4Context &ctx, int x1, int y1, int x2, int y2, int depth);

void TrySpawnEdge(L4Context &ctx, int ex1, int ey1, int ex2, int ey2, int depth)
{
    int w = ctx.RandInt(ctx.roomMin, ctx.roomMax);
    int h = ctx.RandInt(ctx.roomMin, ctx.roomMax);

    int cx = (ex1 + ex2) / 2;
    int cy = (ey1 + ey2) / 2;
    int rx1 = cx - w / 2;
    int ry1 = cy - h / 2;
    int rx2 = rx1 + w - 1;
    int ry2 = ry1 + h - 1;

    if (!CheckRoomQ1(ctx, rx1, ry1, rx2, ry2))
        return;

    FillRect(ctx, rx1, ry1, rx2, ry2);

    // 折れ線廊下
    int sx = (ex1 + ex2) / 2;
    int sy = (ey1 + ey2) / 2;
    int tx = (rx1 + rx2) / 2;
    int ty = (ry1 + ry2) / 2;
    int xlo = (sx < tx) ? sx : tx;
    int xhi = (sx > tx) ? sx : tx;
    int ylo = (sy < ty) ? sy : ty;
    int yhi = (sy > ty) ? sy : ty;
    for (int x = xlo; x <= xhi; ++x)
        if (ctx.InBounds(x, sy)) ctx.Cell(x, sy) = 1;
    for (int y = ylo; y <= yhi; ++y)
        if (ctx.InBounds(tx, y)) ctx.Cell(tx, y) = 1;

    GenerateRoom(ctx, rx1, ry1, rx2, ry2, depth + 1);
}

void GenerateRoom(L4Context &ctx, int x1, int y1, int x2, int y2, int depth)
{
    if (depth > 10) return;

    if (ctx.Chance(55))
        TrySpawnEdge(ctx, x1, y1 - 1, x2, y1 - 1, depth);
    if (ctx.Chance(55))
        TrySpawnEdge(ctx, x1, y2 + 1, x2, y2 + 1, depth);
    if (ctx.Chance(55))
        TrySpawnEdge(ctx, x1 - 1, y1, x1 - 1, y2, depth);
    if (ctx.Chance(55))
        TrySpawnEdge(ctx, x2 + 1, y1, x2 + 1, y2, depth);
}

// 初期部屋（第1象限中央付近）を配置して出芽開始
void FirstRoom(L4Context &ctx)
{
    int rw = ctx.RandInt(ctx.roomMin, ctx.roomMax);
    int rh = ctx.RandInt(ctx.roomMin, ctx.roomMax);
    int rx1 = ctx.HW / 2 - rw / 2;
    int ry1 = ctx.HH / 2 - rh / 2;
    int rx2 = rx1 + rw - 1;
    int ry2 = ry1 + rh - 1;

    if (rx1 < 1) rx1 = 1;
    if (ry1 < 1) ry1 = 1;
    if (rx2 >= ctx.HW - 1) rx2 = ctx.HW - 2;
    if (ry2 >= ctx.HH - 1) ry2 = ctx.HH - 2;

    FillRect(ctx, rx1, ry1, rx2, ry2);
    GenerateRoom(ctx, rx1, ry1, rx2, ry2, 0);
}

// 第1象限の内側に中央通路を確保（横1本＋縦1本）
// 参照: drlg_l4.cpp:869 PrepareInnerBorders()
void PrepareInnerBorders(L4Context &ctx)
{
    // 横通路は継ぎ目 x=HW-1 まで延ばす（ミラー後に x=HW と接続し左右象限が連結）
    int corridorY = ctx.RandInt(ctx.HH / 3, ctx.HH - 3);
    for (int x = 1; x < ctx.HW; ++x)
        ctx.Cell(x, corridorY) = 1;

    // 縦通路は継ぎ目 y=HH-1 まで延ばす（ミラー後に y=HH と接続し上下象限が連結）
    int corridorX = ctx.RandInt(ctx.HW / 3, ctx.HW - 3);
    for (int y = 1; y < ctx.HH; ++y)
        ctx.Cell(corridorX, y) = 1;
}

// 第1象限を残り3象限にミラーコピー
// 参照: drlg_l4.cpp:300 MirrorDungeonLayout()
void MirrorDungeonLayout(L4Context &ctx)
{
    for (int y = 0; y < ctx.HH; ++y)
    {
        for (int x = 0; x < ctx.HW; ++x)
        {
            int v = ctx.Cell(x, y);
            ctx.Cell(ctx.W - 1 - x, y) = v;
            ctx.Cell(x, ctx.H - 1 - y) = v;
            ctx.Cell(ctx.W - 1 - x, ctx.H - 1 - y) = v;
        }
    }
}

void BuildDungeon(L4Context &ctx)
{
    ctx.Reset();
    FirstRoom(ctx);
    PrepareInnerBorders(ctx);
    MirrorDungeonLayout(ctx);
}

} // anonymous namespace

namespace MapGenHellL4
{

bool GenerateHellL4(
    const MapGen::Params &p,
    std::vector<int> &outGrid,
    int &outW,
    int &outH,
    uint32_t &outSeed)
{
    int W = (p.width  % 2 == 0) ? p.width  : p.width  + 1;
    int H = (p.height % 2 == 0) ? p.height : p.height + 1;

    L4Context ctx;
    ctx.W = W;
    ctx.H = H;
    ctx.HW = W / 2;
    ctx.HH = H / 2;
    ctx.roomMin = (p.blockMin >= 2) ? p.blockMin : 2;
    ctx.roomMax = (p.blockMax >= ctx.roomMin) ? p.blockMax : ctx.roomMin + 4;
    ctx.grid.resize(ctx.W * ctx.H, 0);

    uint32_t seed = p.seed;
    if (seed == 0)
        seed = static_cast<uint32_t>(std::time(nullptr));

    for (int attempt = 0; attempt < p.maxRetries; ++attempt)
    {
        ctx.rng.seed(seed + static_cast<uint32_t>(attempt));

        BuildDungeon(ctx);

        // 最大連結成分だけ残す（ミラー継ぎ目の非連結を吸収）
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

} // namespace MapGenHellL4
