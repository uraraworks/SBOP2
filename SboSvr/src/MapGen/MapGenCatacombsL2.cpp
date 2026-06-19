#include "StdAfx.h"
#include "MapGenCatacombsL2.h"

#include <random>
#include <ctime>
#include <algorithm>
#include <vector>
#include <cstdint>

namespace
{

struct Room
{
    int x1, y1, x2, y2;
};

struct HallNode
{
    int bx, by;
    int ex, ey;
};

struct L2Context
{
    int W;
    int H;
    int roomMin;
    int roomMax;
    std::vector<int> grid;
    std::mt19937 rng;
    std::vector<Room> roomList;
    std::vector<HallNode> hallList;
    int roomCnt;
    static const int kMaxRooms = 80;

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
        roomList.clear();
        hallList.clear();
        roomCnt = 0;
    }

    int FloorArea() const
    {
        int cnt = 0;
        for (int i = 0; i < W * H; ++i) if (grid[i] == 1) ++cnt;
        return cnt;
    }
};

// 再帰フラッドフィル
void FloodFill(const L2Context &ctx, int x, int y, std::vector<bool> &visited, int &count)
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

bool IsConnected(const L2Context &ctx)
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

void PlaceRoom(L2Context &ctx, int x1, int y1, int x2, int y2)
{
    for (int y = y1; y <= y2; ++y)
        for (int x = x1; x <= x2; ++x)
            if (ctx.InBounds(x, y))
                ctx.Cell(x, y) = 1;
}

// ステアリングアルゴリズムで廊下を引く
// 参照: drlg_l2.cpp:1878 ConnectHall()
void ConnectHall(L2Context &ctx, HallNode hall)
{
    int cx = hall.bx;
    int cy = hall.by;
    int ex = hall.ex;
    int ey = hall.ey;

    bool fMinus = ctx.Chance(50);
    bool fPlus  = ctx.Chance(50);

    while (cx != ex || cy != ey)
    {
        if (ctx.InBounds(cx, cy))
        {
            ctx.Cell(cx, cy) = 1;
            if (fMinus)
            {
                if (cx != ex && ctx.InBounds(cx, cy - 1)) ctx.Cell(cx, cy - 1) = 1;
                if (cy != ey && ctx.InBounds(cx - 1, cy)) ctx.Cell(cx - 1, cy) = 1;
            }
            if (fPlus)
            {
                if (cx != ex && ctx.InBounds(cx, cy + 1)) ctx.Cell(cx, cy + 1) = 1;
                if (cy != ey && ctx.InBounds(cx + 1, cy)) ctx.Cell(cx + 1, cy) = 1;
            }
        }

        int nDx = (ex > cx) ? (ex - cx) : (cx - ex);
        int nDy = (ey > cy) ? (ey - cy) : (cy - ey);

        int probH = (nDx * 2 < 30) ? nDx * 2 : 30;
        int probV = (nDy * 5 < 80) ? nDy * 5 : 80;

        bool goH = (nDx > 0) && (nDy == 0 || ctx.Chance(probH));
        bool goV = (nDy > 0) && (nDx == 0 || ctx.Chance(probV));

        if (!goH && !goV)
        {
            goH = (nDx >= nDy);
            goV = !goH;
        }

        if (goH)
            cx += (ex > cx) ? 1 : -1;
        else if (goV)
            cy += (ey > cy) ? 1 : -1;
    }

    if (ctx.InBounds(cx, cy))
        ctx.Cell(cx, cy) = 1;
}

// 再帰4分割部屋生成
// 参照: drlg_l2.cpp:1766 CreateRoom()
void CreateRoom(L2Context &ctx,
                int ax1, int ay1, int ax2, int ay2,
                int parentRoomIdx)
{
    if (ctx.roomCnt >= L2Context::kMaxRooms)
        return;
    if (ax2 - ax1 < 2 || ay2 - ay1 < 2)
        return;

    int areaW = ax2 - ax1;
    int areaH = ay2 - ay1;

    int rMax = (areaW < ctx.roomMax) ? areaW : ctx.roomMax;
    int rMin = (rMax >= ctx.roomMin) ? ctx.roomMin : rMax;
    int rw = (rMax > rMin) ? ctx.RandInt(rMin, rMax) : rMax;

    rMax = (areaH < ctx.roomMax) ? areaH : ctx.roomMax;
    rMin = (rMax >= ctx.roomMin) ? ctx.roomMin : rMax;
    int rh = (rMax > rMin) ? ctx.RandInt(rMin, rMax) : rMax;

    int maxRx = ax2 - rw;
    if (maxRx < ax1) maxRx = ax1;
    int maxRy = ay2 - rh;
    if (maxRy < ay1) maxRy = ay1;

    int rx1 = (maxRx > ax1) ? ctx.RandInt(ax1, maxRx) : ax1;
    int ry1 = (maxRy > ay1) ? ctx.RandInt(ay1, maxRy) : ay1;

    // clamp [1, W-2] / [1, H-2]
    if (rx1 < 1) rx1 = 1;
    if (ry1 < 1) ry1 = 1;
    int rx2 = rx1 + rw - 1;
    int ry2 = ry1 + rh - 1;
    if (rx2 >= ctx.W - 1) rx2 = ctx.W - 2;
    if (ry2 >= ctx.H - 1) ry2 = ctx.H - 2;

    PlaceRoom(ctx, rx1, ry1, rx2, ry2);

    int thisIdx = (int)ctx.roomList.size();
    Room rm = {rx1, ry1, rx2, ry2};
    ctx.roomList.push_back(rm);
    ctx.roomCnt++;

    if (parentRoomIdx >= 0 && parentRoomIdx < (int)ctx.roomList.size() - 1)
    {
        const Room &pr = ctx.roomList[parentRoomIdx];
        HallNode hn;
        hn.bx = (rx1 + rx2) / 2;
        hn.by = (ry1 + ry2) / 2;
        hn.ex = (pr.x1 + pr.x2) / 2;
        hn.ey = (pr.y1 + pr.y2) / 2;
        ctx.hallList.push_back(hn);
    }

    // 再帰4分割
    bool splitV = (areaH > areaW);

    if (splitV)
    {
        if (ry1 - ay1 > 2)
            CreateRoom(ctx, ax1, ay1, ax2, ry1 - 1, thisIdx);
        if (ay2 - ry2 > 2)
            CreateRoom(ctx, ax1, ry2 + 1, ax2, ay2, thisIdx);
        if (rx1 - ax1 > 2)
            CreateRoom(ctx, ax1, ay1, rx1 - 1, ay2, thisIdx);
        if (ax2 - rx2 > 2)
            CreateRoom(ctx, rx2 + 1, ay1, ax2, ay2, thisIdx);
    }
    else
    {
        if (rx1 - ax1 > 2)
            CreateRoom(ctx, ax1, ay1, rx1 - 1, ay2, thisIdx);
        if (ax2 - rx2 > 2)
            CreateRoom(ctx, rx2 + 1, ay1, ax2, ay2, thisIdx);
        if (ry1 - ay1 > 2)
            CreateRoom(ctx, ax1, ay1, ax2, ry1 - 1, thisIdx);
        if (ay2 - ry2 > 2)
            CreateRoom(ctx, ax1, ry2 + 1, ax2, ay2, thisIdx);
    }
}

bool BuildDungeon(L2Context &ctx)
{
    ctx.Reset();
    CreateRoom(ctx, 2, 2, ctx.W - 2, ctx.H - 2, -1);

    for (int i = 0; i < (int)ctx.hallList.size(); ++i)
        ConnectHall(ctx, ctx.hallList[i]);

    return true;
}

} // anonymous namespace

namespace MapGenCatacombsL2
{

bool GenerateCatacombsL2(
    const MapGen::Params &p,
    std::vector<int> &outGrid,
    int &outW,
    int &outH,
    uint32_t &outSeed)
{
    L2Context ctx;
    ctx.W = p.width;
    ctx.H = p.height;
    ctx.roomMin = (p.blockMin >= 4) ? p.blockMin : 4;
    ctx.roomMax = (p.blockMax >= ctx.roomMin) ? p.blockMax : 10;
    ctx.grid.resize(ctx.W * ctx.H, 0);
    ctx.roomCnt = 0;

    uint32_t seed = p.seed;
    if (seed == 0)
        seed = static_cast<uint32_t>(std::time(nullptr));

    for (int attempt = 0; attempt < p.maxRetries; ++attempt)
    {
        ctx.rng.seed(seed + static_cast<uint32_t>(attempt));

        BuildDungeon(ctx);

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

} // namespace MapGenCatacombsL2
