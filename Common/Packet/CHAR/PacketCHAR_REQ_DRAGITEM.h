/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_REQ_DRAGITEM.h									 */
/* 内容			:コマンド(キャラ系:アイテム位置変更要求) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/11/22													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketCHAR_REQ_DRAGITEM : public CPacketBase
{
public:
			CPacketCHAR_REQ_DRAGITEM();							/* コンストラクタ */
	virtual ~CPacketCHAR_REQ_DRAGITEM();						/* デストラクタ */

	void	Make	(DWORD dwCharID, DWORD dwItemID, POINT ptNewPos);	/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);									/* パケットを設定 */


public:
	DWORD	m_dwCharID,					/* キャラID */
			m_dwItemID;					/* アイテムID */
	POINT	m_ptNewPos;					/* 新しい位置 */
} CPacketCHAR_REQ_DRAGITEM, *PCPacketCHAR_REQ_DRAGITEM;

/* Copyright(C)URARA-works 2008 */
