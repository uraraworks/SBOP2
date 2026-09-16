/// @file TestAtackTargetDecision.cpp
/// @brief 攻撃対象になり得る移動種別か判定 AtackTargetDecision の単体テスト
/// @copyright Copyright(C)URARA-works
///
/// docs/battle-redesign.md S1(サーバーへホワイトリスト実装)→S3(Commonへ移動)。
/// CInfoCharBase::IsAtackTarget()(サーバー・クライアント共用)とクライアントの
/// 敵検出(CStateProcMAP::IsEnemyChar)の両方が使う判定本体。

#include "StdAfx.h"
#include "TestFramework.h"
#include "Info/AtackTargetDecision.h"

using namespace AtackTargetDecision;

namespace
{
	// Common/Info/InfoCharBase.h の CHARMOVETYPE_* と同じ値
	const int CHARMOVETYPE_PC = 0;
	const int CHARMOVETYPE_STAND = 1;
	const int CHARMOVETYPE_BALL = 2;
	const int CHARMOVETYPE_SCORE = 3;
	const int CHARMOVETYPE_STYLECOPY_PUT = 4;
	const int CHARMOVETYPE_ATACKANIME = 5;
	const int CHARMOVETYPE_MOVEATACK = 6;
	const int CHARMOVETYPE_STYLECOPY_GET = 7;
	const int CHARMOVETYPE_PUTNPC = 8;
	const int CHARMOVETYPE_BATTLE1 = 9;
	const int CHARMOVETYPE_BATTLE2 = 10;
}

TEST(AtackTargetDecision_PCは攻撃対象になり得る)
{
	CHECK(IsAtackTargetMoveType(CHARMOVETYPE_PC));
}

TEST(AtackTargetDecision_戦闘1は攻撃対象になり得る)
{
	CHECK(IsAtackTargetMoveType(CHARMOVETYPE_BATTLE1));
}

TEST(AtackTargetDecision_戦闘2は攻撃対象になり得る)
{
	CHECK(IsAtackTargetMoveType(CHARMOVETYPE_BATTLE2));
}

TEST(AtackTargetDecision_被弾アニメーションは攻撃対象になり得る)
{
	CHECK(IsAtackTargetMoveType(CHARMOVETYPE_ATACKANIME));
}

TEST(AtackTargetDecision_移動しないSTANDは対象外)
{
	// 店員NPC等
	CHECK(!IsAtackTargetMoveType(CHARMOVETYPE_STAND));
}

TEST(AtackTargetDecision_ボールは対象外)
{
	CHECK(!IsAtackTargetMoveType(CHARMOVETYPE_BALL));
}

TEST(AtackTargetDecision_得点は対象外)
{
	CHECK(!IsAtackTargetMoveType(CHARMOVETYPE_SCORE));
}

TEST(AtackTargetDecision_容姿コピー取り込みは対象外)
{
	CHECK(!IsAtackTargetMoveType(CHARMOVETYPE_STYLECOPY_PUT));
}

TEST(AtackTargetDecision_容姿コピー反映は対象外)
{
	CHECK(!IsAtackTargetMoveType(CHARMOVETYPE_STYLECOPY_GET));
}

TEST(AtackTargetDecision_移動して攻撃する矢等は対象外)
{
	// 親付きの攻撃側であり的にはしない
	CHECK(!IsAtackTargetMoveType(CHARMOVETYPE_MOVEATACK));
}

TEST(AtackTargetDecision_NPC発生は対象外)
{
	CHECK(!IsAtackTargetMoveType(CHARMOVETYPE_PUTNPC));
}
