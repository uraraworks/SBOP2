/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_REQ_USEITEM.h									 */
/* 内容			:コマンド(キャラ系:アイテムを使う要求) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/07/05													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketCHAR_REQ_USEITEM : public CPacketBase
{
public:
			CPacketCHAR_REQ_USEITEM();					/* コンストラクタ */
	virtual ~CPacketCHAR_REQ_USEITEM();					/* デストラクタ */

	void	Make	(DWORD dwCharID, DWORD dwItemID);			/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	DWORD	m_dwCharID,					/* キャラID */
			m_dwItemID;					/* アイテムID */
} CPacketCHAR_REQ_USEITEM, *PCPacketCHAR_REQ_USEITEM;

/* Copyright(C)URARA-works 2008 */
