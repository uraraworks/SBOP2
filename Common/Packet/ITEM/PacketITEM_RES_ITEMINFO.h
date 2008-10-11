/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketITEM_RES_ITEMINFO.h									 */
/* 内容			:コマンド(アイテム系:アイテム情報応答) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/05/05													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoItem;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketITEM_RES_ITEMINFO : public CPacketBase
{
public:
			CPacketITEM_RES_ITEMINFO();					/* コンストラクタ */
	virtual ~CPacketITEM_RES_ITEMINFO();				/* デストラクタ */

	void	Make	(CInfoItem *pInfo);							/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	CInfoItem	*m_pInfo;					/* アイテム情報 */
} CPacketITEM_RES_ITEMINFO, *PCPacketITEM_RES_ITEMINFO;

/* Copyright(C)URARA-works 2007 */
