/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketITEM_REQ_ITEMINFO.h									 */
/* 内容			:コマンド(アイテム系:アイテム情報要求) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/08/16													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketITEM_REQ_ITEMINFO : public CPacketBase
{
public:
			CPacketITEM_REQ_ITEMINFO();					/* コンストラクタ */
	virtual ~CPacketITEM_REQ_ITEMINFO();				/* デストラクタ */

	void	Make	(DWORD dwItemID);							/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	DWORD m_dwItemID;				/* アイテムID */
} CPacketITEM_REQ_ITEMINFO, *PCPacketITEM_REQ_ITEMINFO;

/* Copyright(C)URARA-works 2007 */
