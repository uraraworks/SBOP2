/// @file PacketITEM_ITEMWEAPONINFO.h
/// @brief コマンド(アイテム系:武器情報通知) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/08/11
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "PacketBase.h"
#include "LibInfoItemWeapon.h"

// クラス宣言

typedef class CPacketITEM_ITEMWEAPONINFO : public CPacketBase
{
public:
			CPacketITEM_ITEMWEAPONINFO();	// コンストラクタ
	virtual ~CPacketITEM_ITEMWEAPONINFO();	// デストラクタ

	void	Make(PCLibInfoItemWeapon pInfo);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	PCLibInfoItemWeapon	m_pLibInfo;	// 武器情報
} CPacketITEM_ITEMWEAPONINFO, *PCPacketITEM_ITEMWEAPONINFO;
