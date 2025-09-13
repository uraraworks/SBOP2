/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketMSGCMD_MAKEITEM.h									 */
/* 内容			:コマンド(メッセージコマンド系:アイテム作成) 定義ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/10/20													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketMSGCMD_MAKEITEM : public CPacketBase
{
public:
			CPacketMSGCMD_MAKEITEM();					/* コンストラクタ */
	virtual ~CPacketMSGCMD_MAKEITEM();					/* デストラクタ */

	void	Make	(DWORD dwCharID, DWORD dwItemTypeID);		/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	DWORD	m_dwCharID,				/* キャラID */
			m_dwItemTypeID;			/* アイテム種別ID */
} CPacketMSGCMD_MAKEITEM, *PCPacketMSGCMD_MAKEITEM;

/* Copyright(C)URARA-works 2007 */
