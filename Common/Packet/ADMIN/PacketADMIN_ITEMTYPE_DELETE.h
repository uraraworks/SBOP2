/// @file PacketADMIN_ITEMTYPE_DELETE.h
/// @brief コマンド(管理者系:アイテム種別情報削除) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/10/01
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketADMIN_ITEMTYPE_DELETE : public CPacketBase
{
public:
			CPacketADMIN_ITEMTYPE_DELETE();	// コンストラクタ
	virtual ~CPacketADMIN_ITEMTYPE_DELETE();	// デストラクタ

	void	Make(DWORD dwTypeID);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD	m_dwTypeID;	// 種別ID
} CPacketADMIN_ITEMTYPE_DELETE, *PCPacketADMIN_ITEMTYPE_DELETE;
