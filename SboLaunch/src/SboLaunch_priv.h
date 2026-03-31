/// @file SboLaunch_priv.h
/// @brief SBOランチャー用定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/01/27
/// @copyright Copyright(C)URARA-works 2008

#pragma once

// 定数定義

// ゲーム状態
enum {
	GAMESTATE_NONE = 0,
	GAMESTATE_CONNECT,							// サーバーへ接続
	GAMESTATE_CONNECTMISS,						// サーバーへ接続失敗
	GAMESTATE_DISCONNECT,						// サーバーと切断
	GAMESTATE_GETFILELIST,						// ファイルリスト取得
	GAMESTATE_CHECKFILELIST,					// ファイルリストチェック
	GAMESTATE_GETFILE,							// ファイル取得
	GAMESTATE_MAKEFILELIST,						// ファイルリスト出力
	GAMESTATE_RUNUPDATE,						// アップデータ起動
	GAMESTATE_RUNSBOCLI,						// クライアント起動
	GAMESTATE_QUIT,								// 終了
};
