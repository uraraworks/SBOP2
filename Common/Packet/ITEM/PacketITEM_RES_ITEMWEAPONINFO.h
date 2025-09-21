/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketITEM_RES_ITEMWEAPONINFO.h							 */
/* 内容			:コマンド(アイテム系:武器情報応答) 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/08/11													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoItemWeapon;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketITEM_RES_ITEMWEAPONINFO : public CPacketBase
{
public:
			CPacketITEM_RES_ITEMWEAPONINFO();			/* コンストラクタ */
	virtual ~CPacketITEM_RES_ITEMWEAPONINFO();			/* デストラクタ */

	void	Make	(CInfoItemWeapon *pInfo);					/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	CInfoItemWeapon	*m_pInfo;					/* 武器情報 */
} CPacketITEM_RES_ITEMWEAPONINFO, *PCPacketITEM_RES_ITEMWEAPONINFO;

/* Copyright(C)URARA-works 2008 */
