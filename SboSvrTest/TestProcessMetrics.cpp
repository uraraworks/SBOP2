/// @file TestProcessMetrics.cpp
/// @brief CPU使用率計算(ProcessMetrics::ComputeCpuUsagePercent)のテスト
/// @copyright Copyright(C)URARA-works
///
/// Web管理画面のサーバー情報ダッシュボード(/api/server?includeMetrics=true)の
/// cpuUsage は、以前は onlinePlayers から作った疑似値(20 + 人数*1.5 等)だった。
/// 実測値(プロセスCPU時間の差分)に差し替えるにあたり、その計算部分
/// (OS APIに依存しない純粋関数として切り出した)だけを検証する。

#include "StdAfx.h"
#include "TestFramework.h"
#include "Web/ProcessMetrics.h"
#include <cmath>

namespace
{
	bool	NearlyEqual(double a, double b)
	{
		return std::fabs(a - b) < 0.01;
	}
}

TEST(ProcessMetrics_CPU使用率を計算できる)
{
	ProcessMetrics::Sample prev;
	prev.nCpuTimeMs = 1000;
	prev.nWallTimeMs = 0;

	ProcessMetrics::Sample cur;
	cur.nCpuTimeMs = 1500;
	cur.nWallTimeMs = 1000;

	double dPercent = -1.0;
	// CPU時間 500ms / 経過 1000ms / 2コア * 100 = 25%
	CHECK(ProcessMetrics::ComputeCpuUsagePercent(prev, cur, 2, &dPercent));
	CHECK(NearlyEqual(25.0, dPercent));
}

TEST(ProcessMetrics_コア数1なら経過とCPU時間が同じで100パーセント)
{
	ProcessMetrics::Sample prev;
	prev.nCpuTimeMs = 0;
	prev.nWallTimeMs = 0;

	ProcessMetrics::Sample cur;
	cur.nCpuTimeMs = 1000;
	cur.nWallTimeMs = 1000;

	double dPercent = -1.0;
	CHECK(ProcessMetrics::ComputeCpuUsagePercent(prev, cur, 1, &dPercent));
	CHECK(NearlyEqual(100.0, dPercent));
}

TEST(ProcessMetrics_初回サンプルは経過時間0のため算出できない)
{
	ProcessMetrics::Sample prev;
	prev.nCpuTimeMs = 0;
	prev.nWallTimeMs = 1000;

	ProcessMetrics::Sample cur;
	cur.nCpuTimeMs = 0;
	cur.nWallTimeMs = 1000;

	double dPercent = -1.0;
	CHECK(!ProcessMetrics::ComputeCpuUsagePercent(prev, cur, 2, &dPercent));
}

TEST(ProcessMetrics_CPU時間が減っていたら異常値として算出できない)
{
	ProcessMetrics::Sample prev;
	prev.nCpuTimeMs = 2000;
	prev.nWallTimeMs = 0;

	ProcessMetrics::Sample cur;
	// プロセス再起動等でCPU時間が巻き戻った想定
	cur.nCpuTimeMs = 500;
	cur.nWallTimeMs = 1000;

	double dPercent = -1.0;
	CHECK(!ProcessMetrics::ComputeCpuUsagePercent(prev, cur, 2, &dPercent));
}

TEST(ProcessMetrics_コア数0は算出できない)
{
	ProcessMetrics::Sample prev;
	prev.nCpuTimeMs = 0;
	prev.nWallTimeMs = 0;

	ProcessMetrics::Sample cur;
	cur.nCpuTimeMs = 100;
	cur.nWallTimeMs = 1000;

	double dPercent = -1.0;
	CHECK(!ProcessMetrics::ComputeCpuUsagePercent(prev, cur, 0, &dPercent));
}
