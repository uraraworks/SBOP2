/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketITEM_REQ_ITEMTYPEINFO.h								 */
/* 内容			:コマンド(アイテム系:アイテム種別情報要求) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/10/03													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketITEM_REQ_ITEMTYPEINFO : public CPacketBase
{
public:
			CPacketITEM_REQ_ITEMTYPEINFO();				/* コンストラクタ */
	virtual ~CPacketITEM_REQ_ITEMTYPEINFO();			/* デストラクタ */

	void	Make	(DWORD dwTypeID);							/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	DWORD m_dwTypeID;				/* 種別ID */
} CPacketITEM_REQ_ITEMTYPEINFO, *PCPacketITEM_REQ_ITEMTYPEINFO;

/* Copyright(C)URARA-works 2007 */
