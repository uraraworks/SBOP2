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

//////////////////////////////////////////////////////////////////////
// おまかせ登録の成功回数制限(連打対策)
//
// CAccountRegisterHandler の登録成功カウンタは、この CIpRateLimiter を
// 「失敗」ではなく「登録成功」を記録する用途に転用している(10回/3600秒)。
// 挙動そのものは上の試行制限と同じクラスなので、ここでは実際に使う値
// (10回, 3600秒)での窓の扱いだけを確認する。
//////////////////////////////////////////////////////////////////////

TEST(登録成功制限_10回までは許可される)
{
	CIpRateLimiter limiter(10, 3600);
	time_t now = 1000;
	int nRetryAfter = 0;

	for (int i = 0; i < 10; i ++) {
		CHECK(limiter.IsLockedOut("1.2.3.4", now, nRetryAfter) == false);
		limiter.RecordFailure("1.2.3.4", now);
	}
}

TEST(登録成功制限_11回目は1時間ロックされる)
{
	CIpRateLimiter limiter(10, 3600);
	time_t now = 1000;
	int nRetryAfter = 0;

	for (int i = 0; i < 10; i ++) {
		limiter.RecordFailure("1.2.3.4", now);
	}
	CHECK(limiter.IsLockedOut("1.2.3.4", now, nRetryAfter) != false);
	CHECK(nRetryAfter > 0);
	CHECK(nRetryAfter <= 3600);
}

TEST(登録成功制限_1時間経過すれば再び登録できる)
{
	CIpRateLimiter limiter(10, 3600);
	time_t now = 1000;
	int nRetryAfter = 0;

	for (int i = 0; i < 10; i ++) {
		limiter.RecordFailure("1.2.3.4", now);
	}
	CHECK(limiter.IsLockedOut("1.2.3.4", now, nRetryAfter) != false);
	CHECK(limiter.IsLockedOut("1.2.3.4", now + 3600, nRetryAfter) == false);
}

TEST(登録成功制限_失敗回数制限とは別インスタンスで独立する)
{
	// register ハンドラの失敗回数制限(5回/300秒)と成功回数制限(10回/3600秒)は
	// 別インスタンスであり、片方の記録がもう片方に影響しないことを確認する。
	CIpRateLimiter failLimiter(5, 300);
	CIpRateLimiter successLimiter(10, 3600);
	time_t now = 1000;
	int nRetryAfter = 0;

	for (int i = 0; i < 5; i ++) {
		failLimiter.RecordFailure("1.2.3.4", now);
	}
	CHECK(failLimiter.IsLockedOut("1.2.3.4", now, nRetryAfter) != false);
	CHECK(successLimiter.IsLockedOut("1.2.3.4", now, nRetryAfter) == false);
}
