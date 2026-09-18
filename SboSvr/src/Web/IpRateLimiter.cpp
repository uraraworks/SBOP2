/// @file IpRateLimiter.cpp
/// @brief IPアドレス単位の試行回数制限 実装ファイル

#include "StdAfx.h"
#include "IpRateLimiter.h"

CIpRateLimiter::CIpRateLimiter(int nMaxFailCount, long lLockoutSeconds, size_t nMaxEntries)
	: m_nMaxFailCount(nMaxFailCount)
	, m_lLockoutSeconds(lLockoutSeconds)
	, m_nMaxEntries(nMaxEntries)
{
}

bool CIpRateLimiter::IsLockedOut(const std::string &strKey, time_t now, int &outRetryAfterSeconds)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	std::map<std::string, FailInfo>::iterator it = m_failMap.find(strKey);
	if (it == m_failMap.end()) {
		return false;
	}

	if (it->second.nFailCount < m_nMaxFailCount) {
		return false;
	}

	time_t elapsed = now - it->second.timeLastFail;
	if (elapsed >= m_lLockoutSeconds) {
		return false;
	}

	outRetryAfterSeconds = (int)(m_lLockoutSeconds - elapsed);
	return true;
}

void CIpRateLimiter::RecordFailure(const std::string &strKey, time_t now)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	if ((m_failMap.size() >= m_nMaxEntries) && (m_failMap.find(strKey) == m_failMap.end())) {
		std::map<std::string, FailInfo>::iterator oldestIt = m_failMap.end();
		for (std::map<std::string, FailInfo>::iterator it = m_failMap.begin(); it != m_failMap.end(); ++it) {
			if ((oldestIt == m_failMap.end()) || (it->second.timeLastFail < oldestIt->second.timeLastFail)) {
				oldestIt = it;
			}
		}
		if (oldestIt != m_failMap.end()) {
			m_failMap.erase(oldestIt);
		}
	}

	FailInfo &info = m_failMap[strKey];
	info.nFailCount++;
	info.timeLastFail = now;
}

void CIpRateLimiter::ClearFailure(const std::string &strKey)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_failMap.erase(strKey);
}
