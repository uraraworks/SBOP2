/// @file TestMoveStateDecision.cpp
/// @brief MOVE_START/MOVE_DIR_CHANGE/MOVE_STOP 受信時の MoveState 遷移先決定のテスト
/// @copyright Copyright(C)URARA-works
///
/// 座り中(CHARMOVESTATE_SIT)に方向キーだけ押すと立ち上がってしまう回帰
/// (Dead Reckoning 移行時、位置が変わっていなくても無条件で「移動中」へ
/// 遷移させていたのが原因)を防ぐための、MoveStateDecision::Decide() の単体テスト。
/// 実際の CHARMOVESTATE_*/SBOCOMMANDID_SUB_CHAR_MOVE_* の値には依存しないよう、
/// テスト側では区別できれば良い適当な定数を使う。

#include "StdAfx.h"
#include "TestFramework.h"
#include "MainFrame/MoveStateDecision.h"

namespace
{
	const int STATE_SIT = 1;	// 現在の状態(座り中)。Decide()には渡らない値
	const int STATE_MOVE = 2;
	const int STATE_BATTLEMOVE = 3;
	const int STATE_STAND = 0;
	const int STATE_BATTLE = 5;
}

TEST(MoveStateDecision_MOVE_STARTで位置が変わっていないなら状態を変更しない)
{
	// 座り中に方向キーだけ押した場合を想定: 位置は変わらない
	int nResult = MoveStateDecision::Decide(
		/*bIsStopCmd=*/false,
		/*bIsCurrentlyMoving=*/false,
		/*bPosChanged=*/false,
		/*nMoveStateOnMove=*/STATE_MOVE,
		/*nStopState=*/STATE_STAND);

	CHECK(nResult == MoveStateDecision::NO_CHANGE);
}

TEST(MoveStateDecision_MOVE_DIR_CHANGEで位置が変わっていないなら状態を変更しない)
{
	int nResult = MoveStateDecision::Decide(
		/*bIsStopCmd=*/false,
		/*bIsCurrentlyMoving=*/true,
		/*bPosChanged=*/false,
		/*nMoveStateOnMove=*/STATE_BATTLEMOVE,
		/*nStopState=*/STATE_BATTLE);

	CHECK(nResult == MoveStateDecision::NO_CHANGE);
}

TEST(MoveStateDecision_MOVE_STARTで位置が変わったなら移動状態へ遷移する)
{
	int nResult = MoveStateDecision::Decide(
		/*bIsStopCmd=*/false,
		/*bIsCurrentlyMoving=*/false,
		/*bPosChanged=*/true,
		/*nMoveStateOnMove=*/STATE_MOVE,
		/*nStopState=*/STATE_STAND);

	CHECK(nResult == STATE_MOVE);
}

TEST(MoveStateDecision_戦闘中に位置が変わったなら戦闘移動状態へ遷移する)
{
	int nResult = MoveStateDecision::Decide(
		/*bIsStopCmd=*/false,
		/*bIsCurrentlyMoving=*/true,
		/*bPosChanged=*/true,
		/*nMoveStateOnMove=*/STATE_BATTLEMOVE,
		/*nStopState=*/STATE_BATTLE);

	CHECK(nResult == STATE_BATTLEMOVE);
}

TEST(MoveStateDecision_移動中のMOVE_STOPは位置が変わっていなくても停止状態を確定する)
{
	// 通常移動の停止(壁にぶつかって位置が変わらず止まった)だけでなく、
	// 従来の挙動(無条件に nStopState を返す)を維持することを確認する。
	int nResult = MoveStateDecision::Decide(
		/*bIsStopCmd=*/true,
		/*bIsCurrentlyMoving=*/true,
		/*bPosChanged=*/false,
		/*nMoveStateOnMove=*/STATE_MOVE,
		/*nStopState=*/STATE_STAND);

	CHECK(nResult == STATE_STAND);
}

TEST(MoveStateDecision_移動中のMOVE_STOPは位置が変わっていれば停止状態を確定する)
{
	int nResult = MoveStateDecision::Decide(
		/*bIsStopCmd=*/true,
		/*bIsCurrentlyMoving=*/true,
		/*bPosChanged=*/true,
		/*nMoveStateOnMove=*/STATE_MOVE,
		/*nStopState=*/STATE_BATTLE);

	CHECK(nResult == STATE_BATTLE);
}

TEST(MoveStateDecision_戦闘移動中のMOVE_STOPは戦闘待機状態を確定する)
{
	int nResult = MoveStateDecision::Decide(
		/*bIsStopCmd=*/true,
		/*bIsCurrentlyMoving=*/true,
		/*bPosChanged=*/true,
		/*nMoveStateOnMove=*/STATE_BATTLEMOVE,
		/*nStopState=*/STATE_BATTLE);

	CHECK(nResult == STATE_BATTLE);
}

TEST(MoveStateDecision_座り中に向きだけ変えた後のMOVE_STOPで立たされない)
{
	// 座り中(SIT)に方向キーを押すと MOVE_START/DIR_CHANGE では既に修正済みだが、
	// それに続けてクライアントが送ってくる MOVE_STOP でも、実際には
	// 移動中(IsStateMove())でなかったので状態を変えてはいけない。
	int nResult = MoveStateDecision::Decide(
		/*bIsStopCmd=*/true,
		/*bIsCurrentlyMoving=*/false,
		/*bPosChanged=*/false,
		/*nMoveStateOnMove=*/STATE_MOVE,
		/*nStopState=*/STATE_STAND);

	CHECK(nResult == MoveStateDecision::NO_CHANGE);
}

TEST(MoveStateDecision_防御中のMOVE_STOPで状態を変えない)
{
	// 防御中(BATTLE_DEFENSE)は IsStateMove()==FALSE。向きだけ変えて
	// 届いた MOVE_STOP で BATTLE/BATTLE_DEFENSE 以外へ崩されないこと。
	int nResult = MoveStateDecision::Decide(
		/*bIsStopCmd=*/true,
		/*bIsCurrentlyMoving=*/false,
		/*bPosChanged=*/false,
		/*nMoveStateOnMove=*/STATE_BATTLEMOVE,
		/*nStopState=*/STATE_BATTLE);

	CHECK(nResult == MoveStateDecision::NO_CHANGE);
}
