/// @file TestIpRateLimiter.cpp
/// @brief IPアドレス単位の試行回数制限のテスト
/// @copyright Copyright(C)URARA-works
///
/// register/issue-code/redeem に掛ける IP 単位の試行制限(5回失敗で300秒ロック)
/// の挙動を、時刻を注入して確認する(実時計に依存しないようにするため)。

#include "StdAfx.h"
#include "TestFramework.h"
#include "Web/IpRateLimiter.h"

TEST(試行制限_規定回数未満なら許可される)
{
	CIpRateLimiter limiter(5, 300);
	time_t now = 1000;
	int nRetryAfter = 0;

	for (int i = 0; i < 4; i ++) {
		limiter.RecordFailure("1.2.3.4", now);
	}
	CHECK(limiter.IsLockedOut("1.2.3.4", now, nRetryAfter) == false);
}

TEST(試行制限_規定回数に達するとロックされる)
{
	CIpRateLimiter limiter(5, 300);
	time_t now = 1000;
	int nRetryAfter = 0;

	for (int i = 0; i < 5; i ++) {
		limiter.RecordFailure("1.2.3.4", now);
	}
	CHECK(limiter.IsLockedOut("1.2.3.4", now, nRetryAfter) != false);
	CHECK(nRetryAfter > 0);
	CHECK(nRetryAfter <= 300);
}

TEST(試行制限_ロック時間が過ぎれば解除される)
{
	CIpRateLimiter limiter(5, 300);
	time_t now = 1000;
	int nRetryAfter = 0;

	for (int i = 0; i < 5; i ++) {
		limiter.RecordFailure("1.2.3.4", now);
	}
	CHECK(limiter.IsLockedOut("1.2.3.4", now, nRetryAfter) != false);

	// 300秒経過後は解除される
	CHECK(limiter.IsLockedOut("1.2.3.4", now + 300, nRetryAfter) == false);
}

TEST(試行制限_成功でクリアすれば即座に許可される)
{
	CIpRateLimiter limiter(5, 300);
	time_t now = 1000;
	int nRetryAfter = 0;

	for (int i = 0; i < 5; i ++) {
		limiter.RecordFailure("1.2.3.4", now);
	}
	CHECK(limiter.IsLockedOut("1.2.3.4", now, nRetryAfter) != false);

	limiter.ClearFailure("1.2.3.4");
	CHECK(limiter.IsLockedOut("1.2.3.4", now, nRetryAfter) == false);
}

TEST(試行制限_キーが異なれば独立してカウントされる)
{
	CIpRateLimiter limiter(5, 300);
	time_t now = 1000;
	int nRetryAfter = 0;

	for (int i = 0; i < 5; i ++) {
		limiter.RecordFailure("1.2.3.4", now);
	}
	CHECK(limiter.IsLockedOut("1.2.3.4", now, nRetryAfter) != false);
	CHECK(limiter.IsLockedOut("5.6.7.8", now, nRetryAfter) == false);
}

TEST(試行制限_存在しないキーはロックされていない)
{
	CIpRateLimiter limiter(5, 300);
	int nRetryAfter = 0;
	CHECK(limiter.IsLockedOut("9.9.9.9", 1000, nRetryAfter) == false);
}
