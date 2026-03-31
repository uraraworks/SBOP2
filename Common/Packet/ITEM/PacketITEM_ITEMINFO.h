/// @file PacketITEM_ITEMINFO.h
/// @brief コマンド(アイテム系:アイテム情報通知) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/05/05
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"
#include "LibInfo/LibInfoItem.h"

// クラス宣言

typedef class CPacketITEM_ITEMINFO : public CPacketBase
{
public:
			CPacketITEM_ITEMINFO();	// コンストラクタ
	virtual ~CPacketITEM_ITEMINFO();	// デストラクタ

	void	Make(PCLibInfoItem pInfo);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	PCLibInfoItem	m_pLibInfo;	// アイテム情報
} CPacketITEM_ITEMINFO, *PCPacketITEM_ITEMINFO;
