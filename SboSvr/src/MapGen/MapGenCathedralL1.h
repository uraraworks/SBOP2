#pragma once

#include <vector>
#include <cstdint>
#include "MapGenTypes.h"

// ---------------------------------------------------------------------------
// MapGenCathedralL1 — Diablo 1 L1 Cathedral 方式の聖堂マップ自動生成エンジン
//
// 参照: docs/dungeon-generation-diablo1-reference.md "L1 Cathedral" セクション
// 生成フロー:
//   1. 背骨に最大3個の 10×10 チェンバーを配置し廊下で接続
//   2. 各チェンバーの辺から GenerateRoom() で再帰的に部屋を出芽
//      - 部屋サイズは blockMin〜blockMax の偶数化
//      - CheckRoom() を通過した場合のみ配置。失敗で自然終了
//   3. 床面積 >= floorAreaMin かつ全床連結を満たすまで再生成
//   4. 安全上限(maxRetries) 超過で失敗を返す
// ---------------------------------------------------------------------------

namespace MapGenCathedralL1
{

bool GenerateCathedralL1(
    const MapGen::Params &p,
    std::vector<int> &outGrid,
    int &outW,
    int &outH,
    uint32_t &outSeed);

} // namespace MapGenCathedralL1
