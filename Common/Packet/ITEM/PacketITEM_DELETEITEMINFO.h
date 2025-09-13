/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketITEM_DELETEITEMINFO.h								 */
/* 内容			:コマンド(アイテム系:アイテム情報削除) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/05/05													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketITEM_DELETEITEMINFO : public CPacketBase
{
public:
			CPacketITEM_DELETEITEMINFO();					/* コンストラクタ */
	virtual ~CPacketITEM_DELETEITEMINFO();					/* デストラクタ */

	void	Make	(DWORD dwItemID);								/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);								/* パケットを設定 */


public:
	DWORD	m_dwItemID;			/* アイテムID */
} CPacketITEM_DELETEITEMINFO, *PCPacketITEM_DELETEITEMINFO;

/* Copyright(C)URARA-works 2007 */
