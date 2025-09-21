/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_REQ_PUTGET.h									 */
/* 内容			:コマンド(キャラ系:アイテムを拾う置く要求) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/05/05													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketCHAR_REQ_PUTGET : public CPacketBase
{
public:
			CPacketCHAR_REQ_PUTGET();					/* コンストラクタ */
	virtual ~CPacketCHAR_REQ_PUTGET();					/* デストラクタ */

	void	Make	(DWORD dwCharID, DWORD dwItemID);			/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	DWORD	m_dwCharID,					/* キャラID */
			m_dwItemID;					/* アイテムID */
} CPacketCHAR_REQ_PUTGET, *PCPacketCHAR_REQ_PUTGET;

/* Copyright(C)URARA-works 2007 */
