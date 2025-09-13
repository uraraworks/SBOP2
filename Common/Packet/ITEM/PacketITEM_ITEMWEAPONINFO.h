/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketITEM_ITEMWEAPONINFO.h								 */
/* 内容			:コマンド(アイテム系:武器情報通知) 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/08/11													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"
#include "LibInfoItemWeapon.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketITEM_ITEMWEAPONINFO : public CPacketBase
{
public:
			CPacketITEM_ITEMWEAPONINFO();			/* コンストラクタ */
	virtual ~CPacketITEM_ITEMWEAPONINFO();			/* デストラクタ */

	void	Make	(PCLibInfoItemWeapon pInfo);			/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);						/* パケットを設定 */


public:
	PCLibInfoItemWeapon	m_pLibInfo;				/* 武器情報 */
} CPacketITEM_ITEMWEAPONINFO, *PCPacketITEM_ITEMWEAPONINFO;

/* Copyright(C)URARA-works 2008 */
