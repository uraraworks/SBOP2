/// @file PacketCONNECT_RES_PLAY.h
/// @brief コマンド(接続系:ゲーム開始応答) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/12/31
/// @copyright Copyright(C)URARA-works 2006

#pragma once

#include "PacketBase.h"

// 定数の定義

enum {
	PLAYRES_NONE = 0,
	PLAYRES_OK,	// 問題無し
	PLAYRES_ADMINLEVEL_ALL,	// 管理者(全権限)
};

// クラス宣言

typedef class CPacketCONNECT_RES_PLAY : public CPacketBase
{
public:
			CPacketCONNECT_RES_PLAY();	// コンストラクタ
	virtual ~CPacketCONNECT_RES_PLAY();	// デストラクタ

	void	Make(int nResult);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	int	m_nResult;	// 結果
} CPacketCONNECT_RES_PLAY, *PCPacketCONNECT_RES_PLAY;
