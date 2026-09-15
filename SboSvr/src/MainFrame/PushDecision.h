/// @file PushDecision.h
/// @brief 押せる物(Push=1 NPC)への押し要求の可否判定(純粋関数) 定義ファイル
/// @copyright Copyright(C)URARA-works
///
/// docs/push-object-redesign.md の S2。RecvProcCHAR_REQ_PUSH() 等から、
/// 「押し要求をどこまで受理するか」の判断だけを分離するためのファイル。
/// MoveStateDecision.h と同じ方針で、ゲーム固有ヘッダ(Command.h/InfoCharBase.h 等)・
/// MFC・ロック・グローバル状態に一切依存しない。マップ当たりや他キャラとの衝突判定は
/// 呼び出し側からコールバックで注入する。SboSvrTest から直接リンクしてテストできる。

#pragma once

#include <functional>

namespace PushDecision
{
	/// 向き。上下左右4方向。CHARDIR_* とインデックスが一致する前提で
	/// 呼び出し側からそのまま int として渡すこと。
	enum DIRECTION
	{
		DIR_UP    = 0,
		DIR_DOWN  = 1,
		DIR_LEFT  = 2,
		DIR_RIGHT = 3,
	};

	/// 矩形(ピクセル座標)。CRect 等 MFC 型に依存しないための最小定義。
	struct RECT_PX
	{
		int nLeft;
		int nTop;
		int nRight;
		int nBottom;
	};

	/// 座標(ピクセル)
	struct POINT_PX
	{
		int x;
		int y;
	};

	/// 却下理由(NG時のログ・デバッグ用。ACCEPTED でも一部理由は使わない)
	enum REASON
	{
		REASON_NONE = 0,
		REASON_SPEED_OVER,          // 経過時間に対して距離が出過ぎている
		REASON_AXIS_INVALID,        // 向きの軸以外の移動を含む
		REASON_SELF_POS_MISMATCH,   // 申告座標がサーバー座標から許容以上ズレている
		REASON_NOT_CONTACT,         // 押せる物に接していない
		REASON_SWAP_NOT_OWNER,      // 入れ替わり中の押せる物を専有者以外が押そうとした
		REASON_SWAP_TARGET_MISMATCH,// 入れ替わり中のボール目標が B0-(P-P0) と不一致
		REASON_SWAP_LATERAL_CHANGE, // 入れ替わり中に横方向が変化した
		REASON_SWAP_TOO_FAR,        // 入れ替わり中の合計移動量が本人の幅を超えた
	};

	/// 1px移動した時にそこへ進めるか(true=空いている)を返すコールバック。
	/// pMoveRect は「その位置に矩形を置いた時の当たり矩形」。
	/// マップ当たり・本人(押している人)以外の全キャラとの衝突を見て判定する。
	/// 押す本人の判定と、押せる物自身の判定とで別の関数を渡すこと
	/// (「本人以外」「押せる物以外」で除外対象が異なるため)。
	typedef std::function<bool(const RECT_PX &rcMoveTo)> IsPositionFreeFunc;

	/// クライアント時刻の逆行チェック結果。
	struct CLIENT_TIME_RESULT
	{
		bool bRejected;        // true: 時刻逆行として却下すべき(受理せず黙って/座標だけ返す)
		bool bTreatedAsReload; // true: 大きな逆行を検知し、前回受理なし扱いに基準をリセットした
		unsigned int dwElapsedMs; // 却下されなかった場合に速度検証へ渡す経過時間(上限クランプ済み)
	};

	/// クライアントの自己申告時刻(dwTimeStamp)を、前回受理したクライアント時刻
	/// (dwLastAcceptedClientTime)と比較し、経過時間・却下要否を判定する。
	///
	/// - dwTimeStamp==0、または前回受理が無い(dwLastAcceptedClientTime==0)場合は
	///   比較対象が無いため却下せず、経過時間を上限値(dwElapsedMaxMs)扱いにする
	///   (最初の1回を0ms扱いにすると許容距離が無くなり毎回却下されてしまうため)。
	/// - 前回より小さい(時刻逆行)場合:
	///   - 逆行幅が dwReloadThresholdMs 以上なら、クライアント(SDL_GetTicks系)が
	///     ページ再読み込み等で0から数え直したものとみなし、却下せず「前回受理なし」
	///     として基準を取り直す(bTreatedAsReload=true)。呼び出し側は
	///     前回受理したクライアント時刻を0に戻すこと。
	///   - それ未満の小さな逆行(パケットの入れ替わり・重複)は従来どおり却下する
	///     (bRejected=true)。
	/// - それ以外(前回以上に進んでいる)は、実測の差を dwElapsedMaxMs でクランプして返す。
	///
	/// @param dwTimeStamp 今回のクライアント自己申告時刻(ms)
	/// @param dwLastAcceptedClientTime 前回受理したクライアント時刻(ms)。無ければ0
	/// @param dwReloadThresholdMs これ以上の逆行は「読み込み直し」とみなす閾値(ms)
	/// @param dwElapsedMaxMs 経過時間の上限(ms)
	CLIENT_TIME_RESULT CheckClientTime(
		unsigned int dwTimeStamp,
		unsigned int dwLastAcceptedClientTime,
		unsigned int dwReloadThresholdMs,
		unsigned int dwElapsedMaxMs);

	/// 速度検証: 経過時間に対して距離が出過ぎていないか。
	/// 許容距離 = 経過ms/1000 * nPixelsPerSec * 1.1 + 8px
	///
	/// @param dwElapsedMs 前回受理時点からの経過時間(ms)
	/// @param nPixelsPerSec 押している人の移動速度(px/s)
	/// @param nRequestedDistance 今回要求された移動距離(px, 絶対値)
	bool CheckSpeed(unsigned int dwElapsedMs, int nPixelsPerSec, int nRequestedDistance);

	/// 向きの軸検証: 向き d の軸以外の移動を含んでいないか。
	/// dx,dy は要求された移動量(そのままの符号)。
	bool CheckAxisOnly(int nDir, int dx, int dy);

	/// 本人座標の妥当性検証: 申告座標がサーバー座標から許容(nTolerance px)以上
	/// ズレていないか。
	bool CheckSelfPosValid(const POINT_PX &ptReportedSelf, const POINT_PX &ptServerSelf, int nTolerance);

	/// 接触検証: 本人の当たり矩形が、押せる物の(向き d と反対側の面)に
	/// -nGapAllowed 〜 +nGapAllowed px の範囲で接しているか
	/// (すき間だけでなく、わずかなめり込みも接触として許容する)。
	/// 押す向きと直角の軸(縦に押すなら横方向、横に押すなら縦方向)についても、
	/// 重なっていることまでは要求せず、同じく nGapAllowed px までのすき間なら
	/// 接触として許容する(斜め移動で押した際、直角の軸がわずかに離れた瞬間の
	/// 最後の押し要求が却下されて押し戻しに見える回帰の対策)。
	/// rcObj には「押せる物の今の矩形」ではなく、呼び出し側が用意した
	/// 「判定したい位置(通常は目標位置)の矩形」を渡すこと。
	bool CheckContact(const RECT_PX &rcSelf, const RECT_PX &rcObj, int nDir, int nGapAllowed);

	/// 通常の押し: 押せる物を向き nDir へ最大 nRequestedDistance px、1pxずつ
	/// IsPositionFreeFunc で確認しながら進め、空いている所まで進めた距離(0以上、
	/// nRequestedDistance 以下)を返す(=途中まで受理)。
	///
	/// @param rcObjStart 押せる物の現在の当たり矩形
	/// @param nDir 押す向き(押している人の向き)
	/// @param nRequestedDistance 要求された移動距離(px, 0以上)
	/// @param isFree 1px先が空いているかを判定するコールバック
	/// @return 実際に進められた距離(px)
	int ResolvePushDistance(const RECT_PX &rcObjStart, int nDir, int nRequestedDistance, const IsPositionFreeFunc &isFree);

	/// 押し要求全体の判定結果
	struct PUSH_RESULT
	{
		bool bAccepted;       // 1px以上受理できたか(速度/軸/接触/本人座標のいずれかで
		                      // 却下された場合は false, distance=0)
		int  nAcceptedDistance; // 実際に受理して進める距離(px, 0以上)
		REASON eReason;       // bAccepted==false の時の却下理由(先頭で弾かれた理由)
	};

	/// 通常の押し要求をまとめて判定する。
	/// まず本人座標(偽装チェック)を確認し、要求距離(dx,dyの絶対値の和)が0なら、
	/// 押せる物を動かさない要求として、接触していなくても却下せず
	/// bAccepted=true, nAcceptedDistance=0 で返す(クライアントが押すのをやめた時等に
	/// 目標座標=サーバー座標のREQ_PUSHを送ってくるケースの対応)。
	/// 要求距離が0でなければ、速度→軸→接触の順にチェックし、いずれかがNGなら
	/// その場で却下(distance=0)。全て通れば ResolvePushDistance() で途中まで
	/// 受理する距離を求める。
	/// 接触確認は rcObj そのものではなく、rcObj を (dx,dy) だけずらした
	/// 「目標位置の矩形」に対して行う(クライアントは押せる物を先に目標位置まで
	/// 動かして送ってくるため、サーバー上の現在位置には押した分めり込んでいても
	/// 目標位置には接している、というのが通常状態のため)。
	///
	/// @param rcSelf 押している人の当たり矩形(申告座標基準)
	/// @param ptReportedSelf 申告された本人座標
	/// @param ptServerSelf サーバーが把握している本人座標
	/// @param nSelfPosTolerance 本人座標許容量(px)
	/// @param rcObj 押せる物の現在の当たり矩形
	/// @param nDir 押す向き
	/// @param dx,dy 要求された移動量(押せる物側。d軸以外を含んでいたら却下。
	///        接触確認では rcObj をこの分ずらした目標位置矩形を使う)
	/// @param dwElapsedMs 前回受理からの経過時間(ms)
	/// @param nPixelsPerSec 押している人の移動速度(px/s)
	/// @param nContactGapAllowed 接触とみなすすき間・めり込みの許容(px)
	/// @param isFree 1px先が空いているかを判定するコールバック
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
		const IsPositionFreeFunc &isFree);

	/// 入れ替わり(SWAP)状態。開始時に記録し、以後の要求で検証に使う。
	/// セッションIDの型は呼び出し側(DWORD等)に合わせて unsigned int で持つ。
	struct SWAP_STATE
	{
		bool         bActive;           // 入れ替わり中か
		unsigned int dwOwnerSessionID;  // 専有している本人のセッションID(改造クライアント対策で照合用)
		POINT_PX     ptP0;              // 開始時の本人位置
		POINT_PX     ptB0;              // 開始時のボール位置
		int          nDir;              // 開始時の(本人の)向き
	};

	/// 入れ替わり開始条件を満たすか判定する。
	/// 「押せる物が BALL であり、ボールの1px先(向きnDir)が障害物で塞がっていて、
	/// 本人が接したまま同じ向きへさらに進もうとしている」を満たすか。
	/// 岩・箱(BALL以外)は壁の前で止まるだけで入れ替わらない(仕様書2章7項)ため、
	/// bIsBall==false なら常に false を返す。
	///
	/// @param bIsBall 押せる物の種別が CHARMOVETYPE_BALL か
	/// @param rcObj ボールの現在の当たり矩形
	/// @param nDir 本人の向き(押す向き)
	/// @param isFreeForObj ボールを1px先に進められるか(本人以外に対して)
	bool CanStartSwap(bool bIsBall, const RECT_PX &rcObj, int nDir, const IsPositionFreeFunc &isFreeForObj);

	/// 入れ替わり開始前に、ボールの逆向き(-nDir)の経路(本人の幅ぶん)が
	/// 1pxずつ、本人以外に対して空いているか確認する。
	///
	/// @param rcObj ボールの現在の当たり矩形
	/// @param nDir 本人の向き(逆向き -nDir へボールが下がっていく)
	/// @param nSelfWidthAlongDir 本人の(向きの軸方向の)幅。この距離ぶん確認する。
	/// @param isFreeForObj ボールをそこに進められるか(本人以外に対して)
	bool IsSwapRetreatPathClear(const RECT_PX &rcObj, int nDir, int nSelfWidthAlongDir, const IsPositionFreeFunc &isFreeForObj);

	/// 入れ替わりを開始する。SWAP_STATE を初期化して返す。
	SWAP_STATE StartSwap(unsigned int dwOwnerSessionID, const POINT_PX &ptP0, const POINT_PX &ptB0, int nDir);

	/// 入れ替わり中の要求を検証する。
	/// - 専有者以外からの要求は却下(REASON_SWAP_NOT_OWNER)
	/// - 向きが開始時と変わっていれば「終了」を返す(bShouldEnd=true, bAccepted=false)。
	///   呼び出し側(RecvProcCHAR_REQ_PUSH)はこの組み合わせを見て、重なりが解けるまで
	///   ボールを本人の2倍速で転がす「自走(eject)」を開始する
	///   (docs/push-object-redesign.md 2章7項)。eject の1px単位の前進計算は
	///   ResolveEjectDistance() が受け持つ。
	/// - ボール目標(要求されたボール座標)が B0-min(P-P0, 幅) の nDir 軸成分と一致し、
	///   横方向が不変で、経路(isFreeForObj)が空いていることを確認する。本人は
	///   1フレームに数px進むため、入れ替わりが終わる要求ではP0からの移動量が本人の幅
	///   をわずかに超えることがある。クライアントはボール目標をB0から幅で打ち切って
	///   送ってくるため、サーバーも同じ打ち切り(min(移動量, 幅))で期待値を計算する。
	///   打ち切り後の移動量が幅に達したら受理してbShouldEnd=trueにする。
	///   REASON_SWAP_TOO_FAR は、これとは別に、要求されたボール目標自体がB0から
	///   幅を超えて下がっている場合にのみ返す(本人の移動量そのものが幅を大きく
	///   超えるものは、本人座標の妥当性・速度の検証で別途弾かれる前提)。
	struct SWAP_UPDATE_RESULT
	{
		bool    bAccepted;
		bool    bShouldEnd;    // true なら SWAP_STATE.bActive を false にすべき
		         // (向き変更検出、または本人がぶつかりきった/ボールが抜けきった)
		POINT_PX ptAcceptedObj; // 受理する場合のボール座標
		REASON  eReason;
	};

	/// @param rcObjAtB0 開始時(B0)のボールの当たり矩形。経路確認の際、この
	///        矩形サイズをB0からの相対位置に当てはめて isFreeForObj へ渡す。

	SWAP_UPDATE_RESULT UpdateSwap(
		const SWAP_STATE &state,
		unsigned int dwRequesterSessionID,
		const POINT_PX &ptSelfNow,
		int nDirNow,
		const POINT_PX &ptRequestedObj,
		const RECT_PX &rcObjAtB0,
		int nSelfWidthAlongDir,
		const IsPositionFreeFunc &isFreeForObj);

	/// 自走(eject): 入れ替わり終了(向き変更)後、専有者と重なったままのボールを
	/// nDir へ最大 nMaxDistance px、1pxずつ isFree で確認しながら進める。
	/// 専有者の当たり矩形(rcOwner)と重ならなくなった時点で、その1pxぶんも含めて
	/// 進めた距離を返し、bSeparated=true にする(=自走終了)。途中で塞がれれば
	/// そこまでの距離を返す(bSeparatedはfalseのまま。自走状態は呼び出し側が
	/// 維持し、次回また同じ位置から再試行できるようにする)。
	///
	/// @param rcObjStart 押せる物(ボール)の現在の当たり矩形
	/// @param nDir 自走する向き(入れ替わり終了時に検出した新しい向き)
	/// @param nMaxDistance 今回進められる最大距離(px, 0以上)
	/// @param rcOwner 専有者(本人)の当たり矩形。これと重ならなくなったら止める
	/// @param isFree 1px先が空いているかを判定するコールバック(専有者以外に対して)
	/// @param bSeparated [out] 専有者の矩形と重ならなくなって自走が終了したか
	int ResolveEjectDistance(
		const RECT_PX &rcObjStart,
		int nDir,
		int nMaxDistance,
		const RECT_PX &rcOwner,
		const IsPositionFreeFunc &isFree,
		bool &bSeparated);
}
