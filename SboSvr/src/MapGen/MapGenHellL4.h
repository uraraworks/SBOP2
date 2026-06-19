#pragma once

#include <vector>
#include <cstdint>
#include "MapGenTypes.h"

// ---------------------------------------------------------------------------
// MapGenHellL4 — Diablo 1 L4 Hell 方式の地獄マップ自動生成エンジン
//
// 参照: docs/dungeon-generation-diablo1-reference.md "L4 Hell" セクション
// 生成フロー:
//   1. 第1象限（W/2 × H/2）で FirstRoom() → GenerateRoom() で部屋出芽
//   2. PrepareInnerBorders() で横1本＋縦1本の中央通路を確保
//   3. MirrorDungeonLayout() で4象限ミラーコピーして全体化
//   4. 床面積 >= floorAreaMin かつ全床連結を満たすまで再生成
//   5. 安全上限(maxRetries) 超過で失敗を返す
// ---------------------------------------------------------------------------

namespace MapGenHellL4
{

bool GenerateHellL4(
    const MapGen::Params &p,
    std::vector<int> &outGrid,
    int &outW,
    int &outH,
    uint32_t &outSeed);

} // namespace MapGenHellL4
