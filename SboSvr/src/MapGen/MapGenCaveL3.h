#pragma once

#include <vector>
#include <cstdint>

// ---------------------------------------------------------------------------
// MapGenCaveL3 — Diablo 1 L3 Caves 方式の洞窟マップ自動生成エンジン
//
// 参照: docs/dungeon-generation-diablo1-reference.md "L3 Caves" セクション
// 生成フロー:
//   1. 中央近傍にランダム起点を選び 3×3 の起点ブロックを配置
//   2. CreateBlock() で4辺に再帰的にブロックを生やす
//      - 内部は常に床(1)、縁は 50% 確率で床
//      - 25% 確率で打ち切り、残り75%で来た辺以外3辺に再帰
//   3. 床面積 >= floorAreaMin かつ全床連結(Lockout)を満たすまで再生成
//   4. 安全上限(maxRetries)超過で失敗を返す
// ---------------------------------------------------------------------------

namespace MapGenCaveL3
{

// 生成パラメータ
struct Params
{
    int width;          // グリッド幅（デフォルト 40）
    int height;         // グリッド高（デフォルト 40）
    int floorAreaMin;   // 最小床面積（デフォルト 600）
    int blockMin;       // ブロック辺最小サイズ（デフォルト 3）
    int blockMax;       // ブロック辺最大サイズ（デフォルト 4）
    int cutoffPercent;  // 再帰打ち切り確率 % （デフォルト 25 = 1/4）
    uint32_t seed;      // 乱数シード（0 = ランダム）
    int maxRetries;     // リトライ安全上限（デフォルト 1000）

    // デフォルト値で初期化
    Params()
        : width(40)
        , height(40)
        , floorAreaMin(600)
        , blockMin(3)
        , blockMax(4)
        , cutoffPercent(25)
        , seed(0)
        , maxRetries(1000)
    {
    }
};

// 生成結果グリッドのセル値
enum CellValue
{
    CELL_WALL  = 0,  // 壁／未確定
    CELL_FLOOR = 1,  // 床
};

// ---------------------------------------------------------------------------
// GenerateCaveL3
//
// 洞窟マップを生成して outGrid に row-major で格納する。
// outGrid のサイズは outW * outH。
//
// 戻り値:
//   true  — 生成成功。outW/outH/outSeed に結果を格納。
//   false — maxRetries 超過で生成失敗。
// ---------------------------------------------------------------------------
bool GenerateCaveL3(
    const Params &p,
    std::vector<int> &outGrid,
    int &outW,
    int &outH,
    int &outSeed);

} // namespace MapGenCaveL3
