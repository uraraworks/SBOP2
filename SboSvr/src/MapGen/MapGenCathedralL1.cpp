#include "StdAfx.h"
#include "MapGenCathedralL1.h"

#include <random>
#include <ctime>
#include <algorithm>
#include <cstdint>

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

// 再帰フラッドフィル（L3と同スタイル）
void FloodFill(const L1Context &ctx, int x, int y, std::vector<bool> &visited, int &count)
{
    if (!ctx.InBounds(x, y)) return;
    if (visited[y * ctx.W + x]) return;
    if (ctx.grid[y * ctx.W + x] != 1) return;
    visited[y * ctx.W + x] = true;
    ++count;
    FloodFill(ctx, x + 1, y, visited, count);
    FloodFill(ctx, x - 1, y, visited, count);
    FloodFill(ctx, x, y + 1, visited, count);
    FloodFill(ctx, x, y - 1, visited, count);
}

bool IsConnected(const L1Context &ctx)
{
    int total = ctx.FloorArea();
    if (total == 0) return false;
    std::vector<bool> visited(ctx.W * ctx.H, false);
    int count = 0;
    for (int y = 0; y < ctx.H; ++y)
        for (int x = 0; x < ctx.W; ++x)
            if (ctx.grid[y * ctx.W + x] == 1)
            {
                FloodFill(ctx, x, y, visited, count);
                return count == total;
            }
    return false;
}

void FillRect(L1Context &ctx, int x1, int y1, int x2, int y2)
{
    for (int y = y1; y <= y2; ++y)
        for (int x = x1; x <= x2; ++x)
            if (ctx.InBounds(x, y))
                ctx.Cell(x, y) = 1;
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

void BuildSpine(L1Context &ctx)
{
    ctx.Reset();

    int cy = ctx.H / 2 - 5;

    bool hasCh1 = ctx.Chance(50);
    bool hasCh2 = true;
    bool hasCh3 = ctx.Chance(50);
    (void)hasCh2;

    int ch1x = 1;
    int ch2x = ctx.W / 2 - 5;
    int ch3x = ctx.W - 11;

    FillRect(ctx, ch2x, cy, ch2x + 9, cy + 9);

    if (hasCh1 && ch1x + 9 < ch2x - 1)
    {
        FillRect(ctx, ch1x, cy, ch1x + 9, cy + 9);
        int corridorY = cy + 3;
        FillRect(ctx, ch1x + 10, corridorY, ch2x - 1, corridorY + 3);
    }

    if (hasCh3 && ch3x > ch2x + 10)
    {
        FillRect(ctx, ch3x, cy, ch3x + 9, cy + 9);
        int corridorY = cy + 3;
        FillRect(ctx, ch2x + 10, corridorY, ch3x - 1, corridorY + 3);
    }

    GenerateRoom(ctx, ch2x, cy, ch2x + 9, cy + 9, 0);
    if (hasCh1 && ch1x + 9 < ch2x - 1)
        GenerateRoom(ctx, ch1x, cy, ch1x + 9, cy + 9, 0);
    if (hasCh3 && ch3x > ch2x + 10)
        GenerateRoom(ctx, ch3x, cy, ch3x + 9, cy + 9, 0);
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

        int area = ctx.FloorArea();
        if (area >= p.floorAreaMin && IsConnected(ctx))
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
