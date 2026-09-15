/// @file PushDecision.cpp
/// @brief 押せる物(Push=1 NPC)への押し要求の可否判定(純粋関数) 実装ファイル
/// @copyright Copyright(C)URARA-works

#include "PushDecision.h"

#include <cmath>
#include <cstdlib>

namespace PushDecision
{

namespace
{
	int Abs(int n) { return (n < 0) ? -n : n; }

	/// 向きの単位ベクトル(x,y)
	void DirVector(int nDir, int &dx, int &dy)
	{
		dx = 0;
		dy = 0;
		switch (nDir) {
		case DIR_UP:    dy = -1; break;
		case DIR_DOWN:  dy =  1; break;
		case DIR_LEFT:  dx = -1; break;
		case DIR_RIGHT: dx =  1; break;
		}
	}

	int OppositeDir(int nDir)
	{
		switch (nDir) {
		case DIR_UP:    return DIR_DOWN;
		case DIR_DOWN:  return DIR_UP;
		case DIR_LEFT:  return DIR_RIGHT;
		case DIR_RIGHT: return DIR_LEFT;
		}
		return nDir;
	}

	RECT_PX Offset(const RECT_PX &rc, int dx, int dy)
	{
		RECT_PX rcResult = rc;
		rcResult.nLeft   += dx;
		rcResult.nRight  += dx;
		rcResult.nTop    += dy;
		rcResult.nBottom += dy;
		return rcResult;
	}

	/// 区間 [a1,a2) と [b1,b2) のすき間。重なっていれば0、離れていればその距離
	/// (負にはならない)を返す。CheckContact の直角軸判定で使う。
	int IntervalGap(int a1, int a2, int b1, int b2)
	{
		if (a2 <= b1) return b1 - a2;
		if (b2 <= a1) return a1 - b2;
		return 0;
	}

	/// 矩形 a,b が重なっているか(隣接するだけ、すき間0は重なりに含めない)。
	/// ResolveEjectDistance の「専有者と重ならなくなった」判定に使う。
	bool RectsOverlap(const RECT_PX &a, const RECT_PX &b)
	{
		return (a.nLeft < b.nRight) && (b.nLeft < a.nRight) &&
			(a.nTop < b.nBottom) && (b.nTop < a.nBottom);
	}
}

CLIENT_TIME_RESULT CheckClientTime(
	unsigned int dwTimeStamp,
	unsigned int dwLastAcceptedClientTime,
	unsigned int dwReloadThresholdMs,
	unsigned int dwElapsedMaxMs)
{
	CLIENT_TIME_RESULT result;
	result.bRejected = false;
	result.bTreatedAsReload = false;
	result.dwElapsedMs = dwElapsedMaxMs;

	if ((dwTimeStamp == 0) || (dwLastAcceptedClientTime == 0)) {
		// 比較対象が無い(初回等)。上限値扱いのまま受理側の処理へ進める。
		return result;
	}

	if (dwTimeStamp <= dwLastAcceptedClientTime) {
		unsigned int dwRewind = dwLastAcceptedClientTime - dwTimeStamp;
		if (dwRewind >= dwReloadThresholdMs) {
			// 大きな逆行=クライアント再起動(ページ再読み込み等)とみなし、
			// 却下せず基準を取り直す。
			result.bTreatedAsReload = true;
			return result;
		}
		// 小さな逆行(パケットの入れ替わり・重複)は従来どおり却下する。
		result.bRejected = true;
		return result;
	}

	unsigned int dwElapsed = dwTimeStamp - dwLastAcceptedClientTime;
	if (dwElapsed > dwElapsedMaxMs) {
		dwElapsed = dwElapsedMaxMs;
	}
	result.dwElapsedMs = dwElapsed;
	return result;
}

bool CheckSpeed(unsigned int dwElapsedMs, int nPixelsPerSec, int nRequestedDistance)
{
	// 許容距離 = 経過ms/1000 * px/s * 1.1 + 8px
	double dAllowed = (static_cast<double>(dwElapsedMs) / 1000.0) * nPixelsPerSec * 1.1 + 8.0;
	return static_cast<double>(Abs(nRequestedDistance)) <= dAllowed;
}

bool CheckAxisOnly(int nDir, int dx, int dy)
{
	int nAxisX, nAxisY;
	DirVector(nDir, nAxisX, nAxisY);

	if (nAxisX == 0) {
		// 縦方向の向き: 横移動(dx)を含んではいけない
		if (dx != 0) {
			return false;
		}
	} else {
		// 横方向の向き: 縦移動(dy)を含んではいけない
		if (dy != 0) {
			return false;
		}
	}

	// 向きと逆方向への移動(バック)も軸違反として扱う
	if (nAxisX != 0 && ((nAxisX > 0) != (dx >= 0)) && dx != 0) {
		return false;
	}
	if (nAxisY != 0 && ((nAxisY > 0) != (dy >= 0)) && dy != 0) {
		return false;
	}

	return true;
}

bool CheckSelfPosValid(const POINT_PX &ptReportedSelf, const POINT_PX &ptServerSelf, int nTolerance)
{
	int dx = Abs(ptReportedSelf.x - ptServerSelf.x);
	int dy = Abs(ptReportedSelf.y - ptServerSelf.y);
	return (dx <= nTolerance) && (dy <= nTolerance);
}

bool CheckContact(const RECT_PX &rcSelf, const RECT_PX &rcObj, int nDir, int nGapAllowed)
{
	// 押す向き d と反対側の面で接しているかを見る。
	// 例: 上(UP)へ押す場合、本人は押せる物の下側に居るはずなので、
	// 本人矩形の上端(nTop)と押せる物矩形の下端(nBottom)の差を見る。
	// nGap は「離れていれば正、めり込んでいれば負」。
	// -nGapAllowed <= nGap <= nGapAllowed の範囲なら接触扱いにする
	// (わずかなめり込みも、わずかな隙間も許容する)。
	// これは呼び出し側が rcObj に「押せる物の目標位置(今の位置 + 要求移動量)」の
	// 矩形を渡す前提。クライアントは押せる物を先に目標位置まで動かして送ってくるため、
	// 本人はサーバー上の現在位置にはめり込んでいても目標位置には接している、という
	// ケースを正しく受理するためにこうしている。
	//
	// 押す向きと直角の軸(横向きに押す時の縦方向、縦向きに押す時の横方向)も、
	// 「重なっている」ことまでは要求せず、押す向きの軸と同じく nGapAllowed px
	// までのすき間なら接触扱いにする。斜めに歩いて押した際、直角の軸がぴったり
	// 重なる前(すき間1px程度)に最後の押し要求が届き、ここで却下されて押せる物が
	// 少し押し戻されたように見える回帰が実機ログで確認されたため。
	int nGap = 0;
	int nPerpGap = 0;

	switch (nDir) {
	case DIR_UP:
		nGap = rcSelf.nTop - rcObj.nBottom;
		if (nGap < -nGapAllowed) return false;
		nPerpGap = IntervalGap(rcSelf.nLeft, rcSelf.nRight, rcObj.nLeft, rcObj.nRight);
		if (nPerpGap > nGapAllowed) return false;
		return nGap <= nGapAllowed;
	case DIR_DOWN:
		nGap = rcObj.nTop - rcSelf.nBottom;
		if (nGap < -nGapAllowed) return false;
		nPerpGap = IntervalGap(rcSelf.nLeft, rcSelf.nRight, rcObj.nLeft, rcObj.nRight);
		if (nPerpGap > nGapAllowed) return false;
		return nGap <= nGapAllowed;
	case DIR_LEFT:
		nGap = rcSelf.nLeft - rcObj.nRight;
		if (nGap < -nGapAllowed) return false;
		nPerpGap = IntervalGap(rcSelf.nTop, rcSelf.nBottom, rcObj.nTop, rcObj.nBottom);
		if (nPerpGap > nGapAllowed) return false;
		return nGap <= nGapAllowed;
	case DIR_RIGHT:
		nGap = rcObj.nLeft - rcSelf.nRight;
		if (nGap < -nGapAllowed) return false;
		nPerpGap = IntervalGap(rcSelf.nTop, rcSelf.nBottom, rcObj.nTop, rcObj.nBottom);
		if (nPerpGap > nGapAllowed) return false;
		return nGap <= nGapAllowed;
	}
	return false;
}

int ResolvePushDistance(const RECT_PX &rcObjStart, int nDir, int nRequestedDistance, const IsPositionFreeFunc &isFree)
{
	if (nRequestedDistance <= 0) {
		return 0;
	}

	int dx, dy;
	DirVector(nDir, dx, dy);

	RECT_PX rcCur = rcObjStart;
	int nMoved = 0;

	for (; nMoved < nRequestedDistance; ++nMoved) {
		RECT_PX rcNext = Offset(rcCur, dx, dy);
		if (!isFree || !isFree(rcNext)) {
			break; // ここで塞がっている。途中まで受理。
		}
		rcCur = rcNext;
	}

	return nMoved;
}

PUSH_RESULT DecidePush(
	const RECT_PX &rcSelf,
	const POINT_PX &ptReportedSelf,
	const POINT_PX &ptServerSelf,
	int nSelfPosTolerance,
	const RECT_PX &rcObj,
	int nDir,
	int dx, int dy,
	unsigned int dwElapsedMs,
	int nPixelsPerSec,
	int nContactGapAllowed,
	const IsPositionFreeFunc &isFree)
{
	PUSH_RESULT result;
	result.bAccepted = false;
	result.nAcceptedDistance = 0;
	result.eReason = REASON_NONE;

	int nRequestedDistance = Abs(dx) + Abs(dy);

	// 本人座標の偽装チェックは、要求距離が0の時も安全のため必ず行う。
	if (!CheckSelfPosValid(ptReportedSelf, ptServerSelf, nSelfPosTolerance)) {
		result.eReason = REASON_SELF_POS_MISMATCH;
		return result;
	}

	if (nRequestedDistance == 0) {
		// 要求距離0(dx==dy==0): クライアントが押すのをやめた時等に、
		// 「目標座標=サーバー上の座標」のREQ_PUSHを送ってくることがある。
		// この場合は押せる物を動かさない(=そのまま)のが正しい状態なので、
		// 接触していなくても却下せず、移動量0で受理として扱う。
		result.bAccepted = true;
		result.nAcceptedDistance = 0;
		result.eReason = REASON_NONE;
		return result;
	}

	if (!CheckSpeed(dwElapsedMs, nPixelsPerSec, nRequestedDistance)) {
		result.eReason = REASON_SPEED_OVER;
		return result;
	}

	if (!CheckAxisOnly(nDir, dx, dy)) {
		result.eReason = REASON_AXIS_INVALID;
		return result;
	}

	// 接触確認は「押せる物の今の矩形」ではなく、要求された移動量(dx,dy)ぶん
	// ずらした「目標位置の矩形」に対して行う。クライアントは押せる物を先に
	// 目標位置まで動かして100ms毎に送ってくるため、サーバー上の現在位置には
	// 押した分めり込んでいても目標位置には接している、というのが通常状態。
	RECT_PX rcObjTarget = Offset(rcObj, dx, dy);
	if (!CheckContact(rcSelf, rcObjTarget, nDir, nContactGapAllowed)) {
		result.eReason = REASON_NOT_CONTACT;
		return result;
	}

	int nAccepted = ResolvePushDistance(rcObj, nDir, nRequestedDistance, isFree);

	result.bAccepted = (nAccepted > 0);
	result.nAcceptedDistance = nAccepted;
	result.eReason = REASON_NONE;
	return result;
}

bool CanStartSwap(bool bIsBall, const RECT_PX &rcObj, int nDir, const IsPositionFreeFunc &isFreeForObj)
{
	if (!bIsBall) {
		return false;
	}

	int dx, dy;
	DirVector(nDir, dx, dy);
	RECT_PX rcNext = Offset(rcObj, dx, dy);
	// ボールの1px先が塞がっていること(=進めない)が開始条件の一部
	return !(isFreeForObj && isFreeForObj(rcNext));
}

bool IsSwapRetreatPathClear(const RECT_PX &rcObj, int nDir, int nSelfWidthAlongDir, const IsPositionFreeFunc &isFreeForObj)
{
	if (nSelfWidthAlongDir <= 0) {
		return true;
	}

	int nRetreatDir = OppositeDir(nDir);
	int dx, dy;
	DirVector(nRetreatDir, dx, dy);

	RECT_PX rcCur = rcObj;
	for (int i = 0; i < nSelfWidthAlongDir; ++i) {
		RECT_PX rcNext = Offset(rcCur, dx, dy);
		if (!isFreeForObj || !isFreeForObj(rcNext)) {
			return false;
		}
		rcCur = rcNext;
	}
	return true;
}

SWAP_STATE StartSwap(unsigned int dwOwnerSessionID, const POINT_PX &ptP0, const POINT_PX &ptB0, int nDir)
{
	SWAP_STATE state;
	state.bActive = true;
	state.dwOwnerSessionID = dwOwnerSessionID;
	state.ptP0 = ptP0;
	state.ptB0 = ptB0;
	state.nDir = nDir;
	return state;
}

SWAP_UPDATE_RESULT UpdateSwap(
	const SWAP_STATE &state,
	unsigned int dwRequesterSessionID,
	const POINT_PX &ptSelfNow,
	int nDirNow,
	const POINT_PX &ptRequestedObj,
	const RECT_PX &rcObjAtB0,
	int nSelfWidthAlongDir,
	const IsPositionFreeFunc &isFreeForObj)
{
	SWAP_UPDATE_RESULT result;
	result.bAccepted = false;
	result.bShouldEnd = false;
	result.ptAcceptedObj = state.ptB0;
	result.eReason = REASON_NONE;

	if (!state.bActive) {
		return result;
	}

	if (dwRequesterSessionID != state.dwOwnerSessionID) {
		result.eReason = REASON_SWAP_NOT_OWNER;
		return result;
	}

	if (nDirNow != state.nDir) {
		// 向き変更を検出したら入れ替わりを終了する(bAccepted=falseのまま)。
		// 呼び出し側(RecvProcCHAR_REQ_PUSH)はこの組み合わせ(bShouldEnd=true &&
		// bAccepted=false)を見て、重なりが解けるまでボールを本人の2倍速で転がす
		// 「自走(eject)」を開始する(docs/push-object-redesign.md 2章7項)。
		// eject の1px単位の前進計算は ResolveEjectDistance() が受け持つ。
		result.bShouldEnd = true;
		return result;
	}

	int nAxisDx, nAxisDy;
	DirVector(state.nDir, nAxisDx, nAxisDy);

	// P0 からの本人の移動量(d軸成分)。本人は1フレームに数px進むため、入れ替わりが
	// 終わる要求では本人の移動量が本人の幅をわずかに超えることがある。クライアントは
	// ボールの下げ幅を本人の幅で打ち切って送ってくるため、サーバーも同じ打ち切りで
	// 判定する(min(移動量, 幅))。REASON_SWAP_TOO_FAR は、この打ち切り後の期待値とは
	// 別に、要求されたボール目標自体がB0から幅を超えて下がっている場合にのみ使う
	// (本人の移動量そのものが幅を大きく超えるものは、本人座標の妥当性・速度の検証で
	// 別途弾かれる前提)。
	int nMovedX = ptSelfNow.x - state.ptP0.x;
	int nMovedY = ptSelfNow.y - state.ptP0.y;
	int nMovedAxis = (nAxisDx != 0) ? nMovedX : nMovedY;

	// 要求されたボール目標がB0からd軸方向へどれだけ下がっているか(符号はnMovedAxisと同じ向き)。
	int nReqAxisDist = (nAxisDx != 0) ? (state.ptB0.x - ptRequestedObj.x) : (state.ptB0.y - ptRequestedObj.y);
	if (Abs(nReqAxisDist) > nSelfWidthAlongDir) {
		result.eReason = REASON_SWAP_TOO_FAR;
		return result;
	}

	// 本人の移動量を本人の幅で打ち切ったもの(符号はnMovedAxisのまま、大きさだけ制限)。
	int nMovedClamped = nMovedAxis;
	if (Abs(nMovedClamped) > nSelfWidthAlongDir) {
		nMovedClamped = (nMovedAxis > 0) ? nSelfWidthAlongDir : -nSelfWidthAlongDir;
	}

	// 目標: B0 - min(P - P0, 幅) の d 軸成分。横方向は B0 のまま不変。
	int nExpectedObjX = state.ptB0.x;
	int nExpectedObjY = state.ptB0.y;
	if (nAxisDx != 0) {
		nExpectedObjX = state.ptB0.x - nMovedClamped;
	} else {
		nExpectedObjY = state.ptB0.y - nMovedClamped;
	}

	if (ptRequestedObj.x != nExpectedObjX || ptRequestedObj.y != nExpectedObjY) {
		// 横方向不一致・軸成分不一致のどちらもここで弾く。
		// 横方向がズレているケースを区別して報告する。
		if ((nAxisDx != 0 && ptRequestedObj.y != nExpectedObjY) ||
			(nAxisDy != 0 && ptRequestedObj.x != nExpectedObjX)) {
			result.eReason = REASON_SWAP_LATERAL_CHANGE;
		} else {
			result.eReason = REASON_SWAP_TARGET_MISMATCH;
		}
		return result;
	}

	// 経路(本人からボールの現在位置までの1px)が空いているか。
	// ここでは「ボールを B0 から目標まで1pxずつ、-d方向へ動かせるか」を確認する。
	// 歩数は打ち切り後の移動量(nMovedClamped)を使う(幅を超える分は進めないため)。
	int nRetreatDir = OppositeDir(state.nDir);
	int rdx, rdy;
	DirVector(nRetreatDir, rdx, rdy);

	POINT_PX ptCur = state.ptB0;
	int nSteps = Abs(nMovedClamped);
	for (int i = 0; i < nSteps; ++i) {
		ptCur.x += rdx;
		ptCur.y += rdy;
		RECT_PX rcStep = Offset(rcObjAtB0, ptCur.x - state.ptB0.x, ptCur.y - state.ptB0.y);
		if (!isFreeForObj || !isFreeForObj(rcStep)) {
			result.bAccepted = false;
			result.ptAcceptedObj = ptCur;
			result.ptAcceptedObj.x -= rdx;
			result.ptAcceptedObj.y -= rdy;
			return result;
		}
	}

	// 終了条件: 本人が障害物にぶつかりきる(打ち切り後の移動量が最大値に達した)、
	// またはボールが本人の後ろへ抜けきった(同条件)場合、この更新で終了とする。
	// 本人の生の移動量が幅をわずかに超えた場合もnMovedClampedは幅で頭打ちに
	// なるため、ここで正しく終了扱いになる。
	if (Abs(nMovedClamped) >= nSelfWidthAlongDir) {
		result.bShouldEnd = true;
	}

	result.bAccepted = true;
	result.ptAcceptedObj = ptRequestedObj;
	result.eReason = REASON_NONE;
	return result;
}

int ResolveEjectDistance(
	const RECT_PX &rcObjStart,
	int nDir,
	int nMaxDistance,
	const RECT_PX &rcOwner,
	const IsPositionFreeFunc &isFree,
	bool &bSeparated)
{
	bSeparated = false;

	if (nMaxDistance <= 0) {
		return 0;
	}

	int dx, dy;
	DirVector(nDir, dx, dy);

	RECT_PX rcCur = rcObjStart;
	int nMoved = 0;

	for (; nMoved < nMaxDistance; ++nMoved) {
		RECT_PX rcNext = Offset(rcCur, dx, dy);
		if (!isFree || !isFree(rcNext)) {
			break; // 塞がっている。途中まで進めて終了(bSeparatedはfalseのまま)。
		}
		rcCur = rcNext;
		if (!RectsOverlap(rcCur, rcOwner)) {
			// 専有者と重ならなくなった。この1pxぶんも進めた上で自走終了。
			bSeparated = true;
			++nMoved;
			break;
		}
	}

	return nMoved;
}

}
