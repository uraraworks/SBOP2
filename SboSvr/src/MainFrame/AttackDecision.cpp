/// @file AttackDecision.cpp
/// @brief 攻撃要求の可否判定(純粋関数) 実装ファイル
/// @copyright Copyright(C)URARA-works

#include "AttackDecision.h"

namespace AttackDecision
{

bool IsAttackIntervalOk(
	unsigned int dwNowMs,
	unsigned int dwLastAcceptedMs,
	unsigned int dwPrevAcceptedMs,
	unsigned int dwMinIntervalMs)
{
	unsigned int dwSinceLast, dwSincePrev;

	if (dwLastAcceptedMs == 0) {
		// 前回受理が無い(初回)。基準が無いので受理する。
		return true;
	}

	dwSinceLast = dwNowMs - dwLastAcceptedMs;	// unsigned差分。tick一周でも正しく計算される
	if (dwSinceLast >= dwMinIntervalMs) {
		return true;
	}

	if (dwPrevAcceptedMs == 0) {
		// 前々回が無く、前回からの間隔だけでは足りない。
		return false;
	}

	dwSincePrev = dwNowMs - dwPrevAcceptedMs;
	if (dwSincePrev >= (dwMinIntervalMs * 2)) {
		// 直近2間隔の平均が最小間隔以上。通信の揺らぎで2発続けて届いた分を許容する。
		return true;
	}

	return false;
}

bool IsPvpAttackBlocked(bool bAttackerIsPC, bool bTargetIsPC, bool bMapAllowsPvp)
{
	if (bMapAllowsPvp) {
		return false;
	}
	return bAttackerIsPC && bTargetIsPC;
}

}
