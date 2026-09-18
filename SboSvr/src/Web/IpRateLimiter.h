/// @file IpRateLimiter.h
/// @brief IPアドレス単位の試行回数制限 定義ファイル
/// @details AuthSessionHandler の管理画面ログイン(5回失敗で300秒ロック)と
///          同じ仕組みを、任意のキー(ここではクライアントIP)で再利用できる
///          ようにクラス化したもの。時刻をテストから注入できるよう、
///          全メソッドで time_t を引数に取る(内部で time(NULL) は呼ばない)。

#pragma once

#include <string>
#include <map>
#include <mutex>
#include <ctime>

class CIpRateLimiter
{
public:
	/// @param nMaxFailCount ロックに入るまでの連続失敗回数
	/// @param lLockoutSeconds ロック継続時間(秒)
	/// @param nMaxEntries 保持するキーの上限(超えたら最古のものを捨てる)
	CIpRateLimiter(int nMaxFailCount, long lLockoutSeconds, size_t nMaxEntries = 512);

	/// @brief ロック中かどうかを判定する。ロック中なら残り秒数を返す。
	bool	IsLockedOut(const std::string &strKey, time_t now, int &outRetryAfterSeconds);

	/// @brief 失敗を1回記録する。
	void	RecordFailure(const std::string &strKey, time_t now);

	/// @brief 成功時に失敗履歴をクリアする。
	void	ClearFailure(const std::string &strKey);

private:
	struct FailInfo
	{
		int		nFailCount;
		time_t	timeLastFail;
	};

	int		m_nMaxFailCount;
	long	m_lLockoutSeconds;
	size_t	m_nMaxEntries;

	std::map<std::string, FailInfo>	m_failMap;
	std::mutex							m_mutex;
};
