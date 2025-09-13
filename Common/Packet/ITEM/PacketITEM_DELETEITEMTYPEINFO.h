/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketITEM_DELETEITEMTYPEINFO.h							 */
/* 内容			:コマンド(アイテム系:アイテム種別情報削除) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/10/03													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketITEM_DELETEITEMTYPEINFO : public CPacketBase
{
public:
			CPacketITEM_DELETEITEMTYPEINFO();				/* コンストラクタ */
	virtual ~CPacketITEM_DELETEITEMTYPEINFO();				/* デストラクタ */

	void	Make	(DWORD dwItemID);								/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);								/* パケットを設定 */


public:
	DWORD	m_dwTypeID;			/* 種別ID */
} CPacketITEM_DELETEITEMTYPEINFO, *PCPacketITEM_DELETEITEMTYPEINFO;

/* Copyright(C)URARA-works 2007 */
