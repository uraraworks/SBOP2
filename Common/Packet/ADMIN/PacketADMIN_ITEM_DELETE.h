/// @file PacketADMIN_ITEM_DELETE.h
/// @brief コマンド(管理者系:アイテム情報削除) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/08/19
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketADMIN_ITEM_DELETE : public CPacketBase
{
public:
			CPacketADMIN_ITEM_DELETE();	// コンストラクタ
	virtual ~CPacketADMIN_ITEM_DELETE();	// デストラクタ

	void	Make(DWORD dwItemID);	// パケットを作成
	void	Make(PARRAYDWORD padwItemID);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	ARRAYDWORD	m_adwItemID;	// アイテムID
} CPacketADMIN_ITEM_DELETE, *PCPacketADMIN_ITEM_DELETE;
