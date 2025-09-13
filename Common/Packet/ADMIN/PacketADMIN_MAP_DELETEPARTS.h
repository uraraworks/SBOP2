/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_MAP_DELETEPARTS.h								 */
/* 内容			:コマンド(管理者系:マップパーツ削除) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/05/04													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketADMIN_MAP_DELETEPARTS : public CPacketBase
{
public:
			CPacketADMIN_MAP_DELETEPARTS();					/* コンストラクタ */
	virtual ~CPacketADMIN_MAP_DELETEPARTS();				/* デストラクタ */

	void	Make	(DWORD dwPartsID);								/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);								/* パケットを設定 */


public:
	DWORD	m_dwPartsID;			/* パーツID */
} CPacketADMIN_MAP_DELETEPARTS, *PCPacketADMIN_MAP_DELETEPARTS;

/* Copyright(C)URARA-works 2007 */
