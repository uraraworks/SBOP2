/// @file PacketITEM_ITEMTYPEINFO.h
/// @brief コマンド(アイテム系:アイテム種別情報通知) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/10/03
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"
#include "LibInfoItemType.h"

// クラス宣言

typedef class CPacketITEM_ITEMTYPEINFO : public CPacketBase
{
public:
			CPacketITEM_ITEMTYPEINFO();	// コンストラクタ
	virtual ~CPacketITEM_ITEMTYPEINFO();	// デストラクタ

	void	Make(PCLibInfoItemType pInfo);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	PCLibInfoItemType	m_pLibInfo;	// アイテム種別情報
} CPacketITEM_ITEMTYPEINFO, *PCPacketITEM_ITEMTYPEINFO;
