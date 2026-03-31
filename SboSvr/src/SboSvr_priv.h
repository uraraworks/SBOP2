/// @file SboSvr_priv.h
/// @brief サーバー用定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/04
/// @copyright Copyright(C)URARA-works 2006

#pragma once

// 定数定義

// ウィンドウメッセージ
enum {
	WM_INITEND = WM_APP,	// 初期化完了
	WM_RECVCOMMAND,	// コマンド受信
	WM_DISCONNECT,	// 切断
};

// 構造体定義
