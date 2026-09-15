/// @file TestAttackDecision.cpp
/// @brief 攻撃要求判定 AttackDecision の単体テスト
/// @copyright Copyright(C)URARA-works
///
/// docs/battle-redesign.md S1。攻撃間隔(初回・境界値・揺らぎでの2連続許容・
/// 3連続拒否・tick一周)とPvP判定の組み合わせを確認する。

#include "StdAfx.h"
#include "TestFramework.h"
#include "MainFrame/AttackDecision.h"

using namespace AttackDecision;

namespace
{
	const unsigned int MIN_INTERVAL_MS = 300;
}

// ---- 攻撃間隔 ----

TEST(AttackDecision_間隔_前回受理が無ければ初回として受理)
{
	CHECK(IsAttackIntervalOk(1000, 0, 0, MIN_INTERVAL_MS));
}

TEST(AttackDecision_間隔_最小間隔未満なら拒否)
{
	// 前回受理から299ms。前々回も無いので拒否。
	CHECK(!IsAttackIntervalOk(1299, 1000, 0, MIN_INTERVAL_MS));
}

TEST(AttackDecision_間隔_最小間隔ちょうどなら受理)
{
	CHECK(IsAttackIntervalOk(1300, 1000, 0, MIN_INTERVAL_MS));
}

TEST(AttackDecision_間隔_最小間隔を超えていれば受理)
{
	CHECK(IsAttackIntervalOk(2000, 1000, 0, MIN_INTERVAL_MS));
}

TEST(AttackDecision_間隔_揺らぎで早く届いても前々回からの平均が足りていれば受理)
{
	// 前々回(550)→前回(1000)は450ms、前回(1000)→今回(1150)は150ms(最小未満)。
	// 前々回から今回までは600ms(=2*300)なので平均は足りている。
	CHECK(IsAttackIntervalOk(1150, 1000, 550, MIN_INTERVAL_MS));
}

TEST(AttackDecision_間隔_前々回が無ければ平均判定はできず拒否)
{
	// まだ2回目の攻撃(前々回が無い)。平均判定に使える基準が無いので、
	// 最小間隔未満なら素直に拒否する(揺らぎ許容を悪用した連打対策)。
	CHECK(!IsAttackIntervalOk(1150, 1000, 0, MIN_INTERVAL_MS));
}

TEST(AttackDecision_間隔_揺らぎでも前々回からの平均が足りなければ拒否)
{
	// 前々回(700)→前回(1000)は300ms、前回(1000)→今回(1150)は150ms。
	// 前々回から今回までは450ms < 600msなので拒否(=直近2発が両方詰まっている)。
	CHECK(!IsAttackIntervalOk(1150, 1000, 700, MIN_INTERVAL_MS));
}

TEST(AttackDecision_間隔_3連続目は揺らぎ許容の対象にならず拒否)
{
	// 1回目 t=1000 受理、2回目 t=1150 が揺らぎで受理された(前々回=1000扱いに更新)
	// とすると、3回目 t=1300 は前回(1150)から150ms・前々回(1000)から300ms
	// (=2*300ではなく300)なので拒否。連続した揺らぎ悪用の防止を確認する。
	CHECK(!IsAttackIntervalOk(1300, 1150, 1000, MIN_INTERVAL_MS));
}

TEST(AttackDecision_間隔_tick一周でも正しく判定する)
{
	// dwLastAcceptedMs が unsigned int の上限付近、dwNowMs が一周後の小さい値。
	// 差分は unsigned演算で 100 になるはず(最小間隔未満なので拒否)。
	unsigned int dwLast = 0xFFFFFFF0u;	// 一周まで16ms
	unsigned int dwNow = 84u;			// 一周後84ms経過 => 合計100ms
	CHECK(!IsAttackIntervalOk(dwNow, dwLast, 0, MIN_INTERVAL_MS));
}

TEST(AttackDecision_間隔_tick一周後に十分な間隔が空けば受理)
{
	unsigned int dwLast = 0xFFFFFFF0u;	// 一周まで16ms
	unsigned int dwNow = 400u;			// 一周後400ms経過 => 合計416ms(300ms以上)
	CHECK(IsAttackIntervalOk(dwNow, dwLast, 0, MIN_INTERVAL_MS));
}

// ---- PvP判定 ----

TEST(AttackDecision_PvP_PC同士でPvP不可マップなら拒否)
{
	CHECK(IsPvpAttackBlocked(/*bAttackerIsPC=*/true, /*bTargetIsPC=*/true, /*bMapAllowsPvp=*/false));
}

TEST(AttackDecision_PvP_PC同士でもPvP可マップなら許可)
{
	CHECK(!IsPvpAttackBlocked(/*bAttackerIsPC=*/true, /*bTargetIsPC=*/true, /*bMapAllowsPvp=*/true));
}

TEST(AttackDecision_PvP_攻撃者がNPCなら許可)
{
	// 敵NPCがPCを攻撃する通常のケース。
	CHECK(!IsPvpAttackBlocked(/*bAttackerIsPC=*/false, /*bTargetIsPC=*/true, /*bMapAllowsPvp=*/false));
}

TEST(AttackDecision_PvP_対象がNPCなら許可)
{
	// PCが敵NPCを攻撃する通常のケース。
	CHECK(!IsPvpAttackBlocked(/*bAttackerIsPC=*/true, /*bTargetIsPC=*/false, /*bMapAllowsPvp=*/false));
}

TEST(AttackDecision_PvP_NPC同士なら許可)
{
	CHECK(!IsPvpAttackBlocked(/*bAttackerIsPC=*/false, /*bTargetIsPC=*/false, /*bMapAllowsPvp=*/false));
}

// ---- モーション全体時間 ----

TEST(AttackDecision_モーション時間_全コマの合計に最終コマ分を追加して10倍する)
{
	// 合計(10+10+5)=25、最終コマ5を追加して30 → ×10=300ms
	unsigned int adwWait[] = {10, 10, 5};
	CHECK_EQ(300u, GetMotionDurationMs(adwWait, 3));
}

TEST(AttackDecision_モーション時間_コマ配列が空なら0)
{
	CHECK_EQ(0u, GetMotionDurationMs(NULL, 0));
}

TEST(AttackDecision_モーション時間_1コマなら合計と最終コマが同じ値を2回加える)
{
	// 20+20=40 → ×10=400ms(素手のイメージ)
	unsigned int adwWait[] = {20};
	CHECK_EQ(400u, GetMotionDurationMs(adwWait, 1));
}

// ---- 攻撃間隔の下限計算 ----

TEST(AttackDecision_間隔計算_候補の最短値の80パーセントになる)
{
	unsigned int adwCandidates[] = {350, 250, 400};	// 最短250 → 80%=200
	CHECK_EQ(200u, ComputeAttackIntervalMs(adwCandidates, 3, 200, 100));
}

TEST(AttackDecision_間隔計算_0の候補は未定義として無視する)
{
	unsigned int adwCandidates[] = {0, 0, 500};	// 有効な候補は500だけ → 80%=400
	CHECK_EQ(400u, ComputeAttackIntervalMs(adwCandidates, 3, 200, 100));
}

TEST(AttackDecision_間隔計算_候補が全部無ければフォールバック)
{
	unsigned int adwCandidates[] = {0, 0};
	CHECK_EQ(200u, ComputeAttackIntervalMs(adwCandidates, 2, 200, 100));
}

TEST(AttackDecision_間隔計算_候補配列が無くてもフォールバック)
{
	CHECK_EQ(200u, ComputeAttackIntervalMs(NULL, 0, 200, 100));
}

TEST(AttackDecision_間隔計算_下限クランプが効く)
{
	unsigned int adwCandidates[] = {100};	// 80%=80 → 下限100に持ち上げ
	CHECK_EQ(100u, ComputeAttackIntervalMs(adwCandidates, 1, 200, 100));
}

TEST(AttackDecision_間隔計算_フォールバック自体が下限未満なら下限にクランプ)
{
	unsigned int adwCandidates[] = {0};
	CHECK_EQ(100u, ComputeAttackIntervalMs(adwCandidates, 1, 50, 100));
}
