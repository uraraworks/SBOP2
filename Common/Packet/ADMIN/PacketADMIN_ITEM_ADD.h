/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_ITEM_ADD.h										 */
/* 内容			:コマンド(管理者系:アイテム情報追加) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/08/16													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoItem;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketADMIN_ITEM_ADD : public CPacketBase
{
public:
			CPacketADMIN_ITEM_ADD();				/* コンストラクタ */
	virtual ~CPacketADMIN_ITEM_ADD();				/* デストラクタ */

	void	Make	(CInfoItem *pInfo);						/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);						/* パケットを設定 */


public:
	CInfoItem	*m_pInfoItem;				/* 追加するアイテム情報 */
} CPacketADMIN_ITEM_ADD, *PCPacketADMIN_ITEM_ADD;

/* Copyright(C)URARA-works 2007 */
