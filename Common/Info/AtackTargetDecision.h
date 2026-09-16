/// @file AtackTargetDecision.h
/// @brief 攻撃対象になり得る移動種別か判定(純粋関数) 定義ファイル
/// @copyright Copyright(C)URARA-works
///
/// docs/battle-redesign.md の S1(サーバーへホワイトリスト実装)→S3(Commonへ移動)。
/// CInfoCharBase::IsAtackTarget()(Common、サーバー・クライアント共用)と、
/// クライアントの敵検出(CStateProcMAP::IsEnemyChar、SboCli)の両方から使う。
/// SboSvr/src/MainFrame の AttackDecision.h/PushDecision.h/MoveStateDecision.h と
/// 同じ方針で、ゲーム固有ヘッダ(InfoCharBase.h等)・MFC・ロック・グローバル状態に
/// 一切依存しない。SboSvrTest から直接リンクしてテストできる。
/// 呼び出し側の CHARMOVETYPE_* はそのまま int として渡すこと
/// (Common/Info/InfoCharBase.h: PC=0, STAND=1, BALL=2, SCORE=3, STYLECOPY_PUT=4,
///  ATACKANIME=5, MOVEATACK=6, STYLECOPY_GET=7, PUTNPC=8, BATTLE1=9, BATTLE2=10)。

#pragma once

namespace AtackTargetDecision
{
	/// 移動種別が攻撃対象になり得るか判定する(PC/戦闘1/戦闘2/被弾アニメーションのみ真)。
	/// 容姿コピー(STYLECOPY_PUT/GET)は会話で動く仕掛けで対象外、矢等(MOVEATACK)も
	/// 対象外(親付きの攻撃側であり的にはしない)。HPやPvP可否はここでは判定しない
	/// (呼び出し側の責務。CInfoCharBase::IsAtackTarget()がHPを、
	/// AttackDecision::IsPvpAttackBlocked()がPvPを担当する)。
	///
	/// @param nMoveType CHARMOVETYPE_* の値
	bool IsAtackTargetMoveType(int nMoveType);
}
