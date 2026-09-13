/// @file MoveStateDecision.h
/// @brief MOVE_START/MOVE_DIR_CHANGE/MOVE_STOP 受信時の MoveState 遷移先決定(純粋関数) 定義ファイル
/// @copyright Copyright(C)URARA-works
///
/// MainFrameRecvProcCHAR.cpp の RecvProcCHAR_MOVEPOS() から、
/// 「SetMoveState() に何を渡すか」の判断だけを分離するためのファイル。
/// ここに置いた関数はゲーム固有ヘッダ(Command.h/InfoCharBase.h 等)・
/// MFC・ロック等に一切依存しないため、SboSvrTest から直接リンクしてテストできる。
/// 呼び出し側の CHARMOVESTATE_* / SBOCOMMANDID_SUB_CHAR_MOVE_* は
/// そのまま int として渡すこと。

#pragma once

namespace MoveStateDecision
{
	/// SetMoveState() に渡す値が無いことを表す値
	const int NO_CHANGE = -1;

	/// MOVE_START/MOVE_DIR_CHANGE/MOVE_STOP 受信時に SetMoveState() へ渡すべき値を決める
	///
	/// Dead Reckoning 移行時の回帰で、位置が変わっていない(向きだけの変更)
	/// MOVE_START/MOVE_DIR_CHANGE でも無条件に「移動中」へ遷移させてしまい、
	/// 座り中(CHARMOVESTATE_SIT)等に方向キーを押しただけで立ち上がってしまう
	/// 不具合があった。位置が変わっていない場合は現在の状態を維持する。
	///
	/// 同様に MOVE_STOP も、実際には移動中でなかった(座り中に向きだけ変えた後に
	/// 届いた MOVE_STOP 等)場合まで無条件に nStopState(STAND/BATTLE等)へ
	/// 確定させてしまうと、座ったまま向きを変えただけで立たされてしまう。
	/// 実際に移動中だった時だけ nStopState へ確定させる。
	///
	/// @param bIsStopCmd true なら MOVE_STOP 受信。
	/// @param bIsCurrentlyMoving 受信時点でキャラが実際に移動中の状態
	///        (CHARMOVESTATE_MOVE / CHARMOVESTATE_BATTLEMOVE、呼び出し側の IsStateMove())
	///        だったか。bIsStopCmd==true の時だけ参照する。
	/// @param bPosChanged 受信した座標が現在座標と異なるか(false=向きだけの変更)
	/// @param nMoveStateOnMove bIsStopCmd==false かつ bPosChanged==true の時に設定する状態
	///        (呼び出し側で CHARMOVESTATE_MOVE / CHARMOVESTATE_BATTLEMOVE を判定して渡す)
	/// @param nStopState bIsStopCmd==true かつ bIsCurrentlyMoving==true の時に設定する状態
	/// @return SetMoveState() に渡すべき値。NO_CHANGE(-1) なら状態を変更しない
	int	Decide(bool bIsStopCmd, bool bIsCurrentlyMoving, bool bPosChanged, int nMoveStateOnMove, int nStopState);
}
