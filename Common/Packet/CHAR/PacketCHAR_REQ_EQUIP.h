/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_REQ_EQUIP.h										 */
/* 内容			:コマンド(キャラ系:装備要求) 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/10/08													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketCHAR_REQ_EQUIP : public CPacketBase
{
public:
			CPacketCHAR_REQ_EQUIP();								/* コンストラクタ */
	virtual ~CPacketCHAR_REQ_EQUIP();								/* デストラクタ */

	void	Make	(DWORD dwCharID, DWORD dwItemID, int nUnsetType = -1);	/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);										/* パケットを設定 */


public:
	int		m_nUnsetType;			/* 装備解除する種別 */
	DWORD	m_dwCharID,				/* キャラID */
			m_dwItemID;				/* 装備するアイテムID */
} CPacketCHAR_REQ_EQUIP, *PCPacketCHAR_REQ_EQUIP;

/* Copyright(C)URARA-works 2007 */
