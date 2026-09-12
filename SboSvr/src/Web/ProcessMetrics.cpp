/// @file ProcessMetrics.cpp
/// @brief CPU使用率計算(サンプル間の差分から%を出す純粋関数) 実装ファイル
/// @copyright Copyright(C)URARA-works

#include "ProcessMetrics.h"

namespace ProcessMetrics
{
	bool	ComputeCpuUsagePercent(const Sample &prev, const Sample &cur,
			unsigned int nCpuCount, double *pOutPercent)
	{
		if (pOutPercent == nullptr) {
			return false;
		}
		if (nCpuCount == 0) {
			return false;
		}

		long long nWallDeltaMs = cur.nWallTimeMs - prev.nWallTimeMs;
		if (nWallDeltaMs <= 0) {
			// 初回サンプル、または時計が巻き戻った(異常値)
			return false;
		}
		if (cur.nCpuTimeMs < prev.nCpuTimeMs) {
			// プロセス再起動等でCPU時間が減っている異常値
			return false;
		}

		unsigned long long nCpuDeltaMs = cur.nCpuTimeMs - prev.nCpuTimeMs;
		double dPercent = (double)nCpuDeltaMs / (double)nWallDeltaMs / (double)nCpuCount * 100.0;

		if (dPercent < 0.0) {
			dPercent = 0.0;
		}
		// 理論上は100%×コア数を超えないはずだが、計測誤差の安全側クランプ
		double dMax = 100.0 * (double)nCpuCount;
		if (dPercent > dMax) {
			dPercent = dMax;
		}

		*pOutPercent = dPercent;
		return true;
	}
}
