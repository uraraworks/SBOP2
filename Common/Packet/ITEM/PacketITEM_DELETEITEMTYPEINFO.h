/// @file PacketITEM_DELETEITEMTYPEINFO.h
/// @brief コマンド(アイテム系:アイテム種別情報削除) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/10/03
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketITEM_DELETEITEMTYPEINFO : public CPacketBase
{
public:
			CPacketITEM_DELETEITEMTYPEINFO();	// コンストラクタ
	virtual ~CPacketITEM_DELETEITEMTYPEINFO();	// デストラクタ

	void	Make(DWORD dwItemID);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD	m_dwTypeID;	// 種別ID
} CPacketITEM_DELETEITEMTYPEINFO, *PCPacketITEM_DELETEITEMTYPEINFO;
