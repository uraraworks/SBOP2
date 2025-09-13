/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_ITEMWEAPON_RENEW.h								 */
/* 内容			:コマンド(アイテム系:武器情報更新) 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/08/11													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoItemWeapon;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketADMIN_ITEMWEAPON_RENEW : public CPacketBase
{
public:
			CPacketADMIN_ITEMWEAPON_RENEW();			/* コンストラクタ */
	virtual ~CPacketADMIN_ITEMWEAPON_RENEW();			/* デストラクタ */

	void	Make	(CInfoItemWeapon *pInfo);					/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	CInfoItemWeapon	*m_pInfo;					/* 武器情報 */
} CPacketADMIN_ITEMWEAPON_RENEW, *PCPacketADMIN_ITEMWEAPON_RENEW;

/* Copyright(C)URARA-works 2007 */
