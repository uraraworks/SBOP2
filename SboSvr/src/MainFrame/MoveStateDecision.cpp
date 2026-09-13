/// @file MoveStateDecision.cpp
/// @brief MOVE_START/MOVE_DIR_CHANGE/MOVE_STOP 受信時の MoveState 遷移先決定(純粋関数) 実装ファイル
/// @copyright Copyright(C)URARA-works

#include "MoveStateDecision.h"

namespace MoveStateDecision
{

int Decide(bool bIsStopCmd, bool bIsCurrentlyMoving, bool bPosChanged, int nMoveStateOnMove, int nStopState)
{
	if (bIsStopCmd) {
		if (!bIsCurrentlyMoving) {
			// 実際には移動中でなかった(座り中に向きだけ変えた後に届いた
			// MOVE_STOP 等)。座り中/防御中などの現在の状態を崩さないよう
			// MoveState には触れない。
			return NO_CHANGE;
		}
		// 移動中からの停止通知は最終位置を確定同期させるため、
		// 位置が変わっていなくても現在の(戦闘/通常)待機状態へ確定させる
		// (従来通りの挙動)。
		return nStopState;
	}

	if (!bPosChanged) {
		// MOVE_START / MOVE_DIR_CHANGE で位置が変わっていない＝向きだけの変更。
		// 座り中などの現在の状態を崩さないよう、MoveState には触れない。
		return NO_CHANGE;
	}

	return nMoveStateOnMove;
}

}
