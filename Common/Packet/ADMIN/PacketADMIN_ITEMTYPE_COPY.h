/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_ITEMTYPE_COPY.h								 */
/* 内容			:コマンド(管理者系:アイテム種別情報コピー) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/10/01													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketADMIN_ITEMTYPE_COPY : public CPacketBase
{
public:
			CPacketADMIN_ITEMTYPE_COPY();					/* コンストラクタ */
	virtual ~CPacketADMIN_ITEMTYPE_COPY();					/* デストラクタ */

	void	Make	(DWORD m_dwTypeID);								/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);								/* パケットを設定 */


public:
	DWORD	m_dwTypeID;			/* 種別ID */
} CPacketADMIN_ITEMTYPE_COPY, *PCPacketADMIN_ITEMTYPE_COPY;

/* Copyright(C)URARA-works 2007 */
