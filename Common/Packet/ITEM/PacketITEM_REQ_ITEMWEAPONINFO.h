/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketITEM_REQ_ITEMWEAPONINFO.h							 */
/* 内容			:コマンド(アイテム系:武器情報要求) 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/08/11													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketITEM_REQ_ITEMWEAPONINFO : public CPacketBase
{
public:
			CPacketITEM_REQ_ITEMWEAPONINFO();			/* コンストラクタ */
	virtual ~CPacketITEM_REQ_ITEMWEAPONINFO();			/* デストラクタ */

	void	Make	(DWORD dwWeaponInfoID);						/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	DWORD m_dwWeaponInfoID;			/* 武器情報ID */
} CPacketITEM_REQ_ITEMWEAPONINFO, *PCPacketITEM_REQ_ITEMWEAPONINFO;

/* Copyright(C)URARA-works 2008 */
