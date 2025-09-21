/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_ITEM_COPY.h									 */
/* 内容			:コマンド(管理者系:アイテム情報コピー) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/22													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketADMIN_ITEM_COPY : public CPacketBase
{
public:
			CPacketADMIN_ITEM_COPY();						/* コンストラクタ */
	virtual ~CPacketADMIN_ITEM_COPY();						/* デストラクタ */

	void	Make	(DWORD dwItemID);								/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);								/* パケットを設定 */


public:
	DWORD	m_dwItemID;			/* アイテムID */
} CPacketADMIN_ITEM_COPY, *PCPacketADMIN_ITEM_COPY;

/* Copyright(C)URARA-works 2007 */
