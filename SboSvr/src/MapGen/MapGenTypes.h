#pragma once

#include <cstdint>

// ---------------------------------------------------------------------------
// MapGen 共通パラメータ構造体
//
// 各ダンジョン生成エンジン（L1/L2/L3/L4）で共有する。
// アルゴ別に使わないフィールドは無視する。
// ---------------------------------------------------------------------------

namespace MapGen
{

struct Params
{
    int width;           // グリッド幅（デフォルト 40）
    int height;          // グリッド高（デフォルト 40）
    int floorAreaMin;    // 最小床面積
    int blockMin;        // ブロック/部屋辺の最小サイズ（アルゴ別に流用）
    int blockMax;        // ブロック/部屋辺の最大サイズ（アルゴ別に流用）
    int cutoffPercent;   // L3: 再帰打ち切り確率 %
    int maxRetries;      // リトライ安全上限（全アルゴ共通）
    uint32_t seed;       // 乱数シード（0 = ランダム）

    Params()
        : width(40)
        , height(40)
        , floorAreaMin(600)
        , blockMin(3)
        , blockMax(4)
        , cutoffPercent(25)
        , maxRetries(1000)
        , seed(0)
    {
    }
};

} // namespace MapGen
