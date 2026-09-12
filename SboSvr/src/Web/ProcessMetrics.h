/// @file ProcessMetrics.h
/// @brief CPU使用率計算(サンプル間の差分から%を出す純粋関数) 定義ファイル
/// @copyright Copyright(C)URARA-works
///
/// プロセスのCPU時間・メモリ量の「取得」は Platform/SvrPlatform.h
/// (SboPlatform::GetProcessMetrics 等)へ寄せてある。ここにあるのは
/// 「2回分のサンプルから%を計算する」部分だけを切り出した純粋関数で、
/// OS API に依存しないためテスト(SboSvrTest)から直接呼べる。

#pragma once

namespace ProcessMetrics
{
	/// CPU使用率計算用の1サンプル
	struct Sample
	{
		unsigned long long	nCpuTimeMs;	// プロセス開始からの累積CPU時間(ms、ユーザー+カーネル)
		long long	nWallTimeMs;	// サンプル取得時刻(任意の単調増加時計、ms)
	};

	/// 2つのサンプルからCPU使用率(%)を計算する
	///
	/// (CPU時間の差分) ÷ (経過時間の差分) ÷ 論理CPU数 × 100。
	/// 初回呼び出し(経過時間が0以下)や、プロセス再起動等でCPU時間が
	/// 減っている異常値の場合は算出できないので false を返す。
	///
	/// @return 算出できれば true。false の場合 *pOutPercent は変更しない。
	bool	ComputeCpuUsagePercent(const Sample &prev, const Sample &cur,
			unsigned int nCpuCount, double *pOutPercent);
}
