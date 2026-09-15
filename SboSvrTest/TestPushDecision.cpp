/// @file TestPushDecision.cpp
/// @brief 押せる物(Push=1 NPC)への押し要求判定 PushDecision の単体テスト
/// @copyright Copyright(C)URARA-works
///
/// docs/push-object-redesign.md S2。速度・軸・本人座標・接触・途中まで受理・
/// 押し合いの順序・入れ替わり(開始/専有/目標検証/横方向/終了/向き変更)を確認する。

#include "StdAfx.h"
#include "TestFramework.h"
#include "MainFrame/PushDecision.h"

using namespace PushDecision;

namespace
{
	const int OBJ_SIZE = 16;   // 押せる物(ボール等)の当たり矩形の1辺
	const int SELF_SIZE = 16;  // 本人の当たり矩形の1辺
	const int SPEED_PPS = 96;  // 想定移動速度(px/s)
	const int POS_TOLERANCE = 16;
	const int CONTACT_GAP = 4;

	RECT_PX MakeRect(int x, int y, int nSize)
	{
		RECT_PX rc;
		rc.nLeft = x;
		rc.nTop = y;
		rc.nRight = x + nSize;
		rc.nBottom = y + nSize;
		return rc;
	}

	// 何もない(常に空いている)フィールド
	bool AlwaysFree(const RECT_PX &)
	{
		return true;
	}

	// 指定した矩形と重なったら塞がっていると判定する障害物
	bool BlockedByRect(const RECT_PX &rcMoveTo, const RECT_PX &rcBlocker)
	{
		bool bOverlapX = (rcMoveTo.nLeft < rcBlocker.nRight) && (rcMoveTo.nRight > rcBlocker.nLeft);
		bool bOverlapY = (rcMoveTo.nTop < rcBlocker.nBottom) && (rcMoveTo.nBottom > rcBlocker.nTop);
		return bOverlapX && bOverlapY;
	}
}

// ---- クライアント時刻の逆行チェック ----

namespace
{
	const unsigned int RELOAD_THRESHOLD_MS = 5000;
	const unsigned int ELAPSED_MAX_MS = 300;
}

TEST(PushDecision_時刻_前回受理が無ければ上限値扱いで受理)
{
	// 前回受理(dwLastAcceptedClientTime==0)が無い最初の1回。
	CLIENT_TIME_RESULT result = CheckClientTime(1000, 0, RELOAD_THRESHOLD_MS, ELAPSED_MAX_MS);
	CHECK(!result.bRejected);
	CHECK(!result.bTreatedAsReload);
	CHECK(result.dwElapsedMs == ELAPSED_MAX_MS);
}

TEST(PushDecision_時刻_今回が0なら上限値扱いで受理)
{
	CLIENT_TIME_RESULT result = CheckClientTime(0, 500, RELOAD_THRESHOLD_MS, ELAPSED_MAX_MS);
	CHECK(!result.bRejected);
	CHECK(!result.bTreatedAsReload);
	CHECK(result.dwElapsedMs == ELAPSED_MAX_MS);
}

TEST(PushDecision_時刻_順当に進んでいれば経過時間を返す)
{
	CLIENT_TIME_RESULT result = CheckClientTime(1100, 1000, RELOAD_THRESHOLD_MS, ELAPSED_MAX_MS);
	CHECK(!result.bRejected);
	CHECK(!result.bTreatedAsReload);
	CHECK(result.dwElapsedMs == 100);
}

TEST(PushDecision_時刻_経過時間は上限でクランプされる)
{
	// 溜めて一気に押す悪用対策。1000ms経過でも上限(300ms)にクランプする。
	CLIENT_TIME_RESULT result = CheckClientTime(2000, 1000, RELOAD_THRESHOLD_MS, ELAPSED_MAX_MS);
	CHECK(!result.bRejected);
	CHECK(!result.bTreatedAsReload);
	CHECK(result.dwElapsedMs == ELAPSED_MAX_MS);
}

TEST(PushDecision_時刻_小さな逆行は却下する)
{
	// 前回受理(324553)より少しだけ小さい(重複・入れ替わり)。
	CLIENT_TIME_RESULT result = CheckClientTime(324000, 324553, RELOAD_THRESHOLD_MS, ELAPSED_MAX_MS);
	CHECK(result.bRejected);
	CHECK(!result.bTreatedAsReload);
}

TEST(PushDecision_時刻_同時刻も逆行として却下する)
{
	CLIENT_TIME_RESULT result = CheckClientTime(1000, 1000, RELOAD_THRESHOLD_MS, ELAPSED_MAX_MS);
	CHECK(result.bRejected);
	CHECK(!result.bTreatedAsReload);
}

TEST(PushDecision_時刻_大きな逆行は読み込み直しとみなし基準を取り直す)
{
	// 実機ログの再現: 今回76716、前回受理324553(閾値5000msを大きく超える逆行)。
	CLIENT_TIME_RESULT result = CheckClientTime(76716, 324553, RELOAD_THRESHOLD_MS, ELAPSED_MAX_MS);
	CHECK(!result.bRejected);
	CHECK(result.bTreatedAsReload);
	CHECK(result.dwElapsedMs == ELAPSED_MAX_MS);
}

TEST(PushDecision_時刻_逆行幅が閾値ちょうどなら読み込み直し扱い)
{
	CLIENT_TIME_RESULT result = CheckClientTime(1000, 1000 + RELOAD_THRESHOLD_MS, RELOAD_THRESHOLD_MS, ELAPSED_MAX_MS);
	CHECK(!result.bRejected);
	CHECK(result.bTreatedAsReload);
}

TEST(PushDecision_時刻_逆行幅が閾値未満なら却下のまま)
{
	CLIENT_TIME_RESULT result = CheckClientTime(1000, 1000 + RELOAD_THRESHOLD_MS - 1, RELOAD_THRESHOLD_MS, ELAPSED_MAX_MS);
	CHECK(result.bRejected);
	CHECK(!result.bTreatedAsReload);
}

// ---- 速度検証 ----

TEST(PushDecision_速度_許容内なら受理)
{
	// 100ms * 96px/s = 9.6px、許容は *1.1+8 = 18.56px。10px要求は許容内。
	CHECK(CheckSpeed(100, SPEED_PPS, 10));
}

TEST(PushDecision_速度_超過なら却下)
{
	// 許容は 18.56px。20pxは超過。
	CHECK(!CheckSpeed(100, SPEED_PPS, 20));
}

// ---- 軸検証 ----

TEST(PushDecision_軸_向きの軸のみの移動は受理)
{
	CHECK(CheckAxisOnly(DIR_RIGHT, 5, 0));
	CHECK(CheckAxisOnly(DIR_DOWN, 0, 5));
}

TEST(PushDecision_軸_向きと異なる軸を含むと却下)
{
	// 右向きなのに縦方向(dy)の移動を含む
	CHECK(!CheckAxisOnly(DIR_RIGHT, 5, 3));
	// 下向きなのに横方向(dx)の移動を含む
	CHECK(!CheckAxisOnly(DIR_DOWN, 3, 5));
}

// ---- 接触検証 ----

TEST(PushDecision_接触_離れていれば却下)
{
	// 右向きに押す=本人は物の左側。すき間5px(許容4pxを超える)。
	RECT_PX rcObj = MakeRect(100, 100, OBJ_SIZE);
	RECT_PX rcSelf = MakeRect(100 - SELF_SIZE - 5, 100, SELF_SIZE);
	CHECK(!CheckContact(rcSelf, rcObj, DIR_RIGHT, CONTACT_GAP));
}

TEST(PushDecision_接触_接していれば受理)
{
	RECT_PX rcObj = MakeRect(100, 100, OBJ_SIZE);
	RECT_PX rcSelf = MakeRect(100 - SELF_SIZE, 100, SELF_SIZE);
	CHECK(CheckContact(rcSelf, rcObj, DIR_RIGHT, CONTACT_GAP));
}

TEST(PushDecision_接触_わずかなめり込みでも受理)
{
	// 許容(4px)ちょうどのめり込み
	RECT_PX rcObj = MakeRect(100, 100, OBJ_SIZE);
	RECT_PX rcSelf = MakeRect(100 - SELF_SIZE + 4, 100, SELF_SIZE);
	CHECK(CheckContact(rcSelf, rcObj, DIR_RIGHT, CONTACT_GAP));
}

TEST(PushDecision_接触_めり込みが大きければ却下)
{
	// 許容(4px)を超える(5px)めり込み
	RECT_PX rcObj = MakeRect(100, 100, OBJ_SIZE);
	RECT_PX rcSelf = MakeRect(100 - SELF_SIZE + 5, 100, SELF_SIZE);
	CHECK(!CheckContact(rcSelf, rcObj, DIR_RIGHT, CONTACT_GAP));
}

// ---- 接触_直角の軸のすき間 ----
// 斜めに歩いて押している時、押す向きと直角の軸(横押しなら縦方向、縦押しなら
// 横方向)がぴったり重なる前に最後の押し要求が届くケースがある。この軸も
// 押す向きの軸と同じく nGapAllowed px までのすき間なら接触として受理する。
// 左右(DIR_RIGHT、直角軸=縦)と上下(DIR_UP、直角軸=横)の両方を確認する。

TEST(PushDecision_接触_直角の軸_辺がちょうど触れていれば受理_左右)
{
	RECT_PX rcObj = MakeRect(100, 100, OBJ_SIZE); // x:100-116 y:100-116
	// 主軸(x)は接触、直角軸(y)は重なり0(境界でちょうど触れている)
	RECT_PX rcSelf = MakeRect(100 - SELF_SIZE, 100 - SELF_SIZE, SELF_SIZE); // x:84-100 y:84-100
	CHECK(CheckContact(rcSelf, rcObj, DIR_RIGHT, CONTACT_GAP));
}

TEST(PushDecision_接触_直角の軸_許容内のすき間なら受理_左右)
{
	RECT_PX rcObj = MakeRect(100, 100, OBJ_SIZE); // y:100-116
	// 直角軸(y)に4px(許容ちょうど)のすき間
	RECT_PX rcSelf = MakeRect(100 - SELF_SIZE, 100 - SELF_SIZE - CONTACT_GAP, SELF_SIZE); // y:80-96
	CHECK(CheckContact(rcSelf, rcObj, DIR_RIGHT, CONTACT_GAP));
}

TEST(PushDecision_接触_直角の軸_許容を超えるすき間は却下_左右)
{
	RECT_PX rcObj = MakeRect(100, 100, OBJ_SIZE); // y:100-116
	// 直角軸(y)に5px(許容超え)のすき間
	RECT_PX rcSelf = MakeRect(100 - SELF_SIZE, 100 - SELF_SIZE - CONTACT_GAP - 1, SELF_SIZE); // y:79-95
	CHECK(!CheckContact(rcSelf, rcObj, DIR_RIGHT, CONTACT_GAP));
}

TEST(PushDecision_接触_直角の軸_辺がちょうど触れていれば受理_上下)
{
	RECT_PX rcObj = MakeRect(100, 100, OBJ_SIZE); // x:100-116 y:100-116
	// 主軸(y)は接触(本人は下側)、直角軸(x)は重なり0(境界でちょうど触れている)
	RECT_PX rcSelf = MakeRect(100 - SELF_SIZE, 100 + OBJ_SIZE, SELF_SIZE); // x:84-100 y:116-132
	CHECK(CheckContact(rcSelf, rcObj, DIR_UP, CONTACT_GAP));
}

TEST(PushDecision_接触_直角の軸_許容内のすき間なら受理_上下)
{
	RECT_PX rcObj = MakeRect(100, 100, OBJ_SIZE); // x:100-116
	// 直角軸(x)に4px(許容ちょうど)のすき間
	RECT_PX rcSelf = MakeRect(100 - SELF_SIZE - CONTACT_GAP, 100 + OBJ_SIZE, SELF_SIZE); // x:80-96
	CHECK(CheckContact(rcSelf, rcObj, DIR_UP, CONTACT_GAP));
}

TEST(PushDecision_接触_直角の軸_許容を超えるすき間は却下_上下)
{
	RECT_PX rcObj = MakeRect(100, 100, OBJ_SIZE); // x:100-116
	// 直角軸(x)に5px(許容超え)のすき間
	RECT_PX rcSelf = MakeRect(100 - SELF_SIZE - CONTACT_GAP - 1, 100 + OBJ_SIZE, SELF_SIZE); // x:79-95
	CHECK(!CheckContact(rcSelf, rcObj, DIR_UP, CONTACT_GAP));
}

// ---- 本人座標の妥当性 ----

TEST(PushDecision_本人座標_申告と実座標が近ければ受理)
{
	POINT_PX ptReported = { 100, 100 };
	POINT_PX ptServer   = { 105, 100 };
	CHECK(CheckSelfPosValid(ptReported, ptServer, POS_TOLERANCE));
}

TEST(PushDecision_本人座標_申告座標の偽装は却下)
{
	POINT_PX ptReported = { 100, 100 };
	POINT_PX ptServer   = { 300, 100 };
	CHECK(!CheckSelfPosValid(ptReported, ptServer, POS_TOLERANCE));
}

// ---- DecidePush 統合(速度・軸・座標・接触のNGはその場で却下) ----

TEST(PushDecision_DecidePush_速度超過で却下)
{
	RECT_PX rcObj = MakeRect(100, 100, OBJ_SIZE);
	RECT_PX rcSelf = MakeRect(100 - SELF_SIZE, 100, SELF_SIZE);
	POINT_PX pt = { rcSelf.nLeft, rcSelf.nTop };

	PUSH_RESULT result = DecidePush(
		rcSelf, pt, pt, POS_TOLERANCE,
		rcObj, DIR_RIGHT, /*dx=*/20, /*dy=*/0,
		/*dwElapsedMs=*/100, SPEED_PPS, CONTACT_GAP, AlwaysFree);

	CHECK(!result.bAccepted);
	CHECK(result.eReason == REASON_SPEED_OVER);
	CHECK(result.nAcceptedDistance == 0);
}

TEST(PushDecision_DecidePush_軸違反で却下)
{
	RECT_PX rcObj = MakeRect(100, 100, OBJ_SIZE);
	RECT_PX rcSelf = MakeRect(100 - SELF_SIZE, 100, SELF_SIZE);
	POINT_PX pt = { rcSelf.nLeft, rcSelf.nTop };

	PUSH_RESULT result = DecidePush(
		rcSelf, pt, pt, POS_TOLERANCE,
		rcObj, DIR_RIGHT, /*dx=*/2, /*dy=*/2,
		100, SPEED_PPS, CONTACT_GAP, AlwaysFree);

	CHECK(!result.bAccepted);
	CHECK(result.eReason == REASON_AXIS_INVALID);
}

TEST(PushDecision_DecidePush_接触していなければ却下)
{
	RECT_PX rcObj = MakeRect(100, 100, OBJ_SIZE);
	RECT_PX rcSelf = MakeRect(100 - SELF_SIZE - 10, 100, SELF_SIZE);
	POINT_PX pt = { rcSelf.nLeft, rcSelf.nTop };

	PUSH_RESULT result = DecidePush(
		rcSelf, pt, pt, POS_TOLERANCE,
		rcObj, DIR_RIGHT, /*dx=*/2, /*dy=*/0,
		100, SPEED_PPS, CONTACT_GAP, AlwaysFree);

	CHECK(!result.bAccepted);
	CHECK(result.eReason == REASON_NOT_CONTACT);
}

// ---- DecidePush 要求距離0(押すのをやめた時等)は接触していなくても受理 ----
// クライアントが押すのをやめた際等に、目標座標=サーバー上の座標(dx==dy==0)の
// REQ_PUSHを送ってくることがある。この場合は押せる物を動かさない要求として、
// 接触判定を行わず受理(移動量0)扱いにする。

TEST(PushDecision_DecidePush_要求距離0は接していなくても却下されない)
{
	// 本人は押せる物から大きく離れている(通常なら接触NGになる位置)。
	RECT_PX rcObj = MakeRect(100, 100, OBJ_SIZE);
	RECT_PX rcSelf = MakeRect(100 - SELF_SIZE - 50, 100, SELF_SIZE);
	POINT_PX pt = { rcSelf.nLeft, rcSelf.nTop };

	PUSH_RESULT result = DecidePush(
		rcSelf, pt, pt, POS_TOLERANCE,
		rcObj, DIR_RIGHT, /*dx=*/0, /*dy=*/0,
		100, SPEED_PPS, CONTACT_GAP, AlwaysFree);

	CHECK(result.bAccepted);
	CHECK(result.nAcceptedDistance == 0);
	CHECK(result.eReason == REASON_NONE);
}

TEST(PushDecision_DecidePush_要求距離0でも押せる物は動かない)
{
	RECT_PX rcObj = MakeRect(100, 100, OBJ_SIZE);
	RECT_PX rcSelf = MakeRect(100 - SELF_SIZE, 100, SELF_SIZE); // 接している場合でも
	POINT_PX pt = { rcSelf.nLeft, rcSelf.nTop };

	PUSH_RESULT result = DecidePush(
		rcSelf, pt, pt, POS_TOLERANCE,
		rcObj, DIR_RIGHT, /*dx=*/0, /*dy=*/0,
		100, SPEED_PPS, CONTACT_GAP, AlwaysFree);

	CHECK(result.bAccepted);
	CHECK(result.nAcceptedDistance == 0);
}

TEST(PushDecision_DecidePush_要求距離0でも本人座標の偽装は却下)
{
	// 安全のための本人座標チェックは要求距離0でも行う。
	RECT_PX rcObj = MakeRect(100, 100, OBJ_SIZE);
	RECT_PX rcSelf = MakeRect(100 - SELF_SIZE, 100, SELF_SIZE);
	POINT_PX ptReported = { rcSelf.nLeft, rcSelf.nTop };
	POINT_PX ptServer = { rcSelf.nLeft + 300, rcSelf.nTop }; // 大きくズレている(偽装)

	PUSH_RESULT result = DecidePush(
		rcSelf, ptReported, ptServer, POS_TOLERANCE,
		rcObj, DIR_RIGHT, /*dx=*/0, /*dy=*/0,
		100, SPEED_PPS, CONTACT_GAP, AlwaysFree);

	CHECK(!result.bAccepted);
	CHECK(result.eReason == REASON_SELF_POS_MISMATCH);
}

// ---- DecidePush 接触判定は「押せる物の目標位置」に対して行う ----
// クライアントは押せる物を先に目標位置まで動かして100ms毎に送ってくるため、
// サーバー上の現在位置には押した分めり込んでいても目標位置には接している、
// というのが実機ログで確認された通常状態。4方向それぞれで確認する。

TEST(PushDecision_DecidePush_今の位置にめり込んでいても目標位置に接していれば受理_右)
{
	// 実機ログの再現: 要求距離7pxぶん、今のOBJ位置に既にめり込んでいる。
	RECT_PX rcObj = MakeRect(100, 100, OBJ_SIZE); // 今の位置 100-116
	RECT_PX rcSelf = MakeRect(100 - SELF_SIZE + 7, 100, SELF_SIZE); // 91-107(7pxめり込み)
	POINT_PX pt = { rcSelf.nLeft, rcSelf.nTop };

	PUSH_RESULT result = DecidePush(
		rcSelf, pt, pt, POS_TOLERANCE,
		rcObj, DIR_RIGHT, /*dx=*/7, /*dy=*/0,
		100, SPEED_PPS, CONTACT_GAP, AlwaysFree);

	CHECK(result.bAccepted);
	CHECK(result.nAcceptedDistance == 7);
	CHECK(result.eReason == REASON_NONE);
}

TEST(PushDecision_DecidePush_目標位置からも離れていれば却下_右)
{
	// 今の位置には接しているが、目標位置(5px先)まではすき間が空いている。
	RECT_PX rcObj = MakeRect(100, 100, OBJ_SIZE); // 今の位置 100-116
	RECT_PX rcSelf = MakeRect(100 - SELF_SIZE, 100, SELF_SIZE); // 84-100(今の位置には接触)
	POINT_PX pt = { rcSelf.nLeft, rcSelf.nTop };

	PUSH_RESULT result = DecidePush(
		rcSelf, pt, pt, POS_TOLERANCE,
		rcObj, DIR_RIGHT, /*dx=*/5, /*dy=*/0,
		100, SPEED_PPS, CONTACT_GAP, AlwaysFree);

	CHECK(!result.bAccepted);
	CHECK(result.eReason == REASON_NOT_CONTACT);
}

TEST(PushDecision_DecidePush_目標位置へのめり込みが許容内なら受理_左)
{
	RECT_PX rcObj = MakeRect(110, 100, OBJ_SIZE); // 今の位置 110-126
	// 目標位置(左へ10px、100-116)への めり込みが4px(許容内)
	RECT_PX rcSelf = MakeRect(112, 100, SELF_SIZE); // 112-128
	POINT_PX pt = { rcSelf.nLeft, rcSelf.nTop };

	PUSH_RESULT result = DecidePush(
		rcSelf, pt, pt, POS_TOLERANCE,
		rcObj, DIR_LEFT, /*dx=*/-10, /*dy=*/0,
		100, SPEED_PPS, CONTACT_GAP, AlwaysFree);

	CHECK(result.bAccepted);
	CHECK(result.nAcceptedDistance == 10);
}

TEST(PushDecision_DecidePush_目標位置へのめり込みが許容を超えれば却下_左)
{
	RECT_PX rcObj = MakeRect(110, 100, OBJ_SIZE); // 今の位置 110-126
	// 目標位置(左へ10px、100-116)への めり込みが5px(許容超え)
	RECT_PX rcSelf = MakeRect(111, 100, SELF_SIZE); // 111-127
	POINT_PX pt = { rcSelf.nLeft, rcSelf.nTop };

	PUSH_RESULT result = DecidePush(
		rcSelf, pt, pt, POS_TOLERANCE,
		rcObj, DIR_LEFT, /*dx=*/-10, /*dy=*/0,
		100, SPEED_PPS, CONTACT_GAP, AlwaysFree);

	CHECK(!result.bAccepted);
	CHECK(result.eReason == REASON_NOT_CONTACT);
}

TEST(PushDecision_DecidePush_目標位置に接していれば受理_上)
{
	RECT_PX rcObj = MakeRect(100, 100, OBJ_SIZE); // 今の位置 x:100-116 y:100-116
	// 目標位置(上へ6px、y:94-110)へ3pxめり込み(許容内)
	RECT_PX rcSelf = MakeRect(100, 107, SELF_SIZE); // x:100-116 y:107-123
	POINT_PX pt = { rcSelf.nLeft, rcSelf.nTop };

	PUSH_RESULT result = DecidePush(
		rcSelf, pt, pt, POS_TOLERANCE,
		rcObj, DIR_UP, /*dx=*/0, /*dy=*/-6,
		100, SPEED_PPS, CONTACT_GAP, AlwaysFree);

	CHECK(result.bAccepted);
	CHECK(result.nAcceptedDistance == 6);
}

TEST(PushDecision_DecidePush_目標位置から離れていれば却下_下)
{
	RECT_PX rcObj = MakeRect(100, 100, OBJ_SIZE); // 今の位置 x:100-116 y:100-116
	// 目標位置(下へ8px、y:108-124)からすき間5px(許容超え)
	RECT_PX rcSelf = MakeRect(100, 87, SELF_SIZE); // x:100-116 y:87-103
	POINT_PX pt = { rcSelf.nLeft, rcSelf.nTop };

	PUSH_RESULT result = DecidePush(
		rcSelf, pt, pt, POS_TOLERANCE,
		rcObj, DIR_DOWN, /*dx=*/0, /*dy=*/8,
		100, SPEED_PPS, CONTACT_GAP, AlwaysFree);

	CHECK(!result.bAccepted);
	CHECK(result.eReason == REASON_NOT_CONTACT);
}

// ---- 途中まで受理 ----

TEST(PushDecision_途中まで受理_行き先に壁があれば手前で止まる)
{
	RECT_PX rcObj = MakeRect(100, 100, OBJ_SIZE); // 100-116
	// 開始位置から4px先で壁に接するように、壁の左端を120に置く
	RECT_PX rcWall = MakeRect(120, 100, OBJ_SIZE);

	IsPositionFreeFunc isFree = [&](const RECT_PX &rcMoveTo) {
		return !BlockedByRect(rcMoveTo, rcWall);
	};

	int nMoved = ResolvePushDistance(rcObj, DIR_RIGHT, /*nRequestedDistance=*/10, isFree);

	// 壁に接する4pxまでは進められるが、要求の10pxには届かず途中で止まる。
	CHECK(nMoved == 4);
}

TEST(PushDecision_途中まで受理_他キャラが行く手にいれば手前で止まる)
{
	RECT_PX rcObj = MakeRect(100, 100, OBJ_SIZE); // 100-116
	RECT_PX rcOtherChar = MakeRect(121, 100, OBJ_SIZE); // 5px先(116+5)に他キャラ

	IsPositionFreeFunc isFree = [&](const RECT_PX &rcMoveTo) {
		return !BlockedByRect(rcMoveTo, rcOtherChar);
	};

	int nMoved = ResolvePushDistance(rcObj, DIR_RIGHT, 10, isFree);
	CHECK(nMoved == 5);
}

TEST(PushDecision_途中まで受理_空いていれば全部進める)
{
	RECT_PX rcObj = MakeRect(100, 100, OBJ_SIZE);
	int nMoved = ResolvePushDistance(rcObj, DIR_RIGHT, 10, AlwaysFree);
	CHECK(nMoved == 10);
}

// ---- 押し合い(順番に処理して重ならない) ----

TEST(PushDecision_押し合い_反対側のプレイヤーが障害物になり重ならない)
{
	// ボールを挟んで左からA(右向きに押す)、右からB(ボールに接して立っている)。
	// Aの押しを処理する時、Bの当たり矩形を障害物として渡せば、
	// ボールはBの手前で止まり、AとBは重ならない。
	RECT_PX rcObj = MakeRect(100, 100, OBJ_SIZE);      // 100-116
	RECT_PX rcB   = MakeRect(120, 100, SELF_SIZE);     // Bはボールの4px右(120-136)

	IsPositionFreeFunc isFree = [&](const RECT_PX &rcMoveTo) {
		return !BlockedByRect(rcMoveTo, rcB);
	};

	// Aが右へ10px押そうとしても、ボールの右端(116)がBの左端(120)に届く
	// 4px(116→120)までしか進めない。
	int nMoved = ResolvePushDistance(rcObj, DIR_RIGHT, 10, isFree);
	CHECK(nMoved == 4);

	RECT_PX rcObjAfter = MakeRect(100 + nMoved, 100, OBJ_SIZE);
	// ボールとBが重ならないことを確認
	CHECK(!BlockedByRect(rcObjAfter, rcB));
}

// ---- 入れ替わり ----

TEST(PushDecision_入れ替わり_開始条件成立で開始できる)
{
	RECT_PX rcObj = MakeRect(100, 100, OBJ_SIZE);
	RECT_PX rcWall = MakeRect(116, 100, OBJ_SIZE); // 1px先(実質すぐ)が塞がっている

	IsPositionFreeFunc isFreeForObj = [&](const RECT_PX &rcMoveTo) {
		return !BlockedByRect(rcMoveTo, rcWall);
	};

	CHECK(CanStartSwap(/*bIsBall=*/true, rcObj, DIR_RIGHT, isFreeForObj));
}

TEST(PushDecision_入れ替わり_BALL以外は開始しない)
{
	RECT_PX rcObj = MakeRect(100, 100, OBJ_SIZE);
	RECT_PX rcWall = MakeRect(116, 100, OBJ_SIZE);

	IsPositionFreeFunc isFreeForObj = [&](const RECT_PX &rcMoveTo) {
		return !BlockedByRect(rcMoveTo, rcWall);
	};

	CHECK(!CanStartSwap(/*bIsBall=*/false, rcObj, DIR_RIGHT, isFreeForObj));
}

TEST(PushDecision_入れ替わり_1px先が空いていれば開始条件を満たさない)
{
	RECT_PX rcObj = MakeRect(100, 100, OBJ_SIZE);
	CHECK(!CanStartSwap(true, rcObj, DIR_RIGHT, AlwaysFree));
}

TEST(PushDecision_入れ替わり_後ろが塞がっていると開始しない)
{
	RECT_PX rcObj = MakeRect(100, 100, OBJ_SIZE);
	// 逆向き(左)の経路上、本人の幅(16px)以内に障害物がある
	RECT_PX rcBlockerBehind = MakeRect(90, 100, 4);

	IsPositionFreeFunc isFreeForObj = [&](const RECT_PX &rcMoveTo) {
		return !BlockedByRect(rcMoveTo, rcBlockerBehind);
	};

	CHECK(!IsSwapRetreatPathClear(rcObj, DIR_RIGHT, SELF_SIZE, isFreeForObj));
}

TEST(PushDecision_入れ替わり_後ろが空いていれば開始できる)
{
	RECT_PX rcObj = MakeRect(100, 100, OBJ_SIZE);
	CHECK(IsSwapRetreatPathClear(rcObj, DIR_RIGHT, SELF_SIZE, AlwaysFree));
}

TEST(PushDecision_入れ替わり_B0から本人移動量を引いた目標と一致すれば受理)
{
	POINT_PX ptP0 = { 84, 100 };  // 本人開始位置(ボールの左に接している)
	POINT_PX ptB0 = { 100, 100 }; // ボール開始位置
	SWAP_STATE state = StartSwap(/*dwOwnerSessionID=*/1, ptP0, ptB0, DIR_RIGHT);

	RECT_PX rcObjAtB0 = MakeRect(ptB0.x, ptB0.y, OBJ_SIZE);

	// 本人が右へ5px進んだ(P = 89,100)。ボール目標は B0-(P-P0) = (100-5, 100) = (95,100)
	POINT_PX ptSelfNow = { 89, 100 };
	POINT_PX ptExpectedObj = { 95, 100 };

	SWAP_UPDATE_RESULT result = UpdateSwap(
		state, /*dwRequesterSessionID=*/1, ptSelfNow, DIR_RIGHT,
		ptExpectedObj, rcObjAtB0, SELF_SIZE, AlwaysFree);

	CHECK(result.bAccepted);
	CHECK(result.ptAcceptedObj.x == 95);
	CHECK(result.ptAcceptedObj.y == 100);
}

TEST(PushDecision_入れ替わり_目標不一致は却下)
{
	POINT_PX ptP0 = { 84, 100 };
	POINT_PX ptB0 = { 100, 100 };
	SWAP_STATE state = StartSwap(1, ptP0, ptB0, DIR_RIGHT);
	RECT_PX rcObjAtB0 = MakeRect(ptB0.x, ptB0.y, OBJ_SIZE);

	POINT_PX ptSelfNow = { 89, 100 };
	// 正しい目標は(95,100)のはずだが、(90,100)を要求(偽装)
	POINT_PX ptBogusObj = { 90, 100 };

	SWAP_UPDATE_RESULT result = UpdateSwap(
		state, 1, ptSelfNow, DIR_RIGHT, ptBogusObj, rcObjAtB0, SELF_SIZE, AlwaysFree);

	CHECK(!result.bAccepted);
	CHECK(result.eReason == REASON_SWAP_TARGET_MISMATCH);
}

TEST(PushDecision_入れ替わり_横方向の変化は却下)
{
	POINT_PX ptP0 = { 84, 100 };
	POINT_PX ptB0 = { 100, 100 };
	SWAP_STATE state = StartSwap(1, ptP0, ptB0, DIR_RIGHT);
	RECT_PX rcObjAtB0 = MakeRect(ptB0.x, ptB0.y, OBJ_SIZE);

	POINT_PX ptSelfNow = { 89, 100 };
	// 縦方向(y)がB0からズレている
	POINT_PX ptLateralObj = { 95, 105 };

	SWAP_UPDATE_RESULT result = UpdateSwap(
		state, 1, ptSelfNow, DIR_RIGHT, ptLateralObj, rcObjAtB0, SELF_SIZE, AlwaysFree);

	CHECK(!result.bAccepted);
	CHECK(result.eReason == REASON_SWAP_LATERAL_CHANGE);
}

TEST(PushDecision_入れ替わり_合計移動量が本人の幅を超えたら却下)
{
	POINT_PX ptP0 = { 84, 100 };
	POINT_PX ptB0 = { 100, 100 };
	SWAP_STATE state = StartSwap(1, ptP0, ptB0, DIR_RIGHT);
	RECT_PX rcObjAtB0 = MakeRect(ptB0.x, ptB0.y, OBJ_SIZE);

	// 本人の幅(16px)を超えて20px進んだことにする
	POINT_PX ptSelfNow = { 104, 100 };
	POINT_PX ptExpectedObj = { 80, 100 }; // B0-(P-P0)=100-20=80(軸成分は一致させる)

	SWAP_UPDATE_RESULT result = UpdateSwap(
		state, 1, ptSelfNow, DIR_RIGHT, ptExpectedObj, rcObjAtB0, SELF_SIZE, AlwaysFree);

	CHECK(!result.bAccepted);
	CHECK(result.eReason == REASON_SWAP_TOO_FAR);
}

TEST(PushDecision_入れ替わり_専有中は他人の押しを却下)
{
	POINT_PX ptP0 = { 84, 100 };
	POINT_PX ptB0 = { 100, 100 };
	SWAP_STATE state = StartSwap(/*dwOwnerSessionID=*/1, ptP0, ptB0, DIR_RIGHT);
	RECT_PX rcObjAtB0 = MakeRect(ptB0.x, ptB0.y, OBJ_SIZE);

	POINT_PX ptSelfNow = { 89, 100 };
	POINT_PX ptExpectedObj = { 95, 100 };

	// 専有者(session=1)以外(session=2)からの要求
	SWAP_UPDATE_RESULT result = UpdateSwap(
		state, /*dwRequesterSessionID=*/2, ptSelfNow, DIR_RIGHT,
		ptExpectedObj, rcObjAtB0, SELF_SIZE, AlwaysFree);

	CHECK(!result.bAccepted);
	CHECK(result.eReason == REASON_SWAP_NOT_OWNER);
}

TEST(PushDecision_入れ替わり_合計移動量が本人の幅に達したら終了)
{
	POINT_PX ptP0 = { 84, 100 };
	POINT_PX ptB0 = { 100, 100 };
	SWAP_STATE state = StartSwap(1, ptP0, ptB0, DIR_RIGHT);
	RECT_PX rcObjAtB0 = MakeRect(ptB0.x, ptB0.y, OBJ_SIZE);

	// 本人の幅ちょうど(16px)進んだ = 本人がぶつかりきった/ボールが抜けきった
	POINT_PX ptSelfNow = { 100, 100 };
	POINT_PX ptExpectedObj = { 84, 100 };

	SWAP_UPDATE_RESULT result = UpdateSwap(
		state, 1, ptSelfNow, DIR_RIGHT, ptExpectedObj, rcObjAtB0, SELF_SIZE, AlwaysFree);

	CHECK(result.bAccepted);
	CHECK(result.bShouldEnd);
}

TEST(PushDecision_入れ替わり_本人が幅を少し超えて進んでもB0マイナス幅の目標なら受理して終了)
{
	// 本人は1フレームに数px進むため、入れ替わりが終わる要求では本人の移動量が
	// 本人の幅をわずかに超えることがある。クライアントはボール目標をB0から
	// 「本人の幅」で打ち切って送ってくるので、サーバーも同じ打ち切りで判定し、
	// 却下せず受理・終了扱いにする(REASON_SWAP_TOO_FARにはしない)。
	POINT_PX ptP0 = { 84, 100 };
	POINT_PX ptB0 = { 100, 100 };
	SWAP_STATE state = StartSwap(1, ptP0, ptB0, DIR_RIGHT);
	RECT_PX rcObjAtB0 = MakeRect(ptB0.x, ptB0.y, OBJ_SIZE);

	// 本人の幅(16px)を4px超えて20px進んだ。ボール目標はB0から幅(16px)で
	// 打ち切った84(=100-16)。
	POINT_PX ptSelfNow = { 104, 100 };
	POINT_PX ptExpectedObj = { 84, 100 };

	SWAP_UPDATE_RESULT result = UpdateSwap(
		state, 1, ptSelfNow, DIR_RIGHT, ptExpectedObj, rcObjAtB0, SELF_SIZE, AlwaysFree);

	CHECK(result.bAccepted);
	CHECK(result.bShouldEnd);
	CHECK(result.ptAcceptedObj.x == 84);
	CHECK(result.ptAcceptedObj.y == 100);
	CHECK(result.eReason == REASON_NONE);
}

TEST(PushDecision_入れ替わり_向き変更で終了)
{
	POINT_PX ptP0 = { 84, 100 };
	POINT_PX ptB0 = { 100, 100 };
	SWAP_STATE state = StartSwap(1, ptP0, ptB0, DIR_RIGHT);
	RECT_PX rcObjAtB0 = MakeRect(ptB0.x, ptB0.y, OBJ_SIZE);

	POINT_PX ptSelfNow = { 89, 100 };
	POINT_PX ptExpectedObj = { 95, 100 };

	// 本人が下向きへ向きを変えた
	SWAP_UPDATE_RESULT result = UpdateSwap(
		state, 1, ptSelfNow, DIR_DOWN, ptExpectedObj, rcObjAtB0, SELF_SIZE, AlwaysFree);

	CHECK(result.bShouldEnd);
}

// ---- 自走(eject): 入れ替わり終了(向き変更)後、専有者と重なったボールを転がす ----

TEST(PushDecision_自走_専有者と重ならなくなった所で止まる)
{
	// ボールは専有者(本人)の右側に重なっている状態から右へ自走する。
	// 専有者の矩形は x:[100,116)。ボールがx:[112,128)から右へ進み、
	// 専有者の右端116を超えて重ならなくなった時点(ボール左端>=116)で止まるはず。
	RECT_PX rcObjStart = MakeRect(112, 100, OBJ_SIZE);   // x:[112,128)
	RECT_PX rcOwner = MakeRect(100, 100, SELF_SIZE);     // x:[100,116)

	bool bSeparated = false;
	int nMoved = ResolveEjectDistance(rcObjStart, DIR_RIGHT, /*nMaxDistance=*/20, rcOwner, AlwaysFree, bSeparated);

	// 重なりが解けるのはボールが4px進んで左端が116になった時(x:[116,132)は
	// 専有者のx:[100,116)と重ならない)。
	CHECK(bSeparated);
	CHECK(nMoved == 4);
}

TEST(PushDecision_自走_塞がれば途中で止まる)
{
	RECT_PX rcObjStart = MakeRect(112, 100, OBJ_SIZE);
	RECT_PX rcOwner = MakeRect(100, 100, SELF_SIZE);
	// 2px進んだ所(ボールがx:[115,131)になる時)で障害物に塞がれ、重なりが
	// 解ける4pxに達する前に止まる。
	RECT_PX rcBlocker = MakeRect(130, 100, OBJ_SIZE);
	auto isFree = [&](const RECT_PX &rcMoveTo) -> bool {
		return !BlockedByRect(rcMoveTo, rcBlocker);
	};

	bool bSeparated = false;
	int nMoved = ResolveEjectDistance(rcObjStart, DIR_RIGHT, 20, rcOwner, isFree, bSeparated);

	CHECK(!bSeparated);
	CHECK(nMoved == 2);
}

TEST(PushDecision_自走_最初から塞がっていれば0)
{
	RECT_PX rcObjStart = MakeRect(112, 100, OBJ_SIZE);
	RECT_PX rcOwner = MakeRect(100, 100, SELF_SIZE);
	auto isBlocked = [](const RECT_PX &) -> bool { return false; };

	bool bSeparated = false;
	int nMoved = ResolveEjectDistance(rcObjStart, DIR_RIGHT, 20, rcOwner, isBlocked, bSeparated);

	CHECK(!bSeparated);
	CHECK(nMoved == 0);
}

TEST(PushDecision_自走_最大距離が0なら0)
{
	RECT_PX rcObjStart = MakeRect(112, 100, OBJ_SIZE);
	RECT_PX rcOwner = MakeRect(100, 100, SELF_SIZE);

	bool bSeparated = false;
	int nMoved = ResolveEjectDistance(rcObjStart, DIR_RIGHT, 0, rcOwner, AlwaysFree, bSeparated);

	CHECK(!bSeparated);
	CHECK(nMoved == 0);
}
