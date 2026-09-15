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

unsigned int GetMotionDurationMs(const unsigned int *padwWait, int nCount)
{
	unsigned int dwSum;
	int i;

	if ((padwWait == nullptr) || (nCount <= 0)) {
		return 0;
	}

	dwSum = 0;
	for (i = 0; i < nCount; i ++) {
		dwSum += padwWait[i];
	}
	// 最終コマぶんをもう一度加える(=最終コマだけ2倍表示)
	dwSum += padwWait[nCount - 1];

	return dwSum * 10;
}

unsigned int ComputeAttackIntervalMs(
	const unsigned int *padwCandidateDurationMs,
	int nCount,
	unsigned int dwFallbackMs,
	unsigned int dwMinClampMs)
{
	unsigned int dwMin, dwInterval;
	bool bFound;
	int i;

	dwMin = 0;
	bFound = false;
	for (i = 0; i < nCount; i ++) {
		if ((padwCandidateDurationMs == nullptr) || (padwCandidateDurationMs[i] == 0)) {
			// 0は「そのモーションは未定義」として候補から除外する。
			continue;
		}
		if (!bFound || (padwCandidateDurationMs[i] < dwMin)) {
			dwMin = padwCandidateDurationMs[i];
			bFound = true;
		}
	}

	if (!bFound) {
		dwInterval = dwFallbackMs;
	} else {
		dwInterval = (dwMin * 80) / 100;
	}

	if (dwInterval < dwMinClampMs) {
		dwInterval = dwMinClampMs;
	}

	return dwInterval;
}

}
