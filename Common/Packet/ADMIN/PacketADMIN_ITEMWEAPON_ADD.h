/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_ITEMWEAPON_ADD.h								 */
/* 内容			:コマンド(管理者系:武器情報追加) 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/08/11													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoItemWeapon;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketADMIN_ITEMWEAPON_ADD : public CPacketBase
{
public:
			CPacketADMIN_ITEMWEAPON_ADD();				/* コンストラクタ */
	virtual ~CPacketADMIN_ITEMWEAPON_ADD();				/* デストラクタ */

	void	Make	(CInfoItemWeapon *pInfo);					/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	CInfoItemWeapon		*m_pInfo;			/* 追加する武器情報 */
} CPacketADMIN_ITEMWEAPON_ADD, *PCPacketADMIN_ITEMWEAPON_ADD;

/* Copyright(C)URARA-works 2008 */
