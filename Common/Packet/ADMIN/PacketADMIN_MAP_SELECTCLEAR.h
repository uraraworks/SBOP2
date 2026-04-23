/// @file PacketADMIN_MAP_SELECTCLEAR.h
/// @brief コマンド(管理者系:マップ選択解除) 定義ファイル

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketADMIN_MAP_SELECTCLEAR : public CPacketBase
{
public:
			CPacketADMIN_MAP_SELECTCLEAR();	// コンストラクタ
	virtual ~CPacketADMIN_MAP_SELECTCLEAR();	// デストラクタ

	void	Make(void);			// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定（ペイロードなし）
} CPacketADMIN_MAP_SELECTCLEAR, *PCPacketADMIN_MAP_SELECTCLEAR;
