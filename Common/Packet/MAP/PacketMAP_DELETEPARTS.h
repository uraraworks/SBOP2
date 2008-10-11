/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketMAP_DELETEPARTS.h									 */
/* 内容			:コマンド(マップ系:マップパーツ削除) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/05/04													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketMAP_DELETEPARTS : public CPacketBase
{
public:
			CPacketMAP_DELETEPARTS();						/* コンストラクタ */
	virtual ~CPacketMAP_DELETEPARTS();						/* デストラクタ */

	void	Make	(DWORD dwPartsID);								/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);								/* パケットを設定 */


public:
	DWORD	m_dwPartsID;			/* パーツID */
} CPacketMAP_DELETEPARTS, *PCPacketMAP_DELETEPARTS;

/* Copyright(C)URARA-works 2007 */
