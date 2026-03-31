/// @file PacketADMIN_ITEM_COPY.h
/// @brief コマンド(管理者系:アイテム情報コピー) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/22
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketADMIN_ITEM_COPY : public CPacketBase
{
public:
			CPacketADMIN_ITEM_COPY();	// コンストラクタ
	virtual ~CPacketADMIN_ITEM_COPY();	// デストラクタ

	void	Make(DWORD dwItemID);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD	m_dwItemID;	// アイテムID
} CPacketADMIN_ITEM_COPY, *PCPacketADMIN_ITEM_COPY;
