/// @file PacketITEM_RES_ITEMWEAPONINFO.h
/// @brief コマンド(アイテム系:武器情報応答) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/08/11
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "PacketBase.h"

class CInfoItemWeapon;

// クラス宣言

typedef class CPacketITEM_RES_ITEMWEAPONINFO : public CPacketBase
{
public:
			CPacketITEM_RES_ITEMWEAPONINFO();	// コンストラクタ
	virtual ~CPacketITEM_RES_ITEMWEAPONINFO();	// デストラクタ

	void	Make(CInfoItemWeapon *pInfo);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	CInfoItemWeapon	*m_pInfo;	// 武器情報
} CPacketITEM_RES_ITEMWEAPONINFO, *PCPacketITEM_RES_ITEMWEAPONINFO;
