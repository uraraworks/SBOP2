/// @file AttackDecision.h
/// @brief 攻撃要求の可否判定(純粋関数) 定義ファイル
/// @copyright Copyright(C)URARA-works
///
/// docs/battle-redesign.md の S1。RecvProcBATTLE_REQ_ATACK() / AtackImple() から、
/// 「攻撃要求をどこまで受理するか」の判断だけを分離するためのファイル。
/// PushDecision.h と同じ方針で、ゲーム固有ヘッダ(Command.h/InfoCharBase.h 等)・
/// MFC・ロック・グローバル状態に一切依存しない。SboSvrTest から直接リンクしてテストできる。

#pragma once

namespace AttackDecision
{
	/// 攻撃間隔チェック: 通信の揺らぎ(2発が続けて届く程度)は許容しつつ、
	/// 連打・改造クライアントによる無制限連射を拒否する。
	///
	/// 受理条件は次のいずれか:
	/// - 前回受理が無い(dwLastAcceptedMs==0)。初回なので受理する。
	/// - 前回受理から dwMinIntervalMs 以上経過している。
	/// - 前々回受理から 2*dwMinIntervalMs 以上経過している
	///   (直近2間隔の平均が dwMinIntervalMs 以上なら可。通信の揺らぎで
	///   本来の間隔より早く2発届いても、その前の間隔が十分空いていれば許容する)。
	///
	/// 引き算はすべて unsigned int で行うため、GetTickMs() 等の tick 値が
	/// 一周(オーバーフロー)しても差分は正しく計算される。
	///
	/// @param dwNowMs 今回の受信時刻(ms)
	/// @param dwLastAcceptedMs 前回受理した時刻(ms)。無ければ0
	/// @param dwPrevAcceptedMs 前々回受理した時刻(ms)。無ければ0
	/// @param dwMinIntervalMs 最小攻撃間隔(ms)
	bool IsAttackIntervalOk(
		unsigned int dwNowMs,
		unsigned int dwLastAcceptedMs,
		unsigned int dwPrevAcceptedMs,
		unsigned int dwMinIntervalMs);

	/// PvP判定: 攻撃の主体(親付きなら親)がPCで、対象もPCで、そのマップがPvP不可なら
	/// 当てない(true=拒否)。
	/// PvP可否のマップ設定はまだ無いため、呼び出し側は bMapAllowsPvp に常に false を
	/// 渡すこと(後段でマップ設定を足したら true を渡せるようにする)。
	///
	/// @param bAttackerIsPC 攻撃の主体(親がいれば親)がPCか
	/// @param bTargetIsPC 対象がPCか
	/// @param bMapAllowsPvp 対象マップがPvP可か
	bool IsPvpAttackBlocked(bool bAttackerIsPC, bool bTargetIsPC, bool bMapAllowsPvp);
}
