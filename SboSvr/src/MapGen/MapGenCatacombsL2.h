#pragma once

#include <vector>
#include <cstdint>
#include "MapGenTypes.h"

// ---------------------------------------------------------------------------
// MapGenCatacombsL2 — Diablo 1 L2 Catacombs 方式の地下墓地マップ自動生成エンジン
//
// 参照: docs/dungeon-generation-diablo1-reference.md "L2 Catacombs" セクション
// 生成フロー:
//   1. CreateRoom() で全体領域を再帰4分割し、各分割にランダムサイズ4〜10の部屋を配置
//   2. 親部屋との廊下候補を HallList に蓄積
//   3. HallList を消費して ConnectHall()（ステアリングアルゴ）で廊下を引く
//   4. 床面積 >= floorAreaMin かつ全床連結を満たすまで再生成
//   5. 安全上限(maxRetries) 超過で失敗を返す
// ---------------------------------------------------------------------------

namespace MapGenCatacombsL2
{

bool GenerateCatacombsL2(
    const MapGen::Params &p,
    std::vector<int> &outGrid,
    int &outW,
    int &outH,
    uint32_t &outSeed);

} // namespace MapGenCatacombsL2
